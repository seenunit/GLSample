// Circle.cpp : implementation file
//

#include "stdafx.h"
#include <iostream>
#include "GLSampleDoc.h"
#include "Circle.h"
#include "Line.h"
#include <CMath.h>

using namespace std;
// CCircle
IMPLEMENT_SERIAL(CCircle, CGeomEntity, 1)

CCircle::CCircle()
{
}

CCircle::CCircle( CGeomPoint ptCenter, double dRadius, int nSpanPts, Vector vecNormal )
{
	m_Center = ptCenter;
	m_dRadii = dRadius;
	m_nSpanPts = nSpanPts;
	m_vecNormal = vecNormal;
	try {
		m_pCirPts = new CGeomPoint[nSpanPts];
	}
	
	catch (bad_alloc xa)
	{
		cout << "Allocation Failure\n";
		exit(EXIT_FAILURE);
	}
	CreateCircle_v( m_nSpanPts, m_dRadii, m_Center, m_vecNormal, m_pCirPts);
}

CCircle::~CCircle()
{
	delete [] m_pCirPts;
}
// CCircle member functions
void CCircle::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);

	m_Center.Serialize( ar );
	
	if(ar.IsStoring())
		ar << m_dRadii;
	else 
		ar >> m_dRadii;
}
void CCircle::Draw( CDC *pDc )
{
	//CreateArc_i( m_nSpanPts, m_dRadii, m_Center, m_pCirPts );

	//Vector NormVect( 0, 0, 1 );

	CreateCircle_v( m_nSpanPts, m_dRadii, m_Center, m_vecNormal, m_pCirPts);

	double dMatrix[4*4] = {1, 0, 0, 0,
						   0, 1, 0, 0,
					       0, 0, 0, 0,
					       0, 0, 0, 1 };

	Matrix matTransf( dMatrix, 4, 4 );

	DrawCurve( pDc, m_nSpanPts, m_pCirPts, matTransf );
}
void CCircle::DrawCircle( CDC *pDc, Matrix &matTransf )
{
	DrawCurve( pDc, m_nSpanPts, m_pCirPts, matTransf );
}

void CCircle::DrawCurve( CDC *pDc, int nSpanPts, CGeomPoint* pgptCurve, Matrix &matTransf )
{
	//Matrix matCurvePt( nSpanPts, 4 );

	//for( int i = 0; i < nSpanPts; i++ )
	//{
	//	for( int j = 0; j < 3; j++ )
	//		matCurvePt( i, j ) = pgptCurve[i].m_Coord[j];
	//	matCurvePt( i , 3 ) = 1;
	//}

	//Matrix matTransPt( nSpanPts, 4 ); 
	//matTransPt = matCurvePt * matTransf;  // transform the circle..

	//int LocIndex = 0;
	//CPoint *Locations = new CPoint[nSpanPts]; //temp storage of pts

	//CSize sizeWndExt = pDc->GetWindowExt();
	//CSize sizeViewExt = pDc->GetViewportExt();

	//for(int i = 0; i < nSpanPts; i++)
	//{
	//	Locations[i].x = matTransPt( i, 0 ) * sizeWndExt.cx;
	//	Locations[i].y = matTransPt( i, 1 ) * sizeWndExt.cy;
	//}

	//pDc->MoveTo( Locations[0]);
	//for(int i = 0; i < nSpanPts; i++)
	//	pDc->LineTo( Locations[i] );

	//pDc->LineTo( Locations[0] ); // For closing the circle.

	//delete [] Locations;

	
	for(int i = 0; i < nSpanPts-1; i++)
	{
		CLine line(pgptCurve[i], pgptCurve[i+1]);
		line.DrawLine(pDc, matTransf);
	}
}
	
