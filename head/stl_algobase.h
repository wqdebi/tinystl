#pragma once
#include"stl_iterator.h"
#include<algorithm>
#include"type_traits.h"
#include <string.h>
#include <stdlib.h>
#include"stl_heap.h"
#define __stl_threshold 16
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
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
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
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
    Predicate pred) {
    first = find_if(first, last, pred);
    ForwardIterator next = first;
    return first == last ? first : remove_copy_if(++next, last, first, pred);
}

//替换将old值替换为新的值
template<class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last,
    const T& old_value, const T& new_value) {
    for (; first != last; ++first) {
        if (*first == old_value)
            *first = new_value;
    }
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
    OutputIterator result, const T& old_value, const T& new_value) {
    for (; first != last; ++first) {
        *result = *first == old_value ? new_value : *first;
    }
    return result;
}

template<class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last,
    Predicate pred, const T& new_value) {
    for (; first != last; ++first)
        if (pred(*first))
            *first = new_value;
}

template<class Iterator, class OutputIterator, class Predicate, class T>
OutputIterator replace_copy_if(Iterator first, Iterator last,
    OutputIterator result, Predicate pred, const T& new_value) {
    for (; first != last; ++first, ++result) {
        *result = pred(*first) ? new_value : *first;
    }
    return result;
}

//将元素在原来的容器中颠倒
template<class BidirectionalIterator>
inline void __reverse(BidirectionalIterator first,
    BidirectionalIterator last, bidirectional_iterator_tag) {
    while (true) {
        if (first == last || first == --last)
            return;
        else
            iter_swap1(first++, last);
    }
}

template<class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last,
    random_access_iterator_tag) {
    while (first < last)
        iter_swap1(first++, --last);
}

template<class BidirectionalIterator>
inline void reverse(BidirectionalIterator first,
    BidirectionalIterator last) {
    __reverse(first, last, iterator_category(first));
}

template<class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first,
    BidirectionalIterator last, OutputIterator result) {
    while (first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}

//最大公因子（辗转相除法）
template<class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
{
    while (n != 0) {
        EuclideanRingElement t = m % n;
        m = n;
        n = t;
    }
    return m;
}

//将[first, middle)和[middle,last)区间进行旋转
//例如 123456 按照3旋转则为345612
template<class ForwardIterator, class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle,
    ForwardIterator last, Distance*, forward_iterator_tag) {
    for (ForwardIterator i = middle;;) {
        iter_swap1(first, i);
        ++first;
        ++i;
        if (first == middle) {
            if (i == last)
                return;
            middle = i;
        }
        else if (i == last)
            i = middle;
    }
}

template<class BidrectionalIterator, class Distance>
void __rotate(BidrectionalIterator first, BidrectionalIterator middle,
    BidrectionalIterator last, Distance*, bidirectional_iterator_tag) {
    reverse(first, middle);
    reverse(middle, last);
    reverse(first, last);
}

template<class RandomAccessIterator, class Distance, class T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last,
    RandomAccessIterator initial, Distance shift, T*)
{
    T value = *initial;
    RandomAccessIterator ptr1 = initial;
    RandomAccessIterator ptr2 = ptr1 + shift;
    while (ptr2 != initial) {
        *ptr1 = *ptr2;
        ptr2 = ptr2;
        if (last - ptr2 > shift)
            ptr2 += shift;
        else
            ptr2 = first + (shift - (last - ptr2));
    }
    *ptr1 = value;
}

template<class RandomAccessIterator, class Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle,
    RandomAccessIterator last, Distance*,
    random_access_iterator_tag) {
    Distance n = __gcd(last - first, middle - first);
    while (n--) {
        __rotate_cycle(first, last, first + n, middle - first, value_type(first));
    }
}

template<class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle,
    ForwardIterator last) {
    if (first == middle || middle == last)
        return;
    __rotate(first, middle, last, distance_type(first),
        iterator_category(first));
}

