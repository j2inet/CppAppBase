#pragma once

#include "framework.h"
#include "AppWindow.h"
#include <wrl/client.h>



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
	TextStyle_Label = 0,
	TextStyle_SmallText = 1
};

interface  IDispose {
	virtual void Dispose() = 0;
};

struct Shape: public IDispose {
	Shape() {}

	Shape(ShapeType shapeType, PaletteIndex paletteIndex)
	{
		this->shapeType = shapeType;
		this->paletteIndex = paletteIndex;
	}
	virtual void Dispose()
	{

	}
	std::wstring tag;
	ShapeType shapeType;
	PaletteIndex paletteIndex;
};

struct TextShape :public Shape {
	TextShape() {}
	TextShape(std::wstring text, D2D1_RECT_F location, TextStyle textStyle = TextStyle_Label, PaletteIndex palette = PaletteIndex_Primary, 
		DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING, 
		DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_CENTER)
		:Shape(ShapeType_Text, palette)
	{
		this->text = text;
		this->location = location;
		this->textStyle = textStyle;
		this->paragraphAlignment = paragraphAlignment;
		this->textAlignment = textAlignment;
	}

	void Dispose() override 
	{
		this->textLayout = nullptr;
	}
	std::wstring text;
	D2D1_RECT_F location;
	TextStyle textStyle;
	ComPtr<IDWriteTextLayout> textLayout;
	DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment;
	DWRITE_TEXT_ALIGNMENT textAlignment;

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
	void RequestRender();
	void EnableAutoRender(boolean isEnabled);
private:
	void Update();
	void Render();
	inline bool IsRenderNeeded()
	{
		return (_alwaysRender || _renderWasRequested);
	}
	D2D1_RECT_F _mySquare;
	bool _alwaysRender;
	bool _renderWasRequested;

	std::vector<std::shared_ptr<Shape>> _shapeList;


	ComPtr<ID2D1Factory>  _pD2D1Factory;
	ComPtr<ID2D1HwndRenderTarget> _pRenderTarget;

	D2D1_COLOR_F _primaryColor, _secondaryColor, _backgroundColor;;
	ComPtr<ID2D1SolidColorBrush> _pBackgroundBrush,
		_pPrimaryBrush,
		_pSecondaryBrush;


	ComPtr< IDWriteFactory> _pWriteFactory;
	ComPtr<IDWriteTextFormat> _pDefaultFont, _pSmallLabel;
	ComPtr<IDWriteTextLayout> _pTextLayout;


	D2D1_SIZE_U _size;

	LARGE_INTEGER _lastPerformanceValue;
	DOUBLE  _performanceFrequency;
};