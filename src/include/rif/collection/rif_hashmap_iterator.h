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
 * @brief Rif hashmap iterator.
 */

#pragma once

#include "rif/collection/rif_hashmap.h"
#include "rif/collection/rif_iterator.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * The `rif_map_t` interface.
 * All rif map implementations inherit from this structure.
 */
typedef struct rif_hashmap_iterator_s {

  /**
   * @private
   *
   * `rif_hashmap_iterator_t` is a `rif_iterator_t` subtype.
   */
  rif_iterator_t _;

  /**
   * @private
   *
   * The map to iterate.
   */
  const rif_hashmap_t *hm_ptr;

  /**
   * @private
   *
   * The pair to use.
   */
  rif_pair_t *pair_ptr;

  /**
   * @private
   *
   * The current index.
   */
  uint32_t index;

  /**
   * @private
   *
   * How many elements have been returned so far.
   */
  uint32_t found;

} rif_hashmap_iterator_t;

/******************************************************************************
 * HOOKS
 */

/**
 * @private
 *
 * Arraymap iterator hooks.
 */
extern const rif_iterator_hooks_t rif_hashmap_iterator_hooks;

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initializes a heap-allocated hashmap iterator.
 *
 * @param it_ptr the iterator to initialize
 * @param hm_ptr the hashmap to iterate
 * @return       the initialized hashmap iterator if successful, or `NULL` otherwise.
 */
RIF_API
rif_hashmap_iterator_t * rif_hashmap_iterator_init(
    rif_hashmap_iterator_t *it_ptr, const rif_hashmap_t *hm_ptr, rif_pair_t *pair_ptr);

/**
 * Creates a stack-allocated hashmap iterator.
 *
 * @param hm_ptr the hashmap to iterate
 * @return       the initialized hashmap iterator if successful, or `NULL` otherwise.
 */
RIF_API
rif_hashmap_iterator_t * rif_hashmap_iterator_new(const rif_hashmap_t *hm_ptr);

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
rif_val_t * rif_hashmap_iterator_next(rif_hashmap_iterator_t *it_ptr);

/**
 * Returns `true` if the iteration has more elements.
 *
 * @param it_ptr the iterator
 * @return       `true` if the iteration has more elements.
 */
RIF_INLINE
bool rif_hashmap_iterator_hasnext(rif_hashmap_iterator_t *it_ptr) {
  return it_ptr->found < rif_hashmap_size(it_ptr->hm_ptr);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function to destroy a `rif_hashmap_iterator_t`.
 */
void rif_hashmap_iterator_destroy_callback(rif_hashmap_iterator_t *it_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
