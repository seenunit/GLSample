#include "StdAfx.h"
#include "BSurf.h"
#include "BSpline.h"
#include "Vector.h"

CBSurf::CBSurf(void) : CGeomEntity(TL_BSURF)
{
	m_ID = (unsigned int)this;
	m_bSelected = false;
}

CBSurf::CBSurf(CBSpline *pUBSpline, CBSpline *pVBSpline): CGeomEntity(TL_BSURF)
{
	m_pUBSpline = pUBSpline;
	m_pVBSpline = pVBSpline;

	if(pVBSpline == NULL)
	{
		double dParamRange[2] = {0, 1};
		double dKnotVector[4] = {0, 0, 1, 1};

		CGeomPoint *pUBSplCtrls = new CGeomPoint[pUBSpline->m_iCtrlPt];

		m_pUBSpline->GetBSplineCtrlPt(pUBSplCtrls);

		Vector vecStart(pUBSplCtrls[0].m_Coord[0], pUBSplCtrls[0].m_Coord[1], pUBSplCtrls[0].m_Coord[2]);
		Vector vecNormal(0,0,1);

		Vector vecEnd = vecStart + vecNormal * 100.0;

		CGeomPoint gptEnd(vecEnd.getx(), vecEnd.gety(), vecEnd.getz());

		CGeomPoint gptVCtrls[2] = {pUBSplCtrls[0], gptEnd} ;

		m_pVBSpline = new CBSpline( 2, 2, 10,  dParamRange, dKnotVector, gptVCtrls, RGB(0,0,0));

		m_pVBSpline->CalculateBSpline();
	}

	m_ID = (unsigned int)this;
	m_bSelected = false;
}

CBSurf::CBSurf(CGeomPoint *pGeomPt, int iUPts, int iVPts, int iUSpan, int iVSpan): CGeomEntity(TL_BSURF)
{
	m_iUPts = iUPts;
	m_iVPts = iVPts;
	m_iUSpan = iUSpan;
	m_iVSpan = iVSpan;
	m_pGeomPts = new CGeomPoint[iUPts*iVPts];

	for(int i = 0; i < iUPts*iVPts; i++)
	{
		for(int j = 0; j < 3; j++)
			m_pGeomPts[i].m_Coord[j] = pGeomPt[i].m_Coord[j];
	}

	m_pSurfPts = new CGeomPoint[iUSpan*iVSpan];

	m_pSurfNormls = new CGeomPoint[iUSpan*iVSpan];

	CalculateBSurface();

	m_ID = (unsigned int)this;
	m_bSelected = false;
}

CBSurf::~CBSurf(void)
{
	if(m_pGeomPts) delete [] m_pGeomPts;
	if(m_pSurfPts) delete [] m_pSurfPts;
	if(m_pSurfNormls) delete [] m_pSurfNormls;
}

