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

	CManilla2DConfigDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

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
