#pragma once

// CDataView view

class CDataView : public CTreeView
{
	DECLARE_DYNCREATE(CDataView)

protected:
	CDataView();           // protected constructor used by dynamic creation
	virtual ~CDataView();

public:
	CGLSampleDoc* GetDocument()
		{
			ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLSampleDoc)));
			return (CGLSampleDoc*) m_pDocument;
		}

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	void CreateTree();
	COLORREF m_prevBsplineColor;
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBsplinedatacontextChangecolor();
	afx_msg void OnBsplinedatacontextControlpolygon();
	afx_msg void OnUpdateBsplinedatacontextControlpolygon(CCmdUI *pCmdUI);
	afx_msg void OnBsplinedatacontextTangentvectors();
	afx_msg void OnUpdateBsplinedatacontextTangentvectors(CCmdUI *pCmdUI);
	afx_msg void OnBsplinedatacontextModifybspline();

	CGeomEntity* GetSelectedEntity(CGLSampleDoc* pDoc);
	afx_msg void OnBsplinedatacontextHidecurve();
	afx_msg void OnUpdateBsplinedatacontextHidecurve(CCmdUI *pCmdUI);
	afx_msg void OnBsplinedatacontextDeletecurve();
	afx_msg void OnUpdateBsplinedatacontextDeletecurve(CCmdUI *pCmdUI);
	afx_msg void OnBsplinedatacontextSweepcurve();
    afx_msg void OnBeamsolvecontextSolve();
};




