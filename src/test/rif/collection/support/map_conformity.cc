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

#include "map_conformity.hh"

/******************************************************************************
 * TEST HELPERS
 */

void rif_map_fill(rif_map_t *map_ptr, uint8_t elements) {
  char str[2] = "a";
  for (uint8_t n = 0; n < elements; ++n) {
    rif_int_t *key = rif_int_new(n);
    rif_string_t *val = rif_string_new_dup((char *) &str);
    rif_map_put(map_ptr, rif_val(key), rif_val(val));
    rif_val_release(key);
    rif_val_release(val);
    ++str[0];
  }
}

#define NUM_ELEMENTS 8

static
bool _alloc_filter_map_tostring(const char *tag) {
  return 0 != strcmp(tag, "RIF_MAP_TOSTRING");
}

static
bool _alloc_filter_int_tostring(const char *tag) {
  return 0 != strcmp(tag, "RIF_INT_TOSTRING");
}

/******************************************************************************
 * TEST DESTROY
 */

TEST_P(MapConformity, map_destroy_should_release_elements) {
  rif_map_t *tmp_map_ptr = GetParam()->init();
  rif_map_fill(tmp_map_ptr, NUM_ELEMENTS);
  rif_val_t *elements[NUM_ELEMENTS];
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    elements[n] = rif_map_get(tmp_map_ptr, (const rif_val_t *) rif_val(ints[n]));
    rif_val_retain(elements[n]);
  }
  GetParam()->destroy(tmp_map_ptr);
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    ASSERT_EQ(1, rif_val_reference_count(elements[n]));
    rif_val_release(elements[n]);
  }
}

/******************************************************************************
 * TEST SIZE
 */

TEST_P(MapConformity, map_size_should_be_initially_empty) {
  ASSERT_EQ(0, rif_map_size(map_ptr));
}

TEST_P(MapConformity, map_size_should_increase_when_elements_are_added) {
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    ASSERT_EQ(n, rif_map_size(map_ptr));
    rif_map_put(map_ptr, rif_val(ints[n]), rif_val(rif_null));
  }
}

/******************************************************************************
 * TEST GET
 */

TEST_P(MapConformity, map_get_should_get_element) {
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    EXPECT_STREQ(rif_string_get(strs[n]), rif_string_get(rif_string_fromval(rif_map_get(map_ptr, rif_val(ints[n])))));
  }
}

TEST_P(MapConformity, map_get_should_return_null_on_nonexistent_element) {
  EXPECT_TRUE(NULL == rif_map_get(map_ptr, rif_val(ints[0])));
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  EXPECT_TRUE(NULL == rif_map_get(map_ptr, rif_val(strs[0])));
}

TEST_P(MapConformity, map_exists_should_check_element_existence) {
  EXPECT_FALSE(rif_map_exists(map_ptr, rif_val(ints[0])));
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    EXPECT_TRUE(rif_map_exists(map_ptr, rif_val(ints[n])));
  }
  rif_map_remove(map_ptr, rif_val(ints[0]));
  EXPECT_FALSE(rif_map_exists(map_ptr, rif_val(ints[0])));
}


/******************************************************************************
 * TEST PUT
 */

TEST_P(MapConformity, map_put_should_put_element) {
  rif_map_put(map_ptr, rif_val(strs[0]), rif_val(ints[0]));
  ASSERT_EQ(1, rif_map_size(map_ptr));
  ASSERT_EQ(0, rif_int_get(rif_int_fromval(rif_map_get(map_ptr, rif_val(strs[0])))));
  rif_map_put(map_ptr, rif_val(strs[1]), rif_val(ints[1]));
  ASSERT_EQ(2, rif_map_size(map_ptr));
  ASSERT_EQ(1, rif_int_get(rif_int_fromval(rif_map_get(map_ptr, rif_val(strs[1])))));
  rif_map_put(map_ptr, rif_val(strs[0]), rif_val(ints[1]));
  ASSERT_EQ(2, rif_map_size(map_ptr));
  ASSERT_EQ(1, rif_int_get(rif_int_fromval(rif_map_get(map_ptr, rif_val(strs[0])))));
}

/******************************************************************************
 * TEST REMOVE
 */

TEST_P(MapConformity, map_remove_should_remove_element) {
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  ASSERT_STREQ("a", rif_string_get(rif_string_fromval(rif_map_get(map_ptr, rif_val(ints[0])))));
  rif_map_remove(map_ptr, rif_val(ints[0]));
  ASSERT_TRUE(NULL == rif_map_get(map_ptr, rif_val(ints[0])));
  ASSERT_EQ(NUM_ELEMENTS - 1, rif_map_size(map_ptr));
}

TEST_P(MapConformity, map_remove_should_release_element) {
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  rif_val_t *val = rif_map_get(map_ptr, rif_val(ints[0]));
  rif_val_retain(val);
  ASSERT_EQ(2, rif_val_reference_count(val));
  rif_map_remove(map_ptr, rif_val(ints[0]));
  ASSERT_EQ(1, rif_val_reference_count(val));
  rif_val_release(val);
}

