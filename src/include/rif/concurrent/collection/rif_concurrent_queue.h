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
 * @brief Rif concurrent queue.
 */

#pragma once

#include "rif/base/rif_val.h"
#include "rif/collection/rif_queue.h"
#include "rif/concurrent/rif_atomic.h"
#include "rif/concurrent/rif_concurrent_pool.h"

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
 * Rif concurrent queue node.
 */
typedef struct rif_concurrent_queue_node_s {

  /**
   * @private
   *
   * A pool node is a que node.
   */
  rif_concurrent_queue_base_node_t _;

  /**
   * @private
   *
   * The element value.
   */
  rif_val_t *val;

} rif_concurrent_queue_node_t;

/**
 * Rif concurrent queue.
 *
 * @extends rif_queue_t
 */
typedef struct rif_concurrent_queue_t {

  /**
   * @private
   *
   * `rif_concurrent_pool_t` is a `rif_pool_t` subtype.
   */
  rif_queue_t _;

  /**
   * @private
   *
   * Element free queue.
   */
  rif_concurrent_queue_base_t queue_base;

  /**
   * @private
   *
   * Element free queue.
   */
  rif_concurrent_pool_t pool;

  /**
   * @private
   *
   * Element size.
   */
  atomic_uint32_t size;

} rif_concurrent_queue_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Hashmap hooks.
 */
extern const rif_queue_hooks_t rif_concurrent_queue_hooks;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a base concurrent queue
 */
RIF_API
rif_concurrent_queue_t * rif_concurrent_queue_init(rif_concurrent_queue_t *queue_ptr);

/******************************************************************************
 * INFO FUNCTIONS
 */

/**
 * Get the size of the queue.
 *
 * @param ll_ptr The list.
 * @return       The number of elements currently in the list.
 */
RIF_INLINE
uint32_t rif_concurrent_queue_size(const rif_concurrent_queue_t *queue_ptr) {
  return atomic_load_explicit(&queue_ptr->size, memory_order_relaxed);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Push a new node to the queue
 */
RIF_API
rif_status_t rif_concurrent_queue_push(rif_concurrent_queue_t *queue_ptr, rif_val_t *val_ptr);

/**
 * Pop the first node of the queue
 */
RIF_API
rif_val_t * rif_concurrent_queue_pop(rif_concurrent_queue_t *queue_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
