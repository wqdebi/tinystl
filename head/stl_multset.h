#pragma once
#include<algorithm>
#include"std_alloc.h"
#include"rbtree.h"
template <class T>
struct identity
{
	identity() {}
	const T& operator()(const T& val) {
		return val;
	}
};
template<class Key, class Compare = std::less<Key>, class Alloc = alloc>
class multset {
public:
	using key_value = Key;
	using value_type = Key;
	using key_compare = Compare;
	using value_compare = Compare;
private:
	using rep_type = rb_tree<key_value, value_type, identity<value_type>, key_compare, Alloc>;
	rep_type t;
public:
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::const_iterator iterator;
	typedef typename rep_type::const_iterator const_iterator;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
	template<class InputIterator>
	multset(InputIterator first, InputIterator last) :t(Compare()) {
		t.insert_equal(first, last);
	}
	template<class InputIterator>
	multset(InputIterator first, InputIterator last, const Compare& comp) : t(comp) {
		t.insert_equal(first, last);
	}

	iterator begin() { return t.begin(); }
	iterator end() { return t.end(); }
	bool empty() { return t.empty(); }
	size_type size() { return t.size(); }
	void swap(set<Key, Compare, Alloc>& x) {
		t.swap(x.t);
	}

	iterator insert(const value_type& x) {
		return t.insert_equal(x);
	}
	iterator insert(iterator position, const value_type& x) {
		typedef typename rep_type::iterator rep_iterator;
		return t.insert_equal((rep_iterator&)position, x);
	}
	template<class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_equal(first, last);
	}
	void erase(iterator position)
	{
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)position);
	}
	size_type erase(const key_value& x) {
		return t.erase(x);
	}
	void erase(iterator first, iterator last) {
		typedef typename rep_type::iterator rep_iterator;
		t.erase((rep_iterator&)first, (rep_iterator&)last);
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

	iterator upper_bound(const key_value& x) {
		return t.upper_bound(x);
	}
	std::pair<iterator, iterator>equal_range(const key_type& x) {
		return t.equal_range(x);
	}
	friend bool operator== <>(const multset&, const multset&);
	friend bool operator< <>(const multset&, const multset&);
};
template<class Key, class Compare, class Alloc>
inline bool operator==(const multset<Key, Compare, Alloc>& x,
	const multset<Key, Compare, Alloc>& y)
{
	return x.t == y.t;
}

template<class Key, class Compare, class Alloc>
inline bool operator<(const multset<Key, Compare, Alloc>& x,
	const multset<Key, Compare, Alloc>& y)
{
	return x.t < y.t;
}