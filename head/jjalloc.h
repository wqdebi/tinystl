#pragma once
//此内存分配类不建议使用，首先效率较低，且功能简单，接口不齐全，没有分级分配器

#include<new>
#include<cstddef> // 包含ptrdiff_t, size_t
#include<cstdlib>//包含exit()
#include<climits>
#include<iostream>
namespace JJ {
	void no_memory()
	{
		std::cout << "out of memory" << std::endl;
		std::exit(1);
	}

	template<typename T>
	inline T* _allocate(ptrdiff_t size, T* t) //内存分配函数
	{
		std::set_new_handler(0);//设置内存分配失败时运行的函数
		//std::set_new_handler(no_memory); //使用no_memory作为内存分配失败运行函数，如使用此行代码，注释掉下面的失败运行代码。
		T* output = (T*)(::operator new((size_t)(size * sizeof(T))));
		if (output == nullptr)
		{
			std::cerr << "out of memory" << std::endl;
			exit(1);
		}
		return output;
	}

	template<typename T>
	inline void _deallocate(T* buffer)
	{
		::operator delete(buffer);
	}

	//在特定的地址p上构造一个T1类型的变量
	template<typename T1, typename T2>
	inline void _construct(T1* p, const T2& value)
	{
		new(p) T1(value);
	}

	template<typename T>
	inline void _destroy(T* ptr)
	{
		ptr -> ~T();
	}

	//allocator类，用于内存分配。
	template<typename T>
	class allocator
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template<typename U>
		struct rebind
		{
			typedef allocator<int> other;
		};

		//构造函数，此函数调用_allocate分配内存
		pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate((difference_type)n, (pointer)0);
		}

		//调用_deallocate  参数n并不是没用，他提供相同接口，否则vector无法使用该分配器
		void deallocate(pointer p,size_type n)
		{
			_deallocate(p);
		}

		void construct(pointer p, const T* value)
		{
			_construct(p, value);
		}

		void destroy(pointer p)
		{
			_destroy(p);
		}

		pointer address(reference x)
		{
			return (pointer)&x;
		}

		const_pointer const_address(const_reference x)
		{
			return (const_pointer)&x;
		}

		size_type max_size() const
		{
			return size_type(INT_MAX / sizeof(T));
		}
	};
}
