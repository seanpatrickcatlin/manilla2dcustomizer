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

// Manilla2DConfigBackgroundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileTreeDlg.h"
#include "WinCeFileUtils.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigBackgroundDlg.h"


// CManilla2DConfigBackgroundDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigBackgroundDlg, CM2DCTabPage)

CManilla2DConfigBackgroundDlg::CManilla2DConfigBackgroundDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigBackgroundDlg::IDD, CManilla2DConfigBackgroundDlg::IDS_TAB, CManilla2DConfigBackgroundDlg::IDS_TITLE)
{
}

CManilla2DConfigBackgroundDlg::~CManilla2DConfigBackgroundDlg()
{
}

void CManilla2DConfigBackgroundDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BACKGROUND_PATH_EDIT, m_backgroundPathEdit);
}

BOOL CManilla2DConfigBackgroundDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigBackgroundDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigBackgroundDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_BACKGROUND_PATH_BROWSE_BTN, &CManilla2DConfigBackgroundDlg::OnBnClickedBackgroundPathBrowseBtn)
    ON_BN_CLICKED(IDC_BACKGROUND_PATH_CLEAR_BTN, &CManilla2DConfigBackgroundDlg::OnBnClickedBackgroundPathClearBtn)
END_MESSAGE_MAP()

// CManilla2DConfigBackgroundDlg message handlers

LRESULT CManilla2DConfigBackgroundDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

/*
void CManilla2DConfigBackgroundDlg::OnBnClickedInternetBrowserBtn()
{

}
*/

void CManilla2DConfigBackgroundDlg::OnOK()
{
    M2DC::BeginMakingChanges();
    
    Sleep(5000);

    CString pathToPng;
    m_backgroundPathEdit.GetWindowTextW(pathToPng);

    if(WinCeFileUtils::FileExists(pathToPng))
    {
        CString backgroundFilePath = M2DC::GetPathToBackgroundFile();

        DWORD dwFileAttributes = GetFileAttributes(backgroundFilePath);
        SetFileAttributes(backgroundFilePath, FILE_ATTRIBUTE_NORMAL);
        SetFileAttributes(pathToPng, FILE_ATTRIBUTE_NORMAL);
        CopyFile(pathToPng, backgroundFilePath, FALSE);
        SetFileAttributes(backgroundFilePath, dwFileAttributes);
    }

    CManilla2DConfigAbstractDlg::OnOK();
}

void CManilla2DConfigBackgroundDlg::OnBnClickedBackgroundPathBrowseBtn()
{
    CFileTreeDlg fileDlg(this, TEXT("\\"), TEXT("png"));
    if(fileDlg.DoModal() == IDOK)
    {
        CString pathToPng = fileDlg.GetFilePath();

        if(WinCeFileUtils::FileExists(pathToPng) &&
            (WinCeFileUtils::GetFileExtNoDirNoName(pathToPng).CompareNoCase(TEXT("png")) == 0))
        {
            m_backgroundPathEdit.SetWindowTextW(pathToPng);
        }
    }
}

void CManilla2DConfigBackgroundDlg::OnBnClickedBackgroundPathClearBtn()
{
    m_backgroundPathEdit.SetWindowTextW(_T(""));
}
