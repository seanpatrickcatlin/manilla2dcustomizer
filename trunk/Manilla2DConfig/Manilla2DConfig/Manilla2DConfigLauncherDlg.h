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

#include "M2DCTabPage.h"

// CManilla2DConfigLauncherDlg dialog

class CManilla2DConfigLauncherDlg : public CM2DCTabPage
{
public:
	CManilla2DConfigLauncherDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigLauncherDlg();

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_LAUNCHER_DIALOG };

    virtual UINT GetIDD() { return IDD_MANILLA2DCONFIG_LAUNCHER_DIALOG; };
    virtual CString GetTabText() { return TEXT("Launcher"); };

    virtual void OnOK();
    virtual void OnCancel();
    virtual void RestoreDefaults();

protected:
    virtual BOOL CManilla2DConfigLauncherDlg::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
    int m_initialNumberOfColumns;

    CButton m_launcherThreeColumnRadioButton;
    CButton m_launcherFourColumnRadioButton;

    int GetNumberOfLauncherColumnsFromHTCHomeSettingsXml();
    void SetNumberOfLauncherColumnsFromHTCHomeSettingsXml(int numberOfColumns);
};