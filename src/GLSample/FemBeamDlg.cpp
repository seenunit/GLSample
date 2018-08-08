// FemBeamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GLSample.h"
#include "FemBeamDlg.h"
//#include "afxdialogex.h"


// CFemBeamDlg dialog

IMPLEMENT_DYNAMIC(CFemBeamDlg, CDialog)

CFemBeamDlg::CFemBeamDlg(CGLSampleDoc *pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CFemBeamDlg::IDD, pParent)
    , m_dBeamLength(0)
    , m_iBeamNodes(0)
    , m_bBeamModuli(0)
    , m_dBeamArea(0)
{
    m_pCurrentDoc = pDoc;
}

CFemBeamDlg::~CFemBeamDlg()
{
}

void CFemBeamDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_BEAMEDIT_LENGTH, m_dBeamLength);
    DDX_Text(pDX, IDC_BEAMEDIT_NODES, m_iBeamNodes);
    DDX_Text(pDX, IDC_BEAMEDIT_YOUNGS, m_bBeamModuli);
    DDX_Text(pDX, IDC_BEAMEDIT_AREA, m_dBeamArea);
}


BEGIN_MESSAGE_MAP(CFemBeamDlg, CDialog)
    ON_BN_CLICKED(IDC_FEM_CANCEL, &CFemBeamDlg::OnBnClickedFemCancel)
    ON_BN_CLICKED(IDC_FEM_OK, &CFemBeamDlg::OnBnClickedFemOk)
END_MESSAGE_MAP()


// CFemBeamDlg message handlers


void CFemBeamDlg::OnBnClickedFemCancel()
{
    CDialog::OnCancel();
    //DestroyWindow();
}


void CFemBeamDlg::OnBnClickedFemOk()
{
    UpdateData(TRUE);//to get updated data from edit box.

    double dBeamLength = 0.0;
    int iBeamNodes = 0;
    double YModuli = 0.0;
    double dSectionArea = 0.0;

    CGeomPoint *pdCtrlPt = new CGeomPoint[2];
    for(int i = 0; i < 2; i++)
        for( int j = 0; j < 3; j++ )
            pdCtrlPt[i].m_Coord[j] = 0.0;

    pdCtrlPt[0].m_Coord[0] = -(m_dBeamLength/2);
    pdCtrlPt[1].m_Coord[0] = (m_dBeamLength/2);

    //Add beam
    m_pCurrentDoc->AddBeam(m_dBeamLength, m_iBeamNodes, m_bBeamModuli, m_dBeamArea, pdCtrlPt);

    CDialog::OnOK();
    return;
}
