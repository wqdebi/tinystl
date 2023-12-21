#pragma once
#include<exception>
#include<memory>
#include<mutex>
#include<stack>
struct empty_stack : std::exception
{
	const char* what() const throw() {
		return "Stack is empty";
	}
};

template<typename T>
class threadsafe_stack {
private:
	std::stack<T> data;
	//即使是const修饰的threadsafe_stack也可以修改m变量
	mutable std::mutex m;
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack<T>& other) {
		std::lock_guard<std::mutex> lock(m);
		data = other.data;
	}
	threadsafe_stack<T>& operator=(const threadsafe_stack<T>&) = delete;
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(std::move(new_value));
	}
	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw empty_stack();
		value = data.top();
		data.pop();
	}
	bool empty()
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};