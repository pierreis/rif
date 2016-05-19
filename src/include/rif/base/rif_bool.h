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
 * @brief Rif bool type.
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
 * Rif bool type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_bool_t` methods.
 */
typedef struct rif_bool_s {

  /**
   * @private
   *
   * `rif_bool_t` is a `rif_val_t` subtype.
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
 * True singleton.
 *
 * @relates rif_bool_t
 */
extern const rif_bool_t *rif_true;

/**
 * False singleton.
 *
 * @relates rif_bool_t
 */
extern const rif_bool_t *rif_false;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_bool_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_bool_t`.
 */
#define rif_bool_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_BOOL, rif_bool_t))

/******************************************************************************
 * BOOL FUNCTIONS
 */

/**
 * Get the bool value of a `rif_bool_t`, or a fallback value if the `rif_bool_t` pointer is `NULL`.
 *
 * @param bool_ptr The `rif_bool_t` to get the corresponding boolean value for.
 * @return         The boolean value, or `NULL` if `bool_ptr` is `NULL`.
 */
RIF_INLINE
bool rif_bool_getorelse(rif_bool_t *bool_ptr, bool fallback) {
  return bool_ptr ? bool_ptr->value : fallback;
}

/**
 * Get the bool value of a `rif_bool_t`.
 *
 * @param bool_ptr The `rif_bool_t` to get the corresponding boolean value for.
 * @return         The boolean value, or `false` if `bool_ptr` is `NULL`.
 */
RIF_INLINE
bool rif_bool_get(rif_bool_t *bool_ptr) {
  return rif_bool_getorelse(bool_ptr, false);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_bool_t`.
 */
uint32_t rif_bool_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_bool_t`.
 *
 * @relates rif_bool_t
 */
char * rif_bool_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
