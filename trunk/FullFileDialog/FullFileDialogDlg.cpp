// FullFileDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FullFileDialog.h"
#include "FullFileDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFullFileDialogDlg dialog

// how to tget a handle to the system image list
HIMAGELIST GetSysImageList()
{
    HIMAGELIST himl;
    SHFILEINFO sfi;

    himl = (HIMAGELIST)SHGetFileInfo( _T("\\"), 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);

    return himl;
} 

// how to get the icon index into the system image list
int GetSysIlIndex(LPCTSTR szFileName)
{
    SHFILEINFO sfi;
    ::SHGetFileInfo(szFileName, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX|SHGFI_SMALLICON);

    return sfi.iIcon;
}

CFullFileDialogDlg::CFullFileDialogDlg(CWnd* pParent, CString initialDirectory, CString fileExtension)
	: CDialog(CFullFileDialogDlg::IDD, pParent)
{
    m_selectedFilePath = "";
    m_initialDirectory = initialDirectory;
    m_targetFileExtension = fileExtension;

    m_systemImageList.Attach(GetSysImageList());
}

CFullFileDialogDlg::~CFullFileDialogDlg()
{
    m_systemImageList.Detach();
}

void CFullFileDialogDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FILE_TREE_CONTROL, m_fileTreeControl);
}

BEGIN_MESSAGE_MAP(CFullFileDialogDlg, CDialog)

END_MESSAGE_MAP()


// CFullFileDialogDlg message handlers

BOOL CFullFileDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_OKCANCELMENU);

    m_fileTreeControl.SetImageList(&m_systemImageList, TVSIL_NORMAL);

    PopulateFileTree();
    SetTreeSelection(m_initialDirectory);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFullFileDialogDlg::OnOK()
{
    // get the selected item and store its path
    m_selectedFilePath = "";

    CDialog::OnOK();
}

void CFullFileDialogDlg::OnCancel()
{
    m_selectedFilePath = "";

    CDialog::OnCancel();
}

void CFullFileDialogDlg::PopulateFileTree()
{
    int imageIndex = GetSysIlIndex(TEXT("\\"));
    HTREEITEM rootItem = m_fileTreeControl.InsertItem(TEXT("My Device"), imageIndex, imageIndex, TVI_ROOT, TVI_LAST);

    AddDirectoryToFileSystemTree(rootItem, TEXT("\\MUSIC"));
}

void CFullFileDialogDlg::SetTreeSelection(CString targetDirectory)
{
    //m_fileTreeControl.Expand(TVI_ROOT, TVE_EXPAND);
}

CString CFullFileDialogDlg::GetTreeSelection()
{
    CString retVal = TEXT("");

    // do stuff here

    return retVal;
}

void CFullFileDialogDlg::AddDirectoryToFileSystemTree(HTREEITEM parentItem, CString directoryName)
{
    CString fileToFind = directoryName;
    fileToFind += "\\*.";
    fileToFind += m_targetFileExtension;

    WIN32_FIND_DATA findData;
    HANDLE hFindHandle = FindFirstFile(fileToFind, &findData);

    if(hFindHandle != INVALID_HANDLE_VALUE)
    {
        BOOL retVal = TRUE;

        while(retVal != FALSE)
        {
            if((lstrcmpi(findData.cFileName, TEXT(".")) != 0) && (lstrcmpi(findData.cFileName, TEXT("..")) != 0))
            {
                CString fullFilePath = directoryName;
                fullFilePath += "\\";
                fullFilePath += findData.cFileName;
                TRACE(fullFilePath);
                TRACE(TEXT("\n"));
                int imageIndex = GetSysIlIndex(fullFilePath);

                HTREEITEM newItem = m_fileTreeControl.InsertItem(findData.cFileName, imageIndex, imageIndex, parentItem, TVI_LAST);

                if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    AddDirectoryToFileSystemTree(newItem, fullFilePath);
                }
            }

            retVal = FindNextFile(hFindHandle, &findData);
        }
    }

    FindClose(hFindHandle);
}
