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
 * TESTS
 */

TEST(Null, rif_null_should_not_have_increasing_or_decreasing_reference_count) {
  rif_val_retain(rif_null);
  ASSERT_EQ(0, rif_val_reference_count(rif_null));
  rif_val_release(rif_null);
  ASSERT_EQ(0, rif_val_reference_count(rif_null));
}

TEST(Null, rif_null_should_have_meaningful_tostring) {
  RIF_EXPECT_TOSTRING("NULL", rif_val_tostring(rif_null));
}

TEST(Null, rif_null_should_have_hashcode) {
  EXPECT_EQ(0, rif_val_hashcode(rif_null));
}

TEST(Null, rif_null_should_be_equal_to_itself) {
  EXPECT_TRUE(rif_val_equals(rif_null, rif_null));
}

TEST(Null, rif_null_fromval_should_cast_value_correctly) {
  EXPECT_FALSE(NULL == rif_null_fromval(rif_val(rif_null)));
}