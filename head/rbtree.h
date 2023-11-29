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
    // �����ڴ�
    link_type get_node() {
        return rb_tree_node_allocator::allocate();
    }
    // �ͷ��ڴ�
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
    // ��¡�ڵ㣬������ɫ
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
    // header��Ա
    link_type& root() const noexcept {
        return reinterpret_cast<link_type&>(header->parent);
    }
    // head->leftָ�������Сֵ
    link_type& leftmost() const noexcept {
        return reinterpret_cast<link_type&>(header->left);
    }
    // head->rightָ��������ֵ
    link_type& rightmost() const noexcept {
        return reinterpret_cast<link_type&>(header->right);
    }

    // ��ͨnode�Ŀ��ٷ������趨
    //  ��̬��Ա�������������ڣ����������������
    // ���ʻ����޸��κζ��󣨷�static�����ݳ�Ա��
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

    // base_node �Ŀ��ٷ��ʺ��趨
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

    // ��ȡ��ֵ
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
    // key_compare()����compare��Ĭ�Ϲ���
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
        // ����һ��header����ɳ�ʼ��
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
    // ���ص�ʱ����Զ�����const_iterator(leftmost())
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
        std::swap(header, rhs.header); // headerҲ��ָ��
        std::swap(node_count, rhs.node_count);
        std::swap(key_compare, rhs.key_compare);
    }

};


// ----------------------------insert-------------
// ÿ���������ݲ��룬һ��������Ҷ�ӽڵ����
// �ýӿ�ֻ�������������val���뵽y�����ӻ����Һ���
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_aux(base_ptr x_, base_ptr y_, const value_type& val) {
    link_type x = reinterpret_cast<link_type>(x_);
    link_type y = reinterpret_cast<link_type>(y_);
    link_type z;
    // ��������߲���
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
    // ����z�Ľڵ�ָ��
    parent(z) = y;
    // ����ڵ�֮����Ҫ����Ƿ��ƻ�ƽ��
    rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);
}

