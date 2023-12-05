#pragma once
#include "D3DAppWindow.h"
#include "TextureLoader.h"

//using namespace DirectX;
//using namespace DirectX::SimpleMath;

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
	ComPtr<ID3D11RasterizerState> rsNoCull;
	void InitPipeline() override;
	virtual void InitConstantsBuffer();
	void DiscardDeviceResources() override;
	void Update(DOUBLE) override;
	void SetBlendState();


	void UpdateConstantsBuffer();

	std::shared_ptr<TextureLoader> imageLoader;

	ComPtr<ID3D11BlendState> blendState;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> planeVertexBuffer;
	ComPtr<ID3D11Buffer> texturedVertexBuffer;
	ComPtr< ID3D11InputLayout> inputLayout;
	ComPtr< ID3D11InputLayout> texturedInputLayout;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11SamplerState> samplerState;
	ComPtr<ID3D11ShaderResourceView> textureResource;
	ComPtr<ID3D11ShaderResourceView> alternateTexture;
	ComPtr<ID3D11ShaderResourceView> planeTexture;
	void RenderTexturedTriangle();
	void RenderColoredTriangle();
	void RenderPlane();

	ComPtr<ID3D11Texture2D> backgroundTexture;


	ComPtr<ID3D11Buffer> positioningConstantsBuffer;
	PositioningConstantsBuffer positioningConstants = { 0.75, 0.25, -0.25 };


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

	VERTEX_TEXTURED background_vertices[9] = {
		{ -0.95f, -0.9f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f}},
		{  0.95f, -0.9f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f}},
		{  0.95f,  0.95f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f}},

		{  0.95f,  0.95f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f}},
		{ -0.95f, -0.90f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f}},
		{ -0.95f,  0.95f, 0.0f, { 1.0f, 0.0f, 1.0f, 1.0f }, {0.0f, 1.0f}},

		{  0.95f,  0.9f,  0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f} },
		{  0.0f,  -0.9f, 0.0f,  { 1.0f, 1.0f, 1.0f, 1.0f }, {0.5f, 0.0f} },
		{ -0.95f,  0.9f, 0.0f,  { 1.0f, 0.0f, 1.0f, 1.0f }, {0.0f, 1.0f} },
	};

	VERTEX_TEXTURED plane_vertices[6] =
	{
		{-0.18f, -0.4f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f}},
		{ 0.18f, -0.4f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0f, 0.0f}},
		{ 0.18f,  0.4f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f}},

		{ 0.18f, 0.4f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {1.0f, 1.0f}},
		{-0.18f,  0.4f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 1.0f}},
		{-0.18f,  -0.4f, 0.0f, { 1.0f, 1.0f, 1.0f, 1.0f }, {0.0f, 0.0f}},


	};

	UINT triangleList[3] = {0,1, 2 };

};