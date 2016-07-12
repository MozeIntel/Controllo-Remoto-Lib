#include "string.h"

#include <algorithm>
#include <cwctype>

crlib::string::string() :
	m_buffer(new char_buffer<wchar_t>)
{
}

crlib::string::string(const_iterator start, const_iterator end) :
	m_buffer(new char_buffer<wchar_t>(start, end - start))
{
}

crlib::string::string(size_t capacity) :
	m_buffer(new char_buffer<wchar_t>(capacity))
{
}

crlib::string::string(const std::wstring& str) :
	m_buffer(new char_buffer<wchar_t>(str.data(), str.size()))
{
}

crlib::string::string(const wchar_t* str) :
	m_buffer(new char_buffer<wchar_t>(str, wcslen(str)))
{
}

crlib::string::string(const string& other) :
	m_buffer(other.m_buffer)
{
}

crlib::string::string(string&& other) :
	m_buffer(nullptr)
{
	swap(*this, other);
}

crlib::string& crlib::string::operator=(string other)
{
	swap(*this, other);
	return *this;
}

crlib::wide_char* crlib::string::char_object_at(size_t index)
{
	return reinterpret_cast<wide_char*>(m_buffer->data() + index);
}

void crlib::string::append(wchar_t c)
{
	copy_if_needed(m_buffer->size() + 1);
	m_buffer->append(c);
}

void crlib::string::append(const wchar_t* buffer, size_t size)
{
	copy_if_needed(m_buffer->size() + size);
	m_buffer->append(buffer, size);
}

void crlib::string::append(const_iterator start, const_iterator end)
{
	append(start, end - start);
}

void crlib::string::insert(wchar_t c, size_t index)
{
	copy_if_needed(m_buffer->size() + 1);
	m_buffer->insert(c, index);
}

void crlib::string::insert(const wchar_t* buffer, size_t size, size_t index)
{
	copy_if_needed(m_buffer->size() + size);
	m_buffer->insert(buffer, size, index);
}

void crlib::string::insert(const_iterator start, const_iterator end, const_iterator pos)
{
	insert(start, end - start, pos - m_buffer->data());
}

void crlib::string::erase(size_t index, size_t count)
{
	copy_if_needed(m_buffer->size());
	m_buffer->erase(index, count);
}

void crlib::string::erase(const_iterator pos, size_t count)
{
	erase(pos - m_buffer->data(), count);
}

void crlib::string::erase(const_iterator start, const_iterator end)
{
	erase(start - m_buffer->data(), end - start);
}

crlib::string crlib::string::sub_str(size_t index, size_t count) const
{
	string str(m_buffer->size() - index - count);
	str.append(m_buffer->data() + index, count);
	return str;
}

crlib::string crlib::string::sub_str(const_iterator start, size_t count) const
{
	return sub_str(start - m_buffer->data(), count);
}

crlib::string crlib::string::sub_str(const_iterator start, const_iterator end) const
{
	return sub_str(start - m_buffer->data(), end - start);
}

void crlib::string::split(std::vector<string>& out_vec, wchar_t delim, size_t limit, bool auto_trim, bool remove_empty)
{
	const_iterator end_iter = end();
	const_iterator prev_iter = begin();
	const_iterator iter = std::find(prev_iter, end_iter, delim);

	while (iter != end_iter)
	{
		string sub = sub_str(prev_iter, iter);

		if (auto_trim)
		{
			sub.trim();
		}

		if (!sub.empty() || !remove_empty)
		{
			out_vec.push_back(sub);

			if (out_vec.size() == limit)
			{
				prev_iter = iter + 1;
				break;
			}
		}

		prev_iter = iter + 1;
		iter = std::find(prev_iter, end_iter, delim);
	}

	if (prev_iter != end_iter)
	{
		string sub = sub_str(prev_iter, end_iter);

		if (auto_trim)
		{
			sub.trim();
		}

		if (!sub.empty() || !remove_empty)
		{
			out_vec.push_back(sub);
		}
	}
}

void crlib::string::trim_start()
{
	iterator start_iter = begin();
	iterator end_iter = end();
	iterator iter = std::find_if_not(start_iter, end_iter, [](const wchar_t& c) { return std::iswspace(c); });

	if (iter != start_iter)
	{
		erase(start_iter, iter);
	}
}

void crlib::string::trim_end()
{
	iterator start_iter = begin();
	iterator end_iter = end();
	iterator iter = std::find_if_not(rev_iterator(end_iter), rev_iterator(start_iter), [](const wchar_t& c) { return std::iswspace(c); }).base();

	if (iter != end_iter)
	{ 
		erase(iter, end_iter);
	}
}

void crlib::string::trim()
{
	trim_start();
	trim_end();
}

crlib::string crlib::string::trim_start_new() const
{
	const_iterator start_iter = begin();
	const_iterator end_iter = end();
	const_iterator iter = std::find_if_not(start_iter, end_iter, [](const wchar_t& c) { return std::iswspace(c); });

	if (iter != start_iter)
	{
		return string(iter, end_iter);
	}
	else
	{
		return *this;
	}
}

crlib::string crlib::string::trim_end_new() const
{
	const_iterator start_iter = begin();
	const_iterator end_iter = end();
	const_iterator iter = std::find_if_not(const_rev_iterator(end_iter), const_rev_iterator(start_iter), [](const wchar_t& c) { return std::iswspace(c); }).base();

	if (iter != end_iter)
	{
		return string(start_iter, iter);
	}
	else
	{
		return *this;
	}
}

crlib::string crlib::string::trim_new() const
{
	string str(*this);
	str.trim();
	return str;
}

void crlib::string::to_upper()
{
	copy_if_needed(m_buffer->size());
	std::for_each(begin(), end(), [](wchar_t& c) { c = std::towupper(c); });
}

void crlib::string::to_lower()
{
	copy_if_needed(m_buffer->size());
	std::for_each(begin(), end(), [](wchar_t& c) { c = std::towlower(c); });
}

crlib::string crlib::string::to_uppder_new() const
{
	string str(*this);
	str.to_upper();
	return str;
}

crlib::string crlib::string::to_lower_new() const
{
	string str(*this);
	str.to_lower();
	return str;
}

void crlib::string::reverse()
{
	wchar_t* ptr = m_buffer->data();

	for (size_t i = 0, size = m_buffer->size(), end = size / 2; i != end; ++i)
	{
		std::swap(ptr[i], ptr[size - 1 - i]);
	}
}

crlib::string crlib::string::reverse_new() const
{
	string str(m_buffer->size());

	for (const_iterator iter = end() - 1, end_iter = begin(); iter >= end_iter; --iter)
	{
		str.append(*iter);
	}

	return str;
}

void crlib::string::copy_if_needed(size_t size)
{
	if (m_buffer.use_count() > 1)
	{
		char_buffer<wchar_t>* new_buffer = new char_buffer<wchar_t>(size);
		new_buffer->append(m_buffer->data(), m_buffer->size());
		m_buffer.reset(new_buffer);
	}
}

void crlib::swap(string& a, string& b)
{
	using std::swap;
	swap(a.m_buffer, b.m_buffer);
}

std::wostream & crlib::operator<<(std::wostream& stream, const string & str)
{
	stream << str.begin();
	return stream;
}
