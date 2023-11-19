#include "TextureLoader.h"
#include  "../common/Exception.h"
#include <map>
#include <memory>
#include <vector>



 ComPtr<IWICImagingFactory> TextureLoader::_imagingFactory;

DXGI_FORMAT FormatMapping(GUID pixelFormat)
{
	if (pixelFormat == GUID_WICPixelFormat128bppRGBAFloat) return DXGI_FORMAT_R32G32B32A32_FLOAT;
	else if (pixelFormat == GUID_WICPixelFormat128bppRGBAFloat) return DXGI_FORMAT_R32G32B32A32_FLOAT;
	else if (pixelFormat == GUID_WICPixelFormat64bppRGBAHalf) return DXGI_FORMAT_R16G16B16A16_FLOAT;
	else if (pixelFormat == GUID_WICPixelFormat64bppRGBA) return DXGI_FORMAT_R16G16B16A16_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat32bppRGBA) return DXGI_FORMAT_R8G8B8A8_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat32bppBGRA) return DXGI_FORMAT_B8G8R8A8_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat32bppBGR) return DXGI_FORMAT_B8G8R8X8_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat32bppRGBA1010102XR) return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat32bppRGBA1010102) return DXGI_FORMAT_R10G10B10A2_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat32bppRGBE) return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
	else if (pixelFormat == GUID_WICPixelFormat24bppBGR) return DXGI_FORMAT_R8G8B8A8_UINT;
	else if (pixelFormat == GUID_WICPixelFormat16bppBGRA5551) return DXGI_FORMAT_B5G5R5A1_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat16bppBGR565) return DXGI_FORMAT_B5G6R5_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat32bppGrayFloat) return DXGI_FORMAT_R32_FLOAT;
	else if (pixelFormat == GUID_WICPixelFormat16bppGrayHalf) return DXGI_FORMAT_R16_FLOAT;
	else if (pixelFormat == GUID_WICPixelFormat16bppGray) return DXGI_FORMAT_R16_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat8bppGray) return DXGI_FORMAT_R8_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat8bppAlpha) return DXGI_FORMAT_A8_UNORM;
	else if (pixelFormat == GUID_WICPixelFormat96bppRGBFloat) return DXGI_FORMAT_R32G32B32_FLOAT;
	return DXGI_FORMAT_UNKNOWN;
}

