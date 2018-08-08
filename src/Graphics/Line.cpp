// Line.cpp : implementation file
//

#include "stdafx.h"
#include "Line.h"


// CLine
IMPLEMENT_SERIAL( CLine, CGeomEntity, 1 )

CLine::CLine() : CGeomEntity( TL_LINE )
{
	m_ID = (unsigned int)this;
}

CLine::CLine( CGeomPoint/*CPoint*/ dptFrom, CGeomPoint/*CPoint*/ dptTo ) : CGeomEntity( TL_LINE )
{
	m_dptStart = dptFrom;
	m_dptEnd = dptTo;
	m_ID = (unsigned int)this;
}

CLine::~CLine()
{
	m_ID = (unsigned int)this;
}

// CLine member functions
void CLine::Serialize(CArchive &ar)
{
	CObject::Serialize( ar );

	m_dptStart.Serialize( ar );
	m_dptEnd.Serialize( ar );

	if(ar.IsStoring())
	{ // storing code	
		//ar << m_dptStart << m_dptEnd;
	}
	else
	{ // loading code
		//ar >> m_dptStart << m_dptEnd;
	}	
}

void CLine::Draw()
{
	glBegin(GL_LINES);
	glVertex3d(m_dptStart.m_Coord[0], m_dptStart.m_Coord[1], m_dptStart.m_Coord[2]);
	glVertex3d(m_dptEnd.m_Coord[0], m_dptEnd.m_Coord[1], m_dptEnd.m_Coord[2]);
	glEnd();
}

void CLine::DrawLine(CDC *pDC, Matrix &matTransf)
{
	Matrix matLinePt( 2, 4 );
	
	for( int j = 0; j < 3; j++ )
		matLinePt( 0, j ) = m_dptStart.m_Coord[j];
	matLinePt( 0, 3 ) = 1;
	
	for( int j = 0; j < 3; j++ )
		matLinePt( 1, j ) = m_dptEnd.m_Coord[j];
	matLinePt( 1, 3 ) = 1;

	Matrix matTransPt( 2, 4 ); 
	matTransPt = matLinePt * matTransf;  // transform the Line..

	CSize sizeWndExt = pDC->GetWindowExt();

	CPoint ptStart, ptEnd;
	ptStart.x = matTransPt( 0, 0 ) * sizeWndExt.cx;
	ptStart.y = matTransPt( 0, 1 ) * sizeWndExt.cy;

	ptEnd.x = matTransPt( 1, 0 ) * sizeWndExt.cx;
	ptEnd.y = matTransPt( 1, 1 ) * sizeWndExt.cy;

	pDC->MoveTo( ptStart );
	pDC->LineTo( ptEnd );
}

void CLine::ModifyLine( CGeomPoint ptFrom, CGeomPoint ptTo )
{
	m_dptStart = ptFrom;
	m_dptEnd = ptTo;
}

CGeomPoint CLine::GetStartPoint()
{
	return m_dptStart;
}
CGeomPoint CLine::GetEndPoint()
{
	return m_dptEnd;
}

