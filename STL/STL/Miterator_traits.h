#pragma once
#include"Mstl_config.h"
namespace __STL
{
	struct input_iterator_tag
	{};
	struct output_iterator_tag
	{};
	struct forward_iterator_tag :public input_iterator_tag
	{};
	struct bidirectional_iterator_tag :public forward_iterator_tag
	{};
	struct random_iterator_tag :public bidirectional_iterator_tag
	{};

	template<class Category, class T, class Distance = ptrdiff_t,
	class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef Category iterator_category;
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
	};

	template<class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::difference_type difference_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};

	//榨汁机工作啦
	template < class T>
	struct iterator_traits<T*>
	{
		typedef random_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef T& reference;
	};

	template < class T>
	struct iterator_traits<const T*>
	{
		typedef random_iterator_tag iterator_category;
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef const T* pointer;
		typedef const T& reference;
	};
	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
	{
		typedef iterator_traits<Iterator>::iterator_category category;
		return category();
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type* value_type(Iterator&)
	{
		return iterator_traits<Iterator>::value_type(0);
	}
	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type* distance_type(Iterator&)
	{
		return iterator_traits<Iterator>::difference_type(0);
	}

}
//全局函数：获取迭代器内部相关信息

using namespace __STL;

template<class T>
class Test_iterator_traits
{
public:
	typedef random_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
};
void Test_iterator_trait()
{
	Test_iterator_traits<int> i;
	iterator_category(i);
	value_type(i);
	distance_type(i);
}