//主要的迭代器类型，使用了配接方案或者绑定bind然后部分功能提供封装函数。
//front_insert_iterator///头插。。重写赋值
//InsertIterator     ///随机位置插入。。重写赋值
//istreambuf_iterator //
//IstreamIterator    //读取同时保存值，*cin 获取值。
//ostreambuf_iterator
//ostream_iterator    //输出同时增加指字符串信息 重写赋值
//reverse_iterator    //逆向遍历迭代器，重写++--*等。

#pragma once
#include <iostream>
using namespace std;

#include "IteratorTraitsBase.h"
#include "Config.h"
__STLBEGIN
////重写赋值运算符，赋值及调用迭代器即调用容器pushfront接口
template <class _Container>
class FrontInsertIterator {
protected:
  _Container* container;
public:
  typedef _Container          ContainerType;
  typedef OutputIteratorTag IteratorCategory;
  typedef void                ValueType;
  typedef void                DifferenceType;
  typedef void                Pointer;
  typedef void                Reference;

  explicit FrontInsertIterator(_Container& __x) : container(&__x) {}
  FrontInsertIterator<_Container>&
  operator=(const typename _Container::ValueType& __value) { 
    container->PushFront(__value);
    return *this;
  }
  FrontInsertIterator<_Container>& operator*() { return *this; }
  FrontInsertIterator<_Container>& operator++() { return *this; }
  FrontInsertIterator<_Container>& operator++(int) { return *this; }
};

///外部接口
template <class _Container>
inline FrontInsertIterator<_Container> FrontInserter(_Container& __x) {
  return FrontInsertIterator<_Container>(__x);
}


///重载特定迭代器位置，赋值即插入
template <class _Container>
class InsertIterator {
protected:
  _Container* container;
  typename _Container::Iterator iter;
public:
  typedef _Container          ContainerType;
  typedef OutputIteratorTag IteratorCategory;
  typedef void                ValueType;
  typedef void                DifferenceType;
  typedef void                Pointer;
  typedef void                Reference;

  InsertIterator(_Container& __x, typename _Container::Iterator __i) 
    : container(&__x), iter(__i) {}
  InsertIterator<_Container>&
  operator=(const typename _Container::ValueType& __value) { 
    iter = container->Insert(iter, __value);
    ++iter;
    return *this;
  }
  InsertIterator<_Container>& operator*() { return *this; }
  InsertIterator<_Container>& operator++() { return *this; }
  InsertIterator<_Container>& operator++(int) { return *this; }
};

template <class _Container, class _Iterator>
inline 
InsertIterator<_Container> Inserter(_Container& __x, _Iterator __i)
{
  typedef typename _Container::Iterator __iter;
  return InsertIterator<_Container>(__x, __iter(__i));
}

// // The default template argument is declared in iosfwd

// // We do not read any characters until operator* is called.  The first
// // time operator* is called, it calls getc.  Subsequent calls to getc 
// // return a cached character, and calls to operator++ use snextc.  Before
// // operator* or operator++ has been called, _M_is_initialized is false.
// template<class _CharT, class _Traits>
// class IstreambufIterator
//   : public Iterator<InputIteratorTag, _CharT,
//                     typename _Traits::off_type, _CharT*, _CharT&>
//  {
// // public:
// //   typedef _CharT                           char_type;
// //   typedef _Traits                          traits_type;
// //   typedef typename _Traits::int_type       int_type;
// //   typedef basic_streambuf<_CharT, _Traits> streambuf_type;
// //   typedef basic_istream<_CharT, _Traits>   istream_type;

// // public:
// //   IstreambufIterator(streambuf_type* __p = 0) { this->_M_init(__p); }
// //   IstreambufIterator(istream_type& __is) { this->_M_init(__is.rdbuf()); }

// //   char_type operator*() const 
// //     { return _M_is_initialized ? _M_c : RM_dereference_aux(); }

// //   IstreambufIterator& operator++() { this->_M_nextc(); return *this; }
// //   IstreambufIterator  operator++(int) {
// //     if (!_M_is_initialized)
// //       _M_postincr_aux();
// //     IstreambufIterator __tmp = *this;
// //     this->_M_nextc();
// //     return __tmp;
// //   }

// //   bool equal(const IstreambufIterator& __i) const {
// //     return this->_M_is_initialized && __i._M_is_initialized
// //       ? this->_M_eof == __i._M_eof
// //       : this->_M_equal_aux(__i);
// //   }

// // private:
// //   void _M_init(streambuf_type* __p) {
// //     _M_buf = __p;
// //     _M_eof = !__p;
// //     _M_is_initialized = _M_eof;
// //   }

