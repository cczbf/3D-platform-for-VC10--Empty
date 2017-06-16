
// 3dviewerView.h : CMy3dViewerView ��Ľӿ�
//

#pragma once

#include "CBmpDblBuffering.h"

class CMy3dViewerView : public CView
{
public:
	void Render();
	void DrawStastic();

private:
    CBmpViewerData *GetBmpViewerData();

protected: // �������л�����
	CMy3dViewerView();
	DECLARE_DYNCREATE(CMy3dViewerView)

// ����
public:
	CBmpDblBuffering* m_pCBmpDblBuf; 

// ����
public:
	CMy3dViewerDoc* GetDocument() const;

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMy3dViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//afx_msg void OnMenuitem32792();
	//afx_msg void OnUpdateMenuitem32792(CCmdUI* pCmdUI);
	//afx_msg void OnMenuitem32791();
	//afx_msg void OnUpdateMenuitem32791(CCmdUI* pCmdUI);



	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuiCaptureImage();
};

#ifndef _DEBUG  // 3dviewerView.cpp �еĵ��԰汾
inline CMy3dViewerDoc* CMy3dViewerView::GetDocument() const
   { return reinterpret_cast<CMy3dViewerDoc*>(m_pDocument); }
#endif

