// CubicSpline.cpp : implementation file
//

#include "stdafx.h"
#include "CubicSpline.h"
#include "matrix.h"
#include "SampleMath.h"
#include "Line.h"

#include <iostream>
#include <new>
#include <cstdlib>
#include <malloc.h>
//#include <math.h>

using namespace std;
// CCubicSpline
IMPLEMENT_SERIAL( CCubicSpline, CGeomEntity, 1 )
//IMPLEMENT_DYNAMIC(CCubicSpline, CDialog)

//CSplBoundryCond CCubicSpline::eBcflag = CLAMPED; 

CCubicSpline::CCubicSpline()
{
	m_bClamped = true;			// TO BE REMOVED WHEN DIALOG IS REMOVED.
	m_bRelaxed = false;
}
CCubicSpline::CCubicSpline(int nCtrlPts, int nSpanPts, CGeomPoint *pCtrlPts, double *pEndTanVec, bool bClamped, bool bRelaxed )
{
	m_nCtrlPts = nCtrlPts;
	m_nSpanPts = nSpanPts;

	m_nCurvePts = nCtrlPts + ( nSpanPts - 2 ) * ( nCtrlPts - 1 );
	try {
		m_pCtrlPt = new CGeomPoint[nCtrlPts];
		for(int i = 0; i < nCtrlPts; i++)
			m_pCtrlPt[i] = pCtrlPts[i];

		m_pCurvePts = new CGeomPoint[m_nCurvePts];
		for( int i = 0; i < m_nCurvePts; i++ )
			for( int j = 0; j < m_pCurvePts[i].m_Dim; j++ )
				m_pCurvePts[i].m_Coord[j] = 0.0;

		m_pdEndTanVec = new double[2*THREEDIM];
		for( int i = 0; i < THREEDIM; i++ )
		{
			m_pdEndTanVec[0*THREEDIM+i] = pEndTanVec[0*THREEDIM+i];
			m_pdEndTanVec[1*THREEDIM+i] = pEndTanVec[1*THREEDIM+i];
		}
	}
	catch (bad_alloc xa) {
			cout << "Allocation Failure\n";
			exit(EXIT_FAILURE); 
	}
	m_bClamped = bClamped;
	m_bRelaxed = bRelaxed;
	cubicspline_i( m_nCtrlPts, m_nSpanPts, m_pCtrlPt, m_pdEndTanVec, m_bClamped, m_bRelaxed, m_pCurvePts, m_nCurvePts );
}

CCubicSpline::~CCubicSpline()
{
	delete []m_pCtrlPt;
	delete []m_pCurvePts;
	delete []m_pdEndTanVec;
}

//void CCubicSpline::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_CLAMPED, m_Clamped);
//	DDX_Control(pDX, IDC_RELAXED, m_Relaxed);
//}
//BEGIN_MESSAGE_MAP( CCubicSpline, CGeomEntity )
//	/*ON_BN_CLICKED(IDC_CLAMPED, &CCubicSpline::OnBnClickedClamped)
//	ON_BN_CLICKED(IDC_RELAXED, &CCubicSpline::OnBnClickedRelaxed)*/
//END_MESSAGE_MAP()

//intialize the dialog box
//afx_msg BOOL CCubicSpline::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	//Update the radio button
//	CButton *rbpClamped = ( CButton * ) GetDlgItem( IDC_CLAMPED );
//	CButton *rbpRelaxed = ( CButton * ) GetDlgItem( IDC_RELAXED );
//
//	rbpClamped->SetCheck(m_bClamped);
//	rbpRelaxed->SetCheck(m_bRelaxed);
//
//	return TRUE;
//}


