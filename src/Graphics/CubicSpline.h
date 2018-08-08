#pragma once
#include "Matrix.h"
#include "afxwin.h"
#include "GeomEntity.h"

// CCubicSpline dialog

class CCubicSpline : public CGeomEntity
{
	DECLARE_SERIAL( CCubicSpline )
//	DECLARE_DYNAMIC(CCubicSpline)

	int m_nCtrlPts;
	int m_nSpanPts;
	int m_nCurvePts;
	bool m_bClamped; // clamped boundry condition
	bool m_bRelaxed; // Relaxed boundry condition

	CGeomPoint *m_pCtrlPt;
	CGeomPoint *m_pCurvePts;
	double *m_pdEndTanVec;

public:
	CCubicSpline( );   // standard constructor
	CCubicSpline(int nCtrlPts, int nSpanPts, CGeomPoint *pCtrlPts, double *pEndTanVec, bool bClamped, bool bRelaxed );
	void Serialize(CArchive &ar);
	//BOOL OnInitDialog();
	void Draw(CDC *pDc);
	void DrawCubicSpline(CDC *pDc, Matrix &matTransf); 
	void DrawCurve( CDC *pDc, int nCurvePts, CGeomPoint *pdCurvePts );
	virtual ~CCubicSpline();

};

void cubicspline_i( int nCtrPts, int nSpanPts, CGeomPoint *pCtrPts, double *pEndTanVec, bool bClamped, bool bRelaxed, CGeomPoint *pCurvePts, int &nCurvePts);

//class CCubicSplineDlg : public CDialog
//{
//	DECLARE_DYNAMIC( CCubicSplineDlg )
//
//	bool m_bClamped; // clamped boundry condition
//	bool m_bRelaxed; // Relaxed boundry condition
//
//	public:
//	CCubicSplineDlg(CWnd* pParent = NULL);   // standard constructor
//	virtual ~CCubicSplineDlg();
//
//	afx_msg BOOL OnInitDialog();
//	
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//
//	DECLARE_MESSAGE_MAP()
//public:
//	// Dialog Data
//	enum { IDD = IDD_CUBICSPLINE };
//	CButton m_buttonClamped;
//	CButton m_buttonRelaxed;
//public:
//	afx_msg void OnBnClickedClamped();
//	afx_msg void OnBnClickedRelaxed();
//};