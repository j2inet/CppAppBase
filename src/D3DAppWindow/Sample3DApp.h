#pragma once
#include "D3DAppWindow.h"


struct PositioningConstantsBuffer {
	float scale;
	float offsetX;
	float offsetY;
	char padding[4];
};


class Sample3DApp : public D3DAppWindow
{
public:
	Sample3DApp(HINSTANCE hInstance);
	void InitDeviceResources() override;
	void Render() override;

protected:
	void InitPipeline() override;
	virtual void InitConstantsBuffer();
	void DiscardDeviceResources() override;
	void Update(DOUBLE) override;


	void UpdateConstantsBuffer();

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr< ID3D11InputLayout> inputLayout;
	ComPtr<ID3D11Buffer> indexBuffer;

	ComPtr<ID3D11Buffer> positioningConstantsBuffer;
	PositioningConstantsBuffer positioningConstants = { 0.25, 0.25, -0.25 };


private:
	DOUBLE totalTime = { 0 };
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