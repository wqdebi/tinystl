#pragma once
//���ڴ�����಻����ʹ�ã�����Ч�ʽϵͣ��ҹ��ܼ򵥣��ӿڲ���ȫ��û�зּ�������

#include<new>
#include<cstddef> // ����ptrdiff_t, size_t
#include<cstdlib>//����exit()
#include<climits>
#include<iostream>
namespace JJ {
	void no_memory()
	{
		std::cout << "out of memory" << std::endl;
		std::exit(1);
	}

	template<typename T>
	inline T* _allocate(ptrdiff_t size, T* t) //�ڴ���亯��
	{
		std::set_new_handler(0);//�����ڴ����ʧ��ʱ���еĺ���
		//std::set_new_handler(no_memory); //ʹ��no_memory��Ϊ�ڴ����ʧ�����к�������ʹ�ô��д��룬ע�͵������ʧ�����д��롣
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

	//���ض��ĵ�ַp�Ϲ���һ��T1���͵ı���
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

	//allocator�࣬�����ڴ���䡣
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

		//���캯�����˺�������_allocate�����ڴ�
		pointer allocate(size_type n, const void* hint = 0)
		{
			return _allocate((difference_type)n, (pointer)0);
		}

		//����_deallocate  ����n������û�ã����ṩ��ͬ�ӿڣ�����vector�޷�ʹ�ø÷�����
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