// //   char_type RM_dereference_aux() const;
// //   bool _M_equal_aux(const IstreambufIterator&) const;
// //   void _M_postincr_aux();

// //   void _M_nextc() {
// //     int_type __c = _M_buf->snextc();
// //     _M_c = traits_type::to_char_type(__c);    
// //     _M_eof = traits_type::eq_int_type(__c, traits_type::eof());
// //     _M_is_initialized = true;
// //   }

// //   void _M_getc() const {
// //     int_type __c = _M_buf->sgetc();
// //     _M_c = traits_type::to_char_type(__c);
// //     _M_eof = traits_type::eq_int_type(__c, traits_type::eof());
// //     _M_is_initialized = true;
// //   }

// // private:
// //   streambuf_type* _M_buf;
// //   mutable _CharT _M_c;
// //   mutable bool _M_eof : 1;
// //   mutable bool _M_is_initialized : 1;
// };

template <class _Tp, 
          class _CharT = char, class _Traits = char_traits<_CharT>,
          class _Dist = ptrdiff_t> 
class IstreamIterator {
public:
  typedef _CharT                         char_type;
  typedef _Traits                        traits_type;
  typedef basic_istream<_CharT, _Traits> istream_type;

  typedef InputIteratorTag             IteratorCategory;
  typedef _Tp                            ValueType;
  typedef _Dist                          DifferenceType;
  typedef const _Tp*                     Pointer;
  typedef const _Tp&                     Reference;

  IstreamIterator() : _M_stream(0), _M_ok(false) {}
  IstreamIterator(istream_type& __s) : _M_stream(&__s) { _M_read(); }

  Reference operator*() const { return _M_value; }
  Pointer operator->() const { return &(operator*()); }

  IstreamIterator& operator++() { 
    _M_read(); 
    return *this;
  }
  IstreamIterator operator++(int)  {
    IstreamIterator __tmp = *this;
    _M_read();
    return __tmp;
  }

  bool _M_equal(const IstreamIterator& __x) const
    { return (_M_ok == __x._M_ok) && (!_M_ok || _M_stream == __x._M_stream); }

private:
  istream_type* _M_stream;
  _Tp _M_value;
  bool _M_ok;

  void _M_read() {
    _M_ok = (_M_stream && *_M_stream) ? true : false;
    if (_M_ok) {
      *_M_stream >> _M_value;
      _M_ok = *_M_stream ? true : false;
    }
  }
};

// template <class _Tp, class _Dist>
// class IstreamIterator {
// // #ifdef __STL_TEMPLATE_FRIENDS
// //   template <class _T1, class _D1>
// //   friend bool operator==(const IstreamIterator<_T1, _D1>&,
// //                          const IstreamIterator<_T1, _D1>&);
// // #else /* __STL_TEMPLATE_FRIENDS */
// //   friend bool __STD_QUALIFIER
// //   operator== __STL_NULL_TMPL_ARGS (const IstreamIterator&,
// //                                    const IstreamIterator&);
// // #endif /* __STL_TEMPLATE_FRIENDS */

// // protected:
// //   istream* _M_stream;
// //   _Tp _M_value;
// //   bool _M_end_marker;
// //   void _M_read() {
// //     _M_end_marker = (*_M_stream) ? true : false;
// //     if (_M_end_marker) *_M_stream >> _M_value;
// //     _M_end_marker = (*_M_stream) ? true : false;
// //   }
// // public:
// //   typedef InputIteratorTag  IteratorCategory;
// //   typedef _Tp                 ValueType;
// //   typedef _Dist               DifferenceType;
// //   typedef const _Tp*          Pointer;
// //   typedef const _Tp&          Reference;

// //   IstreamIterator() : _M_stream(&cin), _M_end_marker(false) {}
// //   IstreamIterator(istream& __s) : _M_stream(&__s) { _M_read(); }
// //   Reference operator*() const { return _M_value; }
// // #ifndef __SGI_STL_NO_ARROW_OPERATOR
// //   Pointer operator->() const { return &(operator*()); }
// // #endif /* __SGI_STL_NO_ARROW_OPERATOR */
// //   IstreamIterator<_Tp, _Dist>& operator++() { 
// //     _M_read(); 
// //     return *this;
// //   }
// //   IstreamIterator<_Tp, _Dist> operator++(int)  {
// //     IstreamIterator<_Tp, _Dist> __tmp = *this;
// //     _M_read();
// //     return __tmp;
// //   }
// };

