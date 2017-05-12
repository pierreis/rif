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

/**
 * @file
 * @brief Rif concurrent memory pool.
 */

#pragma once

#include "rif_threads.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * @private
 *
 * Rif pool block.
 */
typedef struct rif_concurrent_pool_t {

  /**
   * @private
   *
   * Element size.
   */
  uint32_t element_size;

  /**
   * @private
   *
   * Block size of the pool.
   */
  uint32_t block_size;

  /**
   * @private
   *
   *
   */
  tss_t pools;

} rif_concurrent_pool_t;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a pool
 *
 * @param pool_ptr pool to initialize
 * @param block_size block size
 * @param element_size element size
 * @param lazy if `true`, the first block will be initialized lazily
 *
 * @return the initialized pool, or `NULL` in case of failure
 */
RIF_API
rif_concurrent_pool_t * rif_concurrent_pool_init(rif_concurrent_pool_t *pool_ptr, uint32_t block_size,
                                                 uint32_t element_size);

/**
 * Destroy a pool
 *
 * @param pool_ptr pool to destroy
 */
RIF_API
void rif_concurrent_pool_destroy(rif_concurrent_pool_t *pool_ptr);

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Borrow an element from the pool
 *
 * If the pool doesn't have available elements, a new memory block is allocated.
 *
 * @param pool_ptr the pool
 *
 * @return an element from the pool, or `NULL` in case of allocation failure
 */
RIF_API
void * rif_concurrent_pool_borrow(rif_concurrent_pool_t *pool_ptr);

/**
 * Return a borrowed element back to the pool
 *
 * If `ptr` is `NULL`, the function has no effect. No additional memory check is done on the returned elements. If `ptr`
 * was not previously borrowed from the pool or is returned multiple times, the behavior is undefined.
 *
 * @param pool_ptr the pool
 * @param ptr element to return to the pool
 */
RIF_API
void rif_concurrent_pool_return(rif_concurrent_pool_t *pool_ptr, void *ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
