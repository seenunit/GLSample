// BSpline.cpp : implementation file
//

#include "stdafx.h"
#include "BSpline.h"
#include <iostream>
#include <fstream>

using namespace std;

// CBSpline

IMPLEMENT_SERIAL( CBSpline, CGeomEntity, 1 )

CBSpline::CBSpline() : CGeomEntity( TL_BSPLINE )
{
    m_pdKnotVec = NULL;
    m_pgptCtrl = NULL;
    m_pgptCurve = NULL;
    m_pgptData = NULL;

    m_bControlPolygon = false;
    m_bFirstDerivatives = false;
    m_bSelected = false;
    m_bHidden = false;
    m_ID = (unsigned int)this;
}

//Constructor for BSPline curve fairing.
CBSpline::CBSpline(int iCtrlPt, int iOrder, int iSpanPt, double *pdParamRange, double *pdKnotVec, CGeomPoint *pgptCtrl, COLORREF clrCurveColor) : CGeomEntity( TL_BSPLINE )
{
    if(iOrder > iCtrlPt)
        iOrder = iCtrlPt;
    m_iCtrlPt = iCtrlPt;
    m_iDataPt = iCtrlPt;
    m_iDegree = iOrder - 1;
    m_iOrder = iOrder;
    m_iSpanPt = iSpanPt;
    SetEntityColor(clrCurveColor);
    m_eBSplineMethod = BSPLINE_FAIR;

    try {
        m_pgptCtrl = new CGeomPoint[m_iCtrlPt];
        m_pgptCurve = new CGeomPoint[m_iSpanPt];
        m_pdKnotVec = new double[ m_iCtrlPt + m_iCtrlPt ];
        m_pgptData = new CGeomPoint[m_iDataPt];

        for( int i = 0; i < m_iCtrlPt; i++ )
            m_pgptCtrl[i]= pgptCtrl[i];
    
        if( pdKnotVec == NULL )
            OpenUniformKnotVector( m_iCtrlPt, m_iOrder, m_pdKnotVec, false );
        else
            for( int i = 0; i < ( iCtrlPt + iOrder ); i++ )
                m_pdKnotVec[i] = pdKnotVec[i];
 
        if( pdParamRange == NULL )
        {
            m_dParamRange[0] = m_pdKnotVec[m_iDegree];
            m_dParamRange[1] = m_pdKnotVec[m_iCtrlPt];
        }
        else
        {
            m_dParamRange[0] = pdParamRange[0];
            m_dParamRange[1] = pdParamRange[1];
        }
        
    }
    catch (bad_alloc xa){
        cout << "Allocation Failure\n";
        exit(EXIT_FAILURE);
    }

    m_eBSplineFitMethod = BSPLINE_FIT_CHORDLENGTH;
    CalculateDataPoints();
    
    //Calculate the Basis functions and the curve points.
    CalculateBSpline();

    ////Parasolid Code
    //ParasolidSessionStart();

    ////vertex
    //double *pdVertex = new double[m_iCtrlPt * THREEDIM];
    //for(int i = 0; i < m_iCtrlPt; i++)
    //{
    //	pdVertex[i*THREEDIM + 0] = m_pgptCtrl[i].m_Coord[0];
    //	pdVertex[i*THREEDIM + 1] = m_pgptCtrl[i].m_Coord[1];
    //	pdVertex[i*THREEDIM + 2] = m_pgptCtrl[i].m_Coord[2];
    //}

    ////multiplicity of knots
    //int *piParaKnotMul = new int[m_iCtrlPt - m_iOrder + 2];
    //for(int i = 0; i < (m_iCtrlPt - m_iOrder + 2); i++)
    //{
    //	if(i == 0 || i == (m_iCtrlPt - m_iOrder + 1))
    //		piParaKnotMul[i] = m_iOrder;
    //	else
    //		piParaKnotMul[i] = 1;
    //}

    ////Knot vector
    //double *pdParaKnot = new double[m_iCtrlPt - m_iOrder + 2];
    //for(int i = 0; i < (m_iCtrlPt - m_iOrder + 2); i++)
    //{		
    //	pdParaKnot[i] = m_pdKnotVec[m_iDegree+i];
    //}

    //PK_BCURVE_sf_t pk_sfBCurve;
    //PK_BCURVE_t pk_BCurve;

    //pk_sfBCurve.degree = m_iDegree;
    //pk_sfBCurve.n_vertices = m_iCtrlPt;
    //pk_sfBCurve.vertex_dim = THREEDIM;
    //pk_sfBCurve.is_rational = PK_LOGICAL_false;
    //pk_sfBCurve.vertex = pdVertex;
    //pk_sfBCurve.form = PK_BCURVE_form_unset_c;
    //pk_sfBCurve.n_knots = (m_iCtrlPt - m_iOrder + 2);
    //pk_sfBCurve.knot_mult = piParaKnotMul;
    //pk_sfBCurve.knot = pdParaKnot;
    //pk_sfBCurve.knot_type = PK_BCURVE_form_unset_c;
    //pk_sfBCurve.is_periodic = PK_LOGICAL_false;
    //pk_sfBCurve.is_closed = PK_LOGICAL_false;
    //pk_sfBCurve.self_intersecting = PK_LOGICAL_false;

    //PK_ERROR_code_t pk_Err = PK_BCURVE_create(&pk_sfBCurve, &pk_BCurve);

    //PK_INTERVAL_t pk_Interval;
    //pk_Err = PK_CURVE_ask_interval(pk_BCurve, &pk_Interval);
    //
    //PK_BODY_t pk_tagBCurveBody;
    //pk_Err = PK_CURVE_make_wire_body(pk_BCurve, pk_Interval, &pk_tagBCurveBody);


    //PK_VECTOR1_t pk_Dir;
    //pk_Dir.coord[0] = 0.0;
    //pk_Dir.coord[1] = 0.0;
    //pk_Dir.coord[2] = -1.0;
    //PK_BODY_extrude_o_t pk_optExtrude;
    //PK_BODY_extrude_o_m(pk_optExtrude);
    //pk_optExtrude.start_bound.forward = PK_LOGICAL_false;
    //pk_optExtrude.start_bound.distance = 50;
    //pk_optExtrude.end_bound.distance = 50;
    //PK_BODY_t pk_BCurExtrude;
    //PK_TOPOL_track_r_t pk_optTopolTrack;
    //PK_TOPOL_local_r_t pk_optTopolLocal;
    //pk_Err = PK_BODY_extrude(pk_tagBCurveBody, pk_Dir, &pk_optExtrude, &pk_BCurExtrude, &pk_optTopolTrack, &pk_optTopolLocal);
    //
    //int iNumFaces = 0;
    //PK_FACE_t *pk_pBCurveFaces = NULL;
    //pk_Err = PK_BODY_ask_faces(pk_BCurExtrude, &iNumFaces, &pk_pBCurveFaces);

    //PK_SURF_t pk_BCurveSurf;
    //pk_Err = PK_FACE_ask_surf(pk_pBCurveFaces[0], &pk_BCurveSurf);

    //pk_Dir.coord[0] = 0.0;
    //pk_Dir.coord[1] = 1.0;
    //pk_Dir.coord[2] = 0.0;
    //PK_BODY_extrude_o_m(pk_optExtrude);
    //pk_optExtrude.end_bound.distance = 50;
    //PK_BODY_t pk_SolidExtrude;
    //pk_Err = PK_BODY_extrude(pk_BCurExtrude, pk_Dir, &pk_optExtrude, &pk_SolidExtrude , &pk_optTopolTrack, &pk_optTopolLocal);
    
    //int iPos = 4;
    //PK_VECTOR_t pk_PosVectors[4] = {0};
    //pk_PosVectors[0].coord[0] = 400;
    //pk_PosVectors[0].coord[1] = 300;
    //pk_PosVectors[0].coord[2] = 0;
    //pk_PosVectors[1].coord[0] = 0;
    //pk_PosVectors[1].coord[1] = 300;
    //pk_PosVectors[1].coord[2] = 0;
    //pk_PosVectors[2].coord[0] = 0;
    //pk_PosVectors[2].coord[1] = -300;
    //pk_PosVectors[2].coord[2] = 0;
    //pk_PosVectors[3].coord[0] = 400;
    //pk_PosVectors[3].coord[1] = -300;
    //pk_PosVectors[3].coord[2] = 0;
    //
    //PK_BODY_create_sheet_planar_o_t pk_optSheetBody;
    //PK_BODY_create_sheet_planar_o_m(pk_optSheetBody);
    //PK_BODY_t pk_PlanarBody;
    //pk_Err = PK_BODY_create_sheet_planar(iPos, pk_PosVectors, &pk_optSheetBody, &pk_PlanarBody);

    //PK_FACE_t *pk_pPlanarFaces = NULL;
    //pk_Err = PK_BODY_ask_faces(pk_PlanarBody, &iNumFaces, &pk_pPlanarFaces);

    //PK_SURF_t pk_planarSurf;
    //pk_Err = PK_FACE_ask_surf(pk_pPlanarFaces[0], &pk_planarSurf);

    //PK_LINE_sf_t pk_SfLine;
    //pk_SfLine.basis_set.axis.coord[0] = 1;
    //pk_SfLine.basis_set.axis.coord[1] = 0;
    //pk_SfLine.basis_set.axis.coord[2] = 0;
    //pk_SfLine.basis_set.location.coord[0] = 0;
    //pk_SfLine.basis_set.location.coord[1] = 300;
    //pk_SfLine.basis_set.location.coord[2] = 0; 
    //PK_LINE_t pk_tagLine;
    //pk_Err = PK_LINE_create(&pk_SfLine, &pk_tagLine);

    //PK_SWEPT_sf_t pk_SfLineSwept;
    //pk_SfLineSwept.curve = pk_tagLine;
    //pk_SfLineSwept.direction.coord[0] = 0;
    //pk_SfLineSwept.direction.coord[1] = -1;
    //pk_SfLineSwept.direction.coord[2] = 0;
    //PK_SWEPT_t pk_tagLineSwept;
    //pk_Err = PK_SWEPT_create(&pk_SfLineSwept, &pk_tagLineSwept);

    //PK_UVBOX_t pk_UVBox;
    ////pk_UVBox.param[0] = 0;
    ////pk_UVBox.param[1] = 400;
    ////pk_UVBox.param[2] = 0;
    ////pk_UVBox.param[3] = -300;
    //pk_Err = PK_SURF_ask_uvbox(pk_tagLineSwept, &pk_UVBox);

    //PK_BSURF_t pk_tagPlaneBsurf;
    //PK_LOGICAL_t pk_logicalExact;
    //pk_Err = PK_SURF_make_bsurf(pk_tagLineSwept, pk_UVBox, PK_LOGICAL_true, PK_LOGICAL_true, 0.00001, &pk_tagPlaneBsurf, &pk_logicalExact);

    //pk_Err = PK_PART_add_geoms(pk_BCurExtrude, 1, &pk_tagPlaneBsurf); // adding planar bsurface to bcurve extrude body.

    //PK_SURF_intersect_surf_o_t pk_optIntersectSurf;
    //PK_SURF_intersect_surf_o_m(pk_optIntersectSurf);
    //int iIntersectPos;
    //PK_VECTOR_t *pk_pIntersectVector = NULL;
    //int iIntersectCur;
    //PK_CURVE_t *pk_pIntersectCurve = NULL;
    //PK_INTERVAL_t *pk_pCurveBounds = NULL;
    //PK_intersect_curve_t *pk_pCurvesType = NULL;

    //pk_Err = PK_SURF_intersect_surf( pk_tagPlaneBsurf, pk_BCurveSurf, &pk_optIntersectSurf, &iIntersectPos, &pk_pIntersectVector, &iIntersectCur, &pk_pIntersectCurve , &pk_pCurveBounds, &pk_pCurvesType);

    //int iICurveIndex = 0;
    //PK_CURVE_t *pk_ICUrves = new PK_CURVE_t[iIntersectCur];

    //for(int i = 0; i < iIntersectCur; i++)
    //{
    //	PK_CLASS_t pk_ClassType;
    //	pk_Err = PK_ENTITY_ask_class(pk_pIntersectCurve[i], &pk_ClassType);

    //	if(pk_ClassType == PK_CLASS_icurve)
    //	{
    //		pk_ICUrves[iICurveIndex] = pk_pIntersectCurve[i];
    //		PK_INTERVAL_t pk_ICurveInterval;
    //		pk_Err = PK_CURVE_ask_interval(pk_pIntersectCurve[i], &pk_ICurveInterval);
    //		PK_PARAM_sf_t pk_SfICurParam; 
    //		pk_Err = PK_CURVE_ask_param(pk_pIntersectCurve[i], &pk_SfICurParam);
    //		iICurveIndex++;
    //	}
    //}

    
    //PK_PART_transmit_o_t pk_optPartTransmit;
    //PK_PART_transmit_o_m(pk_optPartTransmit);
    //int iNumBody = 1;
    //PK_BODY_t *pk_pBody = new PK_BODY_t[iNumBody];
    //pk_pBody[0] = pk_tagBCurveBody; //pk_BCurExtrude;
    //pk_Err = PK_PART_transmit(iNumBody, pk_pBody, "E:\\Parasolid Files\\GLSample\\ICurve", &pk_optPartTransmit);

    //int iNumEdges = 0;
    //PK_EDGE_t *pk_pEdges = NULL;
    //pk_Err = PK_BODY_ask_edges(pk_SolidExtrude, &iNumEdges, &pk_pEdges);

    //int iICurveIndex = 0;
    //PK_CURVE_t *pk_ICUrves = new PK_CURVE_t[iNumEdges];

    //for(int i = 0; i < iNumEdges; i++)
    //{
    //	PK_CURVE_t pk_EdgeCurve;
    //	pk_Err = PK_EDGE_ask_curve(pk_pEdges[i], &pk_EdgeCurve);
    //	PK_CLASS_t pk_ClassType;
    //	pk_Err = PK_ENTITY_ask_class(pk_EdgeCurve, &pk_ClassType);

    //	if(pk_ClassType == PK_CLASS_icurve)
    //	{
    //		pk_ICUrves[iICurveIndex] = pk_EdgeCurve;
    //	}
    //}

    
    //ParasolidSessionStop();

    ////free bcurve memory
    //if(pdVertex) delete [] pdVertex;
    //if(piParaKnotMul) delete [] piParaKnotMul;
    //if(pdParaKnot) delete [] pdParaKnot;

    //settings
    m_bControlPolygon = false;
    m_bFirstDerivatives = false;
    m_bSelected = false;
    m_bHidden = false;
    m_ID = (unsigned int)this;
}

