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

#include "rif/base/rif_int.h"
#include "rif/util/rif_hash.h"

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_int_t * rif_int_build(rif_int_t *int_ptr, bool free, int64_t value) {
  if (!int_ptr) {
    return int_ptr;
  }
  rif_val_init(rif_val(int_ptr), RIF_INT, free);
  int_ptr->value = value;
  return int_ptr;
}

rif_int_t * rif_int_init(rif_int_t *int_ptr, int64_t value) {
  return rif_int_build(int_ptr, false, value);
}

rif_int_t * rif_int_new(int64_t value) {
  rif_int_t *int_ptr = rif_malloc(sizeof(rif_int_t), "RIF_INT_NEW");
  return rif_int_build(int_ptr, true, value);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_int_destroy_callback(rif_val_t *val_ptr) {
  rif_int_t *int_ptr = rif_int_fromval(val_ptr);
  int_ptr->value = 0;
}

uint32_t rif_int_hashcode_callback(const rif_val_t *val_ptr) {
  rif_int_t *int_ptr = rif_int_fromval(val_ptr);
  return (uint32_t) rif_hash_64(13 * ((uint64_t) rif_int_get(int_ptr)));
}

bool rif_int_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  rif_int_t *int_ptr = rif_int_fromval(val_ptr);
  rif_int_t *other_int_ptr = rif_int_fromval(other_ptr);
  return rif_int_get(int_ptr) == rif_int_get(other_int_ptr);
}

char * rif_int_tostring_callback(const rif_val_t *val_ptr) {
  rif_int_t *int_ptr = rif_int_fromval(val_ptr);
  char *tostring_str = rif_malloc(sizeof(char) * 32, "RIF_INT_TOSTRING");
  if (!tostring_str) {
    return tostring_str;
  }
  memset(tostring_str, 0, 32);
  snprintf(tostring_str, sizeof(char) * 32, "%" PRId64, rif_int_get(int_ptr));
  return tostring_str;
}