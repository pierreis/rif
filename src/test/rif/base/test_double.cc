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
 * TEST FIXTURES
 */

static
void * _failing_malloc(size_t size) {
  return NULL;
}

/******************************************************************************
 * TEST CONFIG
 */

class Double : public MemoryAwareTest {

public:

  rif_double_t double_0;
  rif_double_t double_1;
  rif_double_t double_5;
  rif_double_t double_1337;

private:

  virtual void SetUp() {
    rif_double_init(&double_0, 0.0);
    rif_double_init(&double_1, 1.0);
    rif_double_init(&double_5, 5.0);
    rif_double_init(&double_1337, 13.37);
  }

  virtual void TearDown() {
    rif_double_release(&double_0);
    rif_double_release(&double_1);
    rif_double_release(&double_5);
    rif_double_release(&double_1337);
  }

};

/******************************************************************************
 * TESTS
 */

TEST_F(Double, rif_double_init_should_return_null_with_null_ptr) {
  ASSERT_TRUE(NULL == rif_double_init(NULL, 0.0));
}

TEST_F(Double, rif_double_new_should_return_an_initialized_double) {
  rif_double_t * double_ptr = rif_double_new(42.0);
  ASSERT_EQ(42.0, rif_double_get(double_ptr));
  rif_double_release(double_ptr);
}

TEST_F(Double, rif_double_should_have_increasing_or_decreasing_reference_count) {
  ASSERT_EQ(1, rif_val_reference_count(&double_0));
  rif_val_retain(&double_0);
  ASSERT_EQ(2, rif_val_reference_count(&double_0));
  rif_val_release(&double_0);
  ASSERT_EQ(1, rif_val_reference_count(&double_0));
}

TEST_F(Double, rif_double_get_should_return_value_assigned) {
  ASSERT_EQ(1.0, rif_double_get(&double_1));
  ASSERT_EQ(13.37, rif_double_get(&double_1337));
}

TEST_F(Double, rif_double_get_should_return_0_with_null_ptr) {
  ASSERT_EQ(0.0, rif_double_get(NULL));
}

TEST_F(Double, rif_double_getorelse_should_return_value_assigned) {
  ASSERT_EQ(1.0, rif_double_getorelse(&double_1, 5.0));
  ASSERT_EQ(13.37, rif_double_getorelse(&double_1337, 5.0));
}

TEST_F(Double, rif_double_getorelse_should_return_fallback_with_null_ptr) {
  ASSERT_EQ(13.37, rif_double_getorelse(NULL, 13.37));
}

TEST_F(Double, rif_double_should_have_meaningful_tostring) {
  EXPECT_STREQ("0.0", rif_val_tostring(&double_0));
  EXPECT_STREQ("1.0", rif_val_tostring(&double_1));
  EXPECT_STREQ("13.37", rif_val_tostring(&double_1337));
}

TEST_F(Double, rif_double_tostring_should_return_null_on_failing_alloc) {
  rif_set_allocators(_failing_malloc, NULL, NULL);
  EXPECT_TRUE(NULL == rif_val_tostring(&double_1337));
  rif_set_allocators(f_malloc, NULL, NULL);
}

TEST_F(Double, rif_double_should_have_value_dependent_hashcode) {
  EXPECT_NE(rif_val_hashcode(&double_0), rif_val_hashcode(&double_5));
  EXPECT_EQ(rif_val_hashcode(&double_1), rif_val_hashcode(&double_1));
}

TEST_F(Double, rif_double_should_be_equal_to_itself) {
  EXPECT_TRUE(rif_val_equals(&double_0, &double_0));
  EXPECT_TRUE(rif_val_equals(&double_1, &double_1));
  EXPECT_TRUE(rif_val_equals(&double_5, &double_5));
  EXPECT_TRUE(rif_val_equals(&double_1337, &double_1337));

  rif_double_t * double_ptr = rif_double_new(1.0);
  EXPECT_TRUE(rif_val_equals(&double_1, double_ptr));
  rif_double_release(double_ptr);
}