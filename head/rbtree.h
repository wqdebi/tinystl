#pragma once

#include <memory>
#include <new>
#include <algorithm>
#include <functional>
#include "rb_tree_iterator.h"
#include "std_alloc.h"

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc = alloc>
class rb_tree {
private:
    using base_ptr = __rb_tree_node_base*;
    using rb_tree_node = __rb_tree_node<Value>;
    using rb_tree_node_allocator = simple_alloc<rb_tree_node, alloc>;
    using color_type = rb_tree_color_type;

public:
    using key_type = Key;
    using value_type = Value;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using link_type = rb_tree_node*;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    // Iterator
    using iterator = rb_tree_iterator<value_type, value_type&, value_type*>;
    using const_iterator = rb_tree_iterator<value_type, const_reference, const_pointer>;

private: // operation of node
    // 分配内存
    link_type get_node() {
        return rb_tree_node_allocator::allocate();
    }
    // 释放内存
    void put_node(link_type p) {
        rb_tree_node_allocator::deallocate(p);
    }
    //
    link_type create_node(const value_type& val) {
        link_type temp = get_node();
        try {
            new (&temp->value_field) value_type(val);

        }
        catch (std::exception&) {
            put_node(temp);
        }
        return temp;
    }
    // 克隆节点，包括颜色
    link_type clone_node(link_type p) {
        link_type temp = create_node(p->value_field);
        temp->color = p->color;
        temp->left = nullptr;
        temp->right = nullptr;
        return temp;
    }


    void destroy_node(link_type p) {
        Value* vp = &(p->value_field);
        vp->~Value();
        put_node(p);
    }

private: // data member
    size_type node_count;
    link_type header;
    Compare key_compare;

private: // data member getter && setter
    // header成员
    link_type& root() const noexcept {
        return reinterpret_cast<link_type&>(header->parent);
    }
    // head->left指向的是最小值
    link_type& leftmost() const noexcept {
        return reinterpret_cast<link_type&>(header->left);
    }
    // head->right指向的是最大值
    link_type& rightmost() const noexcept {
        return reinterpret_cast<link_type&>(header->right);
    }

    // 普通node的快速访问与设定
    //  静态成员函数的作用在于：调用这个函数不会
    // 访问或者修改任何对象（非static）数据成员。
    static link_type& left(link_type p) {
        return reinterpret_cast<link_type&>(p->left);
    }
    static link_type& right(link_type p) {
        return reinterpret_cast<link_type&>(p->right);
    }
    static link_type& parent(link_type p) {
        return reinterpret_cast<link_type&>(p->parent);
    }
    static reference& value(link_type p) {
        return p->value_field;
    }
    static const Key& key(link_type p) {
        return KeyOfValue()(value(p));
    }
    static color_type& color(link_type p) { return p->color; }

    // base_node 的快速访问和设定
    static link_type& left(base_ptr p) {
        return reinterpret_cast<link_type&>(p->left);
    }
    static link_type& right(base_ptr p) {
        return reinterpret_cast<link_type&>(p->right);
    }
    static link_type& parent(base_ptr p) {
        return reinterpret_cast<link_type&>(p->parent);
    }

    static reference& value(base_ptr p) {
        return reinterpret_cast<link_type&>(p)->value_field;
    }
    static const Key& key(base_ptr p) {
        return KeyOfValue()(value(reinterpret_cast<link_type&>(p)));
    }
    static color_type& color(base_ptr p) {
        return reinterpret_cast<link_type&>(p)->color;
    }

