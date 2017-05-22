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
 * HOOK HELPERS
 */

static
rif_val_t * _rif_linkedlist_iterator_hook_next(rif_iterator_t *it_ptr) {
  return rif_linkedlist_iterator_next((rif_linkedlist_iterator_t *) it_ptr);
}

static
bool _rif_linkedlist_iterator_hook_hasnext(rif_iterator_t *it_ptr) {
  return rif_linkedlist_iterator_hasnext((rif_linkedlist_iterator_t *) it_ptr);
}

/******************************************************************************
 * HOOKS
 */

const rif_iterator_hooks_t rif_linkedlist_iterator_hooks = {
    .destroy = NULL,
    .next    = _rif_linkedlist_iterator_hook_next,
    .hasnext = _rif_linkedlist_iterator_hook_hasnext
};