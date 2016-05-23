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

static
bool _alloc_filter_string_new(const char *tag) {
  return 0 != strcmp(tag, "RIF_STRING_NEW");
}

static
bool _alloc_filter_string_new_dup(const char *tag) {
  return 0 != strcmp(tag, "RIF_STRING_NEW_DUP");
}

static
bool _alloc_filter_string_tostring(const char *tag) {
  return 0 != strcmp(tag, "RIF_STRING_TOSTRING");
}

/******************************************************************************
 * TEST CONFIG
 */

static const char *_strs[3] = {"foo", "bar", "moo"};

class String : public MemoryAwareTest {

public:

  rif_string_t str_foo;
  rif_string_t str_bar;
  rif_string_t str_moo;

private:

  virtual void SetUp() {
    rif_string_init(&str_foo, (char *) _strs[0], false);
    rif_string_init(&str_bar, (char *) _strs[1], false);
    rif_string_init(&str_moo, (char *) _strs[2], false);
  }

  virtual void TearDown() {
    rif_string_release(&str_foo);
    rif_string_release(&str_bar);
    rif_string_release(&str_moo);
  }

};

/******************************************************************************
 * TESTS
 */

TEST_F(String, rif_string_init_should_return_null_with_null_ptr) {
  ASSERT_TRUE(NULL == rif_string_init(NULL, NULL, false));
}

TEST_F(String, rif_string_init_should_return_an_initialized_string_with_null_string) {
  rif_string_t str_tmp;
  ASSERT_TRUE(&str_tmp == rif_string_init(&str_tmp, NULL, false));
  rif_val_release(&str_tmp);
}

TEST_F(String, rif_string_init_should_return_an_initialized_string) {
  rif_string_t str_tmp;
  ASSERT_TRUE(&str_tmp == rif_string_init(&str_tmp, (char *) _strs[0], false));
  ASSERT_STREQ("foo", rif_string_get(&str_tmp));
  rif_val_release(&str_tmp);
}

TEST_F(String, rif_string_init_wlen_should_return_an_initialized_string) {
  rif_string_t str_tmp;
  ASSERT_TRUE(&str_tmp == rif_string_init_wlen(&str_tmp, (char *) _strs[0], 3, false));
  ASSERT_STREQ("foo", rif_string_get(&str_tmp));
  rif_val_release(&str_tmp);
}

TEST_F(String, rif_string_new_should_return_an_initialized_string) {
  rif_string_t *str_ptr;
  str_ptr = rif_string_new((char *) _strs[0], false);
  ASSERT_FALSE(NULL == str_ptr);
  ASSERT_STREQ("foo", rif_string_get(str_ptr));
  rif_val_release(str_ptr);
}

TEST_F(String, rif_string_new_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_string_new);
  ASSERT_TRUE(NULL == rif_string_new((char *) _strs[0], false));
  rif_alloc_set_filter(NULL);
}

TEST_F(String, rif_string_new_should_return_an_initialized_string_with_null_string) {
  rif_string_t *str_ptr;
  str_ptr = rif_string_new(NULL, false);
  ASSERT_FALSE(NULL == str_ptr);
  rif_val_release(str_ptr);
}

TEST_F(String, rif_string_new_wlen_should_return_an_initialized_string) {
  rif_string_t *str_ptr;
  str_ptr = rif_string_new_wlen((char *) _strs[0], 3, false);
  ASSERT_FALSE(NULL == str_ptr);
  ASSERT_STREQ("foo", rif_string_get(str_ptr));
  rif_val_release(str_ptr);
}

TEST_F(String, rif_string_new_wlen_should_return_an_initialized_string_with_null_string) {
  rif_string_t *str_ptr;
  str_ptr = rif_string_new_wlen(NULL, 0, false);
  ASSERT_FALSE(NULL == str_ptr);
  rif_val_release(str_ptr);
}

TEST_F(String, rif_string_new_dup_should_return_an_initialized_string) {
  rif_string_t *str_ptr;
  str_ptr = rif_string_new_dup(_strs[0]);
  ASSERT_FALSE(NULL == str_ptr);
  ASSERT_STREQ("foo", rif_string_get(str_ptr));
  ASSERT_FALSE(_strs[0] == rif_string_get(str_ptr));
  rif_val_release(str_ptr);
}

TEST_F(String, rif_string_new_dup_should_return_null_on_null_string) {
  ASSERT_TRUE(NULL == rif_string_new_dup(NULL));
}

TEST_F(String, rif_string_new_dup_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_string_new_dup);
  ASSERT_TRUE(NULL == rif_string_new_dup(_strs[0]));
  rif_alloc_set_filter(_alloc_filter_string_new);
  ASSERT_TRUE(NULL == rif_string_new_dup(_strs[0]));
  rif_alloc_set_filter(NULL);
}

TEST_F(String, rif_string_tostring_should_be_maningful) {
  RIF_EXPECT_TOSTRING("\"foo\"", rif_val_tostring(&str_foo));
}

TEST_F(String, rif_string_tostring_should_return_null_with_null_value) {
  rif_string_t *str_ptr;
  str_ptr = rif_string_new(NULL, false);
  EXPECT_TRUE(NULL == rif_val_tostring(str_ptr));
  rif_val_release(str_ptr);
}

TEST_F(String, rif_string_tostring_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_string_tostring);
  EXPECT_TRUE(NULL == rif_val_tostring(&str_foo));
  rif_alloc_set_filter(NULL);
}

TEST_F(String, rif_string_hashcode_should_be_value_dependent) {
  EXPECT_EQ(rif_val_hashcode(&str_foo), rif_val_hashcode(&str_foo));
  EXPECT_NE(rif_val_hashcode(&str_foo), rif_val_hashcode(&str_bar));

  rif_string_t *str_tmp1_ptr;
  str_tmp1_ptr = rif_string_new_dup(_strs[0]);
  ASSERT_EQ(rif_val_hashcode(str_tmp1_ptr), rif_val_hashcode(&str_foo));
  rif_val_release(str_tmp1_ptr);

  rif_string_t *str_tmp2_ptr;
  str_tmp2_ptr = rif_string_new(NULL, false);
  ASSERT_EQ(rif_val_hashcode(str_tmp2_ptr), rif_val_hashcode(str_tmp2_ptr));
  rif_val_release(str_tmp2_ptr);
}

TEST_F(String, rif_string_equals_should_be_value_dependent) {
  EXPECT_TRUE(rif_val_equals(&str_foo, &str_foo));
  EXPECT_FALSE(rif_val_equals(&str_foo, &str_bar));

  rif_string_t *str_tmp1_ptr;
  str_tmp1_ptr = rif_string_new_dup(_strs[0]);
  ASSERT_TRUE(rif_val_equals(str_tmp1_ptr, &str_foo));
  rif_val_release(str_tmp1_ptr);

  rif_string_t *str_tmp2_ptr;
  str_tmp2_ptr = rif_string_new(NULL, false);
  ASSERT_TRUE(rif_val_equals(str_tmp2_ptr, str_tmp2_ptr));
  rif_val_release(str_tmp2_ptr);
}