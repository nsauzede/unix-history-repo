/*-
 * Copyright (c) 2004 Pawel Jakub Dawidek <pjd@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <errno.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <libgeom.h>
#include <geom/mirror/g_mirror.h>
#include <core/geom.h>
#include <misc/subr.h>


uint32_t lib_version = G_LIB_VERSION;
uint32_t version = G_MIRROR_VERSION;

static char label_balance[] = "split", configure_balance[] = "none";
static intmax_t label_slice = 4096, configure_slice = -1;
static intmax_t insert_priority = 0;

static void mirror_main(struct gctl_req *req, unsigned flags);
static void mirror_activate(struct gctl_req *req);
static void mirror_clear(struct gctl_req *req);
static void mirror_dump(struct gctl_req *req);
static void mirror_label(struct gctl_req *req);

struct g_command class_commands[] = {
	{ "activate", G_FLAG_VERBOSE, mirror_main, G_NULL_OPTS },
	{ "clear", G_FLAG_VERBOSE, mirror_main, G_NULL_OPTS },
	{ "configure", G_FLAG_VERBOSE, NULL,
	    {
		{ 'a', "autosync", NULL, G_TYPE_NONE },
		{ 'b', "balance", configure_balance, G_TYPE_STRING },
		{ 'd', "dynamic", NULL, G_TYPE_NONE },
		{ 'h', "hardcode", NULL, G_TYPE_NONE },
		{ 'n', "noautosync", NULL, G_TYPE_NONE },
		{ 's', "slice", &configure_slice, G_TYPE_NUMBER },
		G_OPT_SENTINEL
	    }
	},
	{ "deactivate", G_FLAG_VERBOSE, NULL, G_NULL_OPTS },
	{ "dump", 0, mirror_main, G_NULL_OPTS },
	{ "forget", G_FLAG_VERBOSE, NULL, G_NULL_OPTS },
	{ "label", G_FLAG_VERBOSE, mirror_main,
	    {
		{ 'b', "balance", label_balance, G_TYPE_STRING },
		{ 'h', "hardcode", NULL, G_TYPE_NONE },
		{ 'n', "noautosync", NULL, G_TYPE_NONE },
		{ 's', "slice", &label_slice, G_TYPE_NUMBER },
		G_OPT_SENTINEL
	    }
	},
	{ "insert", G_FLAG_VERBOSE, NULL,
	    {
		{ 'h', "hardcode", NULL, G_TYPE_NONE },
		{ 'i', "inactive", NULL, G_TYPE_NONE },
		{ 'p', "priority", &insert_priority, G_TYPE_NUMBER },
		G_OPT_SENTINEL
	    }
	},
	{ "rebuild", G_FLAG_VERBOSE, NULL, G_NULL_OPTS },
	{ "remove", G_FLAG_VERBOSE, NULL, G_NULL_OPTS },
	{ "stop", G_FLAG_VERBOSE, NULL,
	    {
		{ 'f', "force", NULL, G_TYPE_NONE },
		G_OPT_SENTINEL
	    }
	},
	G_CMD_SENTINEL
};

static int verbose = 0;

void usage(const char *);
void
usage(const char *comm)
{
	fprintf(stderr,
	    "usage: %s label [-hnv] [-b balance] [-s slice] name prov [prov [...]]\n"
	    "       %s clear [-v] prov [prov [...]]\n"
	    "       %s dump prov [prov [...]]\n"
	    "       %s configure [-adhnv] [-b balance] [-s slice] name\n"
	    "       %s rebuild [-v] name prov [prov [...]]\n"
	    "       %s insert [-hiv] [-p priority] name prov [prov [...]]\n"
	    "       %s remove [-v] name prov [prov [...]]\n"
	    "       %s activate [-v] name prov [prov [...]]\n"
	    "       %s deactivate [-v] name prov [prov [...]]\n"
	    "       %s forget name [name [...]]\n"
	    "       %s stop [-fv] name [...]\n",
	    comm, comm, comm, comm, comm, comm, comm, comm, comm, comm, comm);
}

static void
mirror_main(struct gctl_req *req, unsigned flags)
{
	const char *name;

	if ((flags & G_FLAG_VERBOSE) != 0)
		verbose = 1;

	name = gctl_get_asciiparam(req, "verb");
	if (name == NULL) {
		gctl_error(req, "No '%s' argument.", "verb");
		return;
	}
	if (strcmp(name, "label") == 0)
		mirror_label(req);
	else if (strcmp(name, "clear") == 0)
		mirror_clear(req);
	else if (strcmp(name, "dump") == 0)
		mirror_dump(req);
	else if (strcmp(name, "activate") == 0)
		mirror_activate(req);
	else
		gctl_error(req, "Unknown command: %s.", name);
}

static void
mirror_label(struct gctl_req *req)
{
	struct g_mirror_metadata md;
	u_char sector[512];
	const char *str;
	char param[16];
	int *hardcode, *nargs, *noautosync, bal, error, i;
	unsigned sectorsize;
	off_t mediasize;
	intmax_t *valp;

	nargs = gctl_get_paraml(req, "nargs", sizeof(*nargs));
	if (nargs == NULL) {
		gctl_error(req, "No '%s' argument.", "nargs");
		return;
	}
	if (*nargs < 2) {
		gctl_error(req, "Too few arguments.");
		return;
	}

	strlcpy(md.md_magic, G_MIRROR_MAGIC, sizeof(md.md_magic));
	md.md_version = G_MIRROR_VERSION;
	str = gctl_get_asciiparam(req, "arg0");
	if (str == NULL) {
		gctl_error(req, "No 'arg%u' argument.", 0);
		return;
	}
	strlcpy(md.md_name, str, sizeof(md.md_name));
	md.md_mid = arc4random();
	md.md_all = *nargs - 1;
	md.md_mflags = 0;
	md.md_dflags = 0;
	md.md_syncid = 1;
	md.md_sync_offset = 0;
	valp = gctl_get_paraml(req, "slice", sizeof(*valp));
	if (valp == NULL) {
		gctl_error(req, "No '%s' argument.", "slice");
		return;
	}
	md.md_slice = *valp;
	str = gctl_get_asciiparam(req, "balance");
	if (str == NULL) {
		gctl_error(req, "No '%s' argument.", "balance");
		return;
	}
	bal = balance_id(str);
	if (bal == -1) {
		gctl_error(req, "Wrong balance algorithm.");
		return;
	}
	md.md_balance = bal;
	noautosync = gctl_get_paraml(req, "noautosync", sizeof(*noautosync));
	if (noautosync == NULL) {
		gctl_error(req, "No '%s' argument.", "noautosync");
		return;
	}
	if (*noautosync)
		md.md_mflags |= G_MIRROR_DEVICE_FLAG_NOAUTOSYNC;
	hardcode = gctl_get_paraml(req, "hardcode", sizeof(*hardcode));
	if (hardcode == NULL) {
		gctl_error(req, "No '%s' argument.", "hardcode");
		return;
	}

	/*
	 * Calculate sectorsize by finding least common multiple from
	 * sectorsizes of every disk and find the smallest mediasize.
	 */
	mediasize = 0;
	sectorsize = 0;
	for (i = 1; i < *nargs; i++) {
		unsigned ssize;
		off_t msize;

		snprintf(param, sizeof(param), "arg%u", i);
		str = gctl_get_asciiparam(req, param);

		msize = g_get_mediasize(str);
		ssize = g_get_sectorsize(str);
		if (msize == 0 || ssize == 0) {
			gctl_error(req, "Can't get informations about %s: %s.",
			    str, strerror(errno));
			return;
		}
		msize -= ssize;
		if (mediasize == 0 || (mediasize > 0 && msize < mediasize))
			mediasize = msize;
		if (sectorsize == 0)
			sectorsize = ssize;
		else
			sectorsize = g_lcm(sectorsize, ssize);
	}
	md.md_mediasize = mediasize;
	md.md_sectorsize = sectorsize;

	/*
	 * Clear last sector first, to spoil all components if device exists.
	 */
	for (i = 1; i < *nargs; i++) {
		snprintf(param, sizeof(param), "arg%u", i);
		str = gctl_get_asciiparam(req, param);

		error = g_metadata_clear(str, NULL);
		if (error != 0) {
			gctl_error(req, "Can't store metadata on %s: %s.", str,
			    strerror(error));
			return;
		}
	}

	/*
	 * Ok, store metadata (use disk number as priority).
	 */
	for (i = 1; i < *nargs; i++) {
		snprintf(param, sizeof(param), "arg%u", i);
		str = gctl_get_asciiparam(req, param);

		md.md_did = arc4random();
		md.md_priority = i - 1;
		if (!*hardcode)
			bzero(md.md_provider, sizeof(md.md_provider));
		else {
			if (strncmp(str, _PATH_DEV, strlen(_PATH_DEV)) == 0)
				str += strlen(_PATH_DEV);
			strlcpy(md.md_provider, str, sizeof(md.md_provider));
		}
		mirror_metadata_encode(&md, sector);
		error = g_metadata_store(str, sector, sizeof(sector));
		if (error != 0) {
			fprintf(stderr, "Can't store metadata on %s: %s.\n",
			    str, strerror(error));
			gctl_error(req, "Not fully done.");
			continue;
		}
		if (verbose)
			printf("Metadata value stored on %s.\n", str);
	}
}

