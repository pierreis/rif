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
 * @brief Rif mathematical helpers.
 */

#pragma once

/******************************************************************************
 * MACROS
 */

/**
 * Compute the minimum of two values.
 *
 * @param a The first value.
 * @param b The second value.
 * @return  `a` if `a < b`, `b` otherwise.
 */
#define rif_min(a, b) ((b) ^ (((a) ^ (b)) & -((a) < (b))))

/**
 * Compute the maximum of two values.
 *
 * @param a The first value.
 * @param b The second value.
 * @return  `a` if `a > b`, `b` otherwise.
 */
#define rif_max(a, b) ((a) ^ (((a) ^ (b)) & -((a) < (b))))

/**
 * Compute the absolute value.
 *
 * @param v The value.
 * @return  `v` if `v >= 0`, `v * -1` otherwise.
 */
#define rif_abs(v) \
  ({ \
    __typeof__((v)) mask = (v) >> (sizeof(__typeof__((v))) * CHAR_BIT - 1); \
    (((v) + mask) ^ mask); \
  })

/**
 * Checks whether a value is a power of two.
 *
 * @param v The value.
 * @return  `true` if `v` is a power of two.
 */
#define rif_is_pow2(v) ((v) && !((v) & ((v) - 1)))

/**
 * Round a value up to the next highest power of two.
 *
 * @param v The value.
 * @return  The next highest power of two. If `v` is already a power of two, the function will return `v`.
 */
#define rif_next_pow2(v) \
  ({ \
    __typeof__((v)) v_copy = v; \
    --v_copy; \
    __typeof__((v)) bit_limit = sizeof(__typeof__((v))) * CHAR_BIT; \
    __typeof__((v)) i = 1; \
    for (; i < bit_limit; i *= 2) { \
      v_copy |= v_copy >> i; \
    } \
    ++v_copy; \
  })

/**
 * Calculates the modulus of a value with a divisor which is a power of two.
 *
 * @param v   The value.
 * @param div The divisor. It MUST be a power of two.
 * @return    The result of `v % div`.
 */
#define rif_mod_pow2(v, div) ((v) & ((div) - 1))
