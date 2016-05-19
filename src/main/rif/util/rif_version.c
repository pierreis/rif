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
 * VERSION ACCESSORS
 */

static const rif_abi_version_t _rif_abi_version = RIF_ABI_VERSION;
static const rif_package_version_t _rif_package_version = RIF_PACKAGE_VERSION;

/******************************************************************************
 * VERSION ACCESSORS
 */

const rif_package_version_t * rif_get_package_version(void) {
  return &_rif_package_version;
}

const rif_abi_version_t * rif_get_abi_version(void) {
  return &_rif_abi_version;
}