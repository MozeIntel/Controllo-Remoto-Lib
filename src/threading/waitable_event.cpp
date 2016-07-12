#include "waitable_event.h"

#include <stdexcept>
#include <Windows.h>

crlib::waitable_event::waitable_event(bool manual_reset, bool start_signaled) :
	m_handle(CreateEvent(nullptr, manual_reset, start_signaled, nullptr))
{
	if (m_handle == nullptr)
	{
		throw std::runtime_error("CreateEven returned nulllptr");
	}
}

crlib::waitable_event::~waitable_event()
{
	CloseHandle(m_handle);
}

void crlib::waitable_event::signal()
{
	SetEvent(m_handle);
}

void crlib::waitable_event::reset()
{
	ResetEvent(m_handle);
}

void crlib::waitable_event::wait()
{
	WaitForSingleObject(m_handle, INFINITE);
}

void crlib::waitable_event::wait(unsigned int millis)
{
	WaitForSingleObject(m_handle, millis);
}
