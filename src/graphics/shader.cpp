#include "shader.h"

#include <iostream>
#include <d3dcompiler.h>

crlib::shader::shader(const char* shader_ascii, const char* vs_entry, const char* ps_entry) :
	m_ps(nullptr),
	m_vs(nullptr),
	m_ps_bytecode(nullptr),
	m_vs_bytecode(nullptr)
{
	HRESULT result;
	UINT flags = 0;
	size_t shader_size = strlen(shader_ascii);
	com_ptr<ID3DBlob> error_msg = nullptr;

#ifdef CRLIB_D3D_DEBUG
	flags |= D3DCOMPILE_DEBUG;
#endif 

	result = D3DCompile(shader_ascii, shader_size, nullptr, nullptr, nullptr, vs_entry, "vs_5_0", flags, 0, &m_vs_bytecode, &error_msg);

	if (FAILED(result))
	{
		std::cout << reinterpret_cast<char*>(error_msg->GetBufferPointer());
		throw std::runtime_error("Failed to compile vertex shader");
	}

	result = D3DCompile(shader_ascii, shader_size, nullptr, nullptr, nullptr, ps_entry, "ps_5_0", flags, 0, &m_ps_bytecode, &error_msg);

	if (FAILED(result))
	{
		std::cout << reinterpret_cast<char*>(error_msg->GetBufferPointer());
		throw std::runtime_error("Failed to compile pixel shader");
	}

	result = d3d_context::device()->CreateVertexShader(m_vs_bytecode->GetBufferPointer(), m_vs_bytecode->GetBufferSize(), nullptr, &m_vs);

	if (FAILED(result))
	{
		throw std::runtime_error("Failed to create vertex shader");
	}

	result = d3d_context::device()->CreatePixelShader(m_ps_bytecode->GetBufferPointer(), m_ps_bytecode->GetBufferSize(), nullptr, &m_ps);

	if (FAILED(result))
	{
		throw std::runtime_error("Failed to create pixel shader");
	}
}

void crlib::shader::set_active()
{
	d3d_context::device_context()->VSSetShader(*m_vs, nullptr, 0);
	d3d_context::device_context()->PSSetShader(*m_ps, nullptr, 0);
}


crlib::shader_resource::shader_resource(com_ptr<ID3D11Texture2D> resource) :
	m_resource(nullptr)
{
	D3D11_TEXTURE2D_DESC texture_desc;
	ZeroMemory(&texture_desc, sizeof(D3D11_TEXTURE2D_DESC));
	resource->GetDesc(&texture_desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_desc;
	ZeroMemory(&shader_resource_desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	shader_resource_desc.Format = texture_desc.Format;
	shader_resource_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shader_resource_desc.Texture2D.MostDetailedMip = texture_desc.MipLevels - 1;
	shader_resource_desc.Texture2D.MipLevels = texture_desc.MipLevels;

	HRESULT res = d3d_context::device()->CreateShaderResourceView(*resource, &shader_resource_desc, &m_resource);

	if (FAILED(res))
	{
		throw std::runtime_error("Failed to create shader resource");
	}
}

void crlib::shader_resource::set_active()
{
	d3d_context::device_context()->PSSetShaderResources(0, 1, &m_resource);
}