template<class ForwardIterator, class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
    ForwardIterator last, OutputIterator result) {
    return copy(first, middle, copy(middle, last, result));
}

//在[first1, last1)内查找[first2, last2)首次出现点
template<class ForwardIterator1, class ForwardIterator2, class Distance1, class Distance2>
ForwardIterator1 __search(ForwardIterator1 first1,
    ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2,
    Distance1*, Distance2*) {
    Distance1 d1 = 0;
    distance(first1, last1, d1);
    Distance2 d2 = 0;
    distance(first2, last2, d2);
    if (d1 < d2)
        return last1;
    ForwardIterator1 current1 = first1;
    ForwardIterator2 current2 = first2;
    while(current2 != last2)
        if (*current1 == *current2) {
            ++current1;
            ++current2;
        }
        else
        {
            if (d1 == d2)
                return last1;
            else {
                current1 == ++first1;
                current2 = first2;
                --d1;
            }
        }
    return first1;
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1,
    ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
    return __search(first1, last1, first2, last2, distance_type(first1), distance_type(first2));
}

//查找n个value
template<class ForwardIterator, class Integer, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
    Integer count, const T& value) {
    if (count <= 0)
        return first;
    else {
        first = find(first, last, value);
        while (first != last) {
            Integer n = count - 1;
            ForwardIterator i = first;
            ++i;
            while (i != last && n != 0 && *i == value)
            {
                ++i;
                --n;
            }
            if (n == 0)
                return first;
            else
                first = find(i, last, value);
        }
        return last;
    }
}

template<class ForwardIterator, class Integer, class T, class BidnaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
    Integer count, const T& value, BidnaryPredicate binary_pred) {
    if (count <= 0)
        return first;
    else {
        while (first != last) {
            if (binary_pred(*first, value))break;
            ++first;
        }
        while (first != last)
        {
            Integer n = count - 1;
            ForwardIterator i = first;
            ++i;
            while (i != last && n != 0 && binary_pred(*i, value)) {
                ++i;
                --n;
            }
            if (n == 0)
                return first;
            else {
                while (i != last)
                {
                    if (binary_pred(*i, value))
                        break;
                    ++i;
                }
                first = i;
            }
        }
        return last;
    }
}

//将[first1,last1)区间内的元素与first2开始的等长区间的元素进行互换
template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
    ForwardIterator2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        iter_swap1(first1, first2);
    }
    return first2;
}

//将仿函数（单参数和双参数）作用在区间所有值上
template<class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last,
    OutputIterator result, UnaryOperation op) {
    for (; first != last; ++first, ++result)
        *result = op(*first);
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
    InputIterator2 first2, OutputIterator result, BinaryOperation op) {
    for (; first != last; ++first1, ++first2, ++result)
        *result = op(*first1, *first2);
    return result;
}

//移除重复元素
template<class InputIterator, class OutputIterator, class T>
OutputIterator __unique_copy(InputIterator first, InputIterator last,
    OutputIterator result, T*) {
    T value = *first;
    *result = value;
    while (++first != last) {
        if (value != *first)
        {
            value = *first;
            *++result = value;
        }
    }
    return ++result;
}

template<class InputIterator, class OutputIterator>
inline OutputIterator __unique_copy(InputIterator first,
    InputIterator last, OutputIterator result, output_iterator_tag) {
    return __unique_copy(first, last, result, value_type(first));
}

template<class InputIterator, class ForwardIterator>
ForwardIterator __unique_copy(InputIterator first, InputIterator last,
    ForwardIterator result, forward_iterator_tag) {
    *result = *first;
    while (++first != last)
        if (*result != *first)
            *++result = *first;
    return ++result;
}

template<class InputIterator, class OutputIterator>
inline OutputIterator unique_copy(InputIterator first, InputIterator last,
    OutputIterator result) {
    if (first == last)
        return result;
    return __unique_copy(first, last, result, iterator_category(result));
}

