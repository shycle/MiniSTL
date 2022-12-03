#pragma once
#include "s_iterator.h"
#include "type_traits.h"
#include "s_construct.h"
#include "s_algobase.h"


// Valid if copy construction is equivalent to assignment, and if the
// destructor is trivial.
template <class _ForwardIter, class _Tp>
inline void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __x, __true_type)
{
    fill(__first, __last, __x);
}

template <class _ForwardIter, class _Tp>
void
__uninitialized_fill_aux(_ForwardIter __first, _ForwardIter __last,
    const _Tp& __x, __false_type)
{
    _ForwardIter __cur = __first;
    __STL_TRY{
      for (; __cur != __last; ++__cur)
        _Construct(&*__cur, __x);
    }
    __STL_UNWIND(_Destroy(__first, __cur));
}

template <class _ForwardIter, class _Tp, class _Tp1>
inline void __uninitialized_fill(_ForwardIter __first,
    _ForwardIter __last, const _Tp& __x, _Tp1*)
{
    typedef typename __type_traits<_Tp1>::is_POD_type _Is_POD;
    __uninitialized_fill_aux(__first, __last, __x, _Is_POD());

}

template <class _ForwardIter, class _Tp>
inline void uninitialized_fill(_ForwardIter __first,
    _ForwardIter __last,
    const _Tp& __x)
{
    __uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
	return fill_n(first, n, x);
}

template <class ForwardIterator, class Size, class T>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
	ForwardIterator cur = first;
	for ( n > 0; --n; ++cur)
	{
		construct(&*cur, x);
	}
	return cur;
}


template <class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
	//return first;
}



#if 0
template <class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return first;
}
#endif

template <class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_m(ForwardIterator first, Size n, const T& x)
{
	return __uninitialized_fill_n(first, n, x, __VALUE_TYPE(first));
}

template <class _InputIter, class _ForwardIter>
inline _ForwardIter
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
    _ForwardIter __result,
    __true_type)
{
    return copy(__first, __last, __result);
}

template <class _InputIter, class _ForwardIter>
_ForwardIter
__uninitialized_copy_aux(_InputIter __first, _InputIter __last,
    _ForwardIter __result,
    __false_type)
{
    _ForwardIter __cur = __result;
    try {
        for (; __first != __last; ++__first, ++__cur)
            _Construct(&*__cur, *__first);
        return __cur;
    }

    catch (int e)
    {
        _Destroy(__result, __cur);
        throw;
    }
}


template <class _InputIter, class _ForwardIter, class _Tp>
inline _ForwardIter
__uninitialized_copy(_InputIter __first, _InputIter __last,
    _ForwardIter __result, _Tp*)
{
    typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
    return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
}

// 内存的配置与对象的构造行为分离开来。
template <class _InputIter, class _ForwardIter>
inline _ForwardIter
uninitialized_copy(_InputIter __first, _InputIter __last,
    _ForwardIter __result)
{
    return __uninitialized_copy(__first, __last, __result,
        __VALUE_TYPE(__result));
}

