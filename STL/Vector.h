#pragma once 

#include <iostream>
using namespace std;

#include "Config.h"
#include "Trace.h"
#include "Construct.h"
#include "TypeTraits.h"
#include "Uninitialized.h"
#include "IteratorTraitsBase.h"
#include "Alloc.h"
__STLBEGIN

template <class Tp,class _Alloc = Alloc>
class Vector
{
public:
  typedef Tp ValueType;
  typedef Tp* Pointer;
  typedef Tp& Reference;
  typedef RandomIteratorTag IteratorCategory;
  typedef size_t DifferenceType;
  typedef size_t SizeType;
  typedef Tp* Iterator;
  // typedef stl::ReverseIterator<Iterator> ReverseIterator;

  typedef const Tp& ConstReference;
public:
  Vector()
    :_start(NULL)
    ,_finish(NULL)
    ,_endofStorage(NULL){}
  Vector(size_t size,const Tp& value)
  {
    __TRACE("size = %u\n",size);
    _finish= UninitializedFillN(_start, size, value); 
  }
  Vector(size_t size)
  {
    __TRACE("size = %d\n",size);
    _finish= UninitializedFillN(_start, size,Tp()); 
  }

  Vector(const Vector<Tp,_Alloc>& __x)
  {
    _finish = UninitializedCopy(__x.Begin(), __x.End(), _start); 
  }





void InsertAux(Iterator position, const ValueType& value)
{
  __TRACE("cursize = %d\n",Size());
  if (_finish != _endofStorage) {
    Construct(_finish, *(_finish - 1));

    ++_finish;
    ValueType __x_copy = value;
    copy_backward(position, _finish - 2, _finish - 1);
    *position = __x_copy;
  }

  else {
    const SizeType oldSize = Size();
    const SizeType len = oldSize != 0 ? 2 * oldSize : 2;

    Iterator newStart = (Pointer)_Alloc::Allocate(len*sizeof(Vector<Tp>));
    Iterator newFinish = newStart;
    __STL_TRY 
    {
      newFinish = UninitializedCopy(_start, position, newStart);
      Construct(newFinish, value);
      ++newFinish;
      newFinish = UninitializedCopy(position, _finish, newFinish);
    }
    __STL_UNWIND((Destroy(newStart,newFinish),_Alloc::Deallocate(newStart,len)));

    Destroy(Begin(), End());
    _Alloc::Deallocate(_start, _endofStorage - _start);
    _start = newStart;
    _finish = newFinish;
    _endofStorage = newStart + len;
  }
}



  Iterator Begin()
  {
    return _start;
  }
  Iterator End()
  {
    return _finish;
  }
  size_t Size()
  {
    return _finish - _start;
  }
  size_t MaxSize()
  {
    return size_t(-1)/sizeof(Tp);
  }
  size_t Capacity()
  {
    return _endofStorage-_start;
  }
  bool Empty()
  {
    return _finish==_start;
  } 
  bool Full()
  {
    return _endofStorage==_finish;
  }
  Reference Front()
  {
    return *Begin();
  }

  ConstReference Front() const
  {
    return *Begin();
  }
  Reference Back()
  {
    return *(End()-1);
  }


public:
  void PushBack(const ValueType& value)
  {
    // __TRACE("cursize = %u\n",Size());
    if(_finish!=_endofStorage)
    {
      Construct(_finish,value);
      ++_finish;
    }
    else
    {
      InsertAux(End(),value);
    }
    __TRACE("%d\n",value);
  }
  void PopBack()
  {
    if(_finish!=_start)
      --_finish;
    Destroy(_finish);
  }

  Iterator Erase(Iterator position)
  {
    if(position+1!=End())
        copy(position + 1, _finish, position);
    --_finish;
    Destroy(_finish);
    return position;
  }
  Iterator Erase(Iterator first, Iterator last)
  {
    Iterator er = copy(last,_finish,first);
    Destroy(er,_finish);
    _finish-= last - first;
    return first;
  }

  size_t Resize(size_t newSize,const ValueType& value)
  {
    if(newSize<=Size())
    {
      Erase(Begin()+newSize,End());
    }
    else
    {
      Insert(End(),newSize-Size(),value);
    }

  }
  size_t Resize(size_t newSize)
  {
    Resize(newSize,ValueType());
  }

  void clear()
  {
    Destroy(Begin(),End());
    _Alloc::Deallocate(_start);
  }
  // template <class _InputIterator>
  // vector(_InputIterator first, _InputIterator last)
  //  {
  //   typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
  //   _M_initialize_aux(__first, __last, _Integral());
  // }

protected:
  Tp * _start;
  Tp * _finish;
  Tp * _endofStorage;
};
__STLEND

void testVector()
{
  stl::Vector<int> ve;
  ve.PushBack(1);
  ve.PushBack(2);
  ve.PushBack(3);
  ve.PushBack(4);
  ve.PushBack(5);

  stl::Vector<int>::Iterator it = ve.Begin();
  while(it != ve.End())
  {
    cout << *it << " ";
    ++it;
  }
  cout << endl;
}