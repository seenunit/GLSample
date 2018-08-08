#pragma once
#include "afxwin.h"
#include "Options.h"
#include "Resource.h"
#include "resource_id.h"

// CGrid dialog
class CGrid : public CDialog
{
	DECLARE_DYNAMIC(CGrid)
public:
	CGrid( COPTIONS *pstOpt, CWnd* pParent = NULL );   // standard constructor
	virtual ~CGrid();

	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_GRID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	int m_nGridSize;
	COPTIONS *m_pstOpt;
public:
	afx_msg void OnBnClickedOk();
};
