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

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigLauncherDlg.h"

#include "tinyxml.h"
#include "tinystr.h"

// CManilla2DConfigTabsDlg dialog

CManilla2DConfigLauncherDlg::CManilla2DConfigLauncherDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigLauncherDlg::IDD, CManilla2DConfigLauncherDlg::IDS_TAB, CManilla2DConfigLauncherDlg::IDS_TITLE)
{
    M2DC::ReadLauncherValuesFromXml(m_numberOfColumns, m_numberOfRows);
}

CManilla2DConfigLauncherDlg::~CManilla2DConfigLauncherDlg()
{
}

void CManilla2DConfigLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_M2DC_LAUNCHER_3_COL_RB, m_launcher3ColumnRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_4_COL_RB, m_launcher4ColumnRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_5_COL_RB, m_launcher5ColumnRadioButton);

    DDX_Control(pDX, IDC_M2DC_LAUNCHER_3_ROW_RB, m_launcher3RowRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_6_ROW_RB, m_launcher6RowRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_9_ROW_RB, m_launcher9RowRadioButton);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigLauncherDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigLauncherDlg::OnQuerySiblings)
END_MESSAGE_MAP()

BOOL CManilla2DConfigLauncherDlg::OnInitDialog()
{
	CManilla2DConfigAbstractDlg::OnInitDialog();

    m_launcher3ColumnRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher4ColumnRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher5ColumnRadioButton.SetCheck(BST_UNCHECKED);

    if(m_numberOfColumns == 3)
    {
        m_launcher3ColumnRadioButton.SetCheck(BST_CHECKED);
    }
    else if(m_numberOfColumns == 4)
    {
        m_launcher4ColumnRadioButton.SetCheck(BST_CHECKED);
    }
    else if(m_numberOfColumns == 5)
    {
        m_launcher5ColumnRadioButton.SetCheck(BST_CHECKED);
    }
    else
    {
        m_launcher3ColumnRadioButton.EnableWindow(FALSE);
        m_launcher4ColumnRadioButton.EnableWindow(FALSE);
        m_launcher5ColumnRadioButton.EnableWindow(FALSE);
    }

    m_launcher3RowRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher6RowRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher9RowRadioButton.SetCheck(BST_UNCHECKED);

    if((m_numberOfRows >= 0) && (m_numberOfRows <= 3))
    {
        m_launcher3RowRadioButton.SetCheck(BST_CHECKED);
    }
    else if((m_numberOfRows > 3) && (m_numberOfRows <= 6))
    {
        m_launcher6RowRadioButton.SetCheck(BST_CHECKED);
    }
    else if(m_numberOfRows > 6)
    {
        m_launcher9RowRadioButton.SetCheck(BST_CHECKED);
    }
    else
    {
        m_launcher3RowRadioButton.EnableWindow(FALSE);
        m_launcher6RowRadioButton.EnableWindow(FALSE);
        m_launcher9RowRadioButton.EnableWindow(FALSE);
    }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CManilla2DConfigLauncherDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigLauncherDlg::OnOK()
{
    int currentColumnCount = -1;
    if(m_launcher3ColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 3;
    }
    else if(m_launcher4ColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 4;
    }
    else if(m_launcher5ColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 5;
    }

    int currentRowCount = -1;
    if(m_launcher3RowRadioButton.GetCheck() == BST_CHECKED)
    {
        currentRowCount = 3;
    }
    else if(m_launcher6RowRadioButton.GetCheck() == BST_CHECKED)
    {
        currentRowCount = 6;
    }
    else if(m_launcher9RowRadioButton.GetCheck() == BST_CHECKED)
    {
        currentRowCount = 9;
    }

    if((m_numberOfColumns != currentColumnCount) || (m_numberOfRows != currentRowCount))
    {
        m_numberOfRows = currentRowCount;
        m_numberOfColumns = currentColumnCount;

        M2DC::BeginMakingChanges();
        M2DC::WriteLauncherValuesToXml(m_numberOfColumns, m_numberOfRows);
    }
}