// CubicSplineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GLSampleDoc.h"
//include "SampleDoc.h"
//#include "CubicSpline.h"
#include "CubicSplineDlg.h"
#include <math.h>
#include "Matrix.h"

// CCubicSplineDlg dialog

IMPLEMENT_DYNAMIC(CCubicSplineDlg, CDialog)

CCubicSplineDlg::CCubicSplineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCubicSplineDlg::IDD, pParent)
	, cs_x1(0)
	, cs_x2(0)
	, cs_y1(0)
	, cs_y2(0)
	, cs_tx1(0)
	, cs_tx2(0)
	, cs_ty1(0)
	, cs_ty2(0)
{

}

CCubicSplineDlg::~CCubicSplineDlg()
{
}

void CCubicSplineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CB_X1, cs_x1);
	DDX_Text(pDX, IDC_CB_X2, cs_x2);
	DDX_Text(pDX, IDC_CB_Y1, cs_y1);
	DDX_Text(pDX, IDC_CB_Y2, cs_y2);
	DDX_Text(pDX, IDC_CB_TX1, cs_tx1);
	DDX_Text(pDX, IDC_CB_TX2, cs_tx2);
	DDX_Text(pDX, IDC_CB_TY1, cs_ty1);
	DDX_Text(pDX, IDC_CB_TY2, cs_ty2);
}


BEGIN_MESSAGE_MAP(CCubicSplineDlg, CDialog)
END_MESSAGE_MAP()

// CCubicSplineDlg message handlers
/*void CCubicSplineDlg::CreateSpline(int x1,int y1, int tx1,int ty1, int x2, int y2, int tx2, int ty2)
{
	int n_pt = MAXLOC;
	double t = 0;
	
	float u[1*4] ;

	float m[4*4] = { 2, -2, 1, 1,
					-3,  3, -2, -1,
					0,   0,  1,  0,
					1,   0,  0,  0,};

	float *um = new float[1*4];
	
	float px[4*1] = {x1,x2,tx1,tx2};
	float py[4*1] = {y1,y2,ty1,ty2};

	float *umpx = new float[1*1];
	float *umpy = new float[1*1];

	Matrix mat;
	CSampleDocDoc splinedoc;
	
	for(int i=0;i<100;i++)
	{
		t = t/n_pt;
		u[1*4]= {t*t*t, t*t, t, 1};
		um = mat.MultplyMat(u,1,4,m,4,4);

		umpx = mat.MultplyMat(um,1,4,px,4,1);
		umpy = mat.MultplyMat(um,1,4,py,4,1);

		splinedoc.m_CSplineLoc[i].x = umpx[0];
		splinedoc.m_CSplineLoc[i].y = umpy[0];

		splinedoc.m_CSplLocIndex++;

	}

}*/