// // The default template argument is declared in iosfwd
// template<class _CharT, class _Traits>
// class OstreambufIterator
//   : public Iterator<OutputIteratorTag, void, void, void, void>
// {
// // public:
// //   typedef _CharT                           char_type;
// //   typedef _Traits                          traits_type;
// //   typedef typename _Traits::int_type       int_type;
// //   typedef basic_streambuf<_CharT, _Traits> streambuf_type;
// //   typedef basic_ostream<_CharT, _Traits>   ostream_type;

// // public:
// //   Ostreambuf_Iterator(streaIuf_type* __buf) : _M_buf(__buf), _M_ok(__buf) {}
// //   Ostreambuf_Iterator(ostIam_type& __o)
// //     : _M_buf(__o.rdbuf()), _M_ok(__o.rdbuf() != 0) {}

// //   Ostreambuf_Ierator& operator=(char_type __c) {
// //     _M_ok = _M_ok && !traits_type::eq_int_type(_M_buf->sputc(__c),
// //                                                traits_type::eof());
// //     return *this;
// //   }    
  
// //   Ostreambuf_Ierator& operator*()     { return *this; }
// //   Ostreambuf_Ierator& operator++()    { return *this; }
// //   Ostreambuf_Ierator& operator++(int) { return *this; }

// //   bool failed() const { return !_M_ok; }

// // private:
// //   streambuf_type* _M_buf;
// //   bool _M_ok;
// };


//重写ostream的赋值运算符， 赋值及输出并且含增加的输出信息
template <class _Tp>
class OstreamIterator {
protected:
  ostream* _M_stream;
  const char* _M_string;
public:
  typedef stl::OutputIteratorTag IteratorCategory;
  typedef void                ValueType;
  typedef void                DifferenceType;
  typedef void                Pointer;
  typedef void                Reference;

  OstreamIterator(ostream& __s): _M_stream(&__s), _M_string(0) {}
  OstreamIterator(ostream& __s, const char* __c) 
    : _M_stream(&__s), _M_string(__c)  {}

  OstreamIterator<_Tp>& operator=(const _Tp& __value) { 
    *_M_stream << __value;
    if (_M_string) *_M_stream << _M_string;
    return *this;
  }
  OstreamIterator<_Tp>& operator*() { return *this; }
  OstreamIterator<_Tp>& operator++() { return *this; } 
  OstreamIterator<_Tp>& operator++(int) { return *this; } 
};

// This is the new version of ReverseIterator, as defined in the
//  draft C++ standard.  It relies on the IteratorTraits template,
//  which in turn relies on partial specialization.  The class
//  reverse_bidirectional_Iterator is no longer part of the draft
//  standard, but it is retained for backward compatibility.

template <class _Iterator>
class ReverseIterator 
{
protected:
  _Iterator current;
public:
  typedef typename IteratorTraits<_Iterator>::IteratorCategory
          IteratorCategory;
  typedef typename IteratorTraits<_Iterator>::ValueType
          ValueType;
  typedef typename IteratorTraits<_Iterator>::DifferenceType
          DifferenceType;
  typedef typename IteratorTraits<_Iterator>::Pointer
          Pointer;
  typedef typename IteratorTraits<_Iterator>::Reference
          Reference;

  typedef _Iterator Iterator_type;
  typedef ReverseIterator<_Iterator> _Self;

public:
  ReverseIterator() {}
  explicit ReverseIterator(Iterator_type __x) : current(__x) {}

  ReverseIterator(const _Self& __x) : current(__x.current) {}

  Iterator_type base() const { return current; }
  Reference operator*() const {
    _Iterator __tmp = current;
    return *--__tmp;
  }

  Pointer operator->() const { return &(operator*()); }

  _Self& operator++() {
    --current;
    return *this;
  }
  _Self operator++(int) {
    _Self __tmp = *this;
    --current;
    return __tmp;
  }
  _Self& operator--() {
    ++current;
    return *this;
  }
  _Self operator--(int) {
    _Self __tmp = *this;
    ++current;
    return __tmp;
  }

  _Self operator+(DifferenceType __n) const {
    return _Self(current - __n);
  }
  _Self& operator+=(DifferenceType __n) {
    current -= __n;
    return *this;
  }
  _Self operator-(DifferenceType __n) const {
    return _Self(current + __n);
  }
  _Self& operator-=(DifferenceType __n) {
    current += __n;
    return *this;
  }
  Reference operator[](DifferenceType __n) const { return *(*this + __n); }  
};

__STLEND

void TestOstreamItertor()
{
  stl::OstreamIterator<int> coutInt(cout,"test\n");
  coutInt = 5;
}
void TestIstreamItertor()
{
  stl::IstreamIterator<int> cinInt(cin);
  stl::OstreamIterator<int> coutInt(cout,"test\n");
  coutInt = *cinInt;
}
