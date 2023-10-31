#pragma once

#include "resource.h"
#include "../CppAppBase/AppWindow.h"
#include "VertexTypes.h"

using namespace Microsoft::WRL;


class D3DAppWindow : public AppWindow {
public:
	D3DAppWindow(HINSTANCE hInstance);
	~D3DAppWindow();

	void Idle() override;
	void Init() override;
	virtual void InitDeviceIndependentResources();
	virtual void InitDeviceResources();
	virtual void DiscardDeviceResources();
	virtual void LoadShader(std::wstring filePath, std::wstring key);
	virtual void Render();
	virtual void SetViewport();


protected:
	std::wstring GetWindowClassName() { return L"D3DAppWindow"; };



	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> devcon;
	ComPtr<ID3D11Texture2D> backBuffer;
	ComPtr<ID3D11RenderTargetView> backBufferTarget;
	ComPtr<ID3D11InputLayout> vertexColorLayout;

	// function prototypes
	void InitD3D();     // sets up and initializes Direct3D
	void DiscardD3D(void);         // closes Direct3D and releases memory

private:	
	LARGE_INTEGER _lastPerformanceValue;
	DOUBLE  _performanceFrequency;
};