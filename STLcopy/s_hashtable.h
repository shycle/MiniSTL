#pragma once
#include "s_alloc.h"
#include "s_iterator.h"
#include "s_algo.h"
//#include "s_vector.h"
#include<vector>

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc = alloc>
class hashtable;

template <class Value>
struct __hashtablenode {
	__hashtablenode* next;
	Value val;
};



template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_iterator {
	typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
//	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef __hashtablenode<Value> node;

	typedef sgistl::forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef Value& reference;
	typedef Value* pointer;

	node* cur;	//迭代器目前所指之节点
	hashtable* ht;	//保持对容器的连结关系

	__hashtable_iterator(node* n, hashtable* tab): cur(n), ht(tab) { }
	__hashtable_iterator() {}
	reference operator*() const { return cur->val; }
	pointer operator->() const { return &(operator*()); }
	iterator& operator++() {
		const node* old = cur;
		cur = cur->next;
		if (!cur) {
			size_type bucket = ht->bkt_num(old->val);
			while (!cur && ++bucket < ht->buckets.size())
				cur = ht->buckets[bucket];
		}
		return *this;
	}
	iterator operator++(int) {
		iterator tmp = *this;
		++* this;
		return tmp;
	}
	bool operator==(const iterator& it) const { return cur == it.cur; }
	bool operator!=(const iterator& it) const { return cur != it.cur; }
};

// Note: assumes long is at least 32 bits.
enum { __stl_num_primes = 28 };

static const size_t __stl_prime_list[__stl_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};



inline size_t __stl_next_prime(size_t __n)
{
	const size_t* __first = __stl_prime_list;
	const size_t* __last = __stl_prime_list + (int)__stl_num_primes;
	const size_t* pos = lower_bound2(__first, __last, __n);
	return pos == __last ? *(__last - 1) : *pos;
}



template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable {
public:
	typedef Value value_type;
	typedef Key key_type;
	typedef HashFcn hasher;
	typedef EqualKey key_equal;

	typedef size_t size_type;
	typedef ptrdiff_t         difference_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

//	hashtable<int, int, hash<int>, identity<int>, equal_to<int>, alloc> iht(50, hash<int>(), equal_to<int>());
public:
	hashtable(size_type n, const HashFcn& hf, const EqualKey& eql) :
		hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
		initialize_buckets(n);
	}


	~hashtable() { clear(); }

public:
	typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
		iterator;
//	typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>	const_iterator;

	friend struct
		__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
//	friend struct 	__hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;

private:
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	typedef __hashtablenode<Value> node;
	typedef simple_alloc<node, Alloc> node_allocator;
//	sgistl::vector<node*> buckets;
	std::vector<node*> buckets;
	size_type num_elements;

private:
	node* new_node(const value_type& obj)
	{
		node* n = node_allocator::allocate();
		n->next = 0;
		__STL_TRY{
			construct(&n->val, obj);
			return n;
		}
		__STL_UNWIND(node_allocator::deallocate(n));
	}

	void deletenode(node* n)
	{
		destroy(&n->val);
		node_allocator::deallocate(n);
	}

	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);
		//如果传入50 返回53 以下爱首先保留53个元素空间 然后全部填0
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, (node*)0);
		num_elements = 0;
	}


public:
	size_type size() const { return num_elements; }
	size_type max_size() const { return size_type(-1); }
	bool empty() const { return size() == 0; }

	

	iterator find(const key_type& __key)
	{
		size_type __n = bkt_num_key(__key);
		node* __first;
		for (__first = buckets[__n];
			__first && !equals(get_key(__first->val), __key);
			__first = __first->next)
		{
		}
		return iterator(__first, this);
	}

	size_type count(const key_type& __key) const
	{
		const size_type __n = bkt_num_key(__key);
		size_type __result = 0;

		for (const node* __cur = buckets[__n]; __cur; __cur = __cur->next)
			if (equals(get_key(__cur->val), __key))
				++__result;
		return __result;
	}

	void swap(hashtable& __ht)
	{
		__STD::swap(hash, __ht.hash);
		__STD::swap(equals, __ht.equals);
		__STD::swap(get_key, __ht.get_key);
		buckets.swap(__ht.buckets);
		__STD::swap(num_elements, __ht.num_elements);
	}

	iterator begin()
	{
		for (size_type __n = 0; __n < buckets.size(); ++__n)
			if (buckets[__n])
				return iterator(buckets[__n], this);
		return end();
	}

	iterator end() { return iterator(0, this); }



	void clear();

	void copy_from(const hashtable& ht);

