// D3DAppWindow.cpp : Defines the entry point for the application.
//
#include "framework.h"
#include <cctype>
#include <string>
#include <format>
#include "D3DAppWindow.h"
#include "../common/Exception.h"
#include "pch.h"




std::vector<std::wstring> loadFileLines(std::wstring sourceFileName)
{
	std::vector<std::wstring> retVal;
	HANDLE hFile = CreateFile(sourceFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return retVal;
	INT64 fileSize;
	DWORD fileSizeHigh;
	fileSize = GetFileSize(hFile, &fileSizeHigh);
	fileSize = fileSize | (fileSizeHigh << 32);
	if (fileSize != 0)
	{
		auto buffer = new char[fileSize + 1];
		ZeroMemory(buffer, fileSize + 1);
		DWORD bytesRead;
		ReadFile(hFile, buffer, fileSize, &bytesRead, FALSE);
		CloseHandle(hFile);
		std::wstring fileContents = Str2Wstr(buffer);
		delete buffer;
		retVal = split(fileContents, L"\n");
	}
	return retVal;
}

std::vector<std::wstring> split(std::wstring s, std::wstring delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::wstring token;
	std::vector<std::wstring> res;
	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}
	res.push_back(s.substr(pos_start));
	return res;
}

void D3DAppWindow::OnResize(UINT width, UINT height)
{
	AppWindow::OnResize(width, height);
	if (this->dev != nullptr && this->swapchain != nullptr)
	{
		devcon->OMSetRenderTargets(0, 0, 0);
		backBufferTarget = nullptr;
		HRESULT hr = this->swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			// If the device was removed for any reason, a new device and swapchain will need to be created.
			this->OnDeviceLost();
			return;
		}
		else if (hr != S_OK)
		{
			ErrorLogger::Log(L"Failed to resize swapchain buffers.", hr);
			TOF(hr);
		}

		ComPtr<ID3D11Texture2D> backBuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
		hr = dev->CreateRenderTargetView(backBuffer.Get(), NULL, &backBufferTarget);

		backBuffer = nullptr;
		devcon->OMSetRenderTargets(1, backBufferTarget.GetAddressOf(), NULL);

		D3D11_VIEWPORT vp = {0, 0, GetClientWidth(), GetClientHeight(), 0.0f, 1.0f};
		devcon->RSSetViewports(1, &vp);

	}
}


void D3DAppWindow::Update(DOUBLE)
{
	++_frameNumber.QuadPart;

}


void D3DAppWindow::OnDeviceLost()
{
	/*
		DiscardDeviceResources();
	HRESULT hr = this->swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		// If the device was removed for any reason, a new device and swapchain will need to be created.
		this->OnDeviceLost();
		return;
	}
	else
		TOF(hr);
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer);
	hr = dev->CreateRenderTargetView(backBuffer.Get(), NULL, &renderTargetView);
	backBuffer = nullptr;
	devcon->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), NULL);
	DiscardDeviceResources();
	this->swapchain->ResizeBuffers(2, GetClientWidth(), GetClientHeight(), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	this->InitDeviceResources();
	*/
}
std::wstring Str2Wstr(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

D3DAppWindow::D3DAppWindow(HINSTANCE hInstance, bool EnableDebug) : AppWindow(hInstance)
{

}


D3DAppWindow::~D3DAppWindow()
{
	DiscardDeviceResources();
}



void D3DAppWindow::Init()
{
	AppWindow::Init();
	InitD3D();
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	this->_performanceFrequency = static_cast<DOUBLE>(performanceFrequency.QuadPart);
	QueryPerformanceCounter(&_lastPerformanceValue);	
}


HRESULT D3DReadFileToBlob(
	LPCWSTR pFileName,
	ID3DBlob** ppContents
);

void D3DAppWindow::SetShaderSource(std::wstring sourceListFile)
{
	auto shaderFileList = loadFileLines(sourceListFile.c_str());
	for (auto it = shaderFileList.begin(); it != shaderFileList.end(); ++it)
	{
		auto parts = split(*it, L",");
		if (parts.size() == 3)
		{
			std::wstring objName = parts[0];
			std::wstring fileName = parts[1];
			std::wstring shaderType = parts[2];
			if(shaderFileList.size() > 0 && shaderType[shaderType.size() - 1] == L'\r')
				shaderType.erase(shaderType.size() - 1);
			//shaderType.erase(std::remove_if(shaderType.begin(), shaderType.end(), std::isspace), shaderType.end());

			std::wstring fileSource = fileName + L".cso";
			
			if (shaderType == StringConverter::Str2Wstr("ps"))
			{
				ComPtr<ID3DBlob> blob;
				D3DReadFileToBlob(fileSource.c_str(), &blob);
				pixelShaderBlobMap.emplace(objName, blob);				
			}
			else if (shaderType == StringConverter::Str2Wstr("vs"))
			{
				ComPtr<ID3DBlob> blob;
				D3DReadFileToBlob(fileSource.c_str(), &blob);
				vertexShaderBlobMap.emplace(objName, blob);
			}
			else
			{
				ErrorLogger::Log(L"Unknown shader type: " + shaderType);
			}
		}
	}
}


void D3DAppWindow::CreateShaderResources()
{
	for (auto const& [key, blob] : pixelShaderBlobMap)
	{
		if (!pixelShaderMap.contains(key))
		{
			ComPtr<ID3D11PixelShader> pixelShader;
			auto size = blob->GetBufferSize();
			HRESULT result = dev->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &pixelShader);
			if (!FAILED(result))
			{
				auto msg = std::format(L"Created pixel shader {}\r\n", key);
				OutputDebugString(msg.c_str());
				pixelShaderMap.emplace(key, pixelShader);
			}
			else
			{
				auto msg = std::format(L"Failed to create pixel shader {}\r\n", key);
				OutputDebugString(msg.c_str());
			}
		}
	}
	for (auto const& [key, blob] : vertexShaderBlobMap)
	{		
		if (!vertexShaderMap.contains(key))
		{
			ComPtr<ID3D11VertexShader> vertexShader;
			auto size = blob->GetBufferSize();
			HRESULT result = dev->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vertexShader);
			if (!FAILED(result))
			{
				auto msg = std::format(L"Created vertex shader {}\r\n", key);
				OutputDebugString(msg.c_str());
				vertexShaderMap.emplace(key, vertexShader);
			}
			else
			{
				auto msg = std::format(L"Failed to create vertex shader {}\r\n", key);
				OutputDebugString(msg.c_str());
			}
		}
	}
}


