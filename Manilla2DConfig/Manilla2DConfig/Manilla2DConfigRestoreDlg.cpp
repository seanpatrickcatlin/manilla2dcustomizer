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
	: CPropertyPage(CManilla2DConfigRestoreDlg::IDD, IDS_M2DC_RESTORE)
{
}

CManilla2DConfigRestoreDlg::~CManilla2DConfigRestoreDlg()
{
}

void CManilla2DConfigRestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CManilla2DConfigRestoreDlg::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_OKCANCELMENU);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigRestoreDlg, CPropertyPage)
    ON_BN_CLICKED(IDC_RESTORE_BUTTON, &CManilla2DConfigRestoreDlg::OnBnClickedRestoreButton)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigRestoreDlg::OnQuerySiblings)
END_MESSAGE_MAP()

// CManilla2DConfigRestoreDlg message handlers

void CManilla2DConfigRestoreDlg::OnBnClickedRestoreButton()
{
    CString caption("This will write all settings back to the original defaults.  Press OK to continue.");
    if(MessageBox(caption, TEXT("Restore defaults?"), MB_OKCANCEL) == IDOK)
    {
        QuerySiblings(ID_RESTORE_DEFAULTS, NULL);
        EndDialog(IDCANCEL);
    }
}

LRESULT CManilla2DConfigRestoreDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}