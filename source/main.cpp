

//day1
//int main()
//{
    /*int ia[5] = { 0,1,2,3,4 };
    unsigned int i;
    std::vector<int, JJ::allocator<int>> iv(ia, ia + 5);
    for (i = 0; i < 5; ++i)
    {
        std::cout << iv[i] << ' ';
    }
    std::cout << std::endl;*/
//}
//day 2
//class A {
//private:
//    int x;
//public:
//    A() {};
//    A(int xx) :x(xx) {};
//};

//day3
//#include<exception>
//#include <iostream>
////#include "std_construct.h"
////#include"jjalloc.h"
//#include"std_alloc.h"
//#include<vector>
//int main()
//{
//    alloc x;
//    //二级分配器
//    auto* p= x.allocate(11);
//    auto* pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//    p = x.allocate(11);
//    pp = x.allocate(11);
//
//    //一级分配器
//    auto* ppp = x.allocate(114514);
//    //A* p= new A[3];
//    return 0;
//}

////day5
//#include<iostream>
//using namespace std;
//struct B {};
//struct D1: public B {};
//struct D2: public D1 {};
//template<typename I>
//void func(I& p, B) {
//    cout << "B" << endl;;
//}
//template<typename I>
//void func(I& p, D2) {
//    cout << "D2" << endl;
//}
//int main()
//{
//    int* p;
//    func(p, B());//调用void func(I& p, B)
//    func(p, D1());//调用void func(I & p, B)
//    func(p, D2());//调用void func(I& p, D2)
//}

////day6
//#include"stl_vector.h"
//#include<iostream>
////using namespace std;
//int main() {
//    vector<int> a(5,5);
//    std::cout << a[0] << std::endl;
//    a.push_back(5);
//    std::cout <<"a[0]:" << a[0] << std::endl;
//    std::cout << "size:" << a.size() << std::endl;
//    std::cout << "cap:" << a.capacity() << std::endl;
//    a.push_back(*a.begin());
//    for (int i = 0; i < a.size(); ++i)
//    {
//        std::cout << a[i] << " ";
//    }
//    std::cout << std::endl;
//    a.pop_back();
//    std::cout << "size:" << a.size() << std::endl;
//    std::cout << "cap:" << a.capacity() << std::endl;
//    std::cout << "empty:" << a.empty();
//    std::cout << std::endl;
//    a[2] = 114;
//    for (int i = 0; i < a.size(); ++i)
//    {
//        std::cout << a[i] << " ";
//    }
//    std::cout << std::endl;
//    auto it = a.end() - 1;
//    a.insert(it, 2, 114);
//    for (int i = 0; i < a.size(); ++i)
//    {
//        std::cout << a[i] << " ";
//    }
//    std::cout << std::endl;
//    a.clear();
//    for (int i = 0; i < a.size(); ++i)
//    {
//        std::cout << a[i] << " ";
//    }
//    vector<int> b;
//    b.push_back(1);
//    b.push_back(2);
//    b.push_back(3);
//    b.push_back(4);
//    b.push_back(5);
//    auto it0 = b.begin();
//    auto it1 = b.begin() + 3;
//    b.erase(it0, it1);
//    for (int i = 0; i < b.size(); ++i)
//    {
//        std::cout << b[i] << " ";
//    }
//    std::cout << std::endl;
//    std::cout << b.front() << std::endl;
//    std::cout << b.back() << std::endl;
//    //a.erase(it);
//    //a.resize(3);
//    //for (int i = 0; i < a.size(); ++i)
//    //{
//    //    std::cout << a[i] << " ";
//    //}
//    return 0;
//}
//day 8
//#include<deque>
//#include<iostream>
//#include"stl_list.h"
//int main()
//{
//    list<int> a;
    //std::cout << a.size();
    //a.push_back(8);
    //a.push_front(9);
    //a.push_back(8);
    //a.push_front(9);
    //a.pop_back();
    //a.pop_front();
    //std::cout << a.front() << std::endl;
    //auto it = a.begin();
    //std::cout << *it;
    //a.push_back(1);
    //a.push_back(2);
    //a.push_back(3);
    //list<int> b;
    //b.push_back(1);
    //b.push_back(2);
    //b.push_back(3);
    //a.merge(b);
    //for (auto it = a.begin(); it != a.end(); ++it)
    //    std::cout << *it << " ";
    //std::cout << std::endl;
    //a.reverse();
    //for (auto it = a.begin(); it != a.end(); ++it)
    //    std::cout << *it << " ";
    //std::cout << std::endl;
    //a.unique();
    //for (auto it = a.begin(); it != a.end(); ++it)
    //    std::cout << *it << " ";
    //std::cout << std::endl;
    //std::cout << a.size();
    // 
    //list<int> c;
    //c.push_back(2);
    //c.push_back(1);
    //c.push_back(3);
    //c.push_back(0);
    //c.push_back(5);
    //c.sort();
    //for (auto it = c.begin(); it != c.end(); ++it)
    //    std::cout << *it << " ";
    //std::cout << std::endl;


    //auto it = c.begin();
    //std::cout << *it << std::endl; 
    //for (auto it1 = a.begin(), it2 = a.end(); it1 != it2; ++it1)
    //{
    //    std::cout << *it1 << std::endl;
    //}
//}
//day 10
#include"stl_deque.h"
#include<iostream>
int main() {
    deque<int> d(5,3);
    std::cout << d.size() << std::endl;
    std::cout << d[2] << std::endl;
    for (auto it = d.begin(); it != d.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    deque<int> c;
    std::cout << c.empty() << std::endl;
    for(int i = 0; i < 100; ++i)
        c.push_back(114);
    std::cout << c.size() << std::endl;
    std::cout << c[90] << std::endl;
    c.pop_back();
    std::cout << c.size() << std::endl;
    c.push_front(5);
    std::cout << c[0] << std::endl;
    c.pop_front();
    std::cout << c[0] << std::endl;
    std::cout << c.front() << std::endl;
    c.push_front(5);
    c.push_front(5);
    c.push_back(1484);
    for (auto it = c.begin(); it != c.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
    c.erase(c.begin(), c.end());
    std::cout << c.size() << std::endl;
    return 0;
}