//Constructor for BSpline Curve-Fitting
CBSpline::CBSpline( int nCtrlPt, int iOrder, int nSpanPt, int nDataPt, eBSplineFitMethod methodBsplineFit, double *pdParamRange, double *pdKnotVec, CGeomPoint *pgptData, COLORREF clrCurveColor ) : CGeomEntity( TL_BSPLINE )
{
    if(nCtrlPt > nDataPt)//no. of control pts must be less than are equal to no. data points n <= j
        nCtrlPt = nDataPt;

    m_iCtrlPt = nCtrlPt;
    
    if(iOrder > m_iCtrlPt) //order must be less than are equal to no.of control(polygon pts) k <= n
        iOrder = nCtrlPt;
    
    m_iOrder = iOrder;
    m_iDegree = iOrder - 1;
    m_iSpanPt = nSpanPt;
    m_iDataPt = nDataPt;

    m_eBSplineMethod = BSPLINE_FIT;
    m_eBSplineFitMethod = methodBsplineFit;

    m_clrCurveColor = clrCurveColor;

    try {
        m_pgptCtrl = new CGeomPoint[m_iCtrlPt];
        m_pgptCurve = new CGeomPoint[m_iSpanPt];
        m_pgptData = new CGeomPoint[m_iDataPt];
        m_pdKnotVec = new double[ m_iCtrlPt + m_iCtrlPt ];
    }
    catch (bad_alloc xa){
        cout << "Allocation Failure\n";
        exit(EXIT_FAILURE);
    }

    for( int i = 0; i < m_iDataPt; i++ )
        m_pgptData[i]= pgptData[i];

    //knot vector must be normalised one
    //OpenUniformKnotVector( m_iCtrlPt, m_iOrder, m_pdKnotVec, true );
    if( pdKnotVec == NULL )
        OpenUniformKnotVector( m_iCtrlPt, m_iOrder, m_pdKnotVec, false );
    else
        for( int i = 0; i < ( nCtrlPt + iOrder ); i++ )
            m_pdKnotVec[i] = pdKnotVec[i];

    //set the parameter Range
    if( pdParamRange == NULL )
    {
        m_dParamRange[0] = m_pdKnotVec[m_iDegree];
        m_dParamRange[1] = m_pdKnotVec[m_iCtrlPt];
    }
    else
    {
        m_dParamRange[0] = pdParamRange[0];
        m_dParamRange[1] = pdParamRange[1];
    }

    CalculateControlPoints();

    CalculateBSpline();

    //Parasolid Code
    //ParasolidSessionStart();

    ////vertex
    //double *pdVertex = new double[m_iCtrlPt * THREEDIM];
    //for(int i = 0; i < m_iCtrlPt; i++)
    //{
    //	pdVertex[i*THREEDIM + 0] = m_pgptCtrl[i].m_Coord[0];
    //	pdVertex[i*THREEDIM + 1] = m_pgptCtrl[i].m_Coord[1];
    //	pdVertex[i*THREEDIM + 2] = m_pgptCtrl[i].m_Coord[2];
    //}

    ////multiplicity of knots
    //int *piParaKnotMul = new int[m_iCtrlPt - m_iOrder + 2];
    //for(int i = 0; i < (m_iCtrlPt - m_iOrder + 2); i++)
    //{
    //	if(i == 0 || i == (m_iCtrlPt - m_iOrder + 1))
    //		piParaKnotMul[i] = m_iOrder;
    //	else
    //		piParaKnotMul[i] = 1;
    //}

    ////Knot vector
    //double *pdParaKnot = new double[m_iCtrlPt - m_iOrder + 2];
    //for(int i = 0; i < (m_iCtrlPt - m_iOrder + 2); i++)
    //{		
    //	pdParaKnot[i] = m_pdKnotVec[m_iDegree+i];
    //}

    //PK_BCURVE_sf_t pk_sfBCurve;
    //PK_BCURVE_t pk_BCurve;

    //pk_sfBCurve.degree = m_iDegree;
    //pk_sfBCurve.n_vertices = m_iCtrlPt;
    //pk_sfBCurve.vertex_dim = THREEDIM;
    //pk_sfBCurve.is_rational = PK_LOGICAL_false;
    //pk_sfBCurve.vertex = pdVertex;
    //pk_sfBCurve.form = PK_BCURVE_form_unset_c;
    //pk_sfBCurve.n_knots = (m_iCtrlPt - m_iOrder + 2);
    //pk_sfBCurve.knot_mult = piParaKnotMul;
    //pk_sfBCurve.knot = pdParaKnot;
    //pk_sfBCurve.knot_type = PK_BCURVE_form_unset_c;
    //pk_sfBCurve.is_periodic = PK_LOGICAL_false;
    //pk_sfBCurve.is_closed = PK_LOGICAL_false;
    //pk_sfBCurve.self_intersecting = PK_LOGICAL_false;

    //PK_ERROR_code_t pk_Err = PK_BCURVE_create(&pk_sfBCurve, &pk_BCurve);

    //PK_INTERVAL_t pk_Interval;
    //pk_Err = PK_CURVE_ask_interval(pk_BCurve, &pk_Interval);
    //
    //PK_BODY_t pk_tagBCurveBody;
    //pk_Err = PK_CURVE_make_wire_body(pk_BCurve, pk_Interval, &pk_tagBCurveBody);

    //// Code To Transmit Part To File
    //PK_PART_transmit_o_t pk_optPartTransmit;
    //PK_PART_transmit_o_m(pk_optPartTransmit);
    //pk_Err = PK_PART_transmit(1, &pk_tagBCurveBody, "E:\\Parasolid Files\\GLSample\\BSpline", &pk_optPartTransmit);

    //// Code To Receive A Part From A File
    //char cPSFile[_MAX_PATH] = "E:\\ODXPS_Work\\16072009 Icurve details\\1\\ICurve.xmt_txt"; 
    //char *pcPSExt = strrchr(cPSFile, '.');
    //
    //PK_PART_receive_o_t pk_optPartRecieve;
    //PK_PART_receive_o_m(pk_optPartRecieve);
    //if( !_stricmp(pcPSExt, ".X_T") || !_stricmp(pcPSExt, ".XMT_TXT") )
    //	pk_optPartRecieve.transmit_format = PK_transmit_format_text_c;
    //else if( !_stricmp(pcPSExt, ".X_B") || !_stricmp(pcPSExt, ".XMT_BIN") )
    //	pk_optPartRecieve.transmit_format = PK_transmit_format_neutral_c;

    //pcPSExt[0] = '\0';
    //
    //int iParts = 0;
    //PK_PART_t *pk_pParts = NULL;
    //pk_Err = PK_PART_receive(cPSFile, &pk_optPartRecieve, &iParts, &pk_pParts);

    //// Enquiry Of The Part Received
    //int iEdges = 0;
    //PK_EDGE_t *pk_pEdges = NULL;
    //pk_Err = PK_BODY_ask_edges( pk_pParts[0], &iEdges, &pk_pEdges);

    //PK_CURVE_t pk_Curve;
    //pk_Err = PK_EDGE_ask_curve(pk_pEdges[0], &pk_Curve);

    //pk_Err = PK_CURVE_ask_interval(pk_Curve, &pk_Interval);

    //PK_BCURVE_t pk_BCurveTransf;
    //PK_LOGICAL_t pk_bLogical;
    //pk_Err = PK_CURVE_make_bcurve(pk_Curve, pk_Interval, PK_LOGICAL_false, PK_LOGICAL_false, .00001, &pk_BCurveTransf, &pk_bLogical);

    //pk_Err = PK_GEOM_is_coincident(pk_BCurve, pk_BCurveTransf, &pk_bLogical);

    //PK_GEOM_range_o_t pk_optGeomRange;
    //PK_GEOM_range_o_m(pk_optGeomRange);

    //pk_optGeomRange.have_tolerance = PK_LOGICAL_true;
    //pk_optGeomRange.tolerance = .0001;
    //pk_optGeomRange.range_type = PK_range_type_maximum_c;
    //
    //PK_range_result_t pk_RangeResult;
    //PK_range_2_r_t pk_RangeData;
    //
    //pk_Err = PK_GEOM_range(pk_BCurve, pk_BCurveTransf, &pk_optGeomRange, &pk_RangeResult, &pk_RangeData);

    ////free bcurve memory
    //if(pdVertex) delete [] pdVertex;
    //if(piParaKnotMul) delete [] piParaKnotMul;
    //if(pdParaKnot) delete [] pdParaKnot;

    //ParasolidSessionStop();

    //settings
    m_bControlPolygon = false;
    m_bFirstDerivatives = false;
    m_bSelected = false;
    m_bHidden = false;
    m_ID = (unsigned int)this;
}

