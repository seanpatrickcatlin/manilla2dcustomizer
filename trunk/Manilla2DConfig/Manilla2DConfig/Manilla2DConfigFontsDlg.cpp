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

int CALLBACK EnumFontFamProc(ENUMLOGFONT *lpelf, TEXTMETRIC *lpntm, int FontType, LPARAM lParam)
{
	CManilla2DConfigFontsDlg* pDlg = (CManilla2DConfigFontsDlg*)lParam;

	pDlg->AddFont(lpelf->elfLogFont.lfFaceName);
	return 1;
}

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
    DDX_Control(pDX, IDC_FONT_PURPOSE_COMBO, m_fontPurposeCombo);
    DDX_Control(pDX, IDC_FONT_FACE_COMBO, m_fontFaceCombo);
    DDX_Control(pDX, IDC_FONT_SIZE_COMBO, m_fontSizeCombo);
    DDX_Control(pDX, IDC_FONT_BOLD_CHECK, m_fontBoldCheck);
    DDX_Control(pDX, IDC_FONT_ITALIC_CHECK, m_fontItalicCheck);
    DDX_Control(pDX, IDC_FONT_DEFAULT_CHECK, m_fontDefaultCheck);
    DDX_Control(pDX, IDC_FONT_PREVIEW_EDIT, m_fontPreviewEdit);
    DDX_Control(pDX, IDC_FONT_RESET_ALL_COMBO, m_fontResetAllButton);
    DDX_Control(pDX, IDC_FONT_ALIGN_COMBO, m_fontAlignCombo);
}

BOOL CManilla2DConfigFontsDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    Manilla2DFontObject fontObj;

    fontObj.purpose = _T("Home Tab Title Text");
    fontObj.registryKey = _T("SOFTWARE\\HTC\\Manila2D\\FONT\\WIDGET\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Scrolling Tab Text");
    fontObj.registryKey = _T("SOFTWARE\\HTC\\Manila2D\\FONT\\WIDGET\\1");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Title Text");
    fontObj.registryKey = _T("SOFTWARE\\HTC\\Manila2D\\FONT\\WIDGET\\2");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Missed Calls Text");
    fontObj.registryKey = _T("SOFTWARE\\HTC\\Manila2D\\FONT\\WIDGET\\HOME\\BUTTON\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    InitializeFontControls();

    EnableFontControls(FALSE);

    m_fontDefaultCheck.SetCheck(BST_CHECKED);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigFontsDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigFontsDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_FONT_COLOR_BTN, &CManilla2DConfigFontsDlg::OnBnClickedFontColorBtn)
    ON_BN_CLICKED(IDC_FONT_RESET_ALL_COMBO, &CManilla2DConfigFontsDlg::OnBnClickedFontResetAllCombo)
    ON_BN_CLICKED(IDC_FONT_DEFAULT_CHECK, &CManilla2DConfigFontsDlg::OnBnClickedFontDefaultCheck)
END_MESSAGE_MAP()

// CManilla2DConfigFontsDlg message handlers

LRESULT CManilla2DConfigFontsDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigFontsDlg::OnBnClickedFontColorBtn()
{   
    m_fontColorBtn.SetColorValues(255,0,0);
}

void CManilla2DConfigFontsDlg::OnBnClickedFontResetAllCombo()
{
    EnableFontControls(FALSE);

    m_fontDefaultCheck.SetCheck(BST_CHECKED);

    // loop through all of the different Manilla2D Font Entries


    // set the font purpose to the first one in the combo box
}

void CManilla2DConfigFontsDlg::OnBnClickedFontDefaultCheck()
{
    BOOL setDefault = TRUE;

    if(m_fontDefaultCheck.GetCheck() == BST_CHECKED)
    {
        setDefault = FALSE;
    }

    EnableFontControls(setDefault);
}

void CManilla2DConfigFontsDlg::EnableFontControls(BOOL bEnable/* = 1*/)
{
    m_fontColorBtn.EnableWindow(bEnable);
    m_fontPurposeCombo.EnableWindow(bEnable);
    m_fontFaceCombo.EnableWindow(bEnable);
    m_fontSizeCombo.EnableWindow(bEnable);
    m_fontBoldCheck.EnableWindow(bEnable);
    m_fontItalicCheck.EnableWindow(bEnable);
    m_fontAlignCombo.EnableWindow(bEnable);

    if(bEnable == TRUE)
    {
        m_fontPreviewEdit.SetWindowTextW(TEXT("Preview Text AaBbCcDdEeFf"));
    }
    else
    {
        m_fontPreviewEdit.SetWindowTextW(TEXT(""));
    }
}

void CManilla2DConfigFontsDlg::AddFont(CString fontFaceName)
{
    m_fontFaceCombo.AddString(fontFaceName);
}

void CManilla2DConfigFontsDlg::InitializeFontControls()
{
    CDC*	pDC = GetDC();
	EnumFontFamilies(*pDC, NULL, (FONTENUMPROC)EnumFontFamProc, (LPARAM)this);
	ReleaseDC(pDC);

    for(size_t i=0; i<m_m2dFontObjects.size(); i++)
    {
        m_fontPurposeCombo.AddString(m_m2dFontObjects[i].purpose);
    }
    m_fontPurposeCombo.SetCurSel(0);

    m_fontSizeCombo.AddString(_T("<Default>"));
    m_fontSizeCombo.AddString(_T("8"));
    m_fontSizeCombo.AddString(_T("9"));
    m_fontSizeCombo.AddString(_T("10"));
    m_fontSizeCombo.AddString(_T("11"));
    m_fontSizeCombo.AddString(_T("12"));
    m_fontSizeCombo.AddString(_T("14"));
    m_fontSizeCombo.AddString(_T("16"));
    m_fontSizeCombo.AddString(_T("20"));
    m_fontSizeCombo.AddString(_T("24"));
    m_fontSizeCombo.AddString(_T("28"));
    m_fontSizeCombo.AddString(_T("36"));
    m_fontSizeCombo.SetCurSel(0);

    m_fontAlignCombo.AddString(_T("<Default>"));
    m_fontAlignCombo.AddString(_T("Left"));
    m_fontAlignCombo.AddString(_T("Center"));
    m_fontAlignCombo.AddString(_T("Right"));
    m_fontAlignCombo.SetCurSel(0);

    m_fontFaceCombo.AddString(_T("<Default>"));
    m_fontFaceCombo.SetCurSel(0);

    m_fontColorBtn.SetColorValues(255, 255, 255);
}