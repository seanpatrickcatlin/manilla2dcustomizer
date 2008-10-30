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

// FileTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "FileTreeDlg.h"
#include "WinCeFileUtils.h"

struct FileTreeItemData
{
    HTREEITEM hItem;
    bool isDir;
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFileTreeDlg dialog

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

CFileTreeDlg::CFileTreeDlg(CWnd* pParent, CString initialDirectory, CString fileExtension)
: CManilla2DConfigAbstractDlg(pParent, CFileTreeDlg::IDD, CFileTreeDlg::IDS_TAB, CFileTreeDlg::IDS_TITLE, true)
{
    m_selectedFilePath = "";
    m_initialDirectory = initialDirectory;
    m_targetFileExtension = fileExtension;

    m_systemImageList.Attach(GetSysImageList());
}

CFileTreeDlg::~CFileTreeDlg()
{
    m_systemImageList.Detach();
}

void CFileTreeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FILE_TREE_CONTROL, m_fileTreeControl);
}

BEGIN_MESSAGE_MAP(CFileTreeDlg, CManilla2DConfigAbstractDlg)
    ON_NOTIFY(TVN_ITEMEXPANDING, IDC_FILE_TREE_CONTROL, &CFileTreeDlg::OnTvnItemexpandingFileTreeControl)
    ON_NOTIFY(NM_DBLCLK, IDC_FILE_TREE_CONTROL, &CFileTreeDlg::OnNMDblclkFileTreeControl)
END_MESSAGE_MAP()


// CFileTreeDlg message handlers

BOOL CFileTreeDlg::OnInitDialog()
{
	CManilla2DConfigAbstractDlg::OnInitDialog();

    m_fileTreeControl.SetImageList(&m_systemImageList, TVSIL_NORMAL);

    PopulateFileTree();
    SetTreeSelection(m_initialDirectory);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileTreeDlg::OnOK()
{
    // get the selected item and store its path
    m_selectedFilePath = GetFullFilePathFromItem(m_fileTreeControl.GetSelectedItem());

    CDialog::OnOK();
}

void CFileTreeDlg::OnCancel()
{
    m_selectedFilePath = "";

    CDialog::OnCancel();
}

void CFileTreeDlg::PopulateFileTree()
{
    int imageIndex = GetSysIlIndex(TEXT("\\"));
    HTREEITEM rootItem = m_fileTreeControl.InsertItem(TEXT("My Device"), imageIndex, imageIndex, TVI_ROOT, TVI_SORT);

    AddDirectoryToFileSystemTree(rootItem, TEXT("\\"));
}

void CFileTreeDlg::SetTreeSelection(CString targetDirectory)
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

        FileTreeItemData* ftidItem1 = (FileTreeItemData*)lParam1;
        FileTreeItemData* ftidItem2 = (FileTreeItemData*)lParam2;

        if((ftidItem1 != NULL) && (ftidItem2 != NULL))
        {
            HTREEITEM hItem1 = ftidItem1->hItem;
            HTREEITEM hItem2 = ftidItem2->hItem;

            if((hItem1 != NULL) && (hItem2 != NULL))
            {
                pTreeCtrl->GetItemImage(hItem1, img1A, img1B);
                pTreeCtrl->GetItemImage(hItem2, img2A, img2B);

                int folderImage = GetSysIlIndex(TEXT("\\"));

                CString itemStr1 = pTreeCtrl->GetItemText(hItem1);
                CString itemStr2 = pTreeCtrl->GetItemText(hItem2);

                if(ftidItem1->isDir && ftidItem2->isDir)
                {
                    retVal = itemStr1.CompareNoCase(itemStr2);
                }
                else if(ftidItem1->isDir)
                {
                    retVal = -1;
                }
                else if(ftidItem2->isDir)
                {
                    retVal = 1;
                }
                else
                {
                    retVal = itemStr1.CompareNoCase(itemStr2);
                }   
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

void CFileTreeDlg::AddDirectoryToFileSystemTree(HTREEITEM parentItem, CString directoryPath)
{
    int childCount = 0;
    AfxGetApp()->BeginWaitCursor();

    std::vector<CString> validExtensions;

    CString extensionList = m_targetFileExtension;

    while(extensionList.GetLength() > 0)
    {
        int newPos = extensionList.Find('|');

        CString fileExt;

        if((newPos >= 0) && (newPos < extensionList.GetLength()))
        {
            fileExt = extensionList.Mid(0, newPos);
            extensionList = extensionList.Mid(newPos+1);
        }
        else
        {
            fileExt = extensionList;
            extensionList.Empty();
        }

        validExtensions.push_back(fileExt);
    }

    CString fileToFind = directoryPath;

    if(fileToFind[fileToFind.GetLength()-1] != '\\')
    {
        fileToFind += "\\";
    }

    fileToFind += "*";

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

                CString fileExtension = findData.cFileName;
                fileExtension = fileExtension.Mid(fileExtension.ReverseFind('.')+1);

                CString debugMsg = findData.cFileName;
                debugMsg += " ";
                debugMsg += fullFilePath;
                debugMsg += " ";
                debugMsg += fileExtension;
                debugMsg += "\n";

                TRACE(debugMsg);

                bool addItem = ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

                for(size_t i=0; (i<validExtensions.size()) && (!addItem); i++)
                {
                    addItem = (fileExtension.CompareNoCase(validExtensions[i]) == 0);
                }

                if(addItem)
                {
                    int imageIndex = GetSysIlIndex(fullFilePath);

                    HTREEITEM newItem = m_fileTreeControl.InsertItem(
                        findData.cFileName, imageIndex, imageIndex, parentItem, TVI_LAST);

                    FileTreeItemData* ftid = new FileTreeItemData();
                    ftid->hItem = newItem;
                    ftid->isDir = ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);

                    TVITEM tvi;
                    tvi.mask = TVIF_PARAM;
                    tvi.lParam = (LPARAM)ftid;
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

    AfxGetApp()->EndWaitCursor();
}

void CFileTreeDlg::OnTvnItemexpandingFileTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
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

CString CFileTreeDlg::GetFullFilePathFromItem(HTREEITEM hItem)
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

void CFileTreeDlg::OnNMDblclkFileTreeControl(NMHDR *pNMHDR, LRESULT *pResult)
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

bool CFileTreeDlg::HasDirectoryBeenProcessed(CString dirPath)
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
