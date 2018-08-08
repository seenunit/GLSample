#pragma once


#include "Options.h"

#include "afxwin.h"
#include <vector>

using namespace std;
class CGLSampleDoc;
// CBSpline dialog
class CBSplineDlg : public CDialog
{
public:
	COPTIONS *m_pstOpt;
	CGLSampleDoc *m_pCurrentDoc;
	CSpinButtonCtrl m_UpDown;
	vector< CBSpline *> m_vecBSpline;
	vector< CGeomPoint > m_vecGeomPts;
	int m_iNumColumns;

public:
	CBSplineDlg( CGLSampleDoc *pDoc, eBSplineMethod eMethodBspline );
	CBSplineDlg( CGLSampleDoc *pDoc, CWnd* pParent = NULL );   // standard constructor
	virtual ~CBSplineDlg();

// Dialog Data
	enum { IDD = IDD_BSPLINE };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBsplineBrowse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBsplineCreate();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCancel();
	CEdit m_editFilename;
	CString m_strKnotVector;
	double m_dParam2;
	CEdit m_CtrlPonitsEdit;
	int m_iDegree;
	int m_nCtrlPts;
	double m_dParam1;
	afx_msg void OnBnClickedBsplineCntrlptsButton();
	int m_iRed;
	int m_iGreen;
	int m_iBlue;
	eBSplineMethod m_eMethodBspline;
	afx_msg void OnBnClickedRefresh();
};