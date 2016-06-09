////////////////////未初始化的空间填充，复制
//filln
//copyn

#pragma once 
#include "IteratorTraitsBase.h"
#include "Construct.h"

__STLBEGIN
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter
__UninitializedFill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, __true_type)
{
  return fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter
__UninitializedFill_n_aux(_ForwardIter __first, _Size __n,
                           const _Tp& __x, __false_type)
{
  _ForwardIter __cur = __first;
  __STL_TRY {
    for ( ; __n > 0; --__n, ++__cur)
      Construct(&*__cur, __x);
    return __cur;
  }
  __STL_UNWIND(Destroy(__first, __cur));
}

template <class _ForwardIter, class _Size, class _Tp, class _Tp1>
inline _ForwardIter 
__UninitializedFillN(_ForwardIter __first, _Size __n, const _Tp& __x, _Tp1*)
{
  typedef typename __TypeTraits<_Tp1>::is_POD _Is_POD;
  return __UninitializedFill_n_aux(__first, __n, __x, _Is_POD());
}


template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter 
UninitializedFillN(_ForwardIter __first, _Size __n, const _Tp& __x)
{
  return __UninitializedFillN(__first, __n, __x, ValueType(__first));
}
////////////////////////////


template <class _InputIter, class _ForwardIter>
_ForwardIter 
__UninitializedCopy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result,
                         __false_type)
{
  _ForwardIter __cur = __result;
  __STL_TRY {
    for ( ; __first != __last; ++__first, ++__cur)
      Construct(&*__cur, *__first);
    return __cur;
  }
  __STL_UNWIND(Destroy(__result, __cur));
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter 
__UninitializedCopy_aux(_InputIter __first, _InputIter __last,
                         _ForwardIter __result,
                         __true_type)
{
  return copy(__first, __last, __result);
}



template <class _InputIter, class _ForwardIter, class _Tp>
inline _ForwardIter
__UninitializedCopy(_InputIter __first, _InputIter __last,
                     _ForwardIter __result, _Tp*)
{
  typedef typename __TypeTraits<_Tp>::is_POD _Is_POD;
  return __UninitializedCopy_aux(__first, __last, __result, _Is_POD());
}


template <class _InputIter, class _ForwardIter>
inline _ForwardIter
UninitializedCopy(_InputIter __first, _InputIter __last,
                     _ForwardIter __result)
{
  return __UninitializedCopy(__first, __last, __result,ValueType(__result));
}
__STLEND


void testUninitFillN()
{

}