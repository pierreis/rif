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

#include "support/list_conformity.hh"

/******************************************************************************
 * TEST FIXTURES
 */

static
bool _alloc_filter_new(const char *tag) {
  return 0 != strcmp(tag, "RIF_LINKEDLIST_NEW");
}

static
bool _alloc_filter_new_node(const char *tag) {
  return 0 != strcmp(tag, "RIF_LINKEDLIST_NEW_NODE");
}

/******************************************************************************
 * TEST CONFIG
 */

class Linkedlist : public MemoryAwareTest {

public:

  rif_linkedlist_t ll_empty;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
    rif_linkedlist_init(&ll_empty);
  }

  virtual void TearDown() {
    rif_linkedlist_release(&ll_empty);
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * INIT TESTS
 */

TEST_F(Linkedlist, rif_linkedlist_init_should_return_null_with_null_ptr) {
  ASSERT_EQ(NULL, rif_linkedlist_init(NULL));
}

TEST_F(Linkedlist, rif_linkedlist_init_should_return_an_initialized_arraylist) {
  rif_linkedlist_t ll_ptr;
  ASSERT_TRUE(NULL != rif_linkedlist_init(&ll_ptr));
  EXPECT_EQ(0, ll_ptr.size);
  rif_linkedlist_release(&ll_ptr);
}

TEST_F(Linkedlist, rif_linkedlist_new_should_return_an_initialized_arraylist) {
  rif_linkedlist_t *ll_ptr = rif_linkedlist_new();
  ASSERT_TRUE(NULL != ll_ptr);
  EXPECT_EQ(0, ll_ptr->size);
  rif_linkedlist_release(ll_ptr);
}

TEST_F(Linkedlist, rif_linkedlist_new_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_new);
  ASSERT_EQ(NULL, rif_linkedlist_new());
  rif_alloc_set_filter(NULL);
}

/******************************************************************************
 * INSERT TESTS
 */

TEST_F(Linkedlist, rif_linkedlist_insert_should_return_memory_error_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_new_node);
  ASSERT_EQ(RIF_ERR_MEMORY, rif_linkedlist_insert(&ll_empty, 0, NULL));
  rif_alloc_set_filter(NULL);
}

/******************************************************************************
 * CONFORMITY
 */

static
rif_list_t *_rif_linkedlist_init() {
  rif_linkedlist_t *list_ptr = (rif_linkedlist_t *) rif_malloc(sizeof(rif_linkedlist_t));
  return (rif_list_t *) rif_linkedlist_init(list_ptr);
}

static
void _rif_linkedlist_destroy(rif_list_t *list_ptr) {
  rif_val_release(list_ptr);
  rif_free(list_ptr);
}

static rif_list_conformity_generator_t rif_linkedlist_generator = {
    .init = _rif_linkedlist_init,
    .destroy = _rif_linkedlist_destroy
};

INSTANTIATE_TEST_CASE_P(LinkedList, ListConformity, ::testing::Values(&rif_linkedlist_generator));