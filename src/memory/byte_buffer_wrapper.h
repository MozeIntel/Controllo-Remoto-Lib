#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace crlib
{
/*
Wrapper class for pre-existing byte buffers,
allowing object serialization/deserialization
and automatic read/write pointer tracking.
*/
class byte_buffer_wrapper
{
public:
	byte_buffer_wrapper(uint8_t* buffer, size_t size);
	byte_buffer_wrapper(const byte_buffer_wrapper& other);
	byte_buffer_wrapper(byte_buffer_wrapper&& other);

	byte_buffer_wrapper& operator=(byte_buffer_wrapper other);

	template<typename T>
	void write(const T& data);

	template<typename T>
	void write(T* buffer, size_t size);

	template<typename T>
	T* read();

	template<typename T>
	void copy(T* out_buffer, size_t size);

	template<typename T>
	byte_buffer_wrapper& operator<<(const T& value);

	template<typename T>
	byte_buffer_wrapper& operator>>(T& value);

	size_t size() const { return m_size; }

	size_t bytes_read() const { return m_read_index; }
	size_t bytes_written() const { return m_write_index; }

	friend void swap(byte_buffer_wrapper& a, byte_buffer_wrapper& b)
	{
		using std::swap;
		swap(a.m_buffer, b.m_buffer);
		swap(a.m_size, b.m_size);
		swap(a.m_read_index, b.m_read_index);
		swap(a.m_write_index, b.m_write_index);
	}

private:
	uint8_t* m_buffer;
	size_t m_size;

protected:
	size_t m_write_index;
	size_t m_read_index;
};


template<typename T>
inline void byte_buffer_wrapper::write(const T& data)
{
	size_t data_size = sizeof(T);

	if ((m_write_index + data_size) > m_size)
	{
		throw std::runtime_error("Index out of range");
	}

	new(m_buffer + m_write_index) T(data);
	m_write_index += data_size;
}

template<typename T>
inline void byte_buffer_wrapper::write(T* buffer, size_t size)
{
	size_t req_size = sizeof(T) * size;

	if ((m_write_index + req_size) > m_size)
	{
		throw std::runtime_error("Index out of range");
	}

	if (std::is_fundamental<T>::value)
	{
		std::memcpy(m_buffer + m_write_index, buffer, req_size);
		m_write_index += req_size;
	}
	else
	{
		for (size_t i = 0; i < size; ++i)
		{
			new(m_buffer + m_write_index) T(buffer[i]);
			m_write_index += sizeof(T);
		}
	}
}

template<typename T>
inline T* byte_buffer_wrapper::read()
{
	size_t data_size = sizeof(T);

	if ((m_read_index + data_size) > m_size)
	{
		throw std::runtime_error("Index out of range");
	}

	T* ptr = reinterpret_cast<T*>(m_buffer + m_read_index);
	m_read_index += data_size;
	return ptr;
}

template<typename T>
inline void byte_buffer_wrapper::copy(T* out_buffer, size_t size)
{
	size_t req_size = sizeof(T) * size;

	if ((m_write_index + req_size) > m_size)
	{
		throw std::runtime_error("Index out of range");
	}

	if (std::is_fundamental<T>::value)
	{
		std::memcpy(out_buffer, m_buffer + m_read_index, req_size);
	}
	else
	{
		T* buff = reinterpret_cast<T*>(m_buffer);

		for (size_t i = 0; i < size; ++i)
		{
			new(out_buffer + i) T(buff[i]);
		}
	}

	m_read_index += req_size;
}

template<typename T>
inline byte_buffer_wrapper& byte_buffer_wrapper::operator<<(const T& value)
{
	write(value);
	return *this;
}

template<typename T>
inline byte_buffer_wrapper& byte_buffer_wrapper::operator>>(T& value)
{
	value = *read<T>();
	return *this;
}

//Template specializations
template<>
inline void byte_buffer_wrapper::write<std::string>(const std::string& str)
{
	write<uint16_t>(static_cast<uint16_t>(str.size()));
	write(str.data(), str.size());
}

template<>
inline byte_buffer_wrapper& byte_buffer_wrapper::operator>><std::string>(std::string& str)
{
	uint16_t size = *read<uint16_t>();

	if ((m_read_index + size) > m_size)
	{
		throw std::runtime_error("Index out of range");
	}

	str.append(reinterpret_cast<char*>(m_buffer + m_read_index), size);
	m_read_index += size;

	return *this;
}


} //namespace