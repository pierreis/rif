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

#pragma once

#include "rif/rif_internal.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * CALLBACK FUNCTIONS
 */

/**
 * @private
 *
 * Callback function to destroy a `rif_arraylist_t`.
 */
void rif_arraylist_destroy_callback(rif_arraylist_t *al_ptr);

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
