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

class CManilla2DConfigThemesDlg : public CPropertyPage
{
	//DECLARE_DYNAMIC(CManilla2DConfigRestoreDlg)

public:
	CManilla2DConfigThemesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigThemesDlg();

// Dialog Data
	enum { IDD = IDD_M2DC_THEMES_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

    void OnPaint();

public:
    BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

protected:
    virtual BOOL OnSetActive();

private:
    CCommandBar m_cmdBar;
};
