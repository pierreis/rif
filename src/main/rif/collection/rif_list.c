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
 * DATA TYPES
 */

struct rif_list_tostring_s {
  char *str;
  uint32_t len;
};

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_list_t * rif_list_init(rif_list_t *list_ptr, const rif_list_hooks_t *hooks, bool free) {
  rif_val_init(rif_val(list_ptr), RIF_LIST, free);
  list_ptr->hooks = hooks;
  return list_ptr;
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_list_destroy_callback(rif_val_t *val_ptr) {
  rif_list_t *list_ptr = rif_list_fromval(val_ptr);
  rif_hook(destroy, 0, list_ptr);
}

uint32_t rif_list_hashcode_callback(const rif_val_t *val_ptr) {
  rif_list_t *list_ptr = rif_list_fromval(val_ptr);
  rif_list_iterator_t it;
  rif_list_iterator_init(&it, list_ptr);
  rif_iterator_t *it_ptr = (rif_iterator_t *) &it;
  uint32_t hash = 37;
  while (rif_iterator_hasnext(it_ptr)) {
    rif_val_t *element_ptr = rif_iterator_next(it_ptr);
    uint32_t element_hash = element_ptr ? rif_val_hashcode(element_ptr) : 31;
    hash = rif_hash_mix_32(hash, element_hash);
  }
  rif_iterator_destroy(it_ptr);
  return hash;
}

bool rif_list_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  rif_list_t *first_ptr = rif_list_fromval(val_ptr);
  rif_list_t *second_ptr = rif_list_fromval(other_ptr);
  rif_list_iterator_t first_it, second_it;
  rif_list_iterator_init(&first_it, first_ptr);
  rif_list_iterator_init(&second_it, second_ptr);
  rif_iterator_t *first_it_ptr = (rif_iterator_t *) &first_it;
  rif_iterator_t *second_it_ptr = (rif_iterator_t *) &second_it;
  while (rif_iterator_hasnext(first_it_ptr) && rif_iterator_hasnext(second_it_ptr)) {
    if (!rif_val_equals(rif_iterator_next(first_it_ptr), rif_iterator_next(second_it_ptr))) {
      return false;
    }
  }
  return rif_iterator_hasnext(first_it_ptr) == rif_iterator_hasnext(second_it_ptr);
}

char * rif_list_tostring_callback(const rif_val_t *val_ptr) {

  // Collect element string representations and sizes.
  rif_list_t *list_ptr = rif_list_fromval(val_ptr);
  uint32_t list_size = rif_list_size(list_ptr);
  char *tostring_str = NULL;
  struct rif_list_tostring_s element_tostring[list_size];
  uint32_t i = 0;
  uint32_t tostring_str_len = 0;
  for (; i < list_size; ++i) {
    rif_val_t *element_ptr = rif_list_get(list_ptr, i);
    element_tostring[i].str = element_ptr ? rif_val_tostring(element_ptr) : (char *) rif_undef_str;
    if (!element_tostring[i].str) {
      goto CLEANUP_EXIT;
    }
    element_tostring[i].len = (uint32_t) strlen(element_tostring[i].str);
    tostring_str_len += strlen(element_tostring[i].str) + 2;
  }

  // Create new string.
  tostring_str_len += 3;
  tostring_str = rif_malloc(sizeof(char) * tostring_str_len, "RIF_LIST_TOSTRING");
  if (!tostring_str) {
    goto CLEANUP_EXIT;
  }
  uint32_t pos = 1;
  strcpy(tostring_str, "[");
  for (uint32_t j = 0; j < list_size; ++j) {
    strcpy(tostring_str + pos, element_tostring[j].str);
    strncpy(tostring_str + pos + element_tostring[j].len, ", ", 2); // Do not copy the terminating NULL character.
    pos += element_tostring[j].len + 2;
  }
  strcpy(tostring_str + pos - (pos == 1 ? 0 : 2), "]");

  CLEANUP_EXIT:

  // Free underlying string representations.
  for (uint32_t k = 0; k < i; ++k) {
    if (element_tostring[k].str != rif_undef_str) {
      rif_free(element_tostring[k].str);
    }
  }

  return tostring_str;
}