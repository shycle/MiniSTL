#pragma once
#include "rb_tree.h"
#include "s_function.h"
#include <iostream>
template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class map {
public:
	typedef Key key_type; //键值型别
	typedef T data_type; //数值型别
	typedef T mapped_type;
	typedef pair<Key, T> value_type; //元素型别
	typedef Compare key_compare; //键值比较函数

	//定义一个functor 作用就是调用 元素比较函数
	class value_compare
		: public binary_function<value_type, value_type, bool> {
		friend class map<Key, T, Compare, Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c):comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y) const {
			return comp(x.first, y.first);
		}
	};

private:
	typedef rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc> rep_type;
	rep_type t;

public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	//	typedef typename rep_type::const_reverse_iterator reverse_iterator;
	//	typedef typename rep_type::const_reverse_iterator const_reverse_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	// allocation/deallocation
	// map底层用insert_unique

	map():t(Compare()) {}
	explicit map(const Compare& comp) :t(comp) {}

	template <class InputIterator>
	map(InputIterator first, InputIterator last)
		: t(Compare()) { t.insert_unique(first, last); }
	
	template <class InputIterator>
	map(InputIterator first, InputIterator last, const Compare& comp)
		:t(comp) { t.insert_unique(first, last);  }

	map(const map<Key, T, Compare, Alloc>& x) :t(x.t) { }
	map<Key, T, Compare, Alloc>& operator=(const map<Key, T, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}

	void test() {
		pair<std::string, int> value(std::string("david"), 5);
		map<std::string, int> simap;
		simap[std::string("jjhou")] = 1;
		simap[std::string("jerry")] = 2;
		simap[std::string("jason")] = 3;
		simap[std::string("jimmy")] = 4;

		simap.insert(value);

		map<std::string, int>::iterator simap_iter = simap.begin();
		for (; simap_iter != simap.end(); ++simap_iter)
			std::cout << simap_iter->first << ' ' << simap_iter->second << std::endl;
		int number = simap[std::string("jjhou")];
		std::cout << number << std::endl;

		auto  ite1 = simap.find(std::string("mchen"));
		if (ite1 == simap.end())
			std::cout << "mchen not found" << std::endl;
		ite1 = simap.find(std::string("jerry"));
		if (ite1 != simap.end())
			std::cout << "jerry found" << std::endl;
		ite1->second = 9;
		int number2 = simap[("jerry")];
		std::cout << number2 << std::endl;
		std::cout << "map size:" << simap.size() << std::endl;
		auto it = find("jimmy");
	//	simap.erase(it);
	//	std::cout << "map size:" << simap.size() << std::endl;

	}

	// accessors map转调用RB-tree的函数
	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }
	iterator begin() { return t.begin(); }
	iterator end() { return t.end(); }
	//	reverse_iterator rbegin() const { return t.rbegin(); }
	//	reverse_iterator rend() const { return t.rend(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }
	void swap(map<Key, T, Compare, Alloc>& x) { t.swap(x.t); }
	T& operator[](const key_type& k) {
		return (*((insert(value_type(k, T()))).first)).second;
#if 0
		iterator __i = lower_bound(__k);
		// __i->first is greater than or equivalent to __k.
		if (__i == end() || key_comp()(__k, (*__i).first))
			__i = insert(__i, value_type(__k, T()));
		return __i->second;
#endif
	}

	//insert/erase
	pair<iterator, bool> insert(const value_type& x) {
		return t.insert_unique(x);
	}

	iterator insert(iterator pos, const value_type& x) {
		return t.insert_unique(pos, x);
	}
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}

	void erase(iterator position) {
		t.erase(position);
	}
	size_type erase(const key_type& x) {
		return t.erase_unique(x);
		//	return t.erase(x);
	}
	void erase(iterator first, iterator last) {
		t.erase(first, last);
	}

	void clear() { t.clear(); }

	//set operarions
	iterator find(const key_type& x) 
	{ 
	//	return begin();
		return t.find(x); 
	}
	//const_iterator find(const key_type& x) const { return t.find(x); }
	size_type count(const key_type& x) /*const*/ { return t.count(x); }
	iterator lower_bound(const key_type& x) const {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x) const {
		return t.upper_bound(x);
	}
	pair<iterator, iterator> equal_range(const key_type& x) const {
		return t.equal_range(x);
	}
//	pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
//		return t.equal_rangec(x);	}

	friend bool operator== __STL_NULL_TMPL_ARGS(const map&, const map&);
	friend bool operator< __STL_NULL_TMPL_ARGS(const map&, const map&);
};

template<class Key, class T, class Compare, class Alloc>
inline
bool operator==(const map<Key, T, Compare, Alloc>& x, const  map<Key, T, Compare, Alloc>& y) {
	return x.t == y.t;
}

template<class Key, class T, class Compare, class Alloc>
inline
bool operator<(const map<Key, T, Compare, Alloc>& x, const map<Key, T, Compare, Alloc>& y) {
	return x.t < y.t;
}