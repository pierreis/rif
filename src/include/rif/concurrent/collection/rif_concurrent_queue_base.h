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
 * @brief Rif concurrent queue basic building block.
 *
 * Provides the basic data structure required to build a queue-based concurrent data structure.
 */

#pragma once

#include "rif/concurrent/rif_atomic.h"

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
typedef struct rif_concurrent_queue_base_node_s {

  /**
   * @private
   *
   * Element size.
   */
  atomic_uint32_t reference_count;

  /**
   * @private
   *
   * Pointer to the successor node.
   */
  atomic_uintptr_t succ;

} rif_concurrent_queue_base_node_t;

/**
 * @private
 *
 * Optional callback function called to destroy elements.
 */
typedef void (*rif_concurrent_queue_base_destroy_t)(rif_concurrent_queue_base_node_t *node_ptr);

/**
 * @private
 *
 * Rif concurrent queue.
 */
typedef struct rif_concurrent_queue_base_t {

  /**
   * @private
   *
   * Pointer to the first node.
   */
  atomic_uintptr_t first;

  /**
   * @private
   *
   * Node destructor.
   */
   rif_concurrent_queue_base_destroy_t dtor;

} rif_concurrent_queue_base_t;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a base concurrent queue
 */
RIF_INLINE
rif_concurrent_queue_base_t * rif_concurrent_queue_base_init(rif_concurrent_queue_base_t *queue_ptr,
                                                             rif_concurrent_queue_base_destroy_t dtor) {
  atomic_init(&queue_ptr->first, 0);
  queue_ptr->dtor = dtor;
  return queue_ptr;
}

/**
 * Destroy a base concurrent queue
 */
void rif_concurrent_queue_base_destroy(rif_concurrent_queue_base_t *queue_ptr);

/******************************************************************************
 * UTILITY FUNCTIONS
 */

/**
 * Initialize a base queue node
 */
RIF_INLINE
void rif_concurrent_queue_base_node_init(rif_concurrent_queue_base_t *queue_ptr,
                                         rif_concurrent_queue_base_node_t *node_ptr) {
  atomic_init(&node_ptr->succ, 0);
  atomic_init(&node_ptr->reference_count, 0);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Push a new node to the queue
 */
RIF_API
void rif_concurrent_queue_base_push(rif_concurrent_queue_base_t *queue_ptr, rif_concurrent_queue_base_node_t *node_ptr);

/**
 * Pop the first node of the queue
 */
RIF_API
rif_concurrent_queue_base_node_t * rif_concurrent_queue_base_pop(rif_concurrent_queue_base_t *queue_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
