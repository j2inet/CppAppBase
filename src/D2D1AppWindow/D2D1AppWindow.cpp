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
	//SetWindowLong(_hWnd, GWL_STYLE, 0);
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
		switch ((*current)->paletteIndex)
		{
		case PaletteIndex_Primary: brush = _pPrimaryBrush; break;
		case PaletteIndex_Secondary: brush = _pSecondaryBrush; break;
		case PaletteIndex_Background: brush = _pBackgroundBrush; break;
		default: brush = _pPrimaryBrush;
		}
		switch ((*current)->shapeType)
		{
			case ShapeType::ShapeType_Rectangle: 
			{
				std::shared_ptr<RectangleShape> r = std::static_pointer_cast<RectangleShape>(*current);
				_pRenderTarget->FillRectangle(r->rect, brush.Get());
				break;
			}
			case ShapeType_Ellipse: 
			{
				std::shared_ptr<EllipseShape> e = std::static_pointer_cast<EllipseShape>(*current);
				_pRenderTarget->FillEllipse(e->ellipse, brush.Get()); 
				break;
			}
			case ShapeType_Text:
			{
				std::shared_ptr<TextShape> t = std::static_pointer_cast<TextShape>(*current);
				if (t->textLayout)
				{
					ComPtr<IDWriteTextFormat> format = _pDefaultFont;
					//_pRenderTarget->DrawTextW(t->text.c_str(), t->text.size(), format.Get(), t->location, brush.Get());
					D2D1_POINT_2F p = { t->location.left, t->location.top };
					_pRenderTarget->DrawTextLayout(p, t->textLayout.Get(), brush.Get());
				}
				break;
			}
			default:
				break;
		}
		
	}
	std::wstring test = TEXT("TEST");
	D2D1_RECT_F r{20,200,800,800};
	//_pRenderTarget->DrawTextW(test.c_str(), test.size(), _pDefaultFont.Get(), &r, _pBackgroundBrush.Get(),D2D1_DRAW_TEXT_OPTIONS_NONE, DWRITE_MEASURING_MODE_NATURAL);
	//_pRenderTarget->DrawTextLayout({ 00,0 }, _pTextLayout.Get(), _pBackgroundBrush.Get());
	
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
	for (auto current = _shapeList.begin(); current != _shapeList.end(); ++current)
	{
		if ((*current)->shapeType == ShapeType_Text)
		{
			std::shared_ptr<TextShape> textShape = std::static_pointer_cast<TextShape>(*current);
			textShape->Dispose();

		}
	}
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
	TOF(DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), &_pWriteFactory));

	_primaryColor = D2D1::ColorF(0.7843F, 0.0f, 0.0f, 1.0f);
	_secondaryColor = D2D1::ColorF(0.09411, 0.09411, 0.08593, 1.0);
	_backgroundColor = D2D1::ColorF(0.91014, 0.847655, 0.75f, 1.0);

	TOF(_pWriteFactory->CreateTextFormat(
		TEXT("Segoe UI"),	//font family
		NULL,				//font collection
		DWRITE_FONT_WEIGHT_EXTRA_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		40.0f,
		TEXT("en-us"),
		&_pDefaultFont
	));

	//
	TOF(_pWriteFactory->CreateTextFormat(
		TEXT("Segoe UI"),	//font family
		NULL,				//font collection
		DWRITE_FONT_WEIGHT_EXTRA_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		18.0f,
		TEXT("en-us"),
		&_pSmallLabel
	));


	TOF(_pDefaultFont->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	TOF(_pDefaultFont->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

std::wstring  stringResult = L"RESULT";
TOF(_pWriteFactory->CreateTextLayout(
	stringResult.c_str(),
	stringResult.size(),
	_pDefaultFont.Get(),
	400, 90, 
	&_pTextLayout
));
DWRITE_TEXT_RANGE range{ 2,3 };
_pTextLayout->SetFontWeight(DWRITE_FONT_WEIGHT_EXTRA_LIGHT, range);


	_mySquare = { 20, 20, 30, 30 };

	
	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{0,0,456,104 }, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"PROCESSING", D2D1_RECT_F{ 0,0,440,104 }, TextStyle_Label, PaletteIndex_Secondary));
	
	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 0,128,456,550 }, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"RESULTS\r\n234", D2D1_RECT_F{ 0,128,456,550 }, TextStyle_Label, PaletteIndex_Secondary, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR));



	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 488,0,508,110 }, PaletteIndex_Secondary ));
	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 488,40,900,115 }, PaletteIndex_Secondary));
	_shapeList.push_back(std::make_shared<TextShape>(L"0000123456", D2D1_RECT_F{ 488,40,890,115 }, TextStyle_Label, PaletteIndex_Primary));


	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 530,130,850,180 }, PaletteIndex_Secondary));	
	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 690,130,850,180 }, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"TOTAL\r\nTIME", D2D1_RECT_F{ 530,130,850,180 }, TextStyle_SmallText, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"00:12.9", D2D1_RECT_F{ 690,134,850,180 }, TextStyle_Label, PaletteIndex_Background));

	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 530,190,850,240 }, PaletteIndex_Secondary));
	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 690,190,850,240 }, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"00:00.0", D2D1_RECT_F{ 530,190,850,240 }, TextStyle_Label, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"00:12.9", D2D1_RECT_F{ 690,190,850,240 }, TextStyle_Label, PaletteIndex_Background));

	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 530,250,850,300 }, PaletteIndex_Secondary));
	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 690,250,850,300 }, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"00:45.0", D2D1_RECT_F{ 530,250,850,300 }, TextStyle_Label, PaletteIndex_Primary));
	_shapeList.push_back(std::make_shared<TextShape>(L"00:23.9", D2D1_RECT_F{ 690,250,850,300 }, TextStyle_Label, PaletteIndex_Background));

	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 0,350,600,450 }, PaletteIndex_Secondary));
	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 0,350,600,450 }, PaletteIndex_Secondary));

	_shapeList.push_back(std::make_shared<EllipseShape>(D2D1_ELLIPSE{ 570,400,60, 60 }, PaletteIndex_Secondary));
	_shapeList.push_back(std::make_shared<EllipseShape>(D2D1_ELLIPSE{ 570,400,57, 57 }, PaletteIndex_Background));
	_shapeList.push_back(std::make_shared<EllipseShape>(D2D1_ELLIPSE{ 570,400,55, 55 }, PaletteIndex_Secondary));
	_shapeList.push_back(std::make_shared<EllipseShape>(D2D1_ELLIPSE{ 570,400,52, 52 }, PaletteIndex_Background));

	_shapeList.push_back(std::make_shared<TextShape>(L"99%", D2D1_RECT_F{ 510,330,630,460 }, TextStyle_Label, PaletteIndex_Primary, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

	_shapeList.push_back(std::make_shared<RectangleShape>(D2D1_RECT_F{ 476,470,870, 550 }, PaletteIndex_Secondary));

	
}



void D2D1AppWindow::OnPaint()
{
	this->OnRender();
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
	for (auto current = _shapeList.begin(); current != _shapeList.end(); ++current)
	{
		if ((*current)->shapeType == ShapeType_Text)
		{
			std::shared_ptr<TextShape> textShape = std::static_pointer_cast<TextShape>(*current);
			ComPtr<IDWriteTextFormat> textFormat = _pDefaultFont;
			switch (textShape->textStyle)
			{
			case TextStyle_Label: textFormat = _pDefaultFont; break;
			case TextStyle_SmallText: textFormat = _pSmallLabel; break;
			}
			_pWriteFactory->CreateTextLayout(textShape->text.c_str(), textShape->text.size(), textFormat.Get(), textShape->location.right - textShape->location.left, textShape->location.bottom - textShape->location.top, &textShape->textLayout);
			textShape->textLayout->SetTextAlignment(textShape->textAlignment);
			textShape->textLayout->SetParagraphAlignment(textShape->paragraphAlignment);
			
		}
	}
}

