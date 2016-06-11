#pragma once 

#if defined(__STL_PTHREADS)
#include <pthread.h>
#endif

#include "Config.h"

__STLBEGIN

struct _STL_mutex_lock
{

#if defined(__STL_PTHREADS)
  pthread_mutex_t _M_lock;
  void _M_initialize()   { pthread_mutex_init(&_M_lock, NULL); }
  void _M_acquire_lock() { pthread_mutex_lock(&_M_lock); }
  void _M_release_lock() { pthread_mutex_unlock(&_M_lock); }
#else /* No threads */
  void _M_initialize()   {}
  void _M_acquire_lock() {}
  void _M_release_lock() {}
#endif
};

__STLEND