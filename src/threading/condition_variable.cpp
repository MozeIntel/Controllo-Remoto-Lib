#include "condition_variable.h"

#include <Windows.h>

crlib::condition_variable::condition_variable()
{
	InitializeConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(&m_handle));
}

void crlib::condition_variable::wake()
{
	WakeConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(&m_handle));
}

void crlib::condition_variable::wake_all()
{
	WakeAllConditionVariable(reinterpret_cast<PCONDITION_VARIABLE>(&m_handle));
}
