#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>




class ThreadPool {
private:
	std::queue<std::function<void()>> q; //queue of functions
	std::vector<std::thread> t;
	size_t threadNum;
	std::condition_variable cv;
	std::mutex q_mutex;
	bool IsEndOfWork = false;
public:
	ThreadPool() : threadNum(std::thread::hardware_concurrency()) { for (int i = 0; i < threadNum; i++) t.push_back(std::thread([this]() {work(); })); }
	~ThreadPool() { IsEndOfWork = true; for (auto& x : t) { if (x.joinable()) x.join(); }t.clear(); }

	size_t get_threadNum() { return threadNum; };

	void enque(std::function<void()> x);
	void deque(std::function<void()> x);
	std::thread::id get_id();
	void work();
	void waitAll();
};



