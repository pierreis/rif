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
 * @brief Rif hash functions.
 */

#pragma once

#include "rif/rif_common.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * SIMPLE HASH PRIMITIVES
 */

/**
 * Hash a 64-bit integer to a 32-bit integer.
 *
 * @param key The integer value.
 * @return    The value hash.
 */
uint32_t rif_hash_64(uint64_t key);

/**
 * Mix two 32-bit integers into one.
 *
 * @param first  The first integer value.
 * @param second The second integer value.
 * @return       The mixed value.
 */
uint32_t rif_hash_mix_32(uint32_t first, uint32_t second);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
