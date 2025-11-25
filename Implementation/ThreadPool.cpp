#include "ThreadPool.h"

void ThreadPool::enque(std::function<void()> x) {
	{
		std::unique_lock<std::mutex> lock(q_mutex);
		q.push(std::move(x));
	}

	cv.notify_one();
};

void ThreadPool::deque(std::function<void()> x) { q.pop(); };

std::thread::id ThreadPool::get_id() { return std::this_thread::get_id(); }

void ThreadPool::work() {
	while (true) {


		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(q_mutex);
			cv.wait(lock, [this]() {return !q.empty() || IsEndOfWork; });

			if (IsEndOfWork == true)
				break;

			task = std::move(q.front());
			q.pop();
		}


		if (task) {
			task();
			cv.notify_one();
		}
	}
}

void ThreadPool::waitAll() {
	std::unique_lock<std::mutex> lock(q_mutex);
	cv.wait(lock, [this] {return q.empty(); });
}
