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

// Manilla2DConfigRestoreDlg.cpp : implementation file
//

#include "stdafx.h"
#include <atlbase.h>
#include "FileTreeDlg.h"
#include "WinCeFileUtils.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigHomeRegDlg.h"


// CManilla2DConfigHomeRegDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigHomeRegDlg, CM2DCTabPage)

CManilla2DConfigHomeRegDlg::CManilla2DConfigHomeRegDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigHomeRegDlg::IDD, CManilla2DConfigHomeRegDlg::IDS_TAB, CManilla2DConfigHomeRegDlg::IDS_TITLE)
{
}

CManilla2DConfigHomeRegDlg::~CManilla2DConfigHomeRegDlg()
{
}

void CManilla2DConfigHomeRegDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_M2DC_CUSTOMERNAME_EDIT, m_customNameEdit);
    DDX_Control(pDX, IDC_M2DC_CUSTOMERICON_EDIT, m_customImageEdit);
    DDX_Control(pDX, IDC_M2DC_CUSTOMERICON_BTN, m_customImageBtn);
    DDX_Control(pDX, IDC_M2DC_HIDENETWORKNAME_CHECK, m_hideNetworkNameCheckBox);
    DDX_Control(pDX, IDC_M2DC_HOMEREG_DEFAULTS_BTN, m_setDefaultsBtn);
}

BOOL CManilla2DConfigHomeRegDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    m_customNameEdit.SetWindowTextW(GetCustomerName());
    m_customNameEdit.SetSel(-1);

    m_customImageEdit.SetWindowTextW(GetCustomerIcon());
    m_customImageEdit.SetSel(-1);

    m_hideNetworkNameCheckBox.SetCheck(BST_UNCHECKED);

    if(GetHideNetworkName())
    {
        m_hideNetworkNameCheckBox.SetCheck(BST_CHECKED);
    }

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigHomeRegDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigHomeRegDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_M2DC_CUSTOMERICON_BTN, &CManilla2DConfigHomeRegDlg::OnBnClickedM2dcCustomericonBtn)
    ON_BN_CLICKED(IDC_M2DC_HOMEREG_DEFAULTS_BTN, &CManilla2DConfigHomeRegDlg::OnBnClickedM2dcHomeregDefaultsBtn)
END_MESSAGE_MAP()

// CManilla2DConfigHomeRegDlg message handlers

LRESULT CManilla2DConfigHomeRegDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigHomeRegDlg::OnOK()
{
    M2DC::BeginMakingChanges();

    SetHideNetworkName(m_hideNetworkNameCheckBox.GetCheck() == BST_CHECKED);
    
    CString customerName;
    m_customNameEdit.GetWindowTextW(customerName);
    SetCustomerName(customerName);

    CString customerIcon;
    m_customImageEdit.GetWindowTextW(customerIcon);
    SetCustomerIcon(customerIcon);

    CManilla2DConfigAbstractDlg::OnOK();
}

void CManilla2DConfigHomeRegDlg::OnBnClickedM2dcCustomericonBtn()
{
    CFileTreeDlg fileDlg(this, TEXT("\\"), TEXT("png"));
    if(fileDlg.DoModal() == IDOK)
    {
        CString pathToImg = fileDlg.GetFilePath();

        if(WinCeFileUtils::FileExists(pathToImg))
        {
            m_customImageEdit.SetWindowTextW(pathToImg);
        }
    }
}

void CManilla2DConfigHomeRegDlg::OnBnClickedM2dcHomeregDefaultsBtn()
{
    m_customImageEdit.SetWindowTextW(TEXT(""));
    m_customNameEdit.SetWindowTextW(TEXT(""));
    m_hideNetworkNameCheckBox.SetCheck(BST_UNCHECKED);
}

bool CManilla2DConfigHomeRegDlg::GetHideNetworkName()
{
    bool retVal = false;

    CRegKey regKey;
    regKey.Create(HKEY_LOCAL_MACHINE, TEXT("\\Software\\HTC\\Manila2D\\Home"));

    DWORD value = 0;

    int ret = regKey.QueryDWORDValue(TEXT("HideNetworkName"), value);

    if(ret == ERROR_SUCCESS)
    {
        retVal = (value != 0);
    }

    return retVal;
}

void CManilla2DConfigHomeRegDlg::SetHideNetworkName(bool doHide)
{
    DWORD value = 0;

    if(doHide)
    {
        value = 1;
    }

    CRegKey regKey;
    regKey.Create(HKEY_LOCAL_MACHINE, TEXT("\\Software\\HTC\\Manila2D\\Home"));
    regKey.SetDWORDValue(TEXT("HideNetworkName"), value);
}

CString CManilla2DConfigHomeRegDlg::GetCustomerIcon()
{
    CString retVal = TEXT("");

    CRegKey regKey;
    regKey.Create(HKEY_LOCAL_MACHINE, TEXT("\\Software\\HTC\\Manila2D\\Home"));

    TCHAR valueBuffer[MAX_PATH];
    DWORD valueBufferSize = MAX_PATH;

    int ret = regKey.QueryStringValue(TEXT("CustomerIcon"), valueBuffer, &valueBufferSize);

    if(ret == ERROR_SUCCESS)
    {
        retVal.Format(TEXT("%s"), valueBuffer);
    }

    return retVal;
}

void CManilla2DConfigHomeRegDlg::SetCustomerIcon(CString iconPath)
{
    CRegKey regKey;
    regKey.Create(HKEY_LOCAL_MACHINE, TEXT("\\Software\\HTC\\Manila2D\\Home"));
    regKey.SetStringValue(TEXT("CustomerIcon"), iconPath);
}

CString CManilla2DConfigHomeRegDlg::GetCustomerName()
{
    CString retVal = TEXT("");

    CRegKey regKey;
    regKey.Create(HKEY_LOCAL_MACHINE, TEXT("\\Software\\HTC\\Manila2D\\Home"));

    TCHAR valueBuffer[MAX_PATH];
    DWORD valueBufferSize = MAX_PATH;

    int ret = regKey.QueryStringValue(TEXT("CustomerName"), valueBuffer, &valueBufferSize);

    if(ret == ERROR_SUCCESS)
    {
        retVal.Format(TEXT("%s"), valueBuffer);
    }

    return retVal;
}

void CManilla2DConfigHomeRegDlg::SetCustomerName(CString customerName)
{
    CRegKey regKey;
    regKey.Create(HKEY_LOCAL_MACHINE, TEXT("\\Software\\HTC\\Manila2D\\Home"));
    regKey.SetStringValue(TEXT("CustomerName"), customerName);
}
