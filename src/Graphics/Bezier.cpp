// Bezier.cpp : implementation file
//

#include "stdafx.h"
#include "GLSampleDoc.h"
#include "Bezier.h"
#include "Matrix.h"
#include <math.h>
#include <iostream>
#include "SampleMath.h"
#include "GeomEntity.h"
#include "Line.h"
#include <fstream>


using namespace std;

// CBezier dialog

//IMPLEMENT_DYNAMIC(CBezier, CDialog)
IMPLEMENT_SERIAL( CBezier, CGeomEntity/*CDialog*/, 1 )

CBezier::CBezier(/*CWnd* pParent*/ /*=NULL*/)
	/*: CDialog(CBezier::IDD, pParent)*/
{
	//try {
	//	m_pdCtrPts = new CDoublePoint[m_nCtrPts];
	//	m_pdCurvePts = new CDoublePoint[m_nSpanPts];
	//}
	//catch (bad_alloc xa) {
	//		cout << "Allocation Failure\n";
	//		exit(EXIT_FAILURE);
	//	}
}

CBezier::CBezier( int nCtrPts, int nSpanPts,  CGeomPoint *dCtrPts )
{
	m_nCtrPts = nCtrPts;
	m_nOrder = nCtrPts;
	m_nDegree = nCtrPts - 1;
	
	try {
			m_pdCtrPts = new CGeomPoint[nCtrPts];
			m_pdCurvePts = new CGeomPoint[nSpanPts];

			for( int i = 0; i < nCtrPts; i++ )
			{
				/*m_pdCtrPts[i].x = dCtrPts[i].x;
				m_pdCtrPts[i].y = dCtrPts[i].y;
				m_pdCtrPts[i].z = dCtrPts[i].z;*/
				for( int j = 0; j < THREEDIM; j++ )
					m_pdCtrPts[i].m_Coord[j] = dCtrPts[i].m_Coord[j];				
			}
	}

	catch (bad_alloc xa) {
			cout << "Allocation Failure\n";
			exit(EXIT_FAILURE);
		}
		m_nSpanPts = nSpanPts;

	bezier_b( m_nCtrPts, m_nSpanPts, m_pdCtrPts, m_pdCurvePts );
}

CBezier::~CBezier()
{
	delete [] m_pdCtrPts;
	delete [] m_pdCurvePts;
}

//void CBezier::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//}

//BEGIN_MESSAGE_MAP(CBezier, CGeomEntity/*CDialog*/)
//END_MESSAGE_MAP()


// CBezier message handlers

// CBezier routines implementation
void CBezier::Serialize(CArchive &ar)
{
	CGeomEntity::Serialize( ar );

	if(ar.IsStoring())
	{
		ar << m_nCtrPts << m_nDegree << m_nOrder << m_nSpanPts;
		for( int i = 0; i < m_nCtrPts; i++ )
			m_pdCtrPts[i].Serialize( ar );		
	}
	else
	{
		ar >> m_nCtrPts >> m_nDegree >> m_nOrder >> m_nSpanPts;
		try
		{
			m_pdCtrPts = new CGeomPoint[m_nCtrPts];
			m_pdCurvePts = new CGeomPoint[m_nSpanPts];
		}		
		catch (bad_alloc xa)
		{
			cout << "Allocation Failure\n";
			exit(EXIT_FAILURE);
		}		
		for( int i = 0; i < m_nCtrPts; i++ )
			m_pdCtrPts[i].Serialize( ar );
	}
}
void CBezier::Draw(CDC *pDc)
{	
	/*FILE* fp = fopen( cFilesList, "r" );
	if( !fp)
	{
		strInfo.AppendFormat( L"\tFile missing : %s\r\n", FILE_LIST );
		return bReturn;
	}*/
	bezier_b( m_nCtrPts, m_nSpanPts, m_pdCtrPts, m_pdCurvePts );

	DrawCurve( pDc, m_nSpanPts, m_pdCurvePts );

	DrawCtrlPolygon( pDc, m_nCtrPts, m_pdCtrPts );
	
}

void CBezier::DrawBezier(CDC *pDc, Matrix &matTransf)
{
	for(int i = 0; i < m_nSpanPts-1; i++)
	{
		CLine line(m_pdCurvePts[i], m_pdCurvePts[i+1]);
		line.DrawLine(pDc, matTransf);
	}

	for(int i = 0; i < m_nCtrPts-1; i++)
	{
		CLine line(m_pdCtrPts[i], m_pdCtrPts[i+1]);
		line.DrawLine(pDc, matTransf);
	}
}

void CBezier::DrawCtrlPolygon( CDC *pDc, int nCtrlPts, CGeomPoint *pdCtrPts )
{
	CPoint *ptCtrPlgn = new CPoint[nCtrlPts];

	CSize sizeWndExt = pDc->GetWindowExt();
	 
	for( int i = 0; i < nCtrlPts; i++ )
	{
		//ptCtrPlgn[i].x = ( long )pdCtrPts[i].x;
		//ptCtrPlgn[i].y = ( long )pdCtrPts[i].y;

		ptCtrPlgn[i].x = (LONG)pdCtrPts[i].m_Coord[0] * sizeWndExt.cx;
		ptCtrPlgn[i].y = (LONG)pdCtrPts[i].m_Coord[1] * sizeWndExt.cy;
	}

	//pDc->DPtoLP( ptCtrPlgn, nCtrlPts );
	pDc->MoveTo( ptCtrPlgn[0] );
	for( int i = 0; i < nCtrlPts; i++ )
		pDc->LineTo( ptCtrPlgn[i] );

	delete [] ptCtrPlgn;
}

