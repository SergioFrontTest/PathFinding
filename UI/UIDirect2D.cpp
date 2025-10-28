#include "UIDirect2D.h"
#include "UI.h"
#include "Image.h"
#include "Resources.h"
#include <afxdlgs.h>  // For CFileDialog

#include <cmath>

#pragma comment(lib, "d2d1")

UIDirect2D::UIDirect2D()
{
	CoInitialize(nullptr);

	m_pImage = new Image();
	ASSERT(m_pImage);
}

UIDirect2D::~UIDirect2D()
{
	free();

	CoUninitialize();
}

bool UIDirect2D::initialize(UI* pUI, int width, int height)
{
	m_pUI = pUI;

	MainFrameWnd* pWnd = m_pUI->GetMainFrameWnd();
	if (!pWnd)
	{
		ASSERT(FALSE);
		return false;
	}
	HWND hWnd = pWnd->GetSafeHwnd();
	if (!hWnd)
	{
		ASSERT(FALSE);
		return false;
	}

	if (!m_pD2DFactory)
	{
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	}

	if (m_pD2DFactory)
	{
		if (!m_pRenderTarget)
		{
			RECT rc;
			pWnd->GetClientRect(&rc);

			D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
			D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(rc.right, rc.bottom));

			m_pD2DFactory->CreateHwndRenderTarget(props, hwndProps, &m_pRenderTarget);

			if (!m_pRenderTarget)
			{
				return false;
			}

			if (m_pImage)
			{
				if (m_pImage->wasLoaded())
					m_pImage->reload(m_pRenderTarget);
				else
				{
					m_pImage->setRenderTarget(m_pRenderTarget);
					VERIFY(m_pImage->loadFromResource(AfxGetInstanceHandle(), IDB_PNG_TERRAIN));
				}
			}
		}

		return true;
	}

	return false;
}

void UIDirect2D::free()
{
	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = nullptr;
	}
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Release();
		m_pRenderTarget = nullptr;
	}
	if (m_pD2DFactory)
	{
		m_pD2DFactory->Release();
		m_pD2DFactory = nullptr;
	}
}

void UIDirect2D::drawScreen()
{
	if (!m_pRenderTarget)
	{
		return;
	}

	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));

	auto renderTargetSize = m_pRenderTarget->GetSize();

	float displayWidth = renderTargetSize.width;
	float displayHeight = renderTargetSize.height;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Scene begin
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	drawTerrain(displayWidth, displayHeight);

	//// Example: draw a simple circle
	//D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(400, 300), 100, 100);
	//ID2D1SolidColorBrush* brush = nullptr;
	//m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &brush);
	//m_pRenderTarget->FillEllipse(ellipse, brush);
	//brush->Release();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Scene end
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HRESULT hr = m_pRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)
	{
		m_pRenderTarget->Release();
		m_pRenderTarget = nullptr;
		// Recreate if lost
		VERIFY(initialize(m_pUI, m_width, m_height));
	}
}

void UIDirect2D::drawBattleUnits(float displayWidth, float displayHeight)
{
	Terrain* pTerrain = &m_pUI->m_terrain;
	std::vector<TerrainWalk>* pWalks = pTerrain->getTerrainWalks();

	if (pWalks->size() < 0)
		return; // No battle units to process

	// Source cell size in the image map
	int cellSourceWidth = m_pImage->getImageWidth() / TerrainSubImagesCountX;
	int cellSourceHeight = m_pImage->getImageHeight() / TerrainSubImagesCountY;

	// Destination cell size on the display
	float cellDestinationWidth = displayWidth / pTerrain->getWidth();
	float cellDestinationHeight = displayHeight / pTerrain->getHeight();

	// Get corresponding image column and row in the image map
	int imageColumn = 0;
	int imageRow = 0;
	pTerrain->getCellToTerrainImageColumnAndRowBattleUnit(imageColumn, imageRow);

	// Get corresponding image position in the image map
	int sourceX = imageColumn * cellSourceWidth;
	int sourceY = imageRow * cellSourceHeight;

	for (TerrainWalk& walk : *pWalks)
	{
		WalkPosition position = walk.getDisplayWalkStep();

		// Ensure that the scaled images are drawn seamlessly in a continuous way
		float drawX = std::floor((float)position.x * cellDestinationWidth);
		float drawY = std::floor((float)position.y * cellDestinationHeight);
		float drawSizeX = std::floor(cellDestinationWidth + 2);
		float drawSizeY = std::floor(cellDestinationHeight + 2);

		m_pImage->drawMapImage(sourceX, sourceY, cellSourceWidth, cellSourceHeight, drawX, drawY, drawSizeX, drawSizeY);
	}

	for (TerrainWalk& walk : *pWalks)
	{
		walk.doDisplayWalkStep();
	}
}

void UIDirect2D::drawTerrain(float displayWidth, float displayHeight)
{
	// Draw terrain
	Terrain* pTerrain = &m_pUI->m_terrain;
	auto pMap = pTerrain->getMap();
	if (m_pImage && pMap)
	{
		// Source cell size in the image map
		int cellSourceWidth = m_pImage->getImageWidth() / TerrainSubImagesCountX;
		int cellSourceHeight = m_pImage->getImageHeight() / TerrainSubImagesCountY;

		// Destination cell size on the display
		float cellDestinationWidth = displayWidth / pTerrain->getWidth();
		float cellDestinationHeight = displayHeight / pTerrain->getHeight();

		float destinationX = 0;
		for (auto& column : *pMap)
		{
			float destinationY = 0;
			for (auto& cell : column)
			{
				// Get corresponding image column and row in the image map
				int imageColumn = 0;
				int imageRow = 0;
				pTerrain->getCellToTerrainImageColumnAndRow(cell, imageColumn, imageRow);

				// Get corresponding image position in the image map
				int sourceX = imageColumn * cellSourceWidth;
				int sourceY = imageRow * cellSourceHeight;

				// Ensure that the scaled images are drawn seamlessly in a continuous way
				float drawX = std::floor(destinationX);
				float drawY = std::floor(destinationY);
				float drawSizeX = std::floor(cellDestinationWidth + 2);
				float drawSizeY = std::floor(cellDestinationHeight + 2);

				m_pImage->drawMapImage(sourceX, sourceY, cellSourceWidth, cellSourceHeight, drawX, drawY, drawSizeX, drawSizeY);

				destinationY += cellDestinationHeight;
			}

			destinationX += cellDestinationWidth;
		}
	}

	drawBattleUnits(displayWidth, displayHeight);
}

bool UIDirect2D::loadTerrainImageFromFileDialogBox()
{
	// Create an open file dialog for JSON files
	CFileDialog dlg(TRUE, "png", nullptr,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		"PNG Image Files (*.png)|*.png||");

	if (dlg.DoModal() == IDOK)
	{
		// Full path to selected file
		CString path = dlg.GetPathName();

		// Convert to std::wstring
		std::wstring wpath = CT2W(path);
		return m_pImage->loadFromFile(wpath);
	}

	return false;
}
