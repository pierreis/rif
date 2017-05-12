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
 * STATIC FUNCTIONS
 */

static
rif_pool_block_t * _rif_pool_block_alloc(rif_pool_t *pool_ptr) {
  rif_pool_block_t *block_ptr = rif_malloc(sizeof(rif_pool_block_t) + pool_ptr->element_size * pool_ptr->block_size,
                                           "RIF_POOL_ALLOC");
  if (!block_ptr) {
    return block_ptr;
  }

  // Register block
  block_ptr->next = pool_ptr->first_block;
  pool_ptr->first_block = block_ptr;

  // Initialize block
  char *cur = (char *) &block_ptr->elements;
  char *last = cur + (pool_ptr->element_size * (pool_ptr->block_size - 1));
  for (; cur <= last - pool_ptr->element_size; cur += pool_ptr->element_size) {
    void **cur_ptr = (void **) cur;
    *cur_ptr = cur + pool_ptr->element_size;
  }

  // Set first available
  void **last_ptr = (void **) last;
  *last_ptr = pool_ptr->first_available;
  pool_ptr->first_available = &block_ptr->elements;

  // Set last available, if necessary
  if (!pool_ptr->last_available) {
    pool_ptr->last_available = last;
  }

  return block_ptr;
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_pool_t * rif_pool_init(rif_pool_t *pool_ptr, uint32_t block_size, uint32_t element_size, bool lazy) {
  if (!pool_ptr) {
    return pool_ptr;
  }
  pool_ptr->block_size = block_size;
  pool_ptr->element_size = rif_max(element_size, sizeof(size_t));
  pool_ptr->first_available = NULL;
  pool_ptr->last_available = NULL;
  pool_ptr->first_block = NULL;
  if (!lazy && !_rif_pool_block_alloc(pool_ptr)) {
    return NULL;
  }
  return pool_ptr;
}

void rif_pool_destroy(rif_pool_t *pool_ptr) {
  rif_pool_block_t *cur = pool_ptr->first_block;
  while (cur) {
    rif_pool_block_t *next = cur->next;
    rif_free(cur);
    cur = next;
  }
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

void * rif_pool_borrow(rif_pool_t *pool_ptr) {
  if (!pool_ptr->first_available && !_rif_pool_block_alloc(pool_ptr)) {
    return NULL;
  }
  void *element = pool_ptr->first_available;
  void **element_ptr = (void **) element;
  pool_ptr->first_available = *element_ptr;
  if (element == pool_ptr->last_available) {
    pool_ptr->last_available = NULL;
  }
  return element;
}

void rif_pool_return(rif_pool_t *pool_ptr, void *ptr) {
  if (!ptr) {
    return;
  }
  void **element_ptr = (void **) ptr;
  *element_ptr = NULL;
  if (pool_ptr->last_available) {
    void **last_element_ptr = (void **) pool_ptr->last_available;
    *last_element_ptr = ptr;
  }
  pool_ptr->last_available = ptr;
  if (!pool_ptr->first_available) {
    pool_ptr->first_available = ptr;
  }
}