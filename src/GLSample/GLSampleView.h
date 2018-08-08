// GLSampleView.h : interface of the CGLSampleView class
//

#define BUFSIZE 516

#pragma once
#include "atltypes.h"

class CGLSampleView : public CView
{
protected: // create from serialization only
	CGLSampleView();
	DECLARE_DYNCREATE(CGLSampleView)

// Attributes
public:
	CGLSampleDoc* GetDocument() const;

// Operations
public:
	void RenderScene(GLenum glRenderMode);	//OPENGL Render OpenGL scene
	void RenderCSYS(void);
	void OutputGlError(char* label) ;		//OPENGL function to display OpenGL errors using TRACE
	void SetViewVolume();
	void SetBsplineFit();

	//Pan, Zoom and Rotate
	void RotateView(CPoint point);
	void PanView(CPoint point);
	void ZoomView( double dScale, CPoint point);
	void PickSquares(CPoint point);
	void ModifyEntity(CPoint point);

	void PrepareScene();

	ParaViewOperation m_currentOperation;

	//Current selected Bspline ID
	unsigned int m_iCurrentBSplineID;
	unsigned int m_iCurrentPointID;
	unsigned int m_iCurrentLineID;
	unsigned int m_iPickEntityID;

//Variables
	int m_iWinWidth;
	int m_iWinHeight;
	CPoint m_currentPoint;
	CPoint m_Startpoint;

	GLdouble m_xRotate;
	GLdouble m_yRotate;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGLSampleView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	BOOL SetWindowPixelFormat(HDC hDC);
	int m_GLPixelIndex;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL CreateViewGLContext(HDC hDC);
protected:
	HGLRC m_hGLContext;
public:
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	BOOL m_bRightDownButton;
	BOOL m_bLeftdownButton;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	CGeomPoint TransCPointToWorld(CPoint point);
	afx_msg void OnViewFront();
	afx_msg void OnViewFit();
	afx_msg void OnViewPan();
};

#ifndef _DEBUG  // debug version in GLSampleView.cpp
inline CGLSampleDoc* CGLSampleView::GetDocument() const
   { return reinterpret_cast<CGLSampleDoc*>(m_pDocument); }
#endif

