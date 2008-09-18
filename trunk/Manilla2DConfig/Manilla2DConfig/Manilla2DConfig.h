// Manilla2DConfig.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CManilla2DConfigApp:
// See Manilla2DConfig.cpp for the implementation of this class
//

class CManilla2DConfigApp : public CWinApp
{
public:
	CManilla2DConfigApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CManilla2DConfigApp theApp;
