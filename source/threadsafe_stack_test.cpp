#include<iostream>
#include<thread>
#include<atomic>
#include"threadsafe_stack.h"
threadsafe_stack<int> stk;
std::atomic<int> atm = 0;
void f1()
{
    for (int i = 0; i < 10; ++i)
        stk.push(i);
    atm = 1;
}
void f2()
{
    while (true)
    {
        int tmp;
        try {
            stk.pop(tmp);
            std::cout << tmp << std::endl;
        }
        catch (...){
            if (atm == 1)
                break;
        }
    }
}

int main()
{
    //try {
    //    throw empty_stack();
    //}
    //catch (const std::exception& e) {
    //    std::cout << "Caught exception: " << e.what() << std::endl;
    //}

    //threadsafe_stack<int> x;
    //threadsafe_stack<int> y(x);

    std::thread thd1(f1);
    std::thread thd2(f2);
    thd1.join();
    thd2.join();
	return 0;
}