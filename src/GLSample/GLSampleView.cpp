// GLSampleView.cpp : implementation of the CGLSampleView class
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "GLSampleView.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CGLSampleDoc *g_pDoc;
ofstream g_streamOut;

// CGLSampleView

IMPLEMENT_DYNCREATE(CGLSampleView, CView)

BEGIN_MESSAGE_MAP(CGLSampleView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VIEW_FRONT, &CGLSampleView::OnViewFront)
	ON_COMMAND(ID_VIEW_FIT, &CGLSampleView::OnViewFit)	
	ON_COMMAND(ID_VIEW_PAN, &CGLSampleView::OnViewPan)
END_MESSAGE_MAP()

void WriteToFile()
{
	//trans cpoint to world
	double dModel[16] = {0};
	double dProj[16] = {0};
	VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dModel));
	VERIFY_GL(glGetDoublev(GL_PROJECTION_MATRIX, dProj));

	GLdouble dlWinPoint[3] = {0};
	GLint glViewPort[4];
	glGetIntegerv (GL_VIEWPORT, glViewPort);
	
	gluProject(0, 0, 0, dModel, dProj, glViewPort, &dlWinPoint[0], &dlWinPoint[1], &dlWinPoint[2]);
	g_streamOut << "Model view Matrix:\n";
	for(int i = 0; i < 4 ; i++)
	{
		for(int j = 0; j < 4; j++)
			g_streamOut << dModel[i*4+j] << "\t";
		g_streamOut << "\n";
	}
	g_streamOut << "Projection Matrix:\n";
	for(int i = 0; i < 4 ; i++)
	{
		for(int j = 0; j < 4; j++)
			g_streamOut << dProj[i*4+j] << "\t";
		g_streamOut << "\n";
	}
	g_streamOut << "View port Extensions:\t" << glViewPort[2] << "\t" << glViewPort[3] << "\n";
	g_streamOut << "Window projects point:\t" << dlWinPoint[0] << "\t" << dlWinPoint[1] << "\t" << dlWinPoint[2] << "\n";


	CGeomPoint gptTemp;
	Matrix M(dModel, 4, 4);
	Matrix P(dProj, 4, 4);

	Matrix Mi = M.InverseMat();
	Matrix Pi = P.InverseMat();

	TransFormEntity(&gptTemp, 1, M, &gptTemp);

	g_streamOut << "Modelview Project Point:\n" << gptTemp.m_Coord[0] << "\t" << gptTemp.m_Coord[1] << "\t" << gptTemp.m_Coord[2] << "\n";
	
	TransFormEntity(&gptTemp, 1, Pi, &gptTemp);
	
	g_streamOut << gptTemp.m_Coord[0] << "\t" << gptTemp.m_Coord[1] << "\t" << gptTemp.m_Coord[2] << "\n";

	g_streamOut << "Project Temp Point:\t" << gptTemp.m_Coord[0]+(glViewPort[2]/2) << "\t" << gptTemp.m_Coord[1]+(glViewPort[3]/2)<< "\t" << gptTemp.m_Coord[2]+0.5 << "\n";
	gluUnProject(dlWinPoint[0], dlWinPoint[1], dlWinPoint[2], dModel, dProj, glViewPort, &dlWinPoint[0], &dlWinPoint[1], &dlWinPoint[2]);
	g_streamOut << "Window unprojects point:\t" << dlWinPoint[0] << "\t" << dlWinPoint[1] << "\t" << dlWinPoint[2] << "\n";

}
// CGLSampleView construction/destruction

CGLSampleView::CGLSampleView()
: m_GLPixelIndex(0)
, m_bRightDownButton(FALSE)
, m_bLeftdownButton(FALSE)
{
	// TODO: add construction code here
	m_hGLContext = NULL;
	m_GLPixelIndex = 0;
	m_iCurrentBSplineID = NULL;
	m_iCurrentPointID = NULL;
	m_iCurrentLineID = NULL;
	m_iPickEntityID = NULL;
	m_pDocument = NULL;

	m_xRotate = 30.0;
	m_yRotate = 45.0;

	char cCurrentDir[_MAX_PATH] = {0};
	GetCurrentDirectory(_MAX_PATH, cCurrentDir);
	strcat_s(cCurrentDir, _MAX_PATH, "\\Points.txt");
	g_streamOut.open(cCurrentDir, ios_base::out);
	g_streamOut.precision(3);
	g_streamOut.width(5);
}

CGLSampleView::~CGLSampleView()
{

}

