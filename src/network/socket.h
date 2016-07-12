#pragma once

#include <cstdint>
#include "ip_address.h"

namespace crlib
{

enum class SOCKET_PROTOCOL
{
	TCP,
	UDP
};

/*
Abstraction for a socket.
Provides equal functionality regradless of the protocol used.
*/
class socket
{
public:
	socket(SOCKET_PROTOCOL protocol);
	socket(const socket& other);
	socket(socket&& other);
	socket& operator=(socket other);

	ip_address& local_ip() { return m_local_ip; }
	const ip_address& local_ip() const { return m_local_ip; }
	ip_address& remote_ip() { return m_remote_ip; }
	const ip_address& remote_ip() const { return m_remote_ip; }

	intptr_t descriptor() const { return m_socket; }


	size_t send(const char* buf, size_t size);
	size_t send_to(const char* buf, size_t size, const ip_address& dest);

	size_t recv(char* buf, size_t size);
	size_t recv_from(char* buf, size_t size, ip_address& from);

	void close();

	friend void swap(socket& a, socket& b);

protected:
	uintptr_t m_socket;
	ip_address m_local_ip;
	ip_address m_remote_ip;

	void connect(const ip_address& remote_addr);
	void disconnect();

	void bind(const ip_address& local_addr);
	void listen();

	socket* accept();

private:
	struct winsock_initializer
	{
		winsock_initializer();
		~winsock_initializer();
	}; 

	static winsock_initializer m_initializer;

	socket() : m_socket(0) {}
	socket(uintptr_t descriptor, const ip_address& local, const ip_address& remote) : m_socket(descriptor), m_local_ip(local), m_remote_ip(remote) {}
};

} //namespace
