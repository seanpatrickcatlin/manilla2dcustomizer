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

// Manilla2DConfigDlg.h : header file
//

#pragma once

#include "afxcmn.h"
#include "M2DCTabPage.h"
#include "Manilla2DConfigUtils.h"


// CManilla2DConfigDlg dialog
class CManilla2DConfigDlg : public CDialog
{
// Construction
public:
	CManilla2DConfigDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CManilla2DConfigDlg();

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
    CTabCtrl m_mainTabControl;
    afx_msg void OnTcnSelchangeMainTabControl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnRestoreDefaultsCommand();

private:
    void SetRectangle();
    vector<CM2DCTabPage*> m_mainTabVector;
    int m_currentTabFocus;
    CCommandBar m_cmdBar;
};
