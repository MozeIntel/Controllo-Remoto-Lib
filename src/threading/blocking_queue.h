#pragma once

#include <queue>
#include "rw_lock.h"

namespace crlib
{

template<typename T>
class blocking_queue
{
public:
	blocking_queue() : m_queue(), m_lock(), m_cond_var() {}

	void push(const T& value);
	T pop();

	void size();

private:
	std::queue<T> m_queue;
	rw_lock m_lock;
	condition_variable m_cond_var;
};

template<typename T>
inline void blocking_queue<T>::push(const T& value)
{
	{
		scoped_rw_lock<RW_LOCK_MODE::EXCLUSLIVE> lock(m_lock);
		m_queue.push(value);
	}

	m_cond_var.wake();
}

template<typename T>
inline T blocking_queue<T>::pop()
{
	scoped_rw_lock<RW_LOCK_MODE::EXCLUSLIVE> lock(m_lock);
	lock.sleep(m_cond_var, [this]() { return !m_queue.empty(); });

	T next = m_queue.front();
	m_queue.pop();
	return next;
}

template<typename T>
inline void blocking_queue<T>::size()
{
	scoped_rw_lock<RW_LOCK_MODE::SHARED> lock(m_lock);
	return m_queue.size();
}

} //namespace