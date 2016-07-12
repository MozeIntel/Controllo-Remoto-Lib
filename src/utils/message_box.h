#pragma once

#include <string>

namespace crlib
{

class message_box
{
public:
	message_box() = delete;

	void show_error(const std::string& title, const std::string& desc);
};


} //namespace