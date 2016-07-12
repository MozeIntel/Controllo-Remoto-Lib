#include "spin_lock.h"

//Don't include the entire Windows.h for the Yield function
extern "C"
{
	__declspec(dllimport) int __stdcall SwitchToThread();
}

void crlib::spin_lock::lock()
{
	while (m_lock.exchange(true, std::memory_order_acquire))
	{
		SwitchToThread();
	}
}

void crlib::spin_lock::unlock()
{
	m_lock.store(false, std::memory_order_release);
}
