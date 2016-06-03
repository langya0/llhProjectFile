#pragma once
#include"Config.h"
typedef int Ptrdiff_t;
__STLBEGIN

struct Input_iterator_tag
{};
struct Output_iterator_tag
{};
struct Forward_iterator_tag :public Input_iterator_tag
{};
struct Bidirectional_iterator_tag :public Forward_iterator_tag
{};
struct Random_iterator_tag :public Bidirectional_iterator_tag
{};

template<class Category, class T, class Distance = ptrdiff_t,
class Ptr = T*, class Ref = T&>
struct Iterator
{
	typedef Category IteratorCategory;
	typedef T ValueType;
	typedef Distance Difference;
	typedef Ptr Pointer;
	typedef Ref Reference;
};

template<class Iterator>
struct IteratorTraits
{
	typedef typename Iterator::IteratorCategory IteratorCategory;
	typedef typename Iterator::ValueType ValueType;
	typedef typename Iterator::DifferenceType DifferenceType;
	typedef typename Iterator::Pointer Pointer;
	typedef typename Iterator::Reference Reference;
};

//Õ¥Ö­»ú¹¤×÷À²
template < class T>
struct IteratorTraits<T*>
{
	typedef Random_iterator_tag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef T* Pointer;
	typedef T& Reference;
};

template < class T>
struct IteratorTraits<const T*>
{
	typedef Random_iterator_tag IteratorCategory;
	typedef T ValueType;
	typedef ptrdiff_t DifferenceType;
	typedef const T* Pointer;
	typedef const T& Reference;
};
template<class Iterator>
inline typename IteratorTraits<Iterator>::IteratorCategory IteratorCategory(const Iterator&)
{
	typedef typename IteratorTraits<Iterator>::IteratorCategory category;
	return category();
}

template<class Iterator>
inline typename IteratorTraits<Iterator>::ValueType* ValueType(Iterator&)
{
	return typename IteratorTraits<Iterator>::ValueType(0);
}
template<class Iterator>
inline typename IteratorTraits<Iterator>::DifferenceType* DistanceType(Iterator&)
{
	return typename IteratorTraits<Iterator>::DifferenceType(0);
}

__STLEND

struct testIteratorTraits
{
	typedef stl::Input_iterator_tag IteratorCategory;
	typedef int ValueType;
	typedef stl::Random_iterator_tag Random_iterator_tag;
	typedef ValueType* Pointer;
	typedef Ptrdiff_t DifferenceType;
	typedef ValueType& Reference;
};

void test()
{
	testIteratorTraits t;
	cout << typeid((stl::ValueType(t))).name()<<endl;
}