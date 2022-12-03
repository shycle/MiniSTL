#pragma once
#include "s_iterator.h"

template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __val, _Distance*)
{
    _Distance __len = 0;
    distance(__first, __last, __len);
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        advance(__middle, __half);
        if (*__middle < __val) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
            __len = __half;
    }
    return __first;
}

template <class _ForwardIter, class _Tp>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __val) {
    __STL_REQUIRES(_ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(_Tp,
        typename iterator_traits<_ForwardIter>::value_type);
    __STL_REQUIRES(_Tp, _LessThanComparable);
    return __lower_bound(__first, __last, __val,
        __DISTANCE_TYPE(__first));
}

template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __val, _Compare __comp, _Distance*)
{
    _Distance __len = 0;
    distance(__first, __last, __len);
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        advance(__middle, __half);
        if (__comp(*__middle, __val)) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
            __len = __half;
    }
    return __first;
}

template <class _ForwardIter, class _Tp, class _Compare>
inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __val, _Compare __comp) {
    __STL_REQUIRES(_ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(_Tp,
        typename iterator_traits<_ForwardIter>::value_type);
    __STL_BINARY_FUNCTION_CHECK(_Compare, bool, _Tp, _Tp);
    return __lower_bound(__first, __last, __val, __comp,
        __DISTANCE_TYPE(__first));
}

// Binary search (lower_bound, upper_bound, equal_range, binary_search).

template <class _ForwardIter, class _Tp, class _Distance>
_ForwardIter __lower_bound2(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __val, _Distance*)
{
    _Distance __len = 0;
    distance(__first, __last, __len);
    _Distance __half;
    _ForwardIter __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        advance(__middle, __half);
        if (*__middle < __val) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        }
        else
            __len = __half;
    }
    return __first;
}

template <class _ForwardIter, class _Tp>
inline _ForwardIter lower_bound2(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __val) {
    __STL_REQUIRES(_ForwardIter, _ForwardIterator);
    __STL_REQUIRES_SAME_TYPE(_Tp,
        typename iterator_traits<_ForwardIter>::value_type);
    __STL_REQUIRES(_Tp, _LessThanComparable);
    return __lower_bound2(__first, __last, __val,
        __DISTANCE_TYPE(__first));
}