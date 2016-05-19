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

/*****************************************************************************
 * GLOBAL CONSTANTS
 */

static const rif_bool_t _rif_false = {
    ._ = {
        .reference_count = ATOMIC_VAR_INIT(0),
        .type = RIF_BOOL,
        .free = false
    },
    .value = false,
    .string = "FALSE",
    .hashcode = 0
};

const rif_bool_t *rif_false = &_rif_false;

static const rif_bool_t _rif_true = {
    ._ = {
        .reference_count = ATOMIC_VAR_INIT(0),
        .type = RIF_BOOL,
        .free = false
    },
    .value = true,
    .string = "TRUE",
    .hashcode = 1
};

const rif_bool_t *rif_true = &_rif_true;

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

uint32_t rif_bool_hashcode_callback(const rif_val_t *val_ptr) {
  return rif_bool_fromval(val_ptr)->hashcode;
}

char * rif_bool_tostring_callback(const rif_val_t *val_ptr) {
  return strdup(rif_bool_fromval(val_ptr)->string);
}