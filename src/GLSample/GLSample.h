// GLSample.h : main header file for the GLSample application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGLSampleApp:
// See GLSample.cpp for the implementation of this class
//

class CGLSampleApp : public CWinApp
{
public:
	CGLSampleApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGLSampleApp theApp;