#pragma once
#include <string.h>
#include "TypeTraits.h"
#include "Alloc.h"

__STLBEGIN
// Internal names

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __value) {		//两个模板类型，可以调构造和拷贝构造。不用运算符重载也能完成身前拷贝
  new ((void*) __p) _T1(__value);
}

template <class _T1>
inline void _Construct(_T1* __p) {
  new ((void*) __p) _T1();
}

template <class _Tp>
inline void _Destroy(_Tp* __pointer) {
  __pointer->~_Tp();
}

///自定义类型，对迭代的每个对象进行显示戏够
template <class _ForwardIterator>
void __Destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type)
{
  for ( ; __first != __last; ++__first)
    _Destroy(&*__first);
}

template <class _ForwardIterator> 
inline void __Destroy_aux(_ForwardIterator, _ForwardIterator, __true_type) {}

/////类型萃取，便于重载策略
template <class _ForwardIterator, class _Tp>
inline void __Destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
  typedef typename __TypeTraits<_Tp>::has_trivial_destructor _Trivial_destructor;
  __Destroy_aux(__first, __last, _Trivial_destructor());    
}

/////////////////////valuetype函数获取类型----类型推演。进行萃取
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) {
  __Destroy(__first, __last, ValueTpye(__first));
}

inline void _Destroy(char*, char*) {}
inline void _Destroy(int*, int*) {}
inline void _Destroy(long*, long*) {}
inline void _Destroy(float*, float*) {}
inline void _Destroy(double*, double*) {}

////////////外部接口
template <class _T1, class _T2>
inline void Construct(_T1* __p, const _T2& __value) {
  __TRACE("p = %p\n",__p);
  _Construct(__p, __value);
}

template <class _T1>
inline void Construct(_T1* __p) {
  _Construct(__p);
}

template <class _Tp>
inline void Destroy(_Tp* __pointer) {
  __TRACE("p = %p\n",__pointer);
  _Destroy(__pointer);
}

template <class _ForwardIterator>
inline void Destroy(_ForwardIterator __first, _ForwardIterator __last) {
  _Destroy(__first, __last);
}

__STLEND

void testConstruct()
{
  int *ip = (int*)stl::Alloc::Allocate(sizeof(int));
  int ia = 4;
  stl::Construct(ip,ia);
  __TRACE("*ip = %d\n",*ip);

  char * cp = (char*)stl::Alloc::Allocate(strlen("hello world"));
  const char * p = "hello world";

  stl::Destroy(ip);
  stl::Destroy(cp);
}
