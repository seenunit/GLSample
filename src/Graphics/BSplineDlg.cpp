#include "stdafx.h"

#include "BSpline.h"
#include "BSurf.h"
#include <iostream>

#include <cstring>
#include <fstream>
#include <io.h>

#include "BSplineDlg.h"
#include "BSplineFitDlg.h"

// CBSplineDlg dialog

//IMPLEMENT_DYNAMIC(CBSpline, CDialog)

CBSplineDlg::CBSplineDlg( CGLSampleDoc *pDoc, eBSplineMethod eMethodBspline )
	: CDialog( )
	, m_strKnotVector(_T(""))
	, m_dParam2(0)
	, m_iDegree(2)
	, m_nCtrlPts(0)
	, m_dParam1(0)
	, m_iRed(255)
	, m_iGreen(245)
	, m_iBlue(143)
{
	m_iNumColumns = 0;
	m_pCurrentDoc = pDoc;
	m_eMethodBspline = eMethodBspline;
}

CBSplineDlg::CBSplineDlg( CGLSampleDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CBSplineDlg::IDD, pParent)
{
	m_pCurrentDoc = pDoc;
}

CBSplineDlg::~CBSplineDlg()
{
}

void CBSplineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BSPLINE_EDIT1, m_editFilename);
	DDX_Text(pDX, IDC_BSPLINE_KNOTVEC_EDIT, m_strKnotVector);
	DDX_Text(pDX, IDC_BSPLINE_PARAM2_EDIT, m_dParam2);
	DDX_Text(pDX, IDC_BSPLINE_DEGREE_EDIT, m_iDegree);
	DDX_Text(pDX, IDC_BSPLINE_NCNTRLPTS_EDIT, m_nCtrlPts);
	DDX_Text(pDX, IDC_BSPLINE_PARAM1_EDIT, m_dParam1);
	DDX_Text(pDX, IDC_BSPLINE_RED_EDIT, m_iRed);
	DDX_Text(pDX, IDC_BSPLINE_GREEN_EDIT, m_iGreen);
	DDX_Text(pDX, IDC_BSPLINE_BLUE_EDIT, m_iBlue);
}
BEGIN_MESSAGE_MAP(CBSplineDlg, CDialog)
	ON_BN_CLICKED(IDC_BSPLINE_OPEN_BROWSE, &CBSplineDlg::OnBnClickedBsplineBrowse)
	ON_BN_CLICKED(IDOK, &CBSplineDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BSPLINE_APPLY, &CBSplineDlg::OnBnClickedBsplineCreate)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDCANCEL, &CBSplineDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BSPLINE_CNTRLPTS_BUTTON, &CBSplineDlg::OnBnClickedBsplineCntrlptsButton)
	ON_BN_CLICKED(ID_REFRESH, &CBSplineDlg::OnBnClickedRefresh)
END_MESSAGE_MAP()




// CBSplineDlg message handlers

void CBSplineDlg::OnBnClickedBsplineBrowse()
{
	//static char szFilter[] = "Srt file (*.srt)|*.srt|";

	//CFileDialog fileDlg( TRUE, (LPCTSTR)"srt", NULL, OFN_EXPLORER|OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilter );
	//fileDlg.m_ofn.lpstrTitle = "BSpline";

	//if( fileDlg.DoModal() == IDOK )
	//{
	//	CString cstrFilename = ( CString )fileDlg.GetPathName();
	//	int fileLength = cstrFilename.GetLength();
	//	char strFileName[_MAX_PATH] = {0};
	//	strcpy_s( strFileName, fileLength+1, (char *)cstrFilename.GetBuffer( fileLength ) );
	//	
	//	m_editFilename.SetSel( 0 , -1 );
	//	m_editFilename.ReplaceSel( strFileName );
	//}
}

void CBSplineDlg::OnBnClickedOk()
{
	DestroyWindow();
	//OnOK();
}

