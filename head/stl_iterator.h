#pragma once
#include<cstddef>
//定义五种迭代器：只读、只写、读写、双向、随机
struct input_iterator_tag {};
struct output_iterator_tag{};
struct forward_iterator_tag :public input_iterator_tag {};
struct bidirectional_iterator_tag :public forward_iterator_tag {};
struct random_access_iterator_tag :public bidirectional_iterator_tag {};

//为了防止写代码遗漏，自己开发的迭代器都继承下面这个iterator
template<class Category, class T, class Distance = ptrdiff_t,
class Pointer = T*, class Reference = T&>
struct iterator {
	typedef Category   iterator_category;
	typedef T          value_type;
	typedef Distance   difference_type;
	typedef Pointer    pointer;
	typedef Reference  reference;
};

//萃取机
template<class Iterator>
struct iterator_traits {
	typedef typename Iterator::iterator_category   iterator_category;
	typedef typename Iterator::value_type          value_type;
	typedef typename Iterator::difference_type     difference_type;
	typedef typename Iterator::pointer             pointer;
	typedef typename Iterator::reference           reference;
};

//针对原生指针的特化版本
template<class T>
struct iterator_traits<T*> {
	typedef random_access_iterator_tag    iterator_category;
	typedef T                             value_type;
	typedef ptrdiff_t                     difference_type;
	typedef T*                            pointer;
	typedef T&                            reference;
};

template<class T>
struct iterator_traits<const T*> {
	typedef random_access_iterator_tag    iterator_category;
	typedef T                             value_type;
	typedef ptrdiff_t                     difference_type;
	typedef const T*                      pointer;
	typedef const T&                      reference;
};

//萃取出迭代器类型
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&)
{
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

//萃取出指针
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

//萃取出值的类型
template<class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

//以下是整组的distance函数

//迭代器最低是只读的情况下
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last,
	input_iterator_tag) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while (first != last) {
		++first;
		++n;
	}
	return n;
}
//迭代器是随机的情况下
template<class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
	random_access_iterator_tag){
	return last - first;
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
	typedef typename
		iterator_traits<InputIterator>::iterator_category category;
	return __distance(first, last, category());
}

//以下是整组的advance函数
template<class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n,
	input_iterator_tag) {
	while(n--)
	{
		++i;
	}
}

template<class BidirectionalIterator, class Distance>
inline void __advance(BidirectionalIterator& i, Distance n,
	bidirectional_iterator_tag) {
	if (n >= 0)
	{
		while(n--)++i;
	}
	else
	{
		while(n++)--i;
	}
}

template<class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& i, Distance n,
	random_access_iterator_tag) {
	i += n;
}

template<class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
	__advance(i, n, iterator_category(i));
}