BOOL CGLSampleView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	//For OpenGL
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS); 

	return CView::PreCreateWindow(cs);
}

void Tetrahedron()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw the tetrahedron.  It is a four sided figure, so when defining it
	// with a triangle strip we have to repeat the last two vertices.
	glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
		glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
		glColor3f(0, 1, 0); glVertex3f(1, 0, 1);
		glColor3f(0, 0, 1); glVertex3f(0, 0, -1.4);
		glColor3f(1, 1, 1); glVertex3f(0, 2, 0);
		glColor3f(1, 0, 0); glVertex3f(-1, 0, 1);
	glEnd();

}

void CreatePolygon()
{
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	glColor3f(0.0, 1.0, 0.0); /* green */

	glBegin(GL_POLYGON);
	glEdgeFlag(GL_TRUE);
	glNormal3d(0.0, 0.0, 1.0);
	glVertex3d(1.0, 0.0, 0.0);
	glVertex3d(2.0, 1.0, 0.0);
	glVertex3d(2.0, 3.0, 0.0);
	glVertex3d(1.0, 2.0, 0.0);
	glVertex3d(0.0, 3.0, 0.0);
	glVertex3d(0.0, 1.0, 0.0);
	glEdgeFlag(GL_FALSE);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0); /* green */
	glVertex3d(0,0,0);
	glColor3f(1.0, 1.0, 0.0); /* yellow */
	glVertex3d(1,0,0);
	glColor3f(1.0, 0.0, 0.0); /* red */
	glVertex3d(1,1,0);
	glColor3f(0.0, 0.0, 1.0); /* blue */
	glVertex3d(0,1,0);
	glEnd();

	glFrontFace(GL_CW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glBegin(GL_POLYGON);
	glColor3f(0.0, 1.0, 0.0); /* green */
	glVertex3d(0,0,0);
	glColor3f(1.0, 1.0, 0.0); /* yellow */
	glVertex3d(0,0,-1);
	glColor3f(1.0, 0.0, 0.0); /* red */
	glVertex3d(0,1,-1);
	glColor3f(0.0, 0.0, 1.0); /* blue */
	glVertex3d(0,1,0);
	glEnd();

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}

// CGLSampleView drawing

void CGLSampleView::OnDraw(CDC* pDC)
{
	CGLSampleDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	g_pDoc = pDoc;

	RenderScene(GL_RENDER);

	//PrepareScene();
	//void CreatePolygon();
	//CreatePolygon();
	Tetrahedron();

	glFlush();
	SwapBuffers(pDC->m_hDC);
}

// CGLSampleView printing

BOOL CGLSampleView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLSampleView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLSampleView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGLSampleView diagnostics

#ifdef _DEBUG
void CGLSampleView::AssertValid() const
{
	CView::AssertValid();
}

void CGLSampleView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLSampleDoc* CGLSampleView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLSampleDoc)));
	return (CGLSampleDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLSampleView message handlers

BOOL CGLSampleView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;
	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | 
	  PFD_DOUBLEBUFFER | 
	  PFD_SUPPORT_OPENGL | 
	  PFD_STEREO_DONTCARE;
	pixelDesc.iPixelType = PFD_TYPE_RGBA;
	pixelDesc.cColorBits = 32;
	pixelDesc.cRedBits = 8; 
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;    
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;
	pixelDesc.cStencilBits = 8;
	pixelDesc.cAuxBuffers = 0;
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_GLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
	if (m_GLPixelIndex==0) // Let's choose a default index.
	{
	  m_GLPixelIndex = 1;    
	  if (DescribePixelFormat(hDC, m_GLPixelIndex, 
	  sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc)==0)
	  {
		 return FALSE;
	  }
	}

	if (SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc)==FALSE)
	{
	  return FALSE;
	}

	return TRUE;;
}

int CGLSampleView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HWND hWnd = GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if( SetWindowPixelFormat(hDC) == FALSE )
		return 0;

	if( CreateViewGLContext( hDC ) == FALSE )
		return 0;

	return 0;
}

BOOL CGLSampleView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if (m_hGLContext == NULL)
	{
	  return FALSE;
	}

	if (wglMakeCurrent(hDC, m_hGLContext)==FALSE)
	{
	  return FALSE;
	}

	return TRUE;
}

void CGLSampleView::OnDestroy()
{
	if( wglGetCurrentContext() != NULL )
	{
		// Make the rendering context not current
		wglMakeCurrent(NULL, NULL);
	}

	if( m_hGLContext != NULL )
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
	//Now associate DC can be released.

	CView::OnDestroy();

	// TODO: Add your message handler code here
}

