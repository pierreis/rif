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

#include "rif/base/rif_pair.h"
#include "rif/util/rif_hash.h"

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_pair_t *rif_pair_build(rif_pair_t *pair_ptr, bool free, rif_val_t *val_ptr_1, rif_val_t *val_ptr_2) {
  if (!pair_ptr) {
    return pair_ptr;
  }
  rif_val_init(rif_val(pair_ptr), RIF_PAIR, free);
  rif_val_retain(val_ptr_1);
  rif_val_retain(val_ptr_2);
  pair_ptr->val_ptr_1 = val_ptr_1;
  pair_ptr->val_ptr_2 = val_ptr_2;
  return pair_ptr;
}

rif_pair_t *rif_pair_init(rif_pair_t *pair_ptr, rif_val_t *val_ptr_1, rif_val_t *val_ptr_2) {
  return rif_pair_build(pair_ptr, false, val_ptr_1, val_ptr_2);
}

rif_pair_t *rif_pair_new(rif_val_t *val_ptr_1, rif_val_t *val_ptr_2) {
  rif_pair_t *pair_ptr = rif_malloc(sizeof(rif_pair_t), "RIF_PAIR_NEW");
  return rif_pair_build(pair_ptr, true, val_ptr_1, val_ptr_2);
}

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

void rif_pair_destroy_callback(rif_val_t *val_ptr) {
  rif_pair_t *pair_ptr = rif_pair_fromval(val_ptr);
  rif_val_release(rif_pair_1(pair_ptr));
  rif_val_release(rif_pair_2(pair_ptr));
}

uint32_t rif_pair_hashcode_callback(const rif_val_t *val_ptr) {
  rif_pair_t *pair_ptr = rif_pair_fromval(val_ptr);
  uint32_t hashcode_1 = rif_val_hashcode(rif_pair_1(pair_ptr));
  uint32_t hashcode_2 = rif_val_hashcode(rif_pair_2(pair_ptr));
  return 11 * rif_hash_mix_32(hashcode_1, hashcode_2);
}

bool rif_pair_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  rif_pair_t *pair_ptr = rif_pair_fromval(val_ptr);
  rif_pair_t *other_pair_ptr = rif_pair_fromval(other_ptr);
  return rif_val_equals(rif_pair_1(pair_ptr), rif_pair_1(other_pair_ptr)) &&
         rif_val_equals(rif_pair_2(pair_ptr), rif_pair_2(other_pair_ptr));
}

char *rif_pair_tostring_callback(const rif_val_t *val_ptr) {

  char *tostring_str = NULL;

  /* Get string representation of both members. */
  const rif_pair_t *pair_ptr = rif_pair_fromval(val_ptr);
  const rif_val_t *val_ptr_1 = rif_pair_1(pair_ptr);
  const rif_val_t *val_ptr_2 = rif_pair_2(pair_ptr);
  const char *tostring_1 = val_ptr_1 ? rif_val_tostring(rif_pair_1(pair_ptr)) : rif_undef_str;
  const char *tostring_2 = val_ptr_2 ? rif_val_tostring(rif_pair_2(pair_ptr)) : rif_undef_str;
  if (!tostring_1 || !tostring_2) {
    goto CLEANUP_EXIT;
  }

  /* Get each length and allocate memory. */
  size_t tostring_1_len = strlen(tostring_1);
  size_t tostring_2_len = strlen(tostring_2);
  size_t tostring_str_len = tostring_1_len + tostring_2_len + 5;
  tostring_str = rif_malloc(tostring_str_len * sizeof(char), "RIF_PAIR_TOSTRING");
  if (!tostring_str) {
    goto CLEANUP_EXIT;
  }

  /* Copy memory. */
  strcpy(tostring_str, "(");
  strcpy(tostring_str + 1, tostring_1);
  strcpy(tostring_str + 1 + tostring_1_len, ", ");
  strcpy(tostring_str + 1 + tostring_1_len + 2, tostring_2);
  strcpy(tostring_str + 1 + tostring_1_len + 2 + tostring_2_len, ")");
  *(tostring_str + 1 + tostring_1_len + 2 + tostring_2_len + 1) = '\0';

  CLEANUP_EXIT:

  /* Free underlying string representations. */
  if (rif_undef_str != tostring_1) {
    rif_free((void *) tostring_1);
  }
  if (rif_undef_str != tostring_2) {
    rif_free((void *) tostring_2);
  }

  return tostring_str;
}