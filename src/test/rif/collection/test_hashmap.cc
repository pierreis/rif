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

#include "support/map_conformity.hh"

/******************************************************************************
 * TEST FIXTURES
 */

static
bool _alloc_filter_capacity_alloc(const char *tag) {
  return 0 != strcmp(tag, "HASHMAP_CAPACITY_ALLOC");
}

/******************************************************************************
 * TEST CONFIG
 */

class Hashmap : public MemoryAwareTest {

public:

  rif_hashmap_t hm_empty;
  rif_hashmap_t hm_empty_fixed;

private:

  virtual void SetUp() {
    rif_hashmap_init(&hm_empty, 8, false);
    rif_hashmap_init(&hm_empty_fixed, 8, true);
  }

  virtual void TearDown() {
    rif_hashmap_release(&hm_empty);
    rif_hashmap_release(&hm_empty_fixed);
  }

};

/******************************************************************************
 * INIT TESTS
 */

TEST_F(Hashmap, rif_hashmap_init_should_return_null_with_null_ptr) {
  ASSERT_EQ(NULL, rif_hashmap_init(NULL, 8, true));
}

TEST_F(Hashmap, rif_hashmap_init_should_return_an_initialized_hashmap) {
  rif_hashmap_t hm_ptr;
  ASSERT_TRUE(NULL != rif_hashmap_init(&hm_ptr, 8, true));
  EXPECT_EQ(hm_ptr.capacity, 8);
  rif_hashmap_release(&hm_ptr);
}

TEST_F(Hashmap, rif_hashmap_init_should_not_allocate_memory_with_null_capacity) {
  rif_hashmap_t al;
  ASSERT_TRUE(NULL != rif_hashmap_init(&al, 0, true));
  EXPECT_EQ(NULL, al.elements);
  EXPECT_EQ(0, al.capacity);
  rif_hashmap_release(&al);
}

TEST_F(Hashmap, rif_hashmap_init_should_return_null_on_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_capacity_alloc);
  rif_hashmap_t al;
  ASSERT_TRUE(NULL == rif_hashmap_init(&al, 8, true));
  rif_alloc_set_filter(NULL);
}

TEST_F(Hashmap, rif_hashmap_inita_should_return_an_initialized_hashmap) {
  rif_hashmap_t hm_ptr;
  rif_hashmap_inita(&hm_ptr, 8);
  EXPECT_EQ(hm_ptr.capacity, 8);
  rif_hashmap_release(&hm_ptr);
}

/******************************************************************************
 * CAPACITY TESTS
 */

TEST_F(Hashmap, rif_hashmap_ensure_capacity_should_increase_capacity) {
  ASSERT_EQ(RIF_OK, rif_hashmap_ensure_capacity(&hm_empty, 16));
  EXPECT_TRUE(16 <= rif_hashmap_capacity(&hm_empty));
}

TEST_F(Hashmap, rif_hashmap_ensure_capacity_should_not_increase_capacity_if_desired_capacity_is_zero) {
  ASSERT_EQ(RIF_OK, rif_hashmap_ensure_capacity(&hm_empty, 0));
  EXPECT_EQ(8, rif_hashmap_capacity(&hm_empty));
}

TEST_F(Hashmap, rif_hashmap_ensure_capacity_should_not_increase_capacity_of_a_fixed_size_hashmap) {
  ASSERT_EQ(RIF_ERR_CAPACITY, rif_hashmap_ensure_capacity(&hm_empty_fixed, 16));
  EXPECT_EQ(8, rif_hashmap_capacity(&hm_empty_fixed));
}

TEST_F(Hashmap, rif_hashmap_ensure_capacity_should_handle_failing_alloc) {
  rif_alloc_set_filter(_alloc_filter_capacity_alloc);
  ASSERT_EQ(RIF_ERR_MEMORY, rif_hashmap_ensure_capacity(&hm_empty, 16));
  EXPECT_EQ(8, rif_hashmap_capacity(&hm_empty));
  rif_alloc_set_filter(NULL);
}

TEST_F(Hashmap, rif_hashmap_put_should_increase_capacity_if_needed) {
  for (uint8_t n = 0; n < 230; ++n) {
    rif_int_t *val = rif_int_new(n);
    EXPECT_EQ(RIF_OK, rif_hashmap_put(&hm_empty, rif_val(val), rif_val(val)));
    rif_val_release(val);
  }
  EXPECT_TRUE(230 <= rif_hashmap_capacity(&hm_empty));
  for (uint8_t n = 0; n < 230; ++n) {
    rif_int_t *val = rif_int_new(n);
    EXPECT_EQ(n, rif_int_get(rif_int_fromval(rif_hashmap_get(&hm_empty, rif_val(val)))));
    rif_val_release(val);
  }
  printf("Average distance: %f\n", rif_hashmap_average_distance(&hm_empty));
}

/******************************************************************************
 * CONFORMITY
 */

static
rif_map_t *_rif_hashmap_init() {
  rif_hashmap_t *map_ptr = (rif_hashmap_t *) rif_malloc(sizeof(rif_hashmap_t));
  return (rif_map_t *) rif_hashmap_init(map_ptr, 8, false);
}

static
void _rif_hashmap_destroy(rif_map_t *map_ptr) {
  rif_val_release(map_ptr);
  rif_free(map_ptr);
}

static rif_map_conformity_generator_t rif_hashmap_generator = {
    .init = _rif_hashmap_init,
    .destroy = _rif_hashmap_destroy
};

INSTANTIATE_TEST_CASE_P(Arraylist, MapConformity, ::testing::Values(&rif_hashmap_generator));