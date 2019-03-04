/*-
 * Copyright (c) 2019 The FreeBSD Foundation
 * All rights reserved.
 *
 * This software was developed by BFF Storage Systems, LLC under sponsorship
 * from the FreeBSD Foundation.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

extern "C" {
#include <fcntl.h>
}

#include "mockfs.hh"
#include "utils.hh"

using namespace testing;

class Create: public FuseTest {};

/*
 * If FUSE_CREATE sets the attr_valid, then subsequent GETATTRs should use the
 * attribute cache
 */
/* https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=235775 */
TEST_F(Create, DISABLED_attr_cache)
{
	const char FULLPATH[] = "mountpoint/some_file.txt";
	const char RELPATH[] = "some_file.txt";
	mode_t mode = 0755;
	uint64_t ino = 42;
	int fd;

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_LOOKUP &&
				strcmp(in->body.lookup, RELPATH) == 0);
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		out->header.error = -ENOENT;
		out->header.len = sizeof(out->header);
	}));

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			const char *name = (const char*)in->body.bytes +
				sizeof(fuse_open_in);
			return (in->header.opcode == FUSE_CREATE &&
				(0 == strcmp(RELPATH, name)));
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		SET_OUT_HEADER_LEN(out, create);
		out->body.create.entry.attr.mode = S_IFREG | mode;
		out->body.create.entry.nodeid = ino;
		out->body.create.entry.entry_valid = UINT64_MAX;
		out->body.create.entry.attr_valid = UINT64_MAX;
	}));

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_GETATTR &&
				in->header.nodeid == ino);
		}, Eq(true)),
		_)
	).Times(0);

	fd = open(FULLPATH, O_CREAT | O_EXCL, mode);
	EXPECT_LE(0, fd) << strerror(errno);
	/* Deliberately leak fd.  close(2) will be tested in release.cc */
}

/* 
 * The fuse daemon fails the request with EEXIST.  This usually indicates a
 * race condition: some other FUSE client created the file in between when the
 * kernel checked for it with lookup and tried to create it with create
 */
TEST_F(Create, eexist)
{
	const char FULLPATH[] = "mountpoint/some_file.txt";
	const char RELPATH[] = "some_file.txt";
	mode_t mode = 0755;

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_LOOKUP &&
				strcmp(in->body.lookup, RELPATH) == 0);
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		out->header.error = -ENOENT;
		out->header.len = sizeof(out->header);
	}));

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			const char *name = (const char*)in->body.bytes +
				sizeof(fuse_open_in);
			return (in->header.opcode == FUSE_CREATE &&
				(0 == strcmp(RELPATH, name)));
		}, Eq(true)),
		_)
	).WillOnce(Invoke([](auto in, auto out) {
		out->header.unique = in->header.unique;
		out->header.error = -EEXIST;
		out->header.len = sizeof(out->header);
	}));
	EXPECT_NE(0, open(FULLPATH, O_CREAT | O_EXCL, mode));
	EXPECT_EQ(EEXIST, errno);
}

// TODO: enosys: kernel should fall back to mknod/open

/*
 * Creating a new file after FUSE_LOOKUP returned a negative cache entry
 */
/* https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=236231 */
TEST_F(Create, DISABLED_entry_cache_negative)
{
	const char FULLPATH[] = "mountpoint/some_file.txt";
	const char RELPATH[] = "some_file.txt";
	mode_t mode = 0755;
	uint64_t ino = 42;
	int fd;

	/* create will first do a LOOKUP, adding a negative cache entry */
	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_LOOKUP &&
				strcmp(in->body.lookup, RELPATH) == 0);
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		/* nodeid means ENOENT and cache it */
		out->body.entry.nodeid = 0;
		out->header.unique = in->header.unique;
		out->header.error = 0;
		/* 
		 * Set entry_valid = 0 because this test isn't concerned with
		 * whether or not we actually cache negative entries, only with
		 * whether we interpret negative cache responses correctly.
		 */
		out->body.entry.entry_valid = 0;
		SET_OUT_HEADER_LEN(out, entry);
	}));

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			const char *name = (const char*)in->body.bytes +
				sizeof(fuse_open_in);
			return (in->header.opcode == FUSE_CREATE &&
				(0 == strcmp(RELPATH, name)));
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		SET_OUT_HEADER_LEN(out, create);
		out->body.create.entry.attr.mode = S_IFREG | mode;
		out->body.create.entry.nodeid = ino;
		out->body.create.entry.entry_valid = UINT64_MAX;
		out->body.create.entry.attr_valid = UINT64_MAX;
	}));

	/* Until the attr cache is working, we may send an additional GETATTR */
	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_GETATTR &&
				in->header.nodeid == ino);
		}, Eq(true)),
		_)
	).WillRepeatedly(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		SET_OUT_HEADER_LEN(out, attr);
		out->body.attr.attr.ino = ino;	// Must match nodeid
		out->body.attr.attr.mode = S_IFREG | 0644;
	}));

	fd = open(FULLPATH, O_CREAT | O_EXCL, mode);
	ASSERT_LE(0, fd) << strerror(errno);
	/* Deliberately leak fd.  close(2) will be tested in release.cc */
}

/*
 * Creating a new file should purge any negative namecache entries
 */
