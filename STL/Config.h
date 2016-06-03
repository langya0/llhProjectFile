#pragma once

namespace __STLNAMESPACE {}

#define __STLBEGIN namespace __STLNAMESPACE {

#define __STLEND }

namespace stl = __STLNAMESPACE;

//for  malloc
// #define __USE_MALLOC

///for  trace
#define __DEBUG
#ifdef __DEBUG
#include <stdio.h>
#define fout stdout
#endif