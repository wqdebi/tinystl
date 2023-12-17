#pragma once
#include"stl_iterator.h"
#include<iostream>
//insert iterators
template<class Container>
class back_insert_iterator {
protected:
	Container* containor;
public:
	typedef output_iterator_tag iterator_category;
	typedef void				value_type;
	typedef void				difference_type;
	typedef void				pointer;
	typedef void				reference;
	explicit back_insert_iterator(Container& x) :containor(&x) {}
	back_insert_iterator<Container>& operator=(const typename Container::value_type& value)
	{
		containor->push_back(value);
		return *this;
	}
	back_insert_iterator<Container>& operator*() { return *this; }
	back_insert_iterator<Container>& operator++() { return *this; }
	back_insert_iterator<Container>& operator++(int) { return *this; }
};

template<class Container>
inline back_insert_iterator<Container> back_inserter(Container& x) {
	return back_insert_iterator<Container>(x);
}

template<class Container>
class front_insert_iterator {
protected:
	Container* containor;
public:
	typedef output_iterator_tag iterator_category;
	typedef void				value_type;
	typedef void				difference_type;
	typedef void				pointer;
	typedef void				reference;
	explicit front_insert_iterator(Container& x) :containor(&x) {}
	front_insert_iterator<Container>& operator=(const typename Container::value_type& value)
	{
		containor->push_back(value);
		return *this;
	}
	front_insert_iterator<Container>& operator*() { return *this; }
	front_insert_iterator<Container>& operator++() { return *this; }
	front_insert_iterator<Container>& operator++(int) { return *this; }
};

template<class Container>
inline front_insert_iterator<Container> front_inserter(Container& x) {
	return front_insert_iterator<Container>(x);
}

template<class Container>
class insert_iterator {
protected:
	Container* containor;
	typename Container::iterator iter;
public:
	typedef output_iterator_tag iterator_category;
	typedef void				value_type;
	typedef void				difference_type;
	typedef void				pointer;
	typedef void				reference;
	insert_iterator(Container& x, typename Container::iterator i) :containor(&x), iter(i) {}
	insert_iterator<Container>& operator=(const typename Container::value_type& value)
	{
		iter = containor->insert(iter, value);
		++iter;
		return *this;
	}
	insert_iterator<Container>& operator*() { return *this; }
	insert_iterator<Container>& operator++() { return *this; }
	insert_iterator<Container>& operator++(int) { return *this; }
};

template<class Container, class Iterator>
inline back_insert_iterator<Container> inserter(Container& x, Iterator i) {
	typedef typename Container::iterator iter;
	return insert_iterator<Container>(x, iter(i));
}


//·´Ïòµü´úÆ÷
template<class Iterator>
class reverse_iterator {
protected:
	Iterator current;
public:
	typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::reference reference;
	typedef Iterator iterator_type;
	typedef reverse_iterator<Iterator> self;
public:
	reverse_iterator() {}
	explicit reverse_iterator(iterator_type x) :current(x) {}
	reverse_iterator(const self& x) :current(x.current) {}
	iterator_type base()const { return current; }
	reference operator*()const {
		Iterator tmp = current;
		return *--tmp;
	}
	pointer operator->()const { return &(operator*()); }
	self& operator++() {
		--current;
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		--current;
		return tmp;
	}
	self& operator--() {
		++current;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		++current;
		return tmp;
	}
	self operator+(difference_type n)const {
		return self(current - n);
	}
	self& operator+=(distance_type n) {
		current -= n;
		return *this;
	}
	self operator-(difference_type n)const {
		return self(current + n);
	}
	self& operator-=(difference_type n)
	{
		current += n;
		return *this;
	}
};

template<class T, class Distance=ptrdiff_t>
class istream_iterator {
	friend bool operator==<>(const istream_iterator<T, Distance>& x,
		const istream_iterator<T, Distance>& y);
protected:
	istream* stream;
	T value;
	bool end_marker;
	void read() {
		end_marker = (*stream) ? true : false;
		if (end_marker)
			*stream >> value;
		end_marker = (*stream) ? true : false;
	}
public:
	typedef input_iterator_tag iterator_category;
	typedef T value_type;
	typedef Distance difference_type;
	typedef const T* pointer;
	typedef const T& reference;
	istream_iterator() :stream(&std::cin), end_marker(false) {}
	istream_iterator(istream& s) :stream(s) { read(); }
	reference operator*()const{return value};
	pointer operator->() const { return &(operator*()); }
	istream_iterator<T, Distance>& operator++() {
		read();
		return *this;
	}
	istream_iterator<T, Distance> operator++(int) {
		istream_iterator<T, Distance> tmp = *this;
		read();
		return tmp;
	}
};

template<class T>
class ostream_iterator {
protected:
	ostream* stream;
	const char* string;
public:
	typedef output_iterator_tag iterator_category;
	typedef void				value_type;
	typedef void				difference_type;
	typedef void				pointer;
	typedef void				reference;
	ostream_iterator(ostream& s) :stream(&s), stirng(0) {}
	ostream_iterator(ostream& s, const char* c) :stream(&s), string(c) {}
	ostream_iterator<T>% operator==(const T& value) {
		*stream << value;
		if (string)
			*stream << string;
		return *this;
	}
	ostream_iterator<T>% operator*() { return *this; }
	ostream_iterator<T>% operator++() { return *this; }
	ostream_iterator<T>% operator++(int) { return *this; }
};