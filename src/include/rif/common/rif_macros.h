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

#pragma once

/******************************************************************************
 * API ACCESS
 */


#ifndef RIF_API
  #ifdef _WIN32
     #if defined(RIF_BUILD_SHARED) /* build dll */
         //#define RIF_API __declspec(dllexport)
     #elif !defined(RIF_BUILD_STATIC) /* use dll */
         //#define RIF_API __declspec(dllimport)
     #else /* static library */
         //#define RIF_API
     #endif
  #else
     #if __GNUC__ >= 4
         //#define RIF_API __attribute__((visibility("default"), unused))
     #else
         //#define RIF_API
     #endif
  #endif
#endif
#define RIF_API

#define RIF_INLINE static inline

/******************************************************************************
 * STATIC ASSERT
 */

#ifdef __GNUC__
  #define RIF_STATIC_ASSERT_HELPER(__expr, __msg) \
      (!!sizeof(struct { unsigned int RIF_STATIC_ASSERTION__##__msg: (__expr) ? 1 : -1; }))
  #define RIF_STATIC_ASSERT_INTERNAL(__expr, __msg) \
      extern int (*assert_function__(void)) [RIF_STATIC_ASSERT_HELPER(__expr, __msg)]
#else
  #define RIF_STATIC_ASSERT_INTERNAL(__expr, __msg) \
      extern char RIF_STATIC_ASSERTION__##__msg[1]; \
      extern char RIF_STATIC_ASSERTION__##__msg[(__expr)?1:2]

#endif /* #ifdef __GNUC__ */

/**
 * @private
 *
 * Asserts that a condition is true at compile-time. If it isn't it throws a compilation error with the given message.
 *
 * @param __expr static condition to check for
 * @param __msg  the identifying message (must follow variable-naming requirements)
 */
#define RIF_STATIC_ASSERT(__expr, __msg) RIF_STATIC_ASSERT_INTERNAL(__expr, __msg)
