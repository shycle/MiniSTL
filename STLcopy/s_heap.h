#pragma once
#include "s_iterator.h"

//�½ڵ����丸�ڵ�Ƚϣ�������ֵ�ȸ��ڵ�� �͸��Ӷһ�λ�ã� ���һֱ����ִ�е� ����һ����ߵ����ڵ�Ϊֹ
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
	//���ݶѽṹ�����ԣ� ��ֵ�������ڵײ�
	//������β�˵ģ� �˼���һ�����ã� (last - first) - 1
}

template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//�ú���������ʱ����Ԫ���Ѿ������ڵײ���������β��
	__push_heap_aux(first, last, __DISTANCE_TYPE(first), __VALUE_TYPE(first));
}

template<class  RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;
	while (secondChild < len)
	{
		//�Ƚ϶��ڵ�����ҽڵ㣬Ȼ����seconde��ʾ�Ƚϴ�Ľڵ�
		if (*(first + secondChild) < *(first + (secondChild - 1)))
			secondChild--;
		//��ϴ���ֵΪ��ֵ��������������ϴ��ӽڵ㴦
		*(first + holeIndex) = *(first + secondChild);
		holeIndex = secondChild;
		//�ҳ��¶��ڵ�����ӽڵ�
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
	//ȡ���ֵ����ɾ�����ڵ�
}

template<class RandomAccessIterator, class Distance, class T>
inline void __xx(RandomAccessIterator first, RandomAccessIterator last)
{
	return;
}

template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
	//�ú���������ʱ����Ԫ���Ѿ������ڵײ���������β��
	__pop_heap_aux(first, last, __VALUE_TYPE(first));
	//__xx(first, last);
}



template<class  RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
	while (last - first > 1)
		pop_heap(first, last--);
}

// [first, last) ����Ϊ heap
template <class _RandomAccessIterator, class _Tp, class _Distance>
void
__make_heap(_RandomAccessIterator __first,
	_RandomAccessIterator __last, _Tp*, _Distance*)
{
	if (__last - __first < 2) return; // �������Ϊ 0 �� 1��������
	_Distance __len = __last - __first; // �����еĳ���
	_Distance __parent = (__len - 2) / 2; // ���ڵ�i�����ӽڵ�2i+2

	while (true) { // ���� __parent ���Ǵ�����ڵ�����
		__adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)));
		if (__parent == 0) return;
		__parent--;
	}
}

// make_heap ��ָ����С�Ƚϱ�׼�������е�����ת��һ�� heap
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