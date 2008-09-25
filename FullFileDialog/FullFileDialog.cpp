// FullFileDialog.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FullFileDialog.h"
#include "FullFileDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFullFileDialogApp

BEGIN_MESSAGE_MAP(CFullFileDialogApp, CWinApp)
END_MESSAGE_MAP()


// CFullFileDialogApp construction
CFullFileDialogApp::CFullFileDialogApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CFullFileDialogApp object
CFullFileDialogApp theApp;

// CFullFileDialogApp initialization

BOOL CFullFileDialogApp::InitInstance()
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

	CFullFileDialogDlg dlg(NULL, TEXT("\\"), TEXT("*"));
	m_pMainWnd = &dlg;
	if(dlg.DoModal() == IDOK)
	{
        CString file = dlg.GetFilePath();
        TRACE(TEXT("\nThe user selected: "));
        TRACE(file);
        TRACE(TEXT("\n\n"));
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}