CBSpline::~CBSpline()
{
    if( m_pgptCtrl ) delete []m_pgptCtrl;
    if( m_pgptCurve ) delete []m_pgptCurve;
    if( m_pdKnotVec ) delete []m_pdKnotVec;
    if(m_pgptData) delete []m_pgptData;
}

void CBSpline::CalculateBSpline()
{
    BSpline_btParam( m_iCtrlPt, m_iOrder, m_iSpanPt, m_dParamRange, m_pdKnotVec, m_pgptCtrl, m_pgptCurve, m_iCurvePts );
}

void CBSpline::CalculateDataPoints()
{
    GetBSplineDataPoints(m_iCtrlPt, m_iOrder, m_dParamRange, m_pdKnotVec, m_eBSplineFitMethod, m_pgptCtrl, m_pgptData);
}

void CBSpline::CalculateControlPoints()
{
    //Get the control points from data points
    BSplineFit(m_iCtrlPt, m_iOrder, m_dParamRange, m_pdKnotVec, m_iDataPt, m_eBSplineFitMethod, m_pgptData, m_pgptCtrl);

    //Get the Control points from Data points and end Tangents.
    //BSplineFitEndTangents(m_iCtrlPt, m_iOrder, m_dParamRange, m_pdKnotVec, m_iDataPt, m_eBSplineMethod, m_pgptData, m_pgptCtrl);
}

//BSpline Serialization
void CBSpline::Serialize(CArchive& ar)
{
    CGeomEntity::Serialize( ar );

    if (ar.IsStoring())	{	// storing code
        ar << m_iCtrlPt << m_iOrder << m_iDegree << m_iSpanPt;
        for( int i = 0; i < m_iCtrlPt; i++ )
            m_pgptCtrl[i].Serialize( ar );
    }
    else {	// loading code
        ar >> m_iCtrlPt >> m_iOrder >> m_iDegree >> m_iSpanPt;
        try {
            m_pgptCtrl = new CGeomPoint[m_iCtrlPt];
            m_pgptCurve = new CGeomPoint[m_iSpanPt];
            m_pdKnotVec = new double[ m_iCtrlPt + m_iOrder ];
        }
        catch (bad_alloc xa){
            cout << "Allocation Failure\n";
            exit(EXIT_FAILURE);
        }

        for( int i = 0; i < m_iCtrlPt; i++ )
            m_pgptCtrl[i].Serialize( ar );
    }
}

