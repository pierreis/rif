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

#pragma once

#include "rif/util/rif_alloc.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

struct rif_allocators_s {

  void * (*f_malloc)(size_t);
  void * (*f_realloc)(void *, size_t);
  void (*f_free)(void *);

};

typedef bool (*rif_alloc_filter_t)(const char* tag);

/******************************************************************************
 * GLOBAL VARIABLES
 */

extern struct rif_allocators_s _rif_allocators;
extern rif_alloc_filter_t _rif_alloc_filter;

/******************************************************************************
 * ALLOCATOR MACROS
 */

#define rif_tagged_malloc(__size, __tag, ...) rif_malloc_helper(__size, __tag);
#define rif_malloc(...) rif_tagged_malloc(__VA_ARGS__, NULL);

#define rif_tagged_realloc(__ptr, __size, __tag, ...) rif_realloc_helper(__ptr, __size, __tag);
#define rif_realloc(...) rif_tagged_realloc(__VA_ARGS__, NULL);

#define rif_tagged_calloc(__count, __size, __tag, ...) rif_calloc_helper(__count, __size, __tag);
#define rif_calloc(...) rif_tagged_calloc(__VA_ARGS__, NULL);

#define rif_tagged_strdup(__str, __tag, ...) rif_strdup_helper(__str, __tag);
#define rif_strdup(...) rif_tagged_strdup(__VA_ARGS__, NULL);

/******************************************************************************
 * ALLOCATOR ACCESSORS
 */

RIF_INLINE
void * rif_malloc_helper(size_t size, const char *tag) {

#ifndef NDEBUG
  if (tag && !_rif_alloc_filter(tag)) {
    return NULL;
  }
#endif

  return _rif_allocators.f_malloc(size);
}

RIF_INLINE
void * rif_realloc_helper(void *ptr, size_t size, const char *tag) {

#ifndef NDEBUG
  if (tag && !_rif_alloc_filter(tag)) {
    return NULL;
  }
#endif

  return _rif_allocators.f_realloc(ptr, size);
}

RIF_INLINE
void rif_free(void *ptr) {
  _rif_allocators.f_free(ptr);
}

RIF_INLINE
void * rif_calloc_helper(size_t count, size_t size, const char *tag) {

#ifndef NDEBUG
  if (tag && !_rif_alloc_filter(tag)) {
    return NULL;
  }
#endif

  void *ptr = rif_malloc(count * size);
  if (!ptr) {
    return ptr;
  }
  memset(ptr, 0, count *size);
  return ptr;
}

RIF_INLINE
char * rif_strdup_helper(const char *str, const char *tag) {
  size_t length = strlen(str) + 1;
  char *rv = (char *) rif_malloc(length, tag);
  if (!rv) {
    return rv;
  }
  memcpy(rv, str, length);
  return rv;
}

#define rif_alloca(__size) (alloca(__size))

/******************************************************************************
 * ALLOCATOR FILTER
 */

void rif_alloc_set_filter(rif_alloc_filter_t f_filter);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
