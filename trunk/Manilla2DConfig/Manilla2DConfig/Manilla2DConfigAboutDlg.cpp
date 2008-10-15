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
#include "Manilla2DConfigAboutDlg.h"


// CManilla2DConfigAboutDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigAboutDlg, CM2DCTabPage)

CManilla2DConfigAboutDlg::CManilla2DConfigAboutDlg(CWnd* pParent /*=NULL*/)
: CPropertyPage(CManilla2DConfigAboutDlg::IDD, CManilla2DConfigAboutDlg::IDS_TAB)
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
    CPropertyPage::OnInitDialog();

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigAboutDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigAboutDlg::OnQuerySiblings)
END_MESSAGE_MAP()

// CManilla2DConfigAboutDlg message handlers

void CManilla2DConfigAboutDlg::OnPaint()
{
    CString titleStr;
    titleStr.LoadStringW(CManilla2DConfigAboutDlg::IDS_TITLE);

    CPaintDC dc(this);

    int nWidth = dc.GetDeviceCaps(HORZRES);
    const int nHeaderHeight = 24;

    // paint title
    CFont *pCurrentFont = dc.GetCurrentFont();
    LOGFONT lf;
    pCurrentFont->GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;

    CFont newFont;
    newFont.CreateFontIndirect(&lf);
    CFont *pSave = dc.SelectObject(&newFont);
    dc.SetTextColor(RGB(0, 0, 156));
    dc.DrawText(titleStr, CRect(8, 0, nWidth, nHeaderHeight), DT_VCENTER | DT_SINGLELINE); dc.SelectObject(pSave);

    // paint line
    CPen blackPen(PS_SOLID, 1, RGB(0,0,0));
    CPen *pOldPen = dc.SelectObject(&blackPen);

    dc.MoveTo(0, nHeaderHeight);
    dc.LineTo(nWidth, nHeaderHeight);

    dc.SelectObject(pOldPen); 
}

LRESULT CManilla2DConfigAboutDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}
