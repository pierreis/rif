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
  return 0;
}

bool rif_list_equals_callback(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  return false;
}

char * rif_list_tostring_callback(const rif_val_t *val_ptr) {
  return NULL;
}