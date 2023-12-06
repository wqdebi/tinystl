#pragma once
#include"stl_iterator.h"
#include<algorithm>
#include"type_traits.h"
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
	return *first;
}

//交换
template<class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap1(ForwardIterator1 a, ForwardIterator2 b){
	__iter_swap(a, b, value_type(a));
}

//字典排序方式
template<class InputIterator1, class InputIterator2>
bool lexicofraphical_compare(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2)
			return true;
		if (*first2 < *first1)
			return false;
	}
	return first1 == last1 && first2 != last2;
}

template<class InputIterator1, class InputIterator2, class Compare>
bool lexicofraphical_compare(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, Compare comp) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if(comp(*first1, *first2))
			return true;
		if(comp(*first2, *first1))
			return false;
	}
	return first1 == last1 && first2 != last2;
}

template<class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}

template<class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	return comp(a, b) ? b : a;
}

template<class T>
inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}

template<class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	return comp(b, a) ? b : a;
}


inline bool lexicofraphical_compare(const unsigned char* first1,
	const unsigned char* last1, const unsigned char* first2,
	const unsigned char* last2) {
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first1;
	const int result = memcmp(first1, first2, min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}

template<class InputIterator1, class InputIterator2>
std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
	InputIterator1 last1, InputIterator2 first2) {
	while (first1 != last1 && *first1 == *first2) {
		++first1;
		++first2;
	}
	return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class InputIterator1, class InputIterator2,
class BinaryPredicate>
std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
	InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first1, *first2)) {
		++first1;
		++first2;
	}
	return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

//接下来是拷贝，采用了萃取和特化的方法，提升速度
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result,
	__true_type) {
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result,
	__false_type) {
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}

template<class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
	OutputIterator result, input_iterator_tag) {
	for (; first != last; ++first, ++result) {
		*result = *first;
	}
	return result;
}

template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
	OutputIterator result, random_access_iterator_tag) {
	return __copy_d(first, last, result, distance_type(first));
}

template<class RandomAccessIterator, class OutputInterator, class Distance>
inline OutputInterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
	OutputInterator result, Distance*) {
	for (Distance n = last - first; n > 0; --n, ++result, ++first)
		*result = *first;
	return result;
}

template<class InputIterator, class OutputIterator>
struct __copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last,
		OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}
};

template<class T>
struct __copy_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template<class T>
struct __copy_dispatch<const T*, T*> {
	T* operator()(const T* first, const T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
	OutputIterator result) {
	return __copy_dispatch(first, last, result);
}

//为char进行的特化
inline char* copy(const char* first, const char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

//为w_char*的特化
__inline wchar_t* copy(const wchar_t* first, const wchar_t* last,
	wchar_t* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

//构建并集
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first < *first2) {
			*result = *first1;
			++first1;
		}
		else if (*first1 > *first2) {
			*result = *first2;
			++first2;
		}
		else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

//构建交集
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2)
			++first1;
		else if (*first2 < *first1)
			++first2;
		else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}

//构建差集
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1)
			++first2;
		else {
			++first1;
			++first2;
		}
	}
	return copy(first1, last1, result);
}

//构造对称差集，即对于集合S1，S2得到(S1 - S2) U (S2 - S1)
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
	InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		}
		else if (*first2 < *first1) {
			*result = *first2;
			++result;
			++first2;
		}
		else {
			++first1;
			++first2;
		}
	}
	return copy(first2, last2, copy(first1, last1, result));
}
