// PathFinding.cpp : Defines the entry point for the application.
//

#include "PathFinding.h"
#include "UI/UI.h"

using namespace std;

CPathFindingApp theApp;

BOOL CPathFindingApp::InitInstance()
{
	m_pUI = new UI();
	if (m_pUI)
	{
		if (!m_pUI->createWindow(this))
			return FALSE;
	}

	return TRUE;
}

int CPathFindingApp::ExitInstance()
{
	int returnValue = CWinApp::ExitInstance();

	if (m_pUI)
	{
		delete m_pUI;
		m_pUI = nullptr;
	}

	return returnValue;
}
