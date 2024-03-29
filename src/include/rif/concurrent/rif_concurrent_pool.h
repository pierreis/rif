/*
 * This file is part of Rif.
 *
 * Copyright 2017 Ironmelt Limited.
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

#include "rif/base/rif_pool.h"
#include "rif/concurrent/rif_threads.h"
#include "rif/concurrent/collection/rif_concurrent_queue_base.h"

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
 * Rif concurrent pool node.
 */
typedef struct rif_concurrent_pool_node_s {

  /**
   * @private
   *
   * A pool node is a que node.
   */
  rif_concurrent_queue_base_node_t _;

  /**
   * @private
   *
   * The actual memory block.
   */
  char element[];

} rif_concurrent_pool_node_t;

/**
 * Rif concurrent pool.
 *
 * @extends rif_pool_t
 */
typedef struct rif_concurrent_pool_t {

  /**
   * @private
   *
   * `rif_concurrent_pool_t` is a `rif_pool_t` subtype.
   */
  rif_pool_t _;

  /**
   * @private
   *
   * Element free queue.
   */
  rif_concurrent_queue_base_t free_queue;

  /**
   * @private
   *
   * Element size.
   */
  uint32_t element_size;

} rif_concurrent_pool_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Concurrent pool hooks.
 */
extern const rif_pool_hooks_t rif_concurrent_pool_hooks;

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
rif_concurrent_pool_t * rif_concurrent_pool_init(rif_concurrent_pool_t *pool_ptr, uint32_t element_size);

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
