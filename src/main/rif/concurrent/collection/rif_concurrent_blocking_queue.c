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

#include "rif/rif_internal.h"

#include "rif/concurrent/collection/rif_concurrent_blocking_queue.h"

/******************************************************************************
 * STATIC FUNCTIONS
 */

static
void _rif_concurrent_blocking_queue_node_add_cb(rif_concurrent_queue_base_t *queue_ptr,
                                                rif_concurrent_queue_base_node_t *node_ptr, void *udata) {
  rif_concurrent_blocking_queue_t *bqueue_ptr = (rif_concurrent_blocking_queue_t *) udata;
  bqueue_ptr->parent_add(queue_ptr, node_ptr, udata);
  sem_post(&bqueue_ptr->semaphore);
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_concurrent_blocking_queue_t * rif_concurrent_blocking_queue_init(rif_concurrent_blocking_queue_t *queue_ptr) {
  if (__unlikely(!rif_concurrent_queue_init((rif_concurrent_queue_t *) queue_ptr))) {
    return NULL;
  }
  if (__unlikely(0 != sem_init(&queue_ptr->semaphore, 0, 0))) {
    rif_val_release(queue_ptr);
    return NULL;
  }

  // Monkey-patch callback to signal additions when they are raised
  queue_ptr->parent_add = ((rif_concurrent_queue_t *) queue_ptr)->queue_base.add;
  ((rif_concurrent_queue_t *) queue_ptr)->queue_base.add = _rif_concurrent_blocking_queue_node_add_cb;

  // Monkey-patch hooks
  ((rif_queue_t *) queue_ptr)->hooks = &rif_concurrent_blocking_queue_hooks;

  return queue_ptr;
}

void rif_concurrent_blocking_queue_destroy_callback(rif_concurrent_blocking_queue_t *queue_ptr) {
  sem_destroy(&queue_ptr->semaphore);
  rif_concurrent_queue_hooks.destroy((rif_queue_t *) queue_ptr);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

rif_val_t * rif_concurrent_blocking_queue_pop(rif_concurrent_blocking_queue_t *queue_ptr) {
  assert(NULL != queue_ptr);
  rif_val_t *val = NULL;
  while (true) {
    sem_wait(&queue_ptr->semaphore);
    val = rif_concurrent_queue_pop((rif_concurrent_queue_t *) queue_ptr);
    if (val) {
      break;
    }
  }
  return val;
}

rif_val_t * rif_concurrent_blocking_queue_trypop(rif_concurrent_blocking_queue_t *queue_ptr) {
  assert(NULL != queue_ptr);
  if (sem_trywait(&queue_ptr->semaphore)) {
    return NULL;
  }
  return rif_concurrent_queue_pop((rif_concurrent_queue_t *) queue_ptr);
}

rif_val_t * rif_concurrent_blocking_queue_timedpop(rif_concurrent_blocking_queue_t *queue_ptr,
                                                   const struct timespec *abs_timeout) {
  assert(NULL != queue_ptr);
  rif_val_t *val = NULL;
  while (true) {
    int sem_timedwait_result = sem_timedwait(&queue_ptr->semaphore, abs_timeout);
    if (-1 == sem_timedwait_result) {
      return NULL;
    }
    val = rif_concurrent_queue_pop((rif_concurrent_queue_t *) queue_ptr);
    assert(NULL != val);
    break;
  }
  return val;
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
