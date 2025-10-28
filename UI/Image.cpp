#include "Image.h"

Image::Image()
{
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_wicFactory));
}

Image::~Image()
{
	freeImage();

	if (m_wicFactory)
	{
		m_wicFactory->Release();
		m_wicFactory = nullptr;
	}
	
	m_renderTarget = nullptr;
}

void Image::freeImage()
{
	if (m_bitmap)
	{
		m_bitmap->Release();
		m_bitmap = nullptr;
	}
}

void Image::setRenderTarget(ID2D1RenderTarget* renderTarget)
{
	m_renderTarget = renderTarget;
}

bool Image::loadFromFile(const std::wstring& filename)
{
	freeImage();

	// Needed to reload the Direct 2D image on display device change
	m_wasLoaded = true;
	m_loadedResourceID = 0;
	m_loadedFilename = filename;

	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frame = nullptr;

	HRESULT hr = m_wicFactory->CreateDecoderFromFilename(
		filename.c_str(), nullptr, GENERIC_READ,
		WICDecodeMetadataCacheOnLoad, &decoder);

	if (SUCCEEDED(hr))
	{
		hr = decoder->GetFrame(0, &frame);
	}

	bool result = SUCCEEDED(hr) && loadBitmapFromWIC(frame);

	if (frame)
	{
		frame->Release();
	}
	if (decoder)
	{
		decoder->Release();
	}

	return result;
}

bool Image::loadFromResource(HINSTANCE hInstance, int resourceID)
{
	freeImage();

	// Needed to reload the Direct 2D image on display device change
	m_wasLoaded = true;
	m_loadedResourceID = resourceID;
	m_loadedFilename.clear();

	HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(resourceID), "PNG");
	if (!hRes)
	{
		return false;
	}

	HGLOBAL hMem = LoadResource(hInstance, hRes);
	if (!hMem)
	{
		return false;
	}

	void* pData = LockResource(hMem);
	DWORD size = SizeofResource(hInstance, hRes);

	IWICStream* stream = nullptr;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* frame = nullptr;

	HRESULT hr = m_wicFactory->CreateStream(&stream);
	if (SUCCEEDED(hr))
	{
		hr = stream->InitializeFromMemory(reinterpret_cast<BYTE*>(pData), size);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_wicFactory->CreateDecoderFromStream(stream, nullptr,
			WICDecodeMetadataCacheOnLoad, &decoder);
	}

	if (SUCCEEDED(hr))
	{
		hr = decoder->GetFrame(0, &frame);
	}

	bool result = SUCCEEDED(hr) && loadBitmapFromWIC(frame);

	if (frame)
	{
		frame->Release();
	}

	if (decoder)
	{
		decoder->Release();
	}

	if (stream)
	{
		stream->Release();
	}

	return result;
}

bool Image::wasLoaded()
{
	return m_wasLoaded;
}

bool Image::reload(ID2D1RenderTarget* renderTarget)
{
	if (m_wasLoaded)
	{
		if (m_loadedResourceID != 0)
		{
			HINSTANCE hInstance = ::GetModuleHandle(nullptr);
			return loadFromResource(hInstance, m_loadedResourceID);
		}
		else if (!m_loadedFilename.empty())
		{
			return loadFromFile(m_loadedFilename);
		}
	}

	return false;
}

bool Image::loadBitmapFromWIC(IWICBitmapSource* wicSource)
{
	IWICFormatConverter* converter = nullptr;
	HRESULT hr = m_wicFactory->CreateFormatConverter(&converter);
	if (SUCCEEDED(hr))
	{
		hr = converter->Initialize(wicSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr, 0.0f,
			WICBitmapPaletteTypeCustom);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_renderTarget->CreateBitmapFromWicBitmap(converter, nullptr, &m_bitmap);
	}

	if (converter)
	{
		converter->Release();
	}

	return SUCCEEDED(hr);
}

UINT Image::getImageWidth() const
{
	return m_bitmap ? static_cast<UINT>(m_bitmap->GetSize().width) : 0;
}

UINT Image::getImageHeight() const
{
	return m_bitmap ? static_cast<UINT>(m_bitmap->GetSize().height) : 0;
}

void Image::drawMapImage(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
	float destinationX, float destinationY,
	float destinationWidth, float destinationHeight)
{
	if (!m_bitmap)
	{
		return;
	}

	D2D1_RECT_F srcRect = D2D1::RectF(
		static_cast<float>(sourceX),
		static_cast<float>(sourceY),
		static_cast<float>(sourceX + sourceWidth),
		static_cast<float>(sourceY + sourceHeight));

	D2D1_RECT_F destRect = D2D1::RectF(
		destinationX,
		destinationY,
		destinationX + destinationWidth,
		destinationY + destinationHeight);

	m_renderTarget->DrawBitmap(m_bitmap, destRect, 1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
}