TEST_P(MapConformity, map_remove_handle_nonexistent_elements) {
  ASSERT_EQ(RIF_OK, rif_map_remove(map_ptr, rif_val(rif_null)));
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  ASSERT_EQ(RIF_OK, rif_map_remove(map_ptr, rif_val(rif_false)));
}

/******************************************************************************
 * TEST ITERATOR
 */

void test_iterator(rif_map_t *map_ptr, rif_iterator_t *it_ptr, uint8_t num_elements) {
  uint8_t n = 0;
  bool check[num_elements];
  memset(&check, 0, num_elements * sizeof(bool));
  char str[2] = "a";
  while (rif_iterator_hasnext(it_ptr)) {
    rif_pair_t *pair_ptr = rif_pair_fromval(rif_iterator_next(it_ptr));
    uint8_t index = (uint8_t) rif_int_get(rif_int_fromval(rif_pair_2(pair_ptr)));
    EXPECT_GT(num_elements, index);
    EXPECT_FALSE(check[index]);
    check[index] = true;
    str[0] = 'a' + index;
    EXPECT_STREQ((char *) &str, rif_string_get(rif_string_fromval(rif_pair_1(pair_ptr))));
    ++n;
  }
  EXPECT_EQ(num_elements, n);
  EXPECT_EQ(NULL, rif_iterator_next(it_ptr));
  EXPECT_FALSE(rif_iterator_hasnext(it_ptr));
}

TEST_P(MapConformity, map_iterator_new_should_return_an_initialized_iterator) {
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  rif_iterator_t *it_ptr = (rif_iterator_t *) rif_map_iterator_new(map_ptr);
  test_iterator(map_ptr, it_ptr, NUM_ELEMENTS);
  rif_iterator_destroy(it_ptr);
}

TEST_P(MapConformity, map_iterator_init_should_return_an_initialized_iterator) {
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  rif_map_iterator_t it_ptr;
  rif_pair_t pair_ptr;
  rif_map_iterator_init(&it_ptr, map_ptr, &pair_ptr);
  test_iterator(map_ptr, (rif_iterator_t *) &it_ptr, NUM_ELEMENTS);
  rif_iterator_destroy((rif_iterator_t *) &it_ptr);
}

TEST_P(MapConformity, map_iterator_init_should_return_null_for_null_iterator) {
  ASSERT_EQ(NULL, rif_map_iterator_init(NULL, map_ptr, NULL));
}

/******************************************************************************
 * TEST MAP CALLBACKS
 */

TEST_P(MapConformity, map_tostring_should_return_meaningful_string) {
  RIF_EXPECT_TOSTRING("{}", rif_val_tostring(map_ptr));
  rif_map_fill(map_ptr, 1);
  RIF_EXPECT_TOSTRING("{0: \"a\"}", rif_val_tostring(map_ptr));
}

TEST_P(MapConformity, map_equals_should_be_value_dependent) {
  EXPECT_TRUE(rif_val_equals(map_ptr, map_ptr));
  rif_map_t *tmp_map_ptr = GetParam()->init();
  EXPECT_TRUE(rif_val_equals(map_ptr, tmp_map_ptr));
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  EXPECT_FALSE(rif_val_equals(map_ptr, tmp_map_ptr));
  rif_map_fill(tmp_map_ptr, NUM_ELEMENTS);
  EXPECT_TRUE(rif_val_equals(map_ptr, tmp_map_ptr));
  rif_map_put(tmp_map_ptr, rif_val(ints[0]), rif_val(rif_null));
  EXPECT_FALSE(rif_val_equals(map_ptr, tmp_map_ptr));
  GetParam()->destroy(tmp_map_ptr);
}

TEST_P(MapConformity, map_tostring_should_return_null_on_failing_alloc) {
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  rif_alloc_set_filter(_alloc_filter_int_tostring);
  EXPECT_EQ(NULL, rif_val_tostring(map_ptr));
  rif_alloc_set_filter(_alloc_filter_map_tostring);
  EXPECT_EQ(NULL, rif_val_tostring(map_ptr));
  rif_alloc_set_filter(NULL);
}

TEST_P(MapConformity, map_hashcode_should_be_value_dependent) {
  EXPECT_EQ(rif_val_hashcode(map_ptr), rif_val_hashcode(map_ptr));
  rif_map_t *tmp_map_ptr = GetParam()->init();
  EXPECT_EQ(rif_val_hashcode(map_ptr), rif_val_hashcode(tmp_map_ptr));
  rif_map_fill(map_ptr, NUM_ELEMENTS);
  EXPECT_NE(rif_val_hashcode(map_ptr), rif_val_hashcode(tmp_map_ptr));
  rif_map_fill(tmp_map_ptr, NUM_ELEMENTS);
  EXPECT_EQ(rif_val_hashcode(map_ptr), rif_val_hashcode(tmp_map_ptr));
  GetParam()->destroy(tmp_map_ptr);
}