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

#pragma once

/*****************************************************************************/

#include "gtest/gtest.h"

#include "rif/rif.h"
#include "rif/rif_internal.h"

/******************************************************************************
 * ALLOC CHECKS
 */

class MemoryAwareTest : public testing::Test {

protected:

  void * (*f_malloc)(size_t);
  void * (*f_realloc)(void *, size_t);
  void (*f_free)(void *);

  rif_hashmap_t allocator_map;

public:

  MemoryAwareTest(void) {
    f_malloc = malloc;
    f_realloc = realloc;
    f_free = free;
  }

protected:

  virtual void SetUp() {
    rif_hashmap_init(&allocator_map, 32, false);
  }

  virtual void TearDown() {
    rif_hashmap_destroy_callback(&allocator_map);
  }

  void * MallocCatcher(size_t size) {
    return f_malloc(size);
  }

};

/******************************************************************************
 * MACROS
 */

#define RIF_EXPECT_TOSTRING(__expected, __func) \
    { \
      char *_str = (__func); \
      EXPECT_STREQ(__expected, _str); \
      rif_free(_str); \
    }
