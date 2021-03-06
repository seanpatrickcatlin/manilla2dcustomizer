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

// CManilla2DConfigTabsDlg dialog

class CManilla2DConfigTabsDlg : public CManilla2DConfigAbstractDlg
{
public:
	CManilla2DConfigTabsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigTabsDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_TABS_DLG,
        IDS_TAB   = IDS_M2DC_TABS_TAB_STR,
        IDS_TITLE = IDS_M2DC_TABS_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    BOOL OnInitDialog();
    virtual void OnOK();
    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

    public:
    afx_msg void OnBnClickedMoveUpButton();
    afx_msg void OnBnClickedMoveDownButton();
    afx_msg void OnNMClickMainListControl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedMainListControl(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool m_bPopulatingListControl;

    NameAndEnabledState_vector_t m_newWidgetVector;
    NameAndEnabledState_vector_t m_currentWidgetVector;

    CListCtrl m_mainListControl;
    CButton m_moveUpButton;
    CButton m_moveDownButton;

    int GetSelectedItemIndex();
    void UpdateListControlFromNewWidgetVector();
};
