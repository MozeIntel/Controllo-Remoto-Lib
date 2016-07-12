#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace crlib
{
/*
A fixed size byte array, that is used for object serialization.
*/
template<size_t Size, typename Allocator = std::allocator<uint8_t>>
class byte_buffer
{

public:
	byte_buffer(const Allocator& alloc = Allocator());
	byte_buffer(uint8_t* buff, const Allocator& alloc = Allocator());
	byte_buffer(const byte_buffer<Size, Allocator>& other) = delete;
	byte_buffer(byte_buffer<Size, Allocator>&& other);
	~byte_buffer();

	byte_buffer& operator=(byte_buffer<Size, Allocator> other);

	template<typename T>
	void write(T* ptr, size_t size);

	template<typename T>
	void write(const T& t);

	template<typename T>
	T* read();

	template<typename T>
	void copy(T* ptr, size_t size);

	template<typename T>
	byte_buffer<Size, Allocator>& operator<<(const T& t);

	template<typename T>
	byte_buffer<Size, Allocator>& operator>>(T& t);

	uint8_t* data() { return m_buffer; }
	const uint8_t* data() const { return m_buffer; }

	size_t size() const { return m_write_index; }
	constexpr size_t capacity() const { return Size; }
	size_t read_index() const { return m_read_index; }

	void set_write_index(size_t new_index);
	void set_read_index(size_t new_index);

	friend void swap(byte_buffer<Size, Allocator>& a, byte_buffer<Size, Allocator>& b)
	{
		using std::swap;
		swap(a.m_buffer, b.m_bufferr);
		swap(a.m_write_index, b.m_write_index);
		swap(a.m_read_index, b.m_read_index);
		swap(a.m_allocator, b.m_allocator);
	}

private:
	Allocator m_allocator;
	size_t m_write_index;
	size_t m_read_index;
	uint8_t* m_buffer;
};

template<size_t Size, typename Allocator>
inline byte_buffer<Size, Allocator>::byte_buffer(const Allocator& alloc) :
	m_buffer(nullptr),
	m_write_index(0),
	m_read_index(0),
	m_allocator(alloc)
{
	m_buffer = m_allocator.allocate(Size);
}

template<size_t Size, typename Allocator>
inline byte_buffer<Size, Allocator>::byte_buffer(uint8_t* buff, const Allocator& alloc) :
	m_buffer(buff),
	m_write_index(Size),
	m_read_index(0),
	m_allocator(alloc)
{
}

template<size_t Size, typename Allocator>
inline byte_buffer<Size, Allocator>::byte_buffer(byte_buffer<Size, Allocator>&& other) :
	m_buffer(nullptr),
	m_allocator(),
	m_read_index(0),
	m_write_index(0)
{
	swap(*this, other);
}

template<size_t Size, typename Allocator>
inline byte_buffer<Size, Allocator>::~byte_buffer()
{
	m_allocator.deallocate(m_buffer, Size);
}

template<size_t Size, typename Allocator>
inline byte_buffer<Size, Allocator>& byte_buffer<Size, Allocator>::operator=(byte_buffer<Size, Allocator> other)
{
	swap(*this, other);
	return *this;
}

template<size_t Size, typename Allocator>
inline void byte_buffer<Size, Allocator>::set_write_index(size_t new_index)
{
	if (new_index > Size)
	{
		throw std::runtime_error("Index out of range");
	}

	m_write_index = new_index;
}

template<size_t Size, typename Allocator>
inline void byte_buffer<Size, Allocator>::set_read_index(size_t new_index)
{
	if (new_index > Size)
	{
		throw std::runtime_error("Index out of range");
	}

	read_index = new_index;
}

template<size_t Size, typename Allocator>
template<typename T>
inline void byte_buffer<Size, Allocator>::write(T* ptr, size_t size)
{
	size_t req_size = size * sizeof(T);

	if (m_write_index + req_size > Size)
	{
		throw std::runtime_error("Buffer overflow");
	}

	if (std::is_fundamental<T>::value)
	{
		std::memcpy(m_buffer + m_write_index, ptr, req_size);
		m_write_index += req_size;
	}
	else
	{
		for (size_t i = 0; i < size; ++i)
		{
			new(m_buffer + m_write_index) T(ptr[i]);
			m_write_index += sizeof(T);
		}
	}
}

template<size_t Size, typename Allocator>
template<typename T>
inline void byte_buffer<Size, Allocator>::write(const T& t)
{
	size_t size = sizeof(T);

	if ((m_write_index + size) > Size)
	{
		throw std::runtime_error("Buffer overflow");
	}

	new(m_buffer + m_write_index) T(t);
	m_write_index += size;
}

template<size_t Size, typename Allocator>
template<typename T>
inline T* byte_buffer<Size, Allocator>::read()
{
	size_t size = sizeof(T);

	if ((m_read_index + size) > Size)
	{
		throw std::runtime_error("Buffer overflow");
	}

	T* ptr = reinterpret_cast<T*>(m_buffer + m_read_index);
	m_read_index += size;
	return ptr;
}

template<size_t Size, typename Allocator>
template<typename T>
inline void byte_buffer<Size, Allocator>::copy(T* ptr, size_t size)
{
	size_t req_size = sizeof(T) * size;

	if ((m_read_index + req_size) > Size)
	{
		throw std::runtime_error("Buffer overflow");
	}

	if (std::is_fundamental<T>::value)
	{
		std::memcpy(ptr, m_buffer + m_read_index, req_size);
	}
	else
	{
		T* buff = reinterpret_cast<T*>(m_buffer + m_read_index);

		for (size_t i = 0; i < size; ++i)
		{
			new(ptr + i) T(buff[i]);
		}
	}

	m_read_index += req_size;
}

template<size_t Size, typename Allocator>
template<typename T>
inline byte_buffer<Size, Allocator>& byte_buffer<Size, Allocator>::operator<<(const T& t)
{
	write(t);
	return *this;
}

template<size_t Size, typename Allocator>
template<typename T>
inline byte_buffer<Size, Allocator>& byte_buffer<Size, Allocator>::operator>>(T& t)
{
	t = *read<T>();
	return *this;
}


} //namespace