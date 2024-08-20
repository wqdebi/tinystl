#pragma once
#include "function_wrapper.h"
#include <deque>
#include <mutex>

class work_stealing_queue {
private:
	typedef function_wrapper data_type;
	std::deque<data_type> the_deque;
	mutable std::mutex the_mutex;

public:
	work_stealing_queue() {}
	work_stealing_queue(const work_stealing_queue& other) = delete;
	work_stealing_queue& operator=(const work_stealing_queue& ohter) = delete;
	void push(data_type data) {
		std::lock_guard<std::mutex> lock(the_mutex);
		the_deque.push_front(std::move(data));
	}
	bool empty() const {
		std::lock_guard<std::mutex> lock(the_mutex);
		return the_deque.empty();
	}
	bool try_pop(data_type& res) {
		std::lock_guard<std::mutex> lock(the_mutex);
		if (the_deque.empty()) {
			return false;
		}
		res = std::move(the_deque.front());
		the_deque.pop_front();
		return true;
	}
	bool try_steal(data_type& res) {
		std::lock_guard<std::mutex> lock(the_mutex);
		if (the_deque.empty()) {
			return false;
		}
		res = std::move(the_deque.front());
		the_deque.pop_front();
		return true;
	}
	int size() {
		std::lock_guard<std::mutex> lock(the_mutex);
		return the_deque.size();
	}
};