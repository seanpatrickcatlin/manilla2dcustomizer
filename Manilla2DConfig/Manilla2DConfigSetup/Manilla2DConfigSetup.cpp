// Manilla2DConfigSetup.cpp : Defines the initialization routines for the DLL.
//

/*
    This file is part of Manilla2DCustomizer.

    Manilla2DCustomizer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Manilla2DCustomizer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Manilla2DCustomizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include "Manilla2DConfigSetup.h"

#include "ce_setup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

#include "..\Manilla2DConfig\Manilla2DConfigUtils.h"

// CManilla2DConfigSetupApp

BEGIN_MESSAGE_MAP(CManilla2DConfigSetupApp, CWinApp)
END_MESSAGE_MAP()


// CManilla2DConfigSetupApp construction

CManilla2DConfigSetupApp::CManilla2DConfigSetupApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}


// The one and only CManilla2DConfigSetupApp object

CManilla2DConfigSetupApp theApp;


// CManilla2DConfigSetupApp initialization

BOOL CManilla2DConfigSetupApp::InitInstance()
{
    CWinApp::InitInstance();

    return TRUE;
}

codeINSTALL_INIT Install_Init(HWND hwndParent, BOOL fFirstCall,
                              BOOL fPreviouslyInstalled, LPCTSTR pszInstallDir)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return codeINSTALL_INIT_CONTINUE;
}

codeINSTALL_EXIT Install_Exit(HWND hwndParent, LPCTSTR pszInstallDir,
                                WORD cFailedDirs, WORD cFailedFiles,
                                WORD cFailedRegKeys, WORD cFailedRegVals,
                                WORD cFailedShortcuts)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return codeINSTALL_EXIT_DONE;
}

codeUNINSTALL_INIT Uninstall_Init(HWND hwndParent, LPCTSTR pszInstallDir)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    int retVal = MessageBox(
        hwndParent,
        TEXT("Would you like to keep your M2DC themes and settings?"),
        TEXT("Uninstall M2DC"),
        MB_YESNOCANCEL|MB_ICONQUESTION);

    if(retVal == IDCANCEL)
    {
        return codeUNINSTALL_INIT_CANCEL;
    }

    if(retVal == IDNO)
    {
        CString debugMsg = TEXT("Uninstall_Init InstallDirectory: ");
        debugMsg += pszInstallDir;

        M2DC::SetAllowPopupDialogs(false);
        M2DC::SetInstallDirectory(pszInstallDir);
        M2DC::RestoreActiveThemeFromBackup();
        M2DC::RestoreHTCHomeSettingsXmlFromBackup();
        M2DC::RecursivelyDeleteDirectory(pszInstallDir);
    }

    return codeUNINSTALL_INIT_CONTINUE;
}

codeUNINSTALL_EXIT Uninstall_Exit(HWND hwndParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    M2DC::RestoreAndReEnableTodayScreen();
    return codeUNINSTALL_EXIT_DONE;
} 
