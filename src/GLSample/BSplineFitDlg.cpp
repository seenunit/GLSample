// BSplineFitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BSplineFitDlg.h"
#include "Options.h"


// CBSplineFitDlg dialog

IMPLEMENT_DYNAMIC(CBSplineFitDlg, CDialog)

CBSplineFitDlg::CBSplineFitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBSplineFitDlg::IDD, pParent)
{

}

CBSplineFitDlg::~CBSplineFitDlg()
{
}

void CBSplineFitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_BSPLINE_FIT_UNIFORMSPACE, m_rbutUniformSpace);
	DDX_Control(pDX, IDC_RADIO_BSPLINE_FIT_CHRODLENGTH, m_rbutChordLength);
	DDX_Control(pDX, IDC_RADIO_BSPLINE_FIT_CENTRIPETAL, m_rbutCentriPetal);
}


BEGIN_MESSAGE_MAP(CBSplineFitDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CBSplineFitDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBSplineFitDlg message handlers

void CBSplineFitDlg::OnBnClickedOk()
{
	int iID = GetCheckedRadioButton(IDC_RADIO_BSPLINE_FIT_UNIFORMSPACE, IDC_RADIO_BSPLINE_FIT_CENTRIPETAL);
	if(iID == IDC_RADIO_BSPLINE_FIT_UNIFORMSPACE)
		m_eMethodBsplineFit = BSPLINE_FIT_UNIFORMSPACE;
	else if(iID == IDC_RADIO_BSPLINE_FIT_CHRODLENGTH)
		m_eMethodBsplineFit = BSPLINE_FIT_CHORDLENGTH;
	else if(iID == IDC_RADIO_BSPLINE_FIT_CENTRIPETAL)
		m_eMethodBsplineFit = BSPLINE_FIT_CENTRIPETAL;

	OnOK();
}

eBSplineFitMethod CBSplineFitDlg::GetBSplineFitMethod()
{
	return m_eMethodBsplineFit;
}
BOOL CBSplineFitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_rbutUniformSpace.SetCheck(TRUE);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
