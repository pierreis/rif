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

#include "support/pool_conformity.hh"

/******************************************************************************
 * TEST HELPERS
 */

#define NUM_ELEMENTS 16

static
bool _alloc_filter(const char *tag) {
  return 0 != strcmp(tag, "RIF_POOL_ALLOC");
}

/******************************************************************************
 * TEST CONFIG
 */

class PagedPool : public MemoryAwareTest {

public:

  rif_paged_pool_t pool;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
    rif_paged_pool_init(&this->pool, sizeof(uint64_t), NUM_ELEMENTS, false);
  }

  virtual void TearDown() {
    rif_paged_pool_destroy(&this->pool);
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * TESTS
 */

TEST_F(PagedPool, rif_paged_pool_init_should_return_null_with_null_ptr) {
  ASSERT_TRUE(NULL == rif_paged_pool_init(NULL, 8, NUM_ELEMENTS, false));
}

TEST_F(PagedPool, rif_paged_pool_init_should_return_an_initialized_pool) {
  rif_paged_pool_t pool_tmp;
  ASSERT_TRUE(&pool_tmp == rif_paged_pool_init(&pool_tmp, 8, NUM_ELEMENTS, false));
  ASSERT_FALSE(pool_tmp.first_block == NULL);
  rif_paged_pool_destroy(&pool_tmp);
}

TEST_F(PagedPool, rif_paged_pool_init_should_return_an_initialized_pool_with_lazy_init) {
  rif_paged_pool_t pool_tmp;
  ASSERT_TRUE(&pool_tmp == rif_paged_pool_init(&pool_tmp, 8, NUM_ELEMENTS, true));
  ASSERT_TRUE(pool_tmp.first_block == NULL);
  rif_paged_pool_destroy(&pool_tmp);
}

TEST_F(PagedPool, rif_paged_pool_init_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter);
  rif_paged_pool_t pool_tmp;
  ASSERT_TRUE(NULL == rif_paged_pool_init(&pool_tmp, 8, NUM_ELEMENTS, false));
  rif_alloc_set_filter(NULL);
}

TEST_F(PagedPool, rif_paged_pool_borrow_should_return_an_existing_pointer) {
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_paged_pool_borrow(&this->pool);
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
  ASSERT_TRUE(NULL == this->pool.first_block->next);
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_paged_pool_borrow(&this->pool);
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
}

TEST_F(PagedPool, rif_paged_pool_return_should_release_a_pointer) {
  void *elements[NUM_ELEMENTS];
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_paged_pool_borrow(&this->pool);
    elements[i] = element;
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_block->next);
  rif_paged_pool_return(&this->pool, elements[0]);
  ASSERT_TRUE(elements[0] == this->pool.first_available);
  ASSERT_TRUE(elements[0] == this->pool.last_available);
  ASSERT_TRUE(NULL == this->pool.first_block->next);
  void *released = elements[0];
  elements[0] = rif_paged_pool_borrow(&this->pool);
  ASSERT_TRUE(released == elements[0]);
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    rif_paged_pool_return(&this->pool, elements[i]);
  }
  for(uint32_t i = 0; i < NUM_ELEMENTS; ++i) {
    void *element = rif_paged_pool_borrow(&this->pool);
    ASSERT_FALSE(element == NULL);
  }
  ASSERT_TRUE(NULL == this->pool.first_available);
  ASSERT_TRUE(NULL == this->pool.last_available);
  ASSERT_TRUE(NULL == this->pool.first_block->next);
}

/******************************************************************************
 * CONFORMITY
 */

static
rif_pool_t *_rif_pool_init() {
  rif_paged_pool_t *pool_ptr = (rif_paged_pool_t *) rif_malloc(sizeof(rif_paged_pool_t));
  return (rif_pool_t *) rif_paged_pool_init(pool_ptr, sizeof(uint32_t), NUM_ELEMENTS, false);
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

INSTANTIATE_TEST_CASE_P(PagedPool, PoolConformity, ::testing::Values(&rif_pool_generator));