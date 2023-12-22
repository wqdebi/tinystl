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

	//�ڶ�β����Ԫ�أ���ͨ���������������������ϵ������߳�
	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(new_value));
		data_cond.notify_one();
	}

	//��ͷ����Ԫ�أ��ú�����һ����������������ܵ���Ԫ��,�ú����������ġ�
	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		value = std::move(data_queue.front());
		data_queue.pop();
	}

	//��ͷ����Ԫ�أ��ú����з���һ��ֻ��ָ�����������Ԫ��,�ú����������ġ�
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		std::shared_ptr<T> output(std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return output;
	}

	//��ͷ����Ԫ�أ��ú�����һ����������������ܵ���Ԫ��,�ú����Ƿ������ģ�����ֵ���������Ƿ�pop�ɹ���
	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	//��ͷ����Ԫ�أ��ú����з���һ��ֻ��ָ�����������Ԫ��,�ú����Ƿ������ģ�����ֵ���������Ƿ�pop�ɹ���
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