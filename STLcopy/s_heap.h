#pragma once
#include "s_iterator.h"

//新节点与其父节点比较，如果其键值比父节点打， 就父子兑换位置， 如此一直上溯执行到 不需兑换或者到根节点为止
//
template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
{
	Distance parent = (holeIndex - 1) / 2;
	while (holeIndex > topIndex && *(first + parent) < value)
	{
		*(first + holeIndex) = *(first + parent);
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;
	}
	*(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
	//根据堆结构的特性， 新值必须置于底部
	//容器最尾端的， 此即第一个洞好， (last - first) - 1
}

template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//该函数被调用时，新元素已经被置于底部容器的最尾端
	__push_heap_aux(first, last, __DISTANCE_TYPE(first), __VALUE_TYPE(first));
}

template<class  RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;
	while (secondChild < len)
	{
		//比较洞节点的左右节点，然后用seconde表示比较大的节点
		if (*(first + secondChild) < *(first + (secondChild - 1)))
			secondChild--;
		//令较大子值为洞值，再令洞号下移至较大子节点处
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		//找出新洞节点的右子节点
		secondChild = 2 * (secondChild + 1);
	}

	if (secondChild == len) {
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
	__push_heap(first, holeIndex, topIndex, value);
}

template<class  RandomAccessIterator, class Distance, class T>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*)
{
	*result = *first;
	__adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), __DISTANCE_TYPE(first));
	//取最大值，即删除根节点
}

template<class RandomAccessIterator, class Distance, class T>
inline void __xx(RandomAccessIterator first, RandomAccessIterator last)
{
	return;
}

template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//该函数被调用时，新元素已经被置于底部容器的最尾端
	__pop_heap_aux(first, last, __VALUE_TYPE(first));
	//__xx(first, last);
}



template<class  RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1)
		pop_heap(first, last--);
}

// [first, last) 调整为 heap
template <class _RandomAccessIterator, class _Tp, class _Distance>
void
__make_heap(_RandomAccessIterator __first,
	_RandomAccessIterator __last, _Tp*, _Distance*)
{
	if (__last - __first < 2) return; // 如果长度为 0 或 1，不排列
	_Distance __len = __last - __first; // 待排列的长度
	_Distance __parent = (__len - 2) / 2; // 父节点i和右子节点2i+2

	while (true) { // 这里 __parent 就是待插入节点索引
		__adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)));
		if (__parent == 0) return;
		__parent--;
	}
}

// make_heap 不指定大小比较标准，将现有的数据转化一个 heap
template <class _RandomAccessIterator>
inline void
make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
	__STL_REQUIRES(_RandomAccessIterator, _Mutable_RandomAccessIterator);
	__STL_REQUIRES(typename iterator_traits<_RandomAccessIterator>::value_type,
		_LessThanComparable);
	__make_heap(__first, __last,
		__VALUE_TYPE(__first), __DISTANCE_TYPE(__first));
}