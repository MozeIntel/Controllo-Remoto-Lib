#pragma once

#include <cstdint>
#include <utility>
#include "d3d_context.h"
#include "d3d_enum.h"

namespace crlib
{

struct texture_desc
{
	texture_desc(const D3D11_TEXTURE2D_DESC& desc);

	uint16_t width;
	uint16_t height;
	COLOR_SPACE_FORMAT color_format;
	RESOURCE_USAGE usage;
	RESOURCE_CPU_ACCESS cpu_access;
};


class gpu_texture
{
public:
	gpu_texture() : m_texture() {}
	gpu_texture(ID3D11Texture2D* texture2D) : m_texture(texture2D) {}
	gpu_texture(uint16_t width, uint16_t height, COLOR_SPACE_FORMAT color_format, RESOURCE_USAGE usage = RESOURCE_USAGE_GPU_READ_WRITE, RESOURCE_CPU_ACCESS cpu_access = RESOURCE_CPU_ACCESS_NONE, bool render_target = false);
	gpu_texture(const gpu_texture& other) : m_texture(other.m_texture) {}
	gpu_texture(gpu_texture&& other) : m_texture(std::move(other.m_texture)) {}

	gpu_texture& operator=(gpu_texture other);

	texture_desc query_info();

	uint8_t* map(RESOURCE_MAP_MODE map_mode = RESOURCE_MAP_MODE_READ);
	void unmap();

	void copy_to(gpu_texture& other);

	com_ptr<ID3D11Texture2D>& handle() { return m_texture; }

	friend void swap(gpu_texture& a, gpu_texture& b);

private:
	com_ptr<ID3D11Texture2D> m_texture;
};


} //namespace