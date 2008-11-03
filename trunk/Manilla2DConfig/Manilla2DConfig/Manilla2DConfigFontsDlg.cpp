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
#include "ColorPickerDlg.h"


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
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Big Icon Scrolling Tab Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\1");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Generic Tab Title Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\2");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Tab Bar Unread Msg Count Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\3");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Home Tab Missed Calls Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\HOME\\BUTTON\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Home Tab Appointment Time");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\HOME\\DATE\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Home Tab Appointment Description");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\HOME\\DATE\\1");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Music Tab Song Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\MUSIC\\INFO\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Music Tab Artist Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\MUSIC\\INFO\\1");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Music Tab Time Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\MUSIC\\INFO\\2");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Photo Tab Count Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\PHOTO\\INDICATOR\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Weather Tab Current Temperature Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\WEATHER\\CURRENTTMP\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Weather Tab High Temperature Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\WEATHER\\FORECAST\\HIGHTEMPRATURE\\LargeView");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Weather Tab Low Temperature Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\WEATHER\\FORECAST\\LOWTEMPRATURE\\LargeView");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Weather Tab Location Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\WEATHER\\LOCATION\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);
    
    fontObj.purpose = _T("Weather Tab Forecast Day Name Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\WEATHER\\FORECAST\\DAY\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Weather Tab Forecast Temperature Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\WEATHER\\FORECAST\\LOWTEMPRATURE\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    fontObj.purpose = _T("Weather Tab Error Text");
    fontObj.registryKey = _T("Software\\HTC\\Manila2D\\FONT\\WIDGET\\WEATHER\\STATUS\\0");
    M2DC::ReadManilla2DFontFromRegistry(&fontObj);
    m_m2dFontObjects.push_back(fontObj);

    InitializeFontControls();
    
    // TODO - Implement the preview feature, until then hide it
    m_fontPreviewEdit.ShowWindow(FALSE);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigFontsDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigFontsDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_FONT_COLOR_BTN, &CManilla2DConfigFontsDlg::OnBnClickedFontColorBtn)
    ON_BN_CLICKED(IDC_FONT_RESET_ALL_COMBO, &CManilla2DConfigFontsDlg::OnBnClickedFontResetAllCombo)
    ON_BN_CLICKED(IDC_FONT_DEFAULT_CHECK, &CManilla2DConfigFontsDlg::OnBnClickedFontDefaultCheck)
    ON_BN_CLICKED(IDC_FONT_BOLD_CHECK, &CManilla2DConfigFontsDlg::OnBnClickedFontBoldCheck)
    ON_BN_CLICKED(IDC_FONT_ITALIC_CHECK, &CManilla2DConfigFontsDlg::OnBnClickedFontItalicCheck)
    ON_CBN_SELCHANGE(IDC_FONT_ALIGN_COMBO, &CManilla2DConfigFontsDlg::OnCbnSelchangeFontAlignCombo)
    ON_CBN_SELCHANGE(IDC_FONT_SIZE_COMBO, &CManilla2DConfigFontsDlg::OnCbnSelchangeFontSizeCombo)
    ON_CBN_SELCHANGE(IDC_FONT_FACE_COMBO, &CManilla2DConfigFontsDlg::OnCbnSelchangeFontFaceCombo)
    ON_CBN_SELCHANGE(IDC_FONT_PURPOSE_COMBO, &CManilla2DConfigFontsDlg::OnCbnSelchangeFontPurposeCombo)
END_MESSAGE_MAP()

// CManilla2DConfigFontsDlg message handlers

LRESULT CManilla2DConfigFontsDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigFontsDlg::OnBnClickedFontColorBtn()
{   
    CColorPickerDlg cpd(this);

    int red = 0;
    int green = 0;
    int blue = 0;

    m_fontColorBtn.GetColorValues(red, green, blue);
    cpd.SetColorValues(red, green, blue);

    if(cpd.DoModal() != IDOK)
    {
        return;
    }

    // read the colors from the actual color picker dialog
    cpd.GetColorValues(red, green, blue);

    // update the button
    m_fontColorBtn.SetColorValues(red, green, blue);

    // update the font object
    Manilla2DFontObject* curFont = GetCurrentFontSelection();
    if(curFont != NULL)
    {
        curFont->fontDefault = false;

        CString colorString;

        colorString.Format(_T("%d,%d,%d"), red, green, blue);

        curFont->fontColor = colorString;
    }
}

