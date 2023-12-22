#include<iostream>
#include<thread>
#include"threadsafe_queue.h"
#include<atomic>
#include<mutex>
std::mutex mut;
std::atomic<int> atm = 0;
threadsafe_queue<int> que;

void test_push()
{
	for (int i = 0; i < 50; ++i)
		que.push(i);
	atm = 1;
}

void test_try_pop_1()
{
	while (true)
	{
		auto it = que.try_pop();
		if (it)
		{
			std::lock_guard<std::mutex> lk(mut);
			std::cout << "test_try_pop_1: " << *it << std::endl;
		}
		if (atm == 1 && que.empty())
			break;
	}
}

void test_try_pop_2()
{
	while (true)
	{
		int T;
		bool x = que.try_pop(T);
		if (x)
		{
			std::lock_guard<std::mutex> lk(mut);
			std::cout << "test_try_pop_2: " << T << std::endl;
		}
		if (atm == 1 && que.empty())
			break;
	}
}

void test_wait_and_pop_1()
{
	while (true)
	{
		auto it = que.wait_and_pop();
		std::lock_guard<std::mutex> lk(mut);
		std::cout << "test_wait_and_pop_1: " << *it << std::endl;
		if (atm == 1 && que.empty())
			break;
	}
}

void test_wait_and_pop_2()
{
	while (true)
	{
		int T;
		que.wait_and_pop(T);
		std::lock_guard<std::mutex> lk(mut);
		std::cout << "test_wait_and_pop_2: " << T << std::endl;
		if (atm == 1 && que.empty())
			break;
	}
}

int main()
{
	std::thread t1(test_try_pop_1);
	std::thread t2(test_try_pop_2);
	std::thread t3(test_wait_and_pop_1);
	std::thread t4(test_wait_and_pop_2);
	test_push();
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return 0;
}