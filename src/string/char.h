#pragma once

namespace crlib
{

class wide_char
{
public:
	wide_char(wchar_t c) : m_char(c) {}

	static wide_char* wrap(wchar_t& c);

	wchar_t get() { return m_char; }
	void set(wchar_t value) { m_char = value; }

	operator wchar_t() const { return m_char; }

	bool is_alpha() const;
	bool is_num() const;
	bool is_alpha_num() const;
	bool is_lower() const;
	bool is_upper() const;
	bool is_space() const;

	void to_lower();
	void to_upper();

private:
	wchar_t m_char;
};

} //namespace