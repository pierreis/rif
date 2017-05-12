/*
 * This file is part of Rif.
 *
 * Copyright 2015 Ironmelt Limited.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#include "rif/rif_internal.h"

/******************************************************************************
 * HOOK HELPERS
 */

static
void _rif_concurrent_pool_hook_destroy(rif_pool_t *pool_ptr) {
  rif_concurrent_pool_destroy((rif_concurrent_pool_t *) pool_ptr);
}

static
void * _rif_concurrent_pool_hook_borrow(rif_pool_t *pool_ptr) {
  return rif_concurrent_pool_borrow((rif_concurrent_pool_t *) pool_ptr);
}

static
void _rif_concurrent_pool_hook_return(rif_pool_t *pool_ptr, void *ptr) {
  return rif_concurrent_pool_return((rif_concurrent_pool_t *) pool_ptr, ptr);
}

/******************************************************************************
 * HOOKS
 */

const rif_pool_hooks_t rif_concurrent_pool_hooks = {
    .destroy = _rif_concurrent_pool_hook_destroy,
    .borrow  = _rif_concurrent_pool_hook_borrow,
    .return_ = _rif_concurrent_pool_hook_return
};