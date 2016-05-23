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
 * @brief Rif list type.
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
 * Forward list iterator definition
 */
typedef union rif_list_iterator_u rif_list_iterator_t;

/**
 * Callback function for `foreach` loops.
 *
 * @param element the pointer to the current element.
 * @param udata   user-provided data.
 *
 * @return `false` to stop iterating, `true` to continue.
 */
typedef bool (*rif_list_foreach_fn_t)(void * element, void * udata);

/* Forward declaration of `rif_list_hooks_t`. */
typedef struct rif_list_hooks_s rif_list_hooks_t;

/**
 * The `rif_list_t` interface.
 * All rif list implementations inherit from this structure.
 */
typedef struct rif_list_s {

  /**
   * @private
   *
   * `rif_list_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * Hooks used by `rif_list_t` functions.
   */
  const rif_list_hooks_t * hooks;

} rif_list_t;

/**
 * Hooks used by `nuuk_list_t` implementations.
 */
struct rif_list_hooks_s {

  /**
   * Destroy a list, and release resources used by it.
   *
   * @param list the list to destroy.
   */
  void (*destroy)(rif_list_t *list_ptr);

  /**
   * @see rif_list_size
   */
  uint32_t (*size)(rif_list_t *list_ptr);

  /**
   * @see rif_list_get
   */
  rif_val_t *(*get)(rif_list_t *list_ptr, uint32_t index);

  /**
   * @see rif_list_insert
   */
  rif_status_t (*insert)(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr);

  /**
   * @see rif_list_append
   */
  rif_status_t (*append)(rif_list_t *list_ptr, rif_val_t *val_ptr);

  /**
   * @see rif_list_prepend
   */
  rif_status_t (*prepend)(rif_list_t *list_ptr, rif_val_t *val_ptr);

  /**
   * @see rif_list_set
   */
  rif_status_t (*set)(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr);

  /**
   * @see rif_list_remove
   */
  rif_status_t (*remove)(rif_list_t *list_ptr, uint32_t index);

  /* Iterator hooks */

  /**
   * @see rif_list_iterator_init
   */
  rif_list_iterator_t *(*iterator_init)(rif_list_t *list_ptr, rif_list_iterator_t *it_ptr);

  /**
   * @see rif_list_iterator_new
   */
  rif_list_iterator_t *(*iterator_new)(rif_list_t *list_ptr);

};

/******************************************************************************
 * HELPER MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_list_t`.
 *
 * @param  __val the `rif_val_t`.
 * @return       the casted `rif_list_t`.
 */
#define rif_list_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_LIST, rif_list_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a list.
 *
 * This function should only be used by `rif_list_t` subtypes.
 *
 * @param list_ptr the list to initialize.
 * @param hooks    the list hooks.
 * @param free     whether or not to free the value when the reference count reaches 0
 * @return         the initialized list, or `NULL` if initialization failed.
 *
 * @relates rif_list_t
 */
RIF_API
rif_list_t * rif_list_init(rif_list_t *list_ptr, const rif_list_hooks_t *hooks, bool free);

/******************************************************************************
 * INFO FUNCTIONS
 */

/**
 * Get the size of the list.
 *
 * @param al_ptr The list.
 * @return       The number of elements currently in the list.
 */
RIF_INLINE
uint32_t rif_list_size(rif_list_t *list_ptr) {
  return rif_hook(size, 0, list_ptr);
}

/******************************************************************************
 * ELEMENT READ FUNCTIONS
 */

/**
 * Returns the element at the specified position in this list.
 *
 * @param al_ptr the list
 * @param index  index of the element to return
 *
 * @return the element at the specified position in the list if it exists, or `NULL` otherwise.
 */
RIF_INLINE
rif_val_t * rif_list_get(rif_list_t *list_ptr, uint32_t index) {
  return rif_hook(get, NULL, list_ptr, index);
}

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

/**
 * Inserts the specified element at the specified position in this list. Shifts the element currently at that position
 * (if any) and any subsequent elements to the right (adds one to their indices).
 *
 * @param list_ptr the list
 * @param index    index at which the specified element is to be inserted
 * @param val_ptr  element to be inserted at the specified position
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is greater than the size of the list.
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the list implementation.
 */
RIF_INLINE
rif_status_t rif_list_insert(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr) {
  return rif_hook(insert, RIF_ERR_UNSUPPORTED, list_ptr, index, val_ptr);
}

/**
 * Appends the specified element to the end of the list.
 *
 * @param list_ptr the list
 * @param val_ptr  element to be appended to the end of the list
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the list implementation.
 */
RIF_INLINE
rif_status_t rif_list_append(rif_list_t *list_ptr, rif_val_t *val_ptr) {
  return rif_hook(append, RIF_ERR_UNSUPPORTED, list_ptr, val_ptr);
}

/**
 * Prepends the specified element to the beginning of the list. Shifts any subsequent elements to the right (adds one to
 * their indices).
 *
 * @param list_ptr the list
 * @param val_ptr  element to be prepended to the beginning of the list
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the list implementation.
 */
RIF_INLINE
rif_status_t rif_list_prepend(rif_list_t *list_ptr, rif_val_t *val_ptr) {
  return rif_hook(prepend, RIF_ERR_UNSUPPORTED, list_ptr, val_ptr);
}

/**
 * Replaces the element at the specified position in the list with the specified element.
 *
 * @param list_ptr the list
 * @param index    index of the element to replace
 * @param val_ptr  element to be stored at the specified position
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is equal or greater than the size of the list.
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the list implementation.
 */
RIF_INLINE
rif_status_t rif_list_set(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr) {
  return rif_hook(set, RIF_ERR_UNSUPPORTED, list_ptr, index, val_ptr);
}

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

/**
 * Removes the element at the specified position in this list. Shifts any subsequent elements to the left (subtracts one
 * from their indices). Returns the element that was removed from the list.
 *
 * @param list_ptr the list
 * @param index    the index of the element to be removed
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is equal or greater than the size of the list.
 *   - `RIF_ERR_UNSUPPORTED`   if this operation is not supported by the list implementation.
 */
RIF_INLINE
rif_status_t rif_list_remove(rif_list_t *list_ptr, uint32_t index) {
  return rif_hook(remove, RIF_ERR_UNSUPPORTED, list_ptr, index);
}

/******************************************************************************
 * ITERATOR FUNCTIONS
 */

/**
 * Initializes a heap-allocated list iterator.
 *
 * @param it_ptr   the iterator to initialize
 * @param list_ptr the list to iterate
 * @return         the initialized list iterator if successful, or `NULL` otherwise.
 */
RIF_INLINE
rif_list_iterator_t * rif_list_iterator_init(rif_list_iterator_t *it_ptr, rif_list_t *list_ptr) {
  return rif_hook(iterator_init, NULL, list_ptr, it_ptr);
}

/**
 * Creates a stack-allocated list iterator.
 *
 * @param list_ptr the list to iterate
 * @return         the initialized list iterator if successful, or `NULL` otherwise.
 */
RIF_INLINE
rif_list_iterator_t * rif_list_iterator_new(rif_list_t *list_ptr) {
  return rif_hook(iterator_new, NULL, list_ptr);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_list_t`.
 */
void rif_list_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_list_t`.
 */
uint32_t rif_list_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function to compare equality of two values.
 */
bool rif_list_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_list_t`.
 */
char * rif_list_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
