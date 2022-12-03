#pragma once
#include "s_alloc.h"
#include "s_iterator.h"
#include <iostream>

namespace sgistl
{



	template <class T>
	struct __list_node {
	public:
		typedef void* void_pointer;
		void_pointer prev; //型别为void*  七十可设置为__list_node<T>*
		void_pointer next;
		T data;
	};

	template <class T, class Ref, class Ptr>
	struct __list_iterator {
		typedef __list_iterator<T, T&, T*>	iterator;
		typedef __list_iterator<T, Ref, Ptr>	self;

		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef __list_node<T>* link_type;
		typedef size_t	size_type;
		typedef ptrdiff_t difference_type;

		link_type node;//迭代器内部的普通指针， 指向list的节点

		//constructor
		__list_iterator(link_type x) :node(x) { }
		__list_iterator() { }
		__list_iterator(const iterator& x) : node(x.node) { }

		bool operator==(const self& x) const { return node == x.node; }
		bool operator!=(const self& x) const { return node != x.node; }

		reference operator*() const { return (*node).data; }
		pointer operator->() const { return &(operator*()); }

		self& operator++() {
			node = (link_type)((*node).next);
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--() {
			node = (link_type)((*node).prev);
			return *this;
		}

		self& operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}


	};


	template <class T, class Alloc = alloc>
	struct list
	{
	protected:
		typedef __list_node<T> list_node;
		typedef simple_alloc<list_node, Alloc> list_node_allocator;

	public:
		typedef list_node* link_type;
		typedef __list_iterator<T, T&, T*> iterator;



		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef const value_type& const_reference;


	protected:
		link_type node;
		size_t _size;

	public:
		list()
		{
			_size = 0;
			empty_initialize();
		}
		explicit list(size_type n);
		list(size_type n, const T& value);
		list(iterator first, iterator last);
		list(const list& rhs);

		iterator begin() { return (link_type)((*node).next); }
		iterator end() { return node; }
		size_type size() {
			size_type result = 0;
			result = distance(begin(), end());
			_size = result;
			return result;
		}
		bool empty() const { return node->next == node; }
		reference front() { return *begin(); }
		reference back() { return *(--end()); }
		void push_front(const T& x) { insert(begin(), x); }
		void push_back(const T& x) { insert(end(), x); }
		void pop_front() {
			erase(begin());
		}
		void pop_back() {
			iterator tmp = end();
			erase(--tmp);
			}

		iterator insert(iterator pos, const T& x)
		{
			link_type tmp = create_node(x);
			tmp->next = pos.node;
			tmp->prev = pos.node->prev;
			(link_type(pos.node->prev))->next = tmp;
			pos.node->prev = tmp;
			if (size() == 1)
				node->next = tmp;
			return tmp;
		}
		iterator erase(iterator iter)
		{
			link_type next_node = link_type(iter.node->next);
			link_type prev_node = link_type(iter.node->prev);

			prev_node->next = next_node;
			next_node->prev = prev_node;
			destroy_node(iter.node);
			return (iterator)next_node;
		}

		void clear();
		void remove(const T& value);
		void unique();
		


	protected:
		link_type get_node() { return list_node_allocator::allocate(); }
		void put_node(link_type p) { list_node_allocator::deallocate(p); }
		void empty_initialize() {
			node = get_node();
			node->next = node;
			node->prev = node;
		}

		link_type create_node(const T& x) {
			link_type p = get_node();
			construct(&p->data, x);
			return p;
		}

		void destroy_node(link_type p) {
			destroy(&p->data);
			put_node(p);
		}

		//将first，last内所有元素移动到pos之前
		void transfer(iterator pos, iterator first, iterator last)
		{
			if (pos != last)
			{
				(*(link_type((*last.node).prev))).next = pos.node;
				(*(link_type((*first.node).prev))).next = last.node;
				(*(link_type((*pos.node).prev))).next = first.node;
				link_type tmp = link_type((*pos.node).prev);
				(*pos.node).prev = (*last.node).prev;
				(*last.node).prev = (*first.node).prev;
				(*first.node).prev = tmp;
			}
		}
	public:
		void swap (list& rhs) noexcept
		{
			mystl::swap(node, rhs.node);
			mystl::swap(_size, rhs._size);
		}

		void splice(iterator pos, list& x) {
			if (!x.empty()) {
				transfer(pos, x.begin(), x.end());
			}
		}
		void splice(iterator pos, list&, iterator i) {
			iterator j = i;
			++j;
			if (pos == i || pos == j) return;
			transfer(pos, i, j);
		}
		void splice(iterator pos, list&, iterator first, iterator last) {
			if (first != last) {
				transfer(pos, first, last);
			}
		}

		void merge(list& x)
		{
			iterator first1 = begin();
			iterator last1 = end();
			iterator first2 = begin();
			iterator last2 = end();

			//前提是两个list必须是已经递增排序过
			while (first1 != last1 && first2 != last2)
			{
				if (*first2 < *first1)
				{
					iterator next = first2;
					transfer(first1, first2, ++next);
					first2 = next;
				}
				else
					++first1;
				if (first2 != last2) transfer(last1, first2, last2);
			}
		}

		void reverse() {
			if (node->next == node || link_type(node->next)->next == node)
				return;
			iterator first = begin();
			++first;
			while (first != end())
			{
				iterator old = first;
				++first;
				transfer(begin(), old, first);
				auto ite = begin();
				for (ite =begin(); ite != end(); ite++)
					std::cout << *ite << ' ';
				std::cout << std::endl;
			}
		}

		//quick sort
		void sort()
		{
			list<T, Alloc> carry;
			list<T, Alloc> counter[64];
			int fill = 0;
			while (!empty())
			{
				carry.splice(carry.begin(), *this, begin());
				int i = 0;
				while (i < fill && !counter[i].empty()) {
					counter[i].merge(carry);
					carry.swap(counter[i++]);
				}
				carry.swap(counter[i]);
				if (i == fill) ++fill;
			}
			for (int i = 1; i < fill; ++i)
				counter[i].merge(counter[i - 1]);
			swap(counter[fill - 1]);
		}
	};

	template<class T, class Alloc>
	void list<T, Alloc>::clear() {
		link_type cur = (link_type)node->next;
		while (cur != node) {
			link_type tmp = cur;
			cur = link_type(cur->next);
			destroy_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::remove(const T& value) {
		iterator first = begin();
		iterator last = end();
		while (first != last)
		{
			iterator next = first;
			++next;
			if (*first == value) erase(first);
			first = next;
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::unique() {
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		iterator next = first;
		while (++next != last)
		{
			if (*first == *next)
				erase(next);
			else
				first = next;
			next = first;
		}
	}
}