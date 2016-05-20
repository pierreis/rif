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

#include "../../test_internal.h"

/******************************************************************************
 * TEST HELPERS
 */

typedef struct rif_list_conformity_generator_s {
  rif_list_t *(*init)();
  void (*destroy)(rif_list_t *);
} rif_list_conformity_generator_t;

class ListConformity : public ::testing::TestWithParam<rif_list_conformity_generator_s *> {

protected:

  rif_list_t *list_ptr;

private:

  virtual void SetUp() {
    list_ptr = GetParam()->init();
  }

  virtual void TearDown() {
    GetParam()->destroy(list_ptr);
  }

};

void rif_list_fill(rif_list_t *list_ptr, uint8_t elements) {
  for (uint8_t n = 0; n < elements; ++n) {
    rif_int_t *int_ptr = rif_int_new(n);
    rif_list_append(list_ptr, rif_val(int_ptr));
    rif_val_release(int_ptr);
  }
}

#define NUM_ELEMENTS 8

/******************************************************************************
 * TEST DESTROY
 */

TEST_P(ListConformity, list_destroy_should_release_elements) {
  rif_list_t *tmp_list_ptr = GetParam()->init();
  rif_list_fill(tmp_list_ptr, NUM_ELEMENTS);
  rif_val_t *elements[NUM_ELEMENTS];
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    elements[n] = rif_list_get(tmp_list_ptr, n);
    rif_val_retain(elements[n]);
  }
  GetParam()->destroy(tmp_list_ptr);
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    EXPECT_EQ(1, rif_val_reference_count(elements[n]));
    rif_val_release(elements[n]);
  }
}

/******************************************************************************
 * TEST SIZE
 */

TEST_P(ListConformity, list_size_should_be_initially_empty) {
  ASSERT_EQ(0, rif_list_size(list_ptr));
}

TEST_P(ListConformity, list_size_should_increase_when_elements_are_added) {
  for (uint8_t n = 0; n < NUM_ELEMENTS; ++n) {
    ASSERT_EQ(n, rif_list_size(list_ptr));
    rif_list_append(list_ptr, NULL);
  }
}

/******************************************************************************
 * TEST SET
 */

TEST_P(ListConformity, list_set_should_set_element) {
  rif_list_fill(list_ptr, NUM_ELEMENTS);
  rif_int_t *int_ptr = rif_int_new(42);
  rif_val_t *prev_ptr = rif_list_get(list_ptr, 0);
  rif_val_retain(prev_ptr);
  uint32_t pref_refs = rif_val_reference_count(prev_ptr);
  ASSERT_EQ(RIF_OK, rif_list_set(list_ptr, 0, rif_val(int_ptr)));
  EXPECT_EQ(pref_refs - 1, rif_val_reference_count(prev_ptr));
  EXPECT_EQ(rif_val(int_ptr), rif_list_get(list_ptr, 0));
  EXPECT_EQ(2, rif_val_reference_count(int_ptr));
  rif_val_release(int_ptr);
  rif_val_release(prev_ptr);
}

TEST_P(ListConformity, list_set_should_handle_bounds_gracefully) {
  rif_list_fill(list_ptr, NUM_ELEMENTS);
  rif_int_t *int_ptr = rif_int_new(42);
  EXPECT_EQ(RIF_ERR_OUT_OF_BOUNDS, rif_list_set(list_ptr, NUM_ELEMENTS, rif_val(int_ptr)));
  EXPECT_EQ(RIF_ERR_OUT_OF_BOUNDS, rif_list_set(list_ptr, NUM_ELEMENTS + 1, rif_val(int_ptr)));
  EXPECT_EQ(1, rif_val_reference_count(int_ptr));
  rif_val_release(int_ptr);
}

/******************************************************************************
 * TEST INSERT
 */

TEST_P(ListConformity, list_insert_should_insert_element) {
  rif_int_t *int_ptr_1 = rif_int_new(42);
  rif_int_t *int_ptr_2 = rif_int_new(1337);
  ASSERT_EQ(RIF_OK, rif_list_insert(list_ptr, 0, rif_val(int_ptr_1)));
  ASSERT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 0));
  ASSERT_EQ(RIF_OK, rif_list_insert(list_ptr, 0, rif_val(int_ptr_2)));
  EXPECT_EQ(rif_val(int_ptr_2), rif_list_get(list_ptr, 0));
  EXPECT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 1));
  ASSERT_EQ(RIF_OK, rif_list_insert(list_ptr, 1, NULL));
  EXPECT_EQ(rif_val(int_ptr_2), rif_list_get(list_ptr, 0));
  ASSERT_EQ(NULL, rif_list_get(list_ptr, 1));
  EXPECT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 2));
  EXPECT_EQ(2, rif_val_reference_count(int_ptr_1));
  EXPECT_EQ(2, rif_val_reference_count(int_ptr_2));
  rif_val_release(int_ptr_1);
  rif_val_release(int_ptr_2);
}

