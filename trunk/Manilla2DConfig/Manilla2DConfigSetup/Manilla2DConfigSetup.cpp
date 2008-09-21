// Manilla2DConfigSetup.cpp : Defines the initialization routines for the DLL.
//

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

void RecursivelyDeleteDirectory(CString sDirPath)
{
    // Declare variables
    WIN32_FIND_DATA wfd;
    HANDLE hFile;
    DWORD dwFileAttr;
    CString sFile;

    CString sPathFile;

    if(sDirPath[sDirPath.GetLength()-1] != '\\')
    {
        sDirPath += '\\';
    }

    CString sSpec = sDirPath;
    sSpec += "*.*";

    // Find the first file
    hFile = FindFirstFile(sSpec, &wfd);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            sFile = wfd.cFileName;
            sPathFile = sDirPath + sFile;
            // Get the file attributes
            dwFileAttr = GetFileAttributes(sPathFile);

            // See if file is read-only : if so unset read-only
            if (dwFileAttr & FILE_ATTRIBUTE_READONLY)
            {
                dwFileAttr &= ~FILE_ATTRIBUTE_READONLY;
                SetFileAttributes(sPathFile, dwFileAttr);
            }

            // See if the file is a directory
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Make sure it isn't current or parent directory
                if (sFile != "." && sFile != "..")
                {
                    sPathFile += "\\";

                    // Recursively delete all files in this folder
                    RecursivelyDeleteDirectory(sPathFile);

                    // Remove the directory
                    RemoveDirectory(sPathFile);
                }
            }
            else
            {
                // Delete the file
                DeleteFile(sPathFile);
            }
        }
        while(FindNextFile(hFile, &wfd));
    }

    // Close handle to file
    FindClose(hFile);

    RemoveDirectory(sDirPath);
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
        TEXT("Uninstall M2DC"),
        TEXT("Would you like to delete your installed M2DC themes?"),
        MB_YESNOCANCEL|MB_ICONQUESTION);

    if(retVal == IDCANCEL)
    {
        return codeUNINSTALL_INIT_CANCEL;
    }

    TCHAR themeDirectoryPath[MAX_PATH];

    _tcscpy(themeDirectoryPath, pszInstallDir);
    _tcscat(themeDirectoryPath, TEXT("\\Themes"));

    if(retVal == IDYES)
    {
        RecursivelyDeleteDirectory(themeDirectoryPath);
    }

    return codeUNINSTALL_INIT_CONTINUE;
}

codeUNINSTALL_EXIT Uninstall_Exit(HWND hwndParent)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return codeUNINSTALL_EXIT_DONE;
} 
