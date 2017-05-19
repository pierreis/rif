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
 * @brief Null value type.
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
 * Null value type.
 *
 * @ref rif_null_t is not intended to be initialized manually. Instead, it defines one singleton instance,
 * @ref rif_null. This singleton can be used or compared against directly.
 *
 * @note Internal members are private, and may change without notice.
 *       They should only be accessed through the public @ref rif_null_t or @ref rif_val_t methods.
 *
 * @extends rif_val_t
 */
typedef struct rif_null_t {

  /**
   * @private
   *
   * @ref rif_null_t is a @ref rif_val_t subtype.
   */
  rif_val_t _;

} rif_null_t;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a @ref rif_val_t to @ref rif_null_t.
 *
 * @pre @a __val_ptr @b MUST be of type @ref rif_null_t, or `NULL`.
 *
 * @param  __val the @ref rif_val_t.
 * @return       the casted @ref rif_null_t.
 *
 * @relates rif_null_t
 */
#define rif_null_fromval(__val_ptr) (rif_val_tosubtype(__val_ptr, RIF_NULL, rif_null_t))

/*****************************************************************************
 * GLOBAL CONSTANTS
 */

/**
 * Singleton @ref rif_null_t instance.
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
 * Value tostring callback for @ref rif_null_t.
 *
 * @memberof rif_null_t
 */
char * rif_null_tostring_callback(const rif_val_t *val_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
