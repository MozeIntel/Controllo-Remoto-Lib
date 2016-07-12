#include "socket.h"

#include <stdexcept>
#include <WinSock2.h>
#include <MSWSock.h>

crlib::socket::winsock_initializer crlib::socket::m_initializer;

crlib::socket::winsock_initializer::winsock_initializer()
{
	WSADATA wsa_data;

	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		throw std::runtime_error("Failed to initialize winsock");
	}
}

crlib::socket::winsock_initializer::~winsock_initializer()
{
	WSACleanup();
}

crlib::socket::socket(SOCKET_PROTOCOL protocol) :
	m_socket(INVALID_SOCKET),
	m_local_ip(),
	m_remote_ip()
{
	int sock_type;
	int sock_proto;

	switch (protocol)
	{
	case SOCKET_PROTOCOL::TCP:
		sock_type = SOCK_STREAM;
		sock_proto = IPPROTO_TCP; 
		break;

	case SOCKET_PROTOCOL::UDP:
		sock_type = SOCK_DGRAM;
		sock_proto = IPPROTO_UDP;
		break;

	default:
		throw std::runtime_error("Unsupported socket protocol");
		break;
	}

	m_socket = WSASocketW(AF_INET, sock_type, sock_proto, nullptr, 0, WSA_FLAG_OVERLAPPED); //::socket(AF_INET, sock_type, sock_proto);

	if (m_socket == INVALID_SOCKET)
	{
		throw std::runtime_error("Socket creation failed");
	}
}

crlib::socket::socket(const socket& other) :
	m_socket(other.m_socket),
	m_local_ip(other.m_local_ip),
	m_remote_ip(other.m_remote_ip)
{
}

crlib::socket::socket(socket&& other) :
	m_socket(0),
	m_local_ip(),
	m_remote_ip()
{
	swap(*this, other);
}

crlib::socket& crlib::socket::operator=(socket other)
{
	swap(*this, other);
	return *this;
}

void crlib::socket::connect(const ip_address& remote_addr)
{
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = remote_addr.address();
	addr.sin_port = remote_addr.net_port();

	if (::connect(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to connect to remote socket");
	}

	m_remote_ip = remote_addr;

	if (m_local_ip == ip_address())
	{
		ZeroMemory(&addr, sizeof(sockaddr_in));
		int addr_len = sizeof(sockaddr_in);

		if (::getsockname(m_socket, reinterpret_cast<sockaddr*>(&addr), &addr_len) == SOCKET_ERROR)
		{
			throw std::runtime_error("getsockname failed");
		}

		m_local_ip = ip_address(addr.sin_addr.S_un.S_addr, addr.sin_port);
	}
}

void crlib::socket::disconnect()
{
	GUID guid = WSAID_DISCONNECTEX;
	LPFN_DISCONNECTEX disconnect_fn = NULL;
	DWORD bytes_returned = 0;

	if (WSAIoctl(m_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(GUID), &disconnect_fn, sizeof(LPFN_DISCONNECTEX), &bytes_returned, NULL, NULL) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to disconnect socket");
	}

	if (!disconnect_fn(m_socket, nullptr, TF_REUSE_SOCKET, 0))
	{
		throw std::runtime_error("Failed to disconnect socket");
	}
}

void crlib::socket::bind(const ip_address& local_addr)
{
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = local_addr.address();
	addr.sin_port = local_addr.net_port();

	if (::bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		throw std::runtime_error("Failed to bind socket");
	}

	ZeroMemory(&addr, sizeof(sockaddr_in));
	int addr_len = sizeof(sockaddr_in);

	if (::getsockname(m_socket, reinterpret_cast<sockaddr*>(&addr), &addr_len) == SOCKET_ERROR)
	{
		throw std::runtime_error("getsockname failed");
	}

	m_local_ip = ip_address(addr.sin_addr.S_un.S_addr, addr.sin_port);
}

void crlib::socket::listen()
{
	if (::listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		throw std::runtime_error("listen call on socket failed");
	}
}

crlib::socket* crlib::socket::accept()
{
	sockaddr_in remote_addr = { 0 };
	int size = sizeof(sockaddr_in);
	uintptr_t remote_sock = ::accept(m_socket, reinterpret_cast<sockaddr*>(&remote_addr), &size);

	if (remote_sock == INVALID_SOCKET)
	{
		throw std::runtime_error("accpet call on socket failed");
	}

	return new socket(remote_sock, ip_address(remote_addr.sin_addr.S_un.S_addr, remote_addr.sin_port), m_local_ip);
}

size_t crlib::socket::send(const char* buf, size_t size)
{
	int sent = ::send(m_socket, buf, size, 0);

	if (sent == SOCKET_ERROR)
	{
		throw std::runtime_error("send call on socket failed");
	}

	return static_cast<size_t>(sent);
}

size_t crlib::socket::send_to(const char* buf, size_t size, const ip_address& dest)
{
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = dest.address();
	addr.sin_port = dest.net_port();

	int sent = ::sendto(m_socket, buf, size, 0, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));

	if (sent == SOCKET_ERROR)
	{
		throw std::runtime_error("sendto call on socket failed");
	}

	return static_cast<size_t>(sent);
}

size_t crlib::socket::recv(char* buf, size_t size)
{
	int received = ::recv(m_socket, buf, size, 0);

	if (received == SOCKET_ERROR)
	{
		throw std::runtime_error("recv call on socket failed");
	}

	return static_cast<size_t>(received);
}

size_t crlib::socket::recv_from(char* buf, size_t size, ip_address& from)
{
	sockaddr_in addr = { 0 };
	int addr_len = sizeof(sockaddr_in);

	int received = ::recvfrom(m_socket, buf, size, 0, reinterpret_cast<sockaddr*>(&addr), &addr_len);

	if (received == SOCKET_ERROR)
	{
		throw std::runtime_error("recvfrom call on socket failed");
	}

	from = ip_address(addr.sin_addr.S_un.S_addr, addr.sin_port);

	return static_cast<size_t>(received);
}

void crlib::socket::close()
{
	::shutdown(m_socket, SD_BOTH);
	::closesocket(m_socket);
}

void crlib::swap(socket & a, socket & b)
{
	using std::swap;
	swap(a.m_socket, b.m_socket);
	swap(a.m_local_ip, b.m_local_ip);
	swap(a.m_remote_ip, b.m_remote_ip);
}
