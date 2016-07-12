#pragma once

namespace crlib
{

class condition_variable
{
public:
	condition_variable();
	condition_variable(const condition_variable&) = delete;
	condition_variable(condition_variable&&) = delete;

	void wake();
	void wake_all();

	void** handle() { return &m_handle; }

private:
	void* m_handle;
};


} //namespace