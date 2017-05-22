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
 * @brief Rif hashmap.
 */

#pragma once

#include "rif/collection/rif_map.h"
#include "rif/common/rif_status.h"

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
 * Rif internal hashmap struct.
 */
typedef struct rif_hashmap_element_s {

  /**
   * @private
   *
   * Element key hash
   */
  uint32_t hash;

  /**
   * @private
   *
   * Element distance to ideal position
   */
  rif_val_t *key_ptr;

  /**
   * @private
   *
   * Element value
   */
  rif_val_t *val_ptr;

} rif_hashmap_element_t;

/**
 * Rif hashmap type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_hashmap_t` methods.
 *
 * @extends rif_map_t
 */
typedef struct rif_hashmap_s {

  /**
   * @private
   *
   * `rif_hashmap_t` is a `rif_map_t` subtype.
   */
  rif_map_t _;

  /**
   * @private
   *
   * Current size of the map.
   */
  uint32_t size;

  /**
   * @private
   *
   * Current allocated capacity of the map.
   */
  uint32_t capacity;

  /**
   * @private
   *
   * Has the map a fixed capacity?
   */
  bool fixed;

  /**
   * @private
   *
   * Whether or not to free the element array upon destroy.
   */
  bool free_elements;

  /**
   * @private
   *
   * Current element array.
   */
  rif_hashmap_element_t *elements;

} rif_hashmap_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Hashmap hooks.
 */
extern const rif_map_hooks_t rif_hashmap_hooks;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a heap-allocated hashmap.
 *
 * @param hm_ptr   the hashmap to initialize
 * @param capacity the initial capacity to allocate ; if `0`, the storage will be allocated lazily
 * @param fixed    if `true`, the map will have a fixed-size of `capacity`
 * @return         the initialized hashmap if successful, or `NULL` otherwise
 */
RIF_API
rif_hashmap_t * rif_hashmap_init(rif_hashmap_t *hm_ptr, uint32_t capacity, bool fixed);

/**
 * Initialize a stack-allocated map.
 *
 * Due to the stack allocation, the resulting map has a fixed size of `capacity`, and cannot be extended.
 *
 * @param __hm_ptr   the map to initialize
 * @param __capacity the initial capacity to allocate. If `0`, the storage will be allocated lazily
 */
#define rif_hashmap_inita(__hm_ptr, __capacity) \
  rif_hashmap_init((__hm_ptr), 0, true); \
  (__hm_ptr)->free_elements = false; \
  (__hm_ptr)->capacity = rif_next_pow2(__capacity + __capacity / 10); \
  (__hm_ptr)->elements = ((rif_hashmap_element_t *) rif_alloca((__hm_ptr)->capacity * sizeof(rif_hashmap_element_t))); \
  memset((__hm_ptr)->elements, 0, (__hm_ptr)->capacity * sizeof(rif_hashmap_element_t));

/**
 * Releases a `rif_hashmap_t`. If the reference count reaches 0, the value will be freed.
 *
 * @param value the `rif_hashmap_t` to release
 */
RIF_INLINE
void rif_hashmap_release(rif_hashmap_t *hm_ptr) {
  rif_val_release(hm_ptr);
}

/******************************************************************************
 * SIZING FUNCTIONS
 */

/**
 * Ensures the map has enough allocated capacity to store at least `capacity` elements without reallocation.
 *
 * The map is resized if it does not have the required allocated capacity.
 *
 * @param hm_ptr   the map
 * @param capacity the desired minimum capacity
 * @return
 *   - `RIF_OK`           if the operation is successful
 *   - `RIF_ERR_MEMORY`   if memory allocation failed
 *   - `RIF_ERR_CAPACITY` if the map has a fixed capacity, and `capacity` is greater than the fixed capacity
 */
RIF_API
rif_status_t rif_hashmap_ensure_capacity(rif_hashmap_t *hm_ptr, uint32_t capacity);

/******************************************************************************
 * INFO FUNCTIONS
 */

/**
 * Get the size of the map.
 *
 * @param hm_ptr the map
 * @return       the number of elements currently in the map
 */
RIF_INLINE
uint32_t rif_hashmap_size(const rif_hashmap_t *hm_ptr) {
  return hm_ptr->size;
}

/**
 * Get the allocated capacity of the map.
 *
 * @param hm_ptr the map
 * @return       the allocated capacity of the hashmap
 */
RIF_INLINE
uint32_t rif_hashmap_capacity(const rif_hashmap_t *hm_ptr) {
  return hm_ptr->capacity;
}

/******************************************************************************
 * ELEMENT READ FUNCTIONS
 */

/**
 * Checks whether an element exists in the map with the specified key.
 *
 * @param hm_ptr  the map
 * @param key_ptr the key of the element to check for existence
 * @return        `true` if an element with the specified key exists in the map, or `false` otherwise
 */
RIF_API
bool rif_hashmap_exists(const rif_hashmap_t *hm_ptr, const rif_val_t *key_ptr);

/**
 * Returns the element with the specified key in this map.
 *
 * @param hm_ptr  the map
 * @param key_ptr the key of the element to return
 * @return        the element with the specified key in the map if it exists, or `NULL` otherwise
 */
RIF_API
rif_val_t * rif_hashmap_get(const rif_hashmap_t *hm_ptr, const rif_val_t *key_ptr);

/**
 * @private
 *
 * Returns the element at a specified index.
 *
 * This function is part of the internal API, and may change at any time.
 *
 * @param hm_ptr the map
 * @param index  the index of the element to return
 * @return       the element at the specified index in the map if it exists, or `NULL` otherwise
 */
RIF_API
rif_hashmap_element_t * rif_hashmap_atindex(const rif_hashmap_t *hm_ptr, uint32_t index);

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

/**
 * Inserts the specified element with the specified key in this map. If an element with the same key already exists in
 * the map, it will be replaced.
 *
 * @param hm_ptr  the map
 * @param key_ptr the key of the element is to be inserted
 * @param val_ptr element to be inserted
 * @return
 *   - `RIF_OK`                if the operation is successful
 *   - `RIF_ERR_MEMORY`        if memory allocation failed
 *   - `RIF_ERR_CAPACITY`      if the map has a fixed capacity, and the new element does not fit in the map
 */
RIF_API
rif_status_t rif_hashmap_put(rif_hashmap_t *hm_ptr, rif_val_t *key_ptr, rif_val_t *val_ptr);

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

/**
 * Removes the element with the specified key in this map.
 *
 * @param hm_ptr  the map
 * @param key_ptr the key of the element to be removed
 * @return        `RIF_OK`
 */
RIF_API
rif_status_t rif_hashmap_remove(rif_hashmap_t *hm_ptr, rif_val_t *key_ptr);

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function to destroy a `rif_hashmap_t`.
 */
void rif_hashmap_destroy_callback(rif_hashmap_t *hm_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