void CBSpline::Draw( )
{
    if(m_bHidden)
        return;

    glColor3d(GetRValue(m_clrCurveColor)/255.0, GetGValue(m_clrCurveColor)/255.0, GetBValue(m_clrCurveColor)/255.0);

    if(m_bSelected)
        glColor3d(1,0,0);
    
    //glEnable(GL_LINE_SMOOTH);//anti-alias

    //Bspline Curve
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < m_iCurvePts; i++)
        glVertex3d(m_pgptCurve[i].m_Coord[0], m_pgptCurve[i].m_Coord[1], m_pgptCurve[i].m_Coord[2]);
    glEnd();

    //glDisable(GL_LINE_SMOOTH);//anti-alias

    //Knot Points
    CGeomPoint *pgptKnotPt = new CGeomPoint[ m_iCtrlPt + m_iOrder ];
    GetBSplineKnotPoints( m_iCtrlPt, m_iOrder, m_pdKnotVec, m_pgptCtrl, pgptKnotPt );
    double  dSize = 0.05;
    DrawPoints((m_iCtrlPt+ m_iOrder), dSize, pgptKnotPt);

    //Draw Control points
    glColor3d(0.078, .4, .57);
    DrawPoints(m_iCtrlPt, dSize, m_pgptCtrl);

    glColor3d(0, .3, .6);
    dSize = 0.09;
    DrawPoints(m_iDataPt, dSize, m_pgptData);

    if(m_bControlPolygon)
    {
        glLineWidth(2.0);

        glColor3d(0.078, .4, .57);
        //Control polygon
        glBegin(GL_LINE_STRIP);
        for(int i = 0; i < m_iCtrlPt; i++)
            glVertex3d(m_pgptCtrl[i].m_Coord[0], m_pgptCtrl[i].m_Coord[1], m_pgptCtrl[i].m_Coord[2]);
        glEnd();

        glLineWidth(1.0);
    }

    if(m_bFirstDerivatives)
    {
        glColor3d(.5, 1, 0);
        glBegin(GL_LINES);
        for( int i = 0; i < ( m_iCtrlPt+m_iOrder ); i++ )
        {
            double t = m_pdKnotVec[i];
            if(!IsPeriodic(m_iCtrlPt, m_iOrder, m_pdKnotVec))
            {
                if(i >= m_iCtrlPt)
                    t = t * 0.999999999;
            }
            else if(i == (m_iCtrlPt+m_iOrder-1))
                t = t * 0.999999999;

            double dVector[3] = {0};
            BSplineFirstDervativeAtParam(t, m_iCtrlPt, m_iOrder, m_pdKnotVec, m_pgptCtrl, dVector);
            Vector vStart(pgptKnotPt[i].m_Coord[0], pgptKnotPt[i].m_Coord[1], pgptKnotPt[i].m_Coord[2]);
            Vector vTangent(dVector[0], dVector[1], dVector[2]);
            t = vTangent.VectorDet();
            Vector vDir = vTangent.UnitVector();
            Vector vEnd = vStart + vDir * t;

            glVertex3d(vStart.getx(), vStart.gety(), vStart.getz());
            glVertex3d(vEnd.getx(), vEnd.gety(), vEnd.getz());
        }
        glEnd();
    }

    if(pgptKnotPt) delete []pgptKnotPt;
}

void CBSpline::GetBSplineCtrlPt( CGeomPoint *pgptCtrlPt )
{
    for( int i = 0; i < m_iCtrlPt; i++ )
        pgptCtrlPt[i] = m_pgptCtrl[i];
}

#define TOL 1e-6

void CBSpline::GetBoundingBox(CGeomPoint &gptMinBound, CGeomPoint &gptMaxBound)
{
    double t = m_pdKnotVec[m_iCtrlPt] * 0.999999999999999;

    double dVector[3] = {1, 1, 1};

    int i = 0;

    // x maximum.
    while( dVector[1] > TOL )
    {
        BSplineFirstDervativeAtParam(t, m_iCtrlPt, m_iOrder, m_pdKnotVec, m_pgptCtrl, dVector);
        double dDVector[3] = {0};
        BSplineSecondDervativeAtParam(t, m_iCtrlPt, m_iOrder, m_pdKnotVec, m_pgptCtrl, dDVector);
        if(dDVector[1] != 0.0)
            t = t - (dVector[1]/dDVector[1]);
        BSplineFirstDervativeAtParam(t, m_iCtrlPt, m_iOrder, m_pdKnotVec, m_pgptCtrl, dVector);
        i++;
    }

    double *pdX = new double[m_iCtrlPt];
    double *pdY = new double[m_iCtrlPt];
    double *pdZ = new double[m_iCtrlPt];
    for( int i = 0; i < m_iCtrlPt; i++ )
    {
        pdX[i] = m_pgptCtrl[i].m_Coord[0]; 
        pdY[i] = m_pgptCtrl[i].m_Coord[1]; 
        pdZ[i] = m_pgptCtrl[i].m_Coord[2]; 
    }
    FindMinMax( pdX, m_iCtrlPt, gptMaxBound.m_Coord[0], gptMinBound.m_Coord[0] );
    FindMinMax( pdY, m_iCtrlPt, gptMaxBound.m_Coord[1], gptMinBound.m_Coord[1] );
    FindMinMax( pdZ, m_iCtrlPt, gptMaxBound.m_Coord[2], gptMinBound.m_Coord[2] );

    if( pdX ) delete [] pdX;
    if( pdY ) delete [] pdY;
    if( pdZ ) delete [] pdZ;
}


void CBSpline::SetControlPolygonStatus(bool bShow)
{
    m_bControlPolygon = bShow;
}

bool CBSpline::GetControlPolygonStatus()
{
    return m_bControlPolygon;
}

void CBSpline::SetTangentVectorStatus(bool bShow)
{
    m_bFirstDerivatives = bShow;
}

bool CBSpline::GetTangentVectorStatus()
{
    return m_bFirstDerivatives;
}


//void CBSpline::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//}
//BEGIN_MESSAGE_MAP(CBSpline, CDialog)
//END_MESSAGE_MAP()


// CBSpline message handlers


//BSpline routines

double DistanceBetweenPoints(double* pdFirst, double* pdEnd)
{
    double dist = 0.0;

    dist  = sqrt(		(pdEnd[0]-pdFirst[0])*(pdEnd[0]-pdFirst[0])+ 
                        (pdEnd[1]-pdFirst[1])*(pdEnd[1]-pdFirst[1])+ 
                        (pdEnd[2]-pdFirst[2])*(pdEnd[2]-pdFirst[2]));

    //dist = sqrt(sqr(pdEnd[0]-pdFirst[0])+ sqr(pdEnd[1]-pdFirst[1])+ sqr(pdEnd[2]-pdFirst[2]));

    return dist;
}

// Finding Min, Max from given array using (bubble) sorting method.
void FindMinMax( double * const pdSourceArr, int iSize, double &dMaxValue, double &dMinValue )
{
    double *pdArr = new double[iSize];

    for(int i = 0; i < iSize; i++)
        pdArr[i] = pdSourceArr[i];

    //int iSize = sizeof pdArr / sizeof( double );
    double dTemp = 0.0;
    for( int i = 0; i < iSize-1; ++i )
    {
       for( int j = i+1; j < iSize; ++j )
       {
           if( pdArr[i] >  pdArr[j])
           {
               dTemp = pdArr[i];
               pdArr[i] = pdArr[j];
               pdArr[j] = dTemp;
           }
       }
    }

    dMinValue = pdArr[0] ;
    dMaxValue = pdArr[iSize-1];	

    if(pdArr) delete [] pdArr;
}
void bspline_i( int n/*iCtrlPt*/, int k/*iOrder*/, int nSpanPt, double *pdKontVec, CGeomPoint *pdCtrlPt, CGeomPoint *pdCurvePt, int &iCurvePt )
{
    Matrix mCtrPts( n, THREEDIM );
    
    for( int i = 0; i < n; i++ )
    {
        //mCtrPts( i, 0 ) = pdCtrlPt[i].x;
        //mCtrPts( i, 1 ) = pdCtrlPt[i].y;
        //mCtrPts( i, 2 ) = pdCtrlPt[i].z;
        for( int j = 0; j < THREEDIM; j++ )
            mCtrPts( i, j ) = pdCtrlPt[i].m_Coord[j];
    }	

    double *X = new double[n + k + 1 ];
    for( int i = 1; i < n+k+1; i++ )
        X[i] = pdKontVec[i-1];

    Matrix N( ( n + k /*-1*/ ), k +1 );
    iCurvePt = 0;
    double d = ( X[ n+k /*-1*/]-X[1] )/( float )( nSpanPt-1 );
    for( double t = X[1/*k*/]; t <= X[n+k/*n+1*/]; t += d )
    {
        for( int i = 1; i < ( n + k ); i++ )
        {
            if(  X[i] <= t && t < X[i+1] )
                N( i, 1 ) = 1;
            else 
                N( i, 1 ) = 0;
        }

        for( int j = 2; j <= k; j++ )
        {
            for( int i = 1; i < (n + k ); i++ )
            {
                if( (i + j) <= ( n + k ) )
                {
                    if( (X[i+j-1] - X[i]) != 0 )
                    {
                        if( (X[i+j] - X[i+1]) != 0 )
                            N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i])) + ((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                        else				
                            N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i]));
                    }
                    else if( (X[i+j] - X[i+1]) != 0 )
                            N( i, j ) =	((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                    else
                        N( i, j ) = 0;
                }
                else N( i, j ) = 0;
            }
        }
        Matrix M( 1, n );
        for( int i = 0; i < n; i++ )
            M( 0, i ) = N( i +1, k/*-1*/ );
        Matrix mPoint = M*mCtrPts;
    //		pdCurvePt[nCount].x = dPoint( 0, 0);
    //		pdCurvePt[nCount].y = dPoint( 0, 1);
    //		pdCurvePt[nCount].z = dPoint( 0, 2);
        for( int j = 0; j < THREEDIM; j++ )
            pdCurvePt[iCurvePt].m_Coord[j] = mPoint( 0, j );
        iCurvePt++;	
    }

    if( X ) delete [] X;
}


