#pragma once
#pragma once
#include<algorithm>
#include"std_alloc.h"
#include"rbtree.h"
#include"stl_function.h"
/////后续删除！！！！！！
//template<class Arg, class Result>
//struct unary_function {
//	typedef Arg argument_type;
//	typedef Result result_type;
//};
//template<class T>
//struct identity : public unary_function<T, T> {
//	const T& operator()(const T& x) { return x; }
//};
//template<class Pair>
//struct select1st : public unary_function<Pair, typename Pair::first_type> {
//	const typename Pair::first_type& operator()(const Pair& x) {
//		return x.first;
//	}
//};

//template <class Arg1, class Arg2, class Result>
//struct binary_function {
//	typedef Arg1 first_argument_type;
//	typedef Arg2 second_argument_type;
//	typedef Result result_type;
//};

template<class Key, class T, class Compare = std::less<Key>, class Alloc = alloc>
class multimap {
public:
	using key_type = Key;
	using data_type = T;
	using mapped_type = T;
	using value_type = std::pair<const Key, T>;
	using key_compare = Compare;

	class value_compare :public binary_function<value_type, value_type, bool> {
		friend class multimap<Key, T, Compare, Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c) :comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y) {
			return comp(x.first, y.first);
		}
	};
private:
	using rep_type = rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc>;
	rep_type t;

public:
	typedef typename rep_type::pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;

	multimap() :t(Compare()) {}
	explicit multimap(const Compare& comp) :t(comp) {}

	template<class InputIterator>
	multimap(InputIterator first, InputIterator last) : t(Compare()) {
		t.insert_equal(first, last);
	}
	template<class InputIterator>
	multimap(InputIterator first, InputIterator last, const Compare& comp) : t(comp) {
		t.insert_equal(first, last);
	}
	multimap(const multimap<Key, T, Compare, Alloc>& x) :t(x.t) {}
	multimap<Key, T, Compare, Alloc>& operator=(const multimap<Key, T, Compare, Alloc>& x) {
		x = x.t;
		return *this;
	}
	iterator begin() { return t.begin(); }
	iterator end() { return t.end(); }
	bool empty() { return t.empty(); }
	size_type size() { return t.size(); }
	void swap(multimap<Key, Compare, Alloc>& x) {
		t.swap(x.t);
	}

	typedef std::pair<iterator, bool> piar_iterator_bool;
	std::pair<iterator, bool>insert(const value_type& x) {
		return t.insert_equal(x);
	}
	iterator insert(iterator position, const value_type& x) {
		return t.insert_equal(position, x);
	}
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_equal(first, last);
	}

	void erase(iterator position)
	{
		t.erase(position);
	}
	size_type erase(const key_type& x) {
		return t.erase(x);
	}
	void erase(iterator first, iterator last) {
		t.erase(first, last);
	}
	void clear() {
		t.clear();
	}
	iterator find(const key_type& x) {
		return t.find(x);
	}

	size_type count(const key_type& x) {
		return t.count(x);
	}

	iterator lower_bound(const key_type& x) {
		return t.lower_bound(x);
	}

	iterator upper_bound(const key_type& x) {
		return t.upper_bound(x);
	}
	std::pair<iterator, iterator>equal_range(const key_type& x) {
		return t.equal_range(x);
	}
	friend bool operator== <>(const multimap&, const multimap&);
	friend bool operator< <>(const multimap&, const multimap&);
};
template<class Key, class T, class Compare, class Alloc>
inline bool operator==(const multimap<Key, Compare, Alloc>& x,
	const multimap<Key, Compare, Alloc>& y)
{
	return x.t == y.t;
}

template<class Key, class T, class Compare, class Alloc>
inline bool operator<(const multimap<Key, Compare, Alloc>& x,
	const multimap<Key, Compare, Alloc>& y)
{
	return x.t < y.t;
}
