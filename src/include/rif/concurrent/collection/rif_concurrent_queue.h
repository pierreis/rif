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
 * @brief Rif concurrent queue.
 */

#pragma once

#include "rif/base/rif_val.h"
#include "rif/concurrent/rif_atomic.h"
#include "rif/concurrent/rif_concurrent_pool.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * @private
 *
 * Rif concurrent queue node.
 */
typedef struct rif_concurrent_queue_node_s {

  /**
   * @private
   *
   * Element size.
   */
  atomic_uint32_t *reference_count;

  /**
   * @private
   *
   * Pointer to the successor node.
   */
  struct rif_concurrent_queue_node_s *succ;

} rif_concurrent_queue_node_t;

/**
 * @private
 *
 * Rif concurrent queue.
 */
typedef struct rif_concurrent_queue_t {

  /**
   * @private
   *
   * Pointer to the first node.
   */
  rif_concurrent_queue_node_t *first;

  /**
   * @private
   *
   * Memory pool.
   */
  rif_concurrent_pool_t pool;

} rif_concurrent_queue_t;