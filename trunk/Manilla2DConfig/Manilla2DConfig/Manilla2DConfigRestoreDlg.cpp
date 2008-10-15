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
: CPropertyPage(CManilla2DConfigRestoreDlg::IDD, CManilla2DConfigRestoreDlg::IDS_TAB)
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
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigRestoreDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigRestoreDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_M2DC_RESTORE_BACKUP_BTN, &CManilla2DConfigRestoreDlg::OnBnClickedBackupButton)
    ON_BN_CLICKED(IDC_M2DC_RESTORE_RESTORE_BTN, &CManilla2DConfigRestoreDlg::OnBnClickedRestoreButton)
    ON_BN_CLICKED(IDC_M2DC_RESTORE_BACKUP_THEME_BTN, &CManilla2DConfigRestoreDlg::OnBnClickedM2dcRestoreBackupThemeBtn)
    ON_BN_CLICKED(IDC_M2DC_RESTORE_RESTORE_THEME_BTN, &CManilla2DConfigRestoreDlg::OnBnClickedM2dcRestoreRestoreThemeBtn)
END_MESSAGE_MAP()

// CManilla2DConfigRestoreDlg message handlers

void CManilla2DConfigRestoreDlg::OnPaint()
{
    CString titleStr;
    titleStr.LoadStringW(CManilla2DConfigRestoreDlg::IDS_TITLE);

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

LRESULT CManilla2DConfigRestoreDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigRestoreDlg::OnBnClickedRestoreButton()
{
    CString caption("This will restore all Manilla 2D xml settings from your backup copy.\nWould you like to continue?");
    if(MessageBox(caption, TEXT("Restore Settings?"), MB_YESNO) == IDYES)
    {
        M2DC::RestoreHTCHomeSettingsXmlFromBackup();
        EndDialog(IDCANCEL);
    }
}

void CManilla2DConfigRestoreDlg::OnBnClickedBackupButton()
{
    CString caption("This will backup you Manilla 2D xml settings and delete any previous backups.\nWould you like to continue?");
    if(MessageBox(caption, TEXT("Backup Settings?"), MB_YESNO) == IDYES)
    {
        M2DC::BackupHTCHomeSettingsXml(true);
    }
}

void CManilla2DConfigRestoreDlg::OnBnClickedM2dcRestoreBackupThemeBtn()
{
    CString caption("This will backup you Manilla 2D theme files and delete any previous backups.\nWould you like to continue?");
    if(MessageBox(caption, TEXT("Backup Settings?"), MB_YESNO) == IDYES)
    {
        M2DC::BackupActiveTheme(true);
    }
}

void CManilla2DConfigRestoreDlg::OnBnClickedM2dcRestoreRestoreThemeBtn()
{
    CString caption("This will restore all Manilla 2D theme files from your backup copy.\nWould you like to continue?");
    if(MessageBox(caption, TEXT("Restore Settings?"), MB_YESNO) == IDYES)
    {
        M2DC::RestoreActiveThemeFromBackup();
        EndDialog(IDCANCEL);
    }
}
