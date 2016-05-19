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
 * @brief Rif null type.
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
 * Rif null type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_null_t` methods.
 */
typedef struct rif_null_s {

  /**
   * @private
   *
   * `rif_null_t` is a `rif_val_t` subtype.
   */
  rif_val_t _;

} rif_null_t;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a `rif_val_t` to `rif_null_t`.
 *
 * @param  __val The `rif_val_t`.
 * @return       The casted `rif_null_t`.
 */
#define rif_null_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_NULL, rif_null_t))

/*****************************************************************************
 * GLOBAL CONSTANTS
 */

/**
 * Null singleton.
 *
 * @relates rif_null_t
 */
extern const rif_null_t *rif_null;

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function get the string representation of a `rif_null_t`.
 *
 * @relates rif_null_t
 */
char * rif_null_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
