#pragma once

#include "framework.h"
#include "AppWindow.h"

using namespace Microsoft::WRL;


enum PaletteIndex {
	Primary = 0,
	Secondary = 1,
	Background = 2
};

enum class ShapeType {
	ShapeType_Rectangle = 0, 
	ShapeType_Ellipse = 1
};

struct ColoredShape
{
	static ColoredShape MakeEllipse(D2D1_ELLIPSE e, PaletteIndex p)
	{
		ColoredShape c{ 0 };
		c.shapeType = ShapeType::ShapeType_Ellipse;
		c.ellipse = e;
		c.paletteIndex = p;
		return c;
	}

	static ColoredShape MakeRectangle(D2D1_RECT_F r, PaletteIndex p)
	{
		ColoredShape c{ 0 };
		c.shapeType = ShapeType::ShapeType_Rectangle;
		c.rect = r;
		c.paletteIndex = p;
		return c;
	}

	union  {
		D2D1_ELLIPSE ellipse;
		D2D1_RECT_F rect;
	};
	ShapeType shapeType;
	PaletteIndex paletteIndex;
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

	std::vector< ColoredShape> _shapeList;


	ComPtr<ID2D1Factory>  _pD2D1Factory;
	ComPtr<ID2D1HwndRenderTarget> _pRenderTarget;

	D2D1_COLOR_F _primaryColor, _secondaryColor, _backgroundColor;;
	ComPtr<ID2D1SolidColorBrush> _pBackgroundBrush,
		_pPrimaryBrush,
		_pSecondaryBrush;
	D2D1_SIZE_U _size;

	LARGE_INTEGER _lastPerformanceValue;
	DOUBLE  _performanceFrequency;
};