    // 求取极值
    static link_type minimum(link_type p) {
        return reinterpret_cast<link_type>(
            __rb_tree_node_base::minimum(p));
    }
    static link_type maximum(link_type p) {
        return reinterpret_cast<link_type>(
            __rb_tree_node_base::maximum(p));
    }
private: // aux interface
    link_type copy(link_type, link_type);
    void empty_initialize() {
        header = get_node();
        color(header) = rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }

private: // rotate && rebalance
    void rb_tree_rotate_left(base_ptr, base_ptr&);
    void rb_tree_rotate_right(base_ptr, base_ptr&);
    void rb_tree_rebalance(base_ptr, base_ptr&);
    base_ptr rb_tree_rebalance_for_erase(base_ptr, base_ptr&,
        base_ptr&, base_ptr&);

public: // ctor && dtor
    // key_compare()调用compare的默认构造
    rb_tree() : node_count(0), key_compare() { empty_initialize(); }
    explicit rb_tree(const Compare& comp) : node_count(0), key_compare(comp) {
        empty_initialize();
    }
    ~rb_tree() {
        clear();
        put_node(header);
    }

public: // copy operation, 
    rb_tree(const rb_tree& rhs) : node_count(0), key_compare(rhs.key_compare) {
        // 创建一个header并完成初始化
        if (!rhs.root())
            empty_initialize();
        else {
            header = get_node();
            color(header) = rb_tree_red;
            root() = copy(rhs.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
        node_count = rhs.node_count;
    }

    rb_tree& operator=(const rb_tree&);

public: // move operation
    rb_tree(rb_tree&& rhs) noexcept {
        empty_initialize();
        swap(rhs);
    }

    rb_tree& operator=(rb_tree&& rhs) noexcept {
        clear();
        swap(rhs);
        return *this;
    }

public: // getter
    // 返回的时候会自动调用const_iterator(leftmost())
    const_iterator begin() const noexcept { return leftmost(); }
    const_iterator end() const noexcept { return header; }
    const_iterator cbegin() const noexcept { return leftmost(); }
    const_iterator cend() const noexcept { return header; }
    bool empty() {
        return node_count == 0;
    }
    size_type size() const noexcept {
        return node_count;
    }

public:
    iterator begin() noexcept { return leftmost(); }
    iterator end() noexcept { return header; }

private: // aux interface for insert
    iterator insert_aux(base_ptr, base_ptr, const value_type&);

public: // insert
    std::pair<iterator, bool> insert_unique(const value_type&);
    iterator insert_unique(iterator, const value_type&);
    template <class InputIterator>
    void insert_unique(InputIterator, InputIterator);
    iterator insert_equal(iterator, const value_type&);
    iterator insert_equal(const value_type&);
    template <class InputIterator>
    void insert_equal(InputIterator, InputIterator);

private: // aux interface for erase
    void erase_aux(link_type) noexcept;

public: // erase;
    void erase(iterator);
    size_type erase(const key_type&);
    void erase(iterator, iterator);
    void clear() noexcept;

public: // find
    iterator find(const key_type&) noexcept;
    const_iterator find(const key_type&) const noexcept;
    size_type count(const key_type&) const noexcept;
    iterator lower_bound(const key_type&)  noexcept;
    const_iterator lower_bound(const key_type&) const noexcept;
    iterator upper_bound(const key_type&)  noexcept;
    const_iterator upper_bound(const key_type&) const noexcept;
    std::pair<iterator, iterator> equal_range(const key_type&) noexcept;
    std::pair<const_iterator, const_iterator> equal_range(const key_type&) const noexcept;

public: // swap
    void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
        std::swap(header, rhs.header); // header也是指针
        std::swap(node_count, rhs.node_count);
        std::swap(key_compare, rhs.key_compare);
    }

};


// ----------------------------insert-------------
// 每次有新数据插入，一定是先在叶子节点插入
// 该接口只做插入操作，把val插入到y的左孩子或者右孩子
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_aux(base_ptr x_, base_ptr y_, const value_type& val) {
    link_type x = reinterpret_cast<link_type>(x_);
    link_type y = reinterpret_cast<link_type>(y_);
    link_type z;
    // 先满足左边插入
    // 
    if (y == header || x || key_compare(KeyOfValue()(val), key(y))) {
        z = create_node(val);
        left(y) = z;
        if (y == header) {
            root() = z;
            rightmost() = z;
        }
        else if (y == leftmost()) {
            leftmost() = z;
        }
    }
    else {
        z = create_node(val);
        right(y) = z;
        if (rightmost() == y) {
            rightmost() = z;
        }
    }
    // 更新z的节点指针
    parent(z) = y;
    // 插入节点之后需要检查是否破坏平衡
    rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);
}

