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

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_string_t *rif_string_build(rif_string_t *str_ptr, bool free, char *value, size_t len, bool value_free) {
  if (!str_ptr) {
    return NULL;
  }
  assert(len == SIZE_MAX || (!value && len == 0) || (value && strlen(value) == len));
  rif_val_init(rif_val(str_ptr), RIF_STRING, free);
  str_ptr->free = value_free;
  str_ptr->value = value;
  str_ptr->len = len;
  return str_ptr;
}

rif_string_t * rif_string_init_wlen(rif_string_t *str_ptr, char *value, size_t len, bool free) {
  return rif_string_build(str_ptr, false, value, len, free);
}

rif_string_t * rif_string_new_wlen(char *value, size_t len, bool free) {
  rif_string_t * str_ptr = rif_malloc(sizeof(rif_string_t), "RIF_STRING_NEW");
  return rif_string_build(str_ptr, true, value, len, free);
}

rif_string_t * rif_string_new_dup(const char *value) {
  if (!value) {
    return NULL;
  }
  char *dup = rif_strdup(value, "RIF_STRING_NEW_DUP");
  if (!dup) {
    return NULL;
  }
  rif_string_t *str_ptr = rif_string_new(dup, true);
  if (!str_ptr) {
    rif_free(dup);
  }
  return str_ptr;
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

size_t rif_string_len(rif_string_t *str_ptr) {
  if (str_ptr->len == SIZE_MAX) {
    str_ptr->len = str_ptr->value ? strlen(str_ptr->value) : 0;
  }
  return str_ptr->len;
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_string_destroy_callback(rif_val_t *val_ptr) {
  rif_string_t *str_ptr = rif_string_fromval(val_ptr);
  if (str_ptr->free) {
    rif_free(str_ptr->value);
  }
  str_ptr->value = NULL;
}

uint32_t rif_string_hashcode_callback(const rif_val_t *val_ptr) {
  rif_string_t *str_ptr = rif_string_fromval(val_ptr);
  if (!str_ptr || !str_ptr->value) {
    return 0;
  }
  uint32_t hash = 0;
  char c;
  const char *str_val = str_ptr->value;
  while ((c = *str_val++)) {
    hash = rif_hash_mix_32(hash, (uint32_t) c);
  }
  return hash;
}

bool rif_string_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  return false;
}

char * rif_string_tostring_callback(const rif_val_t *val_ptr) {
  rif_string_t *str_ptr = rif_string_fromval(val_ptr);
  if (!str_ptr->value) {
    return NULL;
  }
  size_t str_len = rif_string_len(str_ptr);
  size_t tostring_str_len = 3 + str_len;
  char * tostring_str = rif_malloc(tostring_str_len * sizeof(char), "RIF_STRING_TOSTRING");
  if (!tostring_str) {
    return NULL;
  }
  *(tostring_str + 0) = '\"';
  strncpy(tostring_str + 1, str_ptr->value, str_len);
  *(tostring_str + 1 + str_len) = '\"';
  *(tostring_str + 1 + str_len + 1) = '\0';
  return tostring_str;
}