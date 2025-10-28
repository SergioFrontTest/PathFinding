// Main application process based on CWinApp
#pragma once

#include <iostream>
#define _WIN32_WINNT 0x0A00  // Windows 10
#include <afxwin.h>

class UI;

class CPathFindingApp : public CWinApp
{
public:
	BOOL InitInstance() override;
	virtual int ExitInstance() override;

	UI* m_pUI = nullptr;
};
