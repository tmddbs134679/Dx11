#include "AkApplication.h"
#include "AkInput.h"
#include "AkTime.h"
#include "AkRenderer.h"

namespace Ak
{

	Application::Application()
		:graphicDevice(nullptr)
		, mHwnd(NULL)
		, mWidth(-1)
		, mHeight(-1)
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();

	}

	void Application::Initialize()
	{
		Time::Initiailize();
		Input::Initialize();

		renderer::Initialize();


	}

	void Application::Update()
	{
		Time::Update();
		Input::Update();
	}

	void Application::LateUpdate()
	{
	}

	void Application::Render()
	{
		Time::Render();
		graphicDevice->Draw();

	}

	void Application::SetWindow(HWND _hwnd, UINT _width, UINT _height)
	{
		if (graphicDevice == nullptr)
		{
			mHwnd = _hwnd;
			mWidth = _width;
			mHeight = _height;

			graphicDevice = std::make_unique<Ak::graphics::GraphicDevice_Dx11>();
			Ak::graphics::GetDivce() = graphicDevice.get();

		}

		RECT rt = { 0, 0, (LONG)_width , (LONG)_height };
		AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
		SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
		ShowWindow(mHwnd, true);
		UpdateWindow(mHwnd);
	}

}
