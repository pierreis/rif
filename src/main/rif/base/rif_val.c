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
 * MACROS
 */

#define RIF_VAL_TYPE_COUNT 12

/******************************************************************************
 * TYPES
 */

/**
 * The type retain callback type.
 */
typedef rif_val_t * (*rif_val_retain_callback_t)(rif_val_t *val_ptr);

/**
 * The type release callback type.
 */
typedef rif_val_t * (*rif_val_release_callback_t)(rif_val_t *val_ptr);

/**
 * The type destroy callback type.
 */
typedef void (*rif_val_destroy_callback_t)(rif_val_t *val_ptr);

/**
 * The type hashcode callback type.
 */
typedef uint32_t (*rif_val_hashcode_callback_t)(const rif_val_t *val_ptr);

/**
 * The type equals callback type.
 */
typedef bool (*rif_val_equals_callback_t)(const rif_val_t *val_ptr, const rif_val_t *other_ptr);

/**
 * The type tostring callback type.
 */
typedef char * (*rif_val_tostring_callback_t)(const rif_val_t *val_ptr);

/******************************************************************************
 * GLOBAL CONSTANTS
 */

const char *rif_undef_str = "[UNDEF]";

/******************************************************************************
 * FUNCTION FORWARD DECLARATIONS
 */

static rif_val_t * _rif_val_retain_count(rif_val_t *val_ptr);
static rif_val_t * _rif_val_retain_noop(rif_val_t *val_ptr);
static rif_val_t * _rif_val_release_count(rif_val_t *val_ptr);
static rif_val_t * _rif_val_release_noop(rif_val_t *val_ptr);
static uint32_t _rif_val_hashcode_noop(const rif_val_t *val_ptr);
static bool _rif_val_equals_address(const rif_val_t *val_ptr, const rif_val_t *other_ptr);
static char * _rif_val_tostring_noop(const rif_val_t *val_ptr);

/******************************************************************************
 * STATIC CONSTANTS
 */

static const rif_val_retain_callback_t _rif_val_retain_callbacks[RIF_VAL_TYPE_COUNT] = {
    [RIF_UNDEF]  = _rif_val_retain_noop,
    [RIF_NULL]   = _rif_val_retain_noop,
    [RIF_BOOL]   = _rif_val_retain_noop,
    [RIF_INT]    = _rif_val_retain_count,
    [RIF_DOUBLE] = _rif_val_retain_count,
    [RIF_STRING] = _rif_val_retain_count,
    [RIF_PTR]    = _rif_val_retain_count,
    [RIF_LIST]   = _rif_val_retain_count,
    [RIF_MAP]    = _rif_val_retain_count,
    [RIF_QUEUE]  = _rif_val_retain_count,
    [RIF_PAIR]   = _rif_val_retain_count,
    [RIF_BUFFER] = _rif_val_retain_count
};

static const rif_val_release_callback_t _rif_val_release_callbacks[RIF_VAL_TYPE_COUNT] = {
    [RIF_UNDEF]  = _rif_val_release_noop,
    [RIF_NULL]   = _rif_val_release_noop,
    [RIF_BOOL]   = _rif_val_release_noop,
    [RIF_INT]    = _rif_val_release_count,
    [RIF_DOUBLE] = _rif_val_release_count,
    [RIF_STRING] = _rif_val_release_count,
    [RIF_PTR]    = _rif_val_release_count,
    [RIF_LIST]   = _rif_val_release_count,
    [RIF_MAP]    = _rif_val_release_count,
    [RIF_QUEUE]  = _rif_val_release_count,
    [RIF_PAIR]   = _rif_val_release_count,
    [RIF_BUFFER] = _rif_val_release_count
};

static const rif_val_destroy_callback_t _rif_val_destroy_callbacks[RIF_VAL_TYPE_COUNT] = {
    [RIF_INT]    = rif_int_destroy_callback,
    [RIF_DOUBLE] = rif_double_destroy_callback,
    [RIF_STRING] = rif_string_destroy_callback,
    //[RIF_PTR]      = rif_ptr_destroy_callback,
    //[RIF_LIST]     = rif_list_destroy_callback,
    [RIF_PAIR]   = rif_pair_destroy_callback,
    //[RIF_BUFFER]   = rif_buffer_destroy_callback
};

static const rif_val_hashcode_callback_t _rif_val_hashcode_callbacks[RIF_VAL_TYPE_COUNT] = {
    [RIF_UNDEF]  = _rif_val_hashcode_noop,
    [RIF_NULL]   = _rif_val_hashcode_noop,
    [RIF_BOOL]   = rif_bool_hashcode_callback,
    [RIF_INT]    = rif_int_hashcode_callback,
    [RIF_DOUBLE] = rif_double_hashcode_callback,
    [RIF_STRING] = rif_string_hashcode_callback,
    //[RIF_PTR]      = rif_ptr_hashcode_callback,
    //[RIF_LIST]     = rif_list_hashcode_callback,
    [RIF_PAIR]   = rif_pair_hashcode_callback,
    //[RIF_BUFFER]   = rif_buffer_hashcode_callback
};

