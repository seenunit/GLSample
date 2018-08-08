// MatDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GLSampleDoc.h"
#include "MatDialog.h"


// MatDialog dialog

IMPLEMENT_DYNAMIC(MatDialog, CDialog)

MatDialog::MatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MatDialog::IDD, pParent)
{

}

MatDialog::~MatDialog()
{
}

void MatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MatDialog, CDialog)
END_MESSAGE_MAP()


// MatDialog message handlers
