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

/******************************************************************************
 * TEST HELPERS
 */

static
bool _alloc_filter_int_tostring(const char *tag) {
  return 0 != strcmp(tag, "RIF_INT_TOSTRING");
}

/******************************************************************************
 * TEST CONFIG
 */

class Int : public MemoryAwareTest {

public:

  rif_int_t int_0;
  rif_int_t int_1;
  rif_int_t int_5;
  rif_int_t int_1337;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
    rif_int_init(&int_0, 0);
    rif_int_init(&int_1, 1);
    rif_int_init(&int_5, 5);
    rif_int_init(&int_1337, 1337);
  }

  virtual void TearDown() {
    rif_int_release(&int_0);
    rif_int_release(&int_1);
    rif_int_release(&int_5);
    rif_int_release(&int_1337);
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * TESTS
 */

TEST_F(Int, rif_int_init_should_return_null_with_null_ptr) {
  ASSERT_TRUE(NULL == rif_int_init(NULL, 0));
}

TEST_F(Int, rif_int_new_should_return_an_initialized_int) {
  rif_int_t * int_ptr = rif_int_new(42);
  ASSERT_EQ(42, rif_int_get(int_ptr));
  rif_int_release(int_ptr);
}

TEST_F(Int, rif_int_should_have_increasing_or_decreasing_reference_count) {
  ASSERT_EQ(1, rif_val_reference_count(&int_0));
  rif_val_retain(&int_0);
  ASSERT_EQ(2, rif_val_reference_count(&int_0));
  rif_val_release(&int_0);
  ASSERT_EQ(1, rif_val_reference_count(&int_0));
}

TEST_F(Int, rif_int_get_should_return_value_assigned) {
  ASSERT_EQ(1, rif_int_get(&int_1));
  ASSERT_EQ(1337, rif_int_get(&int_1337));
}

TEST_F(Int, rif_int_get_should_return_0_with_null_ptr) {
  ASSERT_EQ(0, rif_int_get(NULL));
}

TEST_F(Int, rif_int_getorelse_should_return_value_assigned) {
  ASSERT_EQ(1, rif_int_getorelse(&int_1, 5));
  ASSERT_EQ(1337, rif_int_getorelse(&int_1337, 5));
}

TEST_F(Int, rif_int_getorelse_should_return_fallback_with_null_ptr) {
  ASSERT_EQ(1337, rif_int_getorelse(NULL, 1337));
}

TEST_F(Int, rif_int_should_have_meaningful_tostring) {
  RIF_EXPECT_TOSTRING("0", rif_val_tostring(&int_0));
  RIF_EXPECT_TOSTRING("1", rif_val_tostring(&int_1));
  RIF_EXPECT_TOSTRING("1337", rif_val_tostring(&int_1337));
}

TEST_F(Int, rif_int_tostring_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_int_tostring);
  EXPECT_TRUE(NULL == rif_val_tostring(&int_1337));
  rif_alloc_set_filter(NULL);
}

TEST_F(Int, rif_int_should_have_value_dependent_hashcode) {
  EXPECT_NE(rif_val_hashcode(&int_0), rif_val_hashcode(&int_5));
  EXPECT_EQ(rif_val_hashcode(&int_1), rif_val_hashcode(&int_1));
}

TEST_F(Int, rif_int_should_be_equal_to_itself) {
  EXPECT_TRUE(rif_val_equals(&int_0, &int_0));
  EXPECT_TRUE(rif_val_equals(&int_1, &int_1));
  EXPECT_TRUE(rif_val_equals(&int_5, &int_5));
  EXPECT_TRUE(rif_val_equals(&int_1337, &int_1337));

  rif_int_t * int_ptr = rif_int_new(1);
  EXPECT_TRUE(rif_val_equals(&int_1, int_ptr));
  rif_int_release(int_ptr);
}