void CManilla2DConfigFontsDlg::OnBnClickedFontResetAllCombo()
{
    // loop through all of the different Manilla2D Font Entries
    for(size_t i=0; i<m_m2dFontObjects.size(); i++)
    {
        m_m2dFontObjects[i].fontDefault = true;
    }

    // set the font purpose to the first one in the combo box
    m_fontPurposeCombo.SetCurSel(0);

    m_fontDefaultCheck.SetCheck(BST_CHECKED);
    OnBnClickedFontDefaultCheck();
}

void CManilla2DConfigFontsDlg::OnBnClickedFontDefaultCheck()
{
    BOOL fontControlEnableState = TRUE;

    if(m_fontDefaultCheck.GetCheck() == BST_CHECKED)
    {
        fontControlEnableState = FALSE;
    }

    EnableFontControls(fontControlEnableState);

    Manilla2DFontObject* curFont = GetCurrentFontSelection();
    if(curFont != NULL)
    {
        curFont->fontDefault = (fontControlEnableState == FALSE);
    }
}

void CManilla2DConfigFontsDlg::OnOK()
{
    M2DC::BeginMakingChanges();

    for(size_t i=0; i<m_m2dFontObjects.size(); i++)
    {
        M2DC::WriteManilla2DFontToRegistry(&(m_m2dFontObjects[i]));
    }
}

void CManilla2DConfigFontsDlg::EnableFontControls(BOOL bEnable/* = 1*/)
{
    m_fontColorBtn.EnableWindow(bEnable);
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

    OnCbnSelchangeFontPurposeCombo();
}

Manilla2DFontObject* CManilla2DConfigFontsDlg::GetCurrentFontSelection()
{
    Manilla2DFontObject* retVal = NULL;
    
    int curSel = m_fontPurposeCombo.GetCurSel();

    if(curSel >= 0)
    {
        CString curSelString;

        m_fontPurposeCombo.GetLBText(curSel, curSelString);

        for(size_t i=0; i<m_m2dFontObjects.size(); i++)
        {
            if(m_m2dFontObjects[i].purpose.CompareNoCase(curSelString) == 0)
            {
                retVal = &(m_m2dFontObjects[i]);
                break;
            }
        }
    }

    return retVal;
}

void CManilla2DConfigFontsDlg::OnBnClickedFontBoldCheck()
{
    Manilla2DFontObject* curFont = GetCurrentFontSelection();
    if(curFont != NULL)
    {
        curFont->fontDefault = false;

        int boldState = 0;

        if(m_fontBoldCheck.GetCheck() == BST_CHECKED)
        {
            boldState = 1;
        }

        curFont->fontBold = boldState;
    }
}

void CManilla2DConfigFontsDlg::OnBnClickedFontItalicCheck()
{
    Manilla2DFontObject* curFont = GetCurrentFontSelection();
    if(curFont != NULL)
    {
        curFont->fontDefault = false;

        int italicState = 0;

        if(m_fontItalicCheck.GetCheck() == BST_CHECKED)
        {
            italicState = 1;
        }

        curFont->fontItalic = italicState;
    }
}

void CManilla2DConfigFontsDlg::OnCbnSelchangeFontAlignCombo()
{
    Manilla2DFontObject* curFont = GetCurrentFontSelection();

    if(curFont != NULL)
    {
        curFont->fontDefault = false;

        int curSel = m_fontAlignCombo.GetCurSel();

        if(curSel >= 0)
        {
            CString curSelString;

            m_fontAlignCombo.GetLBText(curSel, curSelString);

            if(curSelString.CompareNoCase(_T("Left")) == 0)
            {
                curFont->fontFmt = 0;
            }
            else if(curSelString.CompareNoCase(_T("Center")) == 0)
            {
                curFont->fontFmt = 1;
            }
            else if(curSelString.CompareNoCase(_T("Right")) == 0)
            {
                curFont->fontFmt = 2;
            }
            else
            {
                curFont->fontFmt = 3;
            }
        }
    }
}

