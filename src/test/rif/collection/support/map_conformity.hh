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

#pragma once

#include "../../test_internal.h"

/******************************************************************************
 * TEST HELPERS
 */

typedef struct rif_map_conformity_generator_s {
  rif_map_t *(*init)();
  void (*destroy)(rif_map_t *);
} rif_map_conformity_generator_t;

class MapConformity : public ::testing::TestWithParam<rif_map_conformity_generator_s *> {

protected:

  rif_map_t *map_ptr;

  rif_int_t *ints[10];
  rif_string_t *strs[10];

private:

  virtual void SetUp() {
    map_ptr = GetParam()->init();
    char str[2] = "a";
    for (uint8_t i = 0; i < 10; ++i) {
      ints[i] = rif_int_new(i);
      strs[i] = rif_string_new_dup((char *) &str);
      ++str[0];
    }
  }

  virtual void TearDown() {
    GetParam()->destroy(map_ptr);
    for (uint8_t i = 0; i < 10; ++i) {
      rif_val_release(ints[i]);
      rif_val_release(strs[i]);
    }
  }

};