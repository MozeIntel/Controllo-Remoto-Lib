#pragma once

#include <queue>
#include "socket.h"
#include "..\string\string.h"

#define NETWORK_STREAM_FRAGMENT_SIZE 1024

namespace crlib
{

class network_stream
{
private:
	struct tcp_stream_fragment
	{
		tcp_stream_fragment() : data(), ptr(data), bytes_read(0) {}

		wchar_t data[NETWORK_STREAM_FRAGMENT_SIZE];
		wchar_t* ptr;
		size_t bytes_read;

		static constexpr size_t size() { return NETWORK_STREAM_FRAGMENT_SIZE; }
	};

public:
	network_stream(socket* sock);

	void send(const string& str);

	crlib::string read_line();

private:
	socket* m_sock;
	std::queue<tcp_stream_fragment*> m_fragments;
	bool m_needs_fragment;
};


} //namespace