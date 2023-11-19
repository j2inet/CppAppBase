#pragma once
#include <Windows.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <string>
#include <Windows.h>
#include <d3d11.h>
using namespace Microsoft::WRL;


//This class is derived from a Microsoft examples that can be found here:
//https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-how-to
class TextureLoader
{
public:
	TextureLoader(ComPtr< ID3D11Device>);
	~TextureLoader() = default;

	void LoadBitmap(std::wstring fileNameSource, size_t maxSize = 0);

protected:

private:
	size_t GetSupportedMaxSize(size_t requestedMaxSize = 0);
	static size_t WICBitsPerPixel(REFGUID guid);
	static ComPtr<IWICImagingFactory> _imagingFactory;
	ComPtr< ID3D11Device> _device;
};