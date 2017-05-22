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

#include "rif/collection/rif_linkedlist.h"

/******************************************************************************
 * CONFIG
 */

#define POOL_SIZE 32

/******************************************************************************
 * INTERNAL HELPER FUNCTIONS
 */

static inline
rif_linkedlist_node_t * _rif_linkedlist_node_select_atindex_asc(const rif_linkedlist_t *ll_ptr, uint32_t index) {
  rif_linkedlist_node_t *node_ptr = ll_ptr->first;
  while (index > 0 && NULL != node_ptr) {
    --index;
    node_ptr = node_ptr->succ;
  }
  return node_ptr;
}

static inline
rif_linkedlist_node_t * _rif_linkedlist_node_select_atindex_desc(const rif_linkedlist_t *ll_ptr, uint32_t index) {
  rif_linkedlist_node_t *node_ptr = ll_ptr->last;
  index = ll_ptr->size - index - 1;
  while (index > 0 && NULL != node_ptr) {
    --index;
    node_ptr = node_ptr->pred;
  }
  return node_ptr;
}

static inline
rif_linkedlist_node_t * _rif_linkedlist_node_select_atindex(const rif_linkedlist_t *ll_ptr, uint32_t index) {
  return index < ll_ptr->size / 2 ?
         _rif_linkedlist_node_select_atindex_asc(ll_ptr, index) :
         _rif_linkedlist_node_select_atindex_desc(ll_ptr, index);
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_linkedlist_t * rif_linkedlist_build(rif_linkedlist_t *ll_ptr, bool free) {
  if (!ll_ptr) {
    return ll_ptr;
  }
  rif_list_init((rif_list_t *) ll_ptr, &rif_linkedlist_hooks, free);
  ll_ptr->first = NULL;
  ll_ptr->last = NULL;
  ll_ptr->size = 0;
  if (!rif_paged_pool_init(&ll_ptr->pool, sizeof(rif_linkedlist_node_t), POOL_SIZE, true)) {
    rif_val_release(ll_ptr);
    return NULL;
  }
  return ll_ptr;
}

rif_linkedlist_t * rif_linkedlist_init(rif_linkedlist_t *ll_ptr) {
  return rif_linkedlist_build(ll_ptr, false);
}

rif_linkedlist_t * rif_linkedlist_new() {
  rif_linkedlist_t *ll_ptr = rif_malloc(sizeof(rif_linkedlist_t), "RIF_LINKEDLIST_NEW");
  return rif_linkedlist_build(ll_ptr, true);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_linkedlist_destroy_callback(rif_linkedlist_t *ll_ptr) {
  rif_linkedlist_node_t *node_ptr = ll_ptr->first;
  while (NULL != node_ptr) {
    rif_linkedlist_node_t *next_ptr = node_ptr->succ;
    rif_val_release(node_ptr->val);
    node_ptr = next_ptr;
  }
  rif_paged_pool_destroy(&ll_ptr->pool);
}

/******************************************************************************
 * ELEMENT READ FUNCTIONS
 */

rif_val_t * rif_linkedlist_get(const rif_linkedlist_t *ll_ptr, uint32_t index) {
  rif_linkedlist_node_t *node_ptr = _rif_linkedlist_node_select_atindex(ll_ptr, index);
  return NULL != node_ptr ? node_ptr->val : NULL;
}

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

rif_status_t rif_linkedlist_insert(rif_linkedlist_t *ll_ptr, uint32_t index, rif_val_t *val_ptr) {

  // Find the element place in the list.
  rif_linkedlist_node_t *pred = NULL, *succ = NULL;
  if (index == 0) {
    succ = ll_ptr->first;
  } else if (index == ll_ptr->size) {
    pred = ll_ptr->last;
  } else if (index < ll_ptr->size) {
    rif_linkedlist_node_t *current = _rif_linkedlist_node_select_atindex(ll_ptr, index);
    succ = current;
    pred = current->pred;
  } else {
    return RIF_ERR_OUT_OF_BOUNDS;
  }

  // Allocate a new node.
  rif_linkedlist_node_t *new_node_ptr = rif_paged_pool_borrow(&ll_ptr->pool);
  if (NULL == new_node_ptr) {
    return RIF_ERR_MEMORY;
  }
  rif_val_retain(val_ptr);
  new_node_ptr->val = val_ptr;
  new_node_ptr->pred = pred;
  new_node_ptr->succ = succ;

  // Place the new node.
  if (NULL != pred) {
    pred->succ = new_node_ptr;
  } else {
    ll_ptr->first = new_node_ptr;
  }
  if (NULL != succ) {
    succ->pred = new_node_ptr;
  } else {
    ll_ptr->last = new_node_ptr;
  }

  ++ll_ptr->size;
  return RIF_OK;
}

rif_status_t rif_linkedlist_set(rif_linkedlist_t *ll_ptr, uint32_t index, rif_val_t *val_ptr) {
  if (index >= ll_ptr->size) {
    return RIF_ERR_OUT_OF_BOUNDS;
  }
  rif_linkedlist_node_t *node_ptr = _rif_linkedlist_node_select_atindex(ll_ptr, index);
  rif_val_release(node_ptr->val);
  rif_val_retain(val_ptr);
  node_ptr->val = val_ptr;
  return RIF_OK;
}

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

rif_status_t rif_linkedlist_remove(rif_linkedlist_t *ll_ptr, uint32_t index) {

  // Check index.
  if (index >= ll_ptr->size) {
    return RIF_ERR_OUT_OF_BOUNDS;
  }

  // Find the element in the list.
  rif_linkedlist_node_t *current = _rif_linkedlist_node_select_atindex(ll_ptr, index);

  // Unreference it from the list.
  if (NULL != current->pred) {
    current->pred->succ = current->succ;
  } else {
    ll_ptr->first = current->succ;
  }
  if (NULL != current->succ) {
    current->succ->pred = current->pred;
  } else {
    ll_ptr->last = current->pred;
  }

  // Free it.
  rif_val_release(current->val);
  rif_paged_pool_return(&ll_ptr->pool, current);

  --ll_ptr->size;
  return RIF_OK;
}