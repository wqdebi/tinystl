#pragma once

#include <cstddef>
#include "rb_tree_node.h"
#include "stl_iterator.h"


struct rb_tree_base_iterator {
    using base_ptr = __rb_tree_node_base::base_ptr;
    using iterator_category = bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;

    base_ptr node;
    // 供重载操作符++使用，node不可能直接为header
    // 顺序移动到下一个位置
    void increment() {
        // 存在右节点，则下一节点比为右子树的最左下角
        if (node->right) {
            node = node->right;
            while (node->left) node = node->left;
        }
        else { // 没有右节点，则应该是不断上溯，第一个不为右子节点的祖先
            while (node->parent->right == node) // 到根节点root不是header也会跳出循环
                node = node->parent;

            // 最右边节点的下一个节点应该是header
            // 只有一种情况node会到header，就是迭代器指向root，并且无右节点
            if (node->right != node->parent) node = node->parent;
            // 当node等于header时，就等于header
        }
    }

    // 供重载操作符--使用，
    // 前一个位置
    void decrement() {
        // header理解为最后一个节点
        // 当node为header时
        if (node->color == rb_tree_red && node->parent->parent == node) {
            node = node->right;
        }
        else if (node->left) // 存在左子节点
        {
            base_ptr l = node->left;
            while (l->right) l = l->right;
            node = l;
        }
        else { // 非header，无左子
            base_ptr p = node->parent;
            while (node == p->left) {
                node = p;
                p = p->parent;
            }
            node = p;
        }
    }
};

template <class T, class Ref, class Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator {
    using value_type = T;
    using reference = Ref;
    using pointer = Ptr;
    using iterator = rb_tree_iterator<T, T&, T*>;
    using const_iterator = rb_tree_iterator<T, const T&, const T*>;
    using self = rb_tree_iterator<T, Ref, Ptr>;
    using link_type = __rb_tree_node<T>*;

    rb_tree_iterator() {}
    rb_tree_iterator(link_type x) { node = x; }
    rb_tree_iterator(const iterator& it) { node = it.node; }

    reference operator*() const {
        return reinterpret_cast<link_type>(node)->value_field;
    }

    pointer operator->() const {
        return &(operator*());
    }

    // 前闭后开，最后一个元素可以++，然后为header
    self& operator++() {
        increment();
        return *this;
    }

    self operator++(int) {
        self temp = *this;
        increment();
        return temp;
    }
    //node不能为最小节点
    self& operator--() {
        decrement();
        return *this;
    }
    self operator--(int) {
        self temp = *this;
        decrement();
        return temp;
    }
};

template <class T, class Ref, class Ptr>
inline bool operator==(const rb_tree_iterator<T, Ref, Ptr>& lhs,
    const rb_tree_iterator<T, Ref, Ptr>& rhs) {
    return lhs.node == rhs.node;
}

template <class T, class Ref, class Ptr>
inline bool operator!=(const rb_tree_iterator<T, Ref, Ptr>& lhs,
    const rb_tree_iterator<T, Ref, Ptr>& rhs) {
    return lhs.node != rhs.node;
}
