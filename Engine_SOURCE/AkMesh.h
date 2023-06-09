#pragma once
#include "AkResource.h"

#include "AkGraphicDevice_Dx11.h"

namespace Ak
{
	using namespace graphics;

	class Mesh :
		public Resource
	{
	public:

		Mesh();
		~Mesh();

		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVertexBuffer(void* _data, UINT _Count);
		bool CreateIndexBuffer(void* _data, UINT _Count);

		void BindBuffer();

		UINT GetIndexCount()
		{
			return mIndexCount;
		}


	private:

		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;

		D3D11_BUFFER_DESC mVBDesc;
		D3D11_BUFFER_DESC mIBDesc;

		UINT mIndexCount;
	};

}
