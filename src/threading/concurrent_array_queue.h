#pragma once

#include <atomic>
#include <stdexcept>

namespace crlib
{

/*
A thread safe queue (FIFO) implemented as a circular buffer.
*/
template<typename T, size_t Size>
class concurrent_array_queue
{
public:
	concurrent_array_queue() :
		m_read_index(0),
		m_write_index(0),
		m_size(0)
	{
	}

	void push(const T& data);
	T& pop();

	size_t size() const { return m_size.load(); }
	constexpr size_t capacity() const { return Size; }

private:
	T m_data[Size];
	std::atomic<size_t> m_read_index;
	std::atomic<size_t> m_write_index;
	std::atomic<size_t> m_size;

};


template<typename T, size_t Size>
inline void concurrent_array_queue<T, Size>::push(const T & data)
{
	size_t write_index;

	do
	{
		write_index = m_write_index.load();
	} while (!m_write_index.compare_exchange_weak(write_index, write_index + 1));

	if (++m_size > Size)
	{
		throw std::runtime_error("Out of memory!");
	}

	m_data[write_index % Size] = data;
}

template<typename T, size_t Size>
inline T& concurrent_array_queue<T, Size>::pop()
{
	size_t read_index;

	do
	{
		read_index = m_read_index.load();

	} while (!m_read_index.compare_exchange_weak(read_index, read_index + 1));

	if (m_size.load() == 0)
	{
		throw std::runtime_error("Out of memory");
	}

	--m_size;
	return m_data[read_index % Size];
}

} //namespace