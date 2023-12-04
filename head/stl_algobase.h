#pragma once

//判断相等
template<class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) 
		if (*first1 != *first2)
			return false;
	return true;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, BinaryPredicate op) {
	for (; first1 != last1; ++first1, ++first2)
		if (!op(*first1, *first2))
			return false;
	return true;
}

//填值
template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last,
	const T& value) {
	for (; first != last; ++first) {
		*first = value;
	}
}

//填n个值
template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
	for (; n > 0; --n, ++first) {
		*first = value;
	}
	return *first
}

//交换
template<class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b){
	__iter_swap(a, b, value_type(a));
}