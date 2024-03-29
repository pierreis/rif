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
 * HOOK HELPERS
 */

static
void _rif_hashmap_iterator_hook_destroy(rif_iterator_t *it_ptr) {
  return rif_hashmap_iterator_destroy_callback((rif_hashmap_iterator_t *) it_ptr);
}

static
rif_val_t * _rif_hashmap_iterator_hook_next(rif_iterator_t *it_ptr) {
  return rif_hashmap_iterator_next((rif_hashmap_iterator_t *) it_ptr);
}

static
bool _rif_hashmap_iterator_hook_hasnext(rif_iterator_t *it_ptr) {
  return rif_hashmap_iterator_hasnext((rif_hashmap_iterator_t *) it_ptr);
}

/******************************************************************************
 * HOOKS
 */

const rif_iterator_hooks_t rif_hashmap_iterator_hooks = {
    .destroy = _rif_hashmap_iterator_hook_destroy,
    .next    = _rif_hashmap_iterator_hook_next,
    .hasnext = _rif_hashmap_iterator_hook_hasnext
};