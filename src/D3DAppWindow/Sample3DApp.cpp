#include "framework.h"
#include "Sample3DApp.h"
#include "ErrorLogger.h"
#include "StringConverter.h"
#include "layout/VertexTypes.h"
#include "Exception.h"


Sample3DApp::Sample3DApp(HINSTANCE hInstance)
	: D3DAppWindow(hInstance)
{
	SetShaderSource(L"shaderList.txt");
}



void Sample3DApp::InitDeviceResources()
{
	D3DAppWindow::InitDeviceResources();

	CreateShaderResources();



	D3D11_BUFFER_DESC vertexBufferDesc = {};
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_COLORED) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	HRESULT result = dev->CreateBuffer(&vertexBufferDesc, NULL, &vertexBuffer);
	if (!SUCCEEDED(result))
	{
		Logger::Log("Failed to create vertex buffer", result);
		return;
	}

	ComPtr<ID3DBlob> vertexShaderBlob = vertexShaderBlobMap.at(L"MyVShader");
	result = dev->CreateInputLayout(
		VERTEX_COLORED_DESCRIPTION,
		2,
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		&inputLayout
	);

	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	result = devcon->Map(vertexBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	if (SUCCEEDED(result))
	{
		_memccpy(mappedResource.pData, vertices, 0, sizeof(vertices));
		devcon->Unmap(vertexBuffer.Get(), NULL);
	}
	else
	{
		Logger::Log("Failed to map vertex buffer", result);
		return;
	}

	// Fill in a buffer description.
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(unsigned int) * 3;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = triangleList;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	TOF(dev->CreateBuffer(&bufferDesc, &InitData, &indexBuffer));
	devcon->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Sample3DApp::InitPipeline()
{
	//D3DAppWindow::InitPipeline();
	SetViewport();
	devcon->VSSetShader(vertexShaderMap.at(L"MyVShader").Get(), 0, 0);
	devcon->PSSetShader(pixelShaderMap.at(L"MyPShader").Get(), 0, 0);
	devcon->IASetInputLayout(inputLayout.Get());
}
void Sample3DApp::Render()
{
	D3DAppWindow::Render();
	UINT stride = sizeof(VERTEX_COLORED);
	UINT offset = 0;
	HRESULT result;
	//devcon->OMSetRenderTargets(1, backBufferTarget.GetAddressOf(), nullptr);
	InitPipeline();
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(3, 0);
}