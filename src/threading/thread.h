#pragma once

#include <cstdint>
#include <functional>

namespace crlib
{


class thread
{
public:
	static void sleep(unsigned int millis);
	static void yield();

	thread(std::function<void()> func);
	thread(const thread&) = delete;
	thread(thread&&) = delete;
	~thread();

	void start();
	void join();

	void* handle() { return m_handle; }

private:
	void* m_handle;
	std::function<void()> m_func;
	friend unsigned long __stdcall crlib_thread_method(void*);
};


} //namespace