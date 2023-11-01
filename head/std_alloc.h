#pragma once
#define __USE_MALLOC
#include<new>
template<int inst>
class __malloc_alloc_template {
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*, size_t);
	static void (*__malloc_alloc_oom_handler)();
public:
	//直接使用malloc分配内存，如果失败则调用oom_malloc一直申请。
	static void* allocate(size_t n)
	{
		void* result = malloc(n);
		if (0 == result)
			result = oom_malloc(n);
		return result;
	}
	//直接使用free释放内存
	static void deallocate(void* p, size_t)
	{
		free(p);
	}
	//使用realloc扩容内存，如果失败则调用oom_realloc一直扩。
	static void* reallocate(void* p, size_t old_sz, size_t new_sz)
	{
		void* result = realloc(p, new_sz);
		if (0 == result)
			result = oom_realloc(p, new_sz);
		return result;
	}

	//函数名：set_malloc_handler  参数：void (*)() 返回类型： void(*)()
	//设置__malloc_alloc_oom_handler，在使用oom_malloc、oom_realloc之前首先先调用此函数
	static void (*set_malloc_handler(void (*f)()))()
	{
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};
//初始化
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t n)
{
	void (* my_malloc_handler)();
	void* result;
	for (;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler)
			throw std::bad_alloc;
		(*my_malloc_handler)();
		result = malloc(n);
		if (result)
			return result;
	}
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
{
	void (*my_malloc_handler)();
	void* result;
	for (;;)
	{
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (0 == my_malloc_handler)
			throw std::bad_alloc;
		(*my_malloc_handler)();
		result = realloc(p, n);
		if (result)
			return result;
	}
}

//如果定义了 __USE_MALLOC，则alloc定义为一级分配器，否则为二级
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0>alloc
#endif // __USE_MALLOC


//多一层包装，在vector<int, Alloc = alloc>的时候会使用alloc构建一个simple_alloc。多一层封装
template<class T, class Alloc>
class simple_alloc {
	static T* allocate(size_t n) 
	{
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void)
	{
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n)
	{
		if (0 == n)
			Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T* p)
	{
		Alloc::deallocate(p, sizeof(T));
	}
};
