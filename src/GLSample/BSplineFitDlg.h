#pragma once

#include "Options.h"
#include "afxwin.h"


// CBSplineFitDlg dialog

class CBSplineFitDlg : public CDialog
{
	DECLARE_DYNAMIC(CBSplineFitDlg)

public:
	CBSplineFitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBSplineFitDlg();

// Dialog Data
	enum { IDD = IDD_BSPLINE_FITMETHOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	eBSplineFitMethod m_eMethodBsplineFit;
	eBSplineFitMethod GetBSplineFitMethod();
	virtual BOOL OnInitDialog();
	CButton m_rbutUniformSpace;
	CButton m_rbutChordLength;
	CButton m_rbutCentriPetal;
};
