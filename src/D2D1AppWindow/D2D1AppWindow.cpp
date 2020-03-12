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
	{
		RECT r;
		GetClientRect(_hWnd, &r);
		this->_size = D2D1_SIZE_U{ static_cast<UINT32>(r.right - r.left), static_cast<UINT32>(r.bottom - r.top) };
	}
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
	_pRenderTarget->Clear(_backgroundColor);
	_pRenderTarget->FillRectangle(&_mySquare, _pPrimaryBrush.Get());
	_pRenderTarget->DrawRectangle(&_mySquare, _pSecondaryBrush.Get());

	for (auto current = _shapeList.begin(); current != _shapeList.end(); ++current)
	{
		ComPtr<ID2D1Brush> brush;
		switch (current->paletteIndex)
		{
		case Primary: brush = _pPrimaryBrush; break;
		case Secondary: brush = _pSecondaryBrush; break;
		case Background: brush = _pBackgroundBrush; break;
		default: brush = _pPrimaryBrush;
		}
		switch (current->shapeType)
		{
		case ShapeType::ShapeType_Rectangle:_pRenderTarget->FillRectangle(&current->rect, brush.Get()); break;
		case ShapeType::ShapeType_Ellipse:_pRenderTarget->FillEllipse(&current->ellipse, brush.Get()); break;
		}
		
	}
	
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
	_pBackgroundBrush = nullptr;
	_pRenderTarget = nullptr;
}

void D2D1AppWindow::OnResize(UINT width, UINT height)
{
	this->_size = D2D1_SIZE_U{ width, height };
	if (_pRenderTarget)
	{
		_pRenderTarget->Resize(_size);
	}
}



void D2D1AppWindow::InitDeviceIndependentResources()
{
	TOF(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&_pD2D1Factory)));
	_primaryColor = D2D1::ColorF(0.7843F, 0.0f, 0.0f, 1.0f);
	_secondaryColor = D2D1::ColorF(0.09411, 0.09411, 0.08593, 1.0);
	_backgroundColor = D2D1::ColorF(0.91014, 0.847655, 0.75f, 1.0);
	_mySquare = { 20, 20, 30, 30 };

	
	_shapeList.push_back(ColoredShape::MakeRectangle({0,0,456,104 }, Primary));
	_shapeList.push_back(ColoredShape::MakeRectangle({ 0,128,456,550 }, Primary ));
	_shapeList.push_back(ColoredShape::MakeRectangle({ 488,0,508,110 }, Secondary ));
	_shapeList.push_back(ColoredShape::MakeRectangle({ 488,40,900,115 }, Secondary ));

	_shapeList.push_back(ColoredShape::MakeRectangle({ 530,130,850,180 }, Primary ));
	_shapeList.push_back(ColoredShape::MakeRectangle({ 530,190,850,240 }, Primary ));
	_shapeList.push_back(ColoredShape::MakeRectangle({ 530,250,850,300 }, Primary ));

	_shapeList.push_back(ColoredShape::MakeRectangle({ 0,350,600,450 }, Secondary ));
	_shapeList.push_back(ColoredShape::MakeRectangle({ 0,350,600,450 }, Secondary));

	_shapeList.push_back(ColoredShape::MakeEllipse({ 570,400,60, 60 }, Secondary));
	_shapeList.push_back(ColoredShape::MakeEllipse({ 570,400,57, 57 }, Background));
	_shapeList.push_back(ColoredShape::MakeEllipse({ 570,400,55, 55 }, Secondary));
	_shapeList.push_back(ColoredShape::MakeEllipse({ 570,400,52, 52 }, Background));

	_shapeList.push_back(ColoredShape::MakeRectangle({ 476,470,870, 550 }, Secondary));
}


void D2D1AppWindow::InitDeviceResources()
{

	TOF(_pD2D1Factory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(_hWnd, _size),
		&_pRenderTarget
	));
	
	TOF(_pRenderTarget->CreateSolidColorBrush(_primaryColor, &_pPrimaryBrush));
	TOF(_pRenderTarget->CreateSolidColorBrush(_secondaryColor, &_pSecondaryBrush));
	TOF(_pRenderTarget->CreateSolidColorBrush(_backgroundColor, &_pBackgroundBrush));
}

