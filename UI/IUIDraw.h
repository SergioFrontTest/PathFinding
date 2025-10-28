#pragma once

#define _WIN32_WINNT 0x0A00  // Windows 10
#include <afxwin.h>

class UI;
class IUIDraw
{
public:
	virtual bool initialize(UI* pUI, int width, int height) = 0;
	virtual void free() = 0;

	virtual void drawScreen() = 0;

	//virtual bool loadTerrainImageFromPNGResources(UINT resourceIDB);
	//virtual bool loadTerrainImageFromFile(CString filename);
	virtual bool loadTerrainImageFromFileDialogBox() = 0;

	//virtual void drawTerrainImage(TCHAR character, int positionXInMapCoordinates, int positionYInMapCoordinates);
	//virtual void drawFocusCaret(int positionXInMapCoordinates, int positionYInMapCoordinates);

	virtual ~IUIDraw() = default; // Ensures proper cleanup when deleting derived objects via a base pointer

protected:
	UI* m_pUI = nullptr;
};
