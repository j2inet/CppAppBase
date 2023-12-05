#include "framework.h"
#include "Sample3DApp.h"
#include "../common/Exception.h"
#include "../common/StringConverter.h"
#include "layout/VertexTypes.h"
#include "../common/ErrorLogger.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"


using namespace DirectX;

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
	result = dev->CreateInputLayout(
		VERTEX_COLORED_DESCRIPTION,
		2,
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),
		&inputLayout
	);
	devcon->IASetInputLayout(inputLayout.Get());

	auto texturedVertexShader = vertexShaderBlobMap.at(L"TexturedVShader");
	result = dev->CreateInputLayout(
		VERTEX_TEXTURED_DESCRIPTION,
		3,
		texturedVertexShader->GetBufferPointer(),
		texturedVertexShader->GetBufferSize(),
		&texturedInputLayout
	);
	TOF(result);

#pragma region Plane Vertex
	{
		const int PLANE_TEXTURE_SIZE = sizeof(VERTEX_TEXTURED) * 6;
		D3D11_BUFFER_DESC planeVertexBufferDesc{};
		planeVertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		planeVertexBufferDesc.ByteWidth = PLANE_TEXTURE_SIZE;
		planeVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		planeVertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		result = dev->CreateBuffer(&planeVertexBufferDesc, NULL, &planeVertexBuffer);
		if (!SUCCEEDED(result))
		{
			ErrorLogger::Log("Failed to create textured vertex buffer", result);
			TOF(result);
		}
		D3D11_MAPPED_SUBRESOURCE texturedResource = { };
		TOF(result = devcon->Map(planeVertexBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &texturedResource));
		if (SUCCEEDED(result))
		{
			const UINT VERTICES_SIZE = sizeof(plane_vertices);
			memcpy(texturedResource.pData, plane_vertices, VERTICES_SIZE);
			devcon->Unmap(planeVertexBuffer.Get(), NULL);
		}
		else
		{
			ErrorLogger::Log("Failed to map vertext buffer ", result);
			TOF(result);
		}
	}
#pragma end region
#pragma region Textured Vertex
	D3D11_BUFFER_DESC texturedVertextBufferDesc = {};
	const UINT TEXTURED_BUFFER_SIZE = sizeof(VERTEX_TEXTURED) * 9;
	texturedVertextBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	texturedVertextBufferDesc.ByteWidth = TEXTURED_BUFFER_SIZE;
	texturedVertextBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	texturedVertextBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	result = dev->CreateBuffer(&texturedVertextBufferDesc, NULL, &texturedVertexBuffer);
	if (!SUCCEEDED(result))
	{
		
		ErrorLogger::Log("Failed to create textured vertex buffer", result);
		TOF(result);
	}
	D3D11_MAPPED_SUBRESOURCE texturedResource = { };
	TOF(result = devcon->Map(texturedVertexBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &texturedResource));
	if (SUCCEEDED(result))
	{
		const UINT VERTICES_SIZE = sizeof(background_vertices);
		memcpy(texturedResource.pData, background_vertices, VERTICES_SIZE);
		devcon->Unmap(texturedVertexBuffer.Get(), NULL);
	}
	else
	{
		ErrorLogger::Log("Failed to map vertext buffer ", result);
		TOF(result);
	}
#pragma endregion

#pragma region Color Vertex
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
#pragma endregion
	CONST UINT width = 320;
	CONST UINT height = 200;
	CONST UINT bpp = 3;
	std::unique_ptr<uint8_t[]> initialImage(new uint8_t[bpp * width * height]);
	for (auto i = 0; i < 320 * 200 * 3; ++i)
	{
		initialImage[i] = 0xFF;
	}
	D3D11_TEXTURE2D_DESC td = { 0 };
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	size_t rowPitch = (width * bpp + 7) / 8;
	size_t imageSize = rowPitch * height;
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = initialImage.get();
	initData.SysMemPitch = rowPitch;
	initData.SysMemSlicePitch = static_cast<UINT>(imageSize);

	result = dev->CreateTexture2D(&td, &initData, &backgroundTexture);

#pragma region create sampler
	D3D11_SAMPLER_DESC sd{};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxAnisotropy = 16;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.BorderColor[0] = 0.0f;
	sd.BorderColor[1] = 0.0f;
	sd.BorderColor[2] = 0.0f;
	sd.BorderColor[3] = 0.0f;
	sd.MinLOD = 0;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 0.0f;
	dev->CreateSamplerState(&sd, &samplerState);
#pragma endregion

	InitConstantsBuffer();
