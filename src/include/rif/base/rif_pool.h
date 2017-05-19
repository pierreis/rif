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
 * @brief Rif memory pool.
 */

#pragma once

#include "rif/rif_common.h"
#include "rif/util/rif_hook.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/* Forward declaration of `rif_pool_hooks_t`. */
typedef struct rif_pool_hooks_t rif_pool_hooks_t;

/**
 * Rif pool.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_pool_t` methods.
 */
typedef struct rif_pool_t {

  /**
   * @private
   *
   * Hooks used by `rif_pool_t` functions.
   */
  const rif_pool_hooks_t * hooks;

} rif_pool_t;

/**
 * @private
 *
 * Hooks used by @ref rif_pool_t implementations.
 */
struct rif_pool_hooks_t {

  /**
   * Destroy callback.
   */
  void (*destroy)(rif_pool_t *pool_ptr);

  /**
   * @see rif_pool_borrow
   */
  void *(*borrow)(rif_pool_t *pool_ptr);

  /**
   * @see rif_pool_return
   */
  void (*return_)(rif_pool_t *pool_ptr, void *ptr);

};

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * Initialize a pool.
 *
 * This function should only be used by `rif_pool_t` subtypes.
 *
 * @param pool_ptr the pool to initialize.
 * @param hooks pool hooks
 * @return the initialized pool, or `NULL` if initialization failed.
 *
 * @relates rif_pool_t
 */
RIF_INLINE
rif_pool_t * rif_pool_init(rif_pool_t *pool_ptr, const rif_pool_hooks_t *hooks) {
  pool_ptr->hooks = hooks;
  return pool_ptr;
}

/**
 * Destroy a pool
 *
 * @param pool_ptr pool to destroy
 */
RIF_INLINE
void rif_pool_destroy(rif_pool_t *pool_ptr) {
  rif_hook_noreturn(destroy, pool_ptr);
}

/******************************************************************************
 * ACCESSOR FUNCTIONS
 */

/**
 * Borrow an element from the pool
 *
 * If the pool doesn't have available elements, a new memory block is allocated.
 *
 * @param pool_ptr the pool
 *
 * @return an element from the pool, or `NULL` in case of allocation failure
 */
RIF_INLINE
void * rif_pool_borrow(rif_pool_t *pool_ptr) {
  return rif_hook(borrow, 0, pool_ptr);
}

/**
 * Return a borrowed element back to the pool
 *
 * If `ptr` is `NULL`, the function has no effect. No additional memory check is done on the returned elements. If `ptr`
 * was not previously borrowed from the pool or is returned multiple times, the behavior is undefined.
 *
 * @param pool_ptr the pool
 * @param ptr element to return to the pool
 */
RIF_INLINE
void rif_pool_return(rif_pool_t *pool_ptr, void *ptr) {
  rif_hook_noreturn(return_, pool_ptr, ptr);
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
