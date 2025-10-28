#pragma once

#include "MainFrameWnd.h"
#include "../Terrain/Terrain.h"
#define _WIN32_WINNT 0x0A00  // Windows 10
#include <afxwin.h>
#include <vector>

class CPathFindingApp;

class UI
{
public:
	bool createWindow(CPathFindingApp* pApp);

	MainFrameWnd* GetMainFrameWnd();
	Terrain m_terrain;

protected:
	bool createMenu();
	MainFrameWnd m_frameWnd;
};