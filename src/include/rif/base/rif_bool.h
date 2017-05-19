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
 * @brief Boolean value type.
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
 * Boolean value type.
 *
 * @ref rif_bool_t is not intended to be initialized manually. Instead, it defines two static boolean instances,
 * @ref rif_true and @ref rif_false. These instances can be used or compared against directly.
 *
 * @note Internal members are private, and may change without notice.
 *       They should only be accessed through the public @ref rif_bool_t or @ref rif_val_t methods.
 *
 * @extends rif_val_t
 */
typedef struct rif_bool_t {

  /**
   * @private
   *
   * @ref rif_bool_t is a @ref rif_val_t subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * Boolean value.
   */
  const bool value;

  /**
   * @private
   *
   * Text representation.
   */
  const char string[6];

  /**
   * @private
   *
   * Hashcode.
   */
  const uint8_t hashcode;

} rif_bool_t;

/*****************************************************************************
 * GLOBAL CONSTANTS
 */

/**
 * True static @ref rif_bool_t instance.
 *
 * @relates rif_bool_t
 */
extern const rif_bool_t *rif_true;

/**
 * False @ref rif_bool_t instance.
 *
 * @relates rif_bool_t
 */
extern const rif_bool_t *rif_false;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a @ref rif_val_t to @ref rif_bool_t.
 *
 * @pre @a __val_ptr @b MUST be of type @ref rif_bool_t, or `NULL`.
 *
 * @param  __val the @ref rif_val_t.
 * @return       the casted @ref rif_bool_t.
 *
 * @relates rif_bool_t
 */
#define rif_bool_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_BOOL, rif_bool_t))

/******************************************************************************
 * API
 */

/**
 * Get the boolean value of a @ref rif_bool_t, or a fallback value if the @ref rif_bool_t pointer is `NULL`.
 *
 * @param bool_ptr the @ref rif_bool_t to get the corresponding boolean value for.
 * @return         the boolean value corresponding to @a dbl_ptr,
 *                 or @a fallback if @a bool_ptr is `NULL`.
 *
 * @public @memberof rif_bool_t
 */
RIF_INLINE
bool rif_bool_getorelse(rif_bool_t *bool_ptr, bool fallback) {
  return bool_ptr ? bool_ptr->value : fallback;
}

/**
 * Get the boolean value of a @ref rif_bool_t.
 *
 * @param bool_ptr the @ref rif_bool_t to get the corresponding boolean value for.
 * @return         the boolean value,
 *                 or `false` if @a bool_ptr is `NULL`.
 *
 * @public @memberof rif_bool_t
 */
RIF_INLINE
bool rif_bool_get(rif_bool_t *bool_ptr) {
  return rif_bool_getorelse(bool_ptr, false);
}

/******************************************************************************
 * VALUE CALLBACKS
 */

/**
 * @private
 *
 * Value hashcode callback for @ref rif_bool_t.
 *
 * @memberof rif_bool_t
 */
uint32_t rif_bool_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Value tostring callback for @ref rif_bool_t.
 *
 * @memberof rif_bool_t
 */
char * rif_bool_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