void BSpline_btParam( int n/*iCtrlPt*/, int k/*iOrder*/, int iSpanPt, double *pdParamRange, double *pdKontVec, CGeomPoint *pgptCtrl, CGeomPoint *pgptCurve, int &iCurvePt )
{
    Matrix mCtrPts( n, THREEDIM );
    for( int i = 0; i < n; i++ )
        for( int j = 0; j < THREEDIM; j++ )
            mCtrPts( i, j ) = pgptCtrl[i].m_Coord[j];

    double *pdBSplnBlends = new double[n];
    double d = ( pdParamRange[1]-pdParamRange[0] )/( float )( iSpanPt-1 );
    iCurvePt = 0;

    //Output file
    //std::ofstream streamOut("C:\\Documents and Settings\\srinivasreddy\\Desktop\\Bspline.txt", std::ios_base::app);
    //static int i = 0;
    //streamOut << "Bspline :" << i++;
    //streamOut.precision(13);
    //streamOut.width(20);
        
    for( double t = pdParamRange[0]; t < pdParamRange[1]; t += d )
    {
        BSplineBlends(t, n, k, pdKontVec, pdBSplnBlends );
        
        Matrix M( 1, n );
        
        for( int i = 0; i < n; i++ )
                M( 0, i ) = pdBSplnBlends[i];
        
        Matrix mPoint = M*mCtrPts;
        
        for( int j = 0; j < THREEDIM; j++ )
            pgptCurve[iCurvePt].m_Coord[j] = mPoint( 0, j );

        ////Output file
        //streamOut << pgptCurve[iCurvePt].m_Coord[0] << "\t" << pgptCurve[iCurvePt].m_Coord[1] <<  "\t" << pgptCurve[iCurvePt].m_Coord[2] << "\tat\t" << t << "\n";
        
        iCurvePt++;
    }

    ////Output file
    //streamOut.close();

    if( pdBSplnBlends ) delete [] pdBSplnBlends;
}

void BSplineBlends( double t/*dParam*/, int n/*iCtrlPt*/, int k/*iOrder*/, double *pdKontVec, double *pdBSplnBlends )
{
    double *X = new double[n + k + 1 ];
    for( int i = 1; i < n+k+1; i++ )
        X[i] = pdKontVec[i-1];

    Matrix N( ( n + k ), k + 1 );

    for( int i = 1; i < ( n + k ); i++ )
    {
        if( X[i] <= t && t < X[i+1] )
            N( i, 1 ) = 1;
        else 
            N( i, 1 ) = 0;
    }

    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i])) + ((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                    else				
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) =	((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                else
                    N( i, j ) = 0;
            }
            else N( i, j ) = 0;
        }
    }

    for( int i = 0; i < n; i++ )
        pdBSplnBlends[i] = N( i +1, k );

    if( X ) delete [] X;
}

int NURBSFindKnotSpan(int nKnots, double *knots, int degree, double u,
                                  int &knot_span)
{
    if( u == knots[degree] )
    {
        knot_span = degree;
        return 0;
    }
    if( u == knots[nKnots-degree-1] )
    {	
        knot_span = nKnots-degree-2;
        return 0;
    }

    int low,high,mid;
    low  = degree;
    high = nKnots-1-degree;
    mid  = (low+high)/2;

    int mid_prev=0;
    while( u < knots[mid]  ||  u >= knots[mid+1] )
    { 
        if( u < knots[mid] )  
            high = mid; 
        else low = mid;
        mid = (low+high)/2;
        if(mid == mid_prev)
            break;
        else
            mid_prev = mid;
    }
    knot_span = mid;
    
    return 1;
}


void BSplineBlends_1( double t/*dParam*/, int n/*iCtrlPt*/, int k/*iOrder*/, double *pdKontVec, double *pdBSplnBlends )
{
    double *knots = pdKontVec;

    int iDegree = k - 1;
    int iKnots = n + k;
    int iKnotSpans = iKnots - 1;
    int iSpan;

    double *N = new double[k];

    int i, l, j;
    for(i = 0; i < iKnotSpans; i++)
    {
        if(t >= knots[i] && t < knots[i+1])
        {
            iSpan = i;
            break;
        }
    }

    double *left = new double [k];
    double *right =new double[k];
    double saved = 0;
    double temp = 0;
    i = iSpan;
    N[0] = 1.0;
    for(j = 1; j <= iDegree; j++)
    {
        left[j]  = t-knots[i+1-j]; 
        right[j] = knots[i+j]-t;
        saved    = 0.0;
        for(l = 0; l < j; l++)
        {
            temp  = N[l]/(right[l+1]+left[j-l]);
            N[l]  = saved + right[l+1]*temp;
            saved = left[j-l]*temp;
        }
        N[j] = saved;
    } 
    delete [] left;
    delete [] right;
    double alpha=0.0;
    j = iSpan-iDegree;
    for (i = j; i <= iSpan; i++ )
    {
        pdBSplnBlends[i] = N[i-j];
    }

    delete [] N;
}

void BSplineBlends_2( double u/*dParam*/, int iCtrlPt, int iOrder, double *pdKontVec, double *pdBSplnBlends, int iDervs, double **pdBlendDervs )
{
    double *pdK = pdKontVec;

    int iDegree = iOrder - 1;
    int iKnots = iCtrlPt + iOrder;
    int iKnotSpans = iKnots - 1;
    int iSpan = 0;

    double **N = new double*[iOrder];
    for(int i = 0; i < iOrder; i++)
        N[i] = new double[iCtrlPt];

    for(int i = 0; i < iOrder; i++)
        for(int j = 0; j < iCtrlPt; j++)
            N[i][j] = 0.0;

    for(int k = 0; k < iKnotSpans; k++)
    {
        if(u >= pdK[k] && u < pdK[k+1])
        {
            iSpan = k;
            N[0][iSpan] = 1.0;
            break;
        }
    }

    for(int d = 1; d <= iDegree; d++)
    {
        N[d][iSpan-d] = (pdK[iSpan+1]-u)/(pdK[iSpan+1]-pdK[(iSpan-d)+1])*N[d-1][(iSpan-d)+1];
        for(int i = iSpan-d+1; i <= (iSpan-1); i++)
        {
            N[d][i] = (u-pdK[i])/(pdK[i+d]-pdK[i])*N[d-1][i] + (pdK[i+d+1]-u)/(pdK[i+d+1]-pdK[i+1])*N[d-1][i+1];
        }
        N[d][iSpan] = (u-pdK[iSpan])/(pdK[iSpan+d]-pdK[iSpan])*N[d-1][iSpan];		
    }

    //Get maximum derivative index and set zero derivatives
    iDervs = ( iDegree < iDervs? iOrder : ++iDervs);
    double **dN = new double*[iDervs];

    for(int i = 0; i < iDervs; i++)
        dN[i] = new double[iCtrlPt];

    for(int i = 0; i < iDervs; i++)
        for(int j = 0; j < iCtrlPt; j++)
            dN[i][j] = 0.0;

    for(int i = 0; i < iCtrlPt; i++ )
    {
        dN[0][i] = N[iDegree][i];
        if(pdBSplnBlends) pdBSplnBlends[i] = N[iDegree][i];
        if(pdBlendDervs) pdBlendDervs[0][i] = N[iDegree][i];
    }

    for(int d = 1; d < iDervs; d++)
    {
        for(int k = iDegree; k > 0; k--)
        {
            N[k][iSpan-k] = k/(pdK[iSpan+1]-pdK[(iSpan-k)+1])*N[k-1][(iSpan-k)+1];
            for(int i = iSpan-k+1; i <= (iSpan-1); i++)
            {
                N[k][i] = k/(pdK[i+k]-pdK[i])*N[k-1][i] + k/(pdK[i+k+1]-pdK[i+1])*N[k-1][i+1];
            }
            N[k][iSpan] = k/(pdK[iSpan+k]-pdK[iSpan])*N[k-1][iSpan];
        }
        for(int i = 0; i < iCtrlPt; i++ )
        {
            dN[d][i] = N[iDegree][i];
            if(pdBlendDervs[d]) pdBlendDervs[d][i] = N[iDegree][i];
        }
    }	

    if(dN)
    {
        for(int i = 0; i < iDervs; i++)
            delete [] dN[i];
        delete [] dN;
    }

    if(N) 
    {
        for(int i = 0; i < iOrder; i++)
            delete [] N[i];
        delete [] N;
    }	
}

