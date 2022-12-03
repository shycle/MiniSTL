#pragma once
#include "s_function.h"
#include <string.h>

template <class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
	for ( ;n > 0; --n, ++first)
		*first = value;
	return first;
}

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
    for (; first != last; first++)
        *first = value;
}

template <class _Tp>
inline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
    memmove(__result, __first, sizeof(_Tp) * (__last - __first));
    return __result + (__last - __first);
}

// copy_backward
// ÄæÏò¸´ÖÆ
template <class _BidirectionalIter1, class _BidirectionalIter2,
    class _Distance>
    inline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first,
        _BidirectionalIter1 __last,
        _BidirectionalIter2 __result,
        sgistl::bidirectional_iterator_tag,
        _Distance*)
{
    while (__first != __last)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
inline _BidirectionalIter __copy_backward(_RandomAccessIter __first,
    _RandomAccessIter __last,
    _BidirectionalIter __result,
    sgistl::random_access_iterator_tag,
    _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
    return __copy_backward(__first, __last, __result,
        __ITERATOR_CATEGORY(__first),
        __DISTANCE_TYPE(__first));
}

template <class T>
const T& max(const T& lhs, const T& rhs)
{
    return lhs < rhs ? rhs : lhs;
}

namespace mystl
{

    template <class T>
    inline void swap(T& a, T& b)
    {
        T tmp = a;
        a = b;
        b = tmp;
    }
}