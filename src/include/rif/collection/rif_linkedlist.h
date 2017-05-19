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
 * @brief Rif doubly-linked list.
 */

#pragma once

#include "rif/base/rif_paged_pool.h"
#include "rif/collection/rif_list.h"
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
 * Rif linkedlist node type.
 */
typedef struct rif_linkedlist_node_s {

  /**
   * @private
   *
   * Pointer to the predecessor node.
   */
  struct rif_linkedlist_node_s *pred;

  /**
   * @private
   *
   * Pointer to the successor node.
   */
  struct rif_linkedlist_node_s *succ;

  /**
   * @private
   *
   * Node value.
   */
  rif_val_t *val;

} rif_linkedlist_node_t;

/**
 * Rif linkedlist type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_linkedlist_t` methods.
 *
 * @extends rif_list_t
 */
typedef struct rif_linkedlist_s {

  /**
   * @private
   *
   * `rif_linkedlist_t` is a `rif_list_t` subtype.
   */
  rif_list_t _;

  /**
   * @private
   *
   * Current size of the list.
   */
  uint32_t size;

  /**
   * @private
   *
   * Pointer to the first node.
   */
  rif_linkedlist_node_t *first;

  /**
   * @private
   *
   * Pointer to the last node.
   */
  rif_linkedlist_node_t *last;

  /**
   * @private
   *
   * Memory pool for node allocation.
   */
  rif_paged_pool_t pool;

} rif_linkedlist_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Arraylist hooks.
 */
extern const rif_list_hooks_t rif_linkedlist_hooks;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_linkedlist_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_linkedlist_t`.
 */
//#define rif_linkedlist_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_STRING, rif_linkedlist_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a heap-allocated linkedlist.
 *
 * @param list The list to initialize.
 * @return     the initialized linkedlist if successful, or `NULL` otherwise.
 */
RIF_API
rif_linkedlist_t * rif_linkedlist_init(rif_linkedlist_t *ll_ptr);

/**
 * Create and initialize a heap-allocated linkedlist.
 *
 * @return the initialized linkedlist if successful, or `NULL` otherwise.
 */
RIF_API
rif_linkedlist_t * rif_linkedlist_new();

/**
 * Releases a `rif_linkedlist_t`. If the reference count reaches 0, the value will be freed.
 *
 * @param value The `rif_linkedlist_t` to release.
 */
RIF_INLINE
void rif_linkedlist_release(rif_linkedlist_t *ll_ptr) {
  rif_val_release(ll_ptr);
}

/******************************************************************************
 * SIZING FUNCTIONS
 */

/**
 * Ensures the list has enough allocated capacity to store at least `capacity` elements without reallocation.
 *
 * The list is resized if it does not have the required allocated capacity.
 *
 * @param ll_ptr   The list.
 * @param capacity The desired minimum capacity.
 *
 * @return
 *   - `RIF_OK`           if the operation is successful.
 *   - `RIF_ERR_MEMORY`   if memory allocation failed.
 *   - `RIF_ERR_CAPACITY` if the list has a fixed capacity, and `capacity` is greater than the fixed capacity.
 */
RIF_API
rif_status_t rif_linkedlist_ensure_capacity(rif_linkedlist_t *ll_ptr, uint32_t capacity);

/******************************************************************************
 * INFO FUNCTIONS
 */

/**
 * Get the size of the list.
 *
 * @param ll_ptr The list.
 * @return       The number of elements currently in the list.
 */
RIF_INLINE
uint32_t rif_linkedlist_size(const rif_linkedlist_t *ll_ptr) {
  return ll_ptr->size;
}

/******************************************************************************
 * ELEMENT READ FUNCTIONS
 */

/**
 * Returns the element at the specified position in this list.
 *
 * @param ll_ptr the list
 * @param index  index of the element to return
 *
 * @return the element at the specified position in the list if it exists, or `NULL` otherwise.
 */
RIF_API
rif_val_t * rif_linkedlist_get(const rif_linkedlist_t *ll_ptr, uint32_t index);

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

/**
 * Inserts the specified element at the specified position in this list. Shifts the element currently at that position
 * (if any) and any subsequent elements to the right (adds one to their indices).
 *
 * @param ll_ptr  the list
 * @param index   index at which the specified element is to be inserted
 * @param val_ptr element to be inserted at the specified position
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is greater than the size of the list.
 */
RIF_API
rif_status_t rif_linkedlist_insert(rif_linkedlist_t *ll_ptr, uint32_t index, rif_val_t *val_ptr);

/**
 * Appends the specified element to the end of the list.
 *
 * @param ll_ptr  the list
 * @param val_ptr element to be appended to the end of the list
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 */
RIF_INLINE
rif_status_t rif_linkedlist_append(rif_linkedlist_t *ll_ptr, rif_val_t *val_ptr) {
  return rif_linkedlist_insert(ll_ptr, ll_ptr->size, val_ptr);
}

/**
 * Prepends the specified element to the beginning of the list. Shifts any subsequent elements to the right (adds one to
 * their indices).
 *
 * @param ll_ptr  the list
 * @param val_ptr element to be prepended to the beginning of the list
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 */
RIF_INLINE
rif_status_t rif_linkedlist_prepend(rif_linkedlist_t *ll_ptr, rif_val_t *val_ptr) {
  return rif_linkedlist_insert(ll_ptr, 0, val_ptr);
}

/**
 * Replaces the element at the specified position in the list with the specified element.
 *
 * @param ll_ptr  the list
 * @param index   index of the element to replace
 * @param val_ptr element to be stored at the specified position
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is equal or greater than the size of the list.
 */
RIF_API
rif_status_t rif_linkedlist_set(rif_linkedlist_t *ll_ptr, uint32_t index, rif_val_t *val_ptr);

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

/**
 * Removes the element at the specified position in this list. Shifts any subsequent elements to the left (subtracts one
 * from their indices). Returns the element that was removed from the list.
 *
 * @param ll_ptr the list
 * @param index  the index of the element to be removed
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is equal or greater than the size of the list.
 */
RIF_API
rif_status_t rif_linkedlist_remove(rif_linkedlist_t *ll_ptr, uint32_t index);

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function to destroy a `rif_linkedlist_t`.
 */
void rif_linkedlist_destroy_callback(rif_linkedlist_t *ll_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