void CBSplineDlg::OnBnClickedBsplineCreate()
{
	//int iStrLen = m_editFilename.LineLength();
	//char cFilePath[_MAX_PATH] = {0};
	//m_editFilename.GetWindowText(cFilePath, iStrLen+1);
	//m_editFilename.SetWindowText("");
	//BsplineDataRead( m_pCurrentDoc, cFilePath, m_vecBSpline );

	UpdateData(TRUE);//to get updated data from edit box.

	int nDataPt = 0, nCtrlPt = 0;
	int nOrder = 0, nSpanPt = BSPLINE_SPANS;

	// No. of control points and order.
	nCtrlPt = m_nCtrlPts;
	nOrder = m_iDegree + 1;

	//Control Points
	CString strText;
	int iCount = (int) m_vecGeomPts.size();
	CGeomPoint *pdCtrlPt = new CGeomPoint[iCount];
	for(int i = 0; i < iCount; i++)
		for( int j = 0; j < 3; j++ )
			pdCtrlPt[i].m_Coord[j] =  m_vecGeomPts[i].m_Coord[j];
	//m_vecGeomPts.clear(); //uncomment to refresh the geom pts after use

    //Add b-spline surface and returns.
	if(m_eMethodBspline == BSPLINE_SWEEP)
	{
		int iUSpan = BSURF_U_SPANS;
		int iVSpan = BSURF_V_SPANS;
		int iNumVPts = m_iNumColumns;
		int iNumUPts = iCount/iNumVPts;
		m_pCurrentDoc->AddBSurf(pdCtrlPt, iNumUPts, iNumVPts, iUSpan, iVSpan);
		return;
	}

	if(m_eMethodBspline == BSPLINE_FAIR)
		nCtrlPt = iCount; //changing the no of control points no. of pts given
	else if(m_eMethodBspline == BSPLINE_FIT)
	{
		if(nCtrlPt > iCount)
		{
			CString strMsg;
			strMsg.Format("Pleaase Check the No. of control points value entered \r\nNo. of Control points(%d) has to be less than or equal to no. of Data points(%d)", nCtrlPt, iCount);
			AfxMessageBox(strMsg.GetBuffer());
			return;
		}
		if(nCtrlPt <= 0)
			nCtrlPt = nDataPt = iCount;
		else
			nDataPt = iCount;
	}

	if(nOrder > nCtrlPt)
	{
		CString strMsg;
		strMsg.Format("Pleaase Check the Degree entered \r\nDegree(%d)+1 has to be less than or equal to no. of Control points(%d)", nOrder-1, nCtrlPt);
		AfxMessageBox(strMsg.GetBuffer());
		return;
	}

	// Parameter Range
	double *pdParaRange = new double[2];
	pdParaRange[0] = m_dParam1;
	pdParaRange[1] = m_dParam2;

	if( m_dParam2 == 0 )
	{
		if(pdParaRange) delete [] pdParaRange;
		pdParaRange = NULL;
	}

	//Knot Vector
	char *pcKnotVector = new char[m_strKnotVector.GetLength()+1];
	strcpy_s( pcKnotVector, m_strKnotVector.GetLength()+1, m_strKnotVector.GetBuffer(m_strKnotVector.GetLength()));
	double *pdKnotVec = NULL;
	char *pcContext = NULL;
	if( strcmp( pcKnotVector, "" ) )
	{
		pdKnotVec = new double[(nCtrlPt+nOrder)];
		for( int i = 0; i < ( nCtrlPt+nOrder ); i++ )
		{
			if( !i )
				pdKnotVec[i] = atof( strtok_s( pcKnotVector, " \t", &pcContext ) );
			else
			{
				char *pStr = strtok_s( NULL, " \t", &pcContext );
				if(pStr == NULL)
					return;
				pdKnotVec[i] = atof(pStr);
			}
		}
		if( pcKnotVector ) delete[] pcKnotVector;
	}

	//Add the bSpline
	if(nCtrlPt > 1)
	{
		CBSpline *pBspline = NULL;
		if(m_eMethodBspline == BSPLINE_FAIR)
			pBspline = m_pCurrentDoc->AddBSpline( nCtrlPt, nOrder, nSpanPt, pdParaRange, pdKnotVec, pdCtrlPt, RGB(m_iRed, m_iGreen, m_iBlue) );
		else if(m_eMethodBspline == BSPLINE_FIT)
		{
			eBSplineFitMethod eMethodBsplineFit;
			CBSplineFitDlg dlgBSplineFit;
			if(dlgBSplineFit.DoModal() == IDOK)
				eMethodBsplineFit = dlgBSplineFit.GetBSplineFitMethod();
			else 
				return;
				
			pBspline = m_pCurrentDoc->AddBSpline( nCtrlPt, nOrder, nSpanPt, nDataPt, eMethodBsplineFit, pdParaRange, pdKnotVec, pdCtrlPt, RGB(m_iRed, m_iGreen, m_iBlue) );
		}
			
		m_vecBSpline.push_back(pBspline); //not necessary.

		// Replace the current data in Edit boxs
		m_nCtrlPts = pBspline->m_iCtrlPt;
		m_iDegree = pBspline->m_iDegree;
		m_dParam1 = pBspline->m_dParamRange[0];
		m_dParam2 = pBspline->m_dParamRange[1];
		m_strKnotVector = "";
		for(int i = 0; i < (m_nCtrlPts+m_iDegree+1); i++)
			m_strKnotVector.AppendFormat("%f ", pBspline->m_pdKnotVec[i] );
	}
	
	if(pdCtrlPt) delete [] pdCtrlPt;
	if(pdKnotVec) delete [] pdKnotVec;
	if(pdParaRange) delete [] pdParaRange;

	UpdateData(FALSE);//refresh the edit boxs

	//m_UpDown.SetRange( 2, (short)( m_pBSpline->m_iCtrlPt - 1 ) );
	//m_UpDown.SetPos( (short)( m_pBSpline->m_iDegree ) );
}

