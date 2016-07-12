#pragma once

#include <cstdint>
#include <cwchar>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "char.h"
#include "char_buffer.h"

namespace crlib
{

class string;

template<typename T>
inline string string_cast(T value) { static_assert(false, "Invalid string_cast"); }
template<> inline string string_cast<string>(string value);
template<> inline string string_cast<const std::wstring&>(const std::wstring& value);
template<> inline string string_cast<int8_t>(int8_t value);
template<> inline string string_cast<uint8_t>(uint8_t value);
template<> inline string string_cast<int16_t>(int16_t value);
template<> inline string string_cast<uint16_t>(uint16_t value);
template<> inline string string_cast<int32_t>(int32_t value);
template<> inline string string_cast<uint32_t>(uint32_t value);
template<> inline string string_cast<int64_t>(int64_t value);
template<> inline string string_cast<uint64_t>(uint64_t value);


class string
{
public:
	typedef wchar_t* iterator;
	typedef const wchar_t* const_iterator;
	typedef std::reverse_iterator<iterator> rev_iterator;
	typedef std::reverse_iterator<const_iterator> const_rev_iterator;

	string();
	string(const_iterator start, const_iterator end);
	explicit string(const std::wstring& str);
	string(const wchar_t* str);
	string(const string& other);
	string(string&& other);
	string& operator=(string other);

	size_t size() const { return m_buffer->size(); }
	bool empty() const { return m_buffer->size() == 0; }

	wchar_t* data() { return m_buffer->data(); }
	const wchar_t* data() const { return m_buffer->data(); }

	wchar_t* begin() { return m_buffer->begin(); }
	const wchar_t* begin() const { return m_buffer->begin(); }

	wchar_t* end() { return m_buffer->end(); }
	const wchar_t* end() const { return m_buffer->end(); }

	wide_char* char_object_at(size_t index);

	void append(wchar_t c);
	void append(const wchar_t* buffer, size_t size);
	void append(const_iterator start, const_iterator end);

	void insert(wchar_t c, size_t index);
	void insert(const wchar_t* buffer, size_t size, size_t index);
	void insert(const_iterator start, const_iterator end, const_iterator pos);

	void erase(size_t index, size_t count);
	void erase(const_iterator pos, size_t count);
	void erase(const_iterator start, const_iterator end);

	string sub_str(size_t index, size_t count) const;
	string sub_str(const_iterator start, size_t count) const;
	string sub_str(const_iterator start, const_iterator end) const;

	void split(std::vector<string>& out_vec, wchar_t delim, size_t limit = 0, bool auto_trim = true, bool remove_empty = true);

	void trim_start();
	void trim_end();
	void trim();

	string trim_start_new() const;
	string trim_end_new() const;
	string trim_new() const;

	void to_upper();
	void to_lower();

	string to_uppder_new() const;
	string to_lower_new() const;

	void reverse();
	string reverse_new() const;

	template<typename T = int, int Base = 10>
	T to_number() const;

	template<typename T>
	string& operator+=(T value);

	template<typename T>
	string operator+(T value) const;

	wchar_t& operator[](size_t index) { return m_buffer->operator[](index); }
	const wchar_t& operator[](size_t index) const { return m_buffer->operator[](index); }

	bool operator==(const string& other) const { return m_buffer->compare(*other.m_buffer) == 0; }
	bool operator!=(const string& other) const { return !(operator==(other)); }
	bool operator<(const string& other) const { return m_buffer->compare(*other.m_buffer) == -1; }
	bool operator>(const string& other) const { return m_buffer->compare(*other.m_buffer) == 1; }
	bool operator<=(const string& other) const { return !(operator>(other)); }
	bool operator>=(const string& other) const { return !(operator<(other)); }

