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
 * @brief Rif base value with reference counting.
 */

#pragma once

#include "rif/rif_common.h"
#include "rif/concurrent/rif_atomic.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * Value subtype type tokens.
 *
 * Each value subtype type token except for @ref RIF_UNDEF has a one-to-one mapping with a corresponding @ref rif_val_t
 * subtype.
 */
typedef enum rif_val_type_t {

    RIF_UNDEF  = 0,  /**< Undefined value type */
    RIF_NULL   = 1,  /**< Value of type @ref rif_null_t */
    RIF_BOOL   = 2,  /**< Value of type @ref rif_bool_t */
    RIF_INT    = 3,  /**< Value of type @ref rif_int_t */
    RIF_DOUBLE = 4,  /**< Value of type @ref rif_double_t */
    RIF_STRING = 5,  /**< Value of type @ref rif_string_t */
    RIF_PTR    = 6,  /**< Value of type @ref rif_ptr_t */
    RIF_LIST   = 7,  /**< Value of type @ref rif_list_t */
    RIF_MAP    = 8,  /**< Value of type @ref rif_map_t */
    RIF_QUEUE  = 9,  /**< Value of type @ref rif_queue_t */
    RIF_PAIR   = 10, /**< Value of type @ref rif_pair_t */
    RIF_BUFFER = 11  /**< Value of type @ref rif_buffer_t */

} rif_val_type_t;

/**
 * Rif base value interface type.
 *
 * This is the type all upper-level Rif value types inherit from. Semantically, a value can be of any type represented
 * by @ref rif_val_type_t.
 *
 * A value wraps a thread-safe reference counter, used to determine when a value can be safely freed.
 * This is especially useful used in combination with Rif collections, to delegate memory management of heap-allocated
 * values to the collection structure, ensuring that the value is freed automatically when the value is not referenced
 * anymore. The reference counter is manipulated using the @ref rif_val_retain and @rif_val_release functions.
 *
 * Unless mentioned otherwise, all the value-level functions accept as parameter any @ref rif_val_t subtype without
 * requiring explicit type casting.
 *
 * @note Internal members are private, and may change without notice.
 *       They should only be accessed through the public @ref rif_val_t methods.
 */
typedef struct rif_val_t {

  /**
   * @private
   *
   * The reference count.
   */
  atomic_uint32_t reference_count;

  /**
   * @private
   *
   * The value subtype type.
   */
  rif_val_type_t type;

  /**
   * @private
   *
   * Whether or not to free the variable when the reference count reaches `0`.
   */
  bool free;

} rif_val_t;

/******************************************************************************
 * GLOBALS
 */

/**
 * @private
 *
 * String representation for undefined value types.
 */
extern const char *rif_undef_str;

/******************************************************************************
 * MACROS
 */

/**
 * Cast a value subtype to @ref rif_val_t.
 *
 * @param  __val_ptr the @ref rif_val_t subtype
 * @return           the casted @ref rif_val_t
 *
 * @relates rif_val_t
 */
#define rif_val(__val_ptr) ((rif_val_t *) __val_ptr)

/**
 * Return the type of a value.
 *
 * @param __val_ptr the value to get the type of.
 * @return          the value type of @a __val_ptr,
 *                  or @ref RIF_UNDEF if either the value type is unknown or @a __val_ptr is `NULL`.
 *
 * @relates rif_val_t
 */
#define rif_val_type(__val_ptr) (__val_ptr ? (rif_val(__val_ptr)->type) : RIF_UNDEF)

/**
 * Retain a value.
 *
 * Increments the reference count of @a __val_ptr by one.
 *
 * @param __val_ptr the value to retain.
 * @return          the value @a __val_ptr.
 *
 * @relates rif_val_t
 */
#define rif_val_retain(__val_ptr) (rif_val_retain_helper(rif_val(__val_ptr)))

/**
 * Release a value.
 *
 * Decrements the reference count of @a __val_ptr by one.
 * If the reference count of @a __val_ptr reaches `0` and the value is heap-allocated, it will be freed.
 *
 * @param __val_ptr the value to release.
 * @return          the value @a __val_ptr if the decremented reference count is strictly greater than zero,
 *                  or `NULL` otherwise.
 *
 * @relates rif_val_t
 */
#define rif_val_release(__val_ptr) (rif_val_release_helper(rif_val(__val_ptr)))

