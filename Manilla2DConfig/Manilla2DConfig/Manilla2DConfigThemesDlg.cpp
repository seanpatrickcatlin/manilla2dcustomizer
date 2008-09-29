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
#include "FileTreeDlg.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigDlg.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigThemesDlg.h"


// CManilla2DConfigThemesDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigThemesDlg, CM2DCTabPage)

CManilla2DConfigThemesDlg::CManilla2DConfigThemesDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CManilla2DConfigThemesDlg::IDD, IDS_M2DC_THEMES_STR)
{
}

CManilla2DConfigThemesDlg::~CManilla2DConfigThemesDlg()
{
}

void CManilla2DConfigThemesDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_M2DC_THEME_LISTBOX, m_themeChooserListBox);
}

BOOL CManilla2DConfigThemesDlg::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigThemesDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigThemesDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_M2DC_THEME_APPLY_BTN, &CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeApplyBtn)
    ON_BN_CLICKED(IDC_M2DC_THEME_IMPORT_BTN, &CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeImportBtn)
END_MESSAGE_MAP()

// CManilla2DConfigThemesDlg message handlers

void CManilla2DConfigThemesDlg::OnPaint()
{
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
    dc.DrawText(TEXT("Theme Settings"), CRect(8, 0, nWidth, nHeaderHeight), DT_VCENTER | DT_SINGLELINE); dc.SelectObject(pSave);

    // paint line
    CPen blackPen(PS_SOLID, 1, RGB(0,0,0));
    CPen *pOldPen = dc.SelectObject(&blackPen);

    dc.MoveTo(0, nHeaderHeight);
    dc.LineTo(nWidth, nHeaderHeight);

    dc.SelectObject(pOldPen); 
}

BOOL CManilla2DConfigThemesDlg::OnSetActive()
{
    BOOL retVal = TRUE;

    CPropertyPage::OnSetActive();

    if(!IsM2DCThemeSupportEnabled())
    {
        CString enableStr = TEXT("M2DC theme support has not been activated.\n");
        enableStr += "Would you Like to enable this feature now?";

        if(MessageBox(enableStr, TEXT("M2DC Themes"), MB_YESNO) == IDNO)
        {
            retVal = FALSE;
        }

        if(retVal == TRUE)
        {
            if(EnableM2DCThemeSupport() != 0)
            {
                retVal = FALSE;
            }
        }
    }

    if(retVal == TRUE)
    {
        RefreshThemeList();
    }

    return retVal;
}

LRESULT CManilla2DConfigThemesDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeApplyBtn()
{
    CString selectedTheme;
    int index = m_themeChooserListBox.GetCurSel();

    if((index < 0) || (index >= m_themeChooserListBox.GetCount()))
    {
        return;
    }

    m_themeChooserListBox.GetText(index, selectedTheme);

    CString themePath = GetPathToM2DCThemesDirectory();
    themePath += "\\";
    themePath += selectedTheme;
    themePath += ".m2dct";

    SetActiveTheme(themePath);

    EndDialog(IDOK);
}

void CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeImportBtn()
{
    CFileTreeDlg fileDlg(this, TEXT("\\"), TEXT("m2dct|zip"));
    if(fileDlg.DoModal() == IDOK)
    {
        CString pathToNewTheme = fileDlg.GetFilePath();

        if(FileExists(pathToNewTheme))
        {
            CString destPath = GetPathToM2DCThemesDirectory();
            destPath += "\\";
            destPath += GetFileBaseName(pathToNewTheme);
            destPath += ".m2dct";

            CopyFile(pathToNewTheme, destPath, FALSE);

            RefreshThemeList();
        }
    }
}

void CManilla2DConfigThemesDlg::RefreshThemeList()
{
    std::vector<CString> themeNames;
    GetNamesOfInstalledThemes(&themeNames);

    m_themeChooserListBox.ResetContent();
    for(size_t i=0; i<themeNames.size(); i++)
    {
        m_themeChooserListBox.AddString(themeNames[i]);
    }
}
