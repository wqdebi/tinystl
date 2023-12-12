#pragma once
#include"stl_iterator.h"
#include<algorithm>
#include"type_traits.h"
#include <string.h>
#include"stl_heap.h"
//判断相等
template<class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2) {
    for (; first1 != last1; ++first1, ++first2)
        if (*first1 != *first2)
            return false;
    return true;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, BinaryPredicate op) {
    for (; first1 != last1; ++first1, ++first2)
        if (!op(*first1, *first2))
            return false;
    return true;
}

//填值
template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last,
    const T& value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

//填n个值
template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T& value) {
    for (; n > 0; --n, ++first) {
        *first = value;
    }
    return *first;
}

//交换
template<class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap1(ForwardIterator1 a, ForwardIterator2 b) {
    __iter_swap(a, b, value_type(a));
}

//字典排序方式
template<class InputIterator1, class InputIterator2>
bool lexicofraphical_compare(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2)
            return true;
        if (*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template<class InputIterator1, class InputIterator2, class Compare>
bool lexicofraphical_compare(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, Compare comp) {
    for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2))
            return true;
        if (comp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template<class T>
inline const T& max(const T& a, const T& b) {
    return a < b ? b : a;
}

template<class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
    return comp(a, b) ? b : a;
}

template<class T>
inline const T& min(const T& a, const T& b) {
    return b < a ? b : a;
}

template<class T, class Compare>
inline const T& Max(const T& a, const T& b, Compare comp) {
    return comp(b, a) ? b : a;
}


inline bool lexicofraphical_compare(const unsigned char* first1,
    const unsigned char* last1, const unsigned char* first2,
    const unsigned char* last2) {
    const size_t len1 = last1 - first1;
    const size_t len2 = last2 - first1;
    const int result = memcmp(first1, first2, min(len1, len2));
    return result != 0 ? result < 0 : len1 < len2;
}

template<class InputIterator1, class InputIterator2>
std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
    InputIterator1 last1, InputIterator2 first2) {
    while (first1 != last1 && *first1 == *first2) {
        ++first1;
        ++first2;
    }
    return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class InputIterator1, class InputIterator2,
    class BinaryPredicate>
std::pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
    InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
    while (first1 != last1 && binary_pred(*first1, *first2)) {
        ++first1;
        ++first2;
    }
    return std::pair<InputIterator1, InputIterator2>(first1, first2);
}

//接下来是拷贝，采用了萃取和特化的方法，提升速度
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result,
    __true_type) {
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}
template<class T>
inline T* __copy_t(const T* first, const T* last, T* result,
    __false_type) {
    return __copy_d(first, last, result, (ptrdiff_t*)0);
}

template<class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last,
    OutputIterator result, input_iterator_tag) {
    for (; first != last; ++first, ++result) {
        *result = *first;
    }
    return result;
}

template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last,
    OutputIterator result, random_access_iterator_tag) {
    return __copy_d(first, last, result, distance_type(first));
}

template<class RandomAccessIterator, class OutputInterator, class Distance>
inline OutputInterator __copy_d(RandomAccessIterator first, RandomAccessIterator last,
    OutputInterator result, Distance*) {
    for (Distance n = last - first; n > 0; --n, ++result, ++first)
        *result = *first;
    return result;
}

template<class InputIterator, class OutputIterator>
struct __copy_dispatch {
    OutputIterator operator()(InputIterator first, InputIterator last,
        OutputIterator result) {
        return __copy(first, last, result, iterator_category(first));
    }
};

template<class T>
struct __copy_dispatch<T*, T*>
{
    T* operator()(T* first, T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template<class T>
struct __copy_dispatch<const T*, T*> {
    T* operator()(const T* first, const T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
    OutputIterator result) {
    return __copy_dispatch(first, last, result);
}

//为char进行的特化
inline char* copy(const char* first, const char* last, char* result) {
    memmove(result, first, last - first);
    return result + (last - first);
}

//为w_char*的特化
__inline wchar_t* copy(const wchar_t* first, const wchar_t* last,
    wchar_t* result) {
    memmove(result, first, last - first);
    return result + (last - first);
}

//构建并集
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        }
        else if (*first1 > *first2) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
            ++first2;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//构建交集
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2)
            ++first1;
        else if (*first2 < *first1)
            ++first2;
        else {
            *result = *first1;
            ++first1;
            ++first2;
            ++result;
        }
    }
    return result;
}

//构建差集
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        }
        else if (*first2 < *first1)
            ++first2;
        else {
            ++first1;
            ++first2;
        }
    }
    return copy(first1, last1, result);
}

//构造对称差集，即对于集合S1，S2得到(S1 - S2) U (S2 - S1)
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
            ++result;
        }
        else if (*first2 < *first1) {
            *result = *first2;
            ++result;
            ++first2;
        }
        else {
            ++first1;
            ++first2;
        }
    }
    return copy(first2, last2, copy(first1, last1, result));
}

//找出第一组满足条件的相邻元素
template<class ForwardIerator>
ForwardIerator adjacent_find(ForwardIerator first, ForwardIerator last) {
    if (first == last)return last;
    ForwardIerator next = first;
    while (++next != last) {
        if (*first == *next)
            return first;
        first = next;
    }
    return last;
}

