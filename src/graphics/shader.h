#pragma once

#include "d3d_context.h"

namespace crlib
{

class shader
{
public:
	shader(const char* shader_ascii, const char* vs_entry = "VS", const char* ps_entry = "PS");

	void set_active();

	com_ptr<ID3D11PixelShader> pixel_shader() { return m_ps; }
	com_ptr<ID3D11VertexShader> vertex_shader() { return m_vs; }
	com_ptr<ID3DBlob> ps_bytecode() { return m_ps_bytecode; }
	com_ptr<ID3DBlob> vs_bytecode() { return m_vs_bytecode; }

private:
	com_ptr<ID3D11PixelShader> m_ps;
	com_ptr<ID3D11VertexShader> m_vs;
	com_ptr<ID3DBlob> m_ps_bytecode;
	com_ptr<ID3DBlob> m_vs_bytecode;
};


class shader_resource
{
public:
	shader_resource(com_ptr<ID3D11Texture2D> resource);

	void set_active();

	com_ptr<ID3D11ShaderResourceView> m_handle() { return m_resource; }

private:
	com_ptr<ID3D11ShaderResourceView> m_resource;
};

} //namespace