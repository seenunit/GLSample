// GeomEntity.cpp : implementation file
//

#include "stdafx.h"

CGLSampleDoc *g_pDoc;

// CGeomEntity

CGeomEntity::CGeomEntity( )
{
	m_bSelected = false;
}

CGeomEntity::~CGeomEntity( )
{
}

 //CGeomEntity member functions

void CGeomEntity::Serialize( CArchive& ar )
{
	if ( ar.IsStoring( ) )
	{	// storing code
	}
	else
	{	// loading code
	}
}

unsigned int CGeomEntity::GetID()
{
	return m_ID;
}

void CGeomEntity::Draw()
{
    // nothing to draw
}

void CGeomEntity::SetSelectionStatus(bool bSelected)
{
	m_bSelected = bSelected;
}

bool CGeomEntity::GetSelectionStatus()
{
	return m_bSelected;
}

void CGeomEntity::SetHiddenStatus(bool bHidden)
{
	m_bHidden = bHidden;
}

bool CGeomEntity::GetHiddenStatus()
{
	return m_bHidden;
}

COLORREF CGeomEntity::SetEntityColor(COLORREF setColor)
{
	COLORREF prevColor = m_clrCurveColor;
	m_clrCurveColor = setColor;
	return prevColor;
}

 //CGeomPoint

IMPLEMENT_SERIAL( CGeomPoint, CGeomEntity, 1 )

CGeomPoint::CGeomPoint( ) : CGeomEntity( TL_POINT )
{
	m_Dim = 4;
	for( int i = 0; i < m_Dim; i++ )
		m_Coord[i] = 0;
	m_ID = (unsigned int)this;
}

CGeomPoint::CGeomPoint( int iDim, double Coord[] ) : CGeomEntity( TL_POINT )
{
	m_Dim = iDim;

	for( int i = 0; i < 4; i++ )
		m_Coord[i] = 0;
	
	for( int i = 0; i < iDim; i++ )
		m_Coord[i] = Coord[i];
	m_ID = (unsigned int)this;
}

CGeomPoint::CGeomPoint( double x, double y, double z ) : CGeomEntity( TL_POINT )
{
	m_Coord[0] = x;
	m_Coord[1] = y;
	m_Coord[2] = z;
	m_Coord[3] = 1;
	m_ID = (unsigned int)this;
}

CGeomPoint::~CGeomPoint( )
{	
}

// CGeomEntity member functions

void CGeomPoint::Draw()
{
	glBegin(GL_POINTS); 
		glVertex3d(m_Coord[0], m_Coord[1], m_Coord[2]);
	glEnd( );
}

void CGeomPoint::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
		ar << m_Dim;
		for( int i = 0; i < m_Dim; i++ )
			ar << m_Coord[i];
	}
	else
	{	// loading code
		ar >> m_Dim;
		for( int i = 0; i < m_Dim; i++ )
			ar >> m_Coord[i];
	}
}

CGeomPoint CGeomPoint::CptToGeomPt( CPoint pt )
{
	double Coord[2] = { pt.x, pt.y };
	return CGeomPoint( 2, Coord );
}

CPoint CGeomPoint::GeomPtToCpt( CGeomPoint* pPt )
{
	return CPoint( ( LONG )pPt->m_Coord[0], ( LONG )pPt->m_Coord[1] );
}

// CDoublePoint

IMPLEMENT_SERIAL( CDoublePoint, CGeomEntity, 1 )

CDoublePoint::CDoublePoint():CGeomEntity(TL_POINT), x(0),y(0),z(0)
{
	
}

CDoublePoint::~CDoublePoint()
{

}

// CDoublePoint member functions

void CDoublePoint::Serialize( CArchive &ar )
{
	CGeomEntity::Serialize( ar );

	if(ar.IsStoring())
	{
		ar << this->x << this->y << this->z;
	}
	else
	{
		ar >> this->x >> this->y >> this->z;
	}
}

void TransFormEntity( CGeomPoint *pgptEntity, int iCount, Matrix &matTransf, CGeomPoint *pgptTransfEntity  )
{
	Matrix matCurvePt( iCount, 4 );

	for( int i = 0; i < iCount; i++ )
	{
		for( int j = 0; j < 3; j++ )
			matCurvePt( i, j ) = pgptEntity[i].m_Coord[j];
		matCurvePt( i , 3 ) = 1;
	}

	//Matrix matTransPt( iCount, 4 );
	Matrix matTransPt = matCurvePt * matTransf;  // transform the circle..

	for( int i = 0; i < iCount; i++ )
		for( int j = 0; j < THREEDIM; j++ )
			pgptTransfEntity[i].m_Coord[j] = matTransPt( i, j );
}

void TransfToWorld( double *pdVector, Matrix &matTransf )
{
	Matrix matVect( pdVector, 1, 4 );

	Matrix matTransfVec( 1, 4 );
	matTransfVec = matVect * matTransf;

	for( int i = 0; i < 3; i++ )
		pdVector[i] = matTransfVec( 0, i );
}

void DrawPoints( int nPt, double dSize, CGeomPoint *pgptKnotPt )
{
	dSize = dSize * (g_pDoc->m_dSphereRad/10) * g_pDoc->m_dScaleFactor;

	for(int i = 0; i < nPt; i++)
	{
		glPushName(pgptKnotPt[i].GetID());
		glBegin(GL_POLYGON); 
			glVertex3d(pgptKnotPt[i].m_Coord[0]-dSize, pgptKnotPt[i].m_Coord[1]+dSize, pgptKnotPt[i].m_Coord[2]);
			glVertex3d(pgptKnotPt[i].m_Coord[0]+dSize, pgptKnotPt[i].m_Coord[1]+dSize, pgptKnotPt[i].m_Coord[2]); 
			glVertex3d(pgptKnotPt[i].m_Coord[0]+dSize, pgptKnotPt[i].m_Coord[1]-dSize, pgptKnotPt[i].m_Coord[2]);
			glVertex3d(pgptKnotPt[i].m_Coord[0]-dSize, pgptKnotPt[i].m_Coord[1]-dSize, pgptKnotPt[i].m_Coord[2]); 
		glEnd( );
		glPopName();
	}
}