void CreateArc_i( int nSpanpts, double dRadii, CGeomPoint ptCenter, CGeomPoint* pCirPts )
{

	double start_ang = 0;
	double dtheta  = MAXANG/(float)nSpanpts;
	double d_ang = dtheta * PI/180;

	CGeomPoint gPtStart, gPtEnd;
	
	start_ang = (start_ang)* PI/180;

	gPtStart.m_Coord[0] = dRadii * cos(start_ang);
	gPtStart.m_Coord[1] = dRadii * sin(start_ang);

	pCirPts[0] = gPtStart;
	
	for(int i = 1; i < nSpanpts; i++)
	{
		gPtEnd.m_Coord[0] =  gPtStart.m_Coord[0] * cos(d_ang) - gPtStart.m_Coord[1] * sin(d_ang);
		gPtEnd.m_Coord[1] =  gPtStart.m_Coord[0] * sin(d_ang) + gPtStart.m_Coord[1] * cos(d_ang);

		pCirPts[i] = gPtEnd;

		gPtStart = gPtEnd;
	}

	for(int i = 0; i < nSpanpts; i++)
		for( int j = 0; j < 2; j++ )
			pCirPts[i].m_Coord[j] =  pCirPts[i].m_Coord[j] + ptCenter.m_Coord[j];
}
void CreateCircle_v( int nSpanpts, double dRadii, CGeomPoint ptCenter, Vector NormVect, CGeomPoint* pCirPts )
{
	double dAng = ( MAXANG )* PI/180;

	dAng = dAng/nSpanpts;

	Vector vCentre( ptCenter.m_Coord[0], ptCenter.m_Coord[1], ptCenter.m_Coord[2] );

	NormVect = NormVect.UnitVector(); // making normal unit vector.
	Vector vecCentreUnit = vCentre.UnitVector();

	Vector tempVec = vecCentreUnit * NormVect ;

	Vector vAbtPostion = vCentre + tempVec*dRadii;

	pCirPts[0].m_Coord[0] = vAbtPostion.getx();
	pCirPts[0].m_Coord[1] = vAbtPostion.gety();
	pCirPts[0].m_Coord[2] = vAbtPostion.getz();

	for( int i = 1; i < nSpanpts; i++ )
	{
		Vector vPt = VectorRotation( vAbtPostion, NormVect, vCentre, dAng );

		pCirPts[i].m_Coord[0] = vAbtPostion.getx();
		pCirPts[i].m_Coord[1] = vAbtPostion.gety();
		pCirPts[i].m_Coord[2] = vAbtPostion.getz();

		vAbtPostion = vPt;
	}
}


// CCircleDialog dialog

IMPLEMENT_DYNAMIC(CCircleDialog, CDialog)

CCircleDialog::CCircleDialog( COPTIONS *pstructOpt, CWnd* pParent /*=NULL*/)
	: CDialog(CCircleDialog::IDD, pParent)
{
	m_pstructOpt = pstructOpt;
	/*m_dRadius = m_dX = m_dY = m_dZ = 0.0;
	m_dVeci = m_dVecj = 0.0;
	m_dVeck = 1.0;*/
}

CCircleDialog::~CCircleDialog()
{
}

void CCircleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_R, m_dRadius);
	//DDV_MinMaxInt(pDX, m_dRadius, 0, 1000);
	DDX_Control(pDX, IDC_EDIT_X, m_dX);
	DDX_Control(pDX, IDC_EDIT_Y, m_dY);
	DDX_Control(pDX, IDC_EDIT_Z, m_dZ);
	DDX_Control(pDX, IDC_EDIT_I, m_dVeci);
	DDX_Control(pDX, IDC_EDIT_J, m_dVecj);
	DDX_Control(pDX, IDC_EDIT_K, m_dVeck);
}


BEGIN_MESSAGE_MAP(CCircleDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CCircleDialog::OnBnClickedOk)
END_MESSAGE_MAP()
// CCircleDialog message handlers

void CCircleDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strRadius;
	m_dRadius.GetWindowTextA( strRadius );
	m_pstructOpt->m_dCirRadius = atof( strRadius.GetBuffer() );
	//m_dX.GetWindowTextA( strRadius );
	//m_pstructOpt->m_gptCirCenter.m_Coord[0] = atof( strRadius.GetBuffer() );
	//m_dY.GetWindowTextA( strRadius );
	//m_pstructOpt->m_gptCirCenter.m_Coord[1] = atof( strRadius.GetBuffer() );
	//m_dZ.GetWindowTextA( strRadius );
	//m_pstructOpt->m_gptCirCenter.m_Coord[2] = atof( strRadius.GetBuffer() );
	m_dVeci.GetWindowTextA( strRadius );
	m_pstructOpt->vecCirNorm.x = atof( strRadius.GetBuffer() );
	m_dVecj.GetWindowTextA( strRadius );
	m_pstructOpt->vecCirNorm.y = atof( strRadius.GetBuffer() );
	m_dVeck.GetWindowTextA( strRadius );
	m_pstructOpt->vecCirNorm.z = atof( strRadius.GetBuffer() );
	OnOK();
}
