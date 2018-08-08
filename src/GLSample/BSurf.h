#pragma once
#include "GeomEntity.h"

#include "afxwin.h"
#include <vector>

using namespace std;
class CBSurf : public CGeomEntity
{
	int m_iVPts;
	int m_iUPts;
	int m_iUSpan;
	int m_iVSpan;
	CGeomPoint* m_pSurfPts;
	CGeomPoint* m_pSurfNormls;
	CBSpline *m_pUBSpline;
	CBSpline *m_pVBSpline;
public:
	CGeomPoint *m_pGeomPts;

public:
	CBSurf(void);
	CBSurf(CBSpline *pUBSpline, CBSpline *pVBSpline);
	CBSurf(CGeomPoint *pGeomPt, int iUPts, int iVPts, int iUSpan, int iVSpan);
	~CBSurf(void);

	void Draw();
	void CalculateBSurface();
	int GetRowNumber();
	int GetColumnNumber();
};