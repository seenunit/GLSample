#pragma once
#include "..\graphics\geomentity.h"
class CFemBeam : public CGeomEntity
{
    double m_dBeamLength;
    int m_iBeamNodes;
    double m_dYModuli;
    double m_dSectionArea;

    CGeomPoint m_gptBeamEndPoints[2];
public:
    //constructors
    CFemBeam(void);
    CFemBeam(double dBeamLength, int iBeamNodes, double dYModuli, double dSectionArea, CGeomPoint *pgptEndPoints);
    void Draw();

    //Distructor
    ~CFemBeam(void);

    //Get Beam variables
    double GetBeamLength();
    int GetBeamNumNodes();
    double GetYoungsModuli();
    double GetSectionArea();

    void GetBeamNodePoints( CGeomPoint *pgptKnotPt );

    //Tree control in dataview window
    void DisplayCtrlView(CTreeCtrl& ctrlTree, HTREEITEM& hBeam);

    //Solution
    void Solve();
};