void CCubicSpline::Serialize(CArchive &ar)
{
	CGeomEntity::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_nCtrlPts << m_nSpanPts << m_nCurvePts << m_bClamped << m_bRelaxed;
		for( int i = 0; i < m_nCtrlPts; i++ )
			m_pCtrlPt[i].Serialize( ar );
		for( int i = 0; i < THREEDIM; i++ )
			ar << m_pdEndTanVec[0*THREEDIM+i] << m_pdEndTanVec[1*THREEDIM+i];
	}
	else
	{
		ar >> m_nCtrlPts >> m_nSpanPts >> m_nCurvePts >> m_bClamped >> m_bRelaxed;
		try
		{
			m_pCtrlPt = new CGeomPoint[m_nCtrlPts];
			m_pdEndTanVec = new double[2*THREEDIM];
			m_pCurvePts = new CGeomPoint[m_nCurvePts];
		}		
		catch (bad_alloc xa)
		{
			cout << "Allocation Failure\n";
			exit(EXIT_FAILURE);
		}
		for( int i = 0; i < m_nCtrlPts; i++ )
			m_pCtrlPt[i].Serialize( ar );
		for( int i = 0; i < THREEDIM; i++)
			ar >> m_pdEndTanVec[0*THREEDIM+i] >> m_pdEndTanVec[1*THREEDIM+i];
	}

}
void CCubicSpline::Draw(CDC *pDc)
{
	//int p1 = MAXLOC; //no pts on each curve span
	//Matrix mCurvePts(m_npts+(p1-2)*(m_npts-1)/*ncpts*/, THREEDIM); //pts on curve
	//int nCount;
	//cubicspline(p1,&pmCurvePts, &nCount);
	//cubicspline_1(p1,&mCurvePts, &nCount);
	
	//int nCurvePts = 0;
	//cubicspline_i( m_nCtrlPts, m_nSpanPts, m_pCtrlPt, m_pdEndTanVec, m_bClamped, m_bRelaxed, m_pCurvePts, nCurvePts );
	DrawCurve( pDc, m_nCurvePts, m_pCurvePts );
}

void CCubicSpline::DrawCubicSpline(CDC *pDc, Matrix &matTransf)
{
	for(int i = 0; i < m_nCurvePts-1; i++)
	{
		CLine line(m_pCurvePts[i], m_pCurvePts[i+1]);
		line.DrawLine(pDc, matTransf);
	}
}

