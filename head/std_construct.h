#pragma once
#include<new.h>
#include"type_traits.h"
#include"stl_iterator.h"
//在指针p出构建一个T2类型的value对象
template<typename T1, typename T2>
inline void construct(T1* p, const T2& value)
{
	new(p)T1(value);
}

//调用单个对象的析构
template<typename T>
inline void destroy(T* pointer)
{
	pointer -> ~T();
}
template<class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first < last; ++first)
		destroy(&*first);
}

//什么也不做
template<class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

//特化版
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

template<typename ForwardIterator, typename T>
inline void __destory(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destory_aux(first, last, trivial_destructor());
}
//destroy第二个版本，接受两个迭代器，析构其中的对象
//利用了萃取机制
template<typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	__destory(first, last, value_type(first));
}



//struct __false_type {};//暂时先需要，在后面写迭代器的时候在具体写出
//struct __true_type {};//以后需要注释这两行代码，这两行代码使用萃取机制和函数重载，直接调用相应的函数。