public:
	//bucket个数即buckets vector的大小
	size_type bucket_count()  { return buckets.size(); }

	size_type elems_in_bucket(size_type __bucket) const
	{
		size_type __result = 0;
		for (node* __cur = buckets[__bucket]; __cur; __cur = __cur->next)
			__result += 1;
		return __result;
	}

	size_type bkt_num(const value_type& obj, size_t n) const {
		return bkt_num_key(get_key(obj), n);
	}

	size_type bkt_num(const value_type& obj) const {
		return bkt_num_key(get_key(obj));
	}

	size_type bkt_num_key(const key_type& key) const
	{
		return bkt_num_key(key, buckets.size());
	}

	size_type bkt_num_key(const key_type& key, size_t n) const
	{
		return hash(key) % n;
	}

	size_type max_bucket_count() const
	{
		return __stl_prime_list[__stl_num_primes - 1];
	}
	
	//返回最近于n并大于或等于n的质数
	size_type next_size(size_type n) const {
		return __stl_next_prime(n);
	}

	pair<iterator, bool> insert_unique(const value_type& obj) {
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}

	void resize(size_type __num_elements_hint);

	pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool> insert_unique_noresize(const value_type& __obj);

	iterator insert_equal_noresize(const value_type& __obj) {
		const size_type __n = bkt_num(__obj);
		node* __first = buckets[__n];

		for (node* __cur = __first; __cur; __cur = __cur->next)
			if (equals(get_key(__cur->val), get_key(__obj))) {
				node* __tmp = new_node(__obj);
				__tmp->next = __cur->next;
				__cur->next = __tmp;
				++num_elements;
				return iterator(__tmp, this);
			}

		node* __tmp = new_node(__obj);
		__tmp->next = __first;
		buckets[__n] = __tmp;
		++num_elements;
		return iterator(__tmp, this);
	}

	iterator insert_equal(const value_type& obj) {
		resize(num_elements + 1);
		return insert_equal_noresize(obj);
	}
};


//<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type __num_elements_hint)
{
	const size_type __old_n = buckets.size();
	if (__num_elements_hint > __old_n) {
		const size_type __n = next_size(__num_elements_hint);
		if (__n > __old_n) {
			std::vector<node*> __tmp(__n, (node*)(0));
			__STL_TRY{
			  for (size_type __bucket = 0; __bucket < __old_n; ++__bucket) {
				node* __first = buckets[__bucket];
				while (__first) {
				  size_type __new_bucket = bkt_num(__first->val, __n);
				  buckets[__bucket] = __first->next;
				  __first->next = __tmp[__new_bucket];
				  __tmp[__new_bucket] = __first;
				  __first = buckets[__bucket];
				}
			  }
			  buckets.swap(__tmp);
			}
			
#         ifdef __STL_USE_EXCEPTIONS
				catch (...) {
				for (size_type __bucket = 0; __bucket < __tmp.size(); ++__bucket) {
					while (__tmp[__bucket]) {
						node* __next = __tmp[__bucket]->next;
						deletenode(__tmp[__bucket]);
						__tmp[__bucket] = __next;
					}
				}
				throw;
			}
#         endif /* __STL_USE_EXCEPTIONS */
			
		}
	}
}

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
::insert_unique_noresize(const value_type& __obj)
{
	const size_type __n = bkt_num(__obj);
	node* __first = buckets[__n];

	for (node* __cur = __first; __cur; __cur = __cur->next)
		if (equals(get_key(__cur->val), get_key(__obj)))
			return pair<iterator, bool>(iterator(__cur, this), false);

	node* __tmp = new_node(__obj);
	__tmp->next = __first;
	buckets[__n] = __tmp;
	++num_elements;
	return pair<iterator, bool>(iterator(__tmp, this), true);
}


template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear() {
	for (size_type __i = 0; __i < buckets.size(); ++__i) {
		node* __cur = buckets[__i];
		while (__cur != 0) {
			node* __next = __cur->next;
			deletenode(__cur);
			__cur = __next;
		}
		buckets[__i] = 0;
	}
	num_elements = 0;
}

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht)
{
	buckets.clear();
	buckets.reserve(ht.buckets.size());
	buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
	__STL_TRY{
	  for (size_type __i = 0; __i < ht.buckets.size(); ++__i) {
		const node* __cur = ht.buckets[__i];
		if (__cur) {
		  node* __copy = newnode(__cur->val);
		  buckets[__i] = __copy;

		  for (node* __next = __cur->next;
			   __next;
			   __cur = __next, __next = __cur->next) {
			__copy->next = newnode(__next->val);
			__copy = __copy->next;
		  }
		}
	  }
	  num_elements = ht.num_elements;
	}
	__STL_UNWIND(clear());
}