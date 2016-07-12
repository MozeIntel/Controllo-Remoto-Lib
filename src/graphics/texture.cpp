#include "texture.h"

#include <stdexcept>

crlib::gpu_texture::gpu_texture(uint16_t width, uint16_t height, COLOR_SPACE_FORMAT color_format, RESOURCE_USAGE usage, RESOURCE_CPU_ACCESS cpu_acces, bool render_target) :
	m_texture()
{
	D3D11_TEXTURE2D_DESC tex_desc = { 0 };
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = static_cast<DXGI_FORMAT>(color_format);
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = static_cast<D3D11_USAGE>(usage);
	tex_desc.BindFlags = render_target ? D3D11_BIND_RENDER_TARGET : 0;
	tex_desc.CPUAccessFlags = static_cast<D3D11_CPU_ACCESS_FLAG>(cpu_acces);
	tex_desc.MiscFlags = 0;

	HRESULT result = d3d_context::device()->CreateTexture2D(&tex_desc, nullptr, &m_texture);

	if (FAILED(result))
	{
		throw std::runtime_error("Failed to create texture");
	}
}

crlib::gpu_texture& crlib::gpu_texture::operator=(gpu_texture other)
{
	swap(*this, other);
	return *this;
}

crlib::texture_desc crlib::gpu_texture::query_info()
{
	D3D11_TEXTURE2D_DESC desc = { 0 };
	m_texture->GetDesc(&desc);
	return texture_desc(desc);
}

uint8_t* crlib::gpu_texture::map(RESOURCE_MAP_MODE map_mode)
{
	D3D11_MAPPED_SUBRESOURCE mapped_texture = { 0 };
	HRESULT result = d3d_context::device_context()->Map(*m_texture, 0, static_cast<D3D11_MAP>(map_mode), 0, &mapped_texture);

	if (FAILED(result))
	{
		throw std::runtime_error("Failed to map texture");
	}

	return reinterpret_cast<uint8_t*>(mapped_texture.pData);
}

void crlib::gpu_texture::unmap()
{
	d3d_context::device_context()->Unmap(*m_texture, 0);
}

void crlib::gpu_texture::copy_to(gpu_texture& other)
{
	d3d_context::device_context()->CopyResource(*other.m_texture, *m_texture);
}

void crlib::swap(gpu_texture & a, gpu_texture & b)
{
	using std::swap;
	swap(a.m_texture, b.m_texture);
}

crlib::texture_desc::texture_desc(const D3D11_TEXTURE2D_DESC& desc) :
	width(desc.Width),
	height(desc.Height),
	color_format(static_cast<COLOR_SPACE_FORMAT>(desc.Format)),
	usage(static_cast<RESOURCE_USAGE>(desc.Usage)),
	cpu_access(static_cast<RESOURCE_CPU_ACCESS>(desc.CPUAccessFlags))
{
}
