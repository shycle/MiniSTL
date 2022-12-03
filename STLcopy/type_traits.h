#pragma once
/*
 *
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

#ifndef __TYPE_TRAITS_H
#define __TYPE_TRAITS_H

#ifndef __STL_CONFIG_H
#include "stl_config.h"
#endif


 /* __type_traits: 提供了一种机制，允许针对不同的类型属性，在编译时期完成函数派送决定(function dispatch). */

 // class 没有任何成员，不会带来额外负担，却又能够标示真假。
struct __true_type {
};

struct __false_type {
};

// trivial 平凡
template <class _Tp>
struct __type_traits {
    typedef __true_type     this_dummy_member_must_be_first;
    /* Do not remove this member. It informs a compiler which
       automatically specializes __type_traits that this
       __type_traits template is special. It just makes sure that
       things work if an implementation is using a template
       called __type_traits for something unrelated. */

       /* The following restrictions should be observed for the sake of
          compilers which automatically produce type specific specializations
          of this class:
              - You may reorder the members below if you wish
              - You may remove any of the members below if you wish
              - You must not rename members without making the corresponding
                name change in the compiler
              - Members you add will be treated like regular members unless
                you add the appropriate support in the compiler. */


    typedef __false_type    has_trivial_default_constructor;
    typedef __false_type    has_trivial_copy_constructor;
    typedef __false_type    has_trivial_assignment_operator;
    typedef __false_type    has_trivial_destructor;
    typedef __false_type    is_POD_type;
};



// Provide some specializations.  This is harmless for compilers that
//  have built-in __types_traits support, and essential for compilers
//  that don't.

#ifndef __STL_NO_BOOL

__STL_TEMPLATE_NULL struct __type_traits<bool> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#endif /* __STL_NO_BOOL */

__STL_TEMPLATE_NULL struct __type_traits<char> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<signed char> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned char> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#ifdef __STL_HAS_WCHAR_T

__STL_TEMPLATE_NULL struct __type_traits<wchar_t> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#endif /* __STL_HAS_WCHAR_T */

__STL_TEMPLATE_NULL struct __type_traits<short> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned short> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<int> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned int> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<long> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#ifdef __STL_LONG_LONG

__STL_TEMPLATE_NULL struct __type_traits<long long> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned long long> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#endif /* __STL_LONG_LONG */

__STL_TEMPLATE_NULL struct __type_traits<float> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<double> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<long double> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

template <class _Tp>
struct __type_traits<_Tp*> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#else /* __STL_CLASS_PARTIAL_SPECIALIZATION */

__STL_TEMPLATE_NULL struct __type_traits<char*> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<signed char*> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned char*> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<const char*> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<const signed char*> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<const unsigned char*> {
    typedef __true_type    has_trivial_default_constructor;
    typedef __true_type    has_trivial_copy_constructor;
    typedef __true_type    has_trivial_assignment_operator;
    typedef __true_type    has_trivial_destructor;
    typedef __true_type    is_POD_type;
};

#endif /* __STL_CLASS_PARTIAL_SPECIALIZATION */


// The following could be written in terms of numeric_limits.  
// We're doing it separately to reduce the number of dependencies.

template <class _Tp> struct _Is_integer {
    typedef __false_type _Integral;
};

#ifndef __STL_NO_BOOL

__STL_TEMPLATE_NULL struct _Is_integer<bool> {
    typedef __true_type _Integral;
};

#endif /* __STL_NO_BOOL */

__STL_TEMPLATE_NULL struct _Is_integer<char> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<signed char> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<unsigned char> {
    typedef __true_type _Integral;
};

#ifdef __STL_HAS_WCHAR_T

__STL_TEMPLATE_NULL struct _Is_integer<wchar_t> {
    typedef __true_type _Integral;
};

#endif /* __STL_HAS_WCHAR_T */

__STL_TEMPLATE_NULL struct _Is_integer<short> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<unsigned short> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<int> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<unsigned int> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<long> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<unsigned long> {
    typedef __true_type _Integral;
};

#ifdef __STL_LONG_LONG

__STL_TEMPLATE_NULL struct _Is_integer<long long> {
    typedef __true_type _Integral;
};

__STL_TEMPLATE_NULL struct _Is_integer<unsigned long long> {
    typedef __true_type _Integral;
};

#endif /* __STL_LONG_LONG */

#endif /* __TYPE_TRAITS_H */

// Local Variables:
// mode:C++
// End:
