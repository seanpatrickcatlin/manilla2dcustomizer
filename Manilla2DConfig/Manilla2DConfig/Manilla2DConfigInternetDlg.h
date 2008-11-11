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

// CManilla2DConfigInternetDlg dialog

class CManilla2DConfigInternetDlg : public CManilla2DConfigAbstractDlg
{
	//DECLARE_DYNAMIC(CManilla2DConfigInternetDlg)

public:
	CManilla2DConfigInternetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigInternetDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_INTERNET_DLG,
        IDS_TAB   = IDS_M2DC_INTERNET_TAB_STR,
        IDS_TITLE = IDS_M2DC_INTERNET_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

public:
    BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
    CEdit m_defaultBrowserEdit;
    CButton m_defaultBrowserBtn;
    afx_msg void OnBnClickedInternetBrowserBtn();
    afx_msg void OnBnClickedInternetDefaultButton();

    void OnOK();
};