#pragma region sample texture
	TOF(DirectX::CreateWICTextureFromFile(dev.Get(), devcon.Get(), L".\\assets\\images\\coloring.png", nullptr, &textureResource));
	TOF(DirectX::CreateWICTextureFromFile(dev.Get(), devcon.Get(), L".\\assets\\images\\shapes.png", nullptr, &alternateTexture), L"Can't load shapes");
	TOF(DirectX::CreateWICTextureFromFile(dev.Get(), devcon.Get(), L".\\assets\\images\\plane_00.png", nullptr, &planeTexture), L"Can't load plane");
#pragma endregion

#pragma region render state
	D3D11_RASTERIZER_DESC rsDesc{};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.FrontCounterClockwise = false;
	rsDesc.DepthClipEnable = false;
	TOF(dev->CreateRasterizerState(&rsDesc, &rsNoCull));

	devcon->RSSetState(rsNoCull.Get());
#pragma end region
}



void Sample3DApp::DiscardDeviceResources()
{
	inputLayout = nullptr;
	vertexBuffer = nullptr;
	backBufferTarget = nullptr;
	D3DAppWindow::DiscardDeviceResources();
}


void Sample3DApp::Update(DOUBLE time)
{
	D3DAppWindow::Update(time);
	totalTime += time;
	positioningConstants.offsetX = (float)sin(totalTime);
	positioningConstants.offsetY = (float)cos(totalTime*0.3);
}


void Sample3DApp::InitPipeline()
{
	//D3DAppWindow::InitPipeline();
	SetViewport();
	devcon->OMSetRenderTargets(1, backBufferTarget.GetAddressOf(), nullptr);
	SetBlendState();
}


void Sample3DApp::InitConstantsBuffer()
{
	D3D11_BUFFER_DESC bufferDesc = {  };
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(PositioningConstantsBuffer);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	TOF(dev->CreateBuffer(&bufferDesc, NULL, &positioningConstantsBuffer));
	UpdateConstantsBuffer();
}

void Sample3DApp::UpdateConstantsBuffer()
{
	devcon->UpdateSubresource(positioningConstantsBuffer.Get(), 0, 0, &positioningConstants, 16, 0);
}

void Sample3DApp::RenderTexturedTriangle()
{
	UINT stride = sizeof(VERTEX_TEXTURED);
	UINT offset = 0;

	devcon->VSSetShader(vertexShaderMap.at(L"TexturedVShader").Get(), 0, 0);
	devcon->PSSetShader( pixelShaderMap.at(L"TexturedPShader").Get(), 0, 0);
	devcon->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	devcon->IASetInputLayout(texturedInputLayout.Get());
	devcon->IASetVertexBuffers(0, 1, texturedVertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->PSSetShaderResources(0, 1, alternateTexture.GetAddressOf());
	devcon->Draw(3, 6);


	devcon->PSSetShaderResources(0, 1, textureResource.GetAddressOf());
	devcon->Draw(6, 0);
}

void Sample3DApp::SetBlendState()
{
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	TOF(dev->CreateBlendState(&blendDesc, &blendState));

	devcon->OMSetBlendState(blendState.Get(), 0, 0xffffffff);

}
void Sample3DApp::RenderPlane()
{
	UINT stride = sizeof(VERTEX_TEXTURED);
	UINT offset = 0;

	devcon->PSSetShaderResources(0, 1, planeTexture.GetAddressOf());
	devcon->VSSetShader(vertexShaderMap.at(L"TexturedVShader").Get(), 0, 0);
	devcon->PSSetShader(pixelShaderMap.at(L"TexturedPShader").Get(), 0, 0);
	devcon->PSSetSamplers(0, 1, samplerState.GetAddressOf());
	devcon->IASetInputLayout(texturedInputLayout.Get());
	devcon->IASetVertexBuffers(0, 1, planeVertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(6, 0);
}
void Sample3DApp::RenderColoredTriangle()
{
	UINT stride = sizeof(VERTEX_COLORED);
	UINT offset = 0;
	UpdateConstantsBuffer();
	devcon->VSSetConstantBuffers(0, 1, positioningConstantsBuffer.GetAddressOf());
	devcon->VSSetShader(vertexShaderMap.at(L"MyVShader").Get(), 0, 0);
	devcon->PSSetShader( pixelShaderMap.at(L"MyPShader").Get(), 0, 0);
	devcon->IASetInputLayout(inputLayout.Get());
	devcon->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(6, 0);
}

void Sample3DApp::Render()
{
	FLOAT backgroundColor[4] = { 0.1f, 0.1f, 0.2f, 1.0f };
	devcon->ClearRenderTargetView(backBufferTarget.Get(), backgroundColor);

	InitPipeline();
	RenderTexturedTriangle();
	RenderColoredTriangle();	
	RenderPlane();
	swapchain->Present(0, 0);
}