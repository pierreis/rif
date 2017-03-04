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

/******************************************************************************
 * TEST DESTROY
 */

TEST_P(MapConformity, list_destroy_should_release_elements) {
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

TEST_P(MapConformity, mqp_size_should_be_initially_empty) {
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