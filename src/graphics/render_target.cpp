#include "render_target.h"

#include <stdexcept>

crlib::render_target::render_target(uint16_t width, uint16_t height) :
	m_render_target(nullptr),
	m_texture(width, height, crlib::COLOR_SPACE_FORMAT_RGBA, crlib::RESOURCE_USAGE_GPU_READ_WRITE, crlib::RESOURCE_CPU_ACCESS_NONE, true),
	m_cpu_texture(width, height, crlib::COLOR_SPACE_FORMAT_RGBA, crlib::RESOURCE_USAGE_CPU_COPY, crlib::RESOURCE_CPU_ACCESS_READ, false)
{
	D3D11_RENDER_TARGET_VIEW_DESC render_target_desc;
	ZeroMemory(&render_target_desc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	render_target_desc.Format = static_cast<DXGI_FORMAT>(crlib::COLOR_SPACE_FORMAT_RGBA);
	render_target_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;


	HRESULT hres = d3d_context::device()->CreateRenderTargetView(*m_texture.handle(), &render_target_desc, &m_render_target);

	if (FAILED(hres))
	{
		throw std::runtime_error("RenderTargetView creation failed");
	}
}

void crlib::render_target::set_active()
{
	d3d_context::device_context()->OMSetRenderTargets(1, &m_render_target, nullptr);

	auto info = m_texture.query_info();
	D3D11_VIEWPORT view_port = { 0 };
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	view_port.Width = info.width;
	view_port.Height = info.height;

	d3d_context::device_context()->RSSetViewports(1, &view_port);
}

crlib::gpu_texture& crlib::render_target::copy_to_cpu()
{
	m_texture.copy_to(m_cpu_texture);
	return m_cpu_texture;
}
