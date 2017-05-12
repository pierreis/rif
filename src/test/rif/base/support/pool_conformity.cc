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

#include "pool_conformity.hh"

/******************************************************************************
 * TEST HELPERS
 */

#define NUM_ELEMENTS 10000

/******************************************************************************
 * TEST SIZE
 */

TEST_P(PoolConformity, pool_borrow_should_reuse_returned_blocks_at_some_point) {
  void *block = rif_pool_borrow(pool_ptr);
  ASSERT_FALSE(NULL == block);
  rif_pool_return(pool_ptr, block);
  void *test = NULL;
  uint32_t i = 0;
  while (test != block) {
    test = rif_pool_borrow(pool_ptr);
  }
  rif_pool_return(pool_ptr, block);
}

TEST_P(PoolConformity, pool_borrow_should_provide_high_item_count) {
  void *blocks[NUM_ELEMENTS];
  for (uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    blocks[i] = rif_pool_borrow(pool_ptr);
    ASSERT_FALSE(NULL == blocks[i]);
  }
  for (uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    rif_pool_return(pool_ptr, blocks[i]);
  }
  for (uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    blocks[i] = rif_pool_borrow(pool_ptr);
    ASSERT_FALSE(NULL == blocks[i]);
  }
  for (uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    rif_pool_return(pool_ptr, blocks[i]);
  }
}