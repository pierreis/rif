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

#include "../test_internal.h"

/******************************************************************************
 * TESTS
 */

TEST(Version, rif_get_package_version_should_return_version_number) {
  const rif_package_version_t *package_ver_ptr = rif_get_package_version();
  EXPECT_EQ(RIF_PACKAGE_VERSION_MAJOR, package_ver_ptr->major);
  EXPECT_EQ(RIF_PACKAGE_VERSION_MINOR, package_ver_ptr->minor);
}

TEST(Version, rif_get_abi_version_should_return_version_number) {
  const rif_abi_version_t *abi_ver_ptr = rif_get_abi_version();
  EXPECT_EQ(RIF_ABI_VERSION_CURRENT, abi_ver_ptr->current);
  EXPECT_EQ(RIF_ABI_VERSION_REVISION, abi_ver_ptr->revision);
  EXPECT_EQ(RIF_ABI_VERSION_AGE, abi_ver_ptr->age);
}

TEST(Version, rif_is_abi_version_compatible_should_be_true) {
  EXPECT_EQ(true, rif_is_abi_version_compatible());
}