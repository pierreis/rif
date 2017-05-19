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

#include "../../test_internal.h"

/******************************************************************************
 * TEST HELPERS
 */

typedef struct rif_list_conformity_generator_s {
  rif_list_t *(*init)();
  void (*destroy)(rif_list_t *);
} rif_list_conformity_generator_t;

class ListConformity : public ::testing::TestWithParam<rif_list_conformity_generator_s *> {

protected:

    rif_list_t *list_ptr;

private:

    virtual void SetUp() {
      list_ptr = GetParam()->init();
    }

    virtual void TearDown() {
      GetParam()->destroy(list_ptr);
    }

};