std::vector<std::tuple<GUID, GUID>> WICConversion = {
	{ GUID_WICPixelFormatBlackWhite,            GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM

	{ GUID_WICPixelFormat1bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
	{ GUID_WICPixelFormat2bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
	{ GUID_WICPixelFormat4bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
	{ GUID_WICPixelFormat8bppIndexed,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 

	{ GUID_WICPixelFormat2bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM 
	{ GUID_WICPixelFormat4bppGray,              GUID_WICPixelFormat8bppGray }, // DXGI_FORMAT_R8_UNORM 

	{ GUID_WICPixelFormat16bppGrayFixedPoint,   GUID_WICPixelFormat16bppGrayHalf }, // DXGI_FORMAT_R16_FLOAT 
	{ GUID_WICPixelFormat32bppGrayFixedPoint,   GUID_WICPixelFormat32bppGrayFloat }, // DXGI_FORMAT_R32_FLOAT 

#ifdef DXGI_1_2_FORMATS
	{ GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat16bppBGRA5551 }, // DXGI_FORMAT_B5G5R5A1_UNORM
#else

	{ GUID_WICPixelFormat16bppBGR555,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat16bppBGRA5551,         GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat16bppBGR565,           GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM

#endif // DXGI_1_2_FORMATS

 { GUID_WICPixelFormat32bppBGR101010,        GUID_WICPixelFormat32bppRGBA1010102 }, // DXGI_FORMAT_R10G10B10A2_UNORM

	{ GUID_WICPixelFormat24bppBGR,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
	{ GUID_WICPixelFormat24bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
	{ GUID_WICPixelFormat32bppPBGRA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
	{ GUID_WICPixelFormat32bppPRGBA,            GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 

	{ GUID_WICPixelFormat48bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat48bppBGR,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppBGRA,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppPRGBA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppPBGRA,            GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

	{ GUID_WICPixelFormat48bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
	{ GUID_WICPixelFormat48bppBGRFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
	{ GUID_WICPixelFormat64bppRGBAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
	{ GUID_WICPixelFormat64bppBGRAFixedPoint,   GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
	{ GUID_WICPixelFormat64bppRGBFixedPoint,    GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
	{ GUID_WICPixelFormat64bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
	{ GUID_WICPixelFormat48bppRGBHalf,          GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 

	{ GUID_WICPixelFormat96bppRGBFixedPoint,    GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
	{ GUID_WICPixelFormat128bppPRGBAFloat,      GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
	{ GUID_WICPixelFormat128bppRGBFloat,        GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
	{ GUID_WICPixelFormat128bppRGBAFixedPoint,  GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
	{ GUID_WICPixelFormat128bppRGBFixedPoint,   GUID_WICPixelFormat128bppRGBAFloat }, // DXGI_FORMAT_R32G32B32A32_FLOAT 
	{ GUID_WICPixelFormat32bppCMYK,             GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM 
	{ GUID_WICPixelFormat64bppCMYK,             GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat40bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat80bppCMYKAlpha,        GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM

#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
	{ GUID_WICPixelFormat32bppRGB,              GUID_WICPixelFormat32bppRGBA }, // DXGI_FORMAT_R8G8B8A8_UNORM
	{ GUID_WICPixelFormat64bppRGB,              GUID_WICPixelFormat64bppRGBA }, // DXGI_FORMAT_R16G16B16A16_UNORM
	{ GUID_WICPixelFormat64bppPRGBAHalf,        GUID_WICPixelFormat64bppRGBAHalf }, // DXGI_FORMAT_R16G16B16A16_FLOAT 
#endif


};


GUID FindWICConversion(GUID typeToConvert)
{
	for (auto const& [sourceType, conversionType] : WICConversion)
	{
		if (typeToConvert == sourceType)
		{
			return conversionType;
		}
	}
}

size_t TextureLoader::WICBitsPerPixel(REFGUID targetGuid)
{
	ComPtr< IWICComponentInfo> cinfo;
	if(FAILED(TextureLoader::_imagingFactory->CreateComponentInfo(targetGuid, &cinfo)))
		return 0;
	WICComponentType type;
	if (FAILED(cinfo->GetComponentType(&type)))
		return 0;
	if (type != WICPixelFormat)
		return 0;
	ComPtr<IWICPixelFormatInfo> pinfo;

	if (FAILED(cinfo.As(&pinfo)))
		return 0;

	UINT bpp = 0;
	if (FAILED(pinfo->GetBitsPerPixel(&bpp)))
		return 0;
	return bpp;
}
TextureLoader::TextureLoader(ComPtr<ID3D11Device> device)
{
	this->_device = device;
	HRESULT result;
	result = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), &_imagingFactory);
	TOF(result);
}

size_t TextureLoader::GetSupportedMaxSize(size_t requestedMaxSize)
{
	auto feature_level = _device->GetFeatureLevel();
	size_t maxsize;
	switch (feature_level)
	{
	case D3D_FEATURE_LEVEL_9_1:
	case D3D_FEATURE_LEVEL_9_2:
		maxsize = 2048 /*D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
		break;

	case D3D_FEATURE_LEVEL_9_3:
		maxsize = 4096 /*D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
		; break;

	case D3D_FEATURE_LEVEL_10_0:
	case D3D_FEATURE_LEVEL_10_1:
		maxsize = 8192 /*D3D10_REQ_TEXTURE2D_U_OR_V_DIMENSION*/;
		break;

	default:
		maxsize = D3D11_REQ_TEXTURE2D_U_OR_V_DIMENSION;
		break;
	}
	if (requestedMaxSize == 0)
		return maxsize;
	return min(maxsize, requestedMaxSize);
}
void TextureLoader::LoadBitmap(std::wstring fileNameSource, size_t maxSize)
{
	ComPtr<IWICBitmapDecoder> bitmapDecoder;
	ComPtr<IWICBitmapFrameDecode> frameDecoder;
	WICPixelFormatGUID pixelFormat;
	WICPixelFormatGUID convertToFormat;
	double resolutionX, resolutionY;
	UINT sizex, sizey;
	_imagingFactory->CreateDecoderFromFilename(fileNameSource.c_str(), 0, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &bitmapDecoder);
	bitmapDecoder->GetFrame(0, &frameDecoder);
	frameDecoder->GetPixelFormat(&pixelFormat);
	convertToFormat = pixelFormat;
	frameDecoder->GetResolution(&resolutionX, &resolutionY);
	frameDecoder->GetSize(&sizex, &sizey);
	DXGI_FORMAT imageFormat = FormatMapping(pixelFormat);
	size_t bpp = 0;
	if ((imageFormat == DXGI_FORMAT_UNKNOWN))
	{
		auto altFormat = FindWICConversion(pixelFormat);
		if (altFormat != GUID_NULL)
		{
			imageFormat = FormatMapping(altFormat);
			bpp = WICBitsPerPixel(altFormat);
		}
		else
		{
			return; //HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );
		}
	}
	else // (imageFormat != DXGI_FORMAT_UNKNOWN)
	{
		bpp = WICBitsPerPixel(pixelFormat);
	}
	if (!bpp)
	{
		return; //E_FAIL;
	}
	UINT support = 0;
	HRESULT result = _device->CheckFormatSupport(imageFormat, &support);
	if (FAILED(result) || !(support & D3D11_FORMAT_SUPPORT_TEXTURE2D))
	{
		imageFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		bpp = 32;
	}
	size_t texturesize = GetSupportedMaxSize(maxSize);
	UINT twidth = sizex, theight = sizey;
	if (twidth > texturesize || theight > texturesize)
	{
		float aspectRatio = static_cast<float>(theight) / static_cast<float>(twidth);
		if (twidth > theight)
		{
			twidth = static_cast<UINT>(texturesize);
			theight = static_cast<UINT>(static_cast<float>(texturesize) * aspectRatio);
		}
		else
		{
			twidth = static_cast<UINT>(static_cast<float>(texturesize) * aspectRatio);
			theight = static_cast<UINT>(texturesize);
		}
	}
	size_t rowPitch = (twidth * bpp + 7) / 8;
	size_t imageSize = rowPitch * theight;
	std::unique_ptr<uint8_t> temp_image(new uint8_t[imageSize]);


	if (convertToFormat == pixelFormat && twidth==sizex && theight == sizey)
	{
		//no conversion or resize needed. Copy the data
		result = frameDecoder->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp_image.get());
		if (FAILED(result))
		{
			return;  //result
		}
	}
	else if (twidth!=sizex || theight !=sizey)
	{
		ComPtr<IWICBitmapScaler> scaler;
		TOF(TextureLoader::_imagingFactory->CreateBitmapScaler(&scaler));
		TOF(scaler->Initialize(frameDecoder.Get(), twidth, theight, WICBitmapInterpolationModeFant));
		WICPixelFormatGUID pfScaler;
		TOF(scaler->GetPixelFormat(&pfScaler));
		if (convertToFormat == pfScaler)
		{
			//no conversion needed. Just resizing. 
			TOF(scaler->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp_image.get()));
		}
		else
		{
			ComPtr<IWICFormatConverter> FC;
			TOF(TextureLoader::_imagingFactory->CreateFormatConverter(&FC));
			FC->Initialize(scaler.Get(), convertToFormat, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
			FC->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp_image.get());

		}

	}
	else
	{
		//Format conversion, but no resize
		ComPtr<IWICFormatConverter> FC;
		TextureLoader::_imagingFactory->CreateFormatConverter(&FC);
		FC->Initialize(frameDecoder.Get(), convertToFormat, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
		FC->CopyPixels(0, static_cast<UINT>(rowPitch), static_cast<UINT>(imageSize), temp_image.get());
	}

	D3D11_TEXTURE2D_DESC texture_description = {};
	texture_description.Width = static_cast<int>(resolutionX);
	texture_description.Height = static_cast<int>(resolutionY);
	texture_description.MipLevels = 1;
	texture_description.ArraySize = 1;
	texture_description.Format = imageFormat;
	texture_description.SampleDesc.Count = 1;
	texture_description.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = temp_image.get();
	initData.SysMemPitch = static_cast<UINT>(rowPitch);
	initData.SysMemSlicePitch = static_cast<UINT>(imageSize);


	ComPtr<ID3D11Texture2D> texture;
	result = this->_device->CreateTexture2D(&texture_description, &initData, &texture);





}