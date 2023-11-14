#pragma once
#include "D3DAppWindow.h"

class Sample3DApp : public D3DAppWindow
{
public:
	Sample3DApp(HINSTANCE hInstance);
	void InitDeviceResources() override;
	void Render() override;

protected:
	void InitPipeline() override;
	void DiscardDeviceResources() override;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr< ID3D11InputLayout> inputLayout;
	ComPtr<ID3D11Buffer> indexBuffer;


private:
	VERTEX_COLORED vertices[6] = {
		{  0.0f,   0.9f, 0.0f, { 0.0f, 0.0f, 0.5f, 1.0f } },
		{  0.95f, -0.9f,  0.0f, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ -0.95f, -0.9f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } },

		{  0.95f,  0.9f,  0.0f, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{  0.0f,  -0.9f, 0.0f, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ -0.95f,  0.9f, 0.0f, { 0.0f, 0.0f, 1.0f, 1.0f } },	
		

	};

	UINT triangleList[3] = {0,1, 2 };

};