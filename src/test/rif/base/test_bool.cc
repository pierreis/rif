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
 * TRUE TESTS
 */

TEST(Bool, rif_true_should_not_have_increasing_or_decreasing_reference_count) {
  rif_val_retain(rif_true);
  ASSERT_EQ(0, rif_val_reference_count(rif_true));
  rif_val_release(rif_true);
  ASSERT_EQ(0, rif_val_reference_count(rif_true));
}

TEST(Bool, rif_true_should_have_meaningful_tostring) {
  RIF_EXPECT_TOSTRING("TRUE", rif_val_tostring(rif_true));
}

TEST(Bool, rif_true_should_have_hashcode) {
  EXPECT_EQ(1, rif_val_hashcode(rif_true));
}

TEST(Bool, rif_true_should_be_equal_to_itself) {
  EXPECT_TRUE(rif_val_equals(rif_true, rif_true));
}

/******************************************************************************
 * FALSE TESTS
 */

TEST(Bool, rif_false_should_not_have_increasing_or_decreasing_reference_count) {
  rif_val_retain(rif_false);
  ASSERT_EQ(0, rif_val_reference_count(rif_false));
  rif_val_release(rif_false);
  ASSERT_EQ(0, rif_val_reference_count(rif_false));
}

TEST(Bool, rif_false_should_have_meaningful_tostring) {
  RIF_EXPECT_TOSTRING("FALSE", rif_val_tostring(rif_false));
}

TEST(Bool, rif_false_should_have_hashcode) {
  EXPECT_EQ(0, rif_val_hashcode(rif_false));
}

TEST(Bool, rif_false_should_be_equal_to_itself) {
  EXPECT_TRUE(rif_val_equals(rif_false, rif_false));
}

/******************************************************************************
 * SHARED TESTS
 */

TEST(Bool, rif_false_should_not_be_equal_to_true) {
  EXPECT_FALSE(rif_val_equals(rif_false, rif_true));
}

TEST(Bool, rif_bool_fromval_should_cast_value_correctly) {
  EXPECT_FALSE(NULL == rif_bool_fromval(rif_val(rif_true)));
  EXPECT_FALSE(NULL == rif_bool_fromval(rif_val(rif_false)));
  EXPECT_TRUE(NULL == rif_bool_fromval(rif_val(rif_null)));
}