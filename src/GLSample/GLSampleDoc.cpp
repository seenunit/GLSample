// GLSampleDoc.cpp : implementation of the CGLSampleDoc class
//

#include "stdafx.h"

#include "BSplineDlg.h"
#include "FemBeamDlg.h"
#include "GLSampleDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLSampleDoc

IMPLEMENT_DYNCREATE(CGLSampleDoc, CDocument)

BEGIN_MESSAGE_MAP(CGLSampleDoc, CDocument)
	ON_COMMAND(ID_GEOMETRY_LINE, &CGLSampleDoc::OnGeometryLine)
	ON_COMMAND(ID_BSPLINE_CURVEFAIR, &CGLSampleDoc::OnBsplineCurvefair)
	ON_COMMAND(ID_BSPLINE_CURVEFIT, &CGLSampleDoc::OnBsplineCurvefit)
	ON_COMMAND(ID_GEOMETRY_BSURFACE, &CGLSampleDoc::OnGeometryBSurface)
	ON_COMMAND(ID_FEM_BEAM, &CGLSampleDoc::OnFemBeam)
END_MESSAGE_MAP()


// CGLSampleDoc construction/destruction

CGLSampleDoc::CGLSampleDoc()
{
	// TODO: add one-time construction code here
	objLineArray.SetSize( 0, 100 );
	m_dScaleFactor = 1.0;
	m_dSphereRad = 1.0;
	m_bLine = false;

	for(int i = 0; i < 3; i++)
		m_ViewCenter[i] = 0.0;
}

CGLSampleDoc::~CGLSampleDoc()
{
}

BOOL CGLSampleDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)


	return TRUE;
}




// CGLSampleDoc serialization

void CGLSampleDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	objLineArray.Serialize( ar );
}


// CGLSampleDoc diagnostics

#ifdef _DEBUG
void CGLSampleDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGLSampleDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGLSampleDoc commands

CGeomEntity *CGLSampleDoc::GetEntityFromID(int ID)
{
	CGeomEntity *pEntity = NULL;
	pEntity = GetLineFromID(ID);
	if(pEntity == NULL)
		pEntity = GetBSplineFromID(ID);
	if(pEntity == NULL)
		pEntity = GetBSurfFromID(ID);
	if(pEntity == NULL)
		pEntity = GetFemBeamID(ID);

	return pEntity;
}

//CSampleDocDoc Line Implementation
CLine *CGLSampleDoc::GetLine(int i)
{
	return (CLine *) objLineArray[i];
}

CLine *CGLSampleDoc::GetLineFromID(int ID)
{
	int count = GetLineCount();
	if( count ) 
	{
		for(int i = 0; i < count; i++)
		{
			CLine* pLine = (CLine*)objLineArray[i];
			if(pLine->GetID() == ID)
			{
				return pLine;
			}
		}
	}
	return NULL;
}
CLine *CGLSampleDoc::CreateLine(CGeomPoint gptFrom, CGeomPoint gptTo)
{
	//CGeomPoint gptFrom = CGeomPoint::CptToGeomPt( ptFrom );
	//CGeomPoint gptTo = CGeomPoint::CptToGeomPt( ptTo );
	CLine *pLine = new CLine( gptFrom, gptTo );
	pLine->Draw();
	return pLine;
}
int CGLSampleDoc::GetLineCount()
{
	return ( int )objLineArray.GetSize();
}
CLine *CGLSampleDoc::AddLine(CGeomPoint gptFrom, CGeomPoint gptTo)
{
	CLine *pLine = new CLine( gptFrom, gptTo);

	//CGeomPoint gptFrom = CGeomPoint::CptToGeomPt( ptFrom );
	//CGeomPoint gptTo = CGeomPoint::CptToGeomPt( ptTo );

	objLineArray.Add(pLine);
	SetModifiedFlag();
	UpdateAllViews( NULL );
	return pLine;
}
void CGLSampleDoc::DeleteLineCounts()
{
	int count = ( int )objLineArray.GetSize();

	if(count)
	{
		for(int i = 0; i < (int)count; i++)
			delete objLineArray[i];

		objLineArray.RemoveAll();
	}
}

