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
 * @brief Double value type.
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
 * Double value type.
 *
 * @note Internal members are private, and may change without notice.
 *       They should only be accessed through the public @ref rif_double_t or @ref rif_val_t methods.
 *
 * @extends rif_val_t
 */
typedef struct rif_double_t {

  /**
   * @private
   *
   * @ref rif_double_t is a @ref rif_val_t subtype.
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
 * Cast a @ref rif_val_t to @ref rif_double_t.
 *
 * @pre @a __val_ptr @b MUST be of type @ref rif_double_t, or `NULL`.
 *
 * @param  __val the @ref rif_val_t.
 * @return       the casted @ref rif_double_t.
 *
 * @relates rif_double_t
 */
#define rif_double_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_DOUBLE, rif_double_t))

/******************************************************************************
 * LIFECYCLE
 */

/**
 * Initialize a @ref rif_double_t.
 *
 * @param value The double value for the @ref rif_double_t.
 * @return      The initialized @ref rif_double_t,
 *              or null if initialization failed.
 *
 * @public @memberof rif_double_t
 */
RIF_API
rif_double_t * rif_double_init(rif_double_t *dbl_ptr, double value);

/**
 * Allocate a new @ref rif_double_t from the stack.
 *
 * @param value The double value for the new @ref rif_double_t.
 * @return      The initialized @ref rif_double_t,
 *              or null if allocation or initialization failed.
 *
 * @public @memberof rif_double_t
 */
RIF_API
rif_double_t * rif_double_new(double value);

/**
 * Release a @ref rif_double_t.
 *
 * Decrements the reference count of @a dbl_ptr by one.
 * If the reference count of @a dbl_ptr reaches `0` and the value is heap-allocated, it will be freed.
 *
 * @param value The @ref rif_double_t to release.
 *
 * @see rif_val_release
 * @public @memberof rif_double_t
 */
RIF_INLINE
void rif_double_release(rif_double_t *dbl_ptr) {
  rif_val_release(dbl_ptr);
}

/******************************************************************************
 * API
 */

/**
 * Get the double value of a @ref rif_double_t, or a fallback value if the @ref rif_double_t pointer is `NULL`.
 *
 * @param dbl_ptr the @ref rif_double_t to get the corresponding double value for.
 * @return        the double value corresponding to @a dbl_ptr,
 *                or @a fallback if @a dbl_ptr is `NULL`.
 *
 * @public @memberof rif_double_t
 */
RIF_INLINE
double rif_double_getorelse(rif_double_t *dbl_ptr, double fallback) {
  return dbl_ptr ? dbl_ptr->value : fallback;
}

/**
 * Get the bool value of a @ref rif_double_t.
 *
 * @param dbl_ptr the @ref rif_double_t to get the corresponding double value for.
 * @return        the double value,
 *                or `0.0` if @a dbl_ptr is `NULL`.
 *
 * @public @memberof rif_double_t
 */
RIF_INLINE
double rif_double_get(rif_double_t *dbl_ptr) {
  return rif_double_getorelse(dbl_ptr, 0.0);
}

/******************************************************************************
 * VALUE CALLBACKS
 */

/**
 * @private
 *
 * Value destroy callback for @ref rif_bool_t.
 *
 * @memberof rif_double_t
 */
void rif_double_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Value hashcode callback for @ref rif_bool_t.
 *
 * @memberof rif_double_t
 */
uint32_t rif_double_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Value equals callback for @ref rif_bool_t.
 *
 * @memberof rif_double_t
 */
bool rif_double_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Value tostring callback for @ref rif_bool_t.
 *
 * @memberof rif_double_t
 */
char * rif_double_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
