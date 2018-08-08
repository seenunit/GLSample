#pragma once

#include "resource_id.h"


// MatDialog dialog

class MatDialog : public CDialog
{
	DECLARE_DYNAMIC(MatDialog)

public:
	MatDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MatDialog();

// Dialog Data
	enum { IDD = IDD_MATDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
