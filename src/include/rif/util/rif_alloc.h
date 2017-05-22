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

/**
 * @file
 * @brief Rif allocator management.
 */

#pragma once

#include "rif/rif_common.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * ALLOCATOR SETTER
 */

/**
 * Set the allocators used by Rif.
 *
 * @param f_malloc  the malloc function, or `NULL` to keep the previously defined one.
 * @param f_realloc the realloc function, or `NULL` to keep the previously defined one.
 * @param f_free    the free function, or `NULL` to keep the previously defined one.
 */
RIF_API
void rif_set_allocators(void * (*f_malloc)(size_t), void * (*f_realloc)(void *, size_t), void (*f_free)(void *));

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