void D3DAppWindow::InitPipeline()
{
	devcon->VSSetShader(vertexShaderMap.at(L"vertexShader").Get(), 0, 0);
	devcon->PSSetShader(pixelShaderMap.at(L"pixelShader").Get(), 0, 0);
}


void D3DAppWindow::InitDeviceIndependentResources()
{

}

void D3DAppWindow::InitDeviceResources()
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 2;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = GetClientWidth();                   // set the back buffer width	
	scd.BufferDesc.Height = GetClientHeight();                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = _hWnd;                           // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching
	// create a device, device context and swap chain using the information in the scd struct
	DWORD createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT | ((IsDebugEnabled()) ? D3D11_CREATE_DEVICE_DEBUG : 0);

	D3D_FEATURE_LEVEL lvl[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1 
	};

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = (D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createFlags,
		lvl,
		_countof(lvl),
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		&featureLevel,
		&devcon));
	if (!SUCCEEDED(hr) && ((createFlags & D3D11_CREATE_DEVICE_DEBUG )== D3D11_CREATE_DEVICE_DEBUG))
	{
		createFlags = createFlags & (~D3D11_CREATE_DEVICE_DEBUG);
		TOF(hr = (D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			createFlags,
			lvl,
			_countof(lvl),
			D3D11_SDK_VERSION,
			&scd,
			&swapchain,
			&dev,
			&featureLevel,
			&devcon)));
	} 
	else
	{
		TOF(hr);
	}
	//auto  featureLevel = dev->GetFeatureLevel();
	ComPtr<ID3D11Texture2D> backBuffer;
	TOF(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	TOF(dev->CreateRenderTargetView(backBuffer.Get(), NULL, backBufferTarget.GetAddressOf()));
	backBuffer = nullptr;
	devcon->OMSetRenderTargets(1, backBufferTarget.GetAddressOf(), nullptr);
	SetViewport();
	CreateShaderResources();
	InitPipeline();
}


void D3DAppWindow::DiscardDeviceResources()
{

	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
	devcon->OMSetRenderTargets(0, 0, 0);
		
	swapchain = nullptr;
	backBufferTarget = nullptr;
	
		pixelShaderMap.clear();
		vertexShaderMap.clear();
		devcon = nullptr;
		dev = nullptr;


}


void D3DAppWindow::Idle()
{
	LARGE_INTEGER currentPerformanceValue;
	QueryPerformanceCounter(&currentPerformanceValue);
	DOUBLE elapsedSeconds = static_cast<DOUBLE>(currentPerformanceValue.QuadPart - _lastPerformanceValue.QuadPart) / _performanceFrequency;
	if (elapsedSeconds > 1.0 / 120.0) {
		_lastPerformanceValue = currentPerformanceValue;
		Render();
		++_frameNumber.QuadPart;		
		Update(elapsedSeconds);
	}
	
}

void D3DAppWindow::InitD3D()
{
	InitDeviceResources();
	InitDeviceIndependentResources();
}


void D3DAppWindow::LoadShaders()
{

}

void D3DAppWindow::LoadShader(std::wstring filePath, std::wstring key)
{
	
}

void D3DAppWindow::Render()
{
	FLOAT backgroundColor[4] = {0.0f, 0.2f, 0.4f, 1.0f};
	devcon->ClearRenderTargetView(backBufferTarget.Get(), backgroundColor);
	swapchain->Present(0, 0);
}

void D3DAppWindow::SetViewport()
{
	D3D11_VIEWPORT viewport = { 0 };
	RECT clientRect = this->ClientRect();
	viewport.Width = static_cast<FLOAT>(clientRect.right - clientRect.left);
	viewport.Height = static_cast<FLOAT>(clientRect.bottom - clientRect.top);
	devcon->RSSetViewports(1, &viewport);
}