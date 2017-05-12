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
 * STATIC HELPERS
 */

static
void _rif_concurrent_pool_destructor(void *pool) {
  if (!pool) {
    return;
  }
  rif_pool_destroy((rif_pool_t *) pool);
  rif_free(pool);
}

static
rif_pool_t * _rif_concurrent_pool_get(rif_concurrent_pool_t *pool_ptr) {
  rif_pool_t *tl_pool_ptr = (rif_pool_t *) tss_get(pool_ptr->pools);
  if (!tl_pool_ptr) {
    tl_pool_ptr = rif_malloc(sizeof(rif_pool_t));
    if (!tl_pool_ptr) {
      return tl_pool_ptr;
    }
    if (!rif_pool_init(tl_pool_ptr, pool_ptr->block_size, pool_ptr->element_size, false)) {
      rif_free(tl_pool_ptr);
      return NULL;
    }
    tss_set(pool_ptr->pools, tl_pool_ptr);
  }
  return tl_pool_ptr;
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_concurrent_pool_t * rif_concurrent_pool_init(rif_concurrent_pool_t *pool_ptr, uint32_t block_size,
                                                 uint32_t element_size) {
  if (!pool_ptr) {
    return pool_ptr;
  }
  pool_ptr->block_size = block_size;
  pool_ptr->element_size = element_size;
  if (thrd_error == tss_create(&pool_ptr->pools, _rif_concurrent_pool_destructor)) {
    return NULL;
  }
  return pool_ptr;
}

void rif_concurrent_pool_destroy(rif_concurrent_pool_t *pool_ptr) {
  tss_delete(pool_ptr->pools);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

void * rif_concurrent_pool_borrow(rif_concurrent_pool_t *pool_ptr) {
  rif_pool_t *tl_pool_ptr = _rif_concurrent_pool_get(pool_ptr);
  if (!tl_pool_ptr) {
    return NULL;
  }
  return rif_pool_borrow(tl_pool_ptr);
}

void rif_concurrent_pool_return(rif_concurrent_pool_t *pool_ptr, void *ptr) {
  rif_pool_t *tl_pool_ptr = _rif_concurrent_pool_get(pool_ptr);
  if (!tl_pool_ptr) {
    return;
  }
  rif_pool_return(tl_pool_ptr, ptr);
}