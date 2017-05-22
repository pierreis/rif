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

#include "rif/collection/rif_queue.h"
#include "rif/util/rif_hash.h"

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_queue_t * rif_queue_init(rif_queue_t *queue_ptr, const rif_queue_hooks_t *hooks, bool free) {
  rif_val_init(rif_val(queue_ptr), RIF_QUEUE, free);
  queue_ptr->hooks = hooks;
  return queue_ptr;
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_queue_destroy_callback(rif_val_t *val_ptr) {
  rif_queue_t *queue_ptr = rif_queue_fromval(val_ptr);
  rif_hook_noreturn(destroy, queue_ptr);
}

uint32_t rif_queue_hashcode_callback(const rif_val_t *val_ptr) {
  return rif_hash_64((uint64_t) val_ptr);
}

char * rif_queue_tostring_callback(const rif_val_t *val_ptr) {
rif_queue_t *queue_ptr = rif_queue_fromval(val_ptr);
  char *tostring_str = rif_malloc(sizeof(char) * 28);
  if (!tostring_str) {
    return tostring_str;
  }
  snprintf(tostring_str, 28, "<Queue [items: %u]>", rif_queue_size(queue_ptr));
  return tostring_str;
}