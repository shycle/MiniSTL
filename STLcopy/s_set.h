#pragma once
#include "s_alloc.h"
#include "s_algobase.h"
#include "s_function.h"
#include "rb_tree.h"


template <class Key, class Compare __STL_DEPENDENT_DEFAULT_TMPL(less<Key>),
	class Alloc = alloc>
class set;


template<class Key, class Compare, class Alloc>
class set {
public:
	//
	typedef Key key_type;
	typedef Key value_type;
	//
	typedef Compare key_compare;
	typedef Compare value_compare;

private:
	/*
	template <class T>
	struct identity : public unarg_function<T, T> {
		const T& operator() (const T& x) const { return x; }
	};
	*/
	typedef rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc> rep_type;
	rep_type t; //采用红黑树来表现set

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

	//set不允许相同键值存在， 注意红黑树insert的使用。
	set() :t(Compare()) {}
	explicit set(const Compare& comp) :t(comp) {}

	template<class InputIterator>
	set(InputIterator first, InputIterator last) : t(Compare()) { 
		t.insert_unique(first, last); 
	}

	template<class InputIterator>
	set(InputIterator first, InputIterator last, const Compare& comp) : t(comp) { t.insert_unique(first, last); }

	set(const set<Key, Compare, Alloc>& x) :t(x.t) {}

	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc>& x) {
		t = x.t;
		return *this;
	}

	//以下所有的set操作行为 红黑树都以提供， 只需set传递调用即可
	//accessors
	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }
	iterator begin() { return t.begin(); }
	iterator end() { return t.end(); }
	//	reverse_iterator rbegin() const { return t.rbegin(); }
	//	reverse_iterator rend() const { return t.rend(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }
	void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }

	//insert/erase
	typedef pair<iterator, bool> pair_iterator_bool;

#if 1
	pair<iterator, bool> insert(const value_type& x) {
		pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
	//	return {};
		return pair<iterator, bool>(p.first, p.second);
	}
#endif 
	iterator insert(iterator position, const value_type& x) {
		typedef typename rep_type::iterator rep_iterator;
		return t.insert_unique((rep_iterator&)position, x);
	}

	template<class Inputerator>
	void insert(Inputerator first, Inputerator last) {
		t.insert_unique(first, last);
	}

	void erase(iterator position) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)position);
	}
	size_type erase(const key_type& x) {
		return t.erase_unique(x);
	//	return t.erase(x);
	}
	void erase(iterator first, iterator last) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)first, (rep_iterator&)last);
	}

	void clear() { t.clear(); }

	//set operarions
	iterator find(const key_type& x) { return t.find(x); }
	size_type count(const key_type& x) /*const*/ { return t.count(x); }
	iterator lower_bound(const key_type& x) const {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x) const {
		return t.upper_bound(x);
	}
	pair<iterator, iterator> equal_range (const key_type& x) const {
		return t.equal_range(x);
	}
	//
	friend bool operator== __STL_NULL_TMPL_ARGS(const set&, const set&);
	friend bool operator< __STL_NULL_TMPL_ARGS(const set&, const set&);
};

template<class Key, class Compare, class Alloc>
bool operator==(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t == y.t;
}

template<class Key, class Compare, class Alloc>
bool operator<(const set<Key, Compare, Alloc>& x, const set<Key, Compare, Alloc>& y) {
	return x.t < y.t;
}