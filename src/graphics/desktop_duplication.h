#pragma once

#include <dxgi1_2.h>
#include "texture.h"

namespace crlib
{

class desktop_duplication
{
public:
	desktop_duplication();

	gpu_texture* aquire_frame(uint32_t timeout = 0);

private:
	com_ptr<IDXGIOutputDuplication> m_dxgi_duplication;
	gpu_texture* m_frame;
};


} //namespace