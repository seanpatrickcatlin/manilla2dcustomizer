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

// Manilla2DConfigRestoreDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigRestoreDlg.h"


// CManilla2DConfigRestoreDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigRestoreDlg, CM2DCTabPage)

CManilla2DConfigRestoreDlg::CManilla2DConfigRestoreDlg(CWnd* pParent /*=NULL*/)
	: CM2DCTabPage(CManilla2DConfigRestoreDlg::IDD, pParent)
{
}

CManilla2DConfigRestoreDlg::~CManilla2DConfigRestoreDlg()
{
}

void CManilla2DConfigRestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigRestoreDlg, CDialog)
    ON_BN_CLICKED(IDC_RESTORE_BUTTON, &CManilla2DConfigRestoreDlg::OnBnClickedRestoreButton)
END_MESSAGE_MAP()


UINT CManilla2DConfigRestoreDlg::GetIDD()
{
    return CManilla2DConfigRestoreDlg::IDD;
}

void CManilla2DConfigRestoreDlg::OnOK()
{
    CDialog::OnOK();
}

void CManilla2DConfigRestoreDlg::OnCancel()
{
    CDialog::OnCancel();
}

CString CManilla2DConfigRestoreDlg::GetTabText()
{
    return TEXT("Restore");
}

void CManilla2DConfigRestoreDlg::RestoreDefaults()
{
    // this is an empty method because there are no defualts to restore
    // on the restore defaults page
}


// CManilla2DConfigRestoreDlg message handlers

void CManilla2DConfigRestoreDlg::OnBnClickedRestoreButton()
{
    CString caption("This will write all settings back to the original defaults.  Press OK to continue.");
    if(MessageBox(caption, TEXT("Restore defaults?"), MB_OKCANCEL) == IDOK)
    {
        GetParent()->SendMessage(WM_COMMAND, ID_RESTORE_DEFAULTS_CMD);
    }
}