static const rif_val_equals_callback_t _rif_val_equals_callbacks[RIF_VAL_TYPE_COUNT] = {
    [RIF_UNDEF]  = _rif_val_equals_address,
    [RIF_NULL]   = _rif_val_equals_address,
    [RIF_BOOL]   = _rif_val_equals_address,
    [RIF_INT]    = rif_int_equals_callback,
    [RIF_DOUBLE] = rif_double_equals_callback,
    [RIF_STRING] = rif_string_equals_callback,
    //[RIF_PTR]      = rif_ptr_equals_callback,
    //[RIF_LIST]     = rif_list_equals_callback,
    [RIF_PAIR]   = rif_pair_equals_callback,
    //[RIF_BUFFER]   = rif_buffer_equals_callback
};

static const rif_val_tostring_callback_t _rif_val_tostring_callbacks[RIF_VAL_TYPE_COUNT] = {
    [RIF_UNDEF]  = _rif_val_tostring_noop,
    [RIF_NULL]   = rif_null_tostring_callback,
    [RIF_BOOL]   = rif_bool_tostring_callback,
    [RIF_INT]    = rif_int_tostring_callback,
    [RIF_DOUBLE] = rif_double_tostring_callback,
    [RIF_STRING] = rif_string_tostring_callback,
    //[RIF_PTR]      = rif_ptr_tostring_callback,
    //[RIF_LIST]     = rif_list_tostring_callback,
    [RIF_PAIR]   = rif_pair_tostring_callback,
    //[RIF_BUFFER]   = rif_buffer_tostring_callback
};

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

rif_val_t * rif_val_init(rif_val_t *val_ptr, rif_val_type_t type, bool free) {
  if (!val_ptr) {
    return val_ptr;
  }
  val_ptr->type = type;
  val_ptr->free = free;
  atomic_init(&val_ptr->reference_count, 1);
  return val_ptr;
}

/******************************************************************************
 * GENERAL CALLBACKS FUNCTIONS
 */

/**
 * The basic retain callback.
 */
static
rif_val_t * _rif_val_retain_count(rif_val_t *val_ptr) {
  atomic_fetch_add(&val_ptr->reference_count, 1);
  return val_ptr;
}

/**
 * A retain callback that does nothing.
 */
static
rif_val_t * _rif_val_retain_noop(rif_val_t *val_ptr) {
  return val_ptr;
}

/**
 * The basic release callback.
 */
static
rif_val_t * _rif_val_release_count(rif_val_t *val_ptr) {
  if (atomic_load(&val_ptr->reference_count) == 0) {
    return val_ptr;
  }
  if (atomic_fetch_sub(&val_ptr->reference_count, 1) == 1) {
    _rif_val_destroy_callbacks[rif_val_type(val_ptr)](val_ptr);
    if (val_ptr->free) {
      rif_free(val_ptr);
    }
    val_ptr = NULL;
  }
  return val_ptr;
}

/**
 * A release callback that does nothing.
 */
static
rif_val_t * _rif_val_release_noop(rif_val_t *val_ptr) {
  return val_ptr;
}

/**
 * A hashcode callback that does nothing.
 */
static
uint32_t _rif_val_hashcode_noop(const rif_val_t *val_ptr) {
  return 0;
}

/**
 * An equals callback that compares value addresses.
 */
static
bool _rif_val_equals_address(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  return val_ptr == other_ptr;
}

/**
 * A tostring callback that does nothing.
 */
static
char * _rif_val_tostring_noop(const rif_val_t *val_ptr) {
  return NULL;
}

/******************************************************************************
 * HELPER FUNCTIONS
 */

rif_val_t * rif_val_retain_helper(rif_val_t *val_ptr) {
  if (!val_ptr) {
    return NULL;
  }
  return _rif_val_retain_callbacks[rif_val_type(val_ptr)](val_ptr);
}

rif_val_t * rif_val_release_helper(rif_val_t *val_ptr) {
  if (!val_ptr) {
    return NULL;
  }
  return _rif_val_release_callbacks[rif_val_type(val_ptr)](val_ptr);
}

uint32_t rif_val_hashcode_helper(const rif_val_t *val_ptr) {
  if (!val_ptr) {
    return 0;
  }
  return _rif_val_hashcode_callbacks[rif_val_type(val_ptr)](val_ptr);
}

bool rif_val_equals_helper(const rif_val_t *val_ptr, const rif_val_t *other_ptr) {
  if (!val_ptr || !other_ptr) {
    return val_ptr == other_ptr;
  }
  if (val_ptr == other_ptr) {
    return true;
  }
  if (val_ptr->type != other_ptr->type) {
    return false;
  }
  return _rif_val_equals_callbacks[rif_val_type(val_ptr)](val_ptr, other_ptr);
}

char * rif_val_tostring_helper(const rif_val_t *val_ptr) {
  if (!val_ptr) {
    return NULL;
  }
  return _rif_val_tostring_callbacks[rif_val_type(val_ptr)](val_ptr);
}