#pragma once

#include <functional>
#include "condition_variable.h"

namespace crlib
{

enum class RW_LOCK_MODE
{
	//Used for writing to shared resources. Only 1 thread can acquire the lock
	EXCLUSLIVE,

	//Used for reading to shared resources. Multiple reader threads can acquire the lock
	SHARED
};

class rw_lock
{
public:
	rw_lock();
	rw_lock(const rw_lock&) = delete;
	rw_lock(rw_lock&&) = delete;

	template<RW_LOCK_MODE Mode = RW_LOCK_MODE::EXCLUSLIVE>
	void acquire();

	template<RW_LOCK_MODE Mode = RW_LOCK_MODE::EXCLUSLIVE>
	void release();

	template<RW_LOCK_MODE Mode = RW_LOCK_MODE::EXCLUSLIVE>
	void sleep(condition_variable& var);

	template<RW_LOCK_MODE Mode = RW_LOCK_MODE::EXCLUSLIVE>
	void sleep(condition_variable& var, std::function<bool()> predicate);

private:
	void* m_handle;
};

template<RW_LOCK_MODE Mode = RW_LOCK_MODE::EXCLUSLIVE>
class scoped_rw_lock
{
	typedef bool(*sleep_condition)();

public:
	scoped_rw_lock(rw_lock& lock) : m_lock(lock) { m_lock.acquire<Mode>(); }
	scoped_rw_lock(const scoped_rw_lock&) = delete;
	scoped_rw_lock(scoped_rw_lock&&) = delete;

	void sleep(condition_variable& var) { m_lock.sleep<Mode>(var); }
	void sleep(condition_variable& var, std::function<bool()> predicate) { m_lock.sleep<Mode>(var, predicate); }

	~scoped_rw_lock() { m_lock.release<Mode>(); }

private:
	rw_lock& m_lock;
};


} //namespace