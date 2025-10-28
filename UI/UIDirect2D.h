#pragma once

#include "IUIDraw.h"

#include <memory>
#include <d2d1.h> // Microsoft Direct 2D

class Image;

class UIDirect2D : public IUIDraw
{
public:
	UIDirect2D();
	virtual ~UIDirect2D(); // Ensures proper cleanup when deleting derived objects via a base pointer

	virtual bool initialize(UI* pUI, int width, int height);
	virtual void free();

	virtual void drawScreen();
	virtual void drawTerrain(float displayWidth, float displayHeight);
	void drawBattleUnits(float displayWidth, float displayHeight);

	//virtual bool loadTerrainImageFromPNGResources(UINT resourceIDB);
	//virtual bool loadTerrainImageFromFile(CString filename);
	virtual bool loadTerrainImageFromFileDialogBox();

	//virtual void drawTerrainImage(TCHAR character, int positionXInMapCoordinates, int positionYInMapCoordinates);
	//virtual void drawFocusCaret(int positionXInMapCoordinates, int positionYInMapCoordinates);


protected:
	UI* m_pUI = nullptr;
	int m_width = 1024;
	int m_height = 1024;

	Image* m_pImage = nullptr;

	// Direct2D
	ID2D1Factory* m_pD2DFactory = nullptr;
	ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
};