	friend void swap(string& a, string& b);
	friend std::wostream& operator<<(std::wostream& stream, const string& str);

protected:
	string(size_t capacity);
	void copy_if_needed(size_t size);

private:
	std::shared_ptr<char_buffer<wchar_t>> m_buffer;
};


template<> string string_cast<string>(string value) { return value; }
template<> string string_cast<const std::wstring&>(const std::wstring& value) { return string(value); }
template<> string string_cast<int8_t>(int8_t value) { return string_cast(static_cast<int64_t>(value)); }
template<> string string_cast<uint8_t>(uint8_t value) { return string_cast(static_cast<uint64_t>(value)); }
template<> string string_cast<int16_t>(int16_t value) { return string_cast(static_cast<int64_t>(value)); }
template<> string string_cast<uint16_t>(uint16_t value) { return string_cast(static_cast<uint64_t>(value)); }
template<> string string_cast<int32_t>(int32_t value) { return string_cast(static_cast<int64_t>(value)); }
template<> string string_cast<uint32_t>(uint32_t value) { return string_cast(static_cast<uint64_t>(value)); }


template<>
string string_cast<int64_t>(int64_t value)
{
	string str;

	uint64_t abs_value = static_cast<uint64_t>(value < 0 ? -value : value);

	while (abs_value > 0)
	{
		str.append(L'0' + (abs_value % 10));
		abs_value /= 10;
	}

	if (value < 0)
	{
		str.append(L'-');
	}

	str.reverse();
	return str;
}

template<>
string string_cast<uint64_t>(uint64_t value)
{
	string str;

	while (value > 0)
	{
		str.append(L'0' + (value % 10));
		value /= 10;
	}

	str.reverse();
	return str;
}

template<typename T, int Base>
inline T string::to_number() const
{
	static_assert(Base >= 2 && Base <= 36, "Invalid conversion base");
	static_assert(std::is_integral<T>::value, "Invalid integer type");


	std::numeric_limits<T> limits = std::numeric_limits<T>();

	const wchar_t* ptr = m_buffer->begin();
	const wchar_t* end_ptr = m_buffer->end();

	if (ptr == end_ptr)
	{
		throw std::invalid_argument("String conversion failed");
	}

	bool positive = true;

	if (*ptr == L'+')
	{
		++ptr;
	}
	else if (*ptr == L'-')
	{
		positive = false;
		++ptr;
	}

	if (!positive && !limits.is_signed)
	{
		throw std::out_of_range("String conversion failed");
	}

	while (ptr != end_ptr && *ptr == L'0')
	{
		++ptr;
	}

	if (ptr == end_ptr)
	{
		return 0;
	}

	if (Base == 16 && (*ptr == 'x' || *ptr == 'X'))
	{
		++ptr;
	}

	wchar_t c;
	T max_value = positive ? (limits.max() / Base) : -(limits.min() / Base);
	T max_last_value = positive ? (limits.max() % Base) : -(limits.min() % Base);
	T value = 0;

	while (ptr != end_ptr)
	{
		c = *ptr++;

		if (c >= L'0' && c <= L'9')
		{
			c -= L'0';
		}
		else if (c >= L'a' && c <= L'z')
		{
			c -= L'a';
			c += 10;
		}
		else if (c >= L'A' && c <= L'Z')
		{
			c -= L'A';
			c += 10;
		}
		else
		{
			throw std::invalid_argument("String conversion failed");
		}

		if (c > (Base - 1) || value > max_value || (value == max_value && c > max_last_value))
		{
			throw std::out_of_range("String conversion failed");
		}

		value *= Base;
		value += c;
	}

	return positive ? value : -value;
}

template<typename T>
inline string& string::operator+=(T value)
{
	string str = string_cast<T>(value);
	append(str.m_buffer->data(), str.m_buffer->size());
	return *this;
}

template<>
inline string& string::operator+=<wchar_t>(wchar_t c)
{
	append(c);
	return *this;
}

template<typename T>
inline string string::operator+(T value) const
{
	string str = string_cast<T>(value);
	string result(m_buffer->size() + str.size());
	result.append(m_buffer->data(), m_buffer->size());
	result.append(str.m_buffer->data(), str.m_buffer->size());
	return result;
}

template<>
inline string string::operator+<wchar_t>(wchar_t c) const
{
	string str(m_buffer->size() + 1);
	str.append(m_buffer->data(), m_buffer->size());
	str.append(c);
	return str;
}

} //namespace