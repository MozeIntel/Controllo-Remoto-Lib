#include "network_stream.h"

#include <algorithm>

crlib::network_stream::network_stream(socket* sock) :
	m_sock(sock),
	m_fragments(),
	m_needs_fragment(true)
{
}

void crlib::network_stream::send(const crlib::string& str)
{
	m_sock->send(reinterpret_cast<const char*>(str.data()), (str.size() + 1) * sizeof(wchar_t));
}

crlib::string crlib::network_stream::read_line()
{
	string str;

	while (true)
	{
		wchar_t* end_iter;
		wchar_t* search_iter;

		while (m_needs_fragment)
		{
			tcp_stream_fragment* fragment = new tcp_stream_fragment;
			fragment->bytes_read = (m_sock->recv(reinterpret_cast<char*>(fragment->data), fragment->size() * sizeof(wchar_t)) / sizeof(wchar_t));
			end_iter = fragment->data + fragment->bytes_read;

			m_fragments.push(fragment);

			if (std::find(fragment->data, end_iter, 0) != end_iter)
			{
				m_needs_fragment = false;
			}
		}

		do
		{
			tcp_stream_fragment* head = m_fragments.front();
			end_iter = head->data + head->bytes_read;
			search_iter = std::find(head->ptr, end_iter, 0);
			str.append(head->ptr, search_iter);

			if (search_iter == end_iter)
			{
				delete head;
				m_fragments.pop();

				if (m_fragments.empty())
				{
					m_needs_fragment = true;
					break;
				}
			}
			else
			{
				head->ptr = search_iter + 1;

				if (head->ptr == end_iter)
				{
					delete head;
					m_fragments.pop();

					if (m_fragments.empty())
					{
						m_needs_fragment = true;
					}
				}

				return str;
			}
		} while (true);
	}
}
