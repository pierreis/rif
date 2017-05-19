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

#include <thread>

#include "../../test_internal.h"

/******************************************************************************
 * TEST HELPERS
 */

#define NUM_ELEMENTS 8
#define NUM_THREADS  8
#define SLEEP_TIME   100 * 1000
#define SLEEP() usleep(SLEEP_TIME)

static
bool _alloc_filter(const char *tag) {
  return 0 != strcmp(tag, "RIF_POOL_ALLOC");
}

/******************************************************************************
 * TEST CONFIG
 */

class ConcurrentBlockingQueue : public MemoryAwareTest {

public:

  rif_concurrent_blocking_queue_t queue;

private:

  virtual void SetUp() {
    MemoryAwareTest::SetUp();
    rif_concurrent_blocking_queue_init(&this->queue);
  }

  virtual void TearDown() {
    rif_concurrent_blocking_queue_release(&this->queue);
    MemoryAwareTest::TearDown();
  }

};

/******************************************************************************
 * TEST THREADS
 */

static
void _rif_concurrent_blocking_queue_test_pop(rif_concurrent_blocking_queue_t *queue) {
  ASSERT_TRUE(rif_val(rif_true) == rif_concurrent_blocking_queue_pop(queue));
}

/******************************************************************************
 * TESTS
 */

TEST_F(ConcurrentBlockingQueue, rif_concurrent_blocking_queue_trypop_should_work_as_non_blocking) {
  ASSERT_TRUE(NULL == rif_concurrent_blocking_queue_trypop(&queue));
  rif_concurrent_blocking_queue_push(&queue, rif_val(rif_true));
  ASSERT_TRUE(rif_val(rif_true) == rif_concurrent_blocking_queue_trypop(&queue));
  ASSERT_TRUE(NULL == rif_concurrent_blocking_queue_trypop(&queue));
}

TEST_F(ConcurrentBlockingQueue, rif_concurrent_blocking_queue_pop_should_block_waiting_for_data) {
  auto thread = std::thread(_rif_concurrent_blocking_queue_test_pop, &queue);
  SLEEP();
  rif_concurrent_blocking_queue_push(&queue, rif_val(rif_true));
  thread.join();
  ASSERT_TRUE(NULL == rif_concurrent_blocking_queue_trypop(&queue));
}

TEST_F(ConcurrentBlockingQueue, rif_concurrent_blocking_queue_pop_should_not_block_if_data_is_available) {
  rif_concurrent_blocking_queue_push(&queue, rif_val(rif_true));
  auto thread = std::thread(_rif_concurrent_blocking_queue_test_pop, &queue);
  thread.join();
  ASSERT_TRUE(NULL == rif_concurrent_blocking_queue_trypop(&queue));
}