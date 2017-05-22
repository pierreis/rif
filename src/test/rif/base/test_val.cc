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

TEST(Val, rif_val_init_should_return_null_on_null_ptr) {
  ASSERT_TRUE(NULL == rif_val_init(NULL, RIF_UNDEF, false));
}

TEST(Val, rif_val_equals_should_return_false_with_different_types) {
  ASSERT_FALSE(rif_val_equals(rif_null, rif_true));
}

TEST(Val, rif_val_hashcode_should_return_0_with_null_ptr) {
  ASSERT_EQ(0, rif_val_hashcode(NULL));
}

TEST(Val, rif_val_tostring_should_return_null_with_null_ptr) {
  ASSERT_TRUE(NULL == rif_val_tostring(NULL));
}

TEST(Val, rif_val_tostring_should_return_null_for_undef) {
  rif_val_t undef;
  rif_val_init(&undef, RIF_UNDEF, false);
  ASSERT_TRUE(NULL == rif_val_tostring(&undef));
  rif_val_release(&undef);
}

TEST(Val, rif_val_release_should_handle_zero_reference_count) {
  rif_val_t fake_integer;
  rif_val_init(&fake_integer, RIF_INT, false);
  atomic_store(&(fake_integer.reference_count), 0);
  ASSERT_TRUE(&fake_integer == rif_val_release(&fake_integer));
}