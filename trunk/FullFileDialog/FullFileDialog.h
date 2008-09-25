// FullFileDialog.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CFullFileDialogApp:
// See FullFileDialog.cpp for the implementation of this class
//

class CFullFileDialogApp : public CWinApp
{
public:
	CFullFileDialogApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CFullFileDialogApp theApp;
