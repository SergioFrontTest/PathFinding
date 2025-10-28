#include "MainFrameWnd.h"
#include "UI.h"
#include "UIDirect2D.h"
#include "Resources.h" // Menu commands definitions
#include "../Terrain/Terrain.h" // TerrainDefaultScreenWidth and TerrainDefaultScreenHeight

constexpr int FramesPerSeconds = 12; // Game FPS
constexpr int DisplayTimerDelay = 1000 / FramesPerSeconds;

BEGIN_MESSAGE_MAP(MainFrameWnd, CFrameWnd)
	ON_COMMAND(ID_FILE_OPEN, &MainFrameWnd::OnFileOpenMap)
	//ON_COMMAND(ID_FILE_SAVE_AS, &MainFrameWnd::OnFileSaveAsMap)
	ON_COMMAND(ID_FILE_OPEN_MAP_PNG, &MainFrameWnd::OnFileOpenMapPNG)
	ON_COMMAND(ID_FILE_SAVE_AS_WALK_POSITIONS, &MainFrameWnd::OnFileSaveAsPositions)
	ON_COMMAND(ID_APP_EXIT, &MainFrameWnd::OnFileExit)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

MainFrameWnd::MainFrameWnd()
{
}

void MainFrameWnd::SetUI(UI* pUI)
{
	ASSERT(pUI);
	m_pUI = pUI;
}

MainFrameWnd::~MainFrameWnd()
{
}

BOOL MainFrameWnd::OnEraseBkgnd(CDC* pDC)
{
	// Don't erase background
	return TRUE;  // Return TRUE to indicate background was erased
}

int MainFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	ASSERT(m_pUI); // Missing call to SetUI(...) before creating this window

	// Create main window
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize drawing (Direct 2D, ...)
	m_pUIDraw = new UIDirect2D();
	ASSERT(m_pUIDraw);
	if (m_pUIDraw)
		m_pUIDraw->initialize(m_pUI, TerrainDefaultScreenWidth, TerrainDefaultScreenHeight);

	// Display refresh rate timer
	m_timerIDFrames = SetTimer(m_timerIDFrames, DisplayTimerDelay, nullptr);

	return 0;
}

void MainFrameWnd::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// Set minimum window size to 256 x 256
	lpMMI->ptMinTrackSize.x = 256;
	lpMMI->ptMinTrackSize.y = 256;

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void MainFrameWnd::OnPaint()
{
	CPaintDC dc(this);
	
	if (m_pUIDraw)
		m_pUIDraw->drawScreen();
}

void MainFrameWnd::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_timerIDFrames)
	{
		// Trigger WM_PAINT
		Invalidate(FALSE);
	}
}

void MainFrameWnd::OnClose()
{
	KillTimer(m_timerIDFrames);

	// Free Direct2D, ...
	if (m_pUIDraw)
	{
		delete m_pUIDraw;
		m_pUIDraw = nullptr;
	}

	// Signal app to exit
	PostQuitMessage(0);
	//CFrameWnd::OnClose();  // Don't call base class, PostQuitMessage(...) is already handling the window destruction
}

void MainFrameWnd::OnFileOpenMap()
{
	m_pUI->m_terrain.loadFromFileUsingDialogBox();
}

//void MainFrameWnd::OnFileSaveAsMap()
//{
//	m_pUI->m_terrain.saveToFileUsingDialogBox();
//}

void MainFrameWnd::OnFileOpenMapPNG()
{
	m_pUIDraw->loadTerrainImageFromFileDialogBox();
}

void MainFrameWnd::OnFileSaveAsPositions()
{
	m_pUI->m_terrain.saveWalksToFileUsingDialogBox();
}

void MainFrameWnd::OnFileExit()
{
	PostMessage(WM_CLOSE);
}

void MainFrameWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CFrameWnd::OnLButtonDown(nFlags, point); // We can call it for this case, just in case we have something else

	int terrainWidth = m_pUI->m_terrain.getWidth();
	int terrainHeight = m_pUI->m_terrain.getHeight();

	if (m_pUI != nullptr && 
		terrainWidth > 0 &&
		terrainHeight > 0)
	{
		CRect clientRect;
		GetClientRect(&clientRect);

		// Cell size
		double sizeX = static_cast<double>(clientRect.Width()) / static_cast<double>(terrainWidth);
		double sizeY = static_cast<double>(clientRect.Height()) / static_cast<double>(terrainHeight);

		// Convert mouse click coordinates to terrain map coordinates
		int x = static_cast<int>(static_cast<double>(point.x) / static_cast<double>(sizeX));
		int y = static_cast<int>(static_cast<double>(point.y) / static_cast<double>(sizeY));

		m_pUI->m_terrain.addBattleUnit(x, y);
	}
}

void MainFrameWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// Route double-clicks to single-clicks
	OnLButtonDown(nFlags, point);
	//CFrameWnd::OnLButtonDblClk(nFlags, point);

}
