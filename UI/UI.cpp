#include "UI.h"
#include "../PathFinding.h"
#include "Resources.h"

bool UI::createMenu()
{
	// Load main menu from resources
	CMenu menu;
	if (menu.LoadMenu(IDR_MAIN_MENU))
	{
		// menu.Detach() to transfers ownership of the menu handle to the window. The menu to persist after the CMenu object goes out of scope.
		if (::SetMenu(m_frameWnd.GetSafeHwnd(), menu.Detach()))
		{
			return true;
		}
		else
		{
			AfxMessageBox("Cannot assign the main menu to the main window.", MB_OK | MB_ICONERROR);
		}

		// Free memory
		menu.DestroyMenu();
	}
	else
	{
		AfxMessageBox("Cannot load the main menu resource.", MB_OK | MB_ICONERROR);
	}

	return false;
}

bool UI::createWindow(CPathFindingApp* pApp)
{
	if (!pApp)
	{
		AfxMessageBox("Cannot create main window: not a valid application pointer.", MB_OK | MB_ICONERROR);
		return false;
	}

	// Assign main UI class to the main frame window
	m_frameWnd.SetUI(this);

	// Create the mainframe window and menu
	if (m_frameWnd.Create(nullptr, "Path Finding"))
	{
		if (createMenu())
		{
			pApp->m_pMainWnd = &m_frameWnd;
			m_frameWnd.ShowWindow(SW_NORMAL);
			return true;
		}
	}

	return false;
}

MainFrameWnd* UI::GetMainFrameWnd()
{
	return &m_frameWnd;
}
