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

// Manilla2DConfigAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigAboutDlg.h"


// CManilla2DConfigAboutDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigAboutDlg, CM2DCTabPage)

CManilla2DConfigAboutDlg::CManilla2DConfigAboutDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigAboutDlg::IDD, CManilla2DConfigAboutDlg::IDS_TAB, CManilla2DConfigAboutDlg::IDS_TITLE)
{
}

CManilla2DConfigAboutDlg::~CManilla2DConfigAboutDlg()
{
}

void CManilla2DConfigAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CManilla2DConfigAboutDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigAboutDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigAboutDlg::OnQuerySiblings)
END_MESSAGE_MAP()

LRESULT CManilla2DConfigAboutDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

