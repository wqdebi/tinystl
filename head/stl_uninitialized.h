#pragma once
#include"std_construct.h"
#include<string.h>
#include<algorithm>
//先定义一个__true_type防止报错
struct __true_type {};//以后将注释
struct __flase_type {};//以后将注释

template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value)
{
	for (; n > 0; --n, ++first)
	{
		*first = value;
	}
	return first;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
	Size n, const T& x, __true_type) {
	return fill_n(first, n, x);
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,
	Size n, const T& x, __false_type) {
	ForwardIterator cut = first;
	for (; n > 0; --n, ++cur)
		construct(&*cur, x);
	return cur;
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n,
	const T& x, T1*) {
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

//从first构造n个T
template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
	return __uninitialized_fill_n(first, n, x, value_type(first));
}

template<class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
	while (first != last)
	{
		*result = *first;
		++result;
		++first;
	}
	return result;
}


template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
	InputIterator last, ForwardIterator result, __true_type) {
	return copy(first, last, result);
}
template<class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,
	InputIterator last, ForwardIterator result, __false_type) {
	ForwardIterator cur = result;
	for (; first != last; ++first, ++cur) {
		construct(&*cur, *first);
	}
	return cur;
}


template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator first,
	InputIterator last, ForwardIterator result, T*) {
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

//在内存块上构造对象
template<typename InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy(InputIterator first, InputIterator last,
	ForwardIterator result) {
	return __uninitialized_copy(first, last, result, value_type(result));
}

//针对char *的特化版本
inline char* uninitialized_copy(const char* first, const char* last, char* result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

//针对wchar_t*的特化版本
inline wchar_t* uninitialized_copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
{
	memmove(result, first, sizeof(wchar_t) * (last - first));
	return result + (last - first);
}

template<class ForwardIterator, class T>
inline void __uninitialized_fill__aux(ForwardIterator first,
	ForwardIterator last, const T& x, __true_type){
	std::fill(first, last, x);
}
template<class ForwardIterator, class T>
inline void __uninitialized_fill__aux(ForwardIterator first,
	ForwardIterator last, const T& x, __false_type) {
	ForwardIterator cur = first;
	for (; cur != last; ++cur) {
		construct(&*cur, x);
	}
}

template<class ForwardIterator, class T, class T1>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last,
	const T& x, T1*) {
	typedef typename __type_traits<T1>::is_POD_tyoe is_POD;
	__uninitialized_fill__aux(first, last, x, is_POD());
}

//在fist到last的开区间内构造X
template<class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last,
	const T& x) {
	__uninitialized_fill(first, last, x, value_type(first));
}
