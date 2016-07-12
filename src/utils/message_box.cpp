#include "message_box.h"

#include <Windows.h>

void crlib::message_box::show_error(const std::string & title, const std::string & desc)
{
	MessageBox(nullptr, desc.c_str(), title.c_str(), MB_OK | MB_ICONERROR | MB_TASKMODAL);
}
