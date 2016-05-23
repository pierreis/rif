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
 * HOOK HELPERS
 */

static
void _rif_arraylist_hook_destroy(rif_list_t *list_ptr) {
  rif_arraylist_destroy_callback((rif_arraylist_t *) list_ptr);
}

static
uint32_t _rif_arraylist_hook_size(rif_list_t *list_ptr) {
  return rif_arraylist_size((rif_arraylist_t *) list_ptr);
}

static
rif_val_t * _rif_arraylist_hook_get(rif_list_t *list_ptr, uint32_t index) {
  return rif_arraylist_get((rif_arraylist_t *) list_ptr, index);
}

static
rif_status_t _rif_arraylist_hook_insert(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr) {
  return rif_arraylist_insert((rif_arraylist_t *) list_ptr, index, val_ptr);
}

static
rif_status_t _rif_arraylist_hook_append(rif_list_t *list_ptr, rif_val_t *val_ptr) {
  return rif_arraylist_append((rif_arraylist_t *) list_ptr, val_ptr);
}

static
rif_status_t _rif_arraylist_hook_prepend(rif_list_t *list_ptr, rif_val_t *val_ptr) {
  return rif_arraylist_prepend((rif_arraylist_t *) list_ptr, val_ptr);
}

static
rif_status_t _rif_arraylist_hook_set(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr) {
  return rif_arraylist_set((rif_arraylist_t *) list_ptr, index, val_ptr);
}

static
rif_status_t _rif_arraylist_hook_remove(rif_list_t *list_ptr, uint32_t index) {
  return rif_arraylist_remove((rif_arraylist_t *) list_ptr, index);
}

static
rif_list_iterator_t * _rif_arraylist_hook_iterator_init(rif_list_t *list_ptr, rif_list_iterator_t *it_ptr) {
  return (rif_list_iterator_t *) rif_arraylist_iterator_init(
      (rif_arraylist_iterator_t *) it_ptr, (rif_arraylist_t *) list_ptr);
}

static
rif_list_iterator_t * _rif_arraylist_hook_iterator_new(rif_list_t *list_ptr) {
  return (rif_list_iterator_t *) rif_arraylist_iterator_new((rif_arraylist_t *) list_ptr);
}

/******************************************************************************
 * HOOKS
 */

const rif_list_hooks_t rif_arraylist_hooks = {
    .destroy       = _rif_arraylist_hook_destroy,
    .size          = _rif_arraylist_hook_size,
    .get           = _rif_arraylist_hook_get,
    .insert        = _rif_arraylist_hook_insert,
    .append        = _rif_arraylist_hook_append,
    .prepend       = _rif_arraylist_hook_prepend,
    .set           = _rif_arraylist_hook_set,
    .remove        = _rif_arraylist_hook_remove,
    .iterator_init = _rif_arraylist_hook_iterator_init,
    .iterator_new  = _rif_arraylist_hook_iterator_new
};