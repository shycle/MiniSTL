#pragma once
#include "s_define.h"

namespace sgistl
{

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template <class _Tp, class _Distance> 
	struct input_iterator {
		typedef input_iterator_tag iterator_category;
		typedef _Tp                value_type;
		typedef _Distance          difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};

	struct output_iterator {
		typedef output_iterator_tag iterator_category;
		typedef void                value_type;
		typedef void                difference_type;
		typedef void                pointer;
		typedef void                reference;
	};

	template <class _Tp, class _Distance> 
	struct forward_iterator {
		typedef forward_iterator_tag iterator_category;
		typedef _Tp                  value_type;
		typedef _Distance            difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};


	template <class _Tp, class _Distance> 
	struct bidirectional_iterator {
		typedef bidirectional_iterator_tag iterator_category;
		typedef _Tp                        value_type;
		typedef _Distance                  difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};

	template <class _Tp, class _Distance> 
	struct random_access_iterator {
		typedef random_access_iterator_tag iterator_category;
		typedef _Tp                        value_type;
		typedef _Distance                  difference_type;
		typedef _Tp* pointer;
		typedef _Tp& reference;
	};


	template <class _Category, class _Tp, class _Distance = ptrdiff_t,
		class _Pointer = _Tp*, class _Reference = _Tp&>
		struct iterator {
		typedef _Category  iterator_category;
		typedef _Tp        value_type;
		typedef _Distance  difference_type;
		typedef _Pointer   pointer;
		typedef _Reference reference;
	};

	// "榨汁机” traits
	template <class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type value_type;
		typedef typename Iterator::difference_type 	difference_type;
		typedef typename Iterator::pointer pointer;
		typedef typename Iterator::reference reference;
	};

	// 针对原生指针设计的traits特偏化版
	template <class T>
	struct iterator_traits<T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T		value_type;
		typedef T&		reference;
		typedef T*		pointer;
		typedef ptrdiff_t		difference_type;
	};

	template <class T>
	struct iterator_traits<const T*> {
		typedef random_access_iterator_tag		iterator_category;
		typedef T		value_type;
		typedef const T& reference;
		typedef const T* pointer;
		typedef ptrdiff_t		difference_type;
	};

	template <class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&)
	{
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	template <class _Tp, class _Distance>
	inline input_iterator_tag
		iterator_category(const input_iterator<_Tp, _Distance>&)
	{
		return input_iterator_tag();
	}

	inline output_iterator_tag iterator_category(const output_iterator&)
	{
		return output_iterator_tag();
	}

	template <class _Tp, class _Distance>
	inline forward_iterator_tag
		iterator_category(const forward_iterator<_Tp, _Distance>&)
	{
		return forward_iterator_tag();
	}

	template <class _Tp, class _Distance>
	inline bidirectional_iterator_tag
		iterator_category(const bidirectional_iterator<_Tp, _Distance>&)
	{
		return bidirectional_iterator_tag();
	}

	template <class _Tp, class _Distance>
	inline random_access_iterator_tag
		iterator_category(const random_access_iterator<_Tp, _Distance>&)
	{
		return random_access_iterator_tag();
	}

	template <class _Tp>
	inline random_access_iterator_tag iterator_category(const _Tp*)
	{
		return random_access_iterator_tag();
	}

	template <class _Tp, class _Distance>
	inline _Tp* value_type(const input_iterator<_Tp, _Distance>&)
	{
		return (_Tp*)(0);
	}

	template <class _Tp, class _Distance>
	inline _Tp* value_type(const forward_iterator<_Tp, _Distance>&)
	{
		return (_Tp*)(0);
	}

	template <class _Tp, class _Distance>
	inline _Tp* value_type(const bidirectional_iterator<_Tp, _Distance>&)
	{
		return (_Tp*)(0);
	}

	template <class _Tp, class _Distance>
	inline _Tp* value_type(const random_access_iterator<_Tp, _Distance>&)
	{
		return (_Tp*)(0);
	}

	template <class _Tp>
	inline _Tp* value_type(const _Tp*) { return (_Tp*)(0); }

	template <class _Tp, class _Distance>
	inline _Distance* distance_type(const input_iterator<_Tp, _Distance>&)
	{
		return (_Distance*)(0);
	}

	template <class _Tp, class _Distance>
	inline _Distance* distance_type(const forward_iterator<_Tp, _Distance>&)
	{
		return (_Distance*)(0);
	}

	template <class _Tp, class _Distance>
	inline _Distance*
		distance_type(const bidirectional_iterator<_Tp, _Distance>&)
	{
		return (_Distance*)(0);
	}

	template <class _Tp, class _Distance>
	inline _Distance*
		distance_type(const random_access_iterator<_Tp, _Distance>&)
	{
		return (_Distance*)(0);
	}

	template <class _Tp>
	inline ptrdiff_t* distance_type(const _Tp*) { return (ptrdiff_t*)(0); }

	template <class II>
	inline typename iterator_traits<II>::difference_type
		__distance(II first, II last, sgistl::input_iterator_tag)
	{
		typename iterator_traits<II>::difference_type n = 0;
		while (first != last) { ++first; ++n; }
		return n;
	}


	template <class II>
	inline typename iterator_traits<II>::difference_type
		__distance(II first, II last, sgistl::random_access_iterator_tag)
	{
		return last - first;
	}

	template <class II>
	inline typename iterator_traits<II>::difference_type
		distance(II first, II last)
	{
		typedef typename iterator_traits<II>::iterator_category category;
		return __distance(first, last, category());
	}


	template <class _InputIterator, class _Distance>
	inline void __distance(_InputIterator __first, _InputIterator __last,
		_Distance& __n, sgistl::input_iterator_tag)
	{
		while (__first != __last) { ++__first; ++__n; }
	}

	template <class _RandomAccessIterator, class _Distance>
	inline void __distance(_RandomAccessIterator __first,
		_RandomAccessIterator __last,
		_Distance& __n, sgistl::random_access_iterator_tag)
	{
		__STL_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
		__n += __last - __first;
	}


	template <class II, class Distance>
	inline typename iterator_traits<II>::difference_type
		distance(II first, II last, Distance& n)
	{
		typedef typename iterator_traits<II>::iterator_category category;
		__distance(first, last, n, category());
		return n;
	}
}


