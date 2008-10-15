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

#include "afxwin.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigAbstractDlg.h"

// CManilla2DConfigRestoreDlg dialog

class CManilla2DConfigThemesDlg : public CManilla2DConfigAbstractDlg
{
	//DECLARE_DYNAMIC(CManilla2DConfigRestoreDlg)

public:
	CManilla2DConfigThemesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigThemesDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_THEMES_DLG,
        IDS_TAB   = IDS_M2DC_THEMES_TAB_STR,
        IDS_TITLE = IDS_M2DC_THEMES_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

public:
    BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL OnSetActive();
    void OnOK();

private:
    int m_previewWidth;
    int m_previewHeight;

    void RefreshThemeList();

public:
    CStatic m_pictureBox;
    CButton m_removeButton;
    CListBox m_themeChooserListBox;

    afx_msg void OnBnClickedM2dcThemeApplyBtn();
    afx_msg void OnBnClickedM2dcThemeImportBtn();
    afx_msg void OnBnClickedM2dcThemeDeleteBtn();
    afx_msg void OnLbnSelchangeM2dcThemeListbox();
};
