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

#include "rif/collection/rif_hashmap_iterator.h"

/******************************************************************************
 * TYPES
 */

typedef struct rif_hashmap_iterator_heap_s {

  rif_hashmap_iterator_t it;
  rif_pair_t pair;

} rif_hashmap_iterator_heap_t;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_hashmap_iterator_t * _rif_hashmap_iterator_build(
    rif_hashmap_iterator_t *it_ptr, const rif_hashmap_t *hm_ptr, rif_pair_t *pair_ptr, bool free) {
  if (!it_ptr) {
    return NULL;
  }
  rif_iterator_init((rif_iterator_t *) it_ptr, &rif_hashmap_iterator_hooks, free);
  it_ptr->hm_ptr = hm_ptr;
  it_ptr->index = 0;
  it_ptr->found = 0;
  it_ptr->pair_ptr = rif_pair_init(pair_ptr, NULL, NULL);
  return it_ptr;
}

rif_hashmap_iterator_t *rif_hashmap_iterator_init(
    rif_hashmap_iterator_t *it_ptr, const rif_hashmap_t *hm_ptr, rif_pair_t *pair_ptr) {
  return _rif_hashmap_iterator_build(it_ptr, hm_ptr, pair_ptr, false);
}

rif_hashmap_iterator_t *rif_hashmap_iterator_new(const rif_hashmap_t *hm_ptr) {
  rif_hashmap_iterator_heap_t *it_heap_ptr =
      rif_malloc(sizeof(rif_hashmap_iterator_heap_t), "RIF_HASHMAP_ITERATOR_NEW");
  return _rif_hashmap_iterator_build(&it_heap_ptr->it, hm_ptr, &it_heap_ptr->pair, true);
}

/******************************************************************************
 * ITERATOR FUNCTIONS
 */

rif_val_t * rif_hashmap_iterator_next(rif_hashmap_iterator_t *it_ptr) {
  if (!rif_hashmap_iterator_hasnext(it_ptr)) {
    return NULL;
  }
  rif_hashmap_element_t *cur = NULL;
  do {
    cur = rif_hashmap_atindex(it_ptr->hm_ptr, it_ptr->index++);
  } while(!cur);
  it_ptr->pair_ptr->val_ptr_1 = cur->val_ptr;
  it_ptr->pair_ptr->val_ptr_2 = cur->key_ptr;
  ++it_ptr->found;
  return rif_val(it_ptr->pair_ptr);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_hashmap_iterator_destroy_callback(rif_hashmap_iterator_t *it_ptr) {
  it_ptr->pair_ptr->val_ptr_1 = NULL;
  it_ptr->pair_ptr->val_ptr_2 = NULL;
  rif_val_release(it_ptr->pair_ptr);
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
