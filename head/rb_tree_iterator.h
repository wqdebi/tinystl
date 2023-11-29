#pragma once

#include <cstddef>
#include "rb_tree_node.h"
#include "stl_iterator.h"


struct rb_tree_base_iterator {
    using base_ptr = __rb_tree_node_base::base_ptr;
    using iterator_category = bidirectional_iterator_tag;
    using difference_type = ptrdiff_t;

    base_ptr node;
    // �����ز�����++ʹ�ã�node������ֱ��Ϊheader
    // ˳���ƶ�����һ��λ��
    void increment() {
        // �����ҽڵ㣬����һ�ڵ��Ϊ�������������½�
        if (node->right) {
            node = node->right;
            while (node->left) node = node->left;
        }
        else { // û���ҽڵ㣬��Ӧ���ǲ������ݣ���һ����Ϊ���ӽڵ������
            while (node->parent->right == node) // �����ڵ�root����headerҲ������ѭ��
                node = node->parent;

            // ���ұ߽ڵ����һ���ڵ�Ӧ����header
            // ֻ��һ�����node�ᵽheader�����ǵ�����ָ��root���������ҽڵ�
            if (node->right != node->parent) node = node->parent;
            // ��node����headerʱ���͵���header
        }
    }

    // �����ز�����--ʹ�ã�
    // ǰһ��λ��
    void decrement() {
        // header���Ϊ���һ���ڵ�
        // ��nodeΪheaderʱ
        if (node->color == rb_tree_red && node->parent->parent == node) {
            node = node->right;
        }
        else if (node->left) // �������ӽڵ�
        {
            base_ptr l = node->left;
            while (l->right) l = l->right;
            node = l;
        }
        else { // ��header��������
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

    // ǰ�պ󿪣����һ��Ԫ�ؿ���++��Ȼ��Ϊheader
    self& operator++() {
        increment();
        return *this;
    }

    self operator++(int) {
        self temp = *this;
        increment();
        return temp;
    }
    //node����Ϊ��С�ڵ�
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
