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
#include "Manilla2DConfigFontsDlg.h"


// CManilla2DConfigFontsDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigFontsDlg, CM2DCTabPage)

CManilla2DConfigFontsDlg::CManilla2DConfigFontsDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigFontsDlg::IDD, CManilla2DConfigFontsDlg::IDS_TAB, CManilla2DConfigFontsDlg::IDS_TITLE)
{
}

CManilla2DConfigFontsDlg::~CManilla2DConfigFontsDlg()
{
}

void CManilla2DConfigFontsDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FONT_COLOR_BTN, m_fontColorBtn);
}

BOOL CManilla2DConfigFontsDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigFontsDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigFontsDlg::OnQuerySiblings)
END_MESSAGE_MAP()

// CManilla2DConfigFontsDlg message handlers

LRESULT CManilla2DConfigFontsDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}