// �ڵ�������Ҫ����ά�������
// ���漰val�Ϳ���ֱ��ʹ��base_ptr�ڵ�����
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance(base_ptr x, base_ptr& root) {
    // ���������������ǵ�ǰ�ڵ�Ӧ���Ǻ�ɫ���Ż����
    x->color = rb_tree_red;
    // ��Ҫ���ϵ��������, �Ǹ��ڵ��Ҹ��ڵ�Ϊ��ɫ
    // ������������Կ��� x->parent != root,��Ȼ����游��root�ڵ�
    while (x != root && x->parent->color == rb_tree_red) {
        // ���Ǹ������游����һ������
        // ���һ������Ϊ�游������
        if (x->parent == x->parent->parent->left) {
            // �����������ɫ
            base_ptr y = x->parent->parent->right;
            // 1�����������Ϊ��ɫ
            if (y && y->color == rb_tree_red)
            {
                // ֱ�ӵ�����ɫ���ף�����Ϊ��ɫ���游��Ϊ��ɫ
                x->parent->color = rb_tree_black;
                y->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // ���ʱ���游��Ҫ��Ϊ��ǰ�ڵ㱻����
                x = x->parent->parent;
            }
            else // 2�����岻���ڻ�Ϊ��ɫ,��ͬһ���������Ϊnullptr����������Ҷ�ӽڵ㣬�Ǻ�ɫ
            {
                //��1�����Ǹ��׵��Һ���,��Ҫ��L(����)��R(����)
                if (x == x->parent->right) {
                    // ����
                    x = x->parent; // ��ת֮����ܼ�����Ϊ���ӽڵ�
                    // x����Ҫ��ת�ڵ��������Ľڵ�
                    rb_tree_rotate_left(x, root);
                }
                // (2) ����x�����
                // �Ȱ���ɫ����������ת��ƽ����
                x->parent->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // ����
                rb_tree_rotate_right(x->parent->parent, root);
            }
        }
        else // �����������Ϊ�游������
        {
            // �����������ɫ
            base_ptr y = x->parent->parent->left;
            // 1�����������Ϊ��ɫ
            if (y && y->color == rb_tree_red)
            {
                // ֱ�ӵ�����ɫ���ף�����Ϊ��ɫ���游��Ϊ��ɫ
                x->parent->color = rb_tree_black;
                y->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // ���ʱ���游��Ҫ��Ϊ��ǰ�ڵ㱻����
                x = x->parent->parent;
            }
            else // 2�����岻���ڻ�Ϊ��ɫ,��ͬһ���������Ϊnullptr����������Ҷ�ӽڵ㣬�Ǻ�ɫ
            {
                //��1�����Ǹ��׵�����,��Ҫ��R(����)��L(����)
                if (x == x->parent->left)
                {
                    // ����
                    x = x->parent; // ��ת֮����ܼ�����Ϊ���ӽڵ�
                    // x����Ҫ��ת�ڵ��������Ľڵ�
                    rb_tree_rotate_right(x, root);
                }
                // (2) ����x���ұ�
                // �Ȱ���ɫ����������ת��ƽ����
                x->parent->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                // ����
                rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    // ���ڵ�һ��Ϊ��ɫ
    root->color = rb_tree_black;
}

// ����
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(base_ptr x, base_ptr& root) {
    // ��ת�ڵ�����
    base_ptr y = x->right;
    // ���ӵ������Ƶ���ǰ�ڵ������
    x->right = y->left;
    if (y->left) y->left->parent = x; // y->left������

    // ����x->parent
    y->parent = x->parent;
    if (x == root) // x->parent == header
        root = y; // �൱��root��ĵ�ֱַ�ӱ����y
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    // ����y
    y->left = x;
    x->parent = y;

}

// ����
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(base_ptr x, base_ptr& root) {
    // ��ת�ڵ�����
    base_ptr y = x->left;
    // ���ӵ��Һ����Ƶ���ǰ�ڵ������
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
    link_type y = header; // ���һ����С��k�Ľڵ�
    link_type x = root();
    // find�ҵ���Ӧ��������ߵ���k�Ľڵ�
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
    // û�ҵ��ͷ���end()
    return (j == end()) || key_compare(k, key(j.node)) ? end() : j;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const noexcept {
    link_type y = header; // ���һ����С��k�Ľڵ�
    link_type x = root();
    // find�ҵ���Ӧ��������ߵ���k�Ľڵ�
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
    // û�ҵ��ͷ���end()
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
    link_type y = header; //y > k y�����ұ�
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
    link_type y = header; //y > k y�����ұ�
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
        clear(); // �ͷŵ���root()�����нڵ㣬header�ص���ʼ��
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

// ֻ����ԭ�������ڵ�����
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
    // yΪ������ڵ�ĸ��ڵ㣬����ΪҶ�ӽڵ�
    if (comp) { // ���븸�ڵ������
        if (j == begin())
            return std::pair<iterator, bool>(insert_aux(x, y, val), true);
        else  // j�Ǵ�����ڵ�ĸ��ڵ㣬--j��j��ǰһ���ڵ�
                // --j <= val < j, 
            --j; // j <= val
    }
    // val >= j
    if (key_compare(key(j.node), KeyOfValue()(val)))
        return std::pair<iterator, bool>(insert_aux(x, y, val), true);
    return std::pair<iterator, bool>(j, false); // �ظ�ֵ
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
            // before��pos�����ӵ����ұߵĽڵ�
            if (!right(before.node)) {
                return insert_aux(nullptr, before.node, val);
            }
            else { // before��pos�����Ƚڵ�
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

// ���Բ�����ͬ��Ԫ��
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

// ĳλ�ò���ĳֵ
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator pos, const value_type& val) {
    // ����λ��[begin(), end()]
    if (pos.node == header->left) { // begin()
        if (key_compare(KeyOfValue()(val), key(pos.node)))
            return insert_aux(pos.node, pos.node, val);
        else
            insert_equal(val);
    }
    else if (pos.node == header) {
        // �ұ�ֱ�Ӳ���val >= rightmost()
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
            // before��pos����ߺ��ӵ����ҽڵ�
            if (!right(before.node))
                return insert_aux(nullptr, before.node, val);
            else // posû����ߺ���
                return insert_aux(pos.node, pos.node, val);
        // pos��before�ұߺ��ӵ�����ڵ�
        else {
            // val < before || val > pos
            // û�а취ֱ�Ӳ���
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
    top->parent = obj_p; // ���ƺ�ÿ���ڵ�ĸ�ָ��

    try
    {
        // ���ݷ����ȴ����ұ߽ڵ�
        if (source_x->right != nullptr)
        {
            // top������һ�����ƽڵ�ĸ���
            top->right = copy(right(source_x), top);
            // �����������top���ұ������Ѿ�����
            obj_p = top;
            source_x = left(source_x);

            // ����top������
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

// ����ɾ���ڵ�
// ���հ���x�ڵ����������
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

// ������erase����Ҫ����ƽ��
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

// ����ƽ���ɾ��
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
    // ɾ��z�ڵ�
    base_ptr y = z; //��Ҫɾ���Ľڵ�
    base_ptr x = nullptr;
    base_ptr x_parent = nullptr;

    // 
    if (!y->left) { // ������
        x = y->right;
    }
    else { // һ��������
        if (!y->right) {
            x = y->left;
        }
        else { // ����������,�ú�̽ڵ��滻
            y = y->right;
            while (y->left) y = y->left;
            x = y->right;
        }
    }
    // y != z,��ôzһ������������
    if (y != z) {
        // ֱ�Ӱ�z�����ݸ�ֵΪy
        reinterpret_cast<link_type>(z)->value_field =
            reinterpret_cast<link_type>(y)->value_field;
        z = y;
    }
    // ��Ҫɾ���ڵ�ĺ���������
    x_parent = y->parent;
    if (x) x->parent = y->parent;
    if (root == z) { // Ҫɾ���Ľڵ��Ǹ��ڵ�
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
    // ƽ����ɫ
    // ɾ����ɫ�ڵ㲻���ƻ�ƽ��
    // ɾ���ڵ���ɫΪ��ɫ������һ�����ֵܽڵ㣬��Ȼ��ɫ��ƽ��
    if (y->color != rb_tree_red) {
        // �������µ���
        // x�����һ����ɫ�ڵ�
        while (x != root && (!x || x->color == rb_tree_black))
        {
            if (x == x_parent->left)
            {
                // x���ֵܽڵ�
                base_ptr w = x_parent->right;
                // case 1 �ֵܽڵ����ɫ�Ǻ�ɫ
                /*
                (01) ��x���ֵܽڵ���Ϊ����ɫ����
                (02) ��x�ĸ��ڵ���Ϊ����ɫ����
                (03) ��x�ĸ��ڵ����������
                (04) ��������������x���ֵܽڵ㡣
                */
                if (w->color == rb_tree_red)
                {
                    w->color = rb_tree_black;
                    x_parent->color = rb_tree_red;
                    rb_tree_rotate_left(x_parent, root);
                    w = x_parent->right;
                } // ��ʱx���ֵܽڵ��Ѿ��ı䣬 x��Ȼ��һ����ɫ�ڵ�
                    //�����ֵܽڵ�Ҳһ���Ǻ�ɫ�ڵ���
                //x���ֵܽڵ��Ǻ�ɫ��x���ֵܽڵ���������Ӷ��Ǻ�ɫ��
                /*
                (01) ��x���ֵܽڵ���Ϊ����ɫ����
                (02) ���á�x�ĸ��ڵ㡱Ϊ���µ�x�ڵ㡱
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
                    // x���ֵܽڵ��Ǻ�ɫ��
                    // x���ֵܽڵ�������Ǻ�ɫ���Һ����Ǻ�ɫ�ġ�
                    /*
                    (01) ��x�ֵܽڵ��������Ϊ����ɫ����
                    (02) ��x�ֵܽڵ���Ϊ����ɫ����
                    (03) ��x���ֵܽڵ����������
                    (04) ��������������x���ֵܽڵ㡣
                    */
                    if (!w->right || w->right->color == rb_tree_black)
                    {
                        if (w->left)
                            w->left->color = rb_tree_black;
                        w->color = rb_tree_red;
                        rb_tree_rotate_right(w, root);
                        w = x_parent->right;
                    } // �ֵܽڵ�Ϊ��ɫ���ұߺ����Ǻ�ɫ
                        // x���ֵܽڵ��Ǻ�ɫ��x���ֵܽڵ���Һ����Ǻ�ɫ�ģ�x���ֵܽڵ������������ɫ��
                    /*
                    (01) ��x���ڵ���ɫ��ֵ�� x���ֵܽڵ㡣
                    (02) ��x���ڵ���Ϊ����ɫ����
                    (03) ��x�ֵܽڵ�����ӽ���Ϊ����ɫ����
                    (04) ��x�ĸ��ڵ����������
                    (05) ���á�x��Ϊ�����ڵ㡱��
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

