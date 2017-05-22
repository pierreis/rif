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

#include "rif/collection/rif_linkedlist_iterator.h"

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_linkedlist_iterator_t *_rif_linkedlist_iterator_build(
    rif_linkedlist_iterator_t *it_ptr, const rif_linkedlist_t *ll_ptr, bool free) {
  if (!it_ptr) {
    return NULL;
  }
  rif_iterator_init((rif_iterator_t *) it_ptr, &rif_linkedlist_iterator_hooks, free);
  it_ptr->current = ll_ptr->first;
  return it_ptr;
}

rif_linkedlist_iterator_t *rif_linkedlist_iterator_init(
    rif_linkedlist_iterator_t *it_ptr, const rif_linkedlist_t *ll_ptr) {
  return _rif_linkedlist_iterator_build(it_ptr, ll_ptr, false);
}

rif_linkedlist_iterator_t *rif_linkedlist_iterator_new(const rif_linkedlist_t *ll_ptr) {
  rif_linkedlist_iterator_t *it_ptr = rif_malloc(sizeof(rif_linkedlist_iterator_t), "RIF_ARRAYLIST_ITERATOR_NEW");
  return _rif_linkedlist_iterator_build(it_ptr, ll_ptr, true);
}

/******************************************************************************
 * ITERATOR FUNCTIONS
 */

rif_val_t *rif_linkedlist_iterator_next(rif_linkedlist_iterator_t *it_ptr) {
  if (!rif_linkedlist_iterator_hasnext(it_ptr)) {
    return NULL;
  }
  rif_val_t *val_ptr = it_ptr->current->val;
  it_ptr->current = it_ptr->current->succ;
  return val_ptr;
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
