#pragma once
#include "s_deque.h"
namespace sgistl {

	template <class T, class Sequence = deque<T> >
	class queue {
		friend bool operator== __STL_NULL_TMPL_ARGS(const queue& x, const queue& y);
		friend bool operator< __STL_NULL_TMPL_ARGS(const queue& x, const queue& y);
	public:
		using value_type = typename Sequence::value_type;
		using size_type = typename Sequence::size_type;
		using reference = typename Sequence::reference;
		using const_reference = typename Sequence::const_reference;
	protected:
		Sequence c;
	public:
		bool empty() const { return c.empty(); }
		size_type size() { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }
		reference back() {	return c.back(); }
		const_reference back() const { return c.back(); }
		void push(const value_type& x) { return c.push_back(x); }
		void pop() { return c.pop_front(); }
	};

	template<class T, class Sequence>
	bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return x.c = y.c;
	}

	template<class T, class Sequence>
	bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
		return x.c < y.c;
	}

}