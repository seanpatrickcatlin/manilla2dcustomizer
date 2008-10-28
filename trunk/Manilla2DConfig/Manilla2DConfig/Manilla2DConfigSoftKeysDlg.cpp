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
#include "Manilla2DConfigSoftKeysDlg.h"


// CManilla2DConfigSoftKeysDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigSoftKeysDlg, CM2DCTabPage)

CManilla2DConfigSoftKeysDlg::CManilla2DConfigSoftKeysDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigSoftKeysDlg::IDD, CManilla2DConfigSoftKeysDlg::IDS_TAB, CManilla2DConfigSoftKeysDlg::IDS_TITLE)
{
}

CManilla2DConfigSoftKeysDlg::~CManilla2DConfigSoftKeysDlg()
{
}

void CManilla2DConfigSoftKeysDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_M2DC_SK1_NAME_EDIT, m_sk1NameEdit);
    DDX_Control(pDX, IDC_M2DC_SK1_CMD_EDIT, m_sk1CmdEdit);
    DDX_Control(pDX, IDC_M2DC_SK2_NAME_EDIT, m_sk2NameEdit);
    DDX_Control(pDX, IDC_M2DC_SK2_CMD_EDIT, m_sk2CmdEdit);
    DDX_Control(pDX, IDC_M2DC_SK1_CMD_BTN, m_sk1CmdBtn);
    DDX_Control(pDX, IDC_M2DC_SK2_CMD_BTN, m_sk2CmdBtn);
}

BOOL CManilla2DConfigSoftKeysDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    CString skName, skCmd, skCmdParams;
    
    GetSoftKeySettings(1, &skName, &skCmd, &skCmdParams);

    m_sk1NameEdit.SetWindowTextW(skName);
    m_sk1NameEdit.SetSel(-1);

    m_sk1CmdEdit.SetWindowTextW(skCmd);
    m_sk1CmdEdit.SetReadOnly(TRUE);
    m_sk1CmdEdit.SetSel(-1);

    GetSoftKeySettings(2, &skName, &skCmd, &skCmdParams);

    m_sk2NameEdit.SetWindowTextW(skName);
    m_sk2NameEdit.SetSel(-1);

    m_sk2CmdEdit.SetWindowTextW(skCmd);
    m_sk2CmdEdit.SetReadOnly(TRUE);
    m_sk2CmdEdit.SetSel(-1);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigSoftKeysDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigSoftKeysDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_M2DC_SK1_CMD_BTN, &CManilla2DConfigSoftKeysDlg::OnBnClickedM2dcSk1CmdBtn)
    ON_BN_CLICKED(IDC_M2DC_SK2_CMD_BTN, &CManilla2DConfigSoftKeysDlg::OnBnClickedM2dcSk2CmdBtn)
END_MESSAGE_MAP()

// CManilla2DConfigSoftKeysDlg message handlers

LRESULT CManilla2DConfigSoftKeysDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigSoftKeysDlg::OnOK()
{
    CString skName, skCmd, skCmdParams;
    
    m_sk1NameEdit.GetWindowTextW(skName);
    m_sk1CmdEdit.GetWindowTextW(skCmd);
    skCmdParams = "";
    SetSoftKeySettings(1, skName, skCmd, skCmdParams);

    m_sk2NameEdit.GetWindowTextW(skName);
    m_sk2CmdEdit.GetWindowTextW(skCmd);
    skCmdParams = "";
    SetSoftKeySettings(2, skName, skCmd, skCmdParams);

    CManilla2DConfigAbstractDlg::OnOK();
}

void CManilla2DConfigSoftKeysDlg::OnBnClickedM2dcSk1CmdBtn()
{
    CFileTreeDlg fileDlg(this, TEXT("\\"), TEXT("exe"));
    if(fileDlg.DoModal() == IDOK)
    {
        CString pathToCmd = fileDlg.GetFilePath();

        if(WinCeFileUtils::FileExists(pathToCmd) &&
            (WinCeFileUtils::GetFileExtNoDirNoName(pathToCmd).CompareNoCase(TEXT("exe")) == 0))
        {
            m_sk1CmdEdit.SetWindowTextW(pathToCmd);
        }
    }
}

