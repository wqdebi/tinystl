

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
#include<exception>
#include <iostream>
//#include "std_construct.h"
//#include"jjalloc.h"
#include"std_alloc.h"
#include<vector>
int main()
{
    alloc x;
    //二级分配器
    auto* p= x.allocate(11);
    auto* pp = x.allocate(11);
    //一级分配器
    auto* ppp = x.allocate(114514);
    //A* p= new A[3];
    return 0;
}