void CCubicSpline::DrawCurve( CDC *pDc, int nCurvePts, CGeomPoint *pdCurvePts )
{
	int LocIndex = 0;
	CPoint *Locations = new CPoint[nCurvePts]; //temp storage of pts
	CSize sizeWndExt = pDc->GetWindowExt();
	for(int i = 0; i < nCurvePts; i++)
	{
		Locations[i].x = pdCurvePts[i].m_Coord[0] * sizeWndExt.cx;
		Locations[i].y = pdCurvePts[i].m_Coord[1] * sizeWndExt.cy;
	}
	pDc->MoveTo( Locations[0] );
	for(int i = 0; i < nCurvePts; i++)
		pDc->LineTo( Locations[i] );	
	delete []Locations;
}
//void CCubicSpline::cubicspline(int nCSpanPts, Matrix *pmCurvePts, int *pCount)
//{
//	register int i,j;
//
//	Matrix *pmDataPts = new Matrix(m_npts, THREEDIM); // data pts array
//
//	for(i = 0; i < m_npts; i++)
//	{	
//		pmDataPts->put(i, 0) = m_ptData[i].x;
//		pmDataPts->put(i, 1) = m_ptData[i].y;
//	}
//	
//	//Matrix *pmCurvePts = new Matrix(p1/*ncpts*/, THREEDIM); //pts on curve
//	
//	Matrix *pmParamT = new Matrix(m_npts, 1); //chordwise approx parameter matrix
//	
//	Matrix *pmCoeffM = new Matrix(m_npts, m_npts); //coeff matrix
//	
//	Matrix *pmRight = new Matrix(m_npts, THREEDIM); // right hand side matrix
//	
//	Matrix *pmGeomMat = new Matrix(4, THREEDIM); //geometry matrix
//	
//	Matrix *pmBlendF = new Matrix(1, 4); //bleding func matrix
//
////	Matrix *pmPoint = new Matrix(1, THREEDIM);// single pts
//	
//	
//
//	
//	//finding parameter values using chaord approx method
//	for(i = 1; i < m_npts; i++)
//		(*pmParamT)(i, 0) = sqrt(sqr((*pmDataPts)(i, 0) - (*pmDataPts)(i-1, 0)) + sqr((*pmDataPts)(i, 1) - (*pmDataPts)(i-1, 1)));
//
//	//set up coeff matrix
//	for(i = 1; i < m_npts-1; i++)
//	{
//		pmCoeffM->put(i, i-1) = pmParamT->get(i+1, 0);
//		pmCoeffM->put(i, i)  = 2*(pmParamT->get(i, 0)+pmParamT->get(i+1, 0));
//		pmCoeffM->put(i, i+1) = pmParamT->get(i, 0);
//	}
//
//	double coeff;
//	//set up the right side matrix
//	for(i = 1; i < m_npts-1; i++)
//	{
//		coeff = 3/(float)(pmParamT->get(i, 0) * pmParamT->get(i+1, 0));
//		for(j = 0; j < 3; j++)
//		{
//			pmRight->put(i, j) = coeff * (sqr(pmParamT->get(i,0))*(pmDataPts->get(i+1,j)-pmDataPts->get(i, j)) + sqr(pmParamT->get(i+1, 0)) * (pmDataPts->get(i, j) - pmDataPts->get(i-1, j))); 
//		}
//	}
//
//	// add boundry condition
//	if(m_bClamped) {
//		pmCoeffM->put(0, 0) = 1;
//		pmCoeffM->put(m_npts-1, m_npts-1) = 1;
//		for(j = 0; j < 3; j++)
//		{
//			pmRight->put(0, j) = m_dEndTanVec[0][j];
//			pmRight->put(m_npts-1, j) = m_dEndTanVec[1][j];
//		}
//	}
//	else if(m_bRelaxed) {
//		pmCoeffM->put(0, 0) = 1;
//		pmCoeffM->put(0, 1) = 1/2;
//		pmCoeffM->put(m_npts-1, m_npts-2) = 2;
//		pmCoeffM->put(m_npts-1, m_npts-1) = 4;
//		for(j = 0; j < 3; j++)
//		{
//			pmRight->put(0, j) = (3/(2*pmParamT->get(1,0)))*(pmDataPts->get(1, j) - pmDataPts->get(0, j));
//			pmRight->put(m_npts-1, j) = (6/pmParamT->get(m_npts-1, 0))*(pmDataPts->get(m_npts-1, j) - pmDataPts->get(m_npts-2,j));
//		}
//	}
//	
//
//	//inveret the coeff matrix
//	Matrix *pmInvCoeffM = pmCoeffM->InverseMat();// inverse coeff matrix
//
//	//cal. internal tangent vector inv[M][R]
//	Matrix *pmTanVec = (*pmInvCoeffM) * (*pmRight); // tangent vector array
//
//	// generates the pts on the curve
//	int ncount = 0;
//
//	double d = 1/(float)(nCSpanPts-1);
//	double tau;
//
//	for(i = 1; i < m_npts; i++)
//	{
//		for(tau = 0 ; tau <=(1); tau += d)
//		{
//			
//			pmBlendF->put(0,0) = 2*(cube(tau)) - 3*(sqr(tau)) + 1;
//			pmBlendF->put(0,1) = -2*(cube(tau)) + 3*(sqr(tau));
//			pmBlendF->put(0,2) = tau*(sqr(tau) - 2*tau + 1)*pmParamT->get(i, 0);
//			pmBlendF->put(0,3) = tau*(sqr(tau) - tau)*pmParamT->get(i,0);
//
//			for(j = 0; j < 3; j++)
//			{
//				pmGeomMat->put(0,j) = pmDataPts->get( i-1, j);
//				pmGeomMat->put(1,j) = pmDataPts->get(i, j);
//				pmGeomMat->put(2,j) = pmTanVec->get(i-1, j);
//				pmGeomMat->put(3,j) = pmTanVec->get(i,j);
//			}
//			// cal single pt on the curve
//			Matrix *pmPoint = (*pmBlendF) * (*pmGeomMat); // single point matrix
//
//			//assign to position vector matrix
//			for(j = 0; j < 3; j++)
//				pmCurvePts->put(ncount, j) = pmPoint->get(0, j);
//
//			ncount++;
//			//m_Locations[ncount].x = pmPoint->get(0, 0);
//			//m_Locations[ncount].y = pmPoint->get(0, 1);	
//		}
//		/*for(int i = 0; i < m_LocIndex; i++)
//		m_Locations[i] += ptCenter;*/
//
//		//pDc->MoveTo(m_Locations[0]);
//
//		//for(int i = 0; i <= ncount; i++)
//		//	pDc->LineTo(m_Locations[i]);	
//
//		//pDc->LineTo(m_Locations[0]);
//
//		ncount--;		
//	}
//
//	ncount++;
//	*pCount = ncount;
//
////free memory
//	//delete [] pmParamT;
//	//delete [] pmCoeffM;
//	//delete [] pmRight;
//	//delete [] pmBlendF;
//	//delete [] pmGeomMat;
//		
//	/*for(j = 0; j < 3; j++)
//	{
//		pmCurvePts->put(0, j) = pmDataPts->get(0, j);
//		pmCurvePts->put(ncount-1, j) = pmDataPts->get(m_npts-1, j);
//	}*/
//}

