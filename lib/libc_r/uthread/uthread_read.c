/*
 * Copyright (c) 1995 John Birrell <jb@cimlogic.com.au>.
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by John Birrell.
 * 4. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY JOHN BIRRELL AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id: uthread_read.c,v 1.3 1997/04/01 22:44:15 jb Exp $
 *
 */
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/uio.h>
#include <errno.h>
#include <unistd.h>
#ifdef _THREAD_SAFE
#include <pthread.h>
#include "pthread_private.h"

ssize_t
read(int fd, void *buf, size_t nbytes)
{
	int	ret;
	int	status;

	/* Lock the file descriptor for read: */
	if ((ret = _thread_fd_lock(fd, FD_READ, NULL,
	    __FILE__, __LINE__)) == 0) {
		/* Perform a non-blocking read syscall: */
		while ((ret = _thread_sys_read(fd, buf, nbytes)) < 0) {
			if ((_thread_fd_table[fd]->flags & O_NONBLOCK) == 0 &&
			    (errno == EWOULDBLOCK || errno == EAGAIN)) {
				_thread_kern_sig_block(&status);
				_thread_run->data.fd.fd = fd;
				_thread_kern_set_timeout(NULL);

				/* Reset the interrupted operation flag: */
				_thread_run->interrupted = 0;

				_thread_kern_sched_state(PS_FDR_WAIT,
				    __FILE__, __LINE__);

				/*
				 * Check if the operation was
				 * interrupted by a signal
				 */
				if (_thread_run->interrupted) {
					ret = -1;
					break;
				}
			} else {
				break;
			}
		}
		_thread_fd_unlock(fd, FD_READ);
	}
	return (ret);
}
#endif
