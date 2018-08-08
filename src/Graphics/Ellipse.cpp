// Ellipse.cpp : implementation file
//

#include "stdafx.h"
#include <iostream>
#include "GLSampleDoc.h"
#include "Ellipse.h"
#include "Line.h"

#include <math.h>

#include "CMath.h"

using namespace std;

// CEllipse
IMPLEMENT_SERIAL(CEllipse, CGeomEntity, 1)

CEllipse::CEllipse()
{
}

CEllipse::CEllipse( CGeomPoint ptCenter, double dMaxR, double dMinR, int nSpanPts )
{
	m_nSpanPts = nSpanPts;
	m_ptCenter = ptCenter;
	m_dMaxRad = dMaxR;
	m_dMinRad = dMinR;
	try {
		m_pElpsePts = new CGeomPoint[nSpanPts];
	}
	
	catch (bad_alloc xa)
	{
		cout << "Allocation Failure\n";
		exit(EXIT_FAILURE);
	}
	CreateEllipse( m_nSpanPts, m_dMaxRad, m_dMinRad, m_ptCenter, m_pElpsePts );
}

CEllipse::~CEllipse()
{
	delete []m_pElpsePts;
}


// CEllipse member functions
void CEllipse::Serialize(CArchive &ar)
{
	CGeomEntity::Serialize(ar);

	m_ptCenter.Serialize( ar );

	if(ar.IsStoring())
		ar << m_dMaxRad << m_dMinRad;
	else 
		ar >> m_dMaxRad >> m_dMinRad;
}

void CEllipse::Draw(CDC *pDc)
{
	//int n_pt = MAXLOC;
	//double start_ang = 0;
	//double dAng = MAXANG/(float)n_pt;

	//dAng = dAng * PI/180;

	//double x_start,y_start,x_end,y_end;

	//x_start = m_dMaxRad * cos(start_ang);
	//y_start = m_dMinRad * sin(start_ang);

	//int LocIndex = 0;
	//CPoint Locations[MAXLOC];
	//
	//Locations[0].x =  x_start;
	//Locations[0].y =  y_start;
	//
	//LocIndex = LocIndex + 1;
	//
	//for(int i = 1; i < n_pt; i++)
	//{
	//	x_end = ( x_start * cos(dAng)) - ((m_dMaxRad/m_dMinRad)*y_start * sin(dAng));
	//	y_end = ((m_dMinRad/m_dMaxRad)* x_start * sin(dAng)) + (y_start * cos(dAng));

	//	//m_Locations[i] = ptEnd;

	//	Locations[i].x = x_end;
	//	Locations[i].y = y_end;
	//	
	//	x_start = x_end;
	//	y_start = y_end;

	//	LocIndex++;
	//}	

	//CPoint ptCenter = CGeomPoint::GeomPtToCpt( &m_ptCenter );

	//for(int i = 0; i < LocIndex; i++)
	//	Locations[i] += ptCenter;

	//pDc->MoveTo( Locations[0] );

	//for(int i = 0; i < LocIndex; i++)
	//	pDc->LineTo( Locations[i] );	

	//pDc->LineTo( Locations[0] );	

	CreateEllipse( m_nSpanPts, m_dMaxRad, m_dMinRad, m_ptCenter, m_pElpsePts );

	DrawCurve( pDc, m_nSpanPts, m_pElpsePts );
}

void CEllipse::DrawEllipse(CDC *pDc, Matrix &matTransf)
{
	for(int i = 0; i < m_nSpanPts-1; i++)
	{
		CLine line(m_pElpsePts[i], m_pElpsePts[i+1]);
		line.DrawLine(pDc, matTransf);
	}
}

void CEllipse::DrawCurve( CDC *pDc, int nSpanPts, CGeomPoint *pElpsePts )
{
	int LocIndex = 0;
	CPoint *Locations = new CPoint[nSpanPts]; //temp storage of pts

	CSize sizeWndExt = pDc->GetWindowExt();

	for(int i = 0; i < nSpanPts; i++)
	{
		Locations[i].x = (LONG) pElpsePts[i].m_Coord[0] * sizeWndExt.cx;
		Locations[i].y = (LONG) pElpsePts[i].m_Coord[1] * sizeWndExt.cy;
	}

	pDc->MoveTo( Locations[0]);
	for(int i = 0; i < nSpanPts; i++)
		pDc->LineTo( Locations[i] );

	pDc->LineTo( Locations[0] ); // For closing the ellipse.

	delete [] Locations;
}


void CreateEllipse( int nSpanPts, double dMaxRad, double dMinRad, CGeomPoint ptCenter, CGeomPoint* pElpsePts )
{
	double start_ang = 0;
	double dtheta  = MAXANG/(float)nSpanPts;
	double d_ang = dtheta * PI/180;

	CGeomPoint gPtStart, gPtEnd;
	
	start_ang = (start_ang)* PI/180;

	gPtStart.m_Coord[0] = dMaxRad * cos(start_ang);
	gPtStart.m_Coord[1] = dMinRad * sin(start_ang);

	pElpsePts[0] = gPtStart;
	
	for(int i = 1; i < nSpanPts; i++)
	{
		gPtEnd.m_Coord[0] =  gPtStart.m_Coord[0] * cos(d_ang) - ((dMaxRad/dMinRad)* gPtStart.m_Coord[1] * sin(d_ang));
		gPtEnd.m_Coord[1] =  ((dMinRad/dMaxRad)*gPtStart.m_Coord[0] * sin(d_ang)) + gPtStart.m_Coord[1] * cos(d_ang);

		pElpsePts[i] = gPtEnd;

		gPtStart = gPtEnd;
	}

	for(int i = 0; i < nSpanPts; i++)
		for( int j = 0; j < 2; j++ )
			pElpsePts[i].m_Coord[j] =  pElpsePts[i].m_Coord[j] + ptCenter.m_Coord[j];
}