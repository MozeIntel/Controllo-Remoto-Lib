#include "byte_buffer_wrapper.h"

#include <utility>

crlib::byte_buffer_wrapper::byte_buffer_wrapper(uint8_t* buffer, size_t size) :
	m_buffer(buffer),
	m_size(size),
	m_write_index(0),
	m_read_index(0)
{
}

crlib::byte_buffer_wrapper::byte_buffer_wrapper(const byte_buffer_wrapper& other) :
	m_buffer(other.m_buffer),
	m_size(other.m_size),
	m_write_index(other.m_write_index),
	m_read_index(other.m_read_index)
{
}

crlib::byte_buffer_wrapper::byte_buffer_wrapper(byte_buffer_wrapper&& other) :
	m_buffer(nullptr),
	m_size(0),
	m_write_index(0),
	m_read_index(0)
{
	swap(*this, other);
}

crlib::byte_buffer_wrapper& crlib::byte_buffer_wrapper::operator=(byte_buffer_wrapper other)
{
	swap(*this, other);
	return *this;
}