void CManilla2DConfigFontsDlg::OnCbnSelchangeFontSizeCombo()
{
    Manilla2DFontObject* curFont = GetCurrentFontSelection();

    if(curFont != NULL)
    {
        curFont->fontDefault = false;

        int curSel = m_fontSizeCombo.GetCurSel();

        if(curSel >= 0)
        {
            CString curSelString;

            m_fontSizeCombo.GetLBText(curSel, curSelString);

            if(curSelString.CompareNoCase(_T("<Default>")) == 0)
            {
                curFont->fontSize = 0;
            }
            else
            {
                // get the int value from the string
                int intVal;

                intVal = _wtoi(curSelString);

                curFont->fontSize = intVal;
            }
        }
    }
}

void CManilla2DConfigFontsDlg::OnCbnSelchangeFontFaceCombo()
{
    Manilla2DFontObject* curFont = GetCurrentFontSelection();

    if(curFont != NULL)
    {
        curFont->fontDefault = false;

        int curSel = m_fontFaceCombo.GetCurSel();

        if(curSel >= 0)
        {
            CString curSelString;

            m_fontFaceCombo.GetLBText(curSel, curSelString);

            if(curSelString.CompareNoCase(_T("<Default>")) == 0)
            {
                curFont->fontFace = _T("");
            }
            else
            {
                curFont->fontFace = curSelString;
            }
        }
    }
}

void CManilla2DConfigFontsDlg::OnCbnSelchangeFontPurposeCombo()
{
    Manilla2DFontObject* curFont = GetCurrentFontSelection();

    if(curFont != NULL)
    {
        if(curFont->fontFmt == 0)
        {
            m_fontAlignCombo.SelectString(0, _T("Left"));
        }
        else if(curFont->fontFmt == 1)
        {
            m_fontAlignCombo.SelectString(0, _T("Center"));
        }
        else if(curFont->fontFmt == 2)
        {
            m_fontAlignCombo.SelectString(0, _T("Right"));
        }
        else
        {
            m_fontAlignCombo.SelectString(0, _T("<Default>"));
        }

        if(curFont->fontBold == 0)
        {
            m_fontBoldCheck.SetCheck(BST_UNCHECKED);
        }
        else
        {
            m_fontBoldCheck.SetCheck(BST_CHECKED);
        }

        // deal with the color string
        int red = 0;
        int green = 0;
        int blue = 0;
        int clrPosLast = 0;
        int clrPos = 0;
        CString colorString;

        clrPos  = curFont->fontColor.Find(',', clrPos);
        colorString = curFont->fontColor.Mid(0, clrPos);
        red = _wtol(colorString);
        clrPosLast = clrPos;
        clrPosLast++;

        clrPos  = curFont->fontColor.Find(',', clrPosLast);
        colorString = curFont->fontColor.Mid(clrPosLast, clrPos-clrPosLast);
        green = _wtol(colorString);
        clrPosLast = clrPos;
        clrPosLast++;

        colorString = curFont->fontColor.Mid(clrPosLast);
        blue = _wtol(colorString);

        m_fontColorBtn.SetColorValues(red, green, blue);

        // deal with the default check
        if(curFont->fontDefault)
        {
            m_fontDefaultCheck.SetCheck(BST_CHECKED);
        }
        else
        {
            m_fontDefaultCheck.SetCheck(BST_UNCHECKED);
        }

        if(curFont->fontFace.GetLength() == 0)
        {
            m_fontFaceCombo.SelectString(0, _T("<Default>"));
        }
        else
        {
            m_fontFaceCombo.SelectString(0, curFont->fontFace);
        }

        if(curFont->fontItalic == 0)
        {
            m_fontItalicCheck.SetCheck(BST_UNCHECKED);
        }
        else
        {
            m_fontItalicCheck.SetCheck(BST_CHECKED);
        }

        if(curFont->fontSize == 0)
        {
            m_fontSizeCombo.SelectString(0, _T("<Default>"));
        }
        else
        {
            CString sizeString;
            sizeString.Format(_T("%d"), curFont->fontSize);

            m_fontSizeCombo.SelectString(0, sizeString);
        }

        // refresh the state of the controls
        OnBnClickedFontDefaultCheck();
    }
}
