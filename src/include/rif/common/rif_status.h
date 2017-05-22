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
 * @brief Rif operation status.
 */

#pragma once

#include "rif/base/rif_val.h"
#include "rif/common/rif_status.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * Rif status codes
 */
typedef enum rif_status_t {
  RIF_OK,
  RIF_ERR_CAPACITY,
  RIF_ERR_MEMORY,
  RIF_ERR_OUT_OF_BOUNDS,
  RIF_ERR_UNSUPPORTED
} rif_status_t;

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif