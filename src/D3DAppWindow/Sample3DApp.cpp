#include "framework.h"
#include "Sample3DApp.h"
#include "../common/Exception.h"
#include "../common/StringConverter.h"
#include "layout/VertexTypes.h"
#include "../common/ErrorLogger.h"


Sample3DApp::Sample3DApp(HINSTANCE hInstance)
	: D3DAppWindow(hInstance, true)
{
	SetShaderSource(L"shaderList.txt");
}

void Sample3DApp::InitDeviceResources()
{
	D3DAppWindow::InitDeviceResources();
	HRESULT result;
	CreateShaderResources();
	ComPtr<ID3DBlob> vertexShaderBlob = vertexShaderBlobMap.at(L"MyVShader");
	TOF(result = dev->CreateInputLayout(
		VERTEX_COLORED_DESCRIPTION,
		2,
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		&inputLayout
	));
	devcon->IASetInputLayout(inputLayout.Get());

	D3D11_BUFFER_DESC vertexBufferDesc = {};
	const UINT BUFFER_SIZE = sizeof(VERTEX_COLORED) * 6;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = BUFFER_SIZE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = dev->CreateBuffer(&vertexBufferDesc, NULL, &vertexBuffer);
	if (!SUCCEEDED(result))
	{
		ErrorLogger::Log("Failed to create vertex buffer", result);
		TOF(result);
	}
	D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
	TOF(result = devcon->Map(vertexBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource));
	if (SUCCEEDED(result))
	{
		const UINT VERTICIES_SIZE = sizeof(vertices) ;
		//_memccpy(mappedResource.pData, vertices, 0, VERTICIES_SIZE);
		memcpy(mappedResource.pData, vertices, VERTICIES_SIZE);
		devcon->Unmap(vertexBuffer.Get(), NULL);
	}
	else
	{
		ErrorLogger::Log("Failed to map vertex buffer", result);
		TOF(result);
	}

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * 3;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = triangleList;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	TOF(dev->CreateBuffer(&bufferDesc, &InitData, &indexBuffer));	
}



void Sample3DApp::DiscardDeviceResources()
{
	inputLayout = nullptr;
	vertexBuffer = nullptr;
	backBufferTarget = nullptr;
	D3DAppWindow::DiscardDeviceResources();
}

void Sample3DApp::InitPipeline()
{
	//D3DAppWindow::InitPipeline();
	SetViewport();
	devcon->OMSetRenderTargets(1, backBufferTarget.GetAddressOf(), nullptr);
	devcon->VSSetShader(vertexShaderMap.at(L"MyVShader").Get(), 0, 0);
	devcon->PSSetShader(pixelShaderMap.at(L"MyPShader").Get(), 0, 0);
	devcon->IASetInputLayout(inputLayout.Get());
}

void Sample3DApp::Render()
{
	FLOAT backgroundColor[4] = { 0.1f, 0.1f, 0.2f, 1.0f };
	devcon->ClearRenderTargetView(backBufferTarget.Get(), backgroundColor);
	UINT stride = sizeof(VERTEX_COLORED);
	UINT offset = 0;
	InitPipeline();
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	
	devcon->Draw(6, 0);	
	swapchain->Present(0, 0);
}