#pragma once

#include <functional>
#include <utility>
#include "..\string\string.h"

namespace crlib
{

/*
Abstraction for IPv4 addresses.
*/
class ip_address
{
public:
	ip_address() : m_address(0), m_port(0) {}
	ip_address(uint32_t addr, uint16_t port) : m_address(addr), m_port(port) {}
	ip_address(string str, uint16_t port); //port must be in little endian (NOT network byte order)
	ip_address(const ip_address& other) : m_address(other.m_address), m_port(other.m_port) {}
	ip_address(ip_address&& other) : m_address(std::move(other.m_address)), m_port(std::move(other.m_port)) {}

	ip_address& operator=(ip_address other);

	bool operator==(const ip_address& other) const { return m_address == other.m_address && m_port == other.m_port; }
	bool operator!=(const ip_address& other) const { return !(operator==(other)); }

	uint32_t address() const { return m_address; }
	uint16_t port() const; //little endian (machine byte order)
	uint16_t net_port() const { return m_port; } //big endian (network byte order)

	string to_string() const;

	friend void swap(ip_address& a, ip_address& b);
	friend std::wostream& operator<<(std::wostream& stream, const ip_address& ip);

private:
	uint32_t m_address;
	uint16_t m_port;
};

} //namespace

//hash function specialization 
namespace std
{
	template<>
	struct std::hash<crlib::ip_address>
	{
		size_t operator()(const crlib::ip_address& ip) const;
	};
}