//void CCubicSpline::cubicspline_1(int nCSpanPts, Matrix *pmCurvePts, int *pCount)
//{
//	register int i,j;
//
//	Matrix mDataPts(m_npts, THREEDIM); // data pts array
//
//	for(i = 0; i < m_npts; i++)
//	{	
//		mDataPts(i, 0) = m_ptData[i].x;
//		mDataPts(i, 1) = m_ptData[i].y;
//	}
//	
//	//Matrix *pmCurvePts = new Matrix(p1/*ncpts*/, THREEDIM); //pts on curve
//	
//	Matrix mParamT(m_npts, 1); //chordwise approx parameter matrix
//	
//	Matrix mCoeffM(m_npts, m_npts); //coeff matrix
//	
//	Matrix mRight(m_npts, THREEDIM); // right hand side matrix
//	
//	Matrix mGeomMat(4, THREEDIM); //geometry matrix
//	
//	Matrix mBlendF(1, 4); //bleding func matrix
//
////	Matrix *pmPoint = new Matrix(1, THREEDIM);// single pts
//	
//	//finding parameter values using chaord approx method
//	for(i = 1; i < m_npts; i++)
//		mParamT(i, 0) = sqrt(sqr(mDataPts(i, 0) - mDataPts(i-1, 0)) + sqr(mDataPts(i, 1) - mDataPts(i-1, 1)));
//
//	//set up coeff matrix
//	for(i = 1; i < m_npts-1; i++)
//	{
//		mCoeffM(i, i-1) = mParamT(i+1, 0);
//		mCoeffM(i, i)  = 2*( mParamT(i, 0)+ mParamT(i+1, 0) );
//		mCoeffM(i, i+1) = mParamT(i, 0);
//	}
//
//	double coeff;
//	//set up the right side matrix
//	for( i = 1; i < m_npts-1; i++ )
//	{
//		coeff = 3/(float)( mParamT(i, 0) * mParamT(i+1, 0) );
//		for(j = 0; j < 3; j++)
//		{
//			mRight(i, j) = coeff * ( sqr( mParamT(i,0))*( mDataPts(i+1,j) - mDataPts(i, j) ) + sqr( mParamT(i+1, 0) ) * ( mDataPts(i, j) - mDataPts(i-1, j) ) ); 
//		}
//	}
//
//	// add boundry condition
//	if(m_bClamped) {
//		mCoeffM(0, 0) = 1;
//		mCoeffM(m_npts-1, m_npts-1) = 1;
//		for(j = 0; j < 3; j++)
//		{
//			mRight(0, j) = m_dEndTanVec[0][j];
//			mRight(m_npts-1, j) = m_dEndTanVec[1][j];
//		}
//	}
//	else if(m_bRelaxed) {
//		mCoeffM(0, 0) = 1;
//		mCoeffM(0, 1) = 1/2;
//		mCoeffM(m_npts-1, m_npts-2) = 2;
//		mCoeffM(m_npts-1, m_npts-1) = 4;
//		for(j = 0; j < 3; j++)
//		{
//			mRight(0, j) = ( 3/( 2 * mParamT(1,0) ) ) * ( mDataPts(1, j) - mDataPts(0, j) );
//			mRight(m_npts-1, j) = ( 6 / mParamT(m_npts-1, 0) ) * ( mDataPts(m_npts-1, j) - mDataPts(m_npts-2,j) );
//		}
//	}
//
//	//inveret the coeff matrix
//	Matrix mInvCoeffM = mCoeffM.InverseMat();// inverse coeff matrix
//
//	//cal. internal tangent vector inv[M][R]
//	Matrix mTanVec = (mInvCoeffM) * (mRight); // tangent vector array
//
//	// generates the pts on the curve
//	int ncount = 0;
//
//	double d = 1/(float)(nCSpanPts-1);
//	double tau;
//
//	for(i = 1; i < m_npts; i++)
//	{
//		for(tau = 0 ; tau <=(1); tau += d)
//		{
//			
//			mBlendF(0,0) = 2*(cube(tau)) - 3*(sqr(tau)) + 1;
//			mBlendF(0,1) = -2*(cube(tau)) + 3*(sqr(tau));
//			mBlendF(0,2) = tau * (sqr(tau) - 2*tau + 1) * mParamT(i, 0);
//			mBlendF(0,3) = tau*(sqr(tau) - tau) * mParamT(i,0);
//
//			for(j = 0; j < 3; j++)
//			{
//				mGeomMat(0,j) = mDataPts( i-1, j);
//				mGeomMat(1,j) = mDataPts(i, j);
//				mGeomMat(2,j) = mTanVec(i-1, j);
//				mGeomMat(3,j) = mTanVec(i,j);
//			}
//			// cal single pt on the curve
//			Matrix mPoint = (mBlendF) * (mGeomMat); // single point matrix
//
//			//assign to position vector matrix
//			for(j = 0; j < 3; j++)
//				(*pmCurvePts)(ncount, j) = mPoint(0, j);
//
//			ncount++;
//			//m_Locations[ncount].x = pmPoint->get(0, 0);
//			//m_Locations[ncount].y = pmPoint->get(0, 1);	
//		}
//		/*for(int i = 0; i < m_LocIndex; i++)
//		m_Locations[i] += ptCenter;*/
//
//		//pDc->MoveTo(m_Locations[0]);
//
//		//for(int i = 0; i <= ncount; i++)
//		//	pDc->LineTo(m_Locations[i]);	
//
//		//pDc->LineTo(m_Locations[0]);
//
//		ncount--;		
//	}
//
//	ncount++;
//	*pCount = ncount;
//	
//	/*for(j = 0; j < 3; j++)
//	{
//		pmCurvePts->put(0, j) = pmDataPts->get(0, j);
//		pmCurvePts->put(ncount-1, j) = pmDataPts->get(m_npts-1, j);
//	}*/
//}

