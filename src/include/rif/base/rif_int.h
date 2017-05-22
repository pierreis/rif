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
 * @brief Integer value type.
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
 *       They should only be accessed through the public @ref rif_int_t or @ref rif_val_t methods.
 *
 * @extends rif_val_t
 */
typedef struct rif_int_t {

  /**
   * @private
   *
   * @ref rif_int_t is a @ref rif_val_t subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * Double value.
   */
  int64_t value;

} rif_int_t;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a @ref rif_val_t to @ref rif_int_t.
 *
 * @pre @a __val_ptr @b MUST be of type @ref rif_int_t, or `NULL`.
 *
 * @param  __val the @ref rif_val_t.
 * @return       the casted @ref rif_int_t.
 *
 * @relates rif_int_t
 */
#define rif_int_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_INT, rif_int_t))

/******************************************************************************
 * LIFECYCLE
 */

/**
 * Initialize a @ref rif_int_t.
 *
 * @param value The integer value for the @ref rif_int_t.
 * @return      The initialized @ref rif_int_t,
 *              or null if initialization failed.
 *
 * @public @memberof rif_int_t
 */
RIF_API
rif_int_t * rif_int_init(rif_int_t *int_ptr, int64_t value);

/**
 * Allocate a new @ref rif_int_t from the stack.
 *
 * @param value The integer value for the new @ref rif_int_t.
 * @return      The initialized @ref rif_int_t,
 *              or null if allocation or initialization failed.
 *
 * @public @memberof rif_int_t
 */
RIF_API
rif_int_t * rif_int_new(int64_t value);

/**
 * Release a @ref rif_int_t.
 *
 * Decrements the reference count of @a int_ptr by one.
 * If the reference count of @a int_ptr reaches `0` and the value is heap-allocated, it will be freed.
 *
 * @param value The @ref rif_int_t to release.
 *
 * @see rif_val_release
 * @public @memberof rif_int_t
 */
RIF_INLINE
void rif_int_release(rif_int_t *int_ptr) {
  rif_val_release(int_ptr);
}

/******************************************************************************
 * API
 */

/**
 * Get the integer value of a @ref rif_int_t, or a fallback value if the @ref rif_int_t pointer is `NULL`.
 *
 * @param int_ptr the @ref rif_int_t to get the corresponding integer value for.
 * @return        the integer value corresponding to @a int_ptr,
 *                or @a fallback if @a int_ptr is `NULL`.
 *
 * @public @memberof rif_int_t
 */
RIF_INLINE
int64_t rif_int_getorelse(rif_int_t *int_ptr, int64_t fallback) {
  return int_ptr ? int_ptr->value : fallback;
}

/**
 * Get the bool value of a @ref rif_int_t.
 *
 * @param int_ptr the @ref rif_int_t to get the corresponding integer value for.
 * @return        the integer value,
 *                or `0.0` if @a int_ptr is `NULL`.
 *
 * @public @memberof rif_int_t
 */
RIF_INLINE
int64_t rif_int_get(rif_int_t *int_ptr) {
  return rif_int_getorelse(int_ptr, 0.0);
}

/******************************************************************************
 * VALUE CALLBACKS
 */

/**
 * @private
 *
 * Value destroy callback for @ref rif_bool_t.
 *
 * @memberof rif_int_t
 */
void rif_int_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Value hashcode callback for @ref rif_bool_t.
 *
 * @memberof rif_int_t
 */
uint32_t rif_int_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Value equals callback for @ref rif_bool_t.
 *
 * @memberof rif_int_t
 */
bool rif_int_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Value tostring callback for @ref rif_bool_t.
 *
 * @memberof rif_int_t
 */
char * rif_int_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
