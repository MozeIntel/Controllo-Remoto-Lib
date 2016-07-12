#pragma once

#include <algorithm>
#include <cstdint>
#include <utility>

#define CHAR_BUFF_START_CAPACITY 16u
#define CHAR_BUFF_CALC_CAPACITY(Size) std::max(CHAR_BUFF_START_CAPACITY, Size)

namespace crlib
{

template<typename T = uint16_t>
class char_buffer
{
public:
	typedef T* iter;
	typedef const T* const_iter;

	char_buffer(size_t capacity = CHAR_BUFF_START_CAPACITY);
	char_buffer(const T* buffer, size_t size);
	char_buffer(const char_buffer<T>& other);
	char_buffer(char_buffer&& other);
	char_buffer<T>& operator=(char_buffer<T> other);

	~char_buffer() { delete[] m_buffer; }

	T* data() { return m_buffer; }
	const T* data() const { return m_buffer; }

	iter begin() { return m_buffer; }
	const_iter begin() const { return m_buffer; }

	iter end() { return m_buffer + m_size; }
	const_iter end() const { return m_buffer + m_size; }

	size_t size() const { return m_size; }
	size_t capacity() const { return m_capacity; }

	void resize(size_t new_size);
	void shrink_to_fit();

	void append(T c);
	void append(const T* data, size_t size);	

	void insert(T c, size_t index);
	void insert(const T* buffer, size_t size, size_t index);

	void erase(size_t index, size_t count);

	int compare(const char_buffer& other);

	T& operator[](size_t index) { return m_buffer[index]; }
	const T& operator[](size_t index) const { return m_buffer[index]; }

	friend void swap(char_buffer<T>& a, char_buffer<T>& b)
	{
		using std::swap;
		swap(a.m_buffer, b.m_buffer);
		swap(a.m_size, b.m_size);
		swap(a.m_capacity, b.m_capacity);
	}

private:
	size_t m_size;
	size_t m_capacity;
	T* m_buffer;
};

template<typename T>
inline char_buffer<T>::char_buffer(size_t capacity) :
	m_size(0),
	m_capacity(CHAR_BUFF_CALC_CAPACITY(capacity + 1)),
	m_buffer(new T[m_capacity])
{
	m_buffer[0] = T(0);
}

template<typename T>
inline char_buffer<T>::char_buffer(const T* buffer, size_t size) :
	m_size(size),
	m_capacity(CHAR_BUFF_CALC_CAPACITY(size + 1)),
	m_buffer(new T[m_capacity])
{
	std::memcpy(m_buffer, buffer, m_size * sizeof(T));
	m_buffer[m_size] = T(0);
}

template<typename T>
inline char_buffer<T>::char_buffer(const char_buffer<T>& other) :
	m_size(other.m_size),
	m_capacity(other.m_capacity),
	m_buffer(new T[m_capacity])
{
	std::memcpy(m_buffer, other.m_buffer, m_size);
	m_buffer[m_size] = T(0);
}

template<typename T>
inline char_buffer<T>::char_buffer(char_buffer&& other) :
	m_size(0),
	m_capacity(0),
	m_buffer(nullptr)
{
	swap(*this, other);
}

template<typename T>
inline char_buffer<T>& char_buffer<T>::operator=(char_buffer<T> other)
{
	swap(*this, other);
	return *this;
}

template<typename T>
inline void char_buffer<T>::resize(size_t new_size)
{
	if (new_size > m_capacity)
	{
		T* new_buffer = new T[new_size];
		std::memcpy(new_buffer, m_buffer, (m_size + 1) * sizeof(T));
		delete[] m_buffer;
		m_buffer = new_buffer;
		m_capacity = new_size;
	}
}

template<typename T>
inline void char_buffer<T>::shrink_to_fit()
{
	if ((m_size + 1) > m_capacity)
	{
		m_capacity = m_size + 1;
		T* new_buff = new T[m_capacity];
		std::memcpy(new_buff, m_buffer, m_size * sizeof(T));
		new_buff[m_size] = T(0);
		delete[] m_buffer;
		m_buffer = new_buff;
	}
}

template<typename T>
inline void char_buffer<T>::append(T c)
{
	if ((m_size + 1) == m_capacity)
	{
		resize((m_size + 1) * 2);
	}

	m_buffer[m_size++] = c;
	m_buffer[m_size] = T(0);
}

template<typename T>
inline void char_buffer<T>::append(const T* data, size_t size)
{
	if ((m_size + 1 + size) >= m_capacity)
	{
		resize(std::max((m_size + 1) * 2, m_size + 1 + size));
	}

	std::memcpy(m_buffer + m_size, data, size * sizeof(T));
	m_size += size;
	m_buffer[m_size] = T(0);
}

template<typename T>
inline void char_buffer<T>::insert(T c, size_t index)
{
	if ((m_size + 1) == m_capacity)
	{
		resize((m_size + 1) * 2);
	}

	if (index == m_size)
	{
		m_buffer[m_size++] = c;
		m_buffer[m_size] = T(0);
	}
	else
	{
		std::memmove(m_buffer + index + 1, m_buffer + index, (m_size - index) * sizeof(T));
		m_buffer[index] = c;
		m_buffer[++m_size] = T(0);
	}
}

template<typename T>
inline void char_buffer<T>::insert(const T* buffer, size_t size, size_t index)
{
	if ((m_size + size + 1) >= m_capacity)
	{
		resize(std::max((m_size + size + 1), (m_size + 1) * 2));
	}

	if (index == m_size)
	{
		std::memcpy(m_buffer + m_size, buffer, size * sizeof(T));
		m_size += size;
		m_buffer[m_size] = T(0);
	}
	else
	{
		std::memmove(m_buffer + index + size, m_buffer + index, (m_size - index) * sizeof(T));
		std::memcpy(m_buffer + index, buffer, size * sizeof(T));
		m_size += size;
		m_buffer[m_size] = T(0);
	}
}

template<typename T>
inline void char_buffer<T>::erase(size_t index, size_t count)
{
	if (m_buffer + m_size != m_buffer + index + count)
	{
		std::memmove(m_buffer + index, m_buffer + index + count, (m_size - index - count) * sizeof(T));
	}

	m_size -= count;
	m_buffer[m_size] = T(0);
}

template<typename T>
inline int char_buffer<T>::compare(const char_buffer& other)
{
	return m_size < other.m_size ? -1 : m_size > other.m_size ? 1 : std::memcmp(m_buffer, other.m_buffer, m_size * sizeof(T));
}

} //namespace