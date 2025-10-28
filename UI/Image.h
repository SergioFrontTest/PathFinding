#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <string>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "windowscodecs")

class Image
{
public:
	Image();
	~Image();
	void freeImage();

	void setRenderTarget(ID2D1RenderTarget* renderTarget);

	bool loadFromFile(const std::wstring& filename);
	bool loadFromResource(HINSTANCE hInstance, int resourceID);

	// Needed to reload the Direct 2D image on display device change
	bool wasLoaded();
	bool reload(ID2D1RenderTarget* renderTarget);

	void drawMapImage(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
		float destinationX, float destinationY,
		float destinationWidth, float destinationHeight);

	UINT getImageWidth() const;
	UINT getImageHeight() const;

protected:
	ID2D1RenderTarget* m_renderTarget = nullptr;
	ID2D1Bitmap* m_bitmap = nullptr;

	IWICImagingFactory* m_wicFactory = nullptr;

	bool loadBitmapFromWIC(IWICBitmapSource* wicSource);

	bool m_wasLoaded = false;
	int m_loadedResourceID = 0;
	std::wstring m_loadedFilename;
};
