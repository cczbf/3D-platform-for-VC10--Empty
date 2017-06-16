
// 3dviewerView.cpp : CMy3dViewerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "3dviewer.h"
#endif

#include "3dviewerDoc.h"
#include "3dviewerView.h"

#include "3dViewer.h"
#include "mainfrm.h"

#include "3dViewerDoc.h"
#include "3dViewerView.h"
#include "CGlobals.h"

#define For3D

#ifdef For3D
		
#include "COpenGL3DEx.h"

#include "CMesh.h"
#include "CRelxationLabelling.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy3dViewerView

IMPLEMENT_DYNCREATE(CMy3dViewerView, CView)

BEGIN_MESSAGE_MAP(CMy3dViewerView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
//	ON_COMMAND(ID_MENUITEM32792, OnMenuitem32792)
//	ON_UPDATE_COMMAND_UI(ID_MENUITEM32792, OnUpdateMenuitem32792)
//	ON_COMMAND(ID_MENUITEM32791, OnMenuitem32791)
//	ON_UPDATE_COMMAND_UI(ID_MENUITEM32791, OnUpdateMenuitem32791)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMy3dViewerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MENUI_CAPTURE_IMAGE, &CMy3dViewerView::OnMenuiCaptureImage)
END_MESSAGE_MAP()

// CMy3dViewerView 构造/析构

CMy3dViewerView::CMy3dViewerView()
{
	// TODO: add construction code here
 m_pCBmpDblBuf	= new   CBmpDblBuffering;

}

CMy3dViewerView::~CMy3dViewerView()
{
	if (m_pCBmpDblBuf!=NULL )
		  delete   m_pCBmpDblBuf;
}

BOOL CMy3dViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy3dViewerView 绘制

void CMy3dViewerView::OnDraw(CDC*  pDC )
{
	CMy3dViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	int nToolID = 0;

	if (g_CGlobals.m_pCMainFrame->m_pCToolBase != NULL)
		nToolID = g_CGlobals.m_pCMainFrame->m_pCToolBase->get_m_nToolID();
	
	if (nToolID != 3)
	{
		//2D interactions
		if (pDoc->m_CBmpViewerData != NULL)
		{			
			HANDLE	hDib	= (pDoc->m_CBmpViewerData)->get_m_hDib();
			HDC		hDC		= pDC->m_hDC;
			
			//	CBmpDblBuffering  CBmpDblBuf;
			
			m_pCBmpDblBuf->BeginDblBuf(hDC);
			hDC		= m_pCBmpDblBuf->GetDC();
			
			int h0	=	(pDoc->m_CBmpViewerData)->get_m_h0();
			int h1	=	(pDoc->m_CBmpViewerData)->get_m_h1();
			int w0	=	(pDoc->m_CBmpViewerData)->get_m_w0();
			int w1	=	(pDoc->m_CBmpViewerData)->get_m_w1();
			
			int x0	=	(pDoc->m_CBmpViewerData)->get_m_x0();
			int x1	=	(pDoc->m_CBmpViewerData)->get_m_x1();
			int y0	=	(pDoc->m_CBmpViewerData)->get_m_y0();
			int y1	=	(pDoc->m_CBmpViewerData)->get_m_y1();
			
			((CDIB)(pDoc->m_CDIB)).StretchDibBlt (
				hDC, x0, y0, w0, h0,//Dest.
				hDib, x1, y1, w1, h1, //Source
				SRCCOPY);

			m_pCBmpDblBuf->Show();
			m_pCBmpDblBuf->EndDblBuf();
		}
	}
	else
	{	
		#ifdef For3D
		//HDC hDC = ::GetDC(m_hWnd);
		HDC hDC = ::wglGetCurrentDC();

		CMy3dViewerDoc* pDoc = GetDocument();
		HGLRC glContext = pDoc->m_pCOpenGL->GetContext();
		wglMakeCurrent(hDC, glContext);	
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
		glPushMatrix();
		
		if( pDoc->m_pCOpenGL != NULL  )
		{			
			pDoc->m_pCOpenGL->OpenGLRendering();  // for opengl rendering
						
			//display the statistics data:
			if(((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh != NULL)
			{
				switch ( ((COpenGL3DEx *)(pDoc->m_pCOpenGL))->m_pCMesh->m_nType )
				{
				case 0:				
					break;
				case 1:				
					DrawStastic();				
					break;
				case 2:
					DrawStastic();				
					break;
				case 3:
					break;
				case 4:
					break;
				}		
			}
		}
		
		glPopMatrix();
		SwapBuffers(wglGetCurrentDC());
		
		//set the new title
		int nCurrentMesh = ( pDoc->m_nCurrentMeshIndex + 1) % 2;
//		pDoc->SetPathName( pDoc->m_strTitle[nCurrentMesh] );				
     #endif
	}
}


// CMy3dViewerView 打印


void CMy3dViewerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMy3dViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMy3dViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMy3dViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMy3dViewerView::OnRButtonUp(UINT nFlags , CPoint point)
{

	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			BOOL bRet = pCMainFrame->m_pCToolBase->OnRButtonUp (nFlags, point);
			ReleaseCapture();

			if( bRet )
				this->Invalidate(FALSE);
		}

	//CGLEnabledView::OnLButtonUp(nFlags, point);
	CView::OnMButtonUp(nFlags, point);
}

void CMy3dViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


CBmpViewerData* CMy3dViewerView::GetBmpViewerData()
{
	CMy3dViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	return ((CBmpViewerData*)(pDoc->m_CBmpViewerData));
}



// CMy3dViewerView 诊断

#ifdef _DEBUG
void CMy3dViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CMy3dViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy3dViewerDoc* CMy3dViewerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy3dViewerDoc)));
	return (CMy3dViewerDoc*)m_pDocument;
}



