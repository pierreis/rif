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
 * @brief Rif string type.
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
 * Rif string type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_string_t` methods.
 */
typedef struct rif_string_s {

  /**
   * @private
   *
   * `rif_string_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

  /**
   * @private
   *
   * String value.
   */
  char *value;

  /**
   * @private
   *
   * String length.
   */
  size_t len;

  /**
   * @private
   *
   * Whether or not to free the string upon destroy.
   */
  bool free;

} rif_string_t;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_string_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_string_t`.
 */
#define rif_string_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_STRING, rif_string_t))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Allocates a new `rif_string_t`.
 *
 * @param value The string value for the `rif_string_t`.
 * @return      The initialized `rif_string_t`.
 */
RIF_API
rif_string_t * rif_string_init_wlen(rif_string_t *str_ptr, char *value, size_t len, bool free);

/**
 * Allocates a new `rif_string_t`.
 *
 * @param value The string value for the `rif_string_t`.
 * @return      The initialized `rif_string_t`.
 */
RIF_INLINE
rif_string_t * rif_string_init(rif_string_t *str_ptr, char *value, bool free) {
  return rif_string_init_wlen(str_ptr, value, SIZE_MAX, free);
}

/**
 * Allocates a new `rif_string_t`.
 *
 * @param value The string value for the new `rif_string_t`.
 * @return      The corresponding new `rif_string_t`, or `NULL` if memory allocation failed.
 */
RIF_API
rif_string_t * rif_string_new_wlen(char *value, size_t len, bool free);

/**
 * Allocates a new `rif_string_t`.
 *
 * @param value The string value for the new `rif_string_t`.
 * @return      The corresponding new `rif_string_t`, or `NULL` if memory allocation failed.
 */
RIF_INLINE
rif_string_t * rif_string_new(char *value, bool free) {
  return rif_string_new_wlen(value, SIZE_MAX, free);
}

/**
 * Allocates a new `rif_string_t`.
 *
 * @param value The string value for the new `rif_string_t`.
 * @return      The corresponding new `rif_string_t`, or `NULL` if memory allocation failed.
 */
RIF_API
rif_string_t * rif_string_new_dup(const char *value);

/**
 * Releases a `rif_string_t`. If the reference count reaches 0, the value will be freed.
 *
 * @param value The `rif_string_t` to release.
 */
RIF_INLINE
void rif_string_release(rif_string_t *str_ptr) {
  rif_val_release(str_ptr);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Get the string value of a `rif_string_t`, or a fallback value if the `rif_string_t` pointer is `NULL`.
 *
 * @param str_ptr The `rif_string_t` to get the corresponding string value for.
 * @return           The string value, or `NULL` if `str_ptr` is `NULL`.
 */
RIF_INLINE
char * rif_string_getorelse(rif_string_t *str_ptr, char *fallback) {
  return str_ptr ? str_ptr->value : fallback;
}

/**
 * Get the string value of a `rif_string_t`.
 *
 * @param str_ptr The `rif_string_t` to get the corresponding string value for.
 * @return           The string value, or `0` if `str_ptr` is `NULL`.
 */
RIF_INLINE
char * rif_string_get(rif_string_t *str_ptr) {
  return rif_string_getorelse(str_ptr, NULL);
}

/**
 * Get the string value of a `rif_string_t`.
 *
 * @param str_ptr The `rif_string_t` to get the corresponding string value for.
 * @return           The string value, or `0` if `str_ptr` is `NULL`.
 */
RIF_API
size_t rif_string_len(rif_string_t *str_ptr);

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_string_t`.
 */
void rif_string_destroy_callback(rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function get the hashcode of a `rif_string_t`.
 */
uint32_t rif_string_hashcode_callback(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Callback function to compare equality of two values.
 */
bool rif_string_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Callback function get the string representation of a `rif_string_t`.
 */
char * rif_string_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