void CGLSampleView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_iWinWidth = cx;
	m_iWinHeight = cy;

	//Creating Global volume(World Coordinate system)
	SetViewVolume();
}

void CGLSampleView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLeftdownButton = TRUE;
	
	if(m_currentOperation != Pan)
		m_currentOperation = Pick;

	CGLSampleDoc *pDoc = GetDocument();

	g_streamOut << "Pick point:\t" << point.x << "\t" << point.y << "\n";
		
	PickSquares(point);

	Invalidate(FALSE);

	m_currentPoint = point;
	m_Startpoint = point;

	if( pDoc->m_bLine )
	{
		CGeomPoint ptStart;
		CGeomPoint ptEnd;

		double dModel[16] = {0};
		double dProj[16] = {0};
		VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dModel));
		VERIFY_GL(glGetDoublev(GL_PROJECTION_MATRIX, dProj));

		GLint glViewPort[4];
		glGetIntegerv (GL_VIEWPORT, glViewPort);
		CPoint glpoint(point.x, glViewPort[3] - point.y);
		CPoint glStartPoint(m_Startpoint.x, glViewPort[3] - m_Startpoint.y);

		gluUnProject(glStartPoint.x, glStartPoint.y, .5, dModel, dProj, glViewPort, &(ptStart.m_Coord[0]), &(ptStart.m_Coord[1]), &(ptStart.m_Coord[2]));
		gluUnProject(glpoint.x, glpoint.y, .5, dModel, dProj, glViewPort, &(ptEnd.m_Coord[0]), &(ptEnd.m_Coord[1]), &(ptEnd.m_Coord[2]));
		
		CLine *pLine = pDoc->AddLine(ptStart, ptEnd);
		m_iCurrentLineID = pLine->GetID();
	}

	CView::OnLButtonDown(nFlags, point);
}

void CGLSampleView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLeftdownButton = FALSE;
	m_currentOperation = Idle;

	CGLSampleDoc *pDoc = GetDocument();

	if( pDoc->m_bLine && m_iCurrentLineID )
	{
		double dModel[16] = {0};
		double dProj[16] = {0};
		VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dModel));
		VERIFY_GL(glGetDoublev(GL_PROJECTION_MATRIX, dProj));

		GLint glViewPort[4];
		glGetIntegerv (GL_VIEWPORT, glViewPort);
		CPoint glpoint(point.x, glViewPort[3] - point.y);
		CGeomPoint ptEnd;
		gluUnProject(glpoint.x, glpoint.y, .5, dModel, dProj, glViewPort, &(ptEnd.m_Coord[0]), &(ptEnd.m_Coord[1]), &(ptEnd.m_Coord[2]));
		
		CLine *pLine = pDoc->GetLineFromID(m_iCurrentLineID);
		pLine->ModifyLine(pLine->GetStartPoint(), ptEnd);

		m_iCurrentLineID = NULL;
				
		Invalidate(FALSE);
	}

	g_streamOut.close();

	CView::OnLButtonUp(nFlags, point);
}

void CGLSampleView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bRightDownButton = TRUE;
	
	CView::OnRButtonDown(nFlags, point);
}

void CGLSampleView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bRightDownButton = FALSE;

	CView::OnRButtonUp(nFlags, point);
}

void CGLSampleView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	CGLSampleDoc *pDoc = GetDocument();

    if(nFlags == MK_MBUTTON)
    {
        RotateView( point );
    }
    
	if ( nFlags /*& MK_LBUTTON */)
	{
		switch ( m_currentOperation )
		{
		//case Rotate : 
		//	RotateView( point );
		//	break;
		case Pan:
			PanView( point );
			break;
		case Zoom:
			//ZoomView( point );
			break;
		case Idle:
			//ProximitySelect( point );
			break;
		case Pick:
			ModifyEntity(point);
		default:
			break;
		}
	}

	m_currentPoint = point;

	if( pDoc->m_bLine && m_iCurrentLineID )
	{
		double dModel[16] = {0};
		double dProj[16] = {0};
		VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dModel));
		VERIFY_GL(glGetDoublev(GL_PROJECTION_MATRIX, dProj));

		GLint glViewPort[4];
		glGetIntegerv (GL_VIEWPORT, glViewPort);
		CPoint glpoint(point.x, glViewPort[3] - point.y);
		CGeomPoint ptEnd;
		gluUnProject(glpoint.x, glpoint.y, .5, dModel, dProj, glViewPort, &(ptEnd.m_Coord[0]), &(ptEnd.m_Coord[1]), &(ptEnd.m_Coord[2]));
		
		CLine *pLine = pDoc->GetLineFromID(m_iCurrentLineID);
		pLine->ModifyLine(pLine->GetStartPoint(), ptEnd);
		
		Invalidate(FALSE);
	}
	
	CView::OnMouseMove(nFlags, point);
}


