#pragma once

#include "BSpline.h"
#include "GeomEntity.h"
#include "Matrix.h"
// CBezier dialog

class CBezier : public CGeomEntity
{
	DECLARE_SERIAL(CBezier)
	
	int m_nCtrPts;
	int m_nDegree;
	int m_nOrder;
	int m_nSpanPts;
	CGeomPoint *m_pdCtrPts;
	CGeomPoint *m_pdCurvePts;

public:
	CBezier(/*CWnd* pParent = NULL*/);   // standard constructor
	CBezier( int nCtrPts, int nSpanPts,  CGeomPoint *dCtrPts );
	void Serialize(CArchive &ar);
	void Draw(CDC *pDc);
	void DrawBezier(CDC *pDc, Matrix &matTransf);
	void DrawCurve( CDC *pDc, int nSpanPts, CGeomPoint *pdCurvePts );
	void DrawCtrlPolygon( CDC *pDc, int nCtrlPts, CGeomPoint *pdCtrPts );
	virtual ~CBezier();
	
//// Dialog Data
//	enum { IDD = IDD_BEZIER };

//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

//	DECLARE_MESSAGE_MAP()
};

double BasisFunc( int nDegree, int ithPt, double nParam );
void bezier_b( int nCtrPts, int nSpanPts, CGeomPoint *pdCtrPts, CGeomPoint *pdCurvePts );
void bezier_m( int nCtrPts, int nSpanPts, CGeomPoint *pdCtrPts, CGeomPoint *pdCurvePts );