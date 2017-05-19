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

#include "rif/rif_internal.h"

#include "rif/collection/rif_hashmap.h"
#include "rif/util/rif_hash.h"
#include "rif/util/rif_math.h"

/******************************************************************************
 * HELPERS
 */

#define MIN_CAPACITY 8

#define slot_distance(__capacity, __hash, __index) \
    (rif_mod_pow2((__index + __capacity - rif_mod_pow2(__hash, __capacity)), __capacity))

#define is_deleted(__hash) ((__hash >> 31) != 0)

#define swap(__x, __y) do { typeof(__x) ___TMPSWAP = __x; __x = __y; __y = ___TMPSWAP; } while (0)

static inline
uint32_t _rif_hashmap_hash(const rif_val_t *val_ptr, uint64_t salt) {

  // Hash the value
  uint32_t hash = rif_val_hashcode(val_ptr);

  // Mix with hashmap element address for salt
  hash = rif_hash_mix_32(hash, rif_hash_64((uint64_t) salt));

  // Clear most significant bit (used to mark deleted elements)
  hash &= 0x7fffffff;

  // Ensure we are not returning 0 (used to mark free slots)
  hash |= hash == 0;

  return hash;
}

/******************************************************************************
 * LIFECYCLE FUNCTIONS
 */

static
rif_hashmap_t * _rif_hashmap_build(rif_hashmap_t *hm_ptr, bool free, uint32_t capacity, bool fixed) {
  if (!hm_ptr) {
    return hm_ptr;
  }
  rif_map_init((rif_map_t *) hm_ptr, &rif_hashmap_hooks, free);
  hm_ptr->capacity = 0;
  hm_ptr->elements = NULL;
  hm_ptr->free_elements = true;
  hm_ptr->size = 0;
  hm_ptr->fixed = fixed;

  // Allocate element array if needed.
  if (capacity && RIF_OK != rif_hashmap_ensure_capacity(hm_ptr, capacity)) {
    return NULL;
  }

  return hm_ptr;
}

rif_hashmap_t * rif_hashmap_init(rif_hashmap_t *hm_ptr, uint32_t capacity, bool fixed) {
  return _rif_hashmap_build(hm_ptr, false, capacity, fixed);
}

void rif_hashmap_destroy_callback(rif_hashmap_t *hm_ptr) {
  uint32_t pos = 0;
  for (; pos < hm_ptr->capacity; ++pos) {
    rif_hashmap_element_t *cur = hm_ptr->elements + pos;
    if (cur->hash && !is_deleted(cur->hash)) {
      rif_val_release(cur->key_ptr);
      rif_val_release(cur->val_ptr);
    }
  }
  if (hm_ptr->free_elements) {
    rif_free(hm_ptr->elements);
  }
}

/******************************************************************************
 * SIZING FUNCTIONS
 */

// Forward declare the put helper we need
static inline
uint32_t _rif_hashmap_put_helper(
    rif_hashmap_element_t *elements, uint32_t capacity, rif_val_t *key_ptr, rif_val_t *val_ptr);

static inline
void _rif_hashmap_remap(rif_hashmap_t *hm_ptr, rif_hashmap_element_t *to, uint32_t to_capacity) {
  uint32_t pos = 0;
  for (; pos < hm_ptr->capacity; ++pos) {
    rif_hashmap_element_t *cur = hm_ptr->elements + pos;
    if (cur->hash && !is_deleted(cur->hash)) {
      _rif_hashmap_put_helper(to, to_capacity, cur->key_ptr, cur->val_ptr);
    }
  }
}

rif_status_t rif_hashmap_ensure_capacity(rif_hashmap_t *hm_ptr, uint32_t capacity) {

  // Calculate the capacity we need to allocate.
  capacity = rif_max(MIN_CAPACITY, capacity);
  uint32_t needed_capacity = rif_next_pow2(capacity + capacity / 10);

  // Maybe we don't need to do anything.
  if (needed_capacity <= hm_ptr->capacity) {
    return RIF_OK;
  }

  // Maybe the map has a fixed size. If so,
  if (hm_ptr->fixed && hm_ptr->elements) {
    return RIF_ERR_CAPACITY;
  }

  // Allocate memory.
  rif_hashmap_element_t *new_elements =
      rif_calloc(needed_capacity, sizeof(rif_hashmap_element_t), "HASHMAP_CAPACITY_ALLOC");
  if (new_elements == NULL) {
    return RIF_ERR_MEMORY;
  }

  // Remap values
  if (hm_ptr->size) {
    _rif_hashmap_remap(hm_ptr, new_elements, needed_capacity);
  }

  // Free existing array
  rif_free(hm_ptr->elements);
  hm_ptr->elements = new_elements;

  // Done.
  hm_ptr->capacity = needed_capacity;
  return RIF_OK;
}

/******************************************************************************
 * ELEMENT READ FUNCTIONS
 */