void CBSurf::CalculateBSurface()
{
	//Fill the surface points.
	Matrix matGeomX(m_iUPts, m_iVPts);
	Matrix matGeomY(m_iUPts, m_iVPts);
	Matrix matGeomZ(m_iUPts, m_iVPts);
	for(int i = 0; i < m_iUPts; i++)
	{
		for(int j = 0; j < m_iVPts; j++)
		{
			matGeomX(i, j) = m_pGeomPts[i*m_iVPts+j].m_Coord[0];
			matGeomY(i, j) = m_pGeomPts[i*m_iVPts+j].m_Coord[1];
			matGeomZ(i, j) = m_pGeomPts[i*m_iVPts+j].m_Coord[2];
		}
	}

	double *pdUKnotVector = new double[m_iUPts + m_iUPts];
	double *pdUBlends = new double[m_iUPts];
	Matrix matUBlend(1, m_iUPts);
	double *pdUuBlend = new double[m_iUPts];
	Matrix matUuBlend(1, m_iUPts);
	OpenUniformKnotVector(m_iUPts, m_iUPts, pdUKnotVector, true);

	double *pdVKnotVector = new double[m_iVPts + m_iVPts];
	double *pdVBlends = new  double[m_iVPts];
	Matrix matVBlend(m_iVPts, 1);
	double *pdVvBlend = new double[m_iVPts];
	Matrix matVvBlend(1, m_iVPts);
	OpenUniformKnotVector(m_iVPts, m_iVPts, pdVKnotVector, true);

	int iSurfPt = 0;

	int iUSpan = 0;
	int iVSpan = 0;
	
	double dU =  1/(double)(m_iUSpan-1);
	double dV =  1/(double)(m_iVSpan-1);
	double u = 0.0;
	for( int i = 0; i < m_iUSpan; i++ )
	{
		if(i == (m_iUSpan-1))
			u = u * 0.9999999;

		BSplineBlends(u, m_iUPts, m_iUPts, pdUKnotVector, pdUBlends);
		for(int k = 0; k < m_iUPts; k++)
			matUBlend(0, k) = pdUBlends[k];

		BSplineFirstDervativeBlends(u, m_iUPts, m_iUPts, pdUKnotVector, pdUuBlend);
		for(int k = 0; k < m_iUPts; k++)
			matUuBlend(0, k) = pdUuBlend[k];
				
		iVSpan = 0;
		double v = 0.0;
		for(int j = 0; j < m_iVSpan; j++ )
		{
			if(j == (m_iVSpan-1))
				v = v * 0.9999999;

			BSplineBlends(v, m_iVPts, m_iVPts, pdVKnotVector, pdVBlends);
			for(int k = 0; k < m_iVPts; k++)
				matVBlend(k, 0) = pdVBlends[k];

			BSplineFirstDervativeBlends(v, m_iVPts, m_iVPts, pdVKnotVector, pdVvBlend);
			for(int k = 0; k < m_iVPts; k++)
				matVvBlend(0, k) = pdVvBlend[k];

			Matrix matX = matUBlend * matGeomX * matVBlend;
			Matrix matY = matUBlend * matGeomY * matVBlend;
			Matrix matZ = matUBlend * matGeomZ * matVBlend;

			m_pSurfPts[iSurfPt].m_Coord[0] = matX(0,0);
			m_pSurfPts[iSurfPt].m_Coord[1] = matY(0,0);
			m_pSurfPts[iSurfPt].m_Coord[2] = matZ(0,0);

			Vector vecUu, vecVv, vecNormal;
			
			Matrix matXu = matUuBlend * matGeomX * matVBlend;
			Matrix matYu = matUuBlend * matGeomY * matVBlend;
			Matrix matZu = matUuBlend * matGeomZ * matVBlend;

			vecUu.x = matXu(0, 0);
			vecUu.y = matYu(0, 0);
			vecUu.z = matZu(0, 0);

			Matrix matXv = matUBlend * matGeomX * matVvBlend;
			Matrix matYv = matUBlend * matGeomY * matVvBlend;
			Matrix matZv = matUBlend * matGeomZ * matVvBlend;

			vecVv.x = matXv(0, 0);
			vecVv.y = matYv(0, 0);
			vecVv.z = matZv(0, 0);

			vecNormal = vecUu * vecVv;

			m_pSurfNormls[iSurfPt].m_Coord[0] = vecNormal.x;
			m_pSurfNormls[iSurfPt].m_Coord[1] = vecNormal.y;
			m_pSurfNormls[iSurfPt].m_Coord[2] = vecNormal.z;
			
			v += dV;
			iSurfPt++;
			iVSpan++;
		}
		u += dU;
		iUSpan++;
	}

	//m_iUSpan = iUSpan;
	//m_iVSpan = iVSpan;

	if(pdUKnotVector) delete [] pdUKnotVector;
	if(pdVKnotVector) delete [] pdVKnotVector;
	if(pdUBlends) delete [] pdUBlends;
	if(pdVBlends) delete [] pdVBlends;
	if(pdUuBlend) delete [] pdUuBlend;
	if(pdVvBlend) delete [] pdVvBlend;
}

