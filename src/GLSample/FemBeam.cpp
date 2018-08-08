#include "StdAfx.h"
#include "FemBeam.h"


CFemBeam::CFemBeam(void) : CGeomEntity( TL_BEAM )
{
}

CFemBeam::CFemBeam(double dBeamLength, int iBeamNodes, double dYModuli, double dSectionArea, CGeomPoint *pgptEndPoints) : CGeomEntity( TL_BEAM )
{
    m_dBeamLength = dBeamLength;
    m_iBeamNodes = iBeamNodes;
    m_dYModuli = dYModuli;
    m_dSectionArea = dSectionArea;

    for(int i = 0; i < 2 ; i++)
        m_gptBeamEndPoints[i] = pgptEndPoints[i];

    //settings
    m_bSelected = false;
    m_bHidden = false;
    m_ID = (unsigned int)this;
}

CFemBeam::~CFemBeam(void)
{
}

void CFemBeam::Draw()
{
    if(m_bHidden)
        return;

    //Beam Line
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < 2; i++)
        glVertex3d(m_gptBeamEndPoints[i].m_Coord[0], m_gptBeamEndPoints[i].m_Coord[1], m_gptBeamEndPoints[i].m_Coord[2]);
    glEnd();

    //nodes
    CGeomPoint *pgptKnotPt = new CGeomPoint[ m_iBeamNodes ];
    GetBeamNodePoints( pgptKnotPt );
    double  dSize = 0.05;
    DrawPoints(m_iBeamNodes, dSize, pgptKnotPt);

}

void CFemBeam::Solve()
{
    AfxMessageBox("Solved");
}

//Funtions to get Beam variables
double CFemBeam::GetBeamLength()
{
    return m_dBeamLength;
}
int CFemBeam::GetBeamNumNodes()
{
    return m_iBeamNodes;
}
double CFemBeam::GetYoungsModuli()
{
    return m_dYModuli;
}

double CFemBeam::GetSectionArea()
{
    return m_dSectionArea;
}

//Tree control in dataview window
void CFemBeam::DisplayCtrlView(CTreeCtrl &ctrlTree, HTREEITEM& hBeam)
{
	HTREEITEM hItem;
	HTREEITEM hPreprocessor;
	CString strName;
	strName.Format(_T("Preprocessor"));
	hPreprocessor = ctrlTree.InsertItem(strName, hBeam);
	if(hPreprocessor != NULL)
	{
		ctrlTree.SetItemData(hPreprocessor, (DWORD_PTR)hPreprocessor);

		double dBeamLen = GetBeamLength();
		CString strBeamLength;
		strBeamLength.Format(_T("Length: %f"), dBeamLen);
		hItem = ctrlTree.InsertItem(strBeamLength, hPreprocessor);

		int iBeamNodes = GetBeamNumNodes();
		CString strBeamNodes;
		strBeamNodes.Format(_T("Nodes: %d"), iBeamNodes);
		hItem = ctrlTree.InsertItem(strBeamNodes, hPreprocessor);

		double dYModuli = GetYoungsModuli();
		CString strYModuli;
		strYModuli.Format(_T("YModuli: %f"), dYModuli);
		hItem = ctrlTree.InsertItem(strYModuli, hPreprocessor);

		double dSectionArea = GetSectionArea();
		CString strSectionArea;
		strSectionArea.Format(_T("Section Area: %f"), dSectionArea);
		hItem = ctrlTree.InsertItem(strSectionArea, hPreprocessor);
	}

    strName.Format(_T("Apply Loads"));
    hPreprocessor = ctrlTree.InsertItem(strName, hBeam);
    if(hPreprocessor != NULL)
    {
        ctrlTree.SetItemData(hItem, (DWORD_PTR)hPreprocessor);
    }

	strName.Format(_T("Solution"));
	hItem = ctrlTree.InsertItem(strName, hBeam);
	unsigned int ID = GetID();
	if(hItem != NULL)
		ctrlTree.SetItemData(hItem, (DWORD)ID);

	strName.Format(_T("PostProcess"));
	hItem = ctrlTree.InsertItem(strName, hBeam);
	if(hItem != NULL)
		ctrlTree.SetItemData(hItem, (DWORD_PTR)hItem);
}

void CFemBeam::GetBeamNodePoints( CGeomPoint *pgptKnotPt )
{
    for(int i = 0; i < m_iBeamNodes; i++)
    {

    }
}

