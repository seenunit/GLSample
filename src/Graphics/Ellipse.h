#pragma once

#include "GeomEntity.h"
#include "Matrix.h"

// CEllipse command target

class CEllipse : public CGeomEntity
{
	DECLARE_SERIAL(CEllipse)

	int m_nSpanPts;
	double m_dMaxRad;
	double m_dMinRad;

	CGeomPoint m_ptCenter;
	CGeomPoint *m_pElpsePts;
public:
	CEllipse();
	CEllipse( CGeomPoint ptCenter, double dMaxR, double dMinR, int nSpanPts );
	virtual ~CEllipse();
public:
	void Serialize(CArchive &ar);
	void Draw(CDC *pDc);
	void DrawEllipse(CDC *pDc, Matrix &matTransf);
	void DrawCurve( CDC *pDc, int nSpanPts, CGeomPoint *pElpsePts );
};

void CreateEllipse( int nSpanPts, double dMaxRad, double dMinRad, CGeomPoint ptCenter, CGeomPoint* pElpsePts );