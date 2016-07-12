#include "tcp_server.h"

crlib::tcp_server::tcp_server(const ip_address& local_ip)  :
	socket(SOCKET_PROTOCOL::TCP)
{
	bind(local_ip);
	listen();
}

void crlib::tcp_server::start_listen(const crlib::ip_address& ip)
{
	bind(ip);
	listen();
}

crlib::tcp_client* crlib::tcp_server::accept()
{
	return reinterpret_cast<tcp_client*>(socket::accept());
}