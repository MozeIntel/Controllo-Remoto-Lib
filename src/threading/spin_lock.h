#pragma once

#include <atomic>

namespace crlib
{

/*
Busy wait lock. Has better performance than mutexes
(doesn't require context switching), but will actively 
use the cpu while waiting.
*/
class spin_lock
{
public:
	spin_lock() : m_lock(false) {}
	spin_lock(const spin_lock&) = delete;
	spin_lock(spin_lock&&) = delete;
	spin_lock& operator=(spin_lock) = delete;

	void lock();
	void unlock();

private:
	std::atomic<bool> m_lock;
};

} //namespace