// Rendres the scene
void CGLSampleView::RenderScene(GLenum glRenderMode)
{
	CGLSampleDoc *pDoc = GetDocument();

	static GLdouble white[3] = {1.0, 1.0, 1.0} ;
		// Set up some colors
	static GLdouble red[3] = {0.8, 0.0, 0.0 } ; 
	static GLdouble green[3] = {0.0, 0.75, 0.0} ;
	static GLdouble purple[3] = {1.0, 0.14, 0.6667} ; 


	PrepareScene();

	glPushMatrix();	//push the matrix to create identical matrix with out translation
	//glTranslated(-pDoc->m_ViewCenter[0], -pDoc->m_ViewCenter[1], -pDoc->m_ViewCenter[2]);

	//Render World Coordinate Axes.
	RenderCSYS();

    if( pDoc->GetLineCount() )
    {
        for( int i = 0; i < pDoc->GetLineCount(); i++ )
        {
            CLine *pLine = pDoc->GetLine(i);
            pLine->Draw();
        }
    }
    if(pDoc->GetBSplineCount())
    {
        for(int i = 0; i < pDoc->GetBSplineCount(); i++)
        {
            CBSpline *pBspline = pDoc->GetBSpline(i);
            if (glRenderMode == GL_SELECT)
                glLoadName(pBspline->GetID());
            pBspline->Draw();
        }
    }
    if(pDoc->GetBSurfCount())
    {
        for(int i = 0; i < pDoc->GetBSurfCount(); i++)
        {
            CBSurf *pBSurf = pDoc->GetBSurf(i);
            if (glRenderMode == GL_SELECT)
                glLoadName(pBSurf->GetID());
            pBSurf->Draw();
        }
    }
    if(pDoc->GetBeamCount())
    {
        CFemBeam *pBeam = pDoc->GetFemBeam(0);
        if (glRenderMode == GL_SELECT)
            glLoadName(pBeam->GetID());
        pBeam->Draw();
    }

	glPopMatrix(); //Get Matrix with out translation component
}

void CGLSampleView::PrepareScene()
{
	//// Clear the color and depth buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // use OR run faster on many machines

	glColor3f(1.0, 0.0, 1.0);

	//GLfloat matSpecular[] = {1.0,0.0,0.0,0.0};
	//GLfloat matShininess[] = {250.0};
	//GLfloat lightPosition0[] = {-0.0,0.0,-5.0,0.0};	
	////GLfloat lightPosition1[] = {0.0,0.0,0.0,0.0};
	//GLfloat sw_Ambient[] = { 0.29f, 0.29f, 0.29f, 1.0f };
	//GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   

	////glClearColor(0.0,0.0,0.0,0.0);
	//glShadeModel(GL_SMOOTH);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, sw_Ambient);
 //   glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, matSpecular);
	//glLightfv(GL_LIGHT0, GL_SHININESS, matShininess);	
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);	
	////glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);	

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);	
	//glEnable(GL_LIGHT1);

	//         

	//glColor3d(1,0,0);
 //   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
 //   glMaterialfv(GL_FRONT, GL_AMBIENT, sw_Ambient);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
 //   glMateriali(GL_FRONT, GL_SHININESS, 400);

	////glEnable(GL_LIGHT1);	
	glEnable(GL_DEPTH_TEST);
	glLineWidth(0.5);

}

//
//	OutputGlError
//
void CGLSampleView::OutputGlError(char* label) 
{
	GLenum errorno = glGetError() ;
	if (errorno != GL_NO_ERROR)
		TRACE("%s had error: #(%d) %s\r\n", label, errorno, gluErrorString(errorno)) ;
}

//-----------------------------------------------------------------------------
// FUNC:	SetViewVolume
// ACTION:	Sets the extents of the view volume. Make sure to MakeCurrent(TRUE)
//			before calling. Call this function if you think you have changed
//			the window extents, part bounding box, or zoom


