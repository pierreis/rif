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
 * @brief Rif queue type.
 */

#pragma once

#include "rif/base/rif_val.h"
#include "rif/common/rif_status.h"
#include "rif/util/rif_hook.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/* Forward declaration of `rif_queue_hooks_t`. */
typedef struct rif_queue_hooks_s rif_queue_hooks_t;

/**
 * The `rif_queue_t` interface.
 * All rif queue implementations inherit from this structure.
 *
 * @extends rif_val_t
 */
typedef struct rif_queue_t {

  /**
   * @private
   *
   * `rif_queue_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * Hooks used by `rif_queue_t` functions.
   */
  const rif_queue_hooks_t * hooks;

} rif_queue_t;

/**
 * Hooks used by `nuuk_queue_t` implementations.
 */
struct rif_queue_hooks_s {

  /**
   * Destroy a queue, and release resources used by it.
   *
   * @param queue the queue to destroy.
   */
  void (*destroy)(rif_queue_t *queue_ptr);

  /**
   * @see rif_queue_size
   */
  uint32_t (*size)(rif_queue_t *queue_ptr);

  /**
   * @see rif_queue_push
   */
  rif_status_t (*push)(rif_queue_t *queue_ptr, rif_val_t *val_ptr);

  /**
   * @see rif_queue_pop
   */
  rif_val_t * (*pop)(rif_queue_t *queue_ptr);

};

/******************************************************************************
 * HELPER MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_queue_t`.
 *
 * @param  __val the `rif_val_t`.
 * @return       the casted `rif_queue_t`.
 */
#define rif_queue_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_QUEUE, rif_queue_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a queue.
 *
 * This function should only be used by `rif_queue_t` subtypes.
 *
 * @param queue_ptr the queue to initialize
 * @param hooks queue hooks
 * @param free whether or not to free the value when the reference count reaches 0
 * @return the initialized queue, or `NULL` if initialization failed
 *
 * @relates rif_queue_t
 */
RIF_API
rif_queue_t * rif_queue_init(rif_queue_t *queue_ptr, const rif_queue_hooks_t *hooks, bool free);

/******************************************************************************
 * INFO FUNCTIONS
 */

/**
 * Get the size of the queue.
 *
 * @param queue_ptr the queue
 * @return number of elements currently in queue
 */
RIF_INLINE
uint32_t rif_queue_size(rif_queue_t *queue_ptr) {
  return rif_hook(size, 0, queue_ptr);
}

/******************************************************************************
 * ELEMENT ACCESS FUNCTIONS
 */

RIF_INLINE
rif_status_t rif_queue_push(rif_queue_t *queue_ptr, rif_val_t *val_ptr) {
  return rif_hook(push, RIF_ERR_UNSUPPORTED, queue_ptr, val_ptr);
}

/**
 * Inserts the specified element with the specified key in this queue. If an element with the same key already exists in
 * the queue, it will be replaced.
 *
 * @param queue_ptr the queue
 * @param key_ptr the key of the element is to be inserted
 * @param val_ptr element to be inserted with the specified key
 * @return
 *   - `RIF_OK`                if the operation is successful
 *   - `RIF_ERR_MEMORY`        if memory allocation failed
 *   - `RIF_ERR_CAPACITY`      if the queue has a fixed capacity, and the new element does not fit in the queue
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the queue implementation
 */
RIF_INLINE
rif_val_t * rif_queue_pop(rif_queue_t *queue_ptr) {
  return rif_hook(pop, NULL, queue_ptr);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_queue_t`.
 */
void rif_queue_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_queue_t`.
 */
uint32_t rif_queue_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_queue_t`.
 */
char * rif_queue_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