/**
 * Get the hashcode of a value.
 *
 * @param __val_ptr the value to get the hashcode for.
 * @return          the hashcode value of @a __val_ptr.
 *
 * @relates rif_val_t
 */
#define rif_val_hashcode(__val_ptr) (rif_val_hashcode_helper(rif_val(__val_ptr)))

/**
 * Check for the semantic equality of two values.
 *
 * The exact meaning of equality for each value depends on the @ref rif_val_t subtype implementation.
 *
 * @param __val_ptr   the first value to compare.
 * @param __other_ptr the second value to compare.
 * @return            `true` if @a __val_ptr semantically equals to @a __other_ptr,
 *                    or `false` otherwise
 *
 * @relates rif_val_t
 */
#define rif_val_equals(__val_ptr, __other_ptr) \
    (rif_val_equals_helper(rif_val(__val_ptr), rif_val(__other_ptr)))

/**
 * Get the string representation of a value.
 *
 * The exact string representation format for each value depends on the @ref rif_val_t subtype implementation.
 *
 * @param __val_ptr the value to get the string representation for.
 * @return          the string representation of @a __val_ptr,
 *                  or `NULL` in case of error.
 *
 * @relates rif_val_t
 */
#define rif_val_tostring(__val_ptr) (rif_val_tostring_helper(rif_val(__val_ptr)))

/**
 * Return the reference count of a value.
 *
 * @param __val_ptr the value to get the reference count of.
 * @return          the reference count of @a __val_ptr.
 *
 * @relates rif_val_t
 */
#define rif_val_reference_count(__val_ptr) (atomic_load(&(rif_val(__val_ptr))->reference_count))

/******************************************************************************
 * API
 */

/**
 * @private
 *
 * Initialize a value.
 *
 * @note This function should only be used by @ref rif_val_t subtypes.
 *
 * @param val_ptr the value to initialize.
 * @param type    the value subtype type token.
 * @param free    whether or not to free the value when the reference count reaches `0`
 * @return        the initialized value,
 *                or `NULL` if initialization failed.
 *
 * @relates rif_val_t
 */
rif_val_t * rif_val_init(rif_val_t *val_ptr, rif_val_type_t type, bool free);

/******************************************************************************
 * INTERNAL
 */

/**
 * @private
 *
 * Cast a @ref rif_val_t to a value subtype.
 *
 * @pre @a __val_ptr @b MUST be of type @a __val_type, represented by the value subtype type token @a __val_type_token,
 *      or `NULL`.
 *
 * @param __val_ptr        the @ref rif_val_t.
 * @param __val_type_token the value type.
 * @param __val_type       the value subtype to cast to.
 * @return                 the casted subtype for @a __val_ptr, or `NULL` if cast is incorrect.
 *
 * @relates rif_val_t
 */
#define rif_val_tosubtype(__val_ptr, __val_type_token, __val_type) \
    ((__val_type *) rif_val_tosubtype_helper((rif_val_t *) (__val_ptr), (__val_type_token)))

/**
 * @private
 *
 * Helper function to retain a value.
 *
 * @memberof rif_val_t
 */
rif_val_t * rif_val_retain_helper(rif_val_t *val_ptr);

/**
 * @private
 *
 * Helper function to release a value.
 *
 * @memberof rif_val_t
 */
rif_val_t * rif_val_release_helper(rif_val_t *val_ptr);

/**
 * @private
 *
 * Helper function get the hashcode of a value.
 *
 * @memberof rif_val_t
 */
uint32_t rif_val_hashcode_helper(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Helper function check for the equality of two values.
 *
 * @memberof rif_val_t
 */
bool rif_val_equals_helper(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Helper function get the string representation of a value.
 *
 * @memberof rif_val_t
 */
char * rif_val_tostring_helper(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Checks a value type and returns the value if it matches.
 *
 * @param val_ptr           the value to check.
 * @param expected_val_type the expected value type.
 * @return                  the value @a val if it matches the expected value type,
 *                          or `NULL` otherwise.
 *
 * @memberof rif_val_t
 */
RIF_INLINE
rif_val_t * rif_val_tosubtype_helper(rif_val_t *val_ptr, rif_val_type_t expected_val_type) {
  if (!val_ptr) {
    return val_ptr;
  }
  assert(val_ptr->type == expected_val_type);
  return val_ptr;
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
