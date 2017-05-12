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
 * @brief Rif hook helpers.
 */

#pragma once

/******************************************************************************
 * MACROS
 */

/**
 * Call a hook on an object. If hook not found, then return the default value.
 *
 * @param __structure The name of the hook structure.
 * @param __hook      The name of the hook to call.
 * @param __default   The default value in case `hook` is not defined for `object`.
 * @param __object    The object to call `hook` for.
 * @param __args      The additional arguments to pass to the hook.
 * @return            The return value of calling `hook` if it is defined, `default` otherwise.
 */
#define rif_hook_named(__structure, __hook, __default, __object, ...) \
    ((__object) && (__object)->__structure && (__object)->__structure->__hook ? \
        (__object)->__structure->__hook((__object), ## __VA_ARGS__) : \
        (__default))

/**
 * Call a hook on an object that does not return.
 *
 * @param __structure The name of the hook structure.
 * @param __hook      The name of the hook to call.
 * @param __object    The object to call `hook` for.
 * @param __args      The additional arguments to pass to the hook.
 * @return            The return value of calling `hook` if it is defined, `default` otherwise.
 */
#define rif_hook_named_noreturn(__structure, __hook, __object, ...) \
    do { \
        if ((__object) && (__object)->__structure && (__object)->__structure->__hook) { \
            (__object)->__structure->__hook((__object), ## __VA_ARGS__); \
        } \
    } while (0)

/**
 * Call a hook on an object, inside a structure named `hook`. If hook not found, then return the default value.
 *
 * @param __hook    The name of the hook to call.
 * @param __default The default value in case `hook` is not defined for `object`.
 * @param __object  The object to call `hook` for.
 * @param __args    The additional arguments to pass to the hook.
 * @return          The return value of calling `hook` if it is defined, `default` otherwise.
 */
#define rif_hook(__hook, __default, __object, ...) rif_hook_named(hooks, __hook, __default, __object, ## __VA_ARGS__)

/**
 * Call a hook on an object that does not return, inside a structure named `hook`.
 *
 * @param __hook    The name of the hook to call.
 * @param __object  The object to call `hook` for.
 * @param __args    The additional arguments to pass to the hook.
 * @return          The return value of calling `hook` if it is defined, `default` otherwise.
 */
#define rif_hook_noreturn(__hook, __object, ...) \
    rif_hook_named_noreturn(hooks, __hook, __object, ## __VA_ARGS__)