void CGLSampleView::SetViewVolume()
{
	CGLSampleDoc* pDoc = GetDocument();
	
	double sphereSize = pDoc->m_dSphereRad * 100.0;

	double xextent = double( m_iWinWidth ) / min( double( m_iWinWidth ), double( m_iWinHeight ));
	double yextent = double( m_iWinHeight ) / min( double( m_iWinWidth ), double( m_iWinHeight ));
	
	glViewport( 0, 0, m_iWinWidth, m_iWinHeight );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -xextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			  xextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			 -yextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			  yextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			  sphereSize, -sphereSize);
}


//
//Render World Cordinte Axes
//
void CGLSampleView::RenderCSYS()
{
	CGLSampleDoc *pDoc = GetDocument();

	// Scaling and Ft not effects the Axis length
	double AxsLen = 1.0 * (pDoc->m_dSphereRad / 10.0) * pDoc->m_dScaleFactor;
	
	glColor3d(0.8, 0.0, 0.0);
	glBegin(GL_LINES);//X-axis
		glVertex3d(     0, 0, 0) ; // Vertex Origin
		glVertex3d(AxsLen, 0, 0) ; // Vertex X
	glEnd();
	
	glColor3d(0.0, 0.75, 0.0);
	glBegin(GL_LINES);//Y-axis
		glVertex3d(0,     0, 0) ; // Vertex Origin
		glVertex3d(0,AxsLen, 0) ; // Vertex Y
	glEnd();

	glColor3d(0.0, 0.0, 0.8);
	glBegin(GL_LINES);//Z-Axis
		glVertex3d(0, 0,	 0) ; // Vertex Origin
		glVertex3d(0, 0,AxsLen) ; // Vertex Z
	glEnd();
}

//-----------------------------------------------------------------------------
// Helper functions for Pan, Zoom and Rotate
//

//-----------------------------------------------------------------------------
// FUNC;	RotateView
// ACTION:	Rotates the view using the difference between two CPoints

void CGLSampleView::RotateView(CPoint point)
{
	//g_streamOut << "Rotation ---------\n";
	//WriteToFile();

	double diffx, diffy;
	//diffx = (double) (TransformDevicePoint(point).x - TransformDevicePoint(m_currentPoint).x);
	//diffy = (double) (TransformDevicePoint(point).y - TransformDevicePoint(m_currentPoint).y);
	diffx = (double) (point.x - m_currentPoint.x);
	diffy = (double) (point.y - m_currentPoint.y);

	m_xRotate = diffx * ROTATE_SENSITIVITY;
	m_yRotate = diffy * ROTATE_SENSITIVITY;

	double m[ 16 ];

	glMatrixMode( GL_MODELVIEW ) ;
	glGetDoublev( GL_MODELVIEW_MATRIX, m ) ;
	glLoadIdentity() ;

	glRotated( m_xRotate, 0.0, 1.0, 0.0 ) ; 
	glRotated( m_yRotate, 1.0, 0.0, 0.0 );
	
	glMultMatrixd( m ) ;
	Invalidate( FALSE );

}

//-----------------------------------------------------------------------------
// FUNC:	PanView
// ACTION:	Moves the centre of the view using the difference between two points.

void CGLSampleView::PanView(CPoint point)
{
	// get the document, manipulating centre 
	CGLSampleDoc* pDoc = GetDocument();
	
	double dFactor = PAN_SENSITIVITY * pDoc->m_dSphereRad * pDoc->m_dScaleFactor;
	double dPanX = (point.x - m_currentPoint.x) * dFactor;
	double dPanY = (-point.y + m_currentPoint.y) * dFactor;

	double dMat[16] = {0};
	VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dMat));

	VERIFY_GL(glMatrixMode(GL_MODELVIEW));
	VERIFY_GL(glLoadIdentity());

	VERIFY_GL(glTranslated(dPanX, dPanY, 0.0));

	VERIFY_GL(glMultMatrixd(dMat));

	Invalidate(FALSE);

	return;
}

//-----------------------------------------------------------------------------
// FUNC:	ZoomView
// ACTION:	Scales up/down the object in the view using the Mouse wheel
//			

void CGLSampleView::ZoomView( double dScale, CPoint point)
{

	CGLSampleDoc* pDoc = GetDocument();

	if( ( pDoc->m_dScaleFactor / dScale ) > MIN_SCALE_FACTOR )
		pDoc->m_dScaleFactor /=  dScale;

	SetViewVolume();

	Invalidate( FALSE );
	
	return;
}

//-----------------------------------------------------------------------------
// FUNC:	ProximitySelect
// ACTION:	Highlight/Selects the proximity entity.
//			
//void CGLSampleView::ProximitySelect( CPoint point)
//{
//}