BOOL CBSplineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	CString strDialogName("BSpline");
	if(m_eMethodBspline == BSPLINE_FAIR)
		strDialogName.Format("BSpline Curve-Fair");
	else if(m_eMethodBspline == BSPLINE_FIT)
		strDialogName.Format("BSpline Curve-Fit");
	SetWindowText(_T(strDialogName));
	//RECT rectUpDown;
	//// Create up-down control
	//rectUpDown.bottom = rectUpDown.right = 50;
	//rectUpDown.left = rectUpDown.top = 10;
	//BOOL bSucc = m_UpDown.Create( WS_VISIBLE | WS_CHILD | WS_BORDER | UDS_SETBUDDYINT | 
	//	UDS_ALIGNRIGHT,rectUpDown, this, IDD_SPIN );
	////  Set range and intial position of up-down control
	//m_UpDown.SetRange( 0, 99 );
	//// Set buddy window
	//m_UpDown.SetBuddy( GetDlgItem( IDC_BSPLINE_DEGREE_EDIT ) );
	//// set Intial position
	//m_UpDown.SetPos( 3 );

	CWnd *pEdit1 = GetDlgItem(IDC_BSPLINE_EDIT1);
	pEdit1->EnableWindow(FALSE);

	CWnd *pEdit2 = GetDlgItem(IDC_BSPLINE_SAVEFILE_EDIT);
	pEdit2->EnableWindow(FALSE);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBSplineDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar != (CScrollBar *)&m_UpDown ) return;

	int iDegree = LOWORD( m_UpDown.GetPos() );

	m_pCurrentDoc->SetModifiedFlag();
	m_pCurrentDoc->UpdateAllViews( NULL );

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CBSplineDlg::OnBnClickedCancel()
{
	DestroyWindow();
	//OnCancel();
}

void CBSplineDlg::OnBnClickedBsplineCntrlptsButton()
{
	// TODO: Add your control notification handler code here
	static char szFilter[] = "Point Data File (*.dat)|*.dat|";

	CFileDialog fileDlg( TRUE, "dat", NULL, OFN_EXPLORER|OFN_EXTENSIONDIFFERENT|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilter );
	fileDlg.m_ofn.lpstrTitle = "BSpline Poles";

	if( fileDlg.DoModal() == IDOK )
	{
		CString strFilename = ( CString )fileDlg.GetPathName();
		
		// Open the File
		ifstream inFile;
		inFile.open( (LPCTSTR) strFilename );
		
		char cReadLine[_MAX_PATH] = {0};
		CString strReadLine;

		char cSeps[] = " ,\t\n";

		bool bBSpline = false, bBSurf = false;

		m_vecGeomPts.clear();
		while( !inFile.eof() )
		{
			inFile.getline( cReadLine, _MAX_PATH);
			strReadLine.Format( "%s", cReadLine );
			
			if( !strReadLine.Compare("") )
				return;

			if( strReadLine.Compare("[BSURF]") == 0)
			{
				bBSurf = true;
				bBSpline = false;
				continue;
			}

			if( strReadLine.Compare("[BSPLINE]") == 0)
			{
				bBSpline = true;
				bBSurf = false;
				continue;
			}

			if(bBSurf)
			{
				CGeomPoint dCtrlPt;
				char *pcContext = NULL;
				char cSeperators[] = " ,{}";
				char *pcStr = new char[strReadLine.GetLength()+1];
				strcpy_s(pcStr, strReadLine.GetLength()+1, strReadLine.GetBuffer());

				int iRowPtCnt = 0;
				char *pcCompare = strtok_s(pcStr, cSeperators, &pcContext);
				
				while(pcCompare != NULL)
				{
					for(int j = 0; j < 3; j++)					
					{
						dCtrlPt.m_Coord[j] = atof( pcCompare );
						pcCompare = strtok_s(NULL, cSeperators, &pcContext);						
					}
					m_vecGeomPts.push_back(dCtrlPt);
					iRowPtCnt++;
				}
				m_iNumColumns = iRowPtCnt;
				if(pcStr) delete [] pcStr;
			}
			if(bBSpline)
			{
				CGeomPoint dCtrlPt;
				char *pcContext = NULL;
				for( int j = 0; j < 3; j++ )
				{
					if( !j )
						dCtrlPt.m_Coord[j] =  atof( strtok_s( strReadLine.GetBuffer(), cSeps, &pcContext ));
					else
						dCtrlPt.m_Coord[j] =  atof( strtok_s( NULL, cSeps, &pcContext ));
				}

				m_vecGeomPts.push_back(dCtrlPt);
			}
		}
	}

}

void CBSplineDlg::OnBnClickedRefresh()
{
	// TODO: Add your control notification handler code here
	// Refresh the data in Edit boxs
	m_nCtrlPts = 0;
	m_iDegree = 2;
	m_dParam1 = 0;
	m_dParam2 = 0;
	m_strKnotVector = "";
	m_vecGeomPts.clear(); //refresh the geom pts
	UpdateData(FALSE);//refresh the edit boxs
}
