#pragma once

/////模拟std实现宏方式开始，结束命名空间
namespace stl{}
// #define  namespace std {
#define __STLBEGIN namespace stl{
#define __STLEND }

// namespace  = __STLNAMESPACE;

//for  malloc
// #define __USE_MALLOC

///for  trace
#define __DEBUG

#ifdef __DEBUG
#include <stdio.h>
#define fout stdout	///方便配置，修改Trace记录位置
#endif

#define __STL_TRY try
#define __STL_UNWIND(event) catch(...) {event; throw;}
