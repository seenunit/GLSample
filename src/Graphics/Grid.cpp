// Grid.cpp : implementation file
//

#include "stdafx.h"
#include "GLSampleDoc.h"
#include "Grid.h"


// CGrid dialog

IMPLEMENT_DYNAMIC(CGrid, CDialog)

CGrid::CGrid( COPTIONS *pstOpt, CWnd* pParent /*=NULL*/)
	: CDialog(CGrid::IDD, pParent)
	, m_nGridSize(10)
{
	m_pstOpt = pstOpt;
}

CGrid::~CGrid()
{
}

void CGrid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CGrid::OnInitDialog()
{
	CDialog::OnInitDialog();

	//intialize edit box

	CEdit *pebGridSize = ( CEdit * ) GetDlgItem( IDC_GRIDSIZE );

	char sText[80] = {0};
	sprintf( sText, "%d", m_pstOpt->m_nGridSize );
	pebGridSize->SetWindowTextA( sText );

	return TRUE;
}



BEGIN_MESSAGE_MAP(CGrid, CDialog)
	ON_BN_CLICKED(IDOK, &CGrid::OnBnClickedOk)
END_MESSAGE_MAP()


// CGrid message handlers

void CGrid::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();

	char sText[80] = {0};
	CEdit *pebGridSize = ( CEdit * ) GetDlgItem( IDC_GRIDSIZE );
	pebGridSize->GetWindowTextA( sText, sizeof sText );

	m_nGridSize = atoi( sText );

	m_pstOpt->m_nGridSize = m_nGridSize;
}
