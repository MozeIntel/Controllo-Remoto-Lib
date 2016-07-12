#pragma once

#include <Windows.h>
#include <d3d11_1.h>
#include "..\utils\com_ptr.h"

namespace crlib
{ 

class d3d_context
{
private:
	struct initializer
	{
		initializer();
	};

public:
	static com_ptr<ID3D11Device> device() { return m_device; }
	static com_ptr<ID3D11DeviceContext> device_context() { return m_device_ctx; }

protected:

private:
	static com_ptr<ID3D11Device> m_device;
	static com_ptr<ID3D11DeviceContext> m_device_ctx;
	static initializer m_initializer;
};


} //namespace