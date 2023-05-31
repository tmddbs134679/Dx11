#include "AkGraphicDevice_Dx11.h"
#include "AkApplication.h"
#include "AkRenderer.h"

extern Ak::Application application;


namespace Ak::graphics
{

	graphics::GraphicDevice_Dx11::GraphicDevice_Dx11()
	{
		//Devcie, Context 생성
		HWND hWnd = application.GetHwnd();
		UINT DeviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel = (D3D_FEATURE_LEVEL)0;

		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			DeviceFlag,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			mDevice.GetAddressOf(),
			&featureLevel,
			mContext.GetAddressOf() );

		//SwapChain
		DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
		
		SwapChainDesc.BufferCount = 2;
		SwapChainDesc.BufferDesc.Width = application.GetWidth();
		SwapChainDesc.BufferDesc.Height = application.GetHeight();

		if (!CreateSwapChain(&SwapChainDesc, hWnd))
			return;

		//Get renderTarget by SwapChain
		if(FAILED(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mRenderTarget.GetAddressOf())))
			return;

		//Create RenderTarget View

		mDevice->CreateRenderTargetView(
			(ID3D11Resource*)mRenderTarget.Get(),
			nullptr,
			mRenderTargetView.GetAddressOf() );

		D3D11_TEXTURE2D_DESC depthStencilDesc = {};
		depthStencilDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.Width = application.GetWidth();
		depthStencilDesc.Height = application.GetHeight();
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.MipLevels = 0;
		depthStencilDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA Data;
		if (!CreateTexture(&depthStencilDesc, &Data))
			return;

		RECT WindRect = {};
		GetClientRect(hWnd, &WindRect);

		mViewPort =
		{
			0.f,
			0.f,
			(float)(WindRect.right - WindRect.left),
			(float)(WindRect.bottom - WindRect.top),
			0.f,
			1.f
		};

		BindViewPort(&mViewPort);



		mContext->OMSetRenderTargets(
			1,
			mRenderTargetView.GetAddressOf(),
			mDepthStencilView.Get());

	}

	graphics::GraphicDevice_Dx11::~GraphicDevice_Dx11()
	{

	}

	bool GraphicDevice_Dx11::CreateSwapChain(const DXGI_SWAP_CHAIN_DESC* _desc, HWND _hwnd)
	{
		DXGI_SWAP_CHAIN_DESC dxgiDesc = {};

		dxgiDesc.OutputWindow = _hwnd;
		//Windowed 창모드여부
		dxgiDesc.Windowed = true;
		dxgiDesc.BufferCount = _desc->BufferCount;
		dxgiDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		dxgiDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		dxgiDesc.BufferDesc.Width = _desc->BufferDesc.Width;
		dxgiDesc.BufferDesc.Height = _desc->BufferDesc.Height;
		dxgiDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dxgiDesc.BufferDesc.RefreshRate.Numerator = 240;
		dxgiDesc.BufferDesc.RefreshRate.Denominator = 1;
		dxgiDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
		dxgiDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		dxgiDesc.SampleDesc.Count = 1;
		dxgiDesc.SampleDesc.Quality = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pFactory = nullptr;

		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf())))
			return false;

		if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
			return false;

		if (FAILED(pFactory->CreateSwapChain(mDevice.Get(), &dxgiDesc, mSwapChain.GetAddressOf())))
			return false;

		return true;

	}

	bool GraphicDevice_Dx11::CreateBuffer(ID3D11Buffer** _buffer, D3D11_BUFFER_DESC* _desc, D3D11_SUBRESOURCE_DATA* _data)
	{
		/*D3D11_BUFFER_DESC TriangleDesc = {};
		TriangleDesc.ByteWidth = _desc->ByteWidth;
		TriangleDesc.BindFlags = _desc->BindFlags;
		TriangleDesc.CPUAccessFlags = _desc->CPUAccessFlags;

		D3D11_SUBRESOURCE_DATA TriangleData = {};
		TriangleData.pSysMem = vertexes;*/

		if (FAILED(mDevice->CreateBuffer(_desc, _data, _buffer)))
			return false;

		return true;
	}

	bool GraphicDevice_Dx11::CreateShader()
	{
		//ID3DBlob* vsBlob = nullptr;

		std::filesystem::path shaderPath = std::filesystem::current_path().parent_path();
		shaderPath += L"\\Shader_SOURCE\\";

		std::filesystem::path vsPath(shaderPath.c_str());
		vsPath += L"TriangleVS.hlsl";

		D3DCompileFromFile(
			vsPath.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",
			0,
			0,
			&Ak::renderer::TriangleVSBlob,
			&Ak::renderer::errorBlob );

		//TriangleVS에러 알려줌
		if (Ak::renderer::errorBlob)
		{
			OutputDebugStringA((char*)Ak::renderer::errorBlob->GetBufferPointer());
			Ak::renderer::errorBlob->Release();
		}

		mDevice->CreateVertexShader(
			Ak::renderer::TriangleVSBlob->GetBufferPointer(),
			Ak::renderer::TriangleVSBlob->GetBufferSize(),
			nullptr,
			&Ak::renderer::TriangleVSShader);


		std::filesystem::path psPath = (shaderPath.c_str());
		psPath += L"TrianglePS.hlsl";

		D3DCompileFromFile(
			psPath.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"ps_5_0",
			0,
			0,
			&Ak::renderer::TrianglePSBlob,
			&Ak::renderer::errorBlob );

		//TriangleVS에러 알려줌
		if (Ak::renderer::errorBlob)
		{
			OutputDebugStringA((char*)Ak::renderer::errorBlob->GetBufferPointer());
			Ak::renderer::errorBlob->Release();
		}

		mDevice->CreatePixelShader(
			Ak::renderer::TrianglePSBlob->GetBufferPointer(),
			Ak::renderer::TrianglePSBlob->GetBufferSize(),
			nullptr,
			&Ak::renderer::TrianglePSShader);

		//Input Layout정점 구조 정보를 넘겨줘야한다.
		D3D11_INPUT_ELEMENT_DESC ArrLayOut[2] = {};

		//Pos
		ArrLayOut[0].AlignedByteOffset = 0;
		ArrLayOut[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ArrLayOut[0].InputSlot = 0;
		ArrLayOut[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ArrLayOut[0].SemanticName = "POSITION";
		ArrLayOut[0].SemanticIndex = 0;

		//Color
		ArrLayOut[1].AlignedByteOffset = 12;
		ArrLayOut[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		ArrLayOut[1].InputSlot = 0;
		ArrLayOut[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ArrLayOut[1].SemanticName = "COLOR";
		ArrLayOut[1].SemanticIndex = 0;

		mDevice->CreateInputLayout(
			ArrLayOut,
			2,
			renderer::TriangleVSBlob->GetBufferPointer(),
			renderer::TriangleVSBlob->GetBufferSize(),
			&renderer::TriangleLayout);

		return true;
	}

	bool GraphicDevice_Dx11::CreateTexture(const D3D11_TEXTURE2D_DESC* _desc, void* _data)
	{
		D3D11_TEXTURE2D_DESC dxgiDesc = { };
		
		dxgiDesc.BindFlags = _desc->BindFlags;
		dxgiDesc.Usage = _desc->Usage;
		dxgiDesc.CPUAccessFlags = 0;

		dxgiDesc.Format = _desc->Format;
		dxgiDesc.Width = _desc->Width;
		dxgiDesc.Height = _desc->Height;
		dxgiDesc.ArraySize = _desc->ArraySize;

		dxgiDesc.SampleDesc.Count = _desc->SampleDesc.Count;
		dxgiDesc.SampleDesc.Quality = 0;

		dxgiDesc.MipLevels = _desc->MipLevels;
		dxgiDesc.MiscFlags = _desc->MiscFlags;



		if (FAILED(mDevice->CreateTexture2D(&dxgiDesc, nullptr, mDepthStencilBuffer.ReleaseAndGetAddressOf())))
			return false;

		if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			return false;

		return true;
	}

	void GraphicDevice_Dx11::BindViewPort(D3D11_VIEWPORT* _ViewPort)
	{
		mContext->RSSetViewports(1, _ViewPort);
	}

	void GraphicDevice_Dx11::Draw()
	{
		FLOAT bgColor[4] = {0.2f, 0.2f, 0.2f, 1.f};
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), bgColor);
		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);

		//viewport update
		HWND Hwnd = application.GetHwnd();
		RECT WindRect = {};
		GetClientRect(Hwnd, &WindRect);

		mViewPort =
		{
			0.f,
			0.f,
			(float)(WindRect.right - WindRect.left),
			(float)(WindRect.bottom - WindRect.top),
			0.f,
			1.f
		};
		BindViewPort(&mViewPort);
		mContext->OMSetRenderTargets(
			1,
			mRenderTargetView.GetAddressOf(),
			mDepthStencilView.Get() );

		//Input assembler 정점데이터 정보 지정
		UINT vertexsize = sizeof(renderer::Vertex);
		UINT offset = 0;
	
		mContext->IASetVertexBuffers(0, 1, &renderer::TriangleBuffer, &vertexsize, &offset);
		mContext->IASetInputLayout(renderer::TriangleLayout);
		mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		//Bind VS, PS

		mContext->VSSetShader(renderer::TriangleVSShader, 0, 0);
		mContext->PSSetShader(renderer::TrianglePSShader, 0, 0);

		//Draw Render Target
		mContext->Draw(12, 0);

		//렌더 타겟에 있는 이미지를 화면에 그려준다.
		mSwapChain->Present(0, 0);
	}
}

