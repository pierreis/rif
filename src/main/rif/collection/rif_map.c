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