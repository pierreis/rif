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
 * @brief Rif list iterator type.
 */

#pragma once

#include "rif/collection/rif_arraylist_iterator.h"
#include "rif/collection/rif_linkedlist_iterator.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * Rif list iterator union.
 */
union rif_list_iterator_u {

  rif_arraylist_iterator_t arraylist_iterator;
  rif_linkedlist_iterator_t linkedlist_iterator;

};

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
