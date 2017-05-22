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
 * @brief Rif pair type.
 */

#pragma once

#include "rif/base/rif_val.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * Rif pair type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_pair_t` methods.
 *
 * @extends rif_val_t
 */
typedef struct rif_pair_s {

  /**
   * @private
   *
   * `rif_pair_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * First value.
   */
  rif_val_t *val_ptr_1;

  /**
   * @private
   *
   * Second value.
   */
  rif_val_t *val_ptr_2;

} rif_pair_t;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_pair_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_pair_t`.
 */
#define rif_pair_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_PAIR, rif_pair_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Allocates a new `rif_pair_t`.
 *
 * @param value The pair value for the `rif_pair_t`.
 * @return      The initialized `rif_pair_t`.
 */
RIF_API
rif_pair_t * rif_pair_init(rif_pair_t *pair_ptr, rif_val_t *val_ptr_1, rif_val_t *val_ptr_2);

/**
 * Allocates a new `rif_pair_t`.
 *
 * @param value The pair value for the new `rif_pair_t`.
 * @return      The corresponding new `rif_pair_t`, or `NULL` if memory allocation failed.
 */
RIF_API
rif_pair_t * rif_pair_new(rif_val_t *val_ptr_1, rif_val_t *val_ptr_2);

/**
 * Releases a `rif_pair_t`. If the reference count reaches 0, the value will be freed.
 *
 * @param value The `rif_pair_t` to release.
 */
RIF_INLINE
void rif_pair_release(rif_pair_t *pair_ptr) {
  rif_val_release(pair_ptr);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Get the first value of a `rif_pair_t`.
 *
 * @param pair_ptr The `rif_pair_t` to get the first value for.
 * @return         The first value of the `rif_pair_t`.
 */
RIF_INLINE
rif_val_t * rif_pair_1(const rif_pair_t *pair_ptr) {
  return pair_ptr ? pair_ptr->val_ptr_1 : NULL;
}

/**
 * Get the second value of a `rif_pair_t`.
 *
 * @param pair_ptr The `rif_pair_t` to get the second value for.
 * @return         The second value of the `rif_pair_t`.
 */
RIF_INLINE
rif_val_t * rif_pair_2(const rif_pair_t *pair_ptr) {
  return pair_ptr ? pair_ptr->val_ptr_2 : NULL;
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_pair_t`.
 */
void rif_pair_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_pair_t`.
 */
uint32_t rif_pair_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function to compare equality of two values.
 */
bool rif_pair_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_pair_t`.
 */
char * rif_pair_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
