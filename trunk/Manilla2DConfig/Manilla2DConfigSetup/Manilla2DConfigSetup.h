// Manilla2DConfigSetup.h : main header file for the Manilla2DConfigSetup DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resourceppc.h"

// CManilla2DConfigSetupApp
// See Manilla2DConfigSetup.cpp for the implementation of this class
//

class CManilla2DConfigSetupApp : public CWinApp
{
public:
	CManilla2DConfigSetupApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

