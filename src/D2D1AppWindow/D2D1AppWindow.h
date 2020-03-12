#pragma once

#include "framework.h"
#include "AppWindow.h"

using namespace Microsoft::WRL;

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

	void Idle() override;

	virtual void OnUpdate(FLOAT timeSinceLastUpdate, LONGLONG globalTimer);
	virtual void OnRender();

	void OnResize(UINT width, UINT height) override;

private:
	void Update();
	void Render();

	ComPtr<ID2D1Factory>  _pD2D1Factory;
	ComPtr<ID2D1HwndRenderTarget> _pRenderTarget;
	ComPtr<ID2D1SolidColorBrush> _pBackgroundBrush;
	D2D1_SIZE_U _size;

	LARGE_INTEGER _lastPerformanceValue;
	DOUBLE  _performanceFrequency;
};