#endif //_DEBUG


// CMy3dViewerView 消息处理程序
void CMy3dViewerView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	BOOL bRet;
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			bRet	= pCMainFrame->m_pCToolBase->OnLBDblClick (nFlags, point);
			if (bRet )
				this->Invalidate(FALSE);
		}
	CView::OnLButtonDblClk(nFlags, point);
}

void CMy3dViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			BOOL bRet = pCMainFrame->m_pCToolBase->OnLButtonDown (nFlags, point);
			
			if( bRet )
				this->Invalidate(FALSE);
		}
		
	//CView::OnLButtonDown(nFlags, point);
}

void CMy3dViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			BOOL bRet = pCMainFrame->m_pCToolBase->OnLButtonUp (nFlags, point);
			if( bRet )
				this->Invalidate(FALSE);

		}	
	CView::OnLButtonUp(nFlags, point);

	//render a new frame and save into Doc->m_CDIB
	//Render();
}

void CMy3dViewerView::OnMButtonDown(UINT nFlags, CPoint point)
{
	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			BOOL bRet = pCMainFrame->m_pCToolBase->OnMButtonDown (nFlags, point);
			SetCapture();
			if( bRet )
				this->Invalidate(FALSE);
		}

	CView::OnMButtonDown(nFlags, point);
}

void CMy3dViewerView::OnMButtonUp(UINT nFlags, CPoint point)
{
	
	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			BOOL bRet = pCMainFrame->m_pCToolBase->OnMButtonUp (nFlags, point);
			ReleaseCapture();

			if( bRet )
				this->Invalidate(FALSE);

		}	

	//CGLEnabledView::OnLButtonUp(nFlags, point);
	CView::OnMButtonUp(nFlags, point);
}

void CMy3dViewerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	BOOL bRet = TRUE ;

	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			bRet = pCMainFrame->m_pCToolBase->OnMouseMove (nFlags, point);
			
			if (bRet )
				this->Invalidate(FALSE);
		}	

	CView::OnMouseMove(nFlags, point);
}


BOOL CMy3dViewerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			if (pCMainFrame->m_pCToolBase->m_pCursorBase!=NULL) 
			{
				POINT ptCsr;
				GetCursorPos( &ptCsr); 
				this->ScreenToClient( &ptCsr ); 
				CDC *pDC;	
				
				(pDC = this->GetDC())->DPtoLP( &ptCsr); 
				this->ReleaseDC(pDC);
  				pCMainFrame->m_pCToolBase->m_pCursorBase->CursorInside(ptCsr.x, ptCsr.y);
 				pCMainFrame->m_pCToolBase->m_pCursorBase->CursorOutside(ptCsr.x, ptCsr.y);
			}
		}

	return 1;		
	//return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CMy3dViewerView::OnRButtonDown(UINT nFlags, CPoint point)
{

	CMainFrame *pCMainFrame = (CMainFrame*)AfxGetMainWnd ();
	if ( pCMainFrame != NULL )
		if ( pCMainFrame->m_pCToolBase != NULL )
		{
			BOOL bRet = pCMainFrame->m_pCToolBase->OnRButtonDown (nFlags, point);
			SetCapture(); // Removed -- zbf20160419
			if( bRet )
				this->Invalidate(FALSE);
		}


	CView::OnMButtonDown(nFlags, point);
}