static void
mirror_clear(struct gctl_req *req)
{
	const char *name;
	char param[16];
	int *nargs, error, i;

	nargs = gctl_get_paraml(req, "nargs", sizeof(*nargs));
	if (nargs == NULL) {
		gctl_error(req, "No '%s' argument.", "nargs");
		return;
	}
	if (*nargs < 1) {
		gctl_error(req, "Too few arguments.");
		return;
	}

	for (i = 0; i < *nargs; i++) {
		snprintf(param, sizeof(param), "arg%u", i);
		name = gctl_get_asciiparam(req, param);

		error = g_metadata_clear(name, G_MIRROR_MAGIC);
		if (error != 0) {
			fprintf(stderr, "Can't clear metadata on %s: %s.\n",
			    name, strerror(error));
			gctl_error(req, "Not fully done.");
			continue;
		}
		if (verbose)
			printf("Metadata cleared on %s.\n", name); 
	}
}

static void
mirror_dump(struct gctl_req *req)
{
	struct g_mirror_metadata md, tmpmd;
	const char *name;
	char param[16];
	int *nargs, error, i;

	nargs = gctl_get_paraml(req, "nargs", sizeof(*nargs));
	if (nargs == NULL) {
		gctl_error(req, "No '%s' argument.", "nargs");
		return;
	}
	if (*nargs < 1) {
		gctl_error(req, "Too few arguments.");
		return;
	}

	for (i = 0; i < *nargs; i++) {
		snprintf(param, sizeof(param), "arg%u", i);
		name = gctl_get_asciiparam(req, param);

		error = g_metadata_read(name, (u_char *)&tmpmd, sizeof(tmpmd),
		    G_MIRROR_MAGIC);
		if (error != 0) {
			fprintf(stderr, "Can't read metadata from %s: %s.\n",
			    name, strerror(error));
			gctl_error(req, "Not fully done.");
			continue;
		}
		if (mirror_metadata_decode((u_char *)&tmpmd, &md) != 0) {
			fprintf(stderr, "MD5 hash mismatch for %s, skipping.\n",
			    name);
			gctl_error(req, "Not fully done.");
			continue;
		}
		printf("Metadata on %s:\n", name);
		mirror_metadata_dump(&md);
		printf("\n");
	}
}

