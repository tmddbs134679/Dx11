#include "AkMesh.h"
#include "AkRenderer.h"

namespace Ak
{
	Mesh::Mesh()
		: mVertexBuffer(nullptr)
		, mIndexBuffer(nullptr)
		, mVBDesc{}
		, mIBDesc{}
		, mIndexCount(0)
	{
	}

	Mesh::~Mesh()
	{
	}

	HRESULT Mesh::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	bool Mesh::CreateVertexBuffer(void* _data, UINT _Count)
	{
		mVBDesc.ByteWidth = sizeof(renderer::Vertex) * _Count;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = _data;

		if (!GetDevice()->CreateBuffer(mVertexBuffer.GetAddressOf(), &mVBDesc, &sub))
			return false;

		return true;
	}

	bool Mesh::CreateIndexBuffer(void* _data, UINT _Count)
	{
		mIndexCount = _Count;
		mIBDesc.ByteWidth = sizeof(UINT) * _Count;
		mIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		mIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mIBDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = _data;

		if (!GetDevice()->CreateBuffer(mIndexBuffer.GetAddressOf(), &mIBDesc, &sub))
			return false;

		return true;

	}

	void Mesh::BindBuffer()
	{
		//Input assembler 정점데이터 정보 지정
		UINT vertexsize = sizeof(renderer::Vertex);
		UINT offset = 0;


		GetDevice()->BindVertexBuffer(0, mVertexBuffer.GetAddressOf(), &vertexsize, &offset);
		GetDevice()->BindIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	}

}