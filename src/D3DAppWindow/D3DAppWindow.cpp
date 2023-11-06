// D3DAppWindow.cpp : Defines the entry point for the application.
//
#include "framework.h"
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

std::wstring Str2Wstr(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

D3DAppWindow::D3DAppWindow(HINSTANCE hInstance) : AppWindow(hInstance)
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
	//InitDeviceResources();
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
			std::wstring shaderVersion = parts[2];

			std::wstring fileSource = fileName + L".cso";
			ComPtr<ID3D11PixelShader> ps;
			ComPtr<ID3DBlob> blob;

			D3DReadFileToBlob(fileSource.c_str(), &blob);
			shaderBlobMap.emplace(objName, blob);
		}
	}
}

void D3DAppWindow::InitDeviceIndependentResources()
{

}

void D3DAppWindow::InitDeviceResources()
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	// clear out the struct for use	
	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = GetClientWidth();                   // set the back buffer width
	scd.BufferDesc.Height = GetClientHeight();                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = _hWnd;                           // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching
	// create a device, device context and swap chain using the information in the scd struct
	TOF(D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon));

	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	dev->CreateRenderTargetView(backBuffer.Get(), NULL, backBufferTarget.GetAddressOf());
	backBuffer->Release();

	devcon->OMSetRenderTargets(1, backBufferTarget.GetAddressOf(), nullptr);

	//dev->CreateInputLayout(&vertexColorLayout, 2, )
}


void D3DAppWindow::DiscardDeviceResources()
{
	swapchain->Release();
	dev->Release();
	devcon->Release();
}


void D3DAppWindow::Idle()
{
	LARGE_INTEGER currentPerformanceValue;
	QueryPerformanceCounter(&currentPerformanceValue);
	DOUBLE elapsedSeconds = static_cast<DOUBLE>(currentPerformanceValue.QuadPart - _lastPerformanceValue.QuadPart) / _performanceFrequency;
	if (elapsedSeconds > 1.0 / 60.0) {
		_lastPerformanceValue = currentPerformanceValue;
		Render();
		swapchain->Present(0, 0);
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
}

void D3DAppWindow::SetViewport()
{
	D3D11_VIEWPORT viewport = { 0 };
	RECT clientRect = this->ClientRect();
	viewport.Width = static_cast<FLOAT>(clientRect.right - clientRect.left);
	devcon->RSSetViewports(1, &viewport);
}