//-----------------------------------------------------------------------------
// FUNC:	ModifyEntity
// ACTION:	Modify Selected the proximity entity.
//			
void CGLSampleView::ModifyEntity( CPoint point)
{
	if(m_iCurrentPointID && m_iPickEntityID)
	{
		CGLSampleDoc *pDoc = GetDocument();
		
		CGeomEntity *pEntity = pDoc->GetEntityFromID(m_iPickEntityID);
		
		CGeomPoint *pgptPicked = NULL;

		SetViewVolume();
		CGeomPoint gpt; 				

		double dModel[16] = {0};
		double dProj[16] = {0};
		VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dModel));
		VERIFY_GL(glGetDoublev(GL_PROJECTION_MATRIX, dProj));

		GLint glViewPort[4];
		glGetIntegerv (GL_VIEWPORT, glViewPort);
		CPoint glpoint(point.x, glViewPort[3] - point.y);

		if(pEntity->GetGeometryType() == TL_BSPLINE)
		{
			CBSpline *pBSpline = (CBSpline*) pEntity;

			int iNumPts = (pBSpline->m_iCtrlPt > pBSpline->m_iDataPt)? pBSpline->m_iCtrlPt : pBSpline->m_iDataPt;

			for(int i = 0; i < iNumPts; i++)
			{
				if(pBSpline->m_iCtrlPt >= iNumPts && pBSpline->m_pgptCtrl[i].GetID() == m_iCurrentPointID)
				{
					GLdouble dlWinPoint[3] = {0};
					gluProject(pBSpline->m_pgptCtrl[i].m_Coord[0], pBSpline->m_pgptCtrl[i].m_Coord[1], pBSpline->m_pgptCtrl[i].m_Coord[2], 
						dModel, dProj, glViewPort, &dlWinPoint[0], &dlWinPoint[1], &dlWinPoint[2]);

					gluUnProject(glpoint.x, glpoint.y, dlWinPoint[2], dModel, dProj, glViewPort, &(gpt.m_Coord[0]), &(gpt.m_Coord[1]), &(gpt.m_Coord[2]));
					for(int j = 0; j < 3; j++)
						pBSpline->m_pgptCtrl[i].m_Coord[j] = gpt.m_Coord[j];

					pBSpline->CalculateDataPoints();
					break;
				}
				else if(pBSpline->m_iDataPt >= iNumPts && pBSpline->m_pgptData[i].GetID() == m_iCurrentPointID)
				{
					GLdouble dlWinPoint[3] = {0};
					gluProject(pBSpline->m_pgptData[i].m_Coord[0], pBSpline->m_pgptData[i].m_Coord[1], pBSpline->m_pgptData[i].m_Coord[2], 
						dModel, dProj, glViewPort, &dlWinPoint[0], &dlWinPoint[1], &dlWinPoint[2]);

					gluUnProject(glpoint.x, glpoint.y, dlWinPoint[2], dModel, dProj, glViewPort, &(gpt.m_Coord[0]), &(gpt.m_Coord[1]), &(gpt.m_Coord[2]));
					for(int j = 0; j < 3; j++)
						pBSpline->m_pgptData[i].m_Coord[j] = gpt.m_Coord[j];
					pBSpline->CalculateControlPoints();
					break;
				}
			}

			pBSpline->CalculateBSpline();
		}else if(pEntity->GetGeometryType() == TL_BSURF)
		{
			CBSurf *pBSurf = (CBSurf *) pEntity;
			 
			int iNumPts = pBSurf->GetColumnNumber() * pBSurf->GetRowNumber();

			for(int i = 0; i < iNumPts; i++)
			{
				if(pBSurf->m_pGeomPts[i].GetID() == m_iCurrentPointID)
				{
					GLdouble dlWinPoint[3] = {0};
					gluProject(pBSurf->m_pGeomPts[i].m_Coord[0], pBSurf->m_pGeomPts[i].m_Coord[1], pBSurf->m_pGeomPts[i].m_Coord[2], 
						dModel, dProj, glViewPort, &dlWinPoint[0], &dlWinPoint[1], &dlWinPoint[2]);

					gluUnProject(glpoint.x, glpoint.y, dlWinPoint[2], dModel, dProj, glViewPort, &(gpt.m_Coord[0]), &(gpt.m_Coord[1]), &(gpt.m_Coord[2]));
					for(int j = 0; j < 3; j++)
						pBSurf->m_pGeomPts[i].m_Coord[j] = gpt.m_Coord[j];
					break;
				}
			}
			pBSurf->CalculateBSurface();
		}
	}
	Invalidate(FALSE);
}



