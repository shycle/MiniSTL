#pragma once
#ifndef __THROW_BAD_ALLOC
#  if defined(__STL_NO_BAD_ALLOC) || !defined(__STL_USE_EXCEPTIONS)
#    include <stdio.h>
#    include <stdlib.h>
#    define __THROW_BAD_ALLOC fprintf(stderr, "out of memory\n"); exit(1)
#  else /* Standard conforming out-of-memory handling */
#    include <new>
#    define __THROW_BAD_ALLOC throw std::bad_alloc()
#  endif
#endif

template <int inst>
class __malloc_alloc_template {
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*, size_t);
	static void (*__malloc_alloc_oom_handler)();

public:
	static void* allocate(size_t n)
	{
		void* result = malloc(n);
		if (0 == result) result = oom_malloc(n);
		return result;
	}

	static void deallocate(void* p, size_t n)
	{
		free(p);
	}

	static void* reallocate(void* p, size_t, size_t new_sz)
	{
		void* result = realloc(p, new_sz);
		if (0 == result) result = oom_realloc(p, new_sz);
		return result;
	}

	static void (*set_malloc_handler(void (*f)()))()
	{
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};

template <int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template <int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n)
{
	void (*my_malloc_handler) ();
	void* result;

	for (;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = malloc(n);
		if (result) return (result);
	}
}

template <int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
{
	void (*my_malloc_handler) ();
	void* result;

	for (;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();
		result = realloc(p, n);
		if (result) return (result);
	}
}

typedef __malloc_alloc_template<0> malloc_alloc;

template<class _Tp, class _Alloc>
class  simple_alloc {

public:
	static _Tp* allocate(size_t __n)
	{
		return 0 == __n ? 0 : (_Tp*)_Alloc::allocate(__n * sizeof(_Tp));
	}
	static _Tp* allocate(void)
	{
		return (_Tp*)_Alloc::allocate(sizeof(_Tp));
	}
	static void deallocate(_Tp* __p, size_t __n)
	{
		if (0 != __n) _Alloc::deallocate(__p, __n * sizeof(_Tp));
	}
	static void deallocate(_Tp* __p)
	{
		_Alloc::deallocate(__p, sizeof(_Tp));
	}
};

#define __USE_MALLOC

# ifdef __USE_MALLOC

typedef malloc_alloc alloc;  // 令 alloc 为第一级配置器
typedef malloc_alloc single_client_alloc;

# else
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;  // 令 alloc 为第二级配置器
typedef __default_alloc_template<false, 0> single_client_alloc;
# endif