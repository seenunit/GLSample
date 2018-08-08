#pragma once


// CCubicSplineDlg dialog
#include "Matrix.h"
class CCubicSplineDlg : public CDialog
{
	DECLARE_DYNAMIC(CCubicSplineDlg)

public:
	CCubicSplineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCubicSplineDlg();

// Dialog Data
	enum { IDD = IDD_CSPLINEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// x- coordinate
	int cs_x1;
	int cs_x2;
public:
	// y-coordinate
	int cs_y1;
	int cs_y2;
public:
	// tangent vector in x-dir
	float cs_tx1;
	float cs_tx2;
public:
	// tangent vector in y-dir
	float cs_ty1;
	float cs_ty2;
	//void CreateSpline(int x1,int y1, int tx1,int ty1, int x2, int y2, int tx2, int ty2);//creats spline points
};
