#pragma once
#include"std_alloc.h"
#include"stl_iterator.h"
#include"stl_uninitialized.h"
#include"type_traits.h"
#define __STL_USE_EXCEPTIONS 0
#ifdef __STL_USE_EXCEPTIONS
#define __STL_TRY   try
#define __STL_UNWIND(action)   catch(...) { action; throw; }
#else
#define __STL_TRY
#define __STL_UNWIND(action)
#endif


//全局函数n不为0则返回n，否则看sz，如果sz小于512返回512 / sz，否则返回1
inline size_t __deque_buf_size(size_t n, size_t sz) {
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

//deque迭代器
template<class T, class Ref, class  Ptr, size_t BufSiz>
struct __deque_iterator {
	typedef __deque_iterator<T, T&, T*, BufSiz>					iterator;
	typedef __deque_iterator<T, const T&, const T*, BufSiz>		const_iterator;
	typedef random_access_iterator_tag							iterator_category;
	typedef T													value_type;
	typedef Ptr													pointer;
	typedef Ref													reference;
	typedef size_t												size_type;
	typedef ptrdiff_t											difference_type;
	typedef T** map_pointer;
	typedef __deque_iterator<T, Ref, Ptr, BufSiz>				self;
	static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T)); }
	T* cur;
	T* first;
	T* last;
	map_pointer node;
	//跳过一个缓冲区
	__deque_iterator():first(nullptr), last(nullptr),cur(nullptr) {}
	void set_node(map_pointer new_node)
	{
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}
	reference operator*()const { return *cur; }
	pointer operator->()const { return &(operator*()); }
	difference_type operator-(const self& x)const {
		return difference_type(buffer_size()) * (node - x.node - 1) +
			(cur - first) + (x.last - x.cur);
	}
	self& operator++() {
		++cur;
		if (cur == last)
		{
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}
	self operator++(int) {
		self tmp = *this;
		++* this;
		return tmp;
	}
	self& operator--() {
		if (cur == first)
		{
			set_node(node - 1);
			cur = first;
		}
		--cur;
		return *this;
	}
	self operator--(int) {
		self tmp = *this;
		--* this;
		return tmp;
	}
	self& operator+=(difference_type n)
	{
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else {
			difference_type node_offset =
				offset > 0 ? offset / difference_type(buffer_size())
				: -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}
	self operator+(difference_type n)const {
		self tmp = *this;
		return tmp += n;
	}
	self& operator-=(difference_type n) { return *this += -n; }
	self operator-(difference_type n) {
		self tmp = *this;
		return tmp -= n;
	}
	reference operator[](difference_type n)const { return *(*this + n); }
	bool operator==(const self& x)const { return cur == x.cur; }
	bool operator!=(const self& x)const { return !(*this == x); }
	bool operator<(const self& x) const {
		return (node == x.node) ? (cur < x.cur) : (node < x.node);
	}
};

template<class T, class Alloc = alloc, size_t BufSiz = 0>
class deque {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
	typedef __deque_iterator<T, const T&, const T&, BufSiz>  const_iterator;
protected:
	typedef pointer* map_pointer;
	typedef simple_alloc<value_type, Alloc> data_allocator;
	typedef simple_alloc<pointer, Alloc> map_allocator;
	iterator start;
	iterator finish;
	map_pointer map;
	size_type map_size;
	static size_type initial_map_size() { return 8; }
	static size_type buffer_size() {//返回
		return __deque_buf_size(BufSiz, sizeof(value_type));
	}
	T* allocate_node()
	{
		return data_allocator::allocate(__deque_buf_size(0, sizeof(T)));
	}
	void deallocate_node(T* p)
	{
		data_allocator::deallocate(p, __deque_buf_size(0, sizeof(T)));
	}
public:
	iterator begin() { return start; }

	iterator end() { return finish; }

	reference operator[](size_type n)
	{
		return start[difference_type(n)];
	}
	reference front() { return *start; }

	reference back() {
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}
	size_type size() { 
		//if (empty())
		//	return 0;
		return finish - start; }

	size_type max_size() { return size_type(-1); }

	bool empty() { return finish == start; }

	deque(size_type n, const value_type& value)
		: start(), finish(), map(0), map_size(0)
	{
		fill_initialize(n, value);
	}
	deque()
		: start(), finish(), map(0), map_size(0){
		fill_initialize(0, 0);
	}
	void fill_initialize(size_type n,
		const value_type& value) {
		create_map_and_nodes(n);
		map_pointer cur;
		__STL_TRY{
		  for (cur = start.node; cur < finish.node; ++cur)
			uninitialized_fill(*cur, *cur + buffer_size(), value);
		  uninitialized_fill(finish.first, finish.cur, value);//尾部可能有多余空间
		}
		catch (...) { throw; }
	}

	void create_map_and_nodes(size_type num_elements) {
		size_type num_nodes = num_elements / buffer_size() + 1;
		/*
		  相当于20/8 + 1 = 3。刚好整除，则多分配一个节点
	  */

		map_size = std::max(initial_map_size(), num_nodes + 2);//map至少管理8个节点，最多是所需节点+2，前后各预留一个位置，扩充时候使用。
		map = map_allocator::allocate(map_size);//分配指针数组

		//先使用map指针数组中间的位置，方便前后扩充
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		__STL_TRY{
		  for (cur = nstart; cur <= nfinish; ++cur)
			*cur = allocate_node();//初始化指针数组成员
		}
		catch (...) { throw; }
		start.set_node(nstart);//存储开始node
		finish.set_node(nfinish);//存储结束node
		start.cur = start.first;//指向第一个元素
		finish.cur = finish.first + num_elements % buffer_size();//指向最后元素的后面一个元素形成[start , finish)左闭右开空间。
	}

	//尾部添加元素
	void push_back(const value_type& t) {
		if (finish.cur != finish.last - 1) {//尾部还有多余空间，一个以上的空间
			construct(finish.cur, t);//直接构造
			++finish.cur;//调整缓冲区状态finish的cur+1
		}
		else
			push_back_aux(t);//没有或者只剩下一个，添加node，然后构造
	}


	void push_back_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_back();//加入后是否大于map内存空间
		*(finish.node + 1) = allocate_node();//分配节点，node
		__STL_TRY{
		  construct(finish.cur, t_copy);//构造元素
		  finish.set_node(finish.node + 1);
		  finish.cur = finish.first;//设定finish
		}
		__STL_UNWIND(deallocate_node(*(finish.node + 1)));//释放返回
	}

	void push_front(const value_type& t) {
		if (start.cur != start.first) {
			construct(start.cur - 1, t);
			--start.cur;
		}
		else
			push_front_aux(t);
	}


	void push_front_aux(const value_type& t) {
		value_type t_copy = t;
		reserve_map_at_front();//是否需要重新分配map
		*(start.node - 1) = allocate_node();//分配node
		__STL_TRY{
		  start.set_node(start.node - 1);
		  start.cur = start.last - 1;
		  construct(start.cur, t_copy);
		}
		catch (...) { throw; }
	}
	void reserve_map_at_back(size_type nodes_to_add = 1) {
		if (nodes_to_add + 1 > map_size - (finish.node - map))
			reallocate_map(nodes_to_add, false);
	}
	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add > start.node - map)
			reallocate_map(nodes_to_add, true);
	}

	void reallocate_map(size_type nodes_to_add,
		bool add_at_front)
	{
		size_type old_num_nodes = finish.node - start.node + 1;
		size_type new_num_nodes = old_num_nodes + nodes_to_add;

		map_pointer new_nstart;
		if (map_size > 2 * new_num_nodes) {
			new_nstart = map + (map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			if (new_nstart < start.node)
				std::copy(start.node, finish.node + 1, new_nstart); ////!!!!!!!!!!
			else
				std::copy_backward(start.node, finish.node + 1,
					new_nstart + old_num_nodes); ////////!!!!
		}
		else {
			size_type new_map_size =
				map_size + std::max(map_size, nodes_to_add) + 2;
			// 配置一块空间准备给新map使用
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nstart = new_map + (new_map_size - new_num_nodes) / 2
				+ (add_at_front ? nodes_to_add : 0);
			std::copy(start.node, finish.node + 1, new_nstart); //!!!!!!!
			map_allocator::deallocate(map, map_size); //释放原map
			// 设定新map的起始地址和大小
			map = new_map;
			map_size = new_map_size;
		}
		// 重新设定迭代器start 和 finish

		start.set_node(new_nstart);
		finish.set_node(new_nstart + old_num_nodes - 1);
	}

	void pop_back() {
		if (finish.cur != finish.first) {
			--finish.cur;
			destroy(finish.cur);
		}
		else
			pop_back_aux();
	}

	void pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	void pop_front() {
		if (start.cur != start.last - 1) {
			destroy(start.cur);
			++start.cur;
		}
		else
			pop_front_aux();
	}

	void pop_front_aux()
	{
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}


	void clear()
	{
		for (map_pointer node = start.node + 1;
			node < finish.node;
			++node) {
			destroy(*node, *node + buffer_size());
			data_allocator::deallocate(*node, buffer_size());
		}

		if (start.node != finish.node) {
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			data_allocator::deallocate(finish.first, buffer_size());
		}
		else
			destroy(start.cur, finish.cur);

		finish = start;
	}

	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (size_type(index) < (this->size() >> 1)) {
			std::copy_backward(start, pos, next);
			pop_front();
		}
		else {
			std::copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	iterator erase(iterator first, iterator last)
	{
		if (first == start && last == finish) {
			clear();
			return finish;
		}
		else {
			difference_type n = last - first;
			difference_type elems_before = first - start;
			if (elems_before < difference_type((this->size() - n) / 2)) {
				std::copy_backward(start, first, last);    ////!!!!!!!                      
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start.node; cur < new_start.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				start = new_start;
			}
			else {
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
					data_allocator::deallocate(*cur, buffer_size());
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	iterator insert(iterator position, const value_type& x) {
		if (position.cur == start.cur) {
			push_front(x);
			return start;
		}
		else if (position.cur == finish.cur) {
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else {
			return insert_aux(position, x);
		}
	}

	iterator insert_aux(iterator pos, const value_type& x)
	{
		difference_type index = pos - start;
		value_type x_copy = x;
		if (size_type(index) < this->size() / 2) {
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;
			copy(front2, pos1, front1);
		}
		else {
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;
			copy_backward(pos, back2, back1);
		}
		*pos = x_copy;
		return pos;
	}
};