//CBSpline Implementation
CBSpline *CGLSampleDoc::GetBSpline( int i )
{
	if( i < objBSplineArray.GetCount() && i >= 0)
		return ( CBSpline * ) objBSplineArray[i];
	else
		return NULL;
}
CBSpline *CGLSampleDoc::GetBSplineFromID(int ID)
{
	int count = GetBSplineCount();
	if( count ) 
	{
		for(int i = 0; i < count; i++)
		{
			CBSpline* pBspline = (CBSpline*)objBSplineArray[i];
			if(pBspline->GetID() == ID)
			{
				return pBspline;
			}
		}
	}
	return NULL;
}
int CGLSampleDoc::GetBSplineCount()
{
	return ( int )objBSplineArray.GetSize();
}
CBSpline *CGLSampleDoc::CreateBSpline( int nCtrlPt, int iOrder, int nSpanPt,  CPoint *pCtrlPt )
{
	CGeomPoint *pdCtrlPt = new CGeomPoint[nCtrlPt];
	for( int i = 0; i < nCtrlPt; i++ )
		pdCtrlPt[i] = CGeomPoint::CptToGeomPt( pCtrlPt[i] );

	double *pKnotVec = new double[nCtrlPt+iOrder];
	OpenUniformKnotVector( nCtrlPt, iOrder, pKnotVec, false );

	CBSpline *pBSpline = new CBSpline( nCtrlPt, iOrder, nSpanPt, NULL, pKnotVec, pdCtrlPt, RGB(255,255,255) );
	pBSpline->Draw();
	return pBSpline;
}
CBSpline *CGLSampleDoc::AddBSpline( int nCtrlPt, int iOrder, int nSpanPt, double *pdParamRange, double *pdKnotVec, CGeomPoint *pdCtrlPt, COLORREF clrCurveColor )
{
	CBSpline *pBSpline = new CBSpline( nCtrlPt, iOrder, nSpanPt, pdParamRange, pdKnotVec, pdCtrlPt, clrCurveColor );

	objBSplineArray.Add( pBSpline );
	SetModifiedFlag();
	UpdateAllViews( NULL );
	return pBSpline;
}
CBSpline *CGLSampleDoc::AddBSpline( int nCtrlPt, int iOrder, int nSpanPt, int nDataPt, eBSplineFitMethod methodBsplineFit, double *pdParamRange, double *pdKnotVec, CGeomPoint *pgptData, COLORREF clrCurveColor )
{
	CBSpline *pBSpline = new CBSpline( nCtrlPt, iOrder, nSpanPt, nDataPt, methodBsplineFit, pdParamRange, pdKnotVec, pgptData, clrCurveColor );

	objBSplineArray.Add( pBSpline );
	SetModifiedFlag();
	UpdateAllViews( NULL );
	return pBSpline;
}
void CGLSampleDoc::DeleteBSplineCounts()
{
	int count = GetBSplineCount();
	if( count ) 
	{
		for( int i = 0; i < count; i++ )
			delete objBSplineArray[i];
		objBSplineArray.RemoveAll();
	}
}

void CGLSampleDoc::DeleteBspline(int ID)
{
	int count = GetBSplineCount();
	if( count ) 
	{
		for(int i = 0; i < count; i++)
		{
			CBSpline* pBspline = (CBSpline*)objBSplineArray[i];

			if(pBspline->GetID() == ID)
			{
				delete objBSplineArray[i];
				objBSplineArray.RemoveAt(i);
				return;
			}
		}
	}
}

bool CGLSampleDoc::ValidateBSplineID(int ID)
{
	int count = GetBSplineCount();
	if( count ) 
	{
		for(int i = 0; i < count; i++)
		{
			CBSpline* pBspline = (CBSpline*)objBSplineArray[i];

			if(pBspline->GetID() == ID)
			{
				return true;
			}
		}
	}
	return false;
}

void CGLSampleDoc::OnGeometryLine()
{
	m_bLine = true;
}

void CGLSampleDoc::OnBsplineCurvefair()
{
    if(m_bLine)
        m_bLine = false;

    static CBSplineDlg cBsplineDlg( this, BSPLINE_FAIR );
	cBsplineDlg.Create( IDD_BSPLINE, NULL );
	
	cBsplineDlg.ShowWindow( SW_SHOW );
}

void CGLSampleDoc::OnBsplineCurvefit()
{
    if(m_bLine)
        m_bLine = false;

	static CBSplineDlg cBsplineDlg( this, BSPLINE_FIT );
	cBsplineDlg.Create( IDD_BSPLINE, NULL );
	cBsplineDlg.ShowWindow( SW_SHOW );
}

void CGLSampleDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	DeleteLineCounts();
	DeleteBSplineCounts();

	CDocument::DeleteContents();
}

void CGLSampleDoc::OnGeometryBSurface()
{
    if(m_bLine)
        m_bLine = false;

	static CBSplineDlg cBsplineDlg( this, BSPLINE_SWEEP );
	cBsplineDlg.Create( IDD_BSPLINE, NULL );
	
	cBsplineDlg.ShowWindow( SW_SHOW );
}

