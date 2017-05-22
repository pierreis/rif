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
 * @brief Rif concurrent blocking queue.
 */

#pragma once

#include "rif/concurrent/collection/rif_concurrent_queue.h"
#include "rif/concurrent/rif_threads.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * Rif concurrent queue.
 *
 * @extends rif_queue_t
 */
typedef struct rif_concurrent_blocking_queue_t {

  /**
   * @private
   *
   * `rif_concurrent_blocking_queue_t` is a `rif_concurrent_queue_t` subtype.
   */
  rif_concurrent_queue_t _;

  /**
   * @private
   *
   * Blocking semaphore.
   */
  rif_sem_t sem;

  /**
   * @private
   *
   * Parent push callback.
   */
  rif_concurrent_queue_base_push_t parent_add;

} rif_concurrent_blocking_queue_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Hashmap hooks.
 */
extern const rif_queue_hooks_t rif_concurrent_blocking_queue_hooks;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a base concurrent queue
 */
RIF_API
rif_concurrent_blocking_queue_t * rif_concurrent_blocking_queue_init(rif_concurrent_blocking_queue_t *queue_ptr);

/**
 * Release a @ref rif_concurrent_blocking_queue_t.
 *
 * Decrements the reference count of @a cbq_ptr by one.
 * If the reference count of @a cbq_ptr reaches `0` and the value is heap-allocated, it will be freed.
 *
 * @param value The @ref rif_concurrent_blocking_queue_t to release.
 *
 * @see rif_val_release
 * @public @memberof rif_concurrent_blocking_queue_t
 */
RIF_INLINE
void rif_concurrent_blocking_queue_release(rif_concurrent_blocking_queue_t *cbq_ptr) {
  rif_val_release(cbq_ptr);
}

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
uint32_t rif_concurrent_blocking_queue_size(const rif_concurrent_blocking_queue_t *queue_ptr) {
  return rif_concurrent_queue_size((rif_concurrent_queue_t *) queue_ptr);
}

void rif_concurrent_blocking_queue_destroy_callback(rif_concurrent_blocking_queue_t *queue_ptr);

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Push a new node to the queue
 */
RIF_INLINE
rif_status_t rif_concurrent_blocking_queue_push(rif_concurrent_blocking_queue_t *queue_ptr, rif_val_t *val_ptr) {
  return rif_concurrent_queue_push((rif_concurrent_queue_t *) queue_ptr, val_ptr);
}

/**
 * Pop the first node of the queue
 */
RIF_API
rif_val_t * rif_concurrent_blocking_queue_pop(rif_concurrent_blocking_queue_t *queue_ptr);

/**
 * Pop the first node of the queue
 */
RIF_API
rif_val_t * rif_concurrent_blocking_queue_trypop(rif_concurrent_blocking_queue_t *queue_ptr);

/**
 * Pop the first node of the queue
 */
RIF_API
rif_val_t * rif_concurrent_blocking_queue_timedpop(rif_concurrent_blocking_queue_t *queue_ptr,
                                                   const struct timespec *time);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