template<class ForwardIerator, class BinaryPredicate>
ForwardIerator adjacent_find(ForwardIerator first, ForwardIerator last,
    BinaryPredicate binary_pred) {
    if (first == last)return last;
    ForwardIerator next = first;
    while (++next != last) {
        if (binary_pred(*first, *next))
            return first;
        first = next;
    }
    return last;
}

//计数
template<class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) {
        if (*first == value)
            ++n;
    }
    return n;
}

//条件计数
template<class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) {
        if (pred(*first))
            ++n;
    }
    return n;
}

//查找
template<class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last,
    const T& value) {
    while (first != last && *first != value)
        ++first;
    return first;
}

//条件查找
template<class InputIterator, class Predicate>
InputIterator find(InputIterator first, InputIterator last,
    Predicate pred) {
    while (first != last && !pred(*first))
        ++first;
    return first;
}

//在first1，last1区间内查找first2，last2最后出现的位置
template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 __ind_end(ForwardIterator1 first1,
    ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
    forward_iterator_tag, forward_iterator_tag) {
    if (first2 == last2)
        return last1;
    else {
        ForwardIterator1 result = last1;
        while (1) {
            ForwardIterator1 new_result = search(first1, last1, first2, last2);
            if (new_result == last1)
                return result;
            else {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}

template<class BidirectionaIterator1, class BidirectionalIterator2>
inline BidirectionaIterator1 __ind_end(BidirectionaIterator1 first1,
    BidirectionaIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2,
    bidirectional_iterator_tag, bidirectional_iterator_tag) {
    typedef reverse_iterator<BidirectionaIterator1> reviter1;
    typedef reverse_iterator<BidirectionalIterator2> reviter2;
    reviter1 rlast1(first1);
    reviter2 rlast2(first2);
    reviter1 rresult = search(reviter1(last1), rlast1,
        reviter2(last2), rlast2);
    if (rresult == rlast1)
        return last1;
    else {
        BidirectionaIterator1 result = rresult.base();
        advance(result, -distance(first2, last2));
        return result;
    }
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1,
    ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
    typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
    typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;
    return __find_end(first1, last1, first2, last2, category1(), category2());
}

template<class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
    ForwardIterator first2, ForwardIterator last2) {
    for (; first1 != last1; ++first1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter) {
            if (*first1 == *iter)
                return first1;
        }
    }
    return last1;
}

template<class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
    ForwardIterator first2, ForwardIterator last2, BinaryPredicate comp) {
    for (; first1 != last1; ++first1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter) {
            if (comp(*first1, *iter))
                return first1;
        }
    }
    return last1;
}

template<class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
    for (; first != last; ++first) {
        f(*first)
    }
    return f;
}

template<class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
    for (; first != last; ++first)
        *first = gen();
}

template<class OutputIterator, class Size, class Generator>
void generate_n(OutputIterator first, Size n, Generator gen) {
    for (; n > 0; --n, ++first)
        *first = gen();
    return first;
}

//判断区间二的每个元素值是否都在区间一内
template<class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2) {
    while (first1 != first2 && first2 != last2) {
        if (*first2 < *first1)
        {
            return false;
        }
        else if (*first1 < *first2) {
            ++first1;
        }
        else {
            ++first1;
            ++first2;
        }
    }
    return first2 == last2;
}

template<class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, Compare comp) {
    while (first1 != first2 && first2 != last2) {
        if (comp(*first2, *first1))
        {
            return false;
        }
        else if (comp(*first1, *first2)) {
            ++first1;
        }
        else {
            ++first1;
            ++first2;
        }
    }
    return first2 == last2;
}

template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first,
    ForwardIterator last) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while (++first != last)
        if (*result < *first)
            result = first;
    return result;
}

template<class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first,
    ForwardIterator last, Compare comp) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while (++first != last)
        if (comp(*result, *first))
            result = first;
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

template<class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while (++first != last)
        if (*first < *result)
            result = first;
    return result;
}

template<class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while (++first != last)
        if (comp(*first, *result))
            result = first;
    return result;
}

//partition将pred判定为TRUE的放到前面，判定为flase的放到后面
template<class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first,
    BidirectionalIterator last, Predicate pred) {
    while (true) {
        while (true)
        {
            if (first == last)
                return first;
            else if (pred(*first))
                ++first;
            else
                break;
            --last;
            while (true) {
                if (first == last)
                    return first;
                else if (!pred(*first))
                    --last;
                else
                    break;
            }
            iter_swap1(first, last);
            ++first;
        }
    }
}
//删除满足条件的值
template<class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
    OutputIterator result, const T& value) {
    for(; first != last; ++first)
        if (*first != value)
        {
            *result = *first;
            ++result;
        }
    return result;
}

template<class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy(InputIterator first, InputIterator last,
    OutputIterator result, Predicate pred) {
    for (; first != last; ++first)
        if (!pred(*first))
        {
            *result = *first;
            ++result;
        }
    return result;
}

template<class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
    const T& value) {
    first = find(first, last, value);
    ForwardIterator next = first;
    return first == last ? first : remove_copy(++next, last, first, value);
}

template<class ForwardIterator, class Predicate>
ForwardIterator remove(ForwardIterator first, ForwardIterator last,
    Predicate pred) {
    first = find_if(first, last, pred);
    ForwardIterator next = first;
    return first == last ? first : remove_copy_if(++next, last, first, pred);
}
