#pragma once

#include "GeomEntity.h"
#include "Vector.h"
#include "Matrix.h"
#include "Options.h"
// CCircle command target

class CCircle : public CGeomEntity
{
	DECLARE_SERIAL( CCircle )
private:
	int m_nSpanPts;
	double m_dRadii;
	CGeomPoint m_Center;
	CGeomPoint *m_pCirPts;
	Vector m_vecNormal;

public:
	CCircle();
	CCircle( CGeomPoint ptCirCenter, double dCirRadii, int nSpanPts, Vector vecNormal );
	void Serialize( CArchive &ar );
	void Draw( CDC *pDc );
	void DrawCircle( CDC *pDc, Matrix &matrixTransf );
	void DrawCurve( CDC *pDc, int nSpanPts, CGeomPoint *pCirPts, Matrix &matTransf );
	virtual ~CCircle();
};

void CreateArc_i( int nSpanpts, double dRadii, CGeomPoint ptCenter, CGeomPoint* pCirPts );
void CreateCircle_v( int nSpanpts, double dRadii, CGeomPoint ptCenter, Vector NormVect, CGeomPoint* pCirPts );


class CCircleDialog : public CDialog
{
	DECLARE_DYNAMIC(CCircleDialog)
	
public:
	CCircleDialog( COPTIONS *pstOpt, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCircleDialog();

// Dialog Data
	enum { IDD = IDD_CIRCLEDIALOG };

protected:
	virtual void DoDataExchange( CDataExchange* pDX );    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	COPTIONS *m_pstructOpt;
	CEdit m_dRadius, m_dX, m_dY, m_dZ;
	CEdit m_dVeci, m_dVecj, m_dVeck;
	afx_msg void OnBnClickedOk();
};
