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

#include "rif/rif_internal.h"

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_arraylist_t * _rif_arraylist_build(rif_arraylist_t *al_ptr, bool free, uint32_t capacity, uint32_t block_size) {
  if (!al_ptr) {
    return al_ptr;
  }
  rif_list_init((rif_list_t *) al_ptr, &rif_arraylist_hooks, free);
  al_ptr->block_size = block_size;
  al_ptr->capacity = 0;
  al_ptr->elements = NULL;
  al_ptr->free_elements = true;
  al_ptr->size = 0;

  // Allocate element array if needed.
  if (capacity && RIF_OK != rif_arraylist_ensure_capacity(al_ptr, capacity)) {
    return NULL;
  }

  return al_ptr;
}

rif_arraylist_t * rif_arraylist_init(rif_arraylist_t *al_ptr, uint32_t capacity, uint32_t block_size) {
  return _rif_arraylist_build(al_ptr, false, capacity, block_size);
}

/******************************************************************************
 * SIZING FUNCTIONS
 */

rif_status_t rif_arraylist_ensure_capacity(rif_arraylist_t *al_ptr, uint32_t capacity) {

  // Maybe we don't need to do anything.
  if (__likely(capacity <= al_ptr->capacity)) {
    return RIF_OK;
  }

  // Maybe the list has a fixed size. If so,
  if (al_ptr->block_size == 0 && al_ptr->elements) {
    return RIF_ERR_CAPACITY;
  }

  // Else, calculate how much space we need to allocate.
  uint32_t needed_capacity;
  if (al_ptr->block_size) {
    uint32_t needed_blocks = (uint32_t) ceil(capacity / (double) al_ptr->block_size);
    needed_capacity = needed_blocks * al_ptr->block_size;
  } else {
    needed_capacity = capacity;
  }
  uint32_t needed_bytes = needed_capacity * sizeof(rif_val_t *);

  // Reallocate memory.
  rif_val_t **new_elements;
  if (al_ptr->capacity) {
    new_elements = rif_realloc(al_ptr->elements, needed_bytes, "ARRAYLIST_CAPACITY_REALLOC");
  } else {
    new_elements = rif_malloc(needed_bytes, "ARRAYLIST_CAPACITY_ALLOC");
  }
  if (new_elements == NULL) {
    return RIF_ERR_MEMORY;
  }
  al_ptr->elements = new_elements;

  // Zero-out every element beyond original pointers.
  rif_val_t **zero_start = al_ptr->elements + al_ptr->capacity;
  uint32_t zero_bytes = needed_bytes - (al_ptr->capacity * sizeof(rif_val_t *));
  memset(zero_start, 0, zero_bytes);

  // Done.
  al_ptr->capacity = needed_capacity;
  return RIF_OK;
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_arraylist_destroy_callback(rif_arraylist_t *al_ptr) {
  uint32_t i = 0;
  for (; i < al_ptr->size; ++i) {
    rif_val_t **elem_ptr = al_ptr->elements + i;
    if (NULL != *elem_ptr) {
      rif_val_release(*elem_ptr);
    }
  }
  if (al_ptr->free_elements) {
    rif_free(al_ptr->elements);
  }
}

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

rif_status_t rif_arraylist_insert(rif_arraylist_t *al_ptr, uint32_t index, rif_val_t *val_ptr) {

  // Check index and ensure sufficient capacity.
  if (__unlikely(index > al_ptr->size)) {
    return RIF_ERR_OUT_OF_BOUNDS;
  }
  rif_status_t ensure_capacity_status = rif_arraylist_ensure_capacity(al_ptr, al_ptr->size + 1);
  if (__unlikely(RIF_OK != ensure_capacity_status)) {
    return ensure_capacity_status;
  }

  // Shift subsequent elements by one, if any.
  rif_val_t **elem_ptr = al_ptr->elements + index;
  if (index < al_ptr->size) {
    memmove(elem_ptr + 1, elem_ptr, sizeof(rif_val_t *) * (al_ptr->size - index));
  }

  // Retain the new value, and put it in place.
  if (__likely(NULL != val_ptr)) {
    rif_val_retain(val_ptr);
  }
  *elem_ptr = val_ptr;

  ++al_ptr->size;
  return RIF_OK;
}

rif_status_t rif_arraylist_set(rif_arraylist_t *al_ptr, uint32_t index, rif_val_t *val_ptr) {
  if (__unlikely(index >= al_ptr->size)) {
    return RIF_ERR_OUT_OF_BOUNDS;
  }
  rif_val_t **elem_ptr = al_ptr->elements + index;
  if (__likely(NULL != *elem_ptr)) {
    rif_val_release(*elem_ptr);
  }
  if (__likely(NULL != val_ptr)) {
    rif_val_retain(val_ptr);
  }
  *elem_ptr = val_ptr;
  return RIF_OK;
}

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

rif_status_t rif_arraylist_remove(rif_arraylist_t *al_ptr, uint32_t index) {

  // Check index.
  if (__unlikely(index >= al_ptr->size)) {
    return RIF_ERR_OUT_OF_BOUNDS;
  }

  // Release the value to remove, and overwrite it.
  rif_val_t **elem_ptr = al_ptr->elements + index;
  if (__likely(NULL != *elem_ptr)) {
    rif_val_release(*elem_ptr);
  }
  *elem_ptr = NULL;

  // Shift subsequent elements by one, if any.
  if (index < al_ptr->size - 1) {
    memmove(elem_ptr, elem_ptr + 1, sizeof(rif_val_t *) * (al_ptr->size - index - 1));
  }

  --al_ptr->size;
  return RIF_OK;
}