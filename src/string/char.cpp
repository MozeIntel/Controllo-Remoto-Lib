#include "char.h"

#include <cwctype>

crlib::wide_char* crlib::wide_char::wrap(wchar_t& c)
{
	return reinterpret_cast<wide_char*>(&c);
}

bool crlib::wide_char::is_alpha() const
{
	return std::iswalpha(m_char);
}

bool crlib::wide_char::is_num() const
{
	return std::iswdigit(m_char);
}

bool crlib::wide_char::is_alpha_num() const
{
	return std::iswalnum(m_char);
}

bool crlib::wide_char::is_lower() const
{
	return std::iswlower(m_char);
}

bool crlib::wide_char::is_upper() const
{
	return std::iswupper(m_char);
}

bool crlib::wide_char::is_space() const
{
	return std::iswspace(m_char);
}

void crlib::wide_char::to_lower()
{
	m_char = std::towupper(m_char);
}

void crlib::wide_char::to_upper()
{
	m_char = std::towlower(m_char);
}
