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
 * HOOK HELPERS
 */

static
void _rif_concurrent_blocking_queue_hook_destroy(rif_queue_t *queue_ptr) {
  rif_concurrent_blocking_queue_destroy_callback((rif_concurrent_blocking_queue_t *) queue_ptr);
}

static
uint32_t _rif_concurrent_blocking_queue_hook_size(rif_queue_t *queue_ptr) {
  return rif_concurrent_blocking_queue_size((rif_concurrent_blocking_queue_t *) queue_ptr);
}

static
rif_status_t _rif_concurrent_blocking_queue_hook_push(rif_queue_t *queue_ptr, rif_val_t *val_ptr) {
  return rif_concurrent_blocking_queue_push((rif_concurrent_blocking_queue_t *) queue_ptr, val_ptr);
}

static
rif_val_t * _rif_concurrent_blocking_queue_hook_pop(rif_queue_t *queue_ptr) {
  return rif_concurrent_blocking_queue_pop((rif_concurrent_blocking_queue_t *) queue_ptr);
}

/******************************************************************************
 * HOOKS
 */

const rif_queue_hooks_t rif_concurrent_blocking_queue_hooks = {
    .destroy = _rif_concurrent_blocking_queue_hook_destroy,
    .size    = _rif_concurrent_blocking_queue_hook_size,
    .push    = _rif_concurrent_blocking_queue_hook_push,
    .pop     = _rif_concurrent_blocking_queue_hook_pop
};