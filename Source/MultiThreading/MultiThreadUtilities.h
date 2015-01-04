#pragma once

#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class concurrent_queue
{
public:
	void push(T const& data)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		m_queue.push(data);
		mlock.unlock();
		m_cond.notify_one();
	}

	bool empty() const
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		return m_queue.empty();
	}

	bool try_pop(T& popped_val)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		if(m_queue.empty()) {
			return false;
		}

		popped_val = m_queue.front();
		m_queue.pop();
		return true;
	}

	void wait_and_pop(T& popped_val)
	{
		std::unique_lock<std::mutex> mlock(m_mutex);
		while(m_queue.empty())
		{
			m_cond.wait(mlock);
		}

		popped_val = m_queue.front();
		m_queue.pop();
	}
private:
	std::mutex m_mutex;
	std::queue<T> m_queue;
	std::condition_variable m_cond;
};