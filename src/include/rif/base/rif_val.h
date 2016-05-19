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

#include "rif/concurrent/rif_atomic.h"

/*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 */

/**
 * Rif val types.
 */
typedef enum rif_val_type_e {

    RIF_UNDEF  = 0,
    RIF_NULL   = 1,
    RIF_BOOL   = 2,
    RIF_INT    = 3,
    RIF_DOUBLE = 4,
    RIF_STRING = 5,
    RIF_PTR    = 6,
    RIF_LIST   = 7,
    RIF_MAP    = 8,
    RIF_QUEUE  = 9,
    RIF_PAIR   = 10,
    RIF_BUFFER = 11

} rif_val_type_t;

/**
 * Rif val base interface type.
 *
 * @note This structure internal members are private, and may change without notice. They should only be accessed
 *       through the public `rif_val_t` methods.
 */
typedef struct rif_val_s {

  /**
   * @private
   *
   * The reference count.
   */
  atomic_uint32_t reference_count;

  /**
   * @private
   *
   * The value type.
   */
  rif_val_type_t type;

  /**
   * @private
   *
   * Whether or not to free the variable when the reference count reaches 0.
   */
  bool free;

} rif_val_t;

/******************************************************************************
 * GLOBAL CONSTANTS
 */

/**
 * @private
 *
 * Undef string representation.
 */
extern const char *rif_undef_str;

/******************************************************************************
 * HELPER MACROS
 */

/**
 * Cast a value subtype to `rif_val_t`.
 *
 * @param  __val_ptr the `rif_val_t` subtype
 * @return           the casted `rif_val_t`
 *
 * @relates rif_val_t
 */
#define rif_val(__val_ptr) ((rif_val_t *) __val_ptr)

/**
 * Attempts to cast a `rif_val_t` to a value subtype. Returns `NULL` if cast fails.
 *
 * @param  __val_ptr the `rif_val_t`
 * @param __val_type the expected value type
 * @param __type     the type to cast to
 * @return           the casted subtype, or `NULL` if cast is incorrect
 *
 * @relates rif_val_t
 */
#define rif_val_tosubtype(__val_ptr, __val_type, __type) \
    ((__type *) rif_val_tosubtype_helper((rif_val_t *) (__val_ptr), (__val_type)))

/**
 * Return the type of a value.
 *
 * @param __val_ptr the value to get the type of
 * @return          the value type. If the type is unknown, it will be RIF_UNDEF
 *
 * @relates rif_val_t
 */
#define rif_val_type(__val_ptr) (__val_ptr ? (rif_val(__val_ptr)->type) : RIF_UNDEF)

/**
 * Increment the reference count of a value.
 *
 * @param __val_ptr the value to retain
 * @return          the value
 *
 * @relates rif_val_t
 */
#define rif_val_retain(__val_ptr) (rif_val_retain_helper(rif_val(__val_ptr)))

/**
 * Decrement the reference count of a value.
 *
 * If `rif_val_t.reference_count` reaches 0 and `rif_val_t.free` is true, the value will be freed.
 *
 * @param __val_ptr the value to release
 * @return          the value if `rif_val_t.reference_count > 0`, otherwise NULL
 *
 * @relates rif_val_t
 */
#define rif_val_release(__val_ptr) (rif_val_release_helper(rif_val(__val_ptr)))

/**
 * Get the hashcode of a value.
 *
 * @param __val_ptr the value to get the hashcode for
 * @return          the hashcode value
 *
 * @relates rif_val_t
 */
#define rif_val_hashcode(__val_ptr) (rif_val_hashcode_helper(rif_val(__val_ptr)))

/**
 * Check for the equality of two values.
 *
 * @param __val_ptr   the first value to compare
 * @param __other_ptr the second value to compare
 * @return            `true` if `__val_ptr` is equal to `__other_ptr`, or `false` otherwise
 */
#define rif_val_equals(__val_ptr, __other_ptr) \
    (rif_val_equals_helper(rif_val(__val_ptr), rif_val(__other_ptr)))

/**
 * Get the string representation of a value.
 *
 * @param __val_ptr the value to get the string representation for
 * @return          the string representation on success, otherwise `NULL`
 *
 * @relates rif_val_t
 */
#define rif_val_tostring(__val_ptr) (rif_val_tostring_helper(rif_val(__val_ptr)))

/**
 * Return the reference count of a value.
 *
 * @param __val_ptr the value to get the reference count of
 * @return          the reference count
 *
 * @relates rif_val_t
 */
#define rif_val_reference_count(__val_ptr) (atomic_load(&(rif_val(__val_ptr))->reference_count))

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

/**
 * @private
 *
 * Initialize a value.
 *
 * This function should only be used by `rif_val_t` subtypes.
 *
 * @param val  the value to initialize
 * @param type the value type
 * @param free whether or not to free the value when the reference count reaches 0
 * @return     the initialized value, or `NULL` if initialization failed
 *
 * @relates rif_val_t
 */
rif_val_t * rif_val_init(rif_val_t *val_ptr, rif_val_type_t type, bool free);

/******************************************************************************
 * HELPER FUNCTIONS
 */

/**
 * @private
 *
 * Helper function to retain a value.
 *
 * @relates rif_val_t
 */
rif_val_t * rif_val_retain_helper(rif_val_t *val_ptr);

/**
 * @private
 *
 * Helper function to release a value.
 *
 * @relates rif_val_t
 */
rif_val_t * rif_val_release_helper(rif_val_t *val_ptr);

/**
 * @private
 *
 * Helper function get the hashcode of a value.
 *
 * @relates rif_val_t
 */
uint32_t rif_val_hashcode_helper(const rif_val_t *val_ptr);

/**
 * @private
 *
 * Helper function check for the equality of two values.
 *
 * @relates rif_val_t
 */
bool rif_val_equals_helper(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * @private
 *
 * Helper function get the string representation of a value.
 *
 * @relates rif_val_t
 */
char * rif_val_tostring_helper(const rif_val_t *val_ptr);

/******************************************************************************
 * SUBTYPE FUNCTIONS
 */

/**
 * @private
 *
 * Checks a value type and returns the value if it matches. Returns `NULL` otherwise.
 *
 * @param val_ptr           the value to check
 * @param expected_val_type the expected value type
 * @return                  the value if it matches the expected value type, or `NULL` otherwise
 *
 * @relates rif_val_t
 */
RIF_INLINE
rif_val_t * rif_val_tosubtype_helper(rif_val_t *val_ptr, rif_val_type_t expected_val_type) {
  return val_ptr->type == expected_val_type ? val_ptr : NULL;
}

/*****************************************************************************/

#ifdef __cplusplus
} /* extern "C" */
#endif
