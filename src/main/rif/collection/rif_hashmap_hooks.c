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
void _rif_hashmap_hook_destroy(rif_map_t *map_ptr) {
  rif_hashmap_destroy_callback((rif_hashmap_t *) map_ptr);
}

static
uint32_t _rif_hashmap_hook_size(rif_map_t *map_ptr) {
  return rif_hashmap_size((rif_hashmap_t *) map_ptr);
}

static
bool _rif_hashmap_hook_exists(rif_map_t *map_ptr, const rif_val_t *key_ptr) {
  return rif_hashmap_exists((rif_hashmap_t *) map_ptr, key_ptr);
}

static
rif_val_t * _rif_hashmap_hook_get(rif_map_t *map_ptr, const rif_val_t *key_ptr) {
  return rif_hashmap_get((rif_hashmap_t *) map_ptr, key_ptr);
}

static
rif_status_t _rif_hashmap_hook_put(rif_map_t *map_ptr, rif_val_t *key_ptr, rif_val_t *val_ptr) {
  return rif_hashmap_put((rif_hashmap_t *) map_ptr, key_ptr, val_ptr);
}

static
rif_status_t _rif_hashmap_hook_remove(rif_map_t *map_ptr, rif_val_t *key_ptr) {
  return rif_hashmap_remove((rif_hashmap_t *) map_ptr, key_ptr);
}

/*

static
rif_status_t _rif_hashmap_hook_prepend(rif_map_t *map_ptr, rif_val_t *val_ptr) {
  return rif_hashmap_prepend((rif_hashmap_t *) map_ptr, val_ptr);
}

static
rif_status_t _rif_hashmap_hook_set(rif_map_t *map_ptr, uint32_t index, rif_val_t *val_ptr) {
  return rif_hashmap_set((rif_hashmap_t *) map_ptr, index, val_ptr);
}

static
rif_status_t _rif_hashmap_hook_remove(rif_map_t *map_ptr, uint32_t index) {
  return rif_hashmap_remove((rif_hashmap_t *) map_ptr, index);
}

static
rif_map_iterator_t * _rif_hashmap_hook_iterator_init(rif_map_t *map_ptr, rif_map_iterator_t *it_ptr) {
  return (rif_map_iterator_t *) rif_hashmap_iterator_init(
      (rif_hashmap_iterator_t *) it_ptr, (rif_hashmap_t *) map_ptr);
}

static
rif_map_iterator_t * _rif_hashmap_hook_iterator_new(rif_map_t *map_ptr) {
  return (rif_map_iterator_t *) rif_hashmap_iterator_new((rif_hashmap_t *) map_ptr);
}
*/

/******************************************************************************
 * HOOKS
 */

const rif_map_hooks_t rif_hashmap_hooks = {
    .destroy       = _rif_hashmap_hook_destroy,
    .size          = _rif_hashmap_hook_size,
    .exists        = _rif_hashmap_hook_exists,
    .get           = _rif_hashmap_hook_get,
    .put           = _rif_hashmap_hook_put,
    .remove        = _rif_hashmap_hook_remove

    /*
    .size          = _rif_hashmap_hook_size,
    .get           = _rif_hashmap_hook_get,
    .insert        = _rif_hashmap_hook_insert,
    .append        = _rif_hashmap_hook_append,
    .prepend       = _rif_hashmap_hook_prepend,
    .set           = _rif_hashmap_hook_set,
    .remove        = _rif_hashmap_hook_remove,
    .iterator_init = _rif_hashmap_hook_iterator_init,
    .iterator_new  = _rif_hashmap_hook_iterator_new
     */
};