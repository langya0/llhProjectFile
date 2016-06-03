#pragma once
#include "Config.h"
__STLBEGIN

#include <typeinfo>
struct __true_type
{};
struct __false_type
{};

template<class T>
struct __TypeTraits
{
	typedef __true_type this_dummy_member_must_be_first;

	typedef __true_type has_traivial_default_constructor;
	typedef __true_type has_traivial_copy_constructor;
	typedef __true_type has_traivial_assignment_operator;
	typedef __true_type has_traivial_destructor;
	typedef __true_type is_POD;
};

template <>
struct __TypeTraits<unsigned char>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<short>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<unsigned short>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<int>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<unsigned int>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<long>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<unsigned long>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<float>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<double>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template <>
struct __TypeTraits<long double>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
template<class T>
struct __TypeTraits<T*>
{
	typedef __false_type has_traivial_default_constructor;
	typedef __false_type has_traivial_copy_constructor;
	typedef __false_type has_traivial_assignment_operator;
	typedef __false_type has_traivial_destructor;
	typedef __false_type is_POD;
};
__STLEND

class Test_type_traits
{
public:
	typedef __true_type has_traivial_default_constructor;
	typedef __true_type has_traivial_copy_constructor;
	typedef __true_type has_traivial_assignment_operator;
	typedef __true_type has_traivial_destructor;
	typedef __true_type is_POD;
};

void test()
{
	typedef typename Test_type_traits::is_POD typ;
	cout << typeid(typ).name()<<endl;
}