template<class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
    first = adjacent_find(first, last);
    return unique_copy(first, last, first);
}

//二分查找，分别找第一个大于、小于的元素
template<class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first,
    ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;
    while(len > 0){
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (*middle < value) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

template<class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first,
    RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first + half;
        if (*middle < value) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

template<class ForwardIterator, class T, class Compare, class Distance>
ForwardIterator __lower_bound(ForwardIterator first,
    ForwardIterator last, const T& value, Compare comp, Distance*, forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(*middle, value)) {
            first = middle;
            ++first;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

template<class RandomAccessIterator, class T, class Compare, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first,
    RandomAccessIterator last, const T& value, Compare comp, Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first + half;
        if (comp(*middle, value)) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}

template<class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
    const T& value) {
    return __lower_bound(first, last, value, distance_type(first),
        iterator_category(first));
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
    const T& value, Compare comp) {
    return __lower_bound(first, last, value, comp, distance_type(first),
        iterator_category(first));
}

template<class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first,
    ForwardIterator last, const T& value, Distance*, forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (value < *middle) {
            len = half;
        }
        else
        {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

template<class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first,
    RandomAccessIterator last, const T& value, Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first + half;
        if (value < *middle) {
            len = half;
        }
        else {
            first = middle + 1;
            len = len - half - 1;
        }
    }
    return first;
}

template<class ForwardIterator, class T, class Compare, class Distance>
ForwardIterator __upper_bound(ForwardIterator first,
    ForwardIterator last, const T& value, Compare comp, Distance*, forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;
    while (len > 0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if (comp(value， *middle)) {
            len = half;
        }
        else {
            first = middle;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

template<class RandomAccessIterator, class T, class Compare, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first,
    RandomAccessIterator last, const T& value, Compare comp, Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;
    while (len > 0)
    {
        half = len >> 1;
        middle = first + half;
        if (comp(value， * middle))) {
            len = half;
        }
        else {
            first = middle + 1;
            len = len - half - 1;
        } 
    }
    return first;
}

template<class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
    const T& value) {
    return __upper_bound(first, last, value, distance_type(first),
        iterator_category(first));
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
    const T& value, Compare comp) {
    return __upper_bound(first, last, value, comp, distance_type(first),
        iterator_category(first));
}

template<class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last,
    const T& value) {
    ForwardIterator i = lower_bound(first, last, value);
    return i != last && !(value < *i);
}

template<class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last,
    const T& value, Compare comp) {
    ForwardIterator i = lower_bound(first, last, value, comp);
    return i != last && !comp(value, i);
}

//下一个排序
template<class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
{
    if (first == last)
        return false;
    BidirectionalIterator i = first;
    ++i;
    if (i == last)
        return false;
    i = last;
    --i;
    for (;;) {
        BidirectionalIterator ii = i;
        --i;
        if (*i < *ii)
        {
            BidirectionalIterator j = last;
            while (!(*i < *--j));
            iter_swap1(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

// 前一个排序
template<class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last)
{
    if (first == last)
        return false;
    BidirectionalIterator i = first;
    ++i;
    if (i == last)
        return false;
    i = last;
    --i;
    for (;;) {
        BidirectionalIterator ii = i;
        --i;
        if (*ii < *i)
        {
            BidirectionalIterator j = last;
            while (!(*--j < *i));
            iter_swap1(i, j);
            reverse(ii, last);
            return true;
        }
        if (i == first) {
            reverse(first, last);
            return false;
        }
    }
}

//随机打乱
template<class RandomAccessIterator, class Distance>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
    Distance*) {
    if (first == last)
        return;
    srand((unsigned int)time(0));
    for (RandomAccessIterator i = first + 1; i != last; ++i)
        iter_swap1(i, first + Distance(rand() % ((i - first) + 1)));
}

template<class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first,
    RandomAccessIterator last) {
    __random_shuffle(first, last, distance_type(first));
}

template<class RandomAccessIterator, class RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
    RandomAccessIterator& rand) {
    if (first == last)
        return;
    srand((unsigned int)time(0));
    for (RandomAccessIterator i = first + 1; i != last; ++i)
        iter_swap1(i, first + rand((i - first) + 1));
}

