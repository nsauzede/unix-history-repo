/*
 * Copyright (c) 2002 Marcel Moolenaar
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <sys/endian.h>
#include <sys/uuid.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void
usage(void)
{
	(void)fprintf(stderr, "usage: uuidgen [-1] [-n count]\n");
	exit(1);
}

static void
uuid_print(uuid_t *uuid)
{
	printf("%08x-%04x-%04x-%02x%02x-", uuid->time_low, uuid->time_mid,
	    uuid->time_hi_and_version, uuid->clock_seq_hi_and_reserved,
	    uuid->clock_seq_low);
	printf("%02x%02x%02x%02x%02x%02x\n", uuid->node[0], uuid->node[1],
	    uuid->node[2], uuid->node[3], uuid->node[4], uuid->node[5]);
}

int
main(int argc, char *argv[])
{
	uuid_t *store, *uuid;
	char *p;
	int ch, count, i, iterate;

	count = -1;	/* no count yet */
	iterate = 0;	/* not one at a time */
	while ((ch = getopt(argc, argv, "1n:")) != -1)
		switch (ch) {
		case '1':
			iterate = 1;
			break;
		case 'n':
			if (count > 0)
				usage();
			count = strtol(optarg, &p, 10);
			if (*p != 0 || count < 1)
				usage();
			break;
		default:
			usage();
		}
	argv += optind;
	argc -= optind;

	if (argc)
		usage();

	if (count == -1)
		count = 1;

	store = (uuid_t*)malloc(sizeof(uuid_t) * count);
	if (store == NULL)
		err(1, "malloc()");

	if (!iterate) {
		/* Get them all in a single batch */
		if (uuidgen(store, count) != 0)
			err(1, "uuidgen()");
	} else {
		uuid = store;
		for (i = 0; i < count; i++) {
			if (uuidgen(uuid++, 1) != 0)
				err(1, "uuidgen()");
		}
	}

	uuid = store;
	while (count--)
		uuid_print(uuid++);

	free(store);
	return (0);
}
