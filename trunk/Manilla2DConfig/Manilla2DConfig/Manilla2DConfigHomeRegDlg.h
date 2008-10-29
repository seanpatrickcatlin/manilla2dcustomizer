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

// CManilla2DConfigHomeRegDlg dialog

class CManilla2DConfigHomeRegDlg : public CManilla2DConfigAbstractDlg
{
	//DECLARE_DYNAMIC(CManilla2DConfigHomeRegDlg)

public:
	CManilla2DConfigHomeRegDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigHomeRegDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_HOMEREG_DLG,
        IDS_TAB   = IDS_M2DC_HOMEREG_TAB_STR,
        IDS_TITLE = IDS_M2DC_HOMEREG_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

public:
    BOOL OnInitDialog();

private:
    void OnOK();
    
    DECLARE_MESSAGE_MAP()

public:
    CEdit m_customNameEdit;
    CEdit m_customImageEdit;
    CButton m_customImageBtn;
    CButton m_hideNetworkNameCheckBox;
    afx_msg void OnBnClickedM2dcCustomericonBtn();
    afx_msg void OnBnClickedM2dcHomeregDefaultsBtn();
    CButton m_setDefaultsBtn;

private:
    bool GetHideNetworkName();
    void SetHideNetworkName(bool doHide);

    CString GetCustomerIcon();
    void SetCustomerIcon(CString iconPath);

    CString GetCustomerName();
    void SetCustomerName(CString customerName);
};

