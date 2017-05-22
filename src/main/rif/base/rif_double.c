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

#include "rif/base/rif_double.h"
#include "rif/util/rif_hash.h"

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_double_t * rif_double_build(rif_double_t *double_ptr, bool free, double value) {
  if (!double_ptr) {
    return double_ptr;
  }
  rif_val_init(rif_val(double_ptr), RIF_DOUBLE, free);
  double_ptr->value = value;
  return double_ptr;
}

rif_double_t * rif_double_init(rif_double_t *double_ptr, double value) {
  return rif_double_build(double_ptr, false, value);
}

rif_double_t * rif_double_new(double value) {
  rif_double_t *double_ptr = rif_malloc(sizeof(rif_double_t));
  return rif_double_build(double_ptr, true, value);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_double_destroy_callback(rif_val_t *val_ptr) {
  rif_double_t *double_ptr = rif_double_fromval(val_ptr);
  double_ptr->value = 0;
}

uint32_t rif_double_hashcode_callback(const rif_val_t *val_ptr) {
  rif_double_t *double_ptr = rif_double_fromval(val_ptr);
  return rif_hash_64((uint64_t) double_ptr->value);
}

bool rif_double_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  rif_double_t *double_ptr = rif_double_fromval(val_ptr);
  rif_double_t *other_double_ptr = rif_double_fromval(other_ptr);
  return rif_double_get(double_ptr) == rif_double_get(other_double_ptr);
}

char * rif_double_tostring_callback(const rif_val_t *val_ptr) {
  rif_double_t *double_ptr = rif_double_fromval(val_ptr);
  char *tostring_str = rif_malloc(sizeof(char) * 64);
  if (!tostring_str) {
    return tostring_str;
  }
  memset(tostring_str, 0, 64);
  snprintf(tostring_str, sizeof(char) * 64, "%.16g", rif_double_get(double_ptr));
  if (!strchr(tostring_str, '.')) {
    size_t tostring_str_len = strlen(tostring_str);
    strcpy(tostring_str + tostring_str_len, ".0\0");
  }
  return tostring_str;
}