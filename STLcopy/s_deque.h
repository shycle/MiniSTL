#pragma once
#include "s_alloc.h"
#include "s_uninitialized.h"
#include "s_algobase.h"
/*
deque的特点  
1 双向开口的连续性空间
2 没有容量概念，动态以分段连续性空间组合
*/
inline size_t __deque_buf_size(size_t bufSize, size_t __size) {
	return bufSize > 0 ? bufSize : (__size < 512 ? size_t(512 / __size) : size_t(1));
}

namespace sgistl {

	template<class T, class Ref, class Ptr, size_t BufSize>
	struct __duque_iterator {
		typedef __duque_iterator<T, T&, T*, BufSize>	iterator;
		typedef __duque_iterator<T, const T&, const T*, BufSize> const_size;
		static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }

		//
		typedef random_access_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef size_t	size_type;
		typedef ptrdiff_t difference_type;
		typedef T** map_pointer;

		typedef __duque_iterator self;

		T* cur;
		T* first;
		T* last;
		map_pointer node;

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		reference operator*() { return *cur; }
		pointer operator->() { return &(operator*()); }
		difference_type operator-(const self& x) const {
			return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}
		self& operator++() {
			++cur;
			if (cur == last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}

		self operator++(int) {
			self tmp = *this;
			++* this;
			return tmp;
		}

		self& operator--() {
			if (cur == first) {
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}

		self operator--(int) {
			self tmp = *this;
			--* this;
			return tmp;
		}

		//以下实现随机存取， 迭代器可以直接跳跃n个距离
		self& operator+=(difference_type n) {
			difference_type offset = n + (cur - first);
			if (offset > 0 && offset < difference_type(buffer_size()))
				//目标位置在同一缓存区内
				cur += n;
			else {
				//标的位置不在同一缓存区内
				difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) :
					-difference_type((-offset - 1) / buffer_size()) - 1;
				//切换至正确的节点   也就是缓存区
				set_node(node + node_offset);
				//切换至正确的元素
				cur = first + (offset - node_offset * difference_type(buffer_size()));
			}
			return *this;
		}

		self& operator=(const iterator& rhs)
		{
			if (this != &rhs)
			{
				cur = rhs.cur;
				first = rhs.first;
				last = rhs.last;
				node = rhs.node;
			}
			return *this;
		}

		self operator+(difference_type n) const {
			self tmp = *this;
			return tmp += n;
		}

		self& operator-=(difference_type n) {
			self tmp = *this;
			return tmp += -n;
		}

		self operator-(difference_type n) const {
			self tmp = *this;
			return tmp -= n;
		}

		//以下实现随机存取， 迭代器可以直接跳跃n个距离

		reference operator[](difference_type __n) const { return *(*this + __n); }


		bool operator ==(const self& x) const {
			return cur == x.cur;
		}

		bool operator !=(const self& x) const {
			return !(*this == x);
		}

		bool operator <(const self& x) const {
			return (node == x.node) ? (cur < x.cur) : (node < x.node);
		}
	};



	template<class T, class Alloc = alloc, size_t BufSize = 0>
	class deque {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

	protected:
		typedef simple_alloc<value_type, Alloc> data_allocator;
		typedef simple_alloc<pointer, Alloc> map_allocator;

		void fill_initialize(size_type n, const value_type& value) {
			create_map_and_nodes(n);
			map_pointer cur;
			try {
				for (cur = start.node; cur < finish.node; ++cur) {
					uninitialized_fill(*cur, *cur + buffer_size(), value);
				}
				uninitialized_fill(finish.first, finish.cur, value);
			}
			catch (...) {

			}
		}

		void create_map_and_nodes(size_type num_elements)
		{
			size_type num_nodes = num_elements / buffer_size() + 1;

			//一个map最少管理8个节点， 最多管理n+2个节点，前后各一个节点作为备用
			map_size = max((size_type)initial_map_size, num_nodes + 2);
			map = map_allocator::allocate(map_size);

			//从最中间节点起， 可保证头尾扩充区间对称
			map_pointer nstart = map + (map_size - num_nodes) / 2;
			map_pointer nfinish = nstart + num_nodes - 1;

			map_pointer cur;
			try {
				for (cur = nstart; cur <= nfinish; ++cur)
					*cur = allocate_node();
			}
			catch (...) {

			}

			//为deque内迭代器 start 和 end设置正确的内容
			start.set_node(nstart);
			finish.set_node(nfinish);
			start.cur = start.first;
			finish.cur = finish.first + num_elements % buffer_size();
		}