void GetBSplineKnotPoints( int iCtrlPt, int iOrder, double  *pdKnotVec, CGeomPoint *pgptCtrlPt, CGeomPoint *pgptKnotPt )
{
    double *pdBSplnBlends = new double[ iCtrlPt ];

    Matrix mCtrPts( iCtrlPt, THREEDIM );

    for( int i = 0; i < iCtrlPt; i++ )
        for( int j = 0; j < THREEDIM; j++ )
            mCtrPts( i, j ) = pgptCtrlPt[i].m_Coord[j];

    for( int i = 0; i < ( iCtrlPt+iOrder ); i++ )
    {
        double t = pdKnotVec[i];
        if(!IsPeriodic(iCtrlPt, iOrder, pdKnotVec))
        {
            if(i >= iCtrlPt)
                t = t * 0.999999999;
        }
        else if(i == (iCtrlPt+iOrder-1))
            t = t * 0.999999999;
        BSplineBlends( t, iCtrlPt, iOrder, pdKnotVec, pdBSplnBlends );
        
        Matrix M( 1, iCtrlPt );

        for( int l = 0; l < iCtrlPt; l++ )
            M( 0, l ) = pdBSplnBlends[l];
        
        Matrix mPoint = M*mCtrPts;
        
        for( int j = 0; j < THREEDIM; j++ )
            pgptKnotPt[i].m_Coord[j] = mPoint( 0, j );
    }

    if( pdBSplnBlends ) delete [] pdBSplnBlends; 
}

void GetBSplineDataPoints( int iCtrlPt, int iOrder, double *pdParamRange, double  *pdKnotVec, eBSplineFitMethod methodBsplineFit, CGeomPoint *pgptCtrlPt, CGeomPoint *pgptDataPt)
{
    double *pdBSplnBlends = new double[iCtrlPt];
    
    Matrix mCtrPts( iCtrlPt, THREEDIM );
    for( int i = 0; i < iCtrlPt; i++ )
        for( int j = 0; j < THREEDIM; j++ )
            mCtrPts( i, j ) = pgptCtrlPt[i].m_Coord[j];

    double dFirst[3] = {0};
    double dEnd[3] = {0};
    double *D = new double[iCtrlPt];
    for(int i = 0; i < iCtrlPt; i++)
        D[i] = 0.0;
    for(int k = 1; k < iCtrlPt; k++)
    {
        if(k)
        {
            for(int i = 0; i < 3; i++)
                dFirst[i] = pgptCtrlPt[k-1].m_Coord[i];
            for(int i = 0; i < 3; i++)
                dEnd[i] = pgptCtrlPt[k].m_Coord[i];

            D[k] = DistanceBetweenPoints(dFirst, dEnd);
        }
    }


    double a = pdParamRange[0];
    double b = pdParamRange[1];

    for(int k = 0; k < iCtrlPt; k++)
    {
        double d = 0.0;
        double Dmax = 0.0;
        double t = 0.0;

        if(methodBsplineFit == BSPLINE_FIT_UNIFORMSPACE)
            t = a + (b - a)*k /(iCtrlPt - 1 );
        else if(methodBsplineFit == BSPLINE_FIT_CHORDLENGTH)
        {
            for(int i = 0; i < k+1; i++)
                d = d + D[i];
            for(int i = 0; i < iCtrlPt; i++)
                Dmax += D[i];
            t = a + (b - a)*(d/ Dmax);
        }
        else if(methodBsplineFit == BSPLINE_FIT_CENTRIPETAL)
        {
            for(int i = 0; i < k+1; i++)
                d = d + sqrt(D[i]);
            for(int i = 0; i < iCtrlPt; i++)
                Dmax += sqrt(D[i]);
            t = a + (b - a)*(d/ Dmax);
        }

        if(k == (iCtrlPt-1))
            t = t * 0.9999999999;

        BSplineBlends( t, iCtrlPt, iOrder, pdKnotVec, pdBSplnBlends );
        
        Matrix M( 1, iCtrlPt );
        
        for( int i = 0; i < iCtrlPt; i++ )
                M( 0, i ) = pdBSplnBlends[i];
        
        Matrix mPoint = M*mCtrPts;
        
        for( int j = 0; j < THREEDIM; j++ )
            pgptDataPt[k].m_Coord[j] = mPoint( 0, j );
    }
}

void OpenUniformKnotVector(int nCtrlPt, int iOrder, double *pdKontVec, bool bNorm )
{
        int n = nCtrlPt - 1;
        int k = iOrder;
        for( int j = 0; j < ( n + k + 1 ); j++ )
        {
            if( j < k )
                pdKontVec[j]=0;
            if(k <= j && j <= n )
                pdKontVec[j]= j - k + 1;
            if( n+1 <= j && j <= n+k )
                pdKontVec[j]= n - k + 2;			
        }
        if( bNorm )
        {
            double d = pdKontVec[n+k];
            for( int i = 0; i < (n+k+1); i++ )
                pdKontVec[i] = pdKontVec[i]/d;
        }
}

void PeriodicKnotVector( int nCtrlPt, int iOrder, double *pdKontVec, bool bNorm )
{
    int n = nCtrlPt - 1;
    int k = iOrder;
    for( int i = 0; i < ( n + k + 1 ); i++ ) //periodic knot vector
        pdKontVec[i] = i;

    double d = pdKontVec[n + k];

    if( bNorm )
    {
        for( int i = 0; i < (n+k+1); i++ )
            pdKontVec[i] = pdKontVec[i]/d;
    }
}

int IsPeriodic( int iCtrlPt, int iOrder, double *pdKnotVec )
{
    int n = iCtrlPt;
    int k = iOrder;
    int iFlag = 0;
    for( int i = 1,j = n+k-1; i < k; i++, j-- )
    {
        if( pdKnotVec[i] == pdKnotVec[i-1] && pdKnotVec[j] == pdKnotVec[j-1] )
            iFlag = 0;
        else
        {
            iFlag = 1;
            break;
        }
    }
    return iFlag;		
}

//BSpline Fit using Chord length approximation technique.
void BSplineFit(int nCtrlPt, int iOrder, double *pdParamRange, double *pdKnotVec, int nDataPt, eBSplineFitMethod methodBsplineFit, CGeomPoint *pgptData, CGeomPoint *pgptCtrl)
{
    Matrix mDataPts( nDataPt, THREEDIM );
    for( int i = 0; i < nDataPt; i++ )
        for( int j = 0; j < THREEDIM; j++ )
            mDataPts( i, j ) = pgptData[i].m_Coord[j];

    double *pdBSplnBlends = new double[nCtrlPt];
    Matrix M( nDataPt, nCtrlPt );
    double tmax = pdParamRange[1];//nCtrlPt-iOrder+1;

    double dFirst[3] = {0};
    double dEnd[3] = {0};
    double *D = new double[nDataPt];
    for(int i = 0; i < nDataPt; i++)
        D[i] = 0.0;
    for(int k = 1; k < nDataPt; k++)
    {
        if(k)
        {
            for(int i = 0; i < 3; i++)
                dFirst[i] = pgptData[k-1].m_Coord[i];
            for(int i = 0; i < 3; i++)
                dEnd[i] = pgptData[k].m_Coord[i];

            D[k] = DistanceBetweenPoints(dFirst, dEnd);
        }
    }


    double a = pdParamRange[0];
    double b = pdParamRange[1];
    double *t = new double[nDataPt];
    for(int i = 0; i < nDataPt; i++)
        t[i] = 0.0;
        
    for(int k = 0; k < nDataPt; k++)
    {
        double d = 0.0;
        double Dmax = 0.0;

        if(methodBsplineFit == BSPLINE_FIT_UNIFORMSPACE)
            t[k] = a + (b - a)*k /(nCtrlPt - 1 );
        else if(methodBsplineFit == BSPLINE_FIT_CHORDLENGTH)
        {
            for(int i = 0; i < k+1; i++)
                d = d + D[i];
            for(int i = 0; i < nDataPt; i++)
                Dmax += D[i];
            t[k] = a + (b - a)*(d/ Dmax);
        }
        else if(methodBsplineFit == BSPLINE_FIT_CENTRIPETAL)
        {
            for(int i = 0; i < k+1; i++)
                d = d + sqrt(D[i]);
            for(int i = 0; i < nDataPt; i++)
                Dmax += sqrt(D[i]);
            t[k] = a + (b - a)*(d/ Dmax);
        }
    }

    //Knot Vector Generation As suggested by de Boor
    for(int i = 0; i < iOrder; i++)
        pdKnotVec[i] = a;
    for(int j = 1; j <= (nCtrlPt - iOrder); j++)
    {
        double u = 0.0;
        for(int i = j; i <= (j+iOrder-2); i++)
            u = u + t[i];
        pdKnotVec[j+iOrder-1] = u/(iOrder-1);	
    }	
    for(int i = nCtrlPt; i < (nCtrlPt + iOrder); i++)
        pdKnotVec[i] = b;

    for(int k = 0; k < nDataPt; k++)
    {
        if(k == (nDataPt-1))
        t[k] = t[k] * 0.9999999999;

        BSplineBlends( t[k], nCtrlPt, iOrder, pdKnotVec, pdBSplnBlends );
        
        for( int i = 0; i < nCtrlPt; i++ )
                M( k, i ) = pdBSplnBlends[i];
    }

    Matrix Mt = M.TransposeMat();

    Matrix MtM = Mt*M;

    Matrix MtM_i = MtM.InverseMat();

    //This is for checking Matrix multiplication -code can be deleted
    Matrix MI = MtM * MtM_i;
    double dDet = MI.MatrixDet();
    MI = MtM_i * MtM;
    dDet = MI.MatrixDet();
    //end

    Matrix mPoint = MtM_i * Mt * mDataPts;
        
    for( int l = 0; l < nCtrlPt; l++ )
        for( int j = 0; j < THREEDIM; j++ )
            pgptCtrl[l].m_Coord[j] = mPoint( l, j );

    if( pdBSplnBlends ) delete [] pdBSplnBlends;
}

