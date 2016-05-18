#pragma once
#include"Mstl_config.h"
namespace __STL
{
	//为了类型判别区分而产生的两个空结构体。用于策略选择
	struct __true_type
	{
		__true_type()
		:_t(1)
		{}
		bool _t;
	};
	struct __false_type
	{
		__false_type()
		:_t(0)
		{}
		bool _t;
	};
	//这里定义默认类型都是__false_type的，而内置类型都对此做了特化。
	//所以，可以看到内置类型会搞笑的利用萃取功能。
	//那么我们的自定义类型如果想要接合STL使用，就要妥协，实现特化。
	template<class T>
	struct __type_traits
	{
		//通知“有能力自动将__type_traits特化”的编译器，这个template是特殊的。
		//保证即使有同名无关变量，该有的事情也能顺利进行。
		typedef __true_type this_dummy_member_must_be_first;

		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned char>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<short>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned short>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<int>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned int>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<long>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<unsigned long>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<float>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<double>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template <>
	struct __type_traits<long double>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
	template<class T>
	struct __type_traits<T*>
	{
		typedef __false_type has_traivial_default_constructor;
		typedef __false_type has_traivial_copy_constructor;
		typedef __false_type has_traivial_assignment_operator;
		typedef __false_type has_traivial_destructor;
		typedef __false_type is_POD;
	};
}

using namespace __STL;
template<class T>
class Test_type_traits
{
public:
	typedef __true_type has_traivial_default_constructor;
	typedef __true_type has_traivial_copy_constructor;
	typedef __true_type has_traivial_assignment_operator;
	typedef __true_type has_traivial_destructor;
	typedef __true_type is_POD;
};

void test(__true_type)
{}
void test(__false_type)
{}
void Test_type_trait()
{
	test(Test_type_traits<int>::is_POD());
}