void CMy3dViewerView::DrawStastic()
{
	CMy3dViewerDoc* pDoc = GetDocument();

	//HDC hDC = ::GetDC(m_hWnd);
	//HGLRC glContext = pDoc->m_pCOpenGL->GetContext();
	//wglMakeCurrent(hDC, glContext);	
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	glPushMatrix();	
	CString strAverage, strConv, strGeoStretch;

	int nCurrentMesh = ( pDoc->m_nCurrentMeshIndex + 1) % 2;

	strAverage = pDoc->m_struStatistic[nCurrentMesh].strAverageOfStretchRatio;
	strConv = pDoc->m_struStatistic[nCurrentMesh].strCoviarianceOfStretchRatio;
	strGeoStretch = pDoc->m_struStatistic[nCurrentMesh].strGeoRatio;
				
	((COpenGL3DEx*)pDoc->m_pCOpenGL)->DrawText( "Average of Stretch Ratio: ", strAverage, (float)-0.6, (float)-1.1, (float)-0.5 );
	((COpenGL3DEx*)pDoc->m_pCOpenGL)->DrawText( "Convariance of Stretch Ratio: ", strConv, (float)-0.6, (float)-1.2, (float)-0.5 );
	((COpenGL3DEx*)pDoc->m_pCOpenGL)->DrawText( "Geometry Stretch Ratio: ", strGeoStretch, (float)-0.6, (float)-1.3, (float)-0.5 );			

	glPopMatrix();
	
	glFinish();
}

void CMy3dViewerView::Render()
	{
	// Get client geometry 
	CRect rect; 
	GetClientRect(&rect); 
	CSize size(rect.Width(),rect.Height()); 
	// getting device context
	
	int nWid=size.cx;
	int nHei=size.cy;
	
	// Alloc pixel bytes 
	int NbBytes =size.cx*size.cy;
	int nScanWid=WIDTHBYTES(size.cx*24);
	
	unsigned char *pPixelData = new unsigned char[4*NbBytes]; 
		
	glReadPixels(0, 0,size.cx, size.cy,
		GL_RGBA,GL_UNSIGNED_BYTE,
		pPixelData); 

	HANDLE hDib = GetDocument()->m_CDIB.CreateDibFromColorBuffer(nWid, nHei, pPixelData);

	delete pPixelData;
	if ( hDib != NULL )
	{
		GetDocument()->m_CBmpViewerData->SetNewPosition();
		GetDocument()->m_CBmpViewerData->SetDIBHandle(hDib);
		//GetDocument()->m_CDIBOrg.FreeDib();
		GetDocument()->UpdateView();
		
        //GetDocument()->SetPathName(" 2D Image... ");
	}
}

BOOL CMy3dViewerView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class

	BOOL bRet = CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);

	if (bRet)
	{
		CMy3dViewerDoc* pDoc = GetDocument();
		pDoc->m_pCOpenGL->InitOpenGL(GetSafeHwnd());
	}

	return bRet;
}


void CMy3dViewerView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	CMy3dViewerDoc* pDoc = GetDocument();
	pDoc->m_pCOpenGL->OpenGLSize(cx, cy);

	// TODO: Add your message handler code here	
}

void CMy3dViewerView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	void CGLDemoView::OnTimer(UINT nIDEvent) 
{
	CMy3dViewerDoc* pDoc = GetDocument();
	pDoc->m_pCOpenGL->OpenGLTimer((CRotateType)nIDEvent);
	
//	CView::OnTimer(nIDEvent);
}
	CView::OnTimer(nIDEvent);
}



void CMy3dViewerView::OnMenuiCaptureImage()
{
	// TODO: 在此添加命令处理程序代码

		Render();
}