void BSplineFitEndTangents(int nCtrlPt, int iOrder, double *pdParamRange, double *pdKontVec, int nDataPt, Vector *pFirstDerivatives, eBSplineFitMethod methodBsplineFit, CGeomPoint *pgptData, CGeomPoint *pgptCtrl)
{
    int iDegree = iOrder - 1;
    Matrix mDataPts( nDataPt, THREEDIM );
    for( int i = 0; i < nDataPt; i++ )
        for( int j = 0; j < THREEDIM; j++ )
            mDataPts( i, j ) = pgptData[i].m_Coord[j];

    double *pdBSplnBlends = new double[nCtrlPt];
    Matrix M( nDataPt, nCtrlPt );
    double tmax = pdParamRange[1];//nCtrlPt-iOrder+1;

    double dFirst[3] = {0};
    double dEnd[3] = {0};
    double *D = new double[nDataPt];
    for(int i = 0; i < nDataPt; i++)
        D[i] = 0.0;
    for(int k = 1; k < nDataPt; k++)
    {
        if(k)
        {
            for(int i = 0; i < 3; i++)
                dFirst[i] = pgptData[k-1].m_Coord[i];
            for(int i = 0; i < 3; i++)
                dEnd[i] = pgptData[k].m_Coord[i];

            D[k] = DistanceBetweenPoints(dFirst, dEnd);
        }
    }

    double a = pdParamRange[0];
    double b = pdParamRange[1];
    double *t = new double[nDataPt];
    for(int i = 0; i < nDataPt; i++)
        t[i] = 0.0;
        
    for(int k = 0; k < nDataPt; k++)
    {
        double d = 0.0;
        double Dmax = 0.0;

        if(methodBsplineFit == BSPLINE_FIT_UNIFORMSPACE)
            t[k] = a + (b - a)*k /(nCtrlPt - 1 );
        else if(methodBsplineFit == BSPLINE_FIT_CHORDLENGTH)
        {
            for(int i = 0; i < k+1; i++)
                d = d + D[i];
            for(int i = 0; i < nDataPt; i++)
                Dmax += D[i];
            t[k] = a + (b - a)*(d/ Dmax);
        }
        else if(methodBsplineFit == BSPLINE_FIT_CENTRIPETAL)
        {
            for(int i = 0; i < k+1; i++)
                d = d + sqrt(D[i]);
            for(int i = 0; i < nDataPt; i++)
                Dmax += sqrt(D[i]);
            t[k] = a + (b - a)*(d/ Dmax);
        }
    }

    int n = 2*(nDataPt-1) + 1; //No. of linear equation
    
    //Knot vector generation only for degree 2 or 3
    for(int i = 0; i < iOrder; i++ )
    {
        pdKontVec[  i  ] = t[0];
        pdKontVec[n + i] = t[nDataPt-1];
    }

    int j = iOrder;
    switch( iDegree )
    {
    case 2:

        pdKontVec[iOrder] = 0.5*(t[1] + t[0]);

      for(int i = 1; i < (nDataPt-1); i++ )
      {
          pdKontVec[++j] = t[i];
          pdKontVec[++j] = 0.5*(t[i] + t[i+1]);
      }
      break;

    case 3:

      if ( (nDataPt-1) > 1 )
      {
          pdKontVec[iOrder] = 0.5*(t[1] + t[0]);
          pdKontVec[n] = 0.5*(t[nDataPt-1] + t[nDataPt-2]);
      }

      double alf = 1.0/3.0;
      double bet = 2.0/3.0;
      for(int i = 1; i < nDataPt-2; i++ )
      {
          pdKontVec[++j] = bet*t[i] + alf*t[i+1];
          pdKontVec[++j] = alf*t[i] + bet*t[i+1];
      }
      break;   
    }

    

    Matrix Mt = M.TransposeMat();

    Matrix MtM = Mt*M;

    Matrix MtM_i = MtM.InverseMat();

    //This is for checking Matrix multiplication -code can be deleted
    Matrix MI = MtM * MtM_i;
    double dDet = MI.MatrixDet();
    MI = MtM_i * MtM;
    dDet = MI.MatrixDet();
    //end

    Matrix mPoint = MtM_i * Mt * mDataPts;
        
    for( int l = 0; l < nCtrlPt; l++ )
        for( int j = 0; j < THREEDIM; j++ )
            pgptCtrl[l].m_Coord[j] = mPoint( l, j );

    if( pdBSplnBlends ) delete [] pdBSplnBlends;
}

void BSplineFirstDervativeBlends(double t/*dParam*/, int n/*iCtrlPt*/, int k/*iOrder*/, double *pdKontVec, double *pdDerivativeBlends)
{
    double *X = new double[n + k + 1 ];
    for( int i = 1; i < n+k+1; i++ )
        X[i] = pdKontVec[i-1];

    Matrix N( ( n + k ), k + 1 );

    for( int i = 1; i < ( n + k ); i++ )
    {
        if( X[i] <= t && t < X[i+1] )
            N( i, 1 ) = 1;
        else 
            N( i, 1 ) = 0;
    }

    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i])) + ((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                    else				
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) =	((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                else
                    N( i, j ) = 0;
            }
            else N( i, j ) = 0;
        }
    }

    //Derivative part

    Matrix dN((n+k), k+1);

    for(int i = 1; i < (n+k); i++)
    {
        dN( i, 1 ) = 0;
    }
    
    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        dN( i, j ) = ((N( i, j-1 )+((t - X[i])*dN( i, j-1 )))/(X[i+j-1] - X[i])) + ((((X[i+j] - t)*dN(i+1,j-1))-N(i+1,j-1)) /(X[i+j] - X[i+1]));
                    else				
                        dN( i, j ) = ((N( i, j-1 )+((t - X[i])*dN( i, j-1 )))/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        dN( i, j ) =	((((X[i+j] - t)*dN(i+1,j-1))-N(i+1,j-1)) /(X[i+j] - X[i+1]));
                else
                    dN( i, j ) = 0;
            }
            else dN( i, j ) = 0;
        }
    }

    for( int i = 0; i < n; i++ )
        pdDerivativeBlends[i] = dN( i +1, k );

    if( X ) delete [] X;
}

