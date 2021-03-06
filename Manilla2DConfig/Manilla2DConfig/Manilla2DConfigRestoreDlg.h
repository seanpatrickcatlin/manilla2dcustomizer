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

// CManilla2DConfigRestoreDlg dialog

class CManilla2DConfigRestoreDlg : public CManilla2DConfigAbstractDlg
{
	//DECLARE_DYNAMIC(CManilla2DConfigRestoreDlg)

public:
	CManilla2DConfigRestoreDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigRestoreDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_RESTORE_DLG,
        IDS_TAB   = IDS_M2DC_RESTORE_TAB_STR,
        IDS_TITLE = IDS_M2DC_RESTORE_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

public:
    BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedRestoreButton();
    afx_msg void OnBnClickedBackupButton();
    afx_msg void OnBnClickedM2dcRestoreBackupThemeBtn();
    afx_msg void OnBnClickedM2dcRestoreRestoreThemeBtn();
};
