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

#define XXH_PRIVATE_API
#define XXH_NAMESPACE rif
#include "xxhash.h"

#include "rif/rif_internal.h"

/*****************************************************************************
 * SIMPLE HASH PRIMITIVES
 */

uint32_t rif_hash_64(uint64_t key) {
  return XXH32(&key, sizeof(uint64_t), 0);
}

uint32_t rif_hash_mix_32(uint32_t first, uint32_t second) {
  uint64_t comb = (uint64_t) first << 32 | second;
  return XXH32(&comb, sizeof(uint64_t), 0);
}