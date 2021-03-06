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

#include "Manilla2DConfigAbstractDlg.h"

// CManilla2DConfigLauncherDlg dialog

class CManilla2DConfigLauncherDlg : public CManilla2DConfigAbstractDlg
{
public:
	CManilla2DConfigLauncherDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigLauncherDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_LAUNCHER_DLG,
        IDS_TAB   = IDS_M2DC_LAUNCHER_TAB_STR,
        IDS_TITLE = IDS_M2DC_LAUNCHER_TITLE_STR
    };

    virtual void OnOK();
    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

protected:
    virtual BOOL CManilla2DConfigLauncherDlg::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
    int m_numberOfRows;
    int m_numberOfColumns;

    CButton m_launcher3ColumnRadioButton;
    CButton m_launcher4ColumnRadioButton;
    CButton m_launcher5ColumnRadioButton;

    CButton m_launcher3RowRadioButton;
    CButton m_launcher6RowRadioButton;
    CButton m_launcher9RowRadioButton;
};
