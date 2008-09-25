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
    ON_NOTIFY(TVN_ITEMEXPANDING, IDC_FILE_TREE_CONTROL, &CFullFileDialogDlg::OnTvnItemexpandingFileTreeControl)
    ON_NOTIFY(NM_DBLCLK, IDC_FILE_TREE_CONTROL, &CFullFileDialogDlg::OnNMDblclkFileTreeControl)
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
    m_selectedFilePath = GetFullFilePathFromItem(m_fileTreeControl.GetSelectedItem());

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
    HTREEITEM rootItem = m_fileTreeControl.InsertItem(TEXT("My Device"), imageIndex, imageIndex, TVI_ROOT, TVI_SORT);

    AddDirectoryToFileSystemTree(rootItem, TEXT("\\"));
}

void CFullFileDialogDlg::SetTreeSelection(CString targetDirectory)
{
    m_fileTreeControl.Expand(m_fileTreeControl.GetRootItem(), TVE_EXPAND);
}

static int CALLBACK DirThenFileAplhaCB(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    int retVal = 0;
    CTreeCtrl* pTreeCtrl = (CTreeCtrl*)lParamSort;
    
    if(pTreeCtrl != NULL)
    {
        int img1A, img1B, img2A, img2B;
        HTREEITEM hItem1 = (HTREEITEM)lParam1;
        HTREEITEM hItem2 = (HTREEITEM)lParam2;

        if((hItem1 != NULL) && (hItem2 != NULL))
        {
            pTreeCtrl->GetItemImage(hItem1, img1A, img1B);
            pTreeCtrl->GetItemImage(hItem2, img2A, img2B);

            int folderImage = GetSysIlIndex(TEXT("\\"));
            
            CString itemStr1 = pTreeCtrl->GetItemText(hItem1);
            CString itemStr2 = pTreeCtrl->GetItemText(hItem2);

            if((img1A == folderImage) && (img2A == folderImage))
            {
                retVal = itemStr1.Compare(itemStr2);
            }
            else if(img1A == folderImage)
            {
                retVal = -1;
            }
            else if(img2A == folderImage)
            {
                retVal = 1;
            }
            else
            {
                retVal = itemStr1.Compare(itemStr2);
            }   
        }
    }

    return retVal;
}

bool IsDir(CString pathToTest)
{
    bool retVal = false;
    WIN32_FIND_DATA findData;
    HANDLE hFindHandle = FindFirstFile(pathToTest, &findData);

    if(hFindHandle != INVALID_HANDLE_VALUE)
    {
        if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            retVal = true;
        }
    }

    FindClose(hFindHandle);

    return retVal;
}

void CFullFileDialogDlg::AddDirectoryToFileSystemTree(HTREEITEM parentItem, CString directoryPath)
{
    int childCount = 0;
    CWaitCursor wait;

    CString fileToFind = directoryPath;

    if(fileToFind[fileToFind.GetLength()-1] != '\\')
    {
        fileToFind += "\\*.";
    }
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
                CString fullFilePath = directoryPath;

                if(directoryPath[directoryPath.GetLength()-1] != '\\')
                {
                    fullFilePath += "\\";
                }

                fullFilePath += findData.cFileName;

                int imageIndex = GetSysIlIndex(fullFilePath);

                HTREEITEM newItem = m_fileTreeControl.InsertItem(
                    findData.cFileName, imageIndex, imageIndex, parentItem, TVI_LAST);

                TVITEM tvi;
                tvi.mask = TVIF_PARAM;
                tvi.lParam = (LPARAM)newItem;
                tvi.hItem = newItem;
                m_fileTreeControl.SetItem(&tvi);

                if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    TVITEM tviDir;
                    tviDir.mask = TVIF_CHILDREN;
                    tviDir.cChildren = 1;
                    tviDir.hItem = newItem;

                    m_fileTreeControl.SetItem(&tviDir);
                }

                childCount++;
            }

            retVal = FindNextFile(hFindHandle, &findData);
        }
    }

    FindClose(hFindHandle);

    m_processedDirectories.push_back(directoryPath);

    if(childCount == 0)
    {
        // set the item to have no children
        TVITEM tvi;
        tvi.mask = TVIF_CHILDREN;
        tvi.cChildren = 0;
        tvi.hItem = parentItem;

        m_fileTreeControl.SetItem(&tvi);
    }
    else
    {
        TVSORTCB tvs;

        tvs.hParent = parentItem;
        tvs.lpfnCompare = DirThenFileAplhaCB;
        tvs.lParam = (LPARAM)&m_fileTreeControl;

        m_fileTreeControl.SortChildrenCB(&tvs);
    }
}

void CFullFileDialogDlg::OnTvnItemexpandingFileTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here

    if(pNMTreeView != NULL)
    {
        if(pNMTreeView->action & TVE_EXPAND)
        {
            TVITEMW validItem = pNMTreeView->itemNew;

            HTREEITEM validItemHandle = validItem.hItem;

            if(validItemHandle != NULL)
            {
                CString fullPath = GetFullFilePathFromItem(validItemHandle);

                if(IsDir(fullPath))
                {
                    if(!HasDirectoryBeenProcessed(fullPath))
                    {
                        AddDirectoryToFileSystemTree(validItemHandle, fullPath);
                    }
                }
            }
        }
    }

    *pResult = 0;
}

CString CFullFileDialogDlg::GetFullFilePathFromItem(HTREEITEM hItem)
{
    CString retVal;

    HTREEITEM parent = m_fileTreeControl.GetParentItem(hItem);

    if(parent == NULL)
    {
        retVal = "\\";
    }
    else
    {
        retVal = GetFullFilePathFromItem(parent);

        if(retVal[retVal.GetLength()-1] != '\\')
        {
            retVal += "\\";
        }

        retVal += m_fileTreeControl.GetItemText(hItem);
    }

    return retVal;
}

void CFullFileDialogDlg::OnNMDblclkFileTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
{
    CPoint point = (CPoint) GetMessagePos();
    m_fileTreeControl.ScreenToClient(&point);

    HTREEITEM hItem = m_fileTreeControl.HitTest(point);

    if(hItem != NULL)
    {
        CString debugMsg = TEXT("Double Click: ");
        debugMsg += GetFullFilePathFromItem(hItem);
        debugMsg += "\n";

        TRACE(debugMsg);

        if(!IsDir(GetFullFilePathFromItem(hItem)))
        {
            SendMessage(WM_COMMAND, IDOK);
        }
    }

    *pResult = 0;
}

bool CFullFileDialogDlg::HasDirectoryBeenProcessed(CString dirPath)
{
    bool retVal = false;

    for(size_t i=0; i<m_processedDirectories.size(); i++)
    {
        if(m_processedDirectories[i] == dirPath)
        {
            retVal = true;
            break;
        }
    }

    return retVal;
}