#pragma once

#include "IUIDraw.h"
#define _WIN32_WINNT 0x0A00  // Windows 10
#include <afxwin.h>
#include <memory>

class MainFrameWnd : public CFrameWnd
{
public:
	MainFrameWnd();
	void SetUI(UI* pUI);
	virtual ~MainFrameWnd();

	//inline IUIDraw* GetUIDraw()
	//{
	//	return m_pUIDraw;
	//}

protected:
	DECLARE_MESSAGE_MAP()

	// Command handlers
	afx_msg void OnFileOpenMap();
	//afx_msg void OnFileSaveAsMap();
	afx_msg void OnFileOpenMapPNG();
	afx_msg void OnFileSaveAsPositions();
	afx_msg void OnFileExit();
	afx_msg void OnClose();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI); // Window min and max size limitation

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	IUIDraw* m_pUIDraw = nullptr;

	UI* m_pUI = nullptr;

	UINT_PTR m_timerIDFrames = 1;
};