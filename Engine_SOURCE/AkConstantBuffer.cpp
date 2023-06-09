#include "AkConstantBuffer.h"
#include "AkGraphicDevice_Dx11.h"


namespace Ak::graphics
{
	ConstantBuffer::ConstantBuffer(const eCBType _type)
		: GpuBuffer()
		, mType(_type)
	{
	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(size_t _size)
	{
		desc.ByteWidth = _size;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		Ak::graphics::GetDevice()->CreateBuffer(buffer.GetAddressOf(), &desc, nullptr);

		return false;
	}

	void ConstantBuffer::SetData(void* _data)
	{
		Ak::graphics::GetDevice()->SetConstantBuffer(buffer.Get(), _data, desc.ByteWidth);
	}

	void ConstantBuffer::Bind(eShaderStage _stage)
	{
		Ak::graphics::GetDevice()->BindConstantBuffer(_stage, mType, buffer.Get());
	}
}