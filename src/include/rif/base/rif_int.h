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
 * @brief Rif integer type.
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
 * Rif integer type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_int_t` methods.
 */
typedef struct rif_int_s {

  /**
   * @private
   *
   * `rif_int_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * Integer value.
   */
  int64_t value;

} rif_int_t;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_int_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_int_t`.
 */
#define rif_int_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_INT, rif_int_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Allocates a new `rif_int_t`.
 *
 * @param value The integer value for the `rif_int_t`.
 * @return      The initialized `rif_int_t`.
 */
RIF_API
rif_int_t * rif_int_init(rif_int_t *int_ptr, int64_t value);

/**
 * Allocates a new `rif_int_t`.
 *
 * @param value The integer value for the new `rif_int_t`.
 * @return      The corresponding new `rif_int_t`, or `NULL` if memory allocation failed.
 */
RIF_API
rif_int_t * rif_int_new(int64_t value);

/**
 * Releases a `rif_int_t`. If the reference count reaches 0, the value will be freed.
 *
 * @param value The `rif_int_t` to release.
 */
RIF_INLINE
void rif_int_release(rif_int_t *int_ptr) {
  rif_val_release(int_ptr);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Get the integer value of a `rif_int_t`, or a fallback value if the `rif_int_t` pointer is `NULL`.
 *
 * @param int_ptr The `rif_int_t` to get the corresponding integer value for.
 * @return        The integer value, or `NULL` if `int_ptr` is `NULL`.
 */
RIF_INLINE
int64_t rif_int_getorelse(rif_int_t *int_ptr, double fallback) {
  return int_ptr ? int_ptr->value : fallback;
}

/**
 * Get the integer value of a `rif_int_t`.
 *
 * @param int_ptr The `rif_int_t` to get the corresponding integer value for.
 * @return        The integer value.
 */
RIF_INLINE
int64_t rif_int_get(rif_int_t *int_ptr) {
  return rif_int_getorelse(int_ptr, 0);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_int_t`.
 */
void rif_int_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_int_t`.
 */
uint32_t rif_int_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function to compare equality of two values.
 */
bool rif_int_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_int_t`.
 */
char * rif_int_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
