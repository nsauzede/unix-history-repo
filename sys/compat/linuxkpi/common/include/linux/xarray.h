/*-
 * Copyright (c) 2020 Mellanox Technologies, Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
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
 * $FreeBSD$
 */
#ifndef	_LINUX_XARRAY_H_
#define	_LINUX_XARRAY_H_

#include <linux/gfp.h>
#include <linux/radix-tree.h>
#include <linux/err.h>

#include <sys/lock.h>
#include <sys/mutex.h>

#define	XA_LIMIT(min, max) \
    ({ CTASSERT((min) == 0); (uint32_t)(max); })

#define	XA_FLAGS_ALLOC (1U << 0)
#define	XA_FLAGS_LOCK_IRQ (1U << 1)

#define	XA_ERROR(x) \
	ERR_PTR(x)

#define	xa_limit_32b XA_LIMIT(0, 0xFFFFFFFF)

#define	XA_ASSERT_LOCKED(xa) mtx_assert(&(xa)->mtx, MA_OWNED)
#define	xa_lock(xa) mtx_lock(&(xa)->mtx)
#define	xa_unlock(xa) mtx_unlock(&(xa)->mtx)

struct xarray {
	struct radix_tree_root root;
	struct mtx mtx;		/* internal mutex */
};

/*
 * Extensible arrays API implemented as a wrapper
 * around the radix tree implementation.
 */
void *xa_erase(struct xarray *, uint32_t);
void *xa_load(struct xarray *, uint32_t);
int xa_alloc(struct xarray *, uint32_t *, void *, uint32_t, gfp_t);
int xa_alloc_cyclic(struct xarray *, uint32_t *, void *, uint32_t, uint32_t *, gfp_t);
int xa_insert(struct xarray *, uint32_t, void *, gfp_t);
void *xa_store(struct xarray *, uint32_t, void *, gfp_t);
void xa_init_flags(struct xarray *, uint32_t);
bool xa_empty(struct xarray *);
void xa_destroy(struct xarray *);
void *xa_next(struct xarray *, unsigned long *, bool);

#define	xa_for_each(xa, index, entry) \
	for ((entry) = NULL, (index) = 0; \
	     ((entry) = xa_next(xa, &index, (entry) != NULL)) != NULL; )

/*
 * Unlocked version of functions above.
 */
void *__xa_erase(struct xarray *, uint32_t);
int __xa_alloc(struct xarray *, uint32_t *, void *, uint32_t, gfp_t);
int __xa_alloc_cyclic(struct xarray *, uint32_t *, void *, uint32_t, uint32_t *, gfp_t);
int __xa_insert(struct xarray *, uint32_t, void *, gfp_t);
void *__xa_store(struct xarray *, uint32_t, void *, gfp_t);
bool __xa_empty(struct xarray *);
void *__xa_next(struct xarray *, unsigned long *, bool);

static inline int
xa_err(void *ptr)
{
	return (PTR_ERR_OR_ZERO(ptr));
}

#endif		/* _LINUX_XARRAY_H_ */
