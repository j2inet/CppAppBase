#pragma once

#include "framework.h"
#include "AppWindow.h"

using namespace Microsoft::WRL;


enum   PaletteIndex {
	PaletteIndex_Primary = 0,
	PaletteIndex_Secondary = 1,
	PaletteIndex_Background = 2
};

enum ShapeType {
	ShapeType_Rectangle = 0, 
	ShapeType_Ellipse = 1,
	ShapeType_Text
};

enum TextStyle {
	TextStyle_Label = 0
};

struct Shape {
	Shape() {}

	Shape(ShapeType shapeType, PaletteIndex paletteIndex)
	{
		this->shapeType = shapeType;
		this->paletteIndex = paletteIndex;
	}
	std::wstring tag;
	ShapeType shapeType;
	PaletteIndex paletteIndex;
};

struct TextShape :public Shape {
	TextShape() {}
	TextShape(std::wstring text, D2D1_RECT_F location, TextStyle textStyle = TextStyle_Label, PaletteIndex palette = PaletteIndex_Primary)
		:Shape(ShapeType_Text, palette)
	{
		this->text = text;
		this->location = location;
		this->textStyle = textStyle;
	}

	std::wstring text;
	D2D1_RECT_F location;
	TextStyle textStyle;
	ComPtr<IDWriteTextLayout> textLayout;

};

struct RectangleShape : public Shape {
	RectangleShape() :Shape(ShapeType_Rectangle, PaletteIndex_Primary) {

	}
	RectangleShape(D2D1_RECT_F rect, PaletteIndex p) :
		Shape(ShapeType_Rectangle, p)
	{
		this->rect = rect;
	}
	D2D1_RECT_F rect;
};

struct EllipseShape : public Shape {
	EllipseShape():Shape(ShapeType_Ellipse, PaletteIndex_Primary)
	{}
	EllipseShape(D2D1_ELLIPSE ellipse, PaletteIndex p = PaletteIndex_Primary)
		:Shape(ShapeType_Ellipse, p)
	{
		this->ellipse = ellipse;
	}

	D2D1_ELLIPSE ellipse;
	
};





class D2D1AppWindow : public AppWindow {
public:
	D2D1AppWindow(HINSTANCE hInstance);

	std::wstring GetWindowClassName() override;
	void Init() override;
	

protected:
	virtual void InitDeviceResources();
	virtual void InitDeviceIndependentResources();
	virtual void DiscardDeviceResources();
	void CloseWindow() override;
	void OnPaint() override;

	void Idle() override;

	virtual void OnUpdate(FLOAT timeSinceLastUpdate, LONGLONG globalTimer);
	virtual void OnRender();

	void OnResize(UINT width, UINT height) override;

private:
	void Update();
	void Render();

	D2D1_RECT_F _mySquare;

	std::vector<std::shared_ptr<Shape>> _shapeList;


	ComPtr<ID2D1Factory>  _pD2D1Factory;
	ComPtr<ID2D1HwndRenderTarget> _pRenderTarget;

	D2D1_COLOR_F _primaryColor, _secondaryColor, _backgroundColor;;
	ComPtr<ID2D1SolidColorBrush> _pBackgroundBrush,
		_pPrimaryBrush,
		_pSecondaryBrush;


	ComPtr< IDWriteFactory> _pWriteFactory;
	ComPtr<IDWriteTextFormat> _pDefaultFont;
	ComPtr<IDWriteTextLayout> _pTextLayout;


	D2D1_SIZE_U _size;

	LARGE_INTEGER _lastPerformanceValue;
	DOUBLE  _performanceFrequency;
};