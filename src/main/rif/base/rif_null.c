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

#include "rif/base/rif_null.h"

/*****************************************************************************
 * GLOBAL CONSTANTS
 */

static const rif_null_t _rif_null = {
    ._ = {
        .reference_count = ATOMIC_VAR_INIT(0),
        .type = RIF_NULL,
        .free = false
    }
};

const rif_null_t *rif_null = &_rif_null;

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

char * rif_null_tostring_callback(const rif_val_t *val_ptr) {
  return rif_strdup("NULL");
}
