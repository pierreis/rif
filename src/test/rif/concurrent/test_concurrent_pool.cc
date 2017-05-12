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

#include <thread>

#include "../test_internal.h"

#include "../base/support/pool_conformity.hh"

/******************************************************************************
 * TEST HELPERS
 */

#define NUM_ELEMENTS 8
#define NUM_THREADS  8

static
bool _alloc_filter(const char *tag) {
  return 0 != strcmp(tag, "RIF_POOL_ALLOC");
}

/******************************************************************************
 * TEST CONFIG
 */

class ConcurrentPool : public MemoryAwareTest {

public:

  rif_concurrent_pool_t pool;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
    rif_concurrent_pool_init(&this->pool, sizeof(uint64_t));
  }

  virtual void TearDown() {
    rif_concurrent_pool_destroy(&this->pool);
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * TEST THREADS
 */

static
void _rif_concurrent_pool_test_worker(rif_concurrent_pool_t *pool) {
  for (uint32_t i = 0; i < 100; ++i) {
    uint32_t alloc = (uint32_t) rand() % 100 + 1;
    void *blocks[alloc];
    for (uint32_t j = 0; j < alloc; ++j) {
      blocks[j] = rif_concurrent_pool_borrow(pool);
      ASSERT_FALSE(NULL == blocks[j]);
    }
    for (uint32_t j = 0; j < alloc; ++j) {
      rif_concurrent_pool_return(pool, blocks[j]);
    }
  }
}

/******************************************************************************
 * TESTS
 */

TEST_F(ConcurrentPool, rif_paged_pool_init_should_work_concurrently) {
  std::thread threads[NUM_THREADS];
  for (uint32_t i = 0; i < NUM_THREADS; ++i) {
    threads[i] = std::thread(_rif_concurrent_pool_test_worker, &pool);
  }
  for (uint32_t i = 0; i < NUM_THREADS; ++i) {
    threads[i].join();
  }
}

/******************************************************************************
 * CONFORMITY
 */

static
rif_pool_t *_rif_pool_init() {
  rif_concurrent_pool_t *pool_ptr = (rif_concurrent_pool_t *) rif_malloc(sizeof(rif_concurrent_pool_t));
  return (rif_pool_t *) rif_concurrent_pool_init(pool_ptr, sizeof(uint32_t));
}

static
void _rif_pool_destroy(rif_pool_t *pool_ptr) {
  rif_pool_destroy(pool_ptr);
  rif_free(pool_ptr);
}

static rif_pool_conformity_generator_t rif_pool_generator = {
    .init = _rif_pool_init,
    .destroy = _rif_pool_destroy
};

INSTANTIATE_TEST_CASE_P(ConcurrentPool, PoolConformity, ::testing::Values(&rif_pool_generator));