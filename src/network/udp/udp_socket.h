#pragma once

#include "..\socket.h"
#include "udp_packet.h"

namespace crlib
{

class udp_socket : public socket
{
public:
	udp_socket(const ip_address& ip) : socket(SOCKET_PROTOCOL::UDP) { bind(ip); }

	using socket::recv_from;
	using socket::send_to;
};

} //namespace