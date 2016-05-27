#pragma once
#include"Mstl_config.h"
__STLBEGIN
struct Input_iterator_tag
{};
struct Output_iterator_tag
{};
struct forward_iterator_tag :public Input_iterator_tag
{};
struct Bidirectional_iterator_tag :public forward_iterator_tag
{};
struct Random_iterator_tag :public Bidirectional_iterator_tag
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
struct Iterator_traits
{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::difference_type difference_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
};

//榨汁机工作啦
template < class T>
struct Iterator_traits<T*>
{
	typedef Random_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;
};

template < class T>
struct Iterator_traits<const T*>
{
	typedef Random_iterator_tag iterator_category;
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef const T* pointer;
	typedef const T& reference;
};
template<class Iterator>
inline typename Iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
{
	typedef Iterator_traits<Iterator>::iterator_category category;
	return category();
}
template<class Iterator>
inline typename Iterator_traits<Iterator>::value_type* value_type(Iterator&)
{
	return Iterator_traits<Iterator>::value_type(0);
}
template<class Iterator>
inline typename Iterator_traits<Iterator>::difference_type* distance_type(Iterator&)
{
	return Iterator_traits<Iterator>::difference_type(0);
}

__STLEND
//全局函数：获取迭代器内部相关信息
