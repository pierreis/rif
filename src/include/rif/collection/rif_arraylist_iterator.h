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

/**
 * @file
 * @brief Rif arraylist iterator.
 */

#pragma once

#include "rif/collection/rif_arraylist.h"
#include "rif/collection/rif_iterator.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * The `rif_list_t` interface.
 * All rif list implementations inherit from this structure.
 */
typedef struct rif_arraylist_iterator_s {

  /**
   * @private
   *
   * `rif_arraylist_iterator_t` is a `rif_iterator_t` subtype.
   */
  rif_iterator_t _;

  /**
   * @private
   *
   * The list to iterate.
   */
  const rif_arraylist_t *al_ptr;

  /**
   * @private
   *
   * The current index.
   */
  uint32_t index;

} rif_arraylist_iterator_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Arraylist iterator hooks.
 */
extern const rif_iterator_hooks_t rif_arraylist_iterator_hooks;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initializes a heap-allocated arraylist iterator.
 *
 * @param it_ptr the iterator to initialize
 * @param al_ptr the arraylist to iterate
 * @return       the initialized arraylist iterator if successful, or `NULL` otherwise.
 */
RIF_API
rif_arraylist_iterator_t *rif_arraylist_iterator_init(rif_arraylist_iterator_t *it_ptr, const rif_arraylist_t *al_ptr);

/**
 * Creates a stack-allocated arraylist iterator.
 *
 * @param al_ptr the arraylist to iterate
 * @return       the initialized arraylist iterator if successful, or `NULL` otherwise.
 */
RIF_API
rif_arraylist_iterator_t *rif_arraylist_iterator_new(const rif_arraylist_t *al_ptr);

/******************************************************************************
 * ITERATOR FUNCTIONS
 */

/**
 * Returns the next element in the iteration.
 *
 * @param it_ptr the iterator
 * @return       the next element in the iteration.
 */
RIF_API
rif_val_t *rif_arraylist_iterator_next(rif_arraylist_iterator_t *it_ptr);

/**
 * Returns `true` if the iteration has more elements.
 *
 * @param it_ptr the iterator
 * @return       `true` if the iteration has more elements.
 */
RIF_INLINE
bool rif_arraylist_iterator_hasnext(rif_arraylist_iterator_t *it_ptr) {
  return it_ptr->index < rif_arraylist_size(it_ptr->al_ptr);
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
