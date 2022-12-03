#pragma once
#include "s_alloc.h"
#include "s_uninitialized.h"

namespace sgistl
{
	template <class _Tp, class Alloc = alloc>
	class vector
	{
	public:
        typedef _Tp value_type;
        typedef value_type* pointer;
        typedef value_type* iterator;
        typedef value_type& reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef const value_type* const_iterator;
        typedef const value_type& const_reference;



    protected:
        typedef simple_alloc<value_type, Alloc> data_allocator;
        iterator    start;
        iterator    finish;
        iterator    end_of_storage;

        void  insert_aux(iterator pos, const _Tp& x);

        void fill_initialize(size_type n, const _Tp& value) {
            start = allocate_and_fill(n, value);
            finish = start + n;
            end_of_storage = finish;
        }

    public:
        vector() :start(0), finish(0), end_of_storage(0) { fill_initialize(0, value_type()); }
        vector(size_type n, const _Tp& value) { fill_initialize(n, value); }
        vector(int n, const _Tp& value) { fill_initialize(n, value); }
        vector(long n, const _Tp& value) { fill_initialize(n, value); }
        explicit vector(size_type n) { fill_initialize(n, _Tp()); }
        vector(const _Tp* __first, const _Tp* __last)
           /// : _Base(__last - __first, )
        {
            start = data_allocator::allocate(__last - __first);
            finish = start;
            end_of_storage = start + (__last - __first);

            finish = uninitialized_copy(__first, __last, start);
        }

        ~vector()
        {
            destroy(start, finish);
         //   deallocate();
        }

    public:
        iterator begin() { return start; }
        iterator end() { return finish; }
        size_type size()  { return size_type(end() - begin()); }
        size_type capacity() {
            return size_type(end_of_storage - begin()); }
        bool empty() { return begin() == end(); }
        reference operator[](size_type n) { return *(begin() + n); }
        
        reference front() { return *begin(); }
     //   const_reference front() const { return *begin(); }
        reference back() { return *(end() - 1); }
        void insert(iterator position, size_type n, const _Tp& x);
        void push_back(const _Tp& x) {
            if (finish != end_of_storage) {
                construct(finish, x);
                ++finish;
            }
            else
                insert_aux(end(), x);
        }

        void pop_back()
        {
            --finish;
            destroy(finish);
        }

        iterator erase(iterator position) {
            if (position + 1 != end())
                copy(position + 1, finish, position);
            --finish;
            destroy(finish);
            return position;
        }

        iterator erase(iterator first, iterator last)
        {
            iterator i = copy(last, finish, first);
            destroy(i, finish);
            finish = finish - (last - first);
            return first;
        }

        void resize(size_type new_size, const _Tp& x)
        {
            if (new_size < size())
                erase(begin() + new_size, end());
            else
                insert(end(), new_size - size(), x);
        }

        void resize(size_type new_size)
        {
            resize(new_size, _Tp());
        }
        void clear() { erase(begin(), end()); }

        void reserve(size_type __n) {
            if (capacity() < __n) {
                const size_type __old_size = size();
                iterator __tmp = allocate_and_copy(__n, start, finish);
                destroy(start, finish);
                data_allocator::deallocate(start, end_of_storage - start);
                start = __tmp;
                finish = __tmp + __old_size;
                end_of_storage = start + __n;
            }
        }

        iterator allocate_and_copy(size_type __n, const_iterator __first,
            const_iterator __last)
        {
            iterator __result = data_allocator::allocate(__n);
            __STL_TRY{
              uninitialized_copy(__first, __last, __result);
              return __result;
            }
            __STL_UNWIND(data_allocator::deallocate(__result, __n));
        }

    protected:
        iterator allocate_and_fill(size_type n, const _Tp& x)
        {
            iterator result = data_allocator::allocate(n);
    //         uninitialized_fill_n(result, n, x);
             uninitialized_fill_m(result, n, x);
            return result;
        }
	};

}



template <class _Tp, class Alloc>
void sgistl::vector<_Tp, Alloc>::insert_aux(iterator position, const _Tp& x)
{
    if (finish != end_of_storage) {
        construct(finish, *(finish - 1));
        ++finish;
        _Tp x_copy = x;
        copy_backward(position, finish - 2, finish - 1);
        *position = x_copy;
    }
    else {
        const size_type __old_size = size();
        const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
        iterator new_start =  data_allocator::allocate(__len);
        iterator new_finish = new_start;
        __STL_TRY
        {
          new_finish = uninitialized_copy(start, position, new_start);
          construct(new_finish, x);
          ++new_finish;
          new_finish = uninitialized_copy(position, finish, new_finish);
        }
            __STL_UNWIND((destroy(new_start, new_finish),
                data_allocator::deallocate(new_start, __len)));
        destroy(begin(), end());
        data_allocator::deallocate(start, end_of_storage - start);
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + __len;
    }
}

#if 1
template <class _Tp, class Alloc>
void sgistl::vector<_Tp, Alloc>::insert(iterator position, size_type n, const _Tp& x)
{
    if (n != 0) {
        // 备用空间大于等于 “新增元素个数”
        if (size_type(end_of_storage - finish) >= n) {
            _Tp x_copy = x;
            const size_type __elems_after = finish - position;
            iterator __old_finish = finish;
            if (__elems_after > n) {
                // "插入点之后的现有元素个数" 大于 "新增元素个数"
                uninitialized_copy(finish - n, finish, finish);
                finish += n; // 将 vector 尾端标记后移
                copy_backward(position, __old_finish - n, __old_finish); // 向后复制
                fill(position, position + n, x_copy); // 填充
            }
            else {
                // 当备用空间小于“新增元素个数”
                uninitialized_fill_m(finish, n - __elems_after, x_copy); // 填充 n - __elems_after 个x_copy值
                finish += n - __elems_after;
                uninitialized_copy(position, __old_finish, finish);  // 将从 position 到 __old_finish 的值 复制到 从  finish 开始
                finish += __elems_after;
                fill(position, __old_finish, x_copy); // 填充
            }
        }
        else {
            // 备用空间 小于 新增元素个数
            size_type __old_size = size();
            size_type __len = __old_size + max(__old_size, n);
            iterator new_start = data_allocator::allocate(__len);
            iterator new_finish = new_start;
            __STL_TRY{
              new_finish = uninitialized_copy(start, position, new_start);  // 1
              new_finish = uninitialized_fill_m(new_finish, n, x); // 2
              new_finish
                = uninitialized_copy(position, finish, new_finish); // 3
            }
                __STL_UNWIND((destroy(new_start, new_finish),
                    data_allocator::deallocate(new_start, __len)));
            destroy(start, finish);
            data_allocator::deallocate(start, end_of_storage - start);
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + __len;
        }
    }
}
#endif
