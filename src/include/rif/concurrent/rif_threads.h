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
 * @brief Rif threading.
 */

#pragma once

#include "../../../../lib/tinycthread/source/tinycthread.h"
#include "rif/rif_common.h"

#if __APPLE__
  #include <dispatch/dispatch.h>
#elif defined(_POSIX_VERSION)
  #include <semaphore.h>
#else
  #error "Unknown compiler"
#endif

/******************************************************************************
 * MAC SEMAPHORE
 */

#if __APPLE__

typedef dispatch_semaphore_t sem_t;

RIF_INLINE
int sem_init(sem_t *sem_handle, int pshared, unsigned int value) {
  dispatch_semaphore_t sem = dispatch_semaphore_create(value);
  if (!sem) {
    return -1;
  }
  *sem_handle = sem;
  return 0;
}

RIF_INLINE
int sem_wait(sem_t *sem_handle) {
  long ret = dispatch_semaphore_wait(*sem_handle, DISPATCH_TIME_FOREVER);
  return 0 == ret ? ret : -1;
}

RIF_INLINE
int sem_trywait(sem_t *sem_handle) {
  long ret = dispatch_semaphore_wait(*sem_handle, DISPATCH_TIME_NOW);
  if (0 != ret) {
    errno = EAGAIN;
    return -1;
  }
  return ret;
}

RIF_INLINE
int sem_timedwait(sem_t *sem_handle, const struct timespec *abs_timeout) {
  long ret = dispatch_semaphore_wait(*sem_handle, dispatch_walltime(abs_timeout, 0));
  if (0 != ret) {
    errno = ETIMEDOUT;
    return -1;
  }
  return ret;
}

RIF_INLINE
int sem_post(sem_t *sem_handle) {
  long ret = dispatch_semaphore_signal(*sem_handle);
  return 0 == ret ? ret : -1;
}

RIF_INLINE
int sem_destroy(sem_t *sem_handle) {
  dispatch_release(*sem_handle);
  return 0;
}

#endif