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

// Manilla2DConfigThemePartsDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>

#include "Manilla2DConfig.h"
#include "Manilla2DConfigAbstractDlg.h"
#include "afxwin.h"

// CManilla2DConfigThemePartsDlg dialog
class CManilla2DConfigThemePartsDlg : public CManilla2DConfigAbstractDlg
{
// Construction
public:
	CManilla2DConfigThemePartsDlg(CWnd* pParent);	// standard constructor
    ~CManilla2DConfigThemePartsDlg();

// Dialog Data
	enum
    {
        IDD = IDD_M2DC_THEME_PARTS_DLG,
        IDS_TAB   = -1,
        IDS_TITLE = IDS_M2DC_THEME_PARTS_TITLE_STR
    };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    void OnOK();
    void OnCancel();

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