static
rif_hashmap_element_t * _rif_hashmap_locate(const rif_hashmap_t *hm_ptr, const rif_val_t *key_ptr) {

  // Hash the key
  uint32_t hash = _rif_hashmap_hash(key_ptr, (uint64_t) hm_ptr->elements);

  // If the map is not yet allocated, return
  if (0 == hm_ptr->capacity) {
    return NULL;
  }

  // Setup counters
  uint32_t pos = hash % hm_ptr->capacity;
  uint32_t dist = 0;

  // Lookup element
  while (true) {
    rif_hashmap_element_t *cur = hm_ptr->elements + pos;
    if (hash == cur->hash && rif_val_equals(cur->key_ptr, key_ptr)) {
      return cur;
    } else if (0 == cur->hash) {
      return NULL;
    } else if (dist > slot_distance(hm_ptr->capacity, cur->hash, pos)) {
      return NULL;
    }
    pos = rif_mod_pow2(pos + 1, hm_ptr->capacity);
    ++dist;
  }

}

bool rif_hashmap_exists(const rif_hashmap_t *hm_ptr, const rif_val_t *key_ptr) {
  return NULL != _rif_hashmap_locate(hm_ptr, key_ptr);
}

rif_val_t * rif_hashmap_get(const rif_hashmap_t *hm_ptr, const rif_val_t *key_ptr) {
  rif_hashmap_element_t *elem_ptr = _rif_hashmap_locate(hm_ptr, key_ptr);
  return NULL == elem_ptr ? NULL : elem_ptr->val_ptr;
}

rif_hashmap_element_t * rif_hashmap_atindex(const rif_hashmap_t *hm_ptr, uint32_t index) {
  assert(index < rif_hashmap_capacity(hm_ptr));
  rif_hashmap_element_t *atindex = hm_ptr->elements + index;
  if (!atindex->hash || is_deleted(atindex->hash)) {
    return NULL;
  }
  return atindex;
}

/******************************************************************************
 * ELEMENT WRITE FUNCTIONS
 */

static inline
uint32_t _rif_hashmap_put_helper(
    rif_hashmap_element_t *elements, uint32_t capacity, rif_val_t *key_ptr, rif_val_t *val_ptr) {

  // Compute hash
  uint32_t hash = _rif_hashmap_hash(key_ptr, (uint64_t) elements);

  // Setup counters
  uint32_t pos = hash % capacity;
  uint32_t dist = 0;

  // Position element
  while (true) {

    rif_hashmap_element_t *cur = elements + pos;
    if (hash == cur->hash && rif_val_equals(cur->key_ptr, key_ptr)) {
      rif_val_release(cur->key_ptr);
      rif_val_release(cur->val_ptr);
      cur->key_ptr = key_ptr;
      cur->val_ptr = val_ptr;
      return 0;
    } else if (0 == cur->hash) {
      cur->hash = hash;
      cur->key_ptr = key_ptr;
      cur->val_ptr = val_ptr;
      return 1;
    }

    uint32_t cur_dist = slot_distance(capacity, cur->hash, pos);

    if (cur_dist < dist) {
      if (is_deleted(cur->hash)) {
        cur->hash = hash;
        cur->key_ptr = key_ptr;
        cur->val_ptr = val_ptr;
        return 1;
      }
      swap(cur->hash, hash);
      swap(cur->key_ptr, key_ptr);
      swap(cur->val_ptr, val_ptr);
      dist = cur_dist;
    }

    pos = rif_mod_pow2(pos + 1, capacity);
    ++dist;
  }

}

rif_status_t rif_hashmap_put(rif_hashmap_t *hm_ptr, rif_val_t *key_ptr, rif_val_t *val_ptr) {

  // Ensure we got sufficient capacity
  rif_status_t ensure_capacity_status = rif_hashmap_ensure_capacity(hm_ptr, hm_ptr->size + 1);
  if (RIF_OK != ensure_capacity_status) {
    return ensure_capacity_status;
  }

  // Insert the element
  rif_val_retain(key_ptr);
  rif_val_retain(val_ptr);
  hm_ptr->size += _rif_hashmap_put_helper(hm_ptr->elements, hm_ptr->capacity, key_ptr, val_ptr);

  // Done
  return RIF_OK;
}

/******************************************************************************
 * ELEMENT DELETE FUNCTIONS
 */

rif_status_t rif_hashmap_remove(rif_hashmap_t *hm_ptr, rif_val_t *key_ptr) {

  // Locate the element
  rif_hashmap_element_t *elem_ptr = _rif_hashmap_locate(hm_ptr, key_ptr);

  // If no element found, we're done
  if (NULL == elem_ptr) {
    return RIF_OK;
  }

  // Mark the element as deleted
  elem_ptr->hash |= 0x80000000;
  rif_val_release(elem_ptr->key_ptr);
  rif_val_release(elem_ptr->val_ptr);
  elem_ptr->val_ptr = NULL;

  // Do the bookkeeping
  --hm_ptr->size;

  return RIF_OK;
}