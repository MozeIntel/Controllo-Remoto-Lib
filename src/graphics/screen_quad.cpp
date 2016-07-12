#include "screen_quad.h"

#include <stdexcept>

crlib::vertex SCREEN_QUAD_VERTEX_DATA[]
{
	crlib::vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
	crlib::vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
	crlib::vertex(1.0f,  1.0f, 1.0f, 1.0f, 0.0f),
	crlib::vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f)
};

DWORD SCREEN_QUAD_INDEX_DATA[]
{
	0, 1, 2,
	0, 2, 3
};

crlib::screen_quad::screen_quad(shader& s) :
	m_shader(s),
	m_vertex_buffer(nullptr),
	m_index_buffer(nullptr),
	m_vertex_layout(nullptr)
{
	D3D11_BUFFER_DESC vertex_buff_desc = { 0 };
	vertex_buff_desc.ByteWidth = sizeof(vertex) * ARRAYSIZE(SCREEN_QUAD_VERTEX_DATA);
	vertex_buff_desc.Usage = D3D11_USAGE_DEFAULT;
	vertex_buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buff_desc.CPUAccessFlags = 0;
	vertex_buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertex_buffer_data = { 0 };
	vertex_buffer_data.pSysMem = SCREEN_QUAD_VERTEX_DATA;
	vertex_buffer_data.SysMemPitch = 0;
	vertex_buffer_data.SysMemSlicePitch = 0;

	HRESULT res = d3d_context::device()->CreateBuffer(&vertex_buff_desc, &vertex_buffer_data, &m_vertex_buffer);

	if (FAILED(res))
	{
		throw std::runtime_error("Failed to create vertex buffer");
	}

	D3D11_BUFFER_DESC index_buff_desc = { 0 };
	index_buff_desc.ByteWidth = sizeof(DWORD) * ARRAYSIZE(SCREEN_QUAD_INDEX_DATA);
	index_buff_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buff_desc.CPUAccessFlags = 0;
	index_buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA index_buffer_data = { 0 };
	index_buffer_data.pSysMem = SCREEN_QUAD_INDEX_DATA;
	index_buffer_data.SysMemPitch = 0;
	index_buffer_data.SysMemSlicePitch = 0;

	res = d3d_context::device()->CreateBuffer(&index_buff_desc, &index_buffer_data, &m_index_buffer);

	if (FAILED(res))
	{
		throw std::runtime_error("Failed to create index buffer");
	}


	D3D11_INPUT_ELEMENT_DESC vertex_layout_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(vertex, vertex::uv), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	res = d3d_context::device()->CreateInputLayout(vertex_layout_desc, ARRAYSIZE(vertex_layout_desc), m_shader.vs_bytecode()->GetBufferPointer(), m_shader.vs_bytecode()->GetBufferSize(), &m_vertex_layout);

	if (FAILED(res))
	{
		throw std::runtime_error("Failed to create input layout");
	}
}

void crlib::screen_quad::set_active()
{
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	d3d_context::device_context()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);
	d3d_context::device_context()->IASetIndexBuffer(*m_index_buffer, DXGI_FORMAT_R32_UINT, 0);
	d3d_context::device_context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3d_context::device_context()->IASetInputLayout(*m_vertex_layout);
}

void crlib::screen_quad::draw(gpu_texture & tex)
{
	shader_resource resource(tex.handle());
	resource.set_active();
	d3d_context::device_context()->DrawIndexed(6, 0, 0);
}