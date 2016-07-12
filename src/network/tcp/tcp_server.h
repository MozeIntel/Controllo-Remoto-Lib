#pragma once

#include "tcp_client.h"
#include "..\..\string\string.h"

namespace crlib
{

class tcp_server : public socket
{
public:
	tcp_server() : socket(SOCKET_PROTOCOL::TCP) {}
	tcp_server(const ip_address& local_ip);
	tcp_server(string ip, uint16_t port) : tcp_server(ip_address(ip, port)) {}
	~tcp_server() { socket::close(); }

	void start_listen(const crlib::ip_address& ip);

	tcp_client* accept();
};

} //namespace