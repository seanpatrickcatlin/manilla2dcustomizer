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

// Manilla2DConfig.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigDlg.h"
#include "Manilla2DConfigUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CManilla2DConfigApp

BEGIN_MESSAGE_MAP(CManilla2DConfigApp, CWinApp)
END_MESSAGE_MAP()


// CManilla2DConfigApp construction
CManilla2DConfigApp::CManilla2DConfigApp()
	: CWinApp()
{

}


// The one and only CManilla2DConfigApp object
CManilla2DConfigApp theApp;

// CManilla2DConfigApp initialization

BOOL CManilla2DConfigApp::InitInstance()
{
    // SHInitExtraControls should be called once during your application's initialization to initialize any
    // of the Windows Mobile specific controls such as CAPEDIT and SIPPREF.
    SHInitExtraControls();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


#ifndef _DEBUG
    if(!FileExists(GetPathToActualHTCHomeSettingsXmlFile()))
    {
        CString msg = TEXT("Unable to find Manilla 2D settings file.\n");
        msg += TEXT("Is Manilla 2D installed?\n");
        msg += TEXT("Exiting...");

        AfxMessageBox(msg);
        return FALSE;
    }
#endif

	CManilla2DConfigDlg dlg(TEXT("Manilla 2D Customizer"));
	
    dlg.SetupPages();
    dlg.DoModal();

    // retore today screen state and reload it
    EndMakingChanges();

    if(FileExists(GetPathToErrorLogFile()))
    {
        CString msg("A log file of errors has been generated ");
        msg += GetPathToErrorLogFile();
        msg += "\nPlease attach this file when reporting errors.";

        AfxMessageBox(msg);
    }

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