void CBSurf::Draw()
{
	if(m_bSelected)
		glColor3d(1,0,0);

	glBegin(GL_TRIANGLES);
	for(int i = 0; i < m_iUSpan-1; i++)
	{
		for(int j = 0; j < m_iVSpan-1; j++)
		{
			glNormal3d(m_pSurfNormls[i*(m_iUSpan)+j].m_Coord[0], m_pSurfNormls[i*(m_iUSpan)+j].m_Coord[1], m_pSurfNormls[i*(m_iUSpan)+j].m_Coord[2]);
			glVertex3d(m_pSurfPts[i*(m_iUSpan)+j].m_Coord[0], m_pSurfPts[i*(m_iUSpan)+j].m_Coord[1], m_pSurfPts[i*(m_iUSpan)+j].m_Coord[2]);
			glNormal3d(m_pSurfNormls[i*(m_iUSpan)+(j+1)].m_Coord[0], m_pSurfNormls[i*(m_iUSpan)+(j+1)].m_Coord[1], m_pSurfNormls[i*(m_iUSpan)+(j+1)].m_Coord[2]);
			glVertex3d(m_pSurfPts[i*(m_iUSpan)+(j+1)].m_Coord[0], m_pSurfPts[i*(m_iUSpan)+(j+1)].m_Coord[1], m_pSurfPts[i*(m_iUSpan)+(j+1)].m_Coord[2]);
			glNormal3d(m_pSurfNormls[(i+1)*(m_iUSpan)+j].m_Coord[0], m_pSurfNormls[(i+1)*(m_iUSpan)+j].m_Coord[1], m_pSurfNormls[(i+1)*(m_iUSpan)+j].m_Coord[2]);
			glVertex3d(m_pSurfPts[(i+1)*(m_iUSpan)+j].m_Coord[0], m_pSurfPts[(i+1)*(m_iUSpan)+j].m_Coord[1], m_pSurfPts[(i+1)*(m_iUSpan)+j].m_Coord[2]);
		}		
	}

	for(int i = 0; i < m_iUSpan-1; i++)
	{
		for(int j = 0; j < m_iVSpan-1; j++)
		{
			glNormal3d(m_pSurfNormls[i*(m_iUSpan)+(j+1)].m_Coord[0], m_pSurfNormls[i*(m_iUSpan)+(j+1)].m_Coord[1], m_pSurfNormls[i*(m_iUSpan)+(j+1)].m_Coord[2]);
			glVertex3d(m_pSurfPts[i*(m_iUSpan)+(j+1)].m_Coord[0], m_pSurfPts[i*(m_iUSpan)+(j+1)].m_Coord[1], m_pSurfPts[i*(m_iUSpan)+(j+1)].m_Coord[2]);
			glNormal3d(m_pSurfNormls[(i+1)*(m_iUSpan)+(j+1)].m_Coord[0], m_pSurfNormls[(i+1)*(m_iUSpan)+(j+1)].m_Coord[1], m_pSurfNormls[(i+1)*(m_iUSpan)+(j+1)].m_Coord[2]);
			glVertex3d(m_pSurfPts[(i+1)*(m_iUSpan)+(j+1)].m_Coord[0], m_pSurfPts[(i+1)*(m_iUSpan)+(j+1)].m_Coord[1], m_pSurfPts[(i+1)*(m_iUSpan)+(j+1)].m_Coord[2]);
			glNormal3d(m_pSurfNormls[(i+1)*(m_iUSpan)+j].m_Coord[0], m_pSurfNormls[(i+1)*(m_iUSpan)+j].m_Coord[1], m_pSurfNormls[(i+1)*(m_iUSpan)+j].m_Coord[2]);
			glVertex3d(m_pSurfPts[(i+1)*(m_iUSpan)+j].m_Coord[0], m_pSurfPts[(i+1)*(m_iUSpan)+j].m_Coord[1], m_pSurfPts[(i+1)*(m_iUSpan)+j].m_Coord[2]);
		}		
	}
	glEnd();

	glBegin(GL_LINES);
	for(int i = 0; i < m_iUSpan; i++)
	{
		for(int j = 0; j < m_iVSpan; j++)
		{
			Vector vecNormal(m_pSurfNormls[i*(m_iUSpan)+j].m_Coord[0], m_pSurfNormls[i*(m_iUSpan)+j].m_Coord[1], m_pSurfNormls[i*(m_iUSpan)+j].m_Coord[2]);
			Vector vecStart(m_pSurfPts[i*(m_iUSpan)+j].m_Coord[0], m_pSurfPts[i*(m_iUSpan)+j].m_Coord[1], m_pSurfPts[i*(m_iUSpan)+j].m_Coord[2]);
			Vector vecEnd = vecStart + vecNormal * 2.0;
			glVertex3d(m_pSurfPts[i*(m_iUSpan)+j].m_Coord[0], m_pSurfPts[i*(m_iUSpan)+j].m_Coord[1], m_pSurfPts[i*(m_iUSpan)+j].m_Coord[2]);
			glVertex3d(vecEnd.getx(), vecEnd.gety(), vecEnd.getz());
		}		
	}

	glEnd();
	
	//Draw Control points
	double dSize =  0.05;
	glColor3d(0.078, .4, .57);
	DrawPoints(m_iVPts*m_iUPts, dSize, m_pGeomPts);

	glLineWidth(2.0);

	//Control polygon
	for(int i = 0; i < m_iUPts; i++)
	{
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < m_iVPts; j++)
		{
			glVertex3d(m_pGeomPts[i*m_iVPts+j].m_Coord[0], m_pGeomPts[i*m_iVPts+j].m_Coord[1], m_pGeomPts[i*m_iVPts+j].m_Coord[2]);
		}
		glEnd();
	}

	for(int i = 0; i < m_iVPts; i++)
	{
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < m_iUPts; j++)
		{
			glVertex3d(m_pGeomPts[i+j*m_iUPts].m_Coord[0], m_pGeomPts[i+j*m_iUPts].m_Coord[1], m_pGeomPts[i+j*m_iUPts].m_Coord[2]);
		}
		glEnd();
	}
	
	glLineWidth(1.0);

	return;
}

int CBSurf::GetRowNumber()
{
	return m_iUPts;
}
int CBSurf::GetColumnNumber()
{
	return m_iVPts;
}