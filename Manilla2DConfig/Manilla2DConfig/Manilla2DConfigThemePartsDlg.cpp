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

// Manilla2DConfigThemePartsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigThemePartsDlg.h"
#include "WinCeFileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CManilla2DConfigThemePartsDlg dialog

CManilla2DConfigThemePartsDlg::CManilla2DConfigThemePartsDlg(CWnd* pParent)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigThemePartsDlg::IDD, CManilla2DConfigThemePartsDlg::IDS_TAB, CManilla2DConfigThemePartsDlg::IDS_TITLE, true)
{

}

CManilla2DConfigThemePartsDlg::~CManilla2DConfigThemePartsDlg()
{
}

void CManilla2DConfigThemePartsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CManilla2DConfigThemePartsDlg, CManilla2DConfigAbstractDlg)
END_MESSAGE_MAP()


// CManilla2DConfigThemePartsDlg message handlers

BOOL CManilla2DConfigThemePartsDlg::OnInitDialog()
{
	CManilla2DConfigAbstractDlg::OnInitDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigThemePartsDlg::OnOK()
{
    CDialog::OnOK();
}

void CManilla2DConfigThemePartsDlg::OnCancel()
{
    CDialog::OnCancel();
}


