// GLGLSampleDoc.h : interface of the CGLSampleDoc class
//

#pragma once

#include "Line.h"
#include "BSpline.h"
#include "BSurf.h"
#include "FemBeam.h"

class CGLSampleDoc : public CDocument
{
protected: // create from serialization only
	CGLSampleDoc();
	DECLARE_DYNCREATE(CGLSampleDoc)

// Attributes
public:
	CObArray objLineArray;
	CObArray objBSplineArray;
	CObArray objBsurfArray;
    CObArray objFemBeamArray;

//Volume Attributes
	double m_dSphereRad; //Volume Radius
	double m_dScaleFactor;

	GLdouble m_ViewCenter[3];

//Flags
	bool m_bLine;
	
// Operations
public:

	CGeomEntity *GetEntityFromID(int ID);

//Line operations
	CLine *GetLine(int i);
	CLine *GetLineFromID(int ID);
	CLine *CreateLine(CGeomPoint ptFrom, CGeomPoint ptTo);
	int GetLineCount();
	CLine *AddLine(CGeomPoint ptFrom, CGeomPoint ptTo);
	void DeleteLineCounts();

//BSpline operations
	CBSpline *GetBSpline( int i );
	CBSpline *GetBSplineFromID(int ID);
	int GetBSplineCount();
	CBSpline *CreateBSpline( int nCtrlPt, int iOrder, int nSpanPt,  CPoint *pCtrlPt );
	CBSpline *AddBSpline( int nCtrlPt, int iOrder, int nSpanPt, double *pdParamRange, double *pdKnotVec, CGeomPoint *pdCtrlPt, COLORREF clrCurveColor );
	CBSpline *AddBSpline( int nCtrlPt, int iOrder, int nSpanPt, int nDataPt, eBSplineFitMethod methodBsplineFit, double *pdParamRange, double *pdKnotVec, CGeomPoint *pgptData, COLORREF clrCurveColor );
	void DeleteBSplineCounts();
	void DeleteBspline(int ID);
	bool ValidateBSplineID(int ID);

//BSurface operations
	CBSurf *GetBSurf(int i);
	CBSurf *GetBSurfFromID(int ID);
	int GetBSurfCount();
	CBSurf *AddBSurf(CBSpline *pUBSpline, CBSpline *pVBSpline);
	CBSurf *AddBSurf(CGeomPoint *pGeomPt, int iRow, int iColumn, int iUSpan, int iVSpan);
	void DleteBSurf(int ID);
	bool ValidateBSurfID(int ID);

//Fem Beam Operations
    //Add one Beam at a time.
    CFemBeam *GetFemBeam(int i);
    CFemBeam *GetFemBeamID(int ID);
    int GetBeamCount();
    CFemBeam *AddBeam(double dBeamLength, int iBeamNodes, double dYModuli, double dSectionArea, CGeomPoint *pgptEndPoints);
    void deleteFemBeam(int ID);
    bool ValidateFemBeamID(int ID);
	
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CGLSampleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGeometryLine();
	afx_msg void OnBsplineCurvefair();
	afx_msg void OnBsplineCurvefit();
	afx_msg void OnGeometryBSurface();
	virtual void DeleteContents();
	afx_msg void OnFemBeam();
};


