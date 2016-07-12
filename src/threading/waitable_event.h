#pragma once

namespace crlib
{

class waitable_event
{
public:
	waitable_event(bool manual_reset = false, bool start_signaled = false);
	~waitable_event();

	void signal();
	void reset();

	void wait();
	void wait(unsigned int millis);

private:
	void* m_handle;
};


} //namespace