void CGLSampleView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_currentOperation = Rotate;

	CView::OnMButtonDown(nFlags, point);
}

void CGLSampleView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_currentOperation = Idle;

	CView::OnMButtonUp(nFlags, point);
}


BOOL CGLSampleView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default

	//g_streamOut << "Zoom ---------\n";
	//WriteToFile();

	double dScaleFactor = -zDelta * ZOOM_SENSITIVITY;

	dScaleFactor = (dScaleFactor < 0) ? -dScaleFactor : 1 / dScaleFactor;

	CGLSampleDoc* pDoc = GetDocument();

	pDoc->m_dScaleFactor /= dScaleFactor;
	SetViewVolume();

	Invalidate(FALSE);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGLSampleView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	Invalidate(FALSE);
}

CGeomPoint CGLSampleView::TransCPointToWorld(CPoint point)
{
	double dModel[16] = {0};
	double dProj[16] = {0};
	VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dModel));
	VERIFY_GL(glGetDoublev(GL_PROJECTION_MATRIX, dProj));

	GLint glViewPort[4];
	glGetIntegerv (GL_VIEWPORT, glViewPort);
	CPoint glpoint(point.x, glViewPort[3] - point.y);
	
	CGeomPoint gpt;
	
	gluUnProject(glpoint.x, glpoint.y, 0, dModel, dProj, glViewPort, &(gpt.m_Coord[0]), &(gpt.m_Coord[1]), &(gpt.m_Coord[2]));

	//CGeomPoint gptTemp = CGeomPoint::CptToGeomPt(glpoint);

	//Matrix M(dModel, 4, 4);
	//Matrix P(dProj, 4, 4);

	//Matrix Mi = M.InverseMat();
	//Matrix Pi = P.InverseMat();

	//Matrix T = P;

	//TransFormEntity(&gptTemp, 1, T, &gptTemp);

	//g_streamOut << "Moved Temp Point:\t" << gptTemp.m_Coord[0] << "\t" << gptTemp.m_Coord[1] << "\t" << gptTemp.m_Coord[2] << "\n";

	return gpt;
}
void CGLSampleView::OnViewFront()
{
	// TODO: Add your command handler code here
	double m[ 16 ];

	VERIFY_GL( glMatrixMode( GL_MODELVIEW ) );
	VERIFY_GL( glGetDoublev( GL_MODELVIEW_MATRIX, m ) );
	VERIFY_GL( glLoadIdentity() );

	Invalidate( FALSE );
}

void CGLSampleView::OnViewFit()
{
	// TODO: Add your command handler code here

	SetBsplineFit(); //Fits the bspline in doc
	SetViewVolume();
	
	Invalidate(FALSE);
}

void CGLSampleView::SetBsplineFit()
{
	CGLSampleDoc* pDoc = GetDocument();
	
	int count = pDoc->GetBSplineCount();

	if( !count )
		return;

	
	CGeomPoint* pgptMinBound = new CGeomPoint[count];
	CGeomPoint* pgptMaxBound = new CGeomPoint[count];

	double* pDx = new double[2*count];
	double* pDy = new double[2*count];
	double* pDz = new double[2*count];

	for(int k = 0; k < count; k++)
	{
		CBSpline *pObjBSpln = pDoc->GetBSpline( k );
		pObjBSpln->GetBoundingBox(pgptMinBound[k], pgptMaxBound[k]);
		pDx[k]		 = pgptMinBound[k].m_Coord[0];
		pDx[k+count] = pgptMaxBound[k].m_Coord[0];
		pDy[k]		 = pgptMinBound[k].m_Coord[1];
		pDy[k+count] = pgptMaxBound[k].m_Coord[1];
		pDz[k]		 = pgptMinBound[k].m_Coord[2];
		pDz[k+count] = pgptMaxBound[k].m_Coord[2];
	}

	if(pgptMaxBound) delete [] pgptMaxBound;
	if(pgptMinBound) delete [] pgptMinBound;

	CGeomPoint gptMaxBound, gptMinBound;

	FindMinMax(pDx, (2*count), gptMaxBound.m_Coord[0], gptMinBound.m_Coord[0]);
	FindMinMax(pDy, (2*count), gptMaxBound.m_Coord[1], gptMinBound.m_Coord[1]);
	FindMinMax(pDz, (2*count), gptMaxBound.m_Coord[2], gptMinBound.m_Coord[2]);

	if(pDx) delete [] pDx;
	if(pDy) delete [] pDy;
	if(pDz) delete [] pDz;
	
	//Find the center point of the Bspines
	CGeomPoint gptCenter;
	gptCenter.m_Dim  = 3;
	for(int i = 0; i < 3; i++)
		gptCenter.m_Coord[i] = (gptMaxBound.m_Coord[i] + gptMinBound.m_Coord[i])/2;


	//Translate it to the Center
	double dMat[16] = {0};
	VERIFY_GL(glGetDoublev(GL_MODELVIEW_MATRIX, dMat));

	VERIFY_GL(glMatrixMode(GL_MODELVIEW));
	VERIFY_GL(glLoadIdentity());

	dMat[12] = 0;
	dMat[13] = 0;
	dMat[14] = 0;

	VERIFY_GL(glMultMatrixd(dMat));
	VERIFY_GL(glTranslated(-gptCenter.m_Coord[0], -gptCenter.m_Coord[1],-gptCenter.m_Coord[2]));

	//Set the Scaling factor
	CGeomPoint gptBound;
	gptBound.m_Dim = 3;
	for(int i = 0; i < 3; i++)
		gptBound.m_Coord[i] = (gptMaxBound.m_Coord[i] - gptMinBound.m_Coord[i]);

	Vector vec(gptBound.m_Coord[0], gptBound.m_Coord[1], gptBound.m_Coord[2]);

	pDoc->m_dSphereRad = vec.VectorDet()/2;
	pDoc->m_dScaleFactor = 1.0;
}

