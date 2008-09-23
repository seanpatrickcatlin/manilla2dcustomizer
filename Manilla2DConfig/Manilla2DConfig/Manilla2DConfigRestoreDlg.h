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

// CManilla2DConfigRestoreDlg dialog

class CManilla2DConfigRestoreDlg : public CPropertyPage
{
	//DECLARE_DYNAMIC(CManilla2DConfigRestoreDlg)

public:
	CManilla2DConfigRestoreDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigRestoreDlg();

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_RESTORE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
    virtual void OnOK();
    virtual void OnCancel();
    virtual UINT GetTabStringID() { return IDS_M2DC_RESTORE; };
    virtual void OnRestoreDefaults();

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedRestoreButton();
};
