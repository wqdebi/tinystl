#pragma once
#include<queue>
#include<memory>
#include<mutex>
#include<condition_variable>

template<class T>
class threadsafe_queue {
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	threadsafe_queue() {}

	threadsafe_queue(threadsafe_queue<T> const& other)
	{
		std::lock_guard<std::mutex> lk(other.mut);
		data_queue = other.data_queue;
	}

	threadsafe_queue<T>& operator=(const threadsafe_queue<T>&) = delete;

	//在队尾插入元素，并通过唤醒阻塞在条件变量上的其他线程
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(new_value));
		data_cond.notify_one();
	}

	//队头弹出元素，该函数有一个输出参数用来接受弹出元素,该函数是阻塞的。
	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		value = std::move(data_queue.front());
		data_queue.pop();
	}

	//队头弹出元素，该函数有返回一个只能指针包裹弹出的元素,该函数是阻塞的。
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		std::shared_ptr<T> output(std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return output;
	}

	//队头弹出元素，该函数有一个输出参数用来接受弹出元素,该函数是非阻塞的，返回值用来返回是否pop成功。
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	//队头弹出元素，该函数有返回一个只能指针包裹弹出的元素,该函数是非阻塞的，返回值用来返回是否pop成功。
	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> output(std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return output;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};