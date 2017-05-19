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

#include "rif/rif_internal.h"

#include "rif/concurrent/collection/rif_concurrent_queue.h"

/******************************************************************************
 * STATIC FUNCTIONS
 */

static
void _rif_concurrent_queue_node_destroy_cb(rif_concurrent_queue_base_node_t *node_ptr, void *udata) {
  rif_concurrent_pool_return(&((rif_concurrent_queue_t *) udata)->pool, node_ptr);
}

static
void _rif_concurrent_queue_node_add_cb(rif_concurrent_queue_base_t *queue_ptr,
                                       rif_concurrent_queue_base_node_t *node_ptr, void *udata) {
  atomic_fetch_add_explicit(&((rif_concurrent_queue_t *) udata)->size, 1, memory_order_relaxed);
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_concurrent_queue_t * rif_concurrent_queue_init(rif_concurrent_queue_t *queue_ptr) {
  if (__unlikely(!queue_ptr)) {
    return queue_ptr;
  }
  rif_queue_init((rif_queue_t *) queue_ptr, &rif_concurrent_queue_hooks, false);
  if (__unlikely(!rif_concurrent_pool_init(&queue_ptr->pool, sizeof(rif_concurrent_queue_node_t)))) {
    return NULL;
  }
  if (__unlikely(!rif_concurrent_queue_base_init(&queue_ptr->queue_base, _rif_concurrent_queue_node_add_cb,
                                                 _rif_concurrent_queue_node_destroy_cb, queue_ptr))) {
    rif_concurrent_pool_destroy(&queue_ptr->pool);
    return NULL;
  }
  atomic_init(&queue_ptr->size, 0);
  return queue_ptr;
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

rif_status_t rif_concurrent_queue_push(rif_concurrent_queue_t *queue_ptr, rif_val_t *val_ptr) {
  rif_concurrent_queue_node_t *node = rif_concurrent_pool_borrow(&queue_ptr->pool);
  if (__unlikely(!node)) {
    return RIF_ERR_MEMORY;
  }
  rif_concurrent_queue_base_node_init((rif_concurrent_queue_base_t *) &queue_ptr,
                                      (rif_concurrent_queue_base_node_t *) node);
  node->val = val_ptr;
  rif_val_retain(val_ptr);
  rif_concurrent_queue_base_push(&queue_ptr->queue_base, (rif_concurrent_queue_base_node_t *) node);
  return RIF_OK;
}

rif_val_t * rif_concurrent_queue_pop(rif_concurrent_queue_t *queue_ptr) {
  rif_concurrent_queue_node_t *node =
      (rif_concurrent_queue_node_t *) rif_concurrent_queue_base_pop(&queue_ptr->queue_base);
  if (!node) {
    return NULL;
  }
  rif_val_t *val_ptr = node->val;
  rif_concurrent_pool_return(&queue_ptr->pool, node);
  atomic_fetch_add_explicit(&queue_ptr->size, -1, memory_order_relaxed);
  return val_ptr;
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
