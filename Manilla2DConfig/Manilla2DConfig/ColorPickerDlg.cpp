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

// FileTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "ColorPickerDlg.h"
#include "WinCeFileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorPickerDlg dialog

CColorPickerDlg::CColorPickerDlg(CWnd* pParent)
: CManilla2DConfigAbstractDlg(pParent, CColorPickerDlg::IDD, CColorPickerDlg::IDS_TAB, CColorPickerDlg::IDS_TITLE, true)
{
}

CColorPickerDlg::~CColorPickerDlg()
{
}

void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CColorPickerDlg, CManilla2DConfigAbstractDlg)
END_MESSAGE_MAP()


// CColorPickerDlg message handlers

BOOL CColorPickerDlg::OnInitDialog()
{
	CManilla2DConfigAbstractDlg::OnInitDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColorPickerDlg::OnOK()
{

    CDialog::OnOK();
}

void CColorPickerDlg::OnCancel()
{

    CDialog::OnCancel();
}
