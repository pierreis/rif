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
#include "rif/collection/rif_linkedlist_iterator.h"

/******************************************************************************
 * HOOK HELPERS
 */

static
void _rif_linkedlist_hook_destroy(rif_list_t *list_ptr) {
  rif_linkedlist_destroy_callback((rif_linkedlist_t *) list_ptr);
}

static
uint32_t _rif_linkedlist_hook_size(rif_list_t *list_ptr) {
  return rif_linkedlist_size((rif_linkedlist_t *) list_ptr);
}

static
rif_val_t * _rif_linkedlist_hook_get(rif_list_t *list_ptr, uint32_t index) {
  return rif_linkedlist_get((rif_linkedlist_t *) list_ptr, index);
}

static
rif_status_t _rif_linkedlist_hook_insert(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr) {
  return rif_linkedlist_insert((rif_linkedlist_t *) list_ptr, index, val_ptr);
}

static
rif_status_t _rif_linkedlist_hook_append(rif_list_t *list_ptr, rif_val_t *val_ptr) {
  return rif_linkedlist_append((rif_linkedlist_t *) list_ptr, val_ptr);
}

static
rif_status_t _rif_linkedlist_hook_prepend(rif_list_t *list_ptr, rif_val_t *val_ptr) {
  return rif_linkedlist_prepend((rif_linkedlist_t *) list_ptr, val_ptr);
}

static
rif_status_t _rif_linkedlist_hook_set(rif_list_t *list_ptr, uint32_t index, rif_val_t *val_ptr) {
  return rif_linkedlist_set((rif_linkedlist_t *) list_ptr, index, val_ptr);
}

static
rif_status_t _rif_linkedlist_hook_remove(rif_list_t *list_ptr, uint32_t index) {
  return rif_linkedlist_remove((rif_linkedlist_t *) list_ptr, index);
}

static
rif_list_iterator_t * _rif_linkedlist_hook_iterator_init(rif_list_t *list_ptr, rif_list_iterator_t *it_ptr) {
  return (rif_list_iterator_t *) rif_linkedlist_iterator_init(
      (rif_linkedlist_iterator_t *) it_ptr, (rif_linkedlist_t *) list_ptr);
}

static
rif_list_iterator_t * _rif_linkedlist_hook_iterator_new(rif_list_t *list_ptr) {
  return (rif_list_iterator_t *) rif_linkedlist_iterator_new((rif_linkedlist_t *) list_ptr);
}

/******************************************************************************
 * HOOKS
 */

const rif_list_hooks_t rif_linkedlist_hooks = {
    .destroy       = _rif_linkedlist_hook_destroy,
    .size          = _rif_linkedlist_hook_size,
    .get           = _rif_linkedlist_hook_get,
    .insert        = _rif_linkedlist_hook_insert,
    .append        = _rif_linkedlist_hook_append,
    .prepend       = _rif_linkedlist_hook_prepend,
    .set           = _rif_linkedlist_hook_set,
    .remove        = _rif_linkedlist_hook_remove,
    .iterator_init = _rif_linkedlist_hook_iterator_init,
    .iterator_new  = _rif_linkedlist_hook_iterator_new
};