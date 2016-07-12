#pragma once

#include "..\socket.h"

namespace crlib
{

class tcp_client : public socket
{
public:
	tcp_client() : socket(SOCKET_PROTOCOL::TCP) {}
	tcp_client(const ip_address& local_ip) : socket(SOCKET_PROTOCOL::TCP) { bind(local_ip); }
	~tcp_client() { socket::close(); }

	using socket::connect;
	using socket::disconnect;

	using socket::send;
	using socket::recv;
};

} //namespaceUeasdas