static void
mirror_activate(struct gctl_req *req)
{
	struct g_mirror_metadata md, tmpmd;
	const char *name, *path;
	int *nargs, error, i;
	char param[16];

	nargs = gctl_get_paraml(req, "nargs", sizeof(*nargs));
	if (nargs == NULL) {
		gctl_error(req, "No '%s' argument.", "nargs");
		return;
	}
	if (*nargs < 2) {
		gctl_error(req, "Too few arguments.");
		return;
	}
	name = gctl_get_asciiparam(req, "arg0");
	if (name == NULL) {
		gctl_error(req, "No 'arg%u' argument.", 0);
		return;
	}

	for (i = 1; i < *nargs; i++) {
		snprintf(param, sizeof(param), "arg%u", i);
		path = gctl_get_asciiparam(req, param);

		error = g_metadata_read(path, (u_char *)&tmpmd, sizeof(tmpmd),
		    G_MIRROR_MAGIC);
		if (error != 0) {
			fprintf(stderr, "Cannot read metadata from %s: %s.\n",
			    path, strerror(error));
			gctl_error(req, "Not fully done.");
			continue;
		}
		if (mirror_metadata_decode((u_char *)&tmpmd, &md) != 0) {
			fprintf(stderr,
			    "MD5 hash mismatch for provider %s, skipping.\n",
			    path);
			gctl_error(req, "Not fully done.");
			continue;
		}
		if (strcmp(md.md_name, name) != 0) {
			fprintf(stderr,
			    "Provider %s is not the mirror %s component.\n",
			    path, name);
			gctl_error(req, "Not fully done.");
			continue;
		}
		md.md_dflags &= ~G_MIRROR_DISK_FLAG_INACTIVE;
		mirror_metadata_encode(&md, (u_char *)&tmpmd);
		error = g_metadata_store(path, (u_char *)&tmpmd, sizeof(tmpmd));
		if (error != 0) {
			fprintf(stderr, "Cannot write metadata from %s: %s.\n",
			    path, strerror(error));
			gctl_error(req, "Not fully done.");
			continue;
		}
		if (verbose)
			printf("Provider %s activated.\n", path); 
	}
}