void CManilla2DConfigSoftKeysDlg::OnBnClickedM2dcSk2CmdBtn()
{
    CFileTreeDlg fileDlg(this, TEXT("\\"), TEXT("exe"));
    if(fileDlg.DoModal() == IDOK)
    {
        CString pathToCmd = fileDlg.GetFilePath();

        if(WinCeFileUtils::FileExists(pathToCmd) &&
            (WinCeFileUtils::GetFileExtNoDirNoName(pathToCmd).CompareNoCase(TEXT("exe")) == 0))
        {
            m_sk2CmdEdit.SetWindowTextW(pathToCmd);
        }
    }
}

void CManilla2DConfigSoftKeysDlg::GetSoftKeySettings(int skNum, CString *skName, CString *skCmd, CString *skCmdParams)
{
    if((skName == NULL) || (skCmd == NULL) || (skCmdParams == NULL) || ((skNum != 1) && (skNum != 2)))
    {
        return;
    }

    skName->Format(TEXT(""));
    skCmd->Format(TEXT(""));
    skCmdParams->Format(TEXT(""));

    CRegKey regKey;
    regKey.Create(HKEY_LOCAL_MACHINE, TEXT("\\Software\\HTC\\Manila2D\\Home"));

    CString valueName;
    TCHAR valueBuffer[MAX_PATH];
    DWORD valueBufferSize = MAX_PATH;

    valueName.Format(TEXT("SK%d"), skNum);
    int ret = regKey.QueryStringValue(valueName, valueBuffer, &valueBufferSize);

    if(ret == ERROR_SUCCESS)
    {
        skName->Format(TEXT("%s"), valueBuffer);
    }

    valueName.Format(TEXT("SK%dCmd"), skNum);
    ret = regKey.QueryStringValue(valueName, valueBuffer, &valueBufferSize);

    if(ret == ERROR_SUCCESS)
    {
        skCmd->Format(TEXT("%s"), valueBuffer);
    }

    valueName.Format(TEXT("SK%dCmdParam"), skNum);
    ret = regKey.QueryStringValue(valueName, valueBuffer, &valueBufferSize);

    if(ret == ERROR_SUCCESS)
    {
        skCmdParams->Format(TEXT("%s"), valueBuffer);
    }
}

void CManilla2DConfigSoftKeysDlg::SetSoftKeySettings(int skNum, CString skName, CString skCmd, CString skCmdParams)
{
    if((skNum != 1) && (skNum != 2))
    {
        return;
    }

    M2DC::BeginMakingChanges();

    HKEY mainHKey;
    CString keyName = TEXT("\\Software\\HTC\\Manila2D\\Home");
    DWORD regVarTypeString = REG_SZ;
    TCHAR valueName[MAX_PATH];
    TCHAR valueBuffer[MAX_PATH];
    DWORD valueBufferSize = MAX_PATH;

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, 0, &mainHKey) == ERROR_SUCCESS)
    {
        swprintf(valueName, TEXT("SK%d"), skNum);
        swprintf(valueBuffer, TEXT("%s"), skName.GetBuffer());
        RegSetValueEx(mainHKey, valueName, NULL, REG_SZ, (CONST BYTE*)valueBuffer, valueBufferSize);

        swprintf(valueName, TEXT("SK%dCmd"), skNum);
        swprintf(valueBuffer, TEXT("%s"), skCmd.GetBuffer());
        RegSetValueEx(mainHKey, valueName, NULL, REG_SZ, (CONST BYTE*)valueBuffer, valueBufferSize);

        swprintf(valueName, TEXT("SK%dCmdParam"), skNum);
        swprintf(valueBuffer, TEXT("%s"), skCmdParams.GetBuffer());
        RegSetValueEx(mainHKey, valueName, NULL, REG_SZ, (CONST BYTE*)valueBuffer, valueBufferSize);

        RegCloseKey(mainHKey);
    }
}
