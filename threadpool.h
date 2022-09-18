#pragma once

#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <condition_variable>

class Threadpool
{
public:
	Threadpool(const size_t threadCount);
	~Threadpool();
	Threadpool(const Threadpool&) = default;
	Threadpool& operator=(const Threadpool&) = default;
	Threadpool(Threadpool&&) = default;
	Threadpool& operator=(Threadpool&&) = default;
	
	template<class F, class... Args>
	void enqueue(F&& f, Args&&... args);

private:
	std::vector<std::thread> m_Workers;
	std::queue<std::function<void()>> m_Tasks;
	std::mutex m_Mutex;
	std::condition_variable m_Condition;
	
	bool m_Stop = false;

private:
	void process();
};

inline Threadpool::Threadpool(const size_t threadCount)
{
	for (int i = 0; i < threadCount; i++)
	{
		m_Workers.emplace_back(&Threadpool::process, this);
	}
}

inline Threadpool::~Threadpool()
{
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);

		m_Stop = true;
	}

	m_Condition.notify_all();

	for (std::thread& worker : m_Workers)
		worker.join();
}

template<class F, class... Args>
inline void Threadpool::enqueue(F&& f, Args&&... args)
{
	{
		std::scoped_lock<std::mutex> lock(m_Mutex);
		m_Tasks.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	}

	m_Condition.notify_one();
}

inline void Threadpool::process()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [this] { return m_Stop || !m_Tasks.empty(); });

		if (m_Stop && m_Tasks.empty())
			break;

		std::function<void()> task = m_Tasks.front();
		m_Tasks.pop();
		lock.unlock();

		task();
	}
}