void cubicspline_i( int nCtrPts, int nSpanPts, CGeomPoint *pCtrPts, double *pEndTanVec, bool bClamped, bool bRelaxed, CGeomPoint *pCurvePts, int &nCurvePts )
{
	Matrix mDataPts(nCtrPts, THREEDIM); // data pts array

	for( int i = 0; i < nCtrPts; i++)
		for( int  j = 0; j < THREEDIM; j++ )
				mDataPts(i, j) = pCtrPts[i].m_Coord[j];
		
	Matrix mParamT(nCtrPts, 1); //chordwise approx parameter matrix
	
	Matrix mCoeffM(nCtrPts, nCtrPts); //coeff matrix
	
	Matrix mRight(nCtrPts, THREEDIM); // right hand side matrix
	
	Matrix mGeomMat(4, THREEDIM); //geometry matrix
	
	Matrix mBlendF(1, 4); //bleding func matrix

	//finding parameter values using chaord approx method
	for(int i = 1; i < nCtrPts; i++)
		mParamT(i, 0) = sqrt(sqr(mDataPts(i, 0) - mDataPts(i-1, 0)) + sqr(mDataPts(i, 1) - mDataPts(i-1, 1)));

	//set up coeff matrix
	for(int i = 1; i < nCtrPts-1; i++)
	{
		mCoeffM(i, i-1) = mParamT(i+1, 0);
		mCoeffM(i, i)  = 2*( mParamT(i, 0)+ mParamT(i+1, 0) );
		mCoeffM(i, i+1) = mParamT(i, 0);
	}
	double coeff;
	//set up the right side matrix
	for( int i = 1; i < nCtrPts-1; i++ )
	{
		coeff = 3/(float)( mParamT(i, 0) * mParamT(i+1, 0) );
		for(int j = 0; j < 3; j++)
		{
			mRight(i, j) = coeff * ( sqr( mParamT(i,0))*( mDataPts(i+1,j) - mDataPts(i, j) ) + sqr( mParamT(i+1, 0) ) * ( mDataPts(i, j) - mDataPts(i-1, j) ) ); 
		}
	}
	// add boundry condition
	if(bClamped) {
		mCoeffM(0, 0) = 1;
		mCoeffM(nCtrPts-1, nCtrPts-1) = 1;
		for( int j = 0; j < 3; j++)
		{
			mRight(0, j) = pEndTanVec[0*THREEDIM+j];
			mRight(nCtrPts-1, j) = pEndTanVec[1*THREEDIM+j];
		}
	}
	else if(bRelaxed) {
		mCoeffM(0, 0) = 1;
		mCoeffM(0, 1) = 1/2;
		mCoeffM(nCtrPts-1, nCtrPts-2) = 2;
		mCoeffM(nCtrPts-1, nCtrPts-1) = 4;
		for( int j = 0; j < 3; j++)
		{
			mRight(0, j) = ( 3/( 2 * mParamT(1,0) ) ) * ( mDataPts(1, j) - mDataPts(0, j) );
			mRight(nCtrPts-1, j) = ( 6 / mParamT(nCtrPts-1, 0) ) * ( mDataPts(nCtrPts-1, j) - mDataPts(nCtrPts-2,j) );
		}
	}
	//inveret the coeff matrix
	Matrix mInvCoeffM = mCoeffM.InverseMat();// inverse coeff matrix

	//cal. internal tangent vector inv[M][R]
	Matrix mTanVec = (mInvCoeffM) * (mRight); // tangent vector array

	// generates the pts on the curve
	nCurvePts = 0;

	double d = 1/(float)(nSpanPts-1);
	double tau;

	for(int i = 1; i < nCtrPts; i++)
	{
		for(tau = 0 ; tau <=(1); tau += d)
		{
			mBlendF(0,0) = 2*(cube(tau)) - 3*(sqr(tau)) + 1;
			mBlendF(0,1) = -2*(cube(tau)) + 3*(sqr(tau));
			mBlendF(0,2) = tau * (sqr(tau) - 2*tau + 1) * mParamT(i, 0);
			mBlendF(0,3) = tau*(sqr(tau) - tau) * mParamT(i,0);

			for(int j = 0; j < 3; j++)
			{
				mGeomMat(0,j) = mDataPts( i-1, j);
				mGeomMat(1,j) = mDataPts(i, j);
				mGeomMat(2,j) = mTanVec(i-1, j);
				mGeomMat(3,j) = mTanVec(i,j);
			}
			// cal single pt on the curve
			Matrix mPoint = (mBlendF) * (mGeomMat); // single point matrix
			//assign to position vector matrix
			for(int j = 0; j < 3; j++)
				pCurvePts[nCurvePts].m_Coord[j]= mPoint(0, j);
			nCurvePts++;
		}
		nCurvePts--;		
	}
	nCurvePts++;
}