// 节点插入后需要重新维护这棵树
// 不涉及val就可以直接使用base_ptr节点类型
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance(base_ptr x, base_ptr& root) {
    // 会调用这个函数，那当前节点应该是红色，才会调整
    x->color = rb_tree_red;
    // 需要向上调整的情况, 非根节点且父节点为红色
    // 从这个条件可以看出 x->parent != root,显然最多祖父到root节点
    while (x != root && x->parent->color == rb_tree_red) {
        // 考虑父亲是祖父的哪一个孩子
        // 情况一：父亲为祖父的左子
        if (x->parent == x->parent->parent->left) {
            // 考虑叔叔的颜色
            base_ptr y = x->parent->parent->right;
            // 1、叔叔存在且为红色
            if (y && y->color == rb_tree_red)
            {
                // 直接调整颜色父亲，叔叔为黑色，祖父调为红色
                x->parent->color = rb_tree_black;
                y->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // 这个时候祖父需要作为当前节点被调整
                x = x->parent->parent;
            }
            else // 2、叔叔不存在或为黑色,是同一种情况，因为nullptr被看作虚拟叶子节点，是黑色
            {
                //（1）我是父亲的右孩子,需要先L(左旋)再R(右旋)
                if (x == x->parent->right) {
                    // 左旋
                    x = x->parent; // 旋转之后才能继续作为孩子节点
                    // x是需要旋转节点的最上面的节点
                    rb_tree_rotate_left(x, root);
                }
                // (2) 现在x在左边
                // 先把颜色互换，再旋转就平衡了
                x->parent->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // 右旋
                rb_tree_rotate_right(x->parent->parent, root);
            }
        }
        else // 情况二：父亲为祖父的右子
        {
            // 考虑叔叔的颜色
            base_ptr y = x->parent->parent->left;
            // 1、叔叔存在且为红色
            if (y && y->color == rb_tree_red)
            {
                // 直接调整颜色父亲，叔叔为黑色，祖父调为红色
                x->parent->color = rb_tree_black;
                y->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // 这个时候祖父需要作为当前节点被调整
                x = x->parent->parent;
            }
            else // 2、叔叔不存在或为黑色,是同一种情况，因为nullptr被看作虚拟叶子节点，是黑色
            {
                //（1）我是父亲的左孩子,需要先R(右旋)在L(左旋)
                if (x == x->parent->left)
                {
                    // 右旋
                    x = x->parent; // 旋转之后才能继续作为孩子节点
                    // x是需要旋转节点的最上面的节点
                    rb_tree_rotate_right(x, root);
                }
                // (2) 现在x在右边
                // 先把颜色互换，再旋转就平衡了
                x->parent->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // 右旋
                rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    // 根节点一定为黑色
    root->color = rb_tree_black;
}

// 左旋
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(base_ptr x, base_ptr& root) {
    // 旋转节点右子
    base_ptr y = x->right;
    // 右子的左孩子移到当前节点的右子
    x->right = y->left;
    if (y->left) y->left->parent = x; // y->left处理完

    // 处理x->parent
    y->parent = x->parent;
    if (x == root) // x->parent == header
        root = y; // 相当于root存的地址直接变成了y
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    // 处理y
    y->left = x;
    x->parent = y;

}

// 右旋
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(base_ptr x, base_ptr& root) {
    // 旋转节点左子
    base_ptr y = x->left;
    // 左子的右孩子移到当前节点的左子
    x->left = y->right;
    if (y->right) y->right->parent = x;

    y->parent = x->parent;
    // root == x;
    if (x == root) {
        root = y;
    }
    else if (x->parent->left == x)
    {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->right = x;
    x->parent = y;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) noexcept {
    link_type y = header; // 最后一个不小于k的节点
    link_type x = root();
    // find找到的应该是最左边等于k的节点
    while (x) {
        if (!key_compare(key(x), k)) {
            y = x;
            x = left(x);
        }
        else {
            x = right(x);
        }
    }
    iterator j = iterator(y);
    // 没找到就返回end()
    return (j == end()) || key_compare(k, key(j.node)) ? end() : j;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const noexcept {
    link_type y = header; // 最后一个不小于k的节点
    link_type x = root();
    // find找到的应该是最左边等于k的节点
    while (x) {
        if (!key_compare(key(x), k)) {
            y = x;
            x = left(x);
        }
        else {
            x = right(x);
        }
    }
    const_iterator j = const_iterator(y);
    // 没找到就返回end()
    return (j == end()) || key_compare(k, key(j.node)) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const noexcept {
    std::pair<iterator, iterator> p = equal_range(k);
    return std::distance(p.first, p.second);
}

// lower_bound(k)  it >= k;
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) noexcept {
    link_type y = header; //y >= k
    link_type x = root();

    while (x) {
        if (!key_compare(key(x), k)) {
            y = x;
            x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return iterator(y);
}

// lower_bound(k)  it >= k;
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) const noexcept {
    link_type y = header; //y >= k
    link_type x = root();

    while (x) {
        if (!key_compare(key(x), k)) {
            y = x;
            x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return const_iterator(y);
}

// upper_bound(k)  it > k;
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) noexcept {
    link_type y = header; //y > k y在最右边
    link_type x = root();

    while (x) {
        if (key_compare(k, key(x))) {
            y = x;
            x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return iterator(y);
}


// upper_bound(k)  it > k;
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) const noexcept {
    link_type y = header; //y > k y在最右边
    link_type x = root();

    while (x) {
        if (key_compare(k, key(x))) {
            y = x;
            x = left(x);
        }
        else {
            x = right(x);
        }
    }
    return const_iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) noexcept {
    return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) const noexcept {
    return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree& rhs) {
    if (this != &rhs) {
        clear(); // 释放的是root()的所有节点，header回到初始化
        key_compare = rhs.key_compare;
        if (rhs.root()) {
            root() = copy(rhs.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count = rhs.node_count;
        }
    }
    return *this;
}

// 只插入原本不存在的数据
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& val) {
    link_type y = header;
    link_type x = root();

    bool comp = true;
    while (x) {
        y = x;
        comp = key_compare(KeyOfValue()(val), key(x));
        x = comp ? left(x) : right(x);
    }
    iterator j(y);
    // y为待插入节点的父节点，并且为叶子节点
    if (comp) { // 插入父节点的左孩子
        if (j == begin())
            return std::pair<iterator, bool>(insert_aux(x, y, val), true);
        else  // j是待插入节点的父节点，--j是j的前一个节点
                // --j <= val < j, 
            --j; // j <= val
    }
    // val >= j
    if (key_compare(key(j.node), KeyOfValue()(val)))
        return std::pair<iterator, bool>(insert_aux(x, y, val), true);
    return std::pair<iterator, bool>(j, false); // 重复值
}

// 
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator pos, const value_type& val) {
    if (pos.node == header->left) { // begin()
        if (size() > 0 && key_compare(KeyOfValue()(val), key(pos.node)))
            return insert_aux(pos.node, pos.node, val);
        else
            return insert_unique(val).first;
    }
    else if (pos.node == header) { // end()
        if (key_compare(key(rightmost()), KeyOfValue()(val)))
            return insert_aux(nullptr, rightmost(), val);
        else
            return insert_unique(val).first;
    }
    else {
        iterator before = pos;
        --before;
        // before < val && val < pos
        if (key_compare(key(before.node), KeyOfValue()(val) &&
            key_compare(KeyOfValue()(val), key(pos.node)))) {
            // before是pos的左孩子的最右边的节点
            if (!right(before.node)) {
                return insert_aux(nullptr, before.node, val);
            }
            else { // before是pos的祖先节点
                return insert_aux(pos.node, pos.node, val);
            }
        }
        else {
            return insert_unique(val).first;
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(
    InputIterator first, InputIterator last) {
    for (; first != last; ++first) insert_unique(*first);
}

// 可以插入相同的元素
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& val) {
    link_type y = header;
    link_type x = root();
    while (x) {
        y = x;
        x = key_compare(KeyOfValue()(val), key(x)) ? left(x) : right(x);
    }
    return insert_aux(x, y, val);
}

// 某位置插入某值
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator pos, const value_type& val) {
    // 插入位置[begin(), end()]
    if (pos.node == header->left) { // begin()
        if (key_compare(KeyOfValue()(val), key(pos.node)))
            return insert_aux(pos.node, pos.node, val);
        else
            insert_equal(val);
    }
    else if (pos.node == header) {
        // 右边直接插入val >= rightmost()
        if (!key_compare(KeyOfValue()(val), key(rightmost())))
            return insert_equal(nullptr, rightmost(), val);
        else
            return insert_equal(val);
    }
    else {
        iterator before = pos;
        --before;

        // before <= val <= pos
        if (!key_compare(KeyOfValue()(val), key(before.node) &&
            !key_compare(key(pos.node), KeyOfValue()(val))))
            // before是pos的左边孩子的最右节点
            if (!right(before.node))
                return insert_aux(nullptr, before.node, val);
            else // pos没有左边孩子
                return insert_aux(pos.node, pos.node, val);
        // pos是before右边孩子的最左节点
        else {
            // val < before || val > pos
            // 没有办法直接插入
            return insert_equal(val);
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(
    InputIterator first, InputIterator last) {
    for (; first != last; ++first) insert_equal(*first);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::copy(link_type source_x, link_type obj_p)
{
    link_type top = clone_node(source_x);
    top->parent = obj_p; // 复制好每个节点的父指针

    try
    {
        // 回溯法，先处理右边节点
        if (source_x->right != nullptr)
        {
            // top就是下一个复制节点的父亲
            top->right = copy(right(source_x), top);
            // 上面结束代表top的右边子树已经建好
            obj_p = top;
            source_x = left(source_x);

            // 建好top的左树
            while (source_x != nullptr)
            {
                link_type y = clone_node(source_x);
                obj_p->left = y;
                y->parent = obj_p;
                if (source_x->right)
                    y->right = copy(right(source_x), y);
                obj_p = y;
                source_x = left(source_x);
            }
        }
    }
    catch (std::exception&)
    {
        erase_aux(top);
    }
    return top;
}

// ----------------delete-----------------------

// 单纯删除节点
// 回收包含x节点的整棵子树
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_aux(link_type x) noexcept {
    while (x) {
        erase_aux(right(x));
        link_type y = left(x);
        destroy_node(x);
        x = y;
    }
}

// 给外界的erase，需要保持平衡
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
    link_type y = reinterpret_cast<link_type>(rb_tree_rebalance_for_erase(
        pos.node, header->parent, header->left, header->right));
    destroy_node(y);
    --node_count;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type& k) {
    std::pair<iterator, iterator> p = equal_range(k);
    size_type n = std::distance(p.first, p.second);
    erase(p.first, p.second);
    return n;
}

// 保持平衡的删除
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
    if (first == begin() && last == end()) {
        clear();
    }
    else {
        while (first != last) erase(first++);
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() noexcept {
    if (node_count) {
        erase_aux(root());
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
        node_count = 0;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance_for_erase(
        base_ptr z, base_ptr& root, base_ptr& leftmost, base_ptr& rightmost) {
    // 删除z节点
    base_ptr y = z; //需要删除的节点
    base_ptr x = nullptr;
    base_ptr x_parent = nullptr;

    // 
    if (!y->left) { // 无左孩子
        x = y->right;
    }
    else { // 一定有左孩子
        if (!y->right) {
            x = y->left;
        }
        else { // 有两个孩子,用后继节点替换
            y = y->right;
            while (y->left) y = y->left;
            x = y->right;
        }
    }
    // y != z,那么z一定有两个孩子
    if (y != z) {
        // 直接把z的数据赋值为y
        reinterpret_cast<link_type>(z)->value_field =
            reinterpret_cast<link_type>(y)->value_field;
        z = y;
    }
    // 把要删除节点的孩子提上来
    x_parent = y->parent;
    if (x) x->parent = y->parent;
    if (root == z) { // 要删除的节点是根节点
        root = x;
    }
    else if (z->parent->left == z) {
        z->parent->left = x;
    }
    else {
        z->parent->right = x;
    }
    if (leftmost == z) {
        if (!z->right)
            leftmost = z->parent;
        else
            leftmost = __rb_tree_node_base::minimum(x);
    }
    if (rightmost == z) {
        if (!z->left)
            rightmost = z->parent;
        else
            rightmost = __rb_tree_node_base::maximum(x);
    }
    // 平衡颜色
    // 删除红色节点不会破坏平衡
    // 删除节点颜色为黑色，则它一定有兄弟节点，不然黑色不平衡
    if (y->color != rb_tree_red) {
        // 自上向下调整
        // x这边少一个黑色节点
        while (x != root && (!x || x->color == rb_tree_black))
        {
            if (x == x_parent->left)
            {
                // x的兄弟节点
                base_ptr w = x_parent->right;
                // case 1 兄弟节点的颜色是红色
                /*
                (01) 将x的兄弟节点设为“黑色”。
                (02) 将x的父节点设为“红色”。
                (03) 对x的父节点进行左旋。
                (04) 左旋后，重新设置x的兄弟节点。
                */
                if (w->color == rb_tree_red)
                {
                    w->color = rb_tree_black;
                    x_parent->color = rb_tree_red;
                    rb_tree_rotate_left(x_parent, root);
                    w = x_parent->right;
                } // 此时x的兄弟节点已经改变， x仍然少一个黑色节点
                    //它的兄弟节点也一定是黑色节点了
                //x的兄弟节点是黑色，x的兄弟节点的两个孩子都是黑色。
                /*
                (01) 将x的兄弟节点设为“红色”。
                (02) 设置“x的父节点”为“新的x节点”
                */
                if ((!w->left || w->left->color == rb_tree_black) &&
                    (!w->right || w->right->color == rb_tree_black))
                {
                    w->color = rb_tree_red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    // x的兄弟节点是黑色；
                    // x的兄弟节点的左孩子是红色，右孩子是黑色的。
                    /*
                    (01) 将x兄弟节点的左孩子设为“黑色”。
                    (02) 将x兄弟节点设为“红色”。
                    (03) 对x的兄弟节点进行右旋。
                    (04) 右旋后，重新设置x的兄弟节点。
                    */
                    if (!w->right || w->right->color == rb_tree_black)
                    {
                        if (w->left)
                            w->left->color = rb_tree_black;
                        w->color = rb_tree_red;
                        rb_tree_rotate_right(w, root);
                        w = x_parent->right;
                    } // 兄弟节点为黑色，右边孩子是红色
                        // x的兄弟节点是黑色；x的兄弟节点的右孩子是红色的，x的兄弟节点的左孩子任意颜色。
                    /*
                    (01) 将x父节点颜色赋值给 x的兄弟节点。
                    (02) 将x父节点设为“黑色”。
                    (03) 将x兄弟节点的右子节设为“黑色”。
                    (04) 对x的父节点进行左旋。
                    (05) 设置“x”为“根节点”。
                    */
                    w->color = x_parent->color;
                    x_parent->color = rb_tree_black;
                    if (w->right)
                        w->right->color = rb_tree_black;
                    rb_tree_rotate_left(x_parent, root);
                    break;
                }
            }
            else
            {
                base_ptr w = x_parent->left;
                if (w->color == rb_tree_red)
                {
                    w->color = rb_tree_black;
                    x_parent->color = rb_tree_red;
                    rb_tree_rotate_right(x_parent, root);
                    w = x_parent->left;
                }
                if ((!w->right || w->right->color == rb_tree_black) &&
                    (!w->left || w->left->color == rb_tree_black))
                {
                    w->color = rb_tree_red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                }
                else
                {
                    if (!w->left || w->left->color == rb_tree_black)
                    {
                        if (w->right)
                            w->right->color = rb_tree_black;
                        w->color = rb_tree_red;
                        rb_tree_rotate_left(w, root);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = rb_tree_black;
                    if (w->left)
                        w->left->color = rb_tree_black;
                    rb_tree_rotate_right(x_parent, root);
                    break;
                }
            }
        }
        if (x)
            x->color = rb_tree_black;
    }
    return y;
}

// operator
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator==(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return lhs.size() == rhs.size() &&
        std::equal(lhs.cbegin(), rhs.cbegin(), rhs.cend());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator!=(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return !(lhs == rhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return std::lexicographical_compare(
        lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return rhs < lhs;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<=(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return !(rhs < lhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>=(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
    return !(lhs < rhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void swap(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) noexcept {
    lhs.swap(rhs);
}

