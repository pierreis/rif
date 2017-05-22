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
bool _alloc_filter_pair_tostring(const char *tag) {
  return 0 != strcmp(tag, "RIF_PAIR_TOSTRING");
}

static
bool _alloc_filter_int_tostring(const char *tag) {
  return 0 != strcmp(tag, "RIF_INT_TOSTRING");
}

/******************************************************************************
 * TEST CONFIG
 */

class Pair : public MemoryAwareTest {

public:

  rif_int_t int_0;
  rif_int_t int_1;
  rif_int_t int_5;

  rif_pair_t pair_0_1;
  rif_pair_t pair_0_5;
  rif_pair_t pair_0_N;
  rif_pair_t pair_N_N;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();

    rif_int_init(&int_0, 0);
    rif_int_init(&int_1, 1);
    rif_int_init(&int_5, 5);

    rif_pair_init(&pair_0_1, rif_val(&int_0), rif_val(&int_1));
    rif_pair_init(&pair_0_5, rif_val(&int_0), rif_val(&int_5));
    rif_pair_init(&pair_0_N, rif_val(&int_0), NULL);
    rif_pair_init(&pair_N_N, NULL, NULL);
  }

  virtual void TearDown() {
    rif_int_release(&int_0);
    rif_int_release(&int_1);
    rif_int_release(&int_5);

    rif_pair_release(&pair_0_1);
    rif_pair_release(&pair_0_5);
    rif_pair_release(&pair_0_N);
    rif_pair_release(&pair_N_N);

    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * TESTS
 */

TEST_F(Pair, rif_pair_init_should_return_null_with_null_ptr) {
  ASSERT_TRUE(NULL == rif_pair_init(NULL, rif_val(&int_0), rif_val(&int_1)));
}

TEST_F(Pair, rif_pair_should_have_increasing_or_decreasing_reference_count) {
  ASSERT_EQ(1, rif_val_reference_count(&pair_0_1));
  rif_val_retain(&pair_0_1);
  ASSERT_EQ(2, rif_val_reference_count(&pair_0_1));
  rif_val_release(&pair_0_1);
  ASSERT_EQ(1, rif_val_reference_count(&pair_0_1));
}

TEST_F(Pair, rif_pair_new_should_return_an_initialized_pair) {
  rif_pair_t * pair_ptr = rif_pair_new(rif_val(&int_5), rif_val(&int_1));
  ASSERT_EQ(rif_val(&int_5), rif_pair_1(pair_ptr));
  ASSERT_EQ(rif_val(&int_1), rif_pair_2(pair_ptr));
  rif_pair_release(pair_ptr);
}

TEST_F(Pair, rif_pair_accessors_should_return_null_on_null_pair) {
  ASSERT_TRUE(NULL == rif_pair_1(NULL));
  ASSERT_TRUE(NULL == rif_pair_2(NULL));
}

TEST_F(Pair, rif_pair_hashcode_should_be_value_dependent) {
  EXPECT_NE(rif_val_hashcode(&pair_0_1), rif_val_hashcode(&pair_0_5));
  EXPECT_EQ(rif_val_hashcode(&pair_0_1), rif_val_hashcode(&pair_0_1));

  rif_pair_t * pair_ptr = rif_pair_new(rif_val(&int_0), rif_val(&int_1));
  EXPECT_EQ(rif_val_hashcode(&pair_0_1), rif_val_hashcode(pair_ptr));
  rif_pair_release(pair_ptr);
}

TEST_F(Pair, rif_pair_tostring_should_be_maningful) {
  RIF_EXPECT_TOSTRING("(0, 1)", rif_val_tostring(&pair_0_1));
  RIF_EXPECT_TOSTRING("(0, [UNDEF])", rif_val_tostring(&pair_0_N));
}

TEST_F(Pair, rif_pair_tostring_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_pair_tostring);
  EXPECT_TRUE(NULL == rif_val_tostring(&pair_0_1));
  rif_alloc_set_filter(_alloc_filter_int_tostring);
  EXPECT_TRUE(NULL == rif_val_tostring(&pair_0_1));
  rif_alloc_set_filter(NULL);
}

TEST_F(Pair, rif_pair_equals_should_be_value_dependent) {
  EXPECT_TRUE(rif_val_equals(&pair_0_1, &pair_0_1));
  EXPECT_FALSE(rif_val_equals(&pair_0_1, &pair_N_N));
  EXPECT_FALSE(rif_val_equals(&pair_0_N, &pair_N_N));

  rif_pair_t * pair_ptr = rif_pair_new(rif_val(&int_0), rif_val(&int_1));
  EXPECT_TRUE(rif_val_equals(&pair_0_1, pair_ptr));
  EXPECT_FALSE(rif_val_equals(&pair_0_5, pair_ptr));
  rif_pair_release(pair_ptr);
}