void CGLSampleView::PickSquares(CPoint point)
{
	CGLSampleDoc *pDoc = GetDocument();

	CGeomEntity *pEntity = pDoc->GetEntityFromID(m_iPickEntityID);
	if(m_iPickEntityID)
	{
		if(pEntity) pEntity->SetSelectionStatus(false);

		m_iPickEntityID = NULL;
		m_iCurrentPointID = NULL;
	}
	
	GLuint glSelectBuff[BUFSIZE];
	GLint gliSelected;
	
	GLint glViewPort[4];
	glGetIntegerv (GL_VIEWPORT, glViewPort);
	glSelectBuffer (BUFSIZE, glSelectBuff);

	(void) glRenderMode (GL_SELECT);
	glInitNames();
	glPushName(0);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity ();

	double glMatrix[16] = {0};
	glGetDoublev(GL_PROJECTION_MATRIX, glMatrix);
	/* create 5x5 pixel picking region near cursor location */
	gluPickMatrix ((GLdouble) point.x, (GLdouble) (glViewPort[3] - point.y), 5.0, 5.0, glViewPort);
	

	glGetDoublev(GL_PROJECTION_MATRIX, glMatrix);
	glGetDoublev(GL_MODELVIEW_MATRIX, glMatrix);
	
	double sphereSize = pDoc->m_dSphereRad * 50.0;
	double xextent = double( m_iWinWidth ) / min( double( m_iWinWidth ), double( m_iWinHeight ));
	double yextent = double( m_iWinHeight ) / min( double( m_iWinWidth ), double( m_iWinHeight ));
	glOrtho( -xextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			  xextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			 -yextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			  yextent * pDoc->m_dSphereRad * pDoc->m_dScaleFactor, 
			  sphereSize, -sphereSize);
	glGetDoublev(GL_PROJECTION_MATRIX, glMatrix);
	//SetViewVolume();
	RenderScene(GL_SELECT);
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
	glFlush ();
	gliSelected = glRenderMode (GL_RENDER);

	int iSel = 0; 
	for(int i = 0; i < gliSelected; i++)
	{
		int iNames = glSelectBuff[iSel++]; iSel++; iSel++;
		for(int j = 0; j < iNames; j++)
		{
			unsigned int uiNameID = glSelectBuff[iSel]; iSel++;
			if(iNames == 2 && j == 1)
			{
				m_iCurrentPointID = uiNameID;
				break;
			}
			m_iPickEntityID = uiNameID;
		}	
	}
	
	if(!gliSelected)
	{
		m_iPickEntityID = NULL;
		m_iCurrentPointID = NULL;
	}

	pEntity = pDoc->GetEntityFromID(m_iPickEntityID);

	m_iPickEntityID = glSelectBuff[3];
	if(pEntity)pEntity->SetSelectionStatus(true);
	
	//processHits (hits, selectBuf);
}
void CGLSampleView::OnViewPan()
{
	m_currentOperation = Pan;

	Invalidate(FALSE);
}
