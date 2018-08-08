#pragma once

#include "Vector.h"
#include "GeomEntity.h"

#define BSPLINE_SPANS 500
#define BSURF_U_SPANS 50
#define BSURF_V_SPANS 50

struct COPTIONS
{
	// Grid options
	int m_nGridSize;

	//Circle Data
	double m_dCirRadius;
	Vector vecCirNorm;
    //CGeomPoint m_gptCirCenter;
};

enum eGeometry
{
	TL_POINT = 0,
	TL_LINE,
	TL_BSPLINE,
	TL_BSURF,
    TL_BEAM
};

enum eBSplineMethod
{
	BSPLINE_FAIR = 0,
	BSPLINE_FIT,
	BSPLINE_SWEEP
};

enum eBSplineFitMethod
{
	BSPLINE_FIT_UNIFORMSPACE = 0,
	BSPLINE_FIT_CHORDLENGTH,
	BSPLINE_FIT_CENTRIPETAL
};