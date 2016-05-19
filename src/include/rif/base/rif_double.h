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
 * @brief Rif double type.
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
 * Rif double type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_double_t` methods.
 */
typedef struct rif_double_s {

  /**
   * @private
   *
   * `rif_double_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * Double value.
   */
  double value;

} rif_double_t;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_double_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_double_t`.
 */
#define rif_double_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_DOUBLE, rif_double_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Allocates a new `rif_double_t`.
 *
 * @param value The double value for the `rif_double_t`.
 * @return      The initialized `rif_double_t`.
 */
RIF_API
rif_double_t * rif_double_init(rif_double_t *double_ptr, double value);

/**
 * Allocates a new `rif_double_t`.
 *
 * @param value The double value for the new `rif_double_t`.
 * @return      The corresponding new `rif_double_t`, or `NULL` if memory allocation failed.
 */
RIF_API
rif_double_t * rif_double_new(double value);

/**
 * Releases a `rif_double_t`. If the reference count reaches 0, the value will be freed.
 *
 * @param value The `rif_double_t` to release.
 */
RIF_INLINE
void rif_double_release(rif_double_t *double_ptr) {
  rif_val_release(double_ptr);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Get the double value of a `rif_double_t`, or a fallback value if the `rif_double_t` pointer is `NULL`.
 *
 * @param double_ptr The `rif_double_t` to get the corresponding double value for.
 * @return           The double value, or `NULL` if `double_ptr` is `NULL`.
 */
RIF_INLINE
double rif_double_getorelse(rif_double_t *double_ptr, double fallback) {
  return double_ptr ? double_ptr->value : fallback;
}

/**
 * Get the double value of a `rif_double_t`.
 *
 * @param double_ptr The `rif_double_t` to get the corresponding double value for.
 * @return           The double value, or `0` if `double_ptr` is `NULL`.
 */
RIF_INLINE
double rif_double_get(rif_double_t *double_ptr) {
  return rif_double_getorelse(double_ptr, 0.0);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_double_t`.
 */
void rif_double_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_double_t`.
 */
uint32_t rif_double_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function to compare equality of two values.
 */
bool rif_double_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_double_t`.
 */
char * rif_double_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
