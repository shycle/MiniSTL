#pragma once
#include "s_vector.h"
#include "s_heap.h"
//#include <concurrent_priority_queue.h>

namespace sgistl 
{
	template<class T, class Sequence = vector<T>, class Compare = less<typename Sequence::value_type> >
	class priority_queue {
	public:
		using value_type = typename Sequence::value_type;
		using size_type = typename Sequence::size_type;
		using reference = typename Sequence::reference;
		using const_reference = typename Sequence::const_reference;
	protected:
		Sequence c;
		Compare comp;
	public:
		priority_queue():c() {}
		explicit priority_queue(const Compare& x):c(), comp(x) {}

		//以下用到heap函数的都是泛型算法
		//任意一个构造函数都立刻于底层容器内产生一个implicit representation heap
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare &x):
			c(first, last), comp(x) {
			make_heap(first, last, comp);
		}

		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last) :
			c(first, last){
			make_heap(first, last);// , comp);
		}

		bool empty() { return c.empty(); }
		size_type size() { return c.size(); }
		reference top() { return c.front(); }
		const_reference top() const { return c.front(); }
		void push(const value_type& x) {
			__STL_TRY{
				c.push_back(x);
				push_heap(c.begin(), c.end());// , comp);
			}
			__STL_UNWIND(c.clear());
		}
		void pop() {
			__STL_TRY{
				pop_heap(c.begin(), c.end());// , comp);
				c.pop_back();
			}
			__STL_UNWIND(c.clear());
		}
	};

}