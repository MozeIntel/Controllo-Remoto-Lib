#include "thread.h"

#include <stdexcept>
#include <Windows.h>

void crlib::thread::sleep(unsigned int millis)
{
	Sleep(millis);
}

void crlib::thread::yield()
{
	SwitchToThread();
}

crlib::thread::thread(std::function<void()> func) :
	m_handle(CreateThread(nullptr, 0, &crlib::crlib_thread_method, reinterpret_cast<void*>(this), CREATE_SUSPENDED, nullptr)),
	m_func(func)
{
	if (m_handle == nullptr)
	{
		throw std::runtime_error("Thread creation failed");
	}
}

crlib::thread::~thread()
{
	CloseHandle(m_handle);
}

void crlib::thread::start()
{
	ResumeThread(m_handle);
}

void crlib::thread::join()
{
	WaitForSingleObject(m_handle, INFINITE);
}

unsigned long __stdcall crlib::crlib_thread_method(void* param)
{
	crlib::thread* t = reinterpret_cast<crlib::thread*>(param);
	t->m_func();
	return 0;
}