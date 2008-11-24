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

// CManilla2DConfigBackgroundDlg dialog

class CManilla2DConfigBackgroundDlg : public CManilla2DConfigAbstractDlg
{
	//DECLARE_DYNAMIC(CManilla2DConfigBackgroundDlg)

public:
	CManilla2DConfigBackgroundDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigBackgroundDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_BACKGROUND_DLG,
        IDS_TAB   = IDS_M2DC_BACKGROUND_TAB_STR,
        IDS_TITLE = IDS_M2DC_BACKGROUND_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

public:
    BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
//    afx_msg void OnBnClickedBackgroundBrowserBtn();

    void OnOK();
    afx_msg void OnBnClickedBackgroundPathBrowseBtn();
    afx_msg void OnBnClickedBackgroundPathClearBtn();
    CEdit m_backgroundPathEdit;
};
