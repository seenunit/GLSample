// DataView.cpp : implementation file
//

#include "stdafx.h"
#include "DataView.h"

// CDataView

IMPLEMENT_DYNCREATE(CDataView, CTreeView)

CDataView::CDataView()
{

}

CDataView::~CDataView()
{
}

BEGIN_MESSAGE_MAP(CDataView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDataView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CDataView::OnTvnSelchanging)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CDataView::OnNMRclick)
	//ON_COMMAND(ID_BSPLINEDATACONTEXT_CHANGECOLOR, &CDataView::OnBsplinedatacontextChangecolor)
	ON_COMMAND(ID_BSPLINEDATACONTEXT_CONTROLPOLYGON, &CDataView::OnBsplinedatacontextControlpolygon)
	ON_UPDATE_COMMAND_UI(ID_BSPLINEDATACONTEXT_CONTROLPOLYGON, &CDataView::OnUpdateBsplinedatacontextControlpolygon)
	ON_COMMAND(ID_BSPLINEDATACONTEXT_TANGENTVECTORS, &CDataView::OnBsplinedatacontextTangentvectors)
	ON_UPDATE_COMMAND_UI(ID_BSPLINEDATACONTEXT_TANGENTVECTORS, &CDataView::OnUpdateBsplinedatacontextTangentvectors)
	ON_COMMAND(ID_BSPLINEDATACONTEXT_MODIFYBSPLINE, &CDataView::OnBsplinedatacontextModifybspline)
	ON_COMMAND(ID_BSPLINEDATACONTEXT_HIDECURVE, &CDataView::OnBsplinedatacontextHidecurve)
	ON_UPDATE_COMMAND_UI(ID_BSPLINEDATACONTEXT_HIDECURVE, &CDataView::OnUpdateBsplinedatacontextHidecurve)
	ON_COMMAND(ID_BSPLINEDATACONTEXT_DELETECURVE, &CDataView::OnBsplinedatacontextDeletecurve)
	ON_COMMAND(ID_BSPLINEDATACONTEXT_SWEEPCURVE, &CDataView::OnBsplinedatacontextSweepcurve)
    ON_COMMAND(ID_BEAMSOLVECONTEXT_SOLVE, &CDataView::OnBeamsolvecontextSolve)
END_MESSAGE_MAP()


// CDataView diagnostics

#ifdef _DEBUG
void CDataView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CDataView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDataView message handlers

void CDataView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	CreateTree();
}

void CDataView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	CreateTree();
}

void CDataView::CreateTree()
{
	CGLSampleDoc* pDoc = GetDocument();

	int iBsplCount  = pDoc->GetBSplineCount();

	int iBSurfCount = pDoc->GetBSurfCount();

    int iBeamcount = pDoc->GetBeamCount();

	if(!iBsplCount && !iBSurfCount && !iBeamcount) return;

	CTreeCtrl& ctrlTree = GetTreeCtrl();

	ctrlTree.DeleteAllItems();

    HTREEITEM hCurve = NULL;
    HTREEITEM hSurface = NULL;
    HTREEITEM hBeam = NULL;

    if(iBsplCount)
        hCurve = ctrlTree.InsertItem(_T("CURVES"));
    if(iBSurfCount)
        hSurface = ctrlTree.InsertItem(_T("SURFACES"));
    if(iBeamcount)
        hBeam = ctrlTree.InsertItem(_T("BEAM"));

	ctrlTree.ModifyStyle(0,WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
   | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
   | TVS_DISABLEDRAGDROP);

    HTREEITEM hItem;

	for(int i = 0; i < iBsplCount; i++)
	{
		CString strName;
		strName.Format(_T("Bspline %d"), i+1);
		hItem = ctrlTree.InsertItem(strName, hCurve);
		if (hItem != NULL)
		{
			CBSpline* pBSpline = pDoc->GetBSpline(i);
			unsigned int ID = pBSpline->GetID();
			ctrlTree.SetItemData(hItem, (DWORD)ID);
		}
	}
	
	for(int i = 0; i < iBSurfCount; i++)
	{
		CString strName;
		strName.Format(_T("BSurface %d"), i+1);
		hItem = ctrlTree.InsertItem(strName, hSurface);
		if (hItem != NULL)
		{
			CBSurf* pBSurf = pDoc->GetBSurf(i);
			unsigned int ID = pBSurf->GetID();
			ctrlTree.SetItemData(hItem, (DWORD)ID);
		}
	}

    if(iBeamcount)
	{		
		CFemBeam* pBeam = pDoc->GetFemBeam(0);
		pBeam->DisplayCtrlView(ctrlTree, hBeam);
    }

    if(hCurve != NULL )
        ctrlTree.Expand(hCurve, TVE_EXPAND);
    if(hSurface != NULL )
        ctrlTree.Expand(hSurface, TVE_EXPAND);
    if(hBeam != NULL )
        ctrlTree.Expand(hBeam, TVE_EXPAND);
}

void CDataView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CGLSampleDoc* pDoc = GetDocument();

	CGeomEntity* PEntity = GetSelectedEntity(pDoc);

	if(PEntity == NULL)
		return;

	PEntity->SetSelectionStatus(true);

	pDoc->UpdateAllViews(this);

	
	*pResult = 0;
}