		T* allocate_node()  // deque 内存分配
		{
			return data_allocator::allocate(__deque_buf_size(BufSize, sizeof(T)));
		}
		void deallocate_node(T* __p) // deque 内存释放
		{
			data_allocator::deallocate(__p, __deque_buf_size(BufSize, sizeof(T)));
		}
		T** allocate_map(size_t __n)
		{
			return map_allocator::allocate(__n);
		}
		void deallocate_map(T** __p, size_t __n)
		{
			map_allocator::deallocate(__p, __n);
		}

	public:
		deque() :start(), finish(), map(0), map_size(0)
		{
			fill_initialize(0, value_type());
		}

		explicit deque(size_type n) :start(), finish(), map(0), map_size(0)
		{
			fill_initialize(n, value_type());
		}

		deque(size_type n, const value_type& value) :start(), finish(), map(0), map_size(0)
		{
			fill_initialize(n, value);
		}

		void push_back(const value_type& t) {
			if (finish.cur != finish.last - 1) {
				construct(finish.cur, t);
				++finish.cur;
			}
			else
				push_back_aux(t);
		}

		void push_front(const value_type& t) {
			if (start.cur != start.first) {
				construct(start.cur - 1, t);
				--start.cur;
			}
			else
				push_front_aux(t);
		}

		void push_back_aux(const value_type& t);
		void push_front_aux(const value_type& t);
		void reserve_map_at_back(size_type nodes_to_add = 1) {
			if (nodes_to_add + 1 > map_size - (finish.node - map))
				reallocate_map(nodes_to_add, false);
		}

		void reserve_map_at_front(size_type nodes_to_add = 1) {
			if (nodes_to_add > size_type(start.node - map))
				reallocate_map(nodes_to_add, true);
		}
		void reallocate_map(size_type nodes_to_add,
			bool add_at_front);

		void pop_back() {
			if (finish.cur != finish.first) {
				--finish.cur;
				destroy(finish.cur);
			}
			else
				pop_back_aux();
		}

		void pop_back_aux();

		void pop_front() {
			if (start.cur != start.last - 1) {
				destroy(start.cur);
				++start.cur;
			}
			pop_front_aux();
		}

		void pop_front_aux();

	public:
		typedef  __duque_iterator <T, T&, T*, BufSize> iterator;

	protected:
		typedef pointer* map_pointer;
		static size_t buffer_size() { return __deque_buf_size(BufSize, sizeof(T)); }
		int initial_map_size = 8 ;

	protected: //Data member
		map_pointer map;
		size_type map_size;

		iterator start;
		iterator finish;

	public:
		iterator begin() { return start; }
		iterator end() { return finish; }

		reference operator[](size_type n) {
			return start[difference_type(n)];
		}
		reference front() { return *start; }
		reference back() {
			iterator tmp = finish;
			--tmp;
			return *tmp;
		}
		size_type size() const {
			return finish - start;
		}
		size_type max_size() const {
			return size_type(-1);
		}
		bool empty() const { return finish == start; }

		iterator erase(iterator pos) {
			iterator next = pos;
			++next;
			difference_type index = pos - start;
			if (size_type(index) < (this->size() >> 1)) {
			//	copy_backward(start, pos, next);
				pop_front();
			}
			else {
				//copy(next, finish, pos);
				pop_back();
			}
			return start + index;
		}
	};


	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::push_back_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node();
		__STL_TRY{
			construct(finish.cur, t_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}

		__STL_UNWIND(deallocate_node(*(finish.node + 1)));
	}

	template<class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::push_front_aux(const value_type& t) {
		value_type __t_copy = t;
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		__STL_TRY{
		  start.set_node(start.node - 1);
		  start.cur = start.last - 1;
		  construct(start.cur, __t_copy);
		}
		__STL_UNWIND((++start, deallocate_node(*(start.node - 1))));
	}

	// 重新分配 map 连续空间
	template <class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::reallocate_map(size_type nodes_to_add,
		bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes) {
			new_nstart = map + (map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				copy(start.node, finish.node + 1, new_nstart);
			else
				copy_backward(start.node, finish.node + 1,
					new_nstart + old_num_nodes);
		}
		else {
			size_type new_map_size =
				map_size + max(map_size, nodes_to_add) + 2;

			map_pointer new_map = allocate_map(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			copy(start.node, finish.node + 1, new_nstart);
			deallocate_map(map, map_size);

			map = new_map;
			map_size = new_map_size;
		}

		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	template <class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::pop_back_aux() {
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	template <class T, class Alloc, size_t BufSize>
	void deque<T, Alloc, BufSize>::pop_front_aux() {
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
}