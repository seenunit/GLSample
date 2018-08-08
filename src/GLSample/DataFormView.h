#pragma once



// CDataFormView form view

class CDataFormView : public CFormView
{
	DECLARE_DYNCREATE(CDataFormView)

protected:
	CDataFormView();           // protected constructor used by dynamic creation
	virtual ~CDataFormView();

public:
	enum { IDD = IDD_DATAFORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


