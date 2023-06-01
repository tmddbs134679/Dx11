#pragma once
#include "Anack_Engine.h"
#include "AkGraphics.h"

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace Ak::graphics
{
	class GraphicDevice_Dx11
	{
	public:
		GraphicDevice_Dx11();
		~GraphicDevice_Dx11();

		bool CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* _desc, HWND _hwnd);
		bool CreateBuffer(ID3D11Buffer** _buffer, D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data);
		bool CreateShader();

		bool CreateTexture(const D3D11_TEXTURE2D_DESC* _desc, void* _data);
		
		void BindViewPort(D3D11_VIEWPORT* _ViewPort);

		void SetConstantBuffer(ID3D11Buffer* _buffer, void* _data, UINT _size);
		void BindConstantBuffer(eShaderStage _stage, eCBType _type, ID3D11Buffer* _buffer);
		void BindsConstantBuffer(eShaderStage _stage, eCBType _type, ID3D11Buffer* _buffer);
		
		void KeyInput();
		
		void Draw();


	private:
		// ���� �׷���ī�� �ϵ���� ��ü
		Microsoft::WRL::ComPtr<ID3D11Device> mDevice;


		//  dx11���� ���������� ����̽���ü ���������ʰ�
		// �̰�ü�� �̿��Ͽ� ����� ������.
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> mContext;

		// ���������� �׷��� �ؽ�ó(��ȭ��)
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mRenderTarget;

		// ����Ÿ�ٿ� ������������ �ʰ� ����Ÿ�ٺ並 ���ؼ� �����Ѵ�.
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView;

		// ���̹���
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer;

		// ���̹��ۿ� �����Ҽ� �ִ� ��
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView;

		// ������۸� �۾��� �������ִ� swapChain
		Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;

		D3D11_VIEWPORT mViewPort;

	};

	inline GraphicDevice_Dx11*& GetDivce()
	{
		static GraphicDevice_Dx11* device = nullptr;
		return device;
	}
}