//BSurface operations Implementation
CBSurf *CGLSampleDoc::GetBSurf(int i)
{
	if( i < objBsurfArray.GetCount() && i >= 0)
		return ( CBSurf * ) objBsurfArray[i];
	else
		return NULL;
}
CBSurf *CGLSampleDoc::GetBSurfFromID(int ID)
{
	int count = GetBSurfCount();
	if( count ) 
	{
		for(int i = 0; i < count; i++)
		{
			CBSurf* pBSurf = (CBSurf*)objBsurfArray[i];
			if(pBSurf->GetID() == ID)
			{
				return pBSurf;
			}
		}
	}
	return NULL;
}
int CGLSampleDoc::GetBSurfCount()
{
	return ( int )objBsurfArray.GetSize();
}
CBSurf *CGLSampleDoc::AddBSurf(CBSpline *pUBSpline, CBSpline *pVBSpline)
{
	CBSurf *pBSurf = new CBSurf(pUBSpline, pVBSpline);

	objBsurfArray.Add(pBSurf);
	SetModifiedFlag();
	UpdateAllViews( NULL );
	return pBSurf;
}

CBSurf *CGLSampleDoc::AddBSurf(CGeomPoint *pGeomPt, int iUPts, int iVPts, int iUSpan, int iVSpan)
{
	CBSurf *pBSurf = new CBSurf(pGeomPt, iUPts, iVPts, iUSpan, iVSpan);

	objBsurfArray.Add(pBSurf);
	SetModifiedFlag();
	UpdateAllViews( NULL );
	return pBSurf;
}
void CGLSampleDoc::DleteBSurf(int ID)
{
	int count = GetBSurfCount();
	if( count ) 
	{
		for(int i = 0; i < count; i++)
		{
			CBSurf* pBSurf = (CBSurf*)objBsurfArray[i];

			if(pBSurf->GetID() == ID)
			{
				delete objBsurfArray[i];
				objBsurfArray.RemoveAt(i);
				return;
			}
		}
	}
}

bool CGLSampleDoc::ValidateBSurfID(int ID)
{
	int count = GetBSurfCount();
	if( count ) 
	{
		for(int i = 0; i < count; i++)
		{
			CBSurf* pBSurf = (CBSurf*)objBsurfArray[i];

			if(pBSurf->GetID() == ID)
			{
				return true;
			}
		}
	}
	return false;
}


void CGLSampleDoc::OnFemBeam()
{
    if(m_bLine)
        m_bLine = false;

    static CFemBeamDlg cFemBeamDlg( this );
	cFemBeamDlg.Create( IDD_FEM_BEAM, NULL );
	
	cFemBeamDlg.ShowWindow( SW_SHOW );
}

CFemBeam *CGLSampleDoc::GetFemBeam(int i)
{
    if( i < objFemBeamArray.GetCount() && i >= 0)
        return ( CFemBeam * ) objFemBeamArray[i];
    else
        return NULL;
}
CFemBeam *CGLSampleDoc::GetFemBeamID(int ID)
{
	int count = GetBeamCount();
    if( count ) 
    {
        for(int i = 0; i < count; i++)
        {
            CFemBeam* pFemBeam = (CFemBeam*)objFemBeamArray[i];
            if(pFemBeam->GetID() == ID)
            {
                return pFemBeam;
            }
        }
    }
    return NULL;
}
int CGLSampleDoc::GetBeamCount()
{
    return ( int )objFemBeamArray.GetSize();
}
CFemBeam *CGLSampleDoc::AddBeam(double dBeamLength, int iBeamNodes, double dYModuli, double dSectionArea, CGeomPoint *pgptEndPoints)
{
    CFemBeam *pFemBeam = new CFemBeam( dBeamLength, iBeamNodes, dYModuli, dSectionArea, pgptEndPoints );

    objFemBeamArray.Add( pFemBeam );
    SetModifiedFlag();
    UpdateAllViews( NULL );
    return pFemBeam;
}
void CGLSampleDoc::deleteFemBeam(int ID)
{
    int count = GetBSurfCount();
    if( count ) 
    {
        for(int i = 0; i < count; i++)
        {
            CFemBeam* pBeam = (CFemBeam*)objFemBeamArray[i];

            if(pBeam->GetID() == ID)
            {
                delete objFemBeamArray[i];
                objFemBeamArray.RemoveAt(i);
                return;
            }
        }
    }
}
bool CGLSampleDoc::ValidateFemBeamID(int ID)
{
    int count = GetBSurfCount();
    if( count ) 
    {
        for(int i = 0; i < count; i++)
        {
            CFemBeam* pBeam = (CFemBeam*)objFemBeamArray[i];

            if(pBeam->GetID() == ID)
            {
                return true;
            }
        }
    }
    return false;
}