void CBezier::DrawCurve( CDC *pDc, int nSpanPts, CGeomPoint *pdCurvePts )
{
	int LocIndex = 0;
	CPoint *Locations = new CPoint[nSpanPts]; //temp storage of pts

	CSize sizeWndExt = pDc->GetWindowExt();

	for(int i = 0; i < nSpanPts; i++)
	{
		Locations[i].x = (long)pdCurvePts[i].m_Coord[0] * sizeWndExt.cx;
		Locations[i].y = (long)pdCurvePts[i].m_Coord[1] * sizeWndExt.cy;
	}
	//pDc->DPtoLP( Locations, nSpanPts );
	pDc->MoveTo( Locations[0]);
	for(int i = 0; i < nSpanPts; i++)
		pDc->LineTo( Locations[i] );

	delete [] Locations;
}

double BasisFunc( int n, int i, double t)
{
	double dBasis = 0;
	dBasis = nCr( n, i )* npower( t, i) * npower( ( 1 - t ), ( n - i ) );
	return dBasis;
}

void bezier_b(int nCtrPts, int nSpanPts, /*CDoublePoint*/CGeomPoint *dCtrPts, /*CDoublePoint*/CGeomPoint *pdCurvePts )
{
	int iOrder = nCtrPts;
	int iDegree = iOrder - 1;
	
	Matrix mCtrPts( nCtrPts, THREEDIM );

	for( int i = 0; i < nCtrPts; i++ )
	{
		//mCtrPts( i, 0 ) = dCtrPts[i].x;
		//mCtrPts( i, 1 ) = dCtrPts[i].y;
		//mCtrPts( i, 2 ) = dCtrPts[i].z;
		for( int j = 0; j < THREEDIM; j++ )
			mCtrPts( i, j ) = dCtrPts[i].m_Coord[j];
	}
	

	double d = 1/( float )( nSpanPts - 1 );
	int nCount = 0;
	
	Matrix mBasis( 1, nCtrPts );
	for( double t = 0; t < 1; t += d )
	{		
		for( int i = 0; i < nCtrPts; i++ )
		{
			mBasis( 0, i ) = BasisFunc( iDegree, i, t ); 
		}

		Matrix dPoint = mBasis * mCtrPts; 

		//pdCurvePts[nCount].x = dPoint( 0, 0);
		//pdCurvePts[nCount].y = dPoint( 0, 1);
		//pdCurvePts[nCount].z = dPoint( 0, 2);

		for( int j = 0; j < THREEDIM; j++ )
			pdCurvePts[nCount].m_Coord[j] = dPoint( 0, j );
		
		nCount++;
	}
}

void bezier_m( int nCtrPts, int nSpanPts, /*CDoublePoint*/CGeomPoint *dCtrPts, /*CDoublePoint*/CGeomPoint *pdCurvePts )
{
	int iOrder = nCtrPts;
	int iDegree = iOrder - 1;

	Matrix mCtrPts( nCtrPts, THREEDIM ); // control point matrix Bi

	for( int i = 0; i < nCtrPts; i++ )
	{
	/*	mCtrPts( i, 0 ) = dCtrPts[i].x;
		mCtrPts( i, 1 ) = dCtrPts[i].y;
		mCtrPts( i, 2 ) = dCtrPts[i].z;*/
		for( int j = 0; j < THREEDIM; j++ )
			mCtrPts( i, j ) = dCtrPts[i].m_Coord[j];
	}

	Matrix N( nCtrPts, nCtrPts ); //basis function matrix Ni,j

	for( int i = 0; i < nCtrPts; i++ )
	{
		for( int j = 0; j < nCtrPts; j++ )
		{
			if( ( i + j ) >= 0 && ( i + j ) <= ( iDegree) )
				N( i ,j ) = pow(( -1.0 ), ( iDegree - i - j) ) * nCr( iDegree, j ) * nCr( ( iDegree - j ), ( iDegree - i - j )); 
			else
				N( i ,j ) = 0;
		}
	}

	double d = 1/( float )( nSpanPts - 1 );
	int nCount = 0;

	Matrix T( 1, nCtrPts ); // paramateric matrix [ t^n t^n-1 t^n-2 .....t 1]

	for( double t = 0; t < 1; t += d )
	{
		for( int i = 0; i < nCtrPts; i++ )
			T( 0, i ) = npower( t, ( iDegree - i ) );
		
		Matrix dPoint = T * N * mCtrPts; // [ t^n t^n-1 t^n-2 .....t 1] * Ni,j * Bi

		//dCurvePts[nCount].x = dPoint( 0, 0);
		//dCurvePts[nCount].y = dPoint( 0, 1);
		//dCurvePts[nCount].z = dPoint( 0, 2);

		for( int j = 0; j < THREEDIM; j++ )
			pdCurvePts[nCount].m_Coord[j] = dPoint( 0, j );

		nCount++;
	}

}
