#pragma once

#include "texture.h"

namespace crlib
{

class render_target
{
public:
	render_target(uint16_t width, uint16_t height);

	void set_active();

	com_ptr<ID3D11RenderTargetView> handle() { return m_render_target; }
	gpu_texture& texture() { return m_texture; }

	gpu_texture& copy_to_cpu();

private:
	com_ptr<ID3D11RenderTargetView> m_render_target;
	gpu_texture m_texture;
	gpu_texture m_cpu_texture;
};

} //namespace