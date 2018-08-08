#pragma once

#include "Options.h"

#include "afxwin.h"
// CBSpline 

class CBSpline : public CGeomEntity
{
	//DECLARE_DYNAMIC(CBSpline)
	DECLARE_SERIAL(CBSpline)
	
	CGeomPoint *m_pgptCtrl;
	CGeomPoint *m_pgptCurve;
	CGeomPoint *m_pgptData;
	double *m_pdKnotVec;
	double m_dParamRange[2];
	CGeomPoint m_gptCentre;
	

	//Settings
	bool m_bControlPolygon;
	bool m_bFirstDerivatives;
	
	eBSplineMethod m_eBSplineMethod;
	eBSplineFitMethod m_eBSplineFitMethod;
	
public:
	int	m_iCtrlPt;
	int m_iOrder;
	int m_iDegree;
	int m_iSpanPt;
	int m_iCurvePts;
	int m_iDataPt;
public:
	CBSpline( );   // standard constructor
	CBSpline( int nCtrlPt, int iOrder, int nSpanPt, double *pdParamRange, double *pdKnotVec, CGeomPoint *pdCtrlPt, COLORREF clrCurveColor );
	CBSpline( int nCtrlPt, int iOrder, int nSpanPt, int nDataPt,  eBSplineFitMethod methodBsplineFit, double *pdParamRange, double *pdKnotVec, CGeomPoint *pgptData, COLORREF clrCurveColor );
	void Draw();
	//void DrawBSpline( CDC *pDc, Matrix &matTransf );
	//void DrawCurve( CDC *pDc, int nSpanPt, CGeomPoint *pdCurvePts );
	//void DrawCtrlPolygon( CDC *pDc, int nCtrlPt, CGeomPoint *pdCtrlPt );
	virtual ~CBSpline();
public:
	void CalculateBSpline();
	void CalculateDataPoints();
	void CalculateControlPoints();
	void GetBSplineCtrlPt( CGeomPoint *pgptCtrlPt );
	void GetBoundingBox( CGeomPoint &gptMinBound, CGeomPoint &gptMaxBound );
public:
	virtual void Serialize(CArchive& ar);
	
	void SetControlPolygonStatus(bool bShow = true);
	bool GetControlPolygonStatus();

	void SetTangentVectorStatus(bool bShow = true);
	bool GetTangentVectorStatus();
	
};

void bspline_i( int nCtrlPt, int iOrder, int nSpanPt, double *pdKontVec, CGeomPoint *pdCtrlPt, CGeomPoint *pdCurvePt, int &iCurvePt );
void BSplineBlends( double dParam, int iCtrlPt, int iOrder, double *pdKontVec, double *pdBSplnBlends );
void BSplineBlends_1( double dParam, int iCtrlPt, int iOrder, double *pdKontVec, double *pdBSplnBlends );
void BSplineBlends_2( double dParam, int iCtrlPt, int iOrder, double *pdKontVec, double *pdBSplnBlends, int iDervs, double **pdBlendDervs );
void OpenUniformKnotVector( int nCtrlPt, int iOrder, double *pdKontVec, bool bNorm = false );
void PeriodicKnotVector( int nCtrlPt, int iOrder, double *pdKontVec, bool bNorm = false );
int IsPeriodic( int iCtrlPt, int iOrder, double *pdKnotVec );
void GetBSplineKnotPoints( int iCtrlPt, int iOrder, double  *pdKnotVec, CGeomPoint *pgptCtrlPt, CGeomPoint *pgptKnotPt );
void BSpline_btParam( int iCtrlPt, int iOrder, int iSpanPt, double *pdParamRange, double *pdKontVec, CGeomPoint *pgptCtrl, CGeomPoint *pgptCurve, int &iCurvePt );
void FindMinMax( double *pdArr, int iSize, double &dMaxValue, double &dMinValue );
void BSplineFit(int nCtrlPt, int iOrder, double *pdParamRange, double *pdKontVec,int nDataPt, eBSplineFitMethod methodBsplineFit, CGeomPoint *pgptData, CGeomPoint *pgptCtrl);
void BSplineFitEndTangents(int nCtrlPt, int iOrder, double *pdParamRange, double *pdKontVec,int nDataPt, Vector *pFirstDerivatives, eBSplineFitMethod methodBsplineFit, CGeomPoint *pgptData, CGeomPoint *pgptCtrl);
void BSplineFirstDervativeBlends(double dParam, int iCtrlPt, int iOrder, double *pdKontVec, double *pdDerivativeBlends);
void BSplineFirstDervativeAtParam(double dParam, int iCtrlPt, int iOrder, double *pdKontVec, CGeomPoint *pdCtrlPt, double *pdVector);
void BSplineSecondDervativeBlends(double dParam, int iCtrlPt, int iOrder, double *pdKontVec, double *pdDerivativeBlends);
void BSplineSecondDervativeAtParam(double dParam, int iCtrlPt, int iOrder, double *pdKontVec, CGeomPoint *pdCtrlPt, double *pdVector);
void ParameterAssignment(int nCtrlPt, int iOrder, int nDataPt, eBSplineFitMethod methodBsplineFit, CGeomPoint *pgptData, double *pParameters);
void GetBSplineDataPoints( int iCtrlPt, int iOrder, double *pdParamRange, double  *pdKnotVec, eBSplineFitMethod methodBsplineFit, CGeomPoint *pgptCtrlPt, CGeomPoint *pgptDataPt);


