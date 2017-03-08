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

static uint8_t _malloc_counter = 0;
static uint8_t _realloc_counter = 0;
static uint8_t _free_counter = 0;

static
void * _test_malloc(size_t size) {
  ++_malloc_counter;
  return NULL;
}

static
void * _test_realloc(void *ptr, size_t size) {
  ++_realloc_counter;
  return NULL;
}

static
void _test_free(void *ptr) {
  ++_free_counter;
}

static
void * _test_malloc_calloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    return ptr;
  }
  memset(ptr, 1, size);
  return ptr;
}

/******************************************************************************
 * TEST CONFIG
 */

class Alloc : public MemoryAwareTest {

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
  }

  virtual void TearDown() {
    rif_set_allocators(f_malloc, f_realloc, f_free);
    _malloc_counter = 0;
    _realloc_counter = 0;
    _free_counter = 0;
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * TESTS
 */

TEST_F(Alloc, rif_set_allocators_should_set_malloc) {
  rif_set_allocators(_test_malloc, f_realloc, f_free);
  rif_malloc(1);
  EXPECT_EQ(1, _malloc_counter);
  rif_set_allocators(NULL, f_realloc, f_free);
  rif_malloc(1);
  EXPECT_EQ(2, _malloc_counter);
}

TEST_F(Alloc, rif_set_allocators_should_set_realloc) {
  rif_set_allocators(f_malloc, _test_realloc, f_free);
  rif_realloc(NULL, 1);
  EXPECT_EQ(1, _realloc_counter);
  rif_set_allocators(f_malloc, NULL, f_free);
  rif_realloc(NULL, 1);
  EXPECT_EQ(2, _realloc_counter);
}

TEST_F(Alloc, rif_set_allocators_should_set_free) {
  rif_set_allocators(f_malloc, f_realloc, _test_free);
  rif_free(NULL);
  EXPECT_EQ(1, _free_counter);
  rif_set_allocators(f_malloc, f_realloc, NULL);
  rif_free(NULL);
  EXPECT_EQ(2, _free_counter);
}

TEST_F(Alloc, rif_calloc_should_use_defined_malloc) {
  rif_set_allocators(_test_malloc, f_realloc, f_free);
  void *ptr = rif_calloc(1, 1);
  EXPECT_TRUE(!ptr);
  EXPECT_EQ(1, _malloc_counter);
}

TEST_F(Alloc, rif_calloc_should_set_bytes_to_0) {
  rif_set_allocators(_test_malloc_calloc, NULL, NULL);
  char *ptr = (char *) rif_calloc(10, 10);
  EXPECT_FALSE(!ptr);
  for (int i = 0; i < 100; ++i) {
    EXPECT_EQ(0, ptr[i]);
  }
  rif_free(ptr);
}

TEST_F(Alloc, rif_strdup_should_work) {
  const char *test = "foo";
  char *test_dup = rif_strdup(test);
  EXPECT_STREQ(test, test_dup);
  EXPECT_NE(test, test_dup);
  rif_free(test_dup);
}

TEST_F(Alloc, rif_strdup_should_use_defined_malloc) {
  rif_set_allocators(_test_malloc, f_realloc, f_free);
  const char *test = "foo";
  char *test_dup = rif_strdup(test);
  EXPECT_TRUE(!test_dup);
  EXPECT_EQ(1, _malloc_counter);
}

TEST_F(Alloc, rif_alloca_should_work) {
  void *ptr = rif_alloca(10);
  EXPECT_FALSE(!ptr);
}