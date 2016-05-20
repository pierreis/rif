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
 * STATIC FUNCTIONS
 */

static
bool _rif_alloc_filter_noop(const char * tag) {
  return true;
}

/******************************************************************************
 * GLOBAL VARIABLES
 */

struct rif_allocators_s _rif_allocators = {
    malloc,
    realloc,
    free
};

rif_alloc_filter_t _rif_alloc_filter = _rif_alloc_filter_noop;

/******************************************************************************
 * ALLOCATOR SETTER
 */

void rif_set_allocators(void * (*f_malloc)(size_t), void * (*f_realloc)(void *, size_t),
                                void (*f_free)(void *)) {
  _rif_allocators.f_malloc = f_malloc ? f_malloc : _rif_allocators.f_malloc;
  _rif_allocators.f_realloc = f_realloc ? f_realloc : _rif_allocators.f_realloc;
  _rif_allocators.f_free = f_free ? f_free : _rif_allocators.f_free;
}

void rif_alloc_set_filter(rif_alloc_filter_t f_filter) {
  if (!f_filter) {
    f_filter = _rif_alloc_filter_noop;
  }
  _rif_alloc_filter = f_filter;
}