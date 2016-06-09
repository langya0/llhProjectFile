#pragma once
#include "Config.h"
#include "Trace.h"
typedef int Ptrdiff_t;

__STLBEGIN

struct InputIteratorTag
{};
struct OutputIteratorTag
{};
struct ForwardIteratorTag :public InputIteratorTag
{};
struct BidirectionalIteratorTag :public ForwardIteratorTag
{};
struct RandomIteratorTag :public BidirectionalIteratorTag
{};

template<class Category, class T, class Distance = Ptrdiff_t,
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
	typedef RandomIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef Ptrdiff_t DifferenceType;
	typedef T* Pointer;
	typedef T& Reference;
};

template < class T>
struct IteratorTraits<const T*>
{
	typedef RandomIteratorTag IteratorCategory;
	typedef T ValueType;
	typedef Ptrdiff_t DifferenceType;
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
	typedef stl::InputIteratorTag IteratorCategory;
	typedef int ValueType;
	typedef stl::RandomIteratorTag RandomIteratorTag;
	typedef ValueType* Pointer;
	typedef Ptrdiff_t DifferenceType;
	typedef ValueType& Reference;
};
#include <typeinfo>
void testIterator()
{
	testIteratorTraits t;
	__TRACE("%s\n",typeid(stl::ValueType(t)).name());
}