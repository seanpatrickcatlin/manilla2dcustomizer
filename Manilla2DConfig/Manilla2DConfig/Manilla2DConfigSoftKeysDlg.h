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

#pragma once

#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigAbstractDlg.h"
#include "afxwin.h"

// CManilla2DConfigSoftKeysDlg dialog

class CManilla2DConfigSoftKeysDlg : public CManilla2DConfigAbstractDlg
{
	//DECLARE_DYNAMIC(CManilla2DConfigSoftKeysDlg)

public:
	CManilla2DConfigSoftKeysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigSoftKeysDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_SOFTKEYS_DLG,
        IDS_TAB   = IDS_M2DC_SOFTKEYS_TAB_STR,
        IDS_TITLE = IDS_M2DC_SOFTKEYS_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

public:
    BOOL OnInitDialog();

private:
    CEdit m_sk1NameEdit;
    CEdit m_sk1CmdEdit;
    CEdit m_sk2NameEdit;
    CEdit m_sk2CmdEdit;

    void OnOK();
    
    afx_msg void OnBnClickedM2dcSk1CmdBtn();
    afx_msg void OnBnClickedM2dcSk2CmdBtn();

    void GetSoftKeySettings(int skNum, CString *skName, CString *skCmd, CString *skCmdParams);
    void SetSoftKeySettings(int skNum, CString skName, CString skCmd, CString skCmdParams);

    DECLARE_MESSAGE_MAP()
public:
    CButton m_sk1CmdBtn;
    CButton m_sk2CmdBtn;
};
