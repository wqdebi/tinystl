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
	//ֱ��ʹ��malloc�����ڴ棬���ʧ�������oom_mallocһֱ���롣
	static void* allocate(size_t n)
	{
		void* result = malloc(n);
		if (0 == result)
			result = oom_malloc(n);
		return result;
	}
	//ֱ��ʹ��free�ͷ��ڴ�
	static void deallocate(void* p, size_t)
	{
		free(p);
	}
	//ʹ��realloc�����ڴ棬���ʧ�������oom_reallocһֱ����
	static void* reallocate(void* p, size_t old_sz, size_t new_sz)
	{
		void* result = realloc(p, new_sz);
		if (0 == result)
			result = oom_realloc(p, new_sz);
		return result;
	}

	//��������set_malloc_handler  ������void (*)() �������ͣ� void(*)()
	//����__malloc_alloc_oom_handler����ʹ��oom_malloc��oom_realloc֮ǰ�����ȵ��ô˺���
	static void (*set_malloc_handler(void (*f)()))()
	{
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return (old);
	}
};
//��ʼ��
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

//��������� __USE_MALLOC����alloc����Ϊһ��������������Ϊ����
#ifdef __USE_MALLOC
typedef __malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0>alloc
#endif // __USE_MALLOC


//��һ���װ����vector<int, Alloc = alloc>��ʱ���ʹ��alloc����һ��simple_alloc����һ���װ
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
