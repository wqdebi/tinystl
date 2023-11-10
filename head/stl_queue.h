#pragma once
#include"stl_deque.h"
//#define __STL_EXPLICIT_FUNCTION_TMPL_ARGS
# ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS 
# define __STL_NULL_TMPL_ARGS <> 
# else 
# define __STL_NULL_TMPL_ARGS 
# endif

template<class T, class Sequence = deque<T>>
class deque {
	friend bool operator==__STL_NULL_TMPL_ARGS(const deque& x, const deque& y) { return x.c == y.c; }
	friend bool operator<__STL_NULL_TMPL_ARGS(const deque& x, const deque& y) { return x.c < y.c; }
protected:
	Sequence c;
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
	bool empty() { return c.empty(); }
	size_t size() { return c.size(); }
	reference front() { return c.front(); }
	//const_reference top() const { return c.back(); }
	void push(const value_type& x) { c.push_back(x); }
	void pop() { c.pop_front(); }

};