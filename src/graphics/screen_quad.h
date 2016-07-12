#pragma once

#include <DirectXMath.h>
#include "d3d_context.h"
#include "shader.h"
#include "texture.h"

namespace crlib
{

struct vertex
{
	vertex(float x, float y, float z, float u, float v) : pos(x, y, z), uv(u, v) {}

	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 uv;
};

class screen_quad
{
public:
	screen_quad(shader& s);
	
	void set_active();

	void draw(gpu_texture& tex);

private:
	shader& m_shader;
	com_ptr<ID3D11Buffer> m_vertex_buffer;
	com_ptr<ID3D11Buffer> m_index_buffer;
	com_ptr<ID3D11InputLayout> m_vertex_layout;
};


} //namespace