void CDataView::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	CGLSampleDoc* pDoc = GetDocument();

	CGeomEntity* PEntity = GetSelectedEntity(pDoc);

	if(PEntity == NULL)
		return;

	PEntity->SetSelectionStatus(false);

	pDoc->UpdateAllViews(this);

	*pResult = 0;
}

void CDataView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	::GetCursorPos(&point);

	CTreeCtrl & Tree = GetTreeCtrl(); 

	CGLSampleDoc *pDoc = GetDocument();

	CGeomEntity *pEntity = GetSelectedEntity(pDoc);

	if(pEntity == NULL)
		return;

	eGeometry eGeomType = pEntity->GetGeometryType();

	if(eGeomType == TL_BSPLINE)
	{
		CMenu menuMain;
		menuMain.LoadMenu(IDR_BSPLINE_CONTEXT);
		CMenu *pContextMenu = menuMain.GetSubMenu(0);
		//pContextMenu->EnableMenuItem( ID_BSPLINEDATACONTEXT_MODIFYBSPLINE, MF_BYCOMMAND | MF_DISABLED|MF_GRAYED );
		pContextMenu->EnableMenuItem( ID_BSPLINEDATACONTEXT_CHANGECOLOR, MF_BYCOMMAND | MF_DISABLED|MF_GRAYED );
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, GetActiveWindow());
	}
	else if(eGeomType == TL_BEAM)
	{
		CMenu menuItem;
		menuItem.LoadMenu(IDR_BEAM_SOLVE_CONTEXT);
		CMenu *pContextMenu = menuItem.GetSubMenu(0);
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, GetActiveWindow());
	}

	*pResult = 0;
}

void CDataView::OnBsplinedatacontextChangecolor()
{

	AfxMessageBox("color");
	
}

void CDataView::OnBsplinedatacontextControlpolygon()
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;

	if(pBspln->GetControlPolygonStatus())
		pBspln->SetControlPolygonStatus(FALSE);
	else
		pBspln->SetControlPolygonStatus(TRUE);
	pDoc->UpdateAllViews( this );

}

void CDataView::OnUpdateBsplinedatacontextControlpolygon(CCmdUI *pCmdUI)
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;

	pCmdUI->SetCheck(pBspln->GetControlPolygonStatus()); //check the menu item.
}

void CDataView::OnBsplinedatacontextTangentvectors()
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;

	if(pBspln->GetTangentVectorStatus())
		pBspln->SetTangentVectorStatus(false);
	else
		pBspln->SetTangentVectorStatus(true);
	pDoc->UpdateAllViews( this );
}

void CDataView::OnUpdateBsplinedatacontextTangentvectors(CCmdUI *pCmdUI)
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;

	pCmdUI->SetCheck(pBspln->GetTangentVectorStatus()); //check the menu item.
}

void CDataView::OnBsplinedatacontextModifybspline()
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;
	AfxMessageBox("Modify Curve");
}

CGeomEntity * CDataView::GetSelectedEntity(CGLSampleDoc* pDoc)
{
	CTreeCtrl & Tree = GetTreeCtrl(); 

    HTREEITEM hSelected = Tree.GetSelectedItem();

	if(hSelected == NULL)
		return NULL;

	int ID = (int)Tree.GetItemData(hSelected);

	if(ID == 0) return NULL;

    CGeomEntity *pEntity = pDoc->GetEntityFromID(ID);

	return pEntity;
}

void CDataView::OnBsplinedatacontextHidecurve()
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;

	if(pBspln->GetHiddenStatus())
	{
		pBspln->SetHiddenStatus(false);
	}
	else
	{
		pBspln->SetHiddenStatus(true);
	}
	pDoc->UpdateAllViews( this );
}

void CDataView::OnUpdateBsplinedatacontextHidecurve(CCmdUI *pCmdUI)
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;

	pCmdUI->SetCheck(pBspln->GetHiddenStatus()); //check the menu item.
}

void CDataView::OnBsplinedatacontextDeletecurve()
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;

	pDoc->DeleteBspline(pBspln->GetID());

	CTreeCtrl & Tree = GetTreeCtrl(); 

    HTREEITEM hSelected = Tree.GetSelectedItem();

	if(hSelected == NULL)
		return;

	Tree.DeleteItem(hSelected);
		
	pDoc->UpdateAllViews( this );	

}

void CDataView::OnBsplinedatacontextSweepcurve()
{
	CGLSampleDoc* pDoc = GetDocument();

	CBSpline* pBspln = (CBSpline*)GetSelectedEntity(pDoc);

	if(pBspln == NULL)
		return;
	
	pDoc->AddBSurf(pBspln, NULL);
	
	pDoc->UpdateAllViews( this );
}

void CDataView::OnBeamsolvecontextSolve()
{
    CGLSampleDoc* pDoc = GetDocument();

    CFemBeam* pBeam = (CFemBeam*)GetSelectedEntity(pDoc);

    if(pBeam == NULL)
        return;
    
    pBeam->Solve();

    pDoc->UpdateAllViews( this );
}