TEST_P(ListConformity, list_insert_should_handle_bounds_gracefully) {
  rif_list_fill(list_ptr, NUM_ELEMENTS);
  rif_int_t *int_ptr = rif_int_new(42);
  EXPECT_EQ(RIF_ERR_OUT_OF_BOUNDS, rif_list_insert(list_ptr, NUM_ELEMENTS + 1, rif_val(int_ptr)));
  EXPECT_EQ(RIF_ERR_OUT_OF_BOUNDS, rif_list_insert(list_ptr, NUM_ELEMENTS + 2, rif_val(int_ptr)));
  EXPECT_EQ(1, rif_val_reference_count(int_ptr));
  rif_val_release(int_ptr);
}

/******************************************************************************
 * TEST APPEND
 */

TEST_P(ListConformity, list_append_should_append_element) {
  rif_int_t *int_ptr_1 = rif_int_new(42);
  rif_int_t *int_ptr_2 = rif_int_new(1337);
  ASSERT_EQ(RIF_OK, rif_list_append(list_ptr, rif_val(int_ptr_1)));
  ASSERT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 0));
  ASSERT_EQ(RIF_OK, rif_list_append(list_ptr, rif_val(int_ptr_2)));
  EXPECT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 0));
  EXPECT_EQ(rif_val(int_ptr_2), rif_list_get(list_ptr, 1));
  ASSERT_EQ(RIF_OK, rif_list_append(list_ptr, NULL));
  EXPECT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 0));
  EXPECT_EQ(rif_val(int_ptr_2), rif_list_get(list_ptr, 1));
  ASSERT_EQ(NULL, rif_list_get(list_ptr, 2));
  EXPECT_EQ(2, rif_val_reference_count(int_ptr_1));
  EXPECT_EQ(2, rif_val_reference_count(int_ptr_2));
  rif_val_release(int_ptr_1);
  rif_val_release(int_ptr_2);
}

/******************************************************************************
 * TEST PREPEND
 */

TEST_P(ListConformity, list_prepend_should_prepend_element) {
  rif_int_t *int_ptr_1 = rif_int_new(42);
  rif_int_t *int_ptr_2 = rif_int_new(1337);
  ASSERT_EQ(RIF_OK, rif_list_prepend(list_ptr, rif_val(int_ptr_1)));
  ASSERT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 0));
  ASSERT_EQ(RIF_OK, rif_list_prepend(list_ptr, rif_val(int_ptr_2)));
  EXPECT_EQ(rif_val(int_ptr_2), rif_list_get(list_ptr, 0));
  EXPECT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 1));
  ASSERT_EQ(RIF_OK, rif_list_prepend(list_ptr, NULL));
  ASSERT_EQ(NULL, rif_list_get(list_ptr, 0));
  EXPECT_EQ(rif_val(int_ptr_2), rif_list_get(list_ptr, 1));
  EXPECT_EQ(rif_val(int_ptr_1), rif_list_get(list_ptr, 2));
  EXPECT_EQ(2, rif_val_reference_count(int_ptr_1));
  EXPECT_EQ(2, rif_val_reference_count(int_ptr_2));
  rif_val_release(int_ptr_1);
  rif_val_release(int_ptr_2);
}

/******************************************************************************
 * TEST REMOVE
 */

TEST_P(ListConformity, list_remove_should_remove_element) {
  rif_list_fill(list_ptr, NUM_ELEMENTS);
  uint8_t count = rif_list_size(list_ptr);
  for (uint8_t n = NUM_ELEMENTS - 1; n >= 0 && n < NUM_ELEMENTS; n -= 2) {
    rif_val_t *element_ptr = rif_list_get(list_ptr, n);
    rif_val_retain(element_ptr);
    EXPECT_EQ(RIF_OK, rif_list_remove(list_ptr, n));
    EXPECT_EQ(--count, rif_list_size(list_ptr));
    EXPECT_EQ(1, rif_val_reference_count(element_ptr));
    rif_val_release(element_ptr);
  }
  for (uint8_t n = 0; n < NUM_ELEMENTS / 2; ++n) {
    rif_val_t *element_ptr = rif_list_get(list_ptr, 0);
    EXPECT_EQ(n * 2, rif_int_get(rif_int_fromval(element_ptr)));
    EXPECT_EQ(RIF_OK, rif_list_remove(list_ptr, 0));
  }
}

TEST_P(ListConformity, list_remove_should_handle_bounds_gracefully) {
  rif_list_fill(list_ptr, NUM_ELEMENTS);
  EXPECT_EQ(RIF_ERR_OUT_OF_BOUNDS, rif_list_remove(list_ptr, NUM_ELEMENTS));
  EXPECT_EQ(RIF_ERR_OUT_OF_BOUNDS, rif_list_remove(list_ptr, NUM_ELEMENTS + 1));
}