template <class II>
int _distanceSize(II first, II last, sgistl::input_iterator_tag)
{

	int __n = 0;
	while (first != last) { ++first; ++__n; }
	return __n;
}

template <class RI>
int _distanceSize(RI first, RI last, sgistl::random_access_iterator_tag)
{

	int __n = 0;
	__n += last - first;
	return __n;
}

template <class II>
int distanceSize(II _first, II _last)
{
	return  _distanceSize(_first, _last, sgistl::iterator_category(_first));
}

#define __VALUE_TYPE(i)			sgistl::value_type(i)
#define __ITERATOR_CATEGORY(__i) sgistl::iterator_category(__i)
#define __DISTANCE_TYPE(__i)     sgistl::distance_type(__i)


template <class _InputIter, class _Distance>
inline void __advance(_InputIter& __i, _Distance __n, sgistl::input_iterator_tag) {
	while (__n--) ++__i;
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma set woff 1183
#endif

template <class _BidirectionalIterator, class _Distance>
inline void __advance(_BidirectionalIterator& __i, _Distance __n,
	sgistl::bidirectional_iterator_tag) {
	__STL_REQUIRES(_BidirectionalIterator, _BidirectionalIterator);
	if (__n >= 0)
		while (__n--) ++__i;
	else
		while (__n++) --__i;
}

#if defined(__sgi) && !defined(__GNUC__) && (_MIPS_SIM != _MIPS_SIM_ABI32)
#pragma reset woff 1183
#endif

template <class _RandomAccessIterator, class _Distance>
inline void __advance(_RandomAccessIterator& __i, _Distance __n,
	sgistl::random_access_iterator_tag) {
	__STL_REQUIRES(_RandomAccessIterator, _RandomAccessIterator);
	__i += __n;
}

template <class _InputIterator, class _Distance>
inline void advance(_InputIterator& __i, _Distance __n) {
	__STL_REQUIRES(_InputIterator, _InputIterator);
	__advance(__i, __n, sgistl::iterator_category(__i));
}