/* https://bugs.freebsd.org/bugzilla/show_bug.cgi?id=236231 */
TEST_F(Create, DISABLED_entry_cache_negative_purge)
{
	const char FULLPATH[] = "mountpoint/some_file.txt";
	const char RELPATH[] = "some_file.txt";
	mode_t mode = 0755;
	uint64_t ino = 42;
	int fd;

	/* create will first do a LOOKUP, adding a negative cache entry */
	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_LOOKUP &&
				strcmp(in->body.lookup, RELPATH) == 0);
		}, Eq(true)),
		_)
	).Times(1)
	.WillOnce(Invoke([=](auto in, auto out) {
		/* nodeid means ENOENT and cache it */
		out->body.entry.nodeid = 0;
		out->header.unique = in->header.unique;
		out->header.error = 0;
		out->body.entry.entry_valid = UINT64_MAX;
		SET_OUT_HEADER_LEN(out, entry);
	})).RetiresOnSaturation();

	/* Then the CREATE should purge the negative cache entry */
	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			const char *name = (const char*)in->body.bytes +
				sizeof(fuse_open_in);
			return (in->header.opcode == FUSE_CREATE &&
				(0 == strcmp(RELPATH, name)));
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		SET_OUT_HEADER_LEN(out, create);
		out->body.create.entry.attr.mode = S_IFREG | mode;
		out->body.create.entry.nodeid = ino;
		out->body.create.entry.attr_valid = UINT64_MAX;
	}));

	/* Until the attr cache is working, we may send an additional GETATTR */
	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_GETATTR &&
				in->header.nodeid == ino);
		}, Eq(true)),
		_)
	).WillRepeatedly(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		SET_OUT_HEADER_LEN(out, attr);
		out->body.attr.attr.ino = ino;	// Must match nodeid
		out->body.attr.attr.mode = S_IFREG | 0644;
	}));

	fd = open(FULLPATH, O_CREAT | O_EXCL, mode);
	ASSERT_LE(0, fd) << strerror(errno);

	/* Finally, a subsequent lookup should query the daemon */
	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_LOOKUP &&
				strcmp(in->body.lookup, RELPATH) == 0);
		}, Eq(true)),
		_)
	).Times(1)
	.WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		out->header.error = 0;
		out->body.entry.nodeid = ino;
		out->body.entry.attr.mode = S_IFREG | mode;
		SET_OUT_HEADER_LEN(out, entry);
	}));

	ASSERT_EQ(0, access(FULLPATH, F_OK)) << strerror(errno);
	/* Deliberately leak fd.  close(2) will be tested in release.cc */
}

/* 
 * The daemon is responsible for checking file permissions (unless the
 * default_permissions mount option was used)
 */
TEST_F(Create, eperm)
{
	const char FULLPATH[] = "mountpoint/some_file.txt";
	const char RELPATH[] = "some_file.txt";
	mode_t mode = 0755;

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_LOOKUP &&
				strcmp(in->body.lookup, RELPATH) == 0);
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		out->header.error = -ENOENT;
		out->header.len = sizeof(out->header);
	}));

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			const char *name = (const char*)in->body.bytes +
				sizeof(fuse_open_in);
			return (in->header.opcode == FUSE_CREATE &&
				(0 == strcmp(RELPATH, name)));
		}, Eq(true)),
		_)
	).WillOnce(Invoke([](auto in, auto out) {
		out->header.unique = in->header.unique;
		out->header.error = -EPERM;
		out->header.len = sizeof(out->header);
	}));
	EXPECT_NE(0, open(FULLPATH, O_CREAT | O_EXCL, mode));
	EXPECT_EQ(EPERM, errno);
}

TEST_F(Create, ok)
{
	const char FULLPATH[] = "mountpoint/some_file.txt";
	const char RELPATH[] = "some_file.txt";
	mode_t mode = 0755;
	uint64_t ino = 42;
	int fd;

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_LOOKUP &&
				strcmp(in->body.lookup, RELPATH) == 0);
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		out->header.error = -ENOENT;
		out->header.len = sizeof(out->header);
	}));

	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			const char *name = (const char*)in->body.bytes +
				sizeof(fuse_open_in);
			return (in->header.opcode == FUSE_CREATE &&
				(0 == strcmp(RELPATH, name)));
		}, Eq(true)),
		_)
	).WillOnce(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		SET_OUT_HEADER_LEN(out, create);
		out->body.create.entry.attr.mode = S_IFREG | mode;
		out->body.create.entry.nodeid = ino;
		out->body.create.entry.entry_valid = UINT64_MAX;
		out->body.create.entry.attr_valid = UINT64_MAX;
	}));

	/* Until the attr cache is working, we may send an additional GETATTR */
	EXPECT_CALL(*m_mock, process(
		ResultOf([=](auto in) {
			return (in->header.opcode == FUSE_GETATTR &&
				in->header.nodeid == ino);
		}, Eq(true)),
		_)
	).WillRepeatedly(Invoke([=](auto in, auto out) {
		out->header.unique = in->header.unique;
		SET_OUT_HEADER_LEN(out, attr);
		out->body.attr.attr.ino = ino;	// Must match nodeid
		out->body.attr.attr.mode = S_IFREG | 0644;
	}));

	fd = open(FULLPATH, O_CREAT | O_EXCL, mode);
	EXPECT_LE(0, fd) << strerror(errno);
	/* Deliberately leak fd.  close(2) will be tested in release.cc */
}
