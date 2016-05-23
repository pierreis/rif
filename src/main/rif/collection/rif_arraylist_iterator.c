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
rif_arraylist_iterator_t *_rif_arraylist_iterator_build(
    rif_arraylist_iterator_t *it_ptr, const rif_arraylist_t *al_ptr, bool free) {
  if (!it_ptr) {
    return NULL;
  }
  rif_iterator_init((rif_iterator_t *) it_ptr, &rif_arraylist_iterator_hooks, free);
  it_ptr->al_ptr = al_ptr;
  it_ptr->index = 0;
  return it_ptr;
}

rif_arraylist_iterator_t *rif_arraylist_iterator_init(rif_arraylist_iterator_t *it_ptr, const rif_arraylist_t *al_ptr) {
  return _rif_arraylist_iterator_build(it_ptr, al_ptr, false);
}

rif_arraylist_iterator_t *rif_arraylist_iterator_new(const rif_arraylist_t *al_ptr) {
  rif_arraylist_iterator_t *it_ptr = rif_malloc(sizeof(rif_arraylist_iterator_t), "RIF_ARRAYLIST_ITERATOR_NEW");
  return _rif_arraylist_iterator_build(it_ptr, al_ptr, true);
}

/******************************************************************************
 * ITERATOR FUNCTIONS
 */

rif_val_t *rif_arraylist_iterator_next(rif_arraylist_iterator_t *it_ptr) {
  if (!rif_arraylist_iterator_hasnext(it_ptr)) {
    return NULL;
  }
  return *(it_ptr->al_ptr->elements + it_ptr->index++);
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
