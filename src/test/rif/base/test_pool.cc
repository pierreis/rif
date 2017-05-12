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

#include "../test_internal.h"

/******************************************************************************
 * TEST HELPERS
 */

#define NUM_ELEMENTS 8

static
bool _alloc_filter(const char *tag) {
  return 0 != strcmp(tag, "RIF_POOL_ALLOC");
}

/******************************************************************************
 * TEST CONFIG
 */

class Pool : public MemoryAwareTest {

public:

  rif_pool_t pool;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
    rif_pool_init(&this->pool, NUM_ELEMENTS, sizeof(uint64_t), false);
  }

  virtual void TearDown() {
    rif_pool_destroy(&this->pool);
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * TESTS
 */

TEST_F(Pool, rif_pool_init_should_return_null_with_null_ptr) {
  ASSERT_TRUE(NULL == rif_pool_init(NULL, NUM_ELEMENTS, 8, false));
}

TEST_F(Pool, rif_pool_init_should_return_an_initialized_pool) {
  rif_pool_t pool_tmp;
  ASSERT_TRUE(&pool_tmp == rif_pool_init(&pool_tmp, NUM_ELEMENTS, 8, false));
  ASSERT_FALSE(pool_tmp.first_block == NULL);
  rif_pool_destroy(&pool_tmp);
}

TEST_F(Pool, rif_pool_init_should_return_an_initialized_pool_with_lazy_init) {
  rif_pool_t pool_tmp;
  ASSERT_TRUE(&pool_tmp == rif_pool_init(&pool_tmp, NUM_ELEMENTS, 8, true));
  ASSERT_TRUE(pool_tmp.first_block == NULL);
  rif_pool_destroy(&pool_tmp);
}

TEST_F(Pool, rif_pool_init_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter);
  rif_pool_t pool_tmp;
  ASSERT_TRUE(NULL == rif_pool_init(&pool_tmp, NUM_ELEMENTS, 8, false));
  rif_alloc_set_filter(NULL);
}

TEST_F(Pool, rif_pool_borrow_should_return_an_existing_pointer) {
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_pool_borrow(&this->pool);
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
  ASSERT_TRUE(NULL == this->pool.first_block->next);
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_pool_borrow(&this->pool);
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
}

TEST_F(Pool, rif_pool_return_should_release_a_pointer) {
  void *elements[NUM_ELEMENTS];
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_pool_borrow(&this->pool);
    elements[i] = element;
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_block->next);
  rif_pool_return(&this->pool, elements[0]);
  ASSERT_TRUE(elements[0] == this->pool.first_available);
  ASSERT_TRUE(elements[0] == this->pool.last_available);
  ASSERT_TRUE(NULL == this->pool.first_block->next);
  void *released = elements[0];
  elements[0] = rif_pool_borrow(&this->pool);
  ASSERT_TRUE(released == elements[0]);
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    rif_pool_return(&this->pool, elements[i]);
  }
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_pool_borrow(&this->pool);
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
  ASSERT_TRUE(NULL == this->pool.first_block->next);
}
