#pragma once

namespace crlib
{

template<typename T>
void com_release(T*& com_obj)
{
	if (com_obj != nullptr)
	{
		com_obj->Release();
		com_obj = nullptr;
	}
}

/*
Equivalend of std::shared_ptr for COM objects.
*/
template<typename T>
class com_ptr
{
public:
	com_ptr() : m_ptr(nullptr) {}
	com_ptr(T* ptr) : m_ptr(ptr) {}

	com_ptr(const com_ptr<T>& other);
	com_ptr(com_ptr<T>&& other);

	~com_ptr() { com_release(m_ptr); }

	com_ptr<T>& operator=(com_ptr<T> other);
	com_ptr<T>& operator=(T* ptr);

	T** operator&() { return &m_ptr; }

	T* operator*() { return m_ptr; }
	const T* operator*() const { return m_ptr; }

	T* operator->() { return m_ptr; }
	const T* operator->() const { return m_ptr; }

	friend void swap(com_ptr<T>& a, com_ptr<T>& b)
	{
		using std::swap;
		swap(a.m_ptr, b.m_ptr);
	}

private:
	T* m_ptr;
};

template<typename T>
inline com_ptr<T>::com_ptr(const com_ptr<T>& other) :
	m_ptr(other.m_ptr)
{
	if (m_ptr != nullptr)
	{
		m_ptr->AddRef();
	}
}

template<typename T>
inline com_ptr<T>::com_ptr(com_ptr<T>&& other) :
	m_ptr(nullptr)
{
	swap(*this, other);
}

template<typename T>
inline com_ptr<T>& com_ptr<T>::operator=(com_ptr<T> other)
{
	swap(*this, other);
	return *this;
}

template<typename T>
inline com_ptr<T>& com_ptr<T>::operator=(T* ptr)
{
	com_release(m_ptr);
	m_ptr = ptr;
}

} //namespace