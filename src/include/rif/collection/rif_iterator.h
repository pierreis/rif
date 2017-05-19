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

/**
 * @file
 * @brief Rif iterator.
 */

#pragma once

#include "rif/base/rif_val.h"
#include "rif/util/rif_hook.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/* Forward declaration of `rif_iterator_hooks_t`. */
typedef struct rif_iterator_hooks_s rif_iterator_hooks_t;

/**
 * Rif iterator type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_iterator_t` methods.
 */
typedef struct rif_iterator_s {

  /**
   * @private
   *
   * Hooks used by `rif_list_t` functions.
   */
  const rif_iterator_hooks_t * hooks;

  /**
   * @private
   *
   * Whether or not to free the variable when the reference count reaches 0.
   */
  bool free;

} rif_iterator_t;

/**
 * Hooks used by `nuuk_list_t` implementations.
 */
struct rif_iterator_hooks_s {

  /**
   * @see rif_iterator_destroy
   */
  void (*destroy)(rif_iterator_t *it_ptr);

  /**
   * @see rif_iterator_next
   */
  rif_val_t *(*next)(rif_iterator_t *it_ptr);

  /**
   * @see rif_iterator_hasnext
   */
  bool (*hasnext)(rif_iterator_t *it_ptr);

};

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initializes an iterator.
 *
 * This function should only be used by `rif_list_t` subtypes.
 *
 * @param list_ptr the iterator to initialize.
 * @param hooks    the iterator hooks.
 * @param free     whether or not to free the iterator when the reference count reaches 0
 * @return         the initialized iterator, or `NULL` if initialization failed.
 */
RIF_API
rif_iterator_t * rif_iterator_init(rif_iterator_t *it_ptr, const rif_iterator_hooks_t *hooks, bool free);

/**
 * Destroys an iterator, and release resources used by it.
 *
 * @param it_ptr the iterator to destroy.
 */
RIF_API
void rif_iterator_destroy(rif_iterator_t *it_ptr);

/******************************************************************************
 * ITERATOR FUNCTIONS
 */

/**
 * Returns the next element in the iteration.
 *
 * @param it_ptr the iterator
 * @return       the next element in the iteration.
 */
RIF_INLINE
rif_val_t * rif_iterator_next(rif_iterator_t *it_ptr) {
  return rif_hook(next, NULL, it_ptr);
}

/**
 * Returns `true` if the iteration has more elements.
 *
 * @param it_ptr the iterator
 * @return       `true` if the iteration has more elements.
 */
RIF_INLINE
bool rif_iterator_hasnext(rif_iterator_t *it_ptr) {
  return rif_hook(hasnext, false, it_ptr);
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