void BSplineSecondDervativeBlends(double t/*dParam*/, int n/*iCtrlPt*/, int k/*iOrder*/, double *pdKontVec, double *pdDerivativeBlends)
{
    double *X = new double[n + k + 1 ];
    for( int i = 1; i < n+k+1; i++ )
        X[i] = pdKontVec[i-1];

    Matrix N( ( n + k ), k + 1 );

    for( int i = 1; i < ( n + k ); i++ )
    {
        if( X[i] <= t && t < X[i+1] )
            N( i, 1 ) = 1;
        else 
            N( i, 1 ) = 0;
    }

    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i])) + ((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                    else				
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) =	((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                else
                    N( i, j ) = 0;
            }
            else N( i, j ) = 0;
        }
    }

    //First Derivative part

    Matrix dN((n+k), k+1);

    for(int i = 1; i < (n+k); i++)
    {
        dN( i, 1 ) = 0;
    }
    
    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        dN( i, j ) = ((N( i, j-1 )+((t - X[i])*dN( i, j-1 )))/(X[i+j-1] - X[i])) + ((((X[i+j] - t)*dN(i+1,j-1))-N(i+1,j-1)) /(X[i+j] - X[i+1]));
                    else				
                        dN( i, j ) = ((N( i, j-1 )+((t - X[i])*dN( i, j-1 )))/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        dN( i, j ) =	((((X[i+j] - t)*dN(i+1,j-1))-N(i+1,j-1)) /(X[i+j] - X[i+1]));
                else
                    dN( i, j ) = 0;
            }
            else dN( i, j ) = 0;
        }
    }

    //Second Derivative part

    Matrix ddN((n+k), k+1);

    for(int i = 1; i < (n+k); i++)
    {
        ddN( i, 1 ) = 0;
    }

    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        ddN( i, j ) = ((dN( i, j-1 )+ (dN( i, j-1 ) + (t - X[i])*ddN( i, j-1 )))/(X[i+j-1] - X[i])) + ((((X[i+j] - t)*ddN(i+1,j-1) - dN(i+1,j-1))- dN(i+1,j-1)) /(X[i+j] - X[i+1]));
                    else				
                        ddN( i, j ) = ((dN( i, j-1 )+ (dN( i, j-1 ) + (t - X[i])*ddN( i, j-1 )))/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        ddN( i, j ) = ((((X[i+j] - t)*ddN(i+1,j-1) - dN(i+1,j-1))- dN(i+1,j-1)) /(X[i+j] - X[i+1]));
                else
                    ddN( i, j ) = 0;
            }
            else ddN( i, j ) = 0;
        }
    }

    for( int i = 0; i < n; i++ )
        pdDerivativeBlends[i] = ddN( i +1, k );

    if( X ) delete [] X;
}

void BSplineFirstDervativeAtParam(double dParam, int iCtrlPt, int iOrder, double *pdKontVec, CGeomPoint *pgptCtrl, double *pdVector)
{
    Matrix mCtrPts( iCtrlPt, THREEDIM );
    for( int i = 0; i < iCtrlPt; i++ )
        for( int j = 0; j < THREEDIM; j++ )
            mCtrPts( i, j ) = pgptCtrl[i].m_Coord[j];

    double* pdDerivativeBlends = new double [iCtrlPt];
    BSplineFirstDervativeBlends(dParam, iCtrlPt, iOrder, pdKontVec, pdDerivativeBlends);
    
    double *pdBlends = new double[iCtrlPt];
    //BSplineBlends(dParam, iCtrlPt, iOrder, pdKontVec, pdBlends);
    double **pdDervs = new double*[2];
    for(int i = 0; i < 2; i++)
        pdDervs[i] = new double[iCtrlPt];
    BSplineBlends_2(dParam, iCtrlPt, iOrder, pdKontVec, pdBlends, 1, pdDervs);

    if(pdBlends) delete [] pdBlends;
    if(pdDervs)
    {
        for(int i = 0; i < 2; i++)
            delete [] pdDervs[i];
        delete [] pdDervs;
    }

    Matrix M( 1, iCtrlPt );
    
    for( int i = 0; i < iCtrlPt; i++ )
        M( 0, i ) = pdDerivativeBlends[i];
    
    Matrix mPoint = M*mCtrPts;

    for( int j = 0; j < THREEDIM; j++ )
        pdVector[j] = mPoint( 0, j );

    if(pdDerivativeBlends) delete [] pdDerivativeBlends;
}

void BSplineSecondDervativeAtParam(double dParam, int iCtrlPt, int iOrder, double *pdKontVec, CGeomPoint *pgptCtrl, double *pdVector)
{
    Matrix mCtrPts( iCtrlPt, THREEDIM );
    for( int i = 0; i < iCtrlPt; i++ )
        for( int j = 0; j < THREEDIM; j++ )
            mCtrPts( i, j ) = pgptCtrl[i].m_Coord[j];

    double* pdDerivativeBlends = new double [iCtrlPt];
    BSplineSecondDervativeBlends(dParam, iCtrlPt, iOrder, pdKontVec, pdDerivativeBlends);
    
    double *pdBlends = new double[iCtrlPt];
    //BSplineBlends(dParam, iCtrlPt, iOrder, pdKontVec, pdBlends);
    double **pdDervs = new double*[3];
    for(int i = 0; i < 3; i++)
        pdDervs[i] = new double[iCtrlPt];
    BSplineBlends_2(dParam, iCtrlPt, iOrder, pdKontVec, pdBlends, 2, pdDervs);

    if(pdBlends) delete [] pdBlends;
    if(pdDervs)
    {
        for(int i = 0; i < 3; i++)
            delete [] pdDervs[i];
        delete [] pdDervs;
    }

    Matrix M( 1, iCtrlPt );
    
    for( int i = 0; i < iCtrlPt; i++ )
        M( 0, i ) = pdDerivativeBlends[i];
    
    Matrix mPoint = M*mCtrPts;

    for( int j = 0; j < THREEDIM; j++ )
        pdVector[j] = mPoint( 0, j );

    if(pdDerivativeBlends) delete [] pdDerivativeBlends;
}

void BSplineDerivativeAtParam(double t/*dParam*/, int n/*iCtrlPt*/, int k/*iOrder*/, double *pdKontVec, int iNumDervs, double *pdDerivativeBlends)
{
    double *X = new double[n + k + 1 ];
    for( int i = 1; i < n+k+1; i++ )
        X[i] = pdKontVec[i-1];

    Matrix N( ( n + k ), k + 1 );

    for( int i = 1; i < ( n + k ); i++ )
    {
        if( X[i] <= t && t < X[i+1] )
            N( i, 1 ) = 1;
        else 
            N( i, 1 ) = 0;
    }

    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i])) + ((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                    else
                        N( i, j ) = ((t - X[i])*N( i, j-1 )/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        N( i, j ) = ((X[i+j] - t)*N(i+1,j-1)/(X[i+j] - X[i+1]));
                else
                    N( i, j ) = 0;
            }
            else N( i, j ) = 0;
        }
    }

    //First Derivative part

    Matrix dN((n+k), k+1);

    for(int i = 1; i < (n+k); i++)
    {
        dN( i, 1 ) = 0;
    }
    
    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        dN( i, j ) = ((N( i, j-1 )+((t - X[i])*dN( i, j-1 )))/(X[i+j-1] - X[i])) + ((((X[i+j] - t)*dN(i+1,j-1))-N(i+1,j-1)) /(X[i+j] - X[i+1]));
                    else
                        dN( i, j ) = ((N( i, j-1 )+((t - X[i])*dN( i, j-1 )))/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        dN( i, j ) = ((((X[i+j] - t)*dN(i+1,j-1))-N(i+1,j-1)) /(X[i+j] - X[i+1]));
                else
                    dN( i, j ) = 0;
            }
            else dN( i, j ) = 0;
        }
    }

    //Second Derivative part

    Matrix ddN((n+k), k+1);

    for(int i = 1; i < (n+k); i++)
    {
        ddN( i, 1 ) = 0;
    }

    for( int j = 2; j <= k; j++ )
    {
        for( int i = 1; i < ( n + k  ); i++ )
        {
            if( (i + j) <= ( n + k ) )
            {
                if( (X[i+j-1] - X[i]) != 0 )
                {
                    if( (X[i+j] - X[i+1]) != 0 )
                        ddN( i, j ) = ((dN( i, j-1 )+ (dN( i, j-1 ) + (t - X[i])*ddN( i, j-1 )))/(X[i+j-1] - X[i])) + ((((X[i+j] - t)*ddN(i+1,j-1) - dN(i+1,j-1))- dN(i+1,j-1)) /(X[i+j] - X[i+1]));
                    else				
                        ddN( i, j ) = ((dN( i, j-1 )+ (dN( i, j-1 ) + (t - X[i])*ddN( i, j-1 )))/(X[i+j-1] - X[i]));
                }
                else if( (X[i+j] - X[i+1]) != 0 )
                        ddN( i, j ) = ((((X[i+j] - t)*ddN(i+1,j-1) - dN(i+1,j-1))- dN(i+1,j-1)) /(X[i+j] - X[i+1]));
                else
                    ddN( i, j ) = 0;
            }
            else ddN( i, j ) = 0;
        }
    }

    for( int i = 0; i < n; i++ )
        pdDerivativeBlends[i] = ddN( i +1, k );

    if( X ) delete [] X;
}
