#pragma once

#include <cstdint>
#include "..\..\memory\byte_buffer_wrapper.h"


namespace crlib
{

class udp_socket;

template<uint16_t Size>
class udp_packet : public byte_buffer_wrapper
{
public:
	udp_packet(uint8_t* buffer, size_t size);
	udp_packet() : byte_buffer_wrapper(m_data, Size) {}

	uint8_t* data() { return m_data; }
	const uint8_t* data() const { return m_data; }

	uint16_t constexpr capacity() const { return Size; }
	uint16_t size() const { return static_cast<uint16_t>(m_write_index); }

private:
	uint8_t m_data[Size];
	friend class udp_socket;
};


template<uint16_t Size>
inline udp_packet<Size>::udp_packet(uint8_t* buffer, size_t size) :
	byte_buffer_wrapper(m_data, size)
{
	if (size > Size)
	{
		throw std::runtime_error("Invalid buffer size");
	}

	std::memcpy(m_data, buffer, size);
	m_write_index = size;
}


} //namespace