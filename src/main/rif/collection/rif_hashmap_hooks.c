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

#include "rif/collection/rif_hashmap.h"
#include "rif/collection/rif_hashmap_iterator.h"

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

static
rif_map_iterator_t * _rif_hashmap_hook_iterator_init(
    rif_map_t *map_ptr, rif_map_iterator_t *it_ptr, rif_pair_t *pair_ptr) {
  return (rif_map_iterator_t *) rif_hashmap_iterator_init(
      (rif_hashmap_iterator_t *) it_ptr, (rif_hashmap_t *) map_ptr, pair_ptr);
}

static
rif_map_iterator_t * _rif_hashmap_hook_iterator_new(rif_map_t *map_ptr) {
  return (rif_map_iterator_t *) rif_hashmap_iterator_new((rif_hashmap_t *) map_ptr);
}

/******************************************************************************
 * HOOKS
 */

const rif_map_hooks_t rif_hashmap_hooks = {
    .destroy       = _rif_hashmap_hook_destroy,
    .size          = _rif_hashmap_hook_size,
    .exists        = _rif_hashmap_hook_exists,
    .get           = _rif_hashmap_hook_get,
    .put           = _rif_hashmap_hook_put,
    .remove        = _rif_hashmap_hook_remove,
    .iterator_init = _rif_hashmap_hook_iterator_init,
    .iterator_new  = _rif_hashmap_hook_iterator_new
};