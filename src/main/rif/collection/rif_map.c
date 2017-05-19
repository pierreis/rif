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

#include "rif/collection/rif_map.h"
#include "rif/collection/rif_map_iterator.h"
#include "rif/util/rif_hash.h"

/******************************************************************************
 * DATA TYPES
 */

struct rif_map_tostring_s {
  char *key_str;
  uint32_t key_len;
  char *val_str;
  uint32_t val_len;
};

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_map_t * rif_map_init(rif_map_t *map_ptr, const rif_map_hooks_t *hooks, bool free) {
  rif_val_init(rif_val(map_ptr), RIF_MAP, free);
  map_ptr->hooks = hooks;
  return map_ptr;
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_map_destroy_callback(rif_val_t *val_ptr) {
  rif_map_t *map_ptr = rif_map_fromval(val_ptr);
  rif_hook(destroy, 0, map_ptr);
}

uint32_t rif_map_hashcode_callback(const rif_val_t *val_ptr) {
  rif_map_t *map_ptr = rif_map_fromval(val_ptr);
  rif_map_iterator_t it;
  rif_pair_t pair;
  rif_map_iterator_init(&it, map_ptr, &pair);
  rif_iterator_t *it_ptr = (rif_iterator_t *) &it;
  uint32_t hash = 0;
  while (rif_iterator_hasnext(it_ptr)) {
    rif_val_t *element_ptr = rif_iterator_next(it_ptr);
    uint32_t element_hash = element_ptr ? rif_val_hashcode(element_ptr) : 31;
    hash += element_hash;
  }
  rif_iterator_destroy(it_ptr);
  return rif_hash_mix_32(37, hash);
}

bool rif_map_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  rif_map_t *first_ptr = rif_map_fromval(val_ptr);
  rif_map_t *second_ptr = rif_map_fromval(other_ptr);
  rif_map_iterator_t first_it;
  rif_pair_t pair_ptr;
  rif_map_iterator_init(&first_it, first_ptr, &pair_ptr);
  rif_iterator_t *first_it_ptr = (rif_iterator_t *) &first_it;
  if (rif_map_size(first_ptr) != rif_map_size(second_ptr)) {
    return false;
  }
  while (rif_iterator_hasnext(first_it_ptr)) {
    rif_pair_t *pair_ptr = rif_pair_fromval(rif_iterator_next((rif_iterator_t *) &first_it));
    if (!rif_val_equals(rif_pair_1(pair_ptr), rif_map_get(second_ptr, rif_pair_2(pair_ptr)))) {
      return false;
    }
  }
  rif_val_release(&first_it);
  return true;
}

char * rif_map_tostring_callback(const rif_val_t *val_ptr) {

  // Collect element string representations and sizes.
  rif_map_t *map_ptr = rif_map_fromval(val_ptr);
  uint32_t map_size = rif_map_size(map_ptr);
  char *tostring_str = NULL;
  struct rif_map_tostring_s element_tostring[map_size];
  rif_map_iterator_t it;
  rif_pair_t pair;
  rif_map_iterator_init(&it, map_ptr, &pair);
  rif_iterator_t *it_ptr = (rif_iterator_t *) &it;
  uint32_t tostring_str_len = 3;
  uint32_t i = 0;
  while (rif_iterator_hasnext(it_ptr)) {
    rif_pair_t *element_ptr = rif_pair_fromval(rif_iterator_next(it_ptr));
    element_tostring[i].key_str = rif_val_tostring(rif_pair_2(element_ptr));
    element_tostring[i].val_str = rif_val_tostring(rif_pair_1(element_ptr));
    if (!element_tostring[i].key_str || !element_tostring[i].val_str) {
      goto CLEANUP_EXIT;
    }
    element_tostring[i].key_len = (uint32_t) strlen(element_tostring[i].key_str);
    element_tostring[i].val_len = (uint32_t) strlen(element_tostring[i].val_str);
    tostring_str_len += element_tostring[i].key_len + element_tostring[i].val_len + 4;
    ++i;
  }

  // Create new string.
  tostring_str = rif_malloc(sizeof(char) * tostring_str_len, "RIF_MAP_TOSTRING");
  if (!tostring_str) {
    goto CLEANUP_EXIT;
  }
  uint32_t pos = 1;
  strcpy(tostring_str, "{");
  uint32_t j = 0;
  for (; j < map_size; ++j) {
    strcpy(tostring_str + pos, element_tostring[j].key_str);
    pos += element_tostring[j].key_len;
    strncpy(tostring_str + pos, ": ", 2);
    pos += 2;
    strcpy(tostring_str + pos, element_tostring[j].val_str);
    pos += element_tostring[j].val_len;
    strncpy(tostring_str + pos, ", ", 2);
    pos += 2;
  }
  strcpy(tostring_str + pos - (pos == 1 ? 0 : 2), "}");

  CLEANUP_EXIT:;

  // Free underlying string representations.
  uint32_t k = 0;
  for (; k < i; ++k) {
    if (element_tostring[k].key_str != rif_undef_str) {
      rif_free(element_tostring[k].key_str);
    }
    if (element_tostring[k].val_str != rif_undef_str) {
      rif_free(element_tostring[k].val_str);
    }
  }

  return tostring_str;
}