// CCubicSpline dialog

//IMPLEMENT_DYNAMIC( CCubicSplineDlg, CDialog )
//
//CCubicSplineDlg::CCubicSplineDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(CCubicSplineDlg::IDD, pParent)
//{
//	m_bClamped = true;						// TO BE REMOVED WHEN DIALOG IS REMOVED.
//	m_bRelaxed = false;
//}
//
//CCubicSplineDlg::~CCubicSplineDlg()
//{
//}
//
//void CCubicSplineDlg::DoDataExchange( CDataExchange* pDX )
//{
//	CDialog::DoDataExchange( pDX );
//	DDX_Control(pDX, IDC_CLAMPED, m_buttonClamped);
//	DDX_Control(pDX, IDC_RELAXED, m_buttonRelaxed);
//}
//
//BEGIN_MESSAGE_MAP( CCubicSplineDlg, CDialog )		
//	ON_BN_CLICKED(IDC_CLAMPED, &CCubicSplineDlg::OnBnClickedClamped)
//	ON_BN_CLICKED(IDC_RELAXED, &CCubicSplineDlg::OnBnClickedRelaxed)
//END_MESSAGE_MAP()
//
//
////intialize the dialog box
//afx_msg BOOL CCubicSplineDlg::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	m_buttonClamped.SetCheck( m_bClamped );
//	m_buttonRelaxed.SetCheck( m_bRelaxed );	
//
//	return TRUE;
//}
//
//void CCubicSplineDlg::OnBnClickedClamped()
//{
//	// TODO: Add your control notification handler code here
//	m_bClamped = true;
//	m_bRelaxed = false;
//}
//
//void CCubicSplineDlg::OnBnClickedRelaxed()
//{
//	// TODO: Add your control notification handler code here
//	m_bClamped = false;
//	m_bRelaxed = true;
//}
