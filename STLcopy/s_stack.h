#pragma once
//#include <deque>
#include "s_deque.h"

//#define __STL_NULL_TMPL_ARGS

namespace sgistl {

	template<class T, class Sequence = sgistl::deque<T>>
	class stack {
			friend bool operator== __STL_NULL_TMPL_ARGS(const stack&, const stack&);
			friend bool operator< __STL_NULL_TMPL_ARGS(const stack&, const stack&);
	public:
		using value_type = typename Sequence::value_type;
		using size_type = typename Sequence::size_type;
		using reference = typename Sequence::reference;
		using const_reference = typename Sequence::const_reference;



		/*
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
		*/

	public:
		stack() = default;

		explicit stack(size_type n)	:c(n) { }

		stack(size_type n, const value_type& value) :c(n, value) { }

	protected:
		Sequence c;
	public:
		bool empty() const { return c.empty(); }
		size_type size() { return c.size(); }
		reference top() { return c.back(); }
		const_reference top() const { return c.back(); }

		void push(const value_type& x) { c.push_back(x); }
		void pop() { c.pop_back(); }

	};

	template<class T, class Sequence>
	bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
		return x.c == y.c;
	}

	template<class T, class Sequence>
	bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y) {
		return x.c < y.c;
	}
}
