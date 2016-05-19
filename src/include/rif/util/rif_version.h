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

/**
 * @file
 * @brief Rif base value with reference counting.
 */

#pragma once

#include "rif/internal/rif_macros.h"
#include "rif/internal/rif_libs.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * ABI SUPPORT
 *
 * Don't change this unless you know exactly what you're doing and have read:
 * http://www.gnu.org/software/libtool/manual/html_node/Libtool-versioning.html
 * http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
 */

/*
 * The ABI version structure.
 */
typedef struct rif_abi_version_s {

  /* The current interface version. */
  uint16_t current;

  /* The latest revision of the current interface. */
  uint16_t revision;

  /* How many past interface versions are still supported. */
  uint16_t age;

} rif_abi_version_t;

/**
 * @private
 *
 * The current interface version.
 */
#define RIF_ABI_VERSION_CURRENT   0

/**
 * @private
 *
 *The latest revision of the current interface.
 */
#define RIF_ABI_VERSION_REVISION  0

/**
 * @private
 *
 *How many past interface versions are still supported.
 */
#define RIF_ABI_VERSION_AGE       0

/**
 * @private
 *
 * The current ABI version.
 */
#define RIF_ABI_VERSION \
    { \
      .current = RIF_ABI_VERSION_CURRENT, \
      .revision = RIF_ABI_VERSION_REVISION, \
      .age = RIF_ABI_VERSION_AGE \
    }

/******************************************************************************
 * VERSION INFORMATION
 */

/**
 * The package version structure.
 */
typedef struct rif_package_version_s {

  /* The version major. */
  uint16_t major;

  /* The version minor. */
  uint16_t minor;

} rif_package_version_t;

/**
 * @private
 *
 * The package version major. */
#define RIF_PACKAGE_VERSION_MAJOR 0

/**
 * @private
 *
 * The package version minor.
 */
#define RIF_PACKAGE_VERSION_MINOR 1

/**
 * @private
 *
 * The current package version.
 */
#define RIF_PACKAGE_VERSION \
    { \
      .major = RIF_PACKAGE_VERSION_MAJOR, \
      .minor = RIF_PACKAGE_VERSION_MINOR, \
    }

/******************************************************************************
 * VERSION ACCESSORS
 */

/**
 * Returns the package version number of Rif.
 *
 * @return the package version number
 */
RIF_API
const rif_package_version_t * rif_get_package_version(void);

/**
 * Returns the ABI version number of Rif.
 *
 * @return the ABI version number
 */
RIF_API
const rif_abi_version_t * rif_get_abi_version(void);

/**
 * Checks whether the compiled library is ABI compatible with the included headers.
 *
 * @return `true` if the compiled library is ABI compatible with the included headers
 */
RIF_INLINE
bool rif_is_abi_version_compatible(void) {
  const rif_abi_version_t *abi_version = rif_get_abi_version();
  return abi_version->current - abi_version->age == RIF_ABI_VERSION_CURRENT - RIF_ABI_VERSION_AGE;
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
