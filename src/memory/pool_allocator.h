#pragma once

#include <memory>
#include <utility>
#include "..\threading\concurrent_array_queue.h"


namespace crlib
{

/*
An std compliant allocator, that pre-allocates N blocks of
memory for an object T. Memory is allocation/deallocation
is thread safe.
*/
template<typename T, size_t Size>
class pool_allocator
{
private:
	struct data
	{
		data();
		~data() { delete[] buffer; }

		T* buffer;
		concurrent_array_queue<T*, Size> free_blocks;
	};

public:
	pool_allocator() : m_data(new data) {}
	pool_allocator(const pool_allocator<T, Size>& other) : m_data(other.m_data) {}
	pool_allocator(pool_allocator<T, Size>&& other);

	pool_allocator<T, Size>& operator=(pool_allocator<T, Size> other);

	void construct(T* ptr, const T& t);

	template<typename... Args>
	void construct(T* ptr, Args&&... args);

	void destroy(T* ptr);

	T* allocate(size_t count = 1);
	void deallocate(T* ptr, size_t count = 1);

	//Allows the allocator to be used as a deleter
	void operator()(T* ptr); 

	constexpr size_t capacity() const { return Size; }
	size_t free_blocks() const { return m_data->free_blocks.size(); }

	friend void swap(pool_allocator<T, Size>& a, pool_allocator<T, Size>& b)
	{
		using std::swap;
		swap(a.m_data, b.m_data);
	}

private:
	std::shared_ptr<data> m_data;
};

template<typename T, size_t Size>
inline pool_allocator<T, Size>::pool_allocator(pool_allocator<T, Size>&& other) :
	m_data(nullptr)
{
	swap(*this, other);
}

template<typename T, size_t Size>
inline pool_allocator<T, Size>& pool_allocator<T, Size>::operator=(pool_allocator<T, Size> other)
{
	swap(*this, other);
	return *this;
}

template<typename T, size_t Size>
inline void pool_allocator<T, Size>::construct(T* ptr, const T& t)
{
	new(ptr) T(t);
}

template<typename T, size_t Size>
inline void pool_allocator<T, Size>::destroy(T* ptr)
{
	ptr->~T();
}

template<typename T, size_t Size>
inline T* crlib::pool_allocator<T, Size>::allocate(size_t count)
{
	return m_data->free_blocks.pop();
}

template<typename T, size_t Size>
inline void crlib::pool_allocator<T, Size>::deallocate(T* ptr, size_t count)
{
	if (ptr < m_data->buffer || ptr >= (m_data->buffer + Size))
	{
		throw std::runtime_error("Invalid ptr");
	}

	m_data->free_blocks.push(ptr);
}

template<typename T, size_t Size>
inline void pool_allocator<T, Size>::operator()(T* ptr)
{
	destroy(ptr);
	deallocate(ptr);
}

template<typename T, size_t Size>
inline pool_allocator<T, Size>::data::data() :
	buffer(new T[Size]),
	free_blocks()
{
	for (size_t i = 0; i < Size; ++i)
	{
		free_blocks.push(buffer + i);
	}
}

template<typename T, size_t Size>
template<typename... Args>
inline void pool_allocator<T, Size>::construct(T* ptr, Args&&... args)
{
	new(ptr) T(std::forward<Args>(args)...);
}

} //namespace