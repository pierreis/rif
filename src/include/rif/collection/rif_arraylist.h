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
 * @brief Rif array list.
 */

#pragma once

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
 * Rif arraylist type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_arraylist_t` methods.
 */
typedef struct rif_arraylist_s {

  /**
   * @private
   *
   * `rif_arraylist_t` is a `rif_list_t` subtype.
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
   * Current allocated capacity of the list.
   */
  uint32_t capacity;

  /**
   * @private
   *
   * Block size of the list.
   */
  uint32_t block_size;

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
  rif_val_t ** elements;

} rif_arraylist_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Arraylist hooks.
 */
extern const rif_list_hooks_t rif_arraylist_hooks;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_arraylist_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_arraylist_t`.
 */
//#define rif_arraylist_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_STRING, rif_arraylist_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a heap-allocated arraylist.
 *
 * @param list         The list to initialize.
 * @param capacity     The initial capacity to allocate. If `0`, the storage will be allocated lazily.
 * @param block_size   The block size of the list, i.e. by how much elements the list will be extended when additional
 *                     capacity is needed. If `0`, the list will have a fixed-size of `capacity`.
 *
 * @return
 *   - `NUUK_ARRAYLIST_OK`        if the list has been initialized successfully.
 *   - `NUUK_ARRAYLIST_ERR_ALLOC` if memory couldn't be allocated.
 */
RIF_API
rif_arraylist_t * rif_arraylist_init(rif_arraylist_t *al_ptr, uint32_t capacity, uint32_t block_size);

/**
 * Initialize a stack-allocated list.
 *
 * Due to the stack allocation, the resulting list has a fixed size of `capacity`, and cannot be extended.
 *
 * @param list         The list to initialize.
 * @param element_size The size of a single element of the list in bytes.
 * @param __capacity   The initial capacity to allocate. If `0`, the storage will be allocated lazily.
 */
#define rif_arraylist_inita(__al_ptr, __capacity) \
  rif_arraylist_init((__al_ptr), 0, 0); \
  (__al_ptr)->free_elements = false; \
  (__al_ptr)->capacity = __capacity; \
  (__al_ptr)->elements = ((rif_val_t **) rif_alloca((__capacity) * sizeof(rif_val_t *)));

/**
 * Releases a `rif_arraylist_t`. If the reference count reaches 0, the value will be freed.
 *
 * @param value The `rif_arraylist_t` to release.
 */
RIF_INLINE
void rif_arraylist_release(rif_arraylist_t *al_ptr) {
  rif_val_release(al_ptr);
}

/******************************************************************************
 * SIZING FUNCTIONS
 */

/**
 * Ensures the list has enough allocated capacity to store at least `capacity` elements without reallocation.
 *
 * The list is resized if it does not have the required allocated capacity.
 *
 * @param al_ptr   The list.
 * @param capacity The desired minimum capacity.
 *
 * @return
 *   - `RIF_OK`           if the operation is successful.
 *   - `RIF_ERR_MEMORY`   if memory allocation failed.
 *   - `RIF_ERR_CAPACITY` if the list has a fixed capacity, and `capacity` is greater than the fixed capacity.
 */
RIF_API
rif_status_t rif_arraylist_ensure_capacity(rif_arraylist_t *al_ptr, uint32_t capacity);

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
uint32_t rif_arraylist_size(rif_arraylist_t *al_ptr) {
  return al_ptr->size;
}

/**
 * Returns the list allocated capacity.
 *
 * @param al_ptr The list.
 * @return       The allocated element capacity of the list.
 */
RIF_INLINE
uint32_t rif_arraylist_capacity(rif_arraylist_t *al_ptr) {
  return al_ptr->capacity;
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
rif_val_t * rif_arraylist_get(rif_arraylist_t *al_ptr, uint32_t index) {
  if (index >= al_ptr->size) {
    return NULL;
  }
  return *(al_ptr->elements + index);
}

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

/**
 * Inserts the specified element at the specified position in this list. Shifts the element currently at that position
 * (if any) and any subsequent elements to the right (adds one to their indices).
 *
 * @param al_ptr  the list
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
rif_status_t rif_arraylist_insert(rif_arraylist_t *al_ptr, uint32_t index, rif_val_t *val_ptr);

/**
 * Appends the specified element to the end of the list.
 *
 * @param al_ptr  the list
 * @param val_ptr element to be appended to the end of the list
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 */
RIF_INLINE
rif_status_t rif_arraylist_append(rif_arraylist_t *al_ptr, rif_val_t *val_ptr) {
  return rif_arraylist_insert(al_ptr, al_ptr->size, val_ptr);
}

/**
 * Prepends the specified element to the beginning of the list. Shifts any subsequent elements to the right (adds one to
 * their indices).
 *
 * @param al_ptr  the list
 * @param val_ptr element to be prepended to the beginning of the list
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_MEMORY`        if memory allocation failed.
 *   - `RIF_ERR_CAPACITY`      if the list has a fixed capacity, and `index` is greater than the fixed capacity.
 */
RIF_INLINE
rif_status_t rif_arraylist_prepend(rif_arraylist_t *al_ptr, rif_val_t *val_ptr) {
  return rif_arraylist_insert(al_ptr, 0, val_ptr);
}

/**
 * Replaces the element at the specified position in the list with the specified element.
 *
 * @param al_ptr  the list
 * @param index   index of the element to replace
 * @param val_ptr element to be stored at the specified position
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is equal or greater than the size of the list.
 */
RIF_API
rif_status_t rif_arraylist_set(rif_arraylist_t *al_ptr, uint32_t index, rif_val_t *val_ptr);

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

/**
 * Removes the element at the specified position in this list. Shifts any subsequent elements to the left (subtracts one
 * from their indices). Returns the element that was removed from the list.
 *
 * @param al_ptr  the list
 * @param index   the index of the element to be removed
 *
 * @return
 *   - `RIF_OK`                if the operation is successful.
 *   - `RIF_ERR_OUT_OF_BOUNDS` if `index` is equal or greater than the size of the list.
 */
RIF_API
rif_status_t rif_arraylist_remove(rif_arraylist_t *al_ptr, uint32_t index);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