//找出前middle-first个最小元素
template<class RandomAccessIterator, class T>
void __partial_sort(RandomAccessIterator first,
    RandomAccessIterator middle, RandomAccessIterator last, T*) {
    make_heap(first, middle);
    for (RandomAccessIterator i = middle; i < last; ++i)
        if (*i < *first)
            __pop_heap(first, middle, i, T(*i), distance_type(first));
    sort_heap(first, middle);
}

template<class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first,
    RandomAccessIterator middle, RandomAccessIterator last) {
    __partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
        RandomAccessIterator last, T*);
}

//插入排序
template<class RandomAccessIterator, class T>
void __unguarded_linear_insert(RandomAccessIterator last, T value)
{
    RandomAccessIterator next = last;
    --next;
    while (value < *next) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

template<class RandomAccessIterator, class T>
inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    T value = *last;
    if (value < *first) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value);
}

template<class RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first,
    RandomAccessIterator last) {
    if (first == last)
        return;
    fpr(RandomAccessIterator i = first + 1; i != last; ++i)
        __linear_insert(first, i, value_type(first));
}

//三点取中值
template<class T>
inline const T& __median(const T& a, const T& b, const T& c)
{
    if (a < b)
        if (b < c)
            return b;
        else if (a < c)
            return c;
        else
            return a;
    else if (a < c)
        return a;
    else if (b < c)
        return c;
    else
        return b;
}

//分割，大于一个阈值的在右边，小于的在左边
template<class RandomAccessIterator, class T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first,
    RandomAccessIterator last, T pivot)
{
    while (true) {
        while (*first < pivot)
            ++first;
        --last;
        while (pivot < *last)
            --last;
        if (!(first < last))
            return fisrt;
        iter_swap1(first, last);
        ++first;
    }
}
//排序
template<class Size>
inline Size __lg(Size n)
{
    Size k;
    for (k = 0; n > 1; n >> 1)
        ++k;
    return k;
}

template <class RandomAccessIterator, class T, class Compare>
void __unguarded_insertion_sort_aux(RandomAccessIterator first,
    RandomAccessIterator last,
    T*, Compare comp) {
    for (RandomAccessIterator i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}

template <class RandomAccessIterator>
inline void __unguarded_insertion_sort(RandomAccessIterator first,
    RandomAccessIterator last) {
    __unguarded_insertion_sort_aux(first, last, value_type(first));
}

template <class RandomAccessIterator>
void __final_insertion_sort(RandomAccessIterator first,
    RandomAccessIterator last) {
    if (last - first > __stl_threshold) {
        __insertion_sort(first, first + __stl_threshold);
        __unguarded_insertion_sort(first + __stl_threshold, last);
    }
    else
        __insertion_sort(first, last);
}

template <class RandomAccessIterator, class T, class Size>
void __introsort_loop(RandomAccessIterator first,
    RandomAccessIterator last, T*,
    Size depth_limit) {
    while (last - first > __stl_threshold) {
        if (depth_limit == 0) {
            partial_sort(first, last, last);
            return;
        }
        --depth_limit;
        RandomAccessIterator cut = __unguarded_partition
        (first, last, T(__median(*first, *(first + (last - first) / 2),
            *(last - 1))));
        __introsort_loop(cut, last, value_type(first), depth_limit);
        last = cut;
    }
}

template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
    if (first != last) {
        __introsort_loop(first, last, value_type(first), __lg(last - first) * 2);
        __final_insertion_sort(first, last);
    }
}
