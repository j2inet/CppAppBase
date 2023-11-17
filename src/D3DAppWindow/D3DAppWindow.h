#pragma once
#include <map>
#include <string>
#include "resource.h"
#include "../CppAppBase/AppWindow.h"
#include "layout/VertexTypes.h"
#include "SmartHandle.h"

using namespace Microsoft::WRL;

std::wstring Str2Wstr(const std::string& str);
std::vector<std::wstring> split(std::wstring s, std::wstring delimiter);
std::vector<std::wstring> loadFileLines(std::wstring sourceFileName);


class D3DAppWindow : public AppWindow {
public:
	D3DAppWindow(HINSTANCE hInstance, bool EnableDebug = false);
	~D3DAppWindow();

	void Idle() override;
	void Init() override;	
	virtual void InitDeviceIndependentResources();
	virtual void InitDeviceResources();
	virtual void DiscardDeviceResources();
	virtual void LoadShader(std::wstring filePath, std::wstring key);
	virtual void Render();
	virtual void SetViewport();
	virtual void LoadShaders();
	virtual void SetShaderSource(std::wstring sourceListFile);
	virtual void CreateShaderResources();



protected:
	std::wstring GetWindowClassName() { return L"D3DAppWindow"; };


	std::map<std::wstring, ComPtr<ID3D11PixelShader>> pixelShaderMap;
	std::map<std::wstring, ComPtr<ID3D11VertexShader>> vertexShaderMap;

	std::map<std::wstring, ComPtr<ID3DBlob>> pixelShaderBlobMap;
	std::map<std::wstring, ComPtr<ID3DBlob>> vertexShaderBlobMap;

	ComPtr<IDXGISwapChain> swapchain;
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> devcon;
	
	ComPtr<ID3D11RenderTargetView> backBufferTarget;
	ComPtr<ID3D11InputLayout> vertexColorLayout;
	

	// function prototypes
	void InitD3D();     // sets up and initializes Direct3D
	virtual void InitPipeline() = 0;     // loads and prepares the shaders
	void DiscardD3D(void);         // closes Direct3D and releases memory
	bool IsDebugEnabled() { return _enableDebug;  }
	virtual void OnResize(UINT width, UINT height) override;
	virtual void Update(DOUBLE timeSinceLastFrame);	
	void OnDeviceLost();
	LARGE_INTEGER GetFrameNumber() { return _frameNumber; }
private:	
	LARGE_INTEGER _lastPerformanceValue;
	LARGE_INTEGER _frameNumber = { 0 };
	LARGE_INTEGER _lastCounterValue;

	DOUBLE  _performanceFrequency;
	bool _enableDebug;
};