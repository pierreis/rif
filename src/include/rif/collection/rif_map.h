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
 * @brief Rif map type.
 */

#pragma once

#include "rif/base/rif_val.h"
#include "rif/collection/rif_iterator.h"
#include "rif/common/rif_status.h"
#include "rif/util/rif_hook.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * Forward map iterator definition
 */
typedef union rif_map_iterator_u rif_map_iterator_t;

/* Forward declaration of `rif_map_hooks_t`. */
typedef struct rif_map_hooks_s rif_map_hooks_t;

/**
 * The `rif_map_t` interface.
 * All rif map implementations inherit from this structure.
 */
typedef struct rif_map_s {

  /**
   * @private
   *
   * `rif_map_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * Hooks used by `rif_map_t` functions.
   */
  const rif_map_hooks_t * hooks;

} rif_map_t;

/**
 * Hooks used by `nuuk_map_t` implementations.
 */
struct rif_map_hooks_s {

  /**
   * Destroy a map, and release resources used by it.
   *
   * @param map the map to destroy.
   */
  void (*destroy)(rif_map_t *map_ptr);

  /**
   * @see rif_map_size
   */
  uint32_t (*size)(rif_map_t *map_ptr);

  /**
   * @see rif_map_exists
   */
  bool (*exists)(rif_map_t *map_ptr, const rif_val_t *key_ptr);

  /**
   * @see rif_map_get
   */
  rif_val_t *(*get)(rif_map_t *map_ptr, const rif_val_t *key_ptr);

  /**
   * @see rif_map_put
   */
  rif_status_t (*put)(rif_map_t *map_ptr, rif_val_t *key_ptr, rif_val_t *val_ptr);

  /**
   * @see rif_map_remove
   */
  rif_status_t (*remove)(rif_map_t *map_ptr, rif_val_t *key_ptr);

  /* Iterator hooks */

  /**
   * @see rif_map_iterator_init
   */
  rif_map_iterator_t *(*iterator_init)(rif_map_t *map_ptr, rif_map_iterator_t *it_ptr, rif_pair_t *pair_ptr);

  /**
   * @see rif_map_iterator_new
   */
  rif_map_iterator_t *(*iterator_new)(rif_map_t *map_ptr);

};

/******************************************************************************
 * HELPER MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_map_t`.
 *
 * @param  __val the `rif_val_t`.
 * @return       the casted `rif_map_t`.
 */
#define rif_map_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_MAP, rif_map_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a map.
 *
 * This function should only be used by `rif_map_t` subtypes.
 *
 * @param map_ptr the map to initialize
 * @param hooks   the map hooks
 * @param free    whether or not to free the value when the reference count reaches 0
 * @return        the initialized map, or `NULL` if initialization failed
 *
 * @relates rif_map_t
 */
RIF_API
rif_map_t * rif_map_init(rif_map_t *map_ptr, const rif_map_hooks_t *hooks, bool free);

/******************************************************************************
 * INFO FUNCTIONS
 */

/**
 * Get the size of the map.
 *
 * @param map_ptr the map
 * @return        the number of elements currently in the map
 */
RIF_INLINE
uint32_t rif_map_size(rif_map_t *map_ptr) {
  return rif_hook(size, 0, map_ptr);
}

/******************************************************************************
 * ELEMENT READ FUNCTIONS
 */

/**
 * Checks whether an element exists in the map with the specified key.
 *
 * @param map_ptr the map
 * @param key_ptr the key of the element to check for existence
 * @return        `true` if an element with the specified key exists in the map, or `false` otherwise
 */
RIF_INLINE
bool rif_map_exists(rif_map_t *map_ptr, const rif_val_t *key_ptr) {
  return rif_hook(exists, false, map_ptr, key_ptr);
}

/**
 * Returns the element with the specified key in this map.
 *
 * @param map_ptr the map
 * @param key_ptr the key of the element to return
 * @return        the element with the specified key in the map if it exists, or `NULL` otherwise
 */
RIF_INLINE
rif_val_t * rif_map_get(rif_map_t *map_ptr, const rif_val_t *key_ptr) {
  return rif_hook(get, NULL, map_ptr, key_ptr);
}

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

/**
 * Inserts the specified element with the specified key in this map. If an element with the same key already exists in
 * the map, it will be replaced.
 *
 * @param map_ptr the map
 * @param key_ptr the key of the element is to be inserted
 * @param val_ptr element to be inserted with the specified key
 * @return
 *   - `RIF_OK`                if the operation is successful
 *   - `RIF_ERR_MEMORY`        if memory allocation failed
 *   - `RIF_ERR_CAPACITY`      if the map has a fixed capacity, and the new element does not fit in the map
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the map implementation
 */
RIF_INLINE
rif_status_t rif_map_put(rif_map_t *map_ptr, rif_val_t *key_ptr, rif_val_t *val_ptr) {
  return rif_hook(put, RIF_ERR_UNSUPPORTED, map_ptr, key_ptr, val_ptr);
}

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

/**
 * Removes the element with the specified key in this map.
 *
 * @param map_ptr the map
 * @param key_ptr the key of the element to be removed
 * @return
 *   - `RIF_OK`                if the operation is successful
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the map implementation
 */
RIF_INLINE
rif_status_t rif_map_remove(rif_map_t *map_ptr, rif_val_t *key_ptr) {
  return rif_hook(remove, RIF_ERR_UNSUPPORTED, map_ptr, key_ptr);
}

/******************************************************************************
 * ITERATOR FUNCTIONS
 */

/**
 * Initializes a heap-allocated map iterator
 *
 * @param it_ptr  the iterator to initialize
 * @param map_ptr the map to iterate
 * @return        the initialized map iterator if successful, or `NULL` otherwise
 */
RIF_INLINE
rif_map_iterator_t * rif_map_iterator_init(rif_map_iterator_t *it_ptr, rif_map_t *map_ptr, rif_pair_t *pair_ptr) {
  return rif_hook(iterator_init, NULL, map_ptr, it_ptr, pair_ptr);
}

/**
 * Creates a stack-allocated map iterator.
 *
 * @param map_ptr the map to iterate
 * @return        the initialized map iterator if successful, or `NULL` otherwise
 */
RIF_INLINE
rif_map_iterator_t * rif_map_iterator_new(rif_map_t *map_ptr) {
  return rif_hook(iterator_new, NULL, map_ptr);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_map_t`.
 */
void rif_map_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_map_t`.
 */
uint32_t rif_map_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function to compare equality of two values.
 */
bool rif_map_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_map_t`.
 */
char * rif_map_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
