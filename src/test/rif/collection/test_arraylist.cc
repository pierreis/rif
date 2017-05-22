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

#include "../test_internal.h"

#include "support/list_conformity.hh"

/******************************************************************************
 * TEST FIXTURES
 */

static
bool _alloc_filter_capacity_alloc(const char *tag) {
  return 0 != strcmp(tag, "ARRAYLIST_CAPACITY_ALLOC");
}

static
bool _alloc_filter_capacity_realloc(const char *tag) {
  return 0 != strcmp(tag, "ARRAYLIST_CAPACITY_REALLOC");
}

/******************************************************************************
 * TEST CONFIG
 */

class Arraylist : public MemoryAwareTest {

public:

  rif_arraylist_t al_empty;
  rif_arraylist_t al_empty_fixed;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
    rif_arraylist_init(&al_empty, 8, 8);
    rif_arraylist_init(&al_empty_fixed, 8, 0);
  }

  virtual void TearDown() {
    rif_arraylist_release(&al_empty);
    rif_arraylist_release(&al_empty_fixed);
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * INIT TESTS
 */

TEST_F(Arraylist, rif_arraylist_init_should_return_null_with_null_ptr) {
  ASSERT_EQ(NULL, rif_arraylist_init(NULL, 8, 8));
}

TEST_F(Arraylist, rif_arraylist_init_should_return_an_initialized_arraylist) {
  rif_arraylist_t al_ptr;
  ASSERT_TRUE(NULL != rif_arraylist_init(&al_ptr, 8, 8));
  EXPECT_EQ(al_ptr.capacity, 8);
  rif_arraylist_release(&al_ptr);
}

TEST_F(Arraylist, rif_arraylist_init_should_not_allocate_memory_with_null_capacity) {
  rif_arraylist_t al;
  ASSERT_TRUE(NULL != rif_arraylist_init(&al, 0, 8));
  EXPECT_EQ(NULL, al.elements);
  EXPECT_EQ(0, al.capacity);
  rif_arraylist_release(&al);
}

TEST_F(Arraylist, rif_arraylist_init_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_capacity_alloc);
  rif_arraylist_t al;
  ASSERT_TRUE(NULL == rif_arraylist_init(&al, 8, 8));
  rif_alloc_set_filter(NULL);
}

TEST_F(Arraylist, rif_arraylist_inita_should_return_an_initialized_arraylist) {
  rif_arraylist_t al_ptr;
  rif_arraylist_inita(&al_ptr, 8);
  EXPECT_EQ(al_ptr.capacity, 8);
  rif_arraylist_release(&al_ptr);
}

/******************************************************************************
 * CAPACITY TESTS
 */

TEST_F(Arraylist, rif_arraylist_ensure_capacity_should_increase_capacity) {
  ASSERT_EQ(RIF_OK, rif_arraylist_ensure_capacity(&al_empty, 16));
  EXPECT_EQ(16, rif_arraylist_capacity(&al_empty));
}

TEST_F(Arraylist, rif_arraylist_ensure_capacity_should_not_increase_capacity_if_desired_capacity_is_zero) {
  ASSERT_EQ(RIF_OK, rif_arraylist_ensure_capacity(&al_empty, 0));
  EXPECT_EQ(8, rif_arraylist_capacity(&al_empty));
}

TEST_F(Arraylist, rif_arraylist_ensure_capacity_should_round_to_the_upper_block_size_multiple) {
  ASSERT_EQ(RIF_OK, rif_arraylist_ensure_capacity(&al_empty, 9));
  EXPECT_EQ(16, rif_arraylist_capacity(&al_empty));
}

TEST_F(Arraylist, rif_arraylist_ensure_capacity_should_not_increase_capacity_of_a_fixed_size_arraylist) {
  ASSERT_EQ(RIF_ERR_CAPACITY, rif_arraylist_ensure_capacity(&al_empty_fixed, 16));
  EXPECT_EQ(8, rif_arraylist_capacity(&al_empty_fixed));
}

TEST_F(Arraylist, rif_arraylist_ensure_capacity_should_handle_failing_realloc) {
  rif_alloc_set_filter(_alloc_filter_capacity_realloc);
  ASSERT_EQ(RIF_ERR_MEMORY, rif_arraylist_ensure_capacity(&al_empty, 16));
  EXPECT_EQ(8, rif_arraylist_capacity(&al_empty));
  rif_alloc_set_filter(NULL);
}

TEST_F(Arraylist, rif_arraylist_insert_should_increase_capacity_if_needed) {
  for (uint8_t n = 0; n < 128; ++n) {
    EXPECT_EQ(RIF_OK, rif_arraylist_insert(&al_empty, 0, NULL));
  }
}

TEST_F(Arraylist, rif_arraylist_insert_should_handle_failing_realloc) {
  for (uint8_t n = 0; n < 128; ++n) {
    EXPECT_EQ(RIF_OK, rif_arraylist_insert(&al_empty, 0, NULL));
  }
  rif_alloc_set_filter(_alloc_filter_capacity_realloc);
  ASSERT_EQ(RIF_ERR_MEMORY, rif_arraylist_insert(&al_empty, 0, NULL));
  rif_alloc_set_filter(NULL);
}

/******************************************************************************
 * CONFORMITY
 */

static
rif_list_t *_rif_arraylist_init() {
  rif_arraylist_t *list_ptr = (rif_arraylist_t *) rif_malloc(sizeof(rif_arraylist_t));
  return (rif_list_t *) rif_arraylist_init(list_ptr, 8, 8);
}

static
void _rif_arraylist_destroy(rif_list_t *list_ptr) {
  rif_val_release(list_ptr);
  rif_free(list_ptr);
}

static rif_list_conformity_generator_t rif_arraylist_generator = {
    .init = _rif_arraylist_init,
    .destroy = _rif_arraylist_destroy
};

INSTANTIATE_TEST_CASE_P(Arraylist, ListConformity, ::testing::Values(&rif_arraylist_generator));