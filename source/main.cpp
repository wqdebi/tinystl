//第一天测试数据
#include<exception>
#include <iostream>
#include"jjalloc.h"
#include<vector>
int main()
{
    int ia[5] = { 0,1,2,3,4 };
    unsigned int i;
    std::vector<int, JJ::allocator<int>> iv(ia, ia + 5);
    for (i = 0; i < 5; ++i)
    {
        std::cout << iv[i] << ' ';
    }
    std::cout << std::endl;

    JJ::allocator<int> x;
    x.allocate(11451400000019, nullptr);
}

