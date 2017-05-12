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

#include "rif/rif_internal.h"

#include "rif/concurrent/collection/rif_concurrent_queue_base.h"

/******************************************************************************
 * MARKERS
 */

#define REFS_MASK 0x7FFFFFFF
#define QUEUE_PUSH 0x80000000

/******************************************************************************
 * STATIC HELPERS
 */

static
void _rif_concurrent_queue_base_add(rif_concurrent_queue_base_t *queue_ptr,
                                    rif_concurrent_queue_base_node_t *node_ptr) {
  rif_concurrent_queue_base_node_t *head =
      (rif_concurrent_queue_base_node_t *) atomic_load_explicit(&queue_ptr->first, memory_order_relaxed);
  while (true) {
    atomic_store_explicit(&node_ptr->succ, (uintptr_t) head, memory_order_relaxed);
    atomic_store_explicit(&node_ptr->reference_count, 1, memory_order_release);
    if (!atomic_compare_exchange_strong_explicit(&queue_ptr->first, (uintptr_t *) &head, (uintptr_t) node_ptr,
                                                 memory_order_release, memory_order_relaxed)) {
      uint32_t reference_count = atomic_fetch_add_explicit(&node_ptr->reference_count, QUEUE_PUSH - 1,
                                                           memory_order_release);
      if (reference_count == 1) {
        continue;
      }
    }
    return;
  }
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Destroy a base concurrent queue
 */
void rif_concurrent_queue_base_destroy(rif_concurrent_queue_base_t *queue_ptr) {
  if (queue_ptr->dtor) {
    rif_concurrent_queue_base_node_t *cur =
        (rif_concurrent_queue_base_node_t *) atomic_load_explicit(&queue_ptr->first, memory_order_relaxed);
    while (cur) {
      rif_concurrent_queue_base_node_t *next =
          (rif_concurrent_queue_base_node_t *) atomic_load_explicit(&cur->succ, memory_order_relaxed);
      queue_ptr->dtor(cur);
      cur = next;
    }
  }
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Push a new node to the queue
 */
void rif_concurrent_queue_base_push(rif_concurrent_queue_base_t *queue_ptr,
                                    rif_concurrent_queue_base_node_t *node_ptr) {
  uint32_t reference_count = atomic_fetch_add_explicit(&node_ptr->reference_count, QUEUE_PUSH, memory_order_release);
  if (reference_count == 0) {
    _rif_concurrent_queue_base_add(queue_ptr, node_ptr);
  }
}

/**
 * Pop the first node of the queue
 */
rif_concurrent_queue_base_node_t * rif_concurrent_queue_base_pop(rif_concurrent_queue_base_t *queue_ptr) {
  rif_concurrent_queue_base_node_t *head =
      (rif_concurrent_queue_base_node_t *) atomic_load_explicit(&queue_ptr->first, memory_order_acquire);
  while (__likely(head)) {

    //printf("Trying to pop %p\n", head);

    // Increase reference count
    rif_concurrent_queue_base_node_t *prev_head = head;
    uint32_t reference_count = atomic_load_explicit(&head->reference_count, memory_order_relaxed);
    //printf("Reference count: %u\n", reference_count);
    if ((reference_count & REFS_MASK) == 0 ||
        !atomic_compare_exchange_strong_explicit(&head->reference_count, &reference_count, reference_count + 1,
                                                 memory_order_acquire, memory_order_relaxed)) {
      head = (rif_concurrent_queue_base_node_t *) atomic_load_explicit(&queue_ptr->first, memory_order_acquire);
      //printf("Moo! %i\n", reference_count & REFS_MASK);
      continue;
    }

    // Set head
    uintptr_t next = atomic_load_explicit(&head->succ, memory_order_relaxed);
    if (atomic_compare_exchange_strong_explicit(&queue_ptr->first, (uintptr_t *) &head, next,
                                                memory_order_acquire, memory_order_relaxed)) {
      atomic_fetch_add_explicit(&head->reference_count, -2, memory_order_relaxed);
      return head;
    }

    // Head has changed on us
    reference_count = atomic_fetch_add_explicit(&prev_head->reference_count, -1, memory_order_acq_rel);
    if (reference_count == QUEUE_PUSH + 1) {
      _rif_concurrent_queue_base_add(queue_ptr, prev_head);
    }
  }

  return NULL;
}