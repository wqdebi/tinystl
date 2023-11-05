

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

//day6
#include"stl_vector.h"
#include<iostream>
//using namespace std;
int main() {
    vector<int> a(5,5);
    std::cout << a[0] << std::endl;
    a.push_back(5);
    std::cout <<"a[0]:" << a[0] << std::endl;
    std::cout << "size:" << a.size() << std::endl;
    std::cout << "cap:" << a.capacity() << std::endl;
    a.push_back(*a.begin());
    for (int i = 0; i < a.size(); ++i)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    a.pop_back();
    std::cout << "size:" << a.size() << std::endl;
    std::cout << "cap:" << a.capacity() << std::endl;
    std::cout << "empty:" << a.empty();
    std::cout << std::endl;
    a[2] = 114;
    for (int i = 0; i < a.size(); ++i)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    auto it = a.end() - 1;
    a.insert(it, 2, 114);
    for (int i = 0; i < a.size(); ++i)
    {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
    a.clear();
    for (int i = 0; i < a.size(); ++i)
    {
        std::cout << a[i] << " ";
    }
    vector<int> b;
    b.push_back(1);
    b.push_back(2);
    b.push_back(3);
    b.push_back(4);
    b.push_back(5);
    auto it0 = b.begin();
    auto it1 = b.begin() + 3;
    b.erase(it0, it1);
    for (int i = 0; i < b.size(); ++i)
    {
        std::cout << b[i] << " ";
    }
    //a.erase(it);
    //a.resize(3);
    //for (int i = 0; i < a.size(); ++i)
    //{
    //    std::cout << a[i] << " ";
    //}
    return 0;
}
