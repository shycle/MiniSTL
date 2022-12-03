#pragma once
// pair 键值对
template <class _T1, class _T2>
struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;

    _T1 first;  // 第一个参数
    _T2 second; // 第二个参数
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(_T1& __a, _T2& __b) : first(__a), second(__b) {}

#ifdef __STL_MEMBER_TEMPLATES
    template <class _U1, class _U2>
    pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
#endif
};

// pair 对象的比较操作
template <class _T1, class _T2>
inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first == __y.first && __x.second == __y.second;
}

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first ||
        (!(__y.first < __x.first) && __x.second < __y.second);
}

#ifdef __STL_FUNCTION_TMPL_PARTIAL_ORDER

template <class _T1, class _T2>
inline bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
    return !(__x == __y);
}

template <class _T1, class _T2>
inline bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
    return __y < __x;
}

template <class _T1, class _T2>
inline bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
    return !(__y < __x);
}

template <class _T1, class _T2>
inline bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
    return !(__x < __y);
}

#endif /* __STL_FUNCTION_TMPL_PARTIAL_ORDER */

// make_pair 创建一个根据参数类型所定义类型的 pair 对象
template <class _T1, class _T2>
inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
{
    return pair<_T1, _T2>(__x, __y);
}