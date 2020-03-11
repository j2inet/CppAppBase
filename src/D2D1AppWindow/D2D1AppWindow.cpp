// D2D1AppWindow.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "D2D1AppWindow.h"
#include "../common/Exception.h"


D2D1AppWindow::D2D1AppWindow(HINSTANCE hInstance):AppWindow(hInstance)
{

}

std::wstring D2D1AppWindow::GetWindowClassName()
{
	return TEXT("D2D1AppWindow");
}


void D2D1AppWindow::Init()
{
	AppWindow::Init();
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	this->_performanceFrequency = static_cast<DOUBLE>(performanceFrequency.QuadPart);
	QueryPerformanceCounter(&_lastPerformanceValue);
	InitDeviceIndependentResources();
	InitDeviceResources();
}

void D2D1AppWindow::CloseWindow()
{
	AppWindow::CloseWindow();

}


void D2D1AppWindow::Idle()
{
	AppWindow::Idle();
	Update();
	Render();
}


void D2D1AppWindow::Update()
{
	LARGE_INTEGER recentValue;
	QueryPerformanceCounter(&recentValue);

	FLOAT timePassed = static_cast<FLOAT>(
		recentValue.QuadPart - _lastPerformanceValue.QuadPart
		) / static_cast<FLOAT>(_performanceFrequency); 

	_lastPerformanceValue = recentValue;
	this->OnUpdate(timePassed, _lastPerformanceValue.QuadPart);
}


void D2D1AppWindow::OnUpdate(FLOAT timeSinceLastUpdate, LONGLONG globalTimer)
{
}

void D2D1AppWindow::Render()
{
	this->OnRender();
}


void D2D1AppWindow::OnRender()
{
	HRESULT hr;
	if (!_pRenderTarget)
		return;
	_pRenderTarget->BeginDraw();

	hr = _pRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		// The surface has been lost. We need to recreate our
		// device dependent resources. 
		this->DiscardDeviceResources();
		this->InitDeviceResources();
	}
}


void D2D1AppWindow::DiscardDeviceResources()
{
	_pRenderTarget = nullptr;
}



void D2D1AppWindow::InitDeviceResources()
{

	TOF(_pD2D1Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(_hWnd, _size),
		&_pRenderTarget
	));
}

void D2D1AppWindow::InitDeviceIndependentResources()
{
	TOF(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&_pD2D1Factory)));
}