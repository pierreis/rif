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
 * STATIC HELPERS
 */

static
void _rif_concurrent_pool_dtor(rif_concurrent_queue_base_node_t *node) {
  rif_free(node);
}

static
rif_concurrent_pool_node_t *_rif_concurrent_pool_alloc(rif_concurrent_pool_t *pool_ptr) {
  rif_concurrent_pool_node_t *node = rif_malloc(sizeof(rif_concurrent_pool_node_t) + pool_ptr->element_size,
                                                "RIF_CONCURRENT_POOL_ALLOC");
  if (__unlikely(!node)) {
    return node;
  }
  rif_concurrent_queue_base_node_init(&pool_ptr->free_queue, (rif_concurrent_queue_base_node_t *) node);
  return node;
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_concurrent_pool_t * rif_concurrent_pool_init(rif_concurrent_pool_t *pool_ptr, uint32_t element_size) {
  if (__unlikely(!pool_ptr)) {
    return pool_ptr;
  }
  if (__unlikely(NULL == rif_concurrent_queue_base_init(&pool_ptr->free_queue, _rif_concurrent_pool_dtor))) {
    return NULL;
  }
  pool_ptr->element_size = element_size;
  return pool_ptr;
}

void rif_concurrent_pool_destroy(rif_concurrent_pool_t *pool_ptr) {
  rif_concurrent_queue_base_destroy(&pool_ptr->free_queue);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

void * rif_concurrent_pool_borrow(rif_concurrent_pool_t *pool_ptr) {
  rif_concurrent_pool_node_t *node =
      (rif_concurrent_pool_node_t *) rif_concurrent_queue_base_pop(&pool_ptr->free_queue);
  if (__likely(NULL != node)) {
    return &node->element;
  }
  return _rif_concurrent_pool_alloc(pool_ptr);
}

void rif_concurrent_pool_return(rif_concurrent_pool_t *pool_ptr, void *ptr) {
  if (__unlikely(!ptr)) {
    return;
  }
  rif_concurrent_pool_node_t *node_ptr = ptr;
  --node_ptr; // Move back before the node header
  rif_concurrent_queue_base_push(&pool_ptr->free_queue, (rif_concurrent_queue_base_node_t *) node_ptr);
}