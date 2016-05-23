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

rif_iterator_t * rif_iterator_init(rif_iterator_t *it_ptr, const rif_iterator_hooks_t *hooks, bool free) {
  it_ptr->hooks = hooks;
  it_ptr->free = free;
  return it_ptr;
}

void rif_iterator_destroy(rif_iterator_t *it_ptr) {
  assert(NULL != it_ptr);
  rif_hook(destroy, 0, it_ptr);
  if (it_ptr->free) {
    rif_free(it_ptr);
  }
}