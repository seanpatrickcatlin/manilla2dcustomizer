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

// Manilla2DConfigInternetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileTreeDlg.h"
#include "WinCeFileUtils.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigInternetDlg.h"


// CManilla2DConfigInternetDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigInternetDlg, CM2DCTabPage)

CManilla2DConfigInternetDlg::CManilla2DConfigInternetDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigInternetDlg::IDD, CManilla2DConfigInternetDlg::IDS_TAB, CManilla2DConfigInternetDlg::IDS_TITLE)
{
}

CManilla2DConfigInternetDlg::~CManilla2DConfigInternetDlg()
{
}

void CManilla2DConfigInternetDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_INTERNET_BROWSER_EDIT, m_defaultBrowserEdit);
    DDX_Control(pDX, IDC_INTERNET_BROWSER_BTN, m_defaultBrowserBtn);
}

BOOL CManilla2DConfigInternetDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    m_defaultBrowserEdit.SetWindowTextW(M2DC::ReadBrowserExeFromRegistry());

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigInternetDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigInternetDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_INTERNET_BROWSER_BTN, &CManilla2DConfigInternetDlg::OnBnClickedInternetBrowserBtn)
    ON_BN_CLICKED(IDC_INTERNET_DEFAULT_BUTTON, &CManilla2DConfigInternetDlg::OnBnClickedInternetDefaultButton)
END_MESSAGE_MAP()

// CManilla2DConfigInternetDlg message handlers

LRESULT CManilla2DConfigInternetDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigInternetDlg::OnBnClickedInternetBrowserBtn()
{
    CFileTreeDlg fileDlg(this, TEXT("\\"), TEXT("exe"));
    if(fileDlg.DoModal() == IDOK)
    {
        CString pathToCmd = fileDlg.GetFilePath();

        if(WinCeFileUtils::FileExists(pathToCmd) &&
            (WinCeFileUtils::GetFileExtNoDirNoName(pathToCmd).CompareNoCase(TEXT("exe")) == 0))
        {
            m_defaultBrowserEdit.SetWindowTextW(pathToCmd);
        }
    }
}

void CManilla2DConfigInternetDlg::OnBnClickedInternetDefaultButton()
{
    m_defaultBrowserEdit.SetWindowTextW(_T(""));
}

void CManilla2DConfigInternetDlg::OnOK()
{
    M2DC::BeginMakingChanges();

    CString internetCmd;
    m_defaultBrowserEdit.GetWindowTextW(internetCmd);

    if(WinCeFileUtils::FileExists(internetCmd) || (internetCmd.GetLength() == 0))
    {
        M2DC::WriteBrowserExeToRegistry(internetCmd);
    }

    CManilla2DConfigAbstractDlg::OnOK();
}