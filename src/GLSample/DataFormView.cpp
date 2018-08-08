// DataFormView.cpp : implementation file
//

#include "stdafx.h"
#include "DataFormView.h"


// CDataFormView

IMPLEMENT_DYNCREATE(CDataFormView, CFormView)

CDataFormView::CDataFormView()
	: CFormView(CDataFormView::IDD)
{

}

CDataFormView::~CDataFormView()
{
}

void CDataFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataFormView, CFormView)
END_MESSAGE_MAP()


// CDataFormView diagnostics

#ifdef _DEBUG
void CDataFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDataFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDataFormView message handlers
