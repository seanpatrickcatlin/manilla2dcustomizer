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

#include <INITGUID.h>
#include <imaging.h>

HBITMAP LoadImageThumbnailWithImagingApi(const CString &strFileName, int imgWid, int imgHei);

// CManilla2DConfigThemesDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigThemesDlg, CM2DCTabPage)

CManilla2DConfigThemesDlg::CManilla2DConfigThemesDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CManilla2DConfigThemesDlg::IDD, IDS_M2DC_THEMES_STR)
{
    m_previewWidth = 75;
    m_previewHeight = 100;
}

CManilla2DConfigThemesDlg::~CManilla2DConfigThemesDlg()
{
}

void CManilla2DConfigThemesDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_M2DC_THEME_LISTBOX, m_themeChooserListBox);
    DDX_Control(pDX, IDC_M2DC_THEMES_PICTURE, m_pictureBox);
    DDX_Control(pDX, IDC_M2DC_THEME_DELETE_BTN, m_removeButton);
}

BOOL CManilla2DConfigThemesDlg::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

    CString imageFilePath = M2DC::GetPathToM2DCThemesDirectory();
    imageFilePath += "\\preview.jpg";

    RECT buttonRect;
    m_removeButton.GetWindowRect(&buttonRect);

    int buttonWidth = buttonRect.right - buttonRect.left;
    int imageHeight = (buttonWidth * 4) / 3;

    m_previewWidth = buttonWidth;
    m_previewHeight = imageHeight;

    HBITMAP previewBitmap =
        LoadImageThumbnailWithImagingApi(M2DC::GetPathToNullThemePreview(), m_previewWidth, m_previewHeight);
    m_pictureBox.SetBitmap(previewBitmap);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigThemesDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigThemesDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_M2DC_THEME_APPLY_BTN, &CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeApplyBtn)
    ON_BN_CLICKED(IDC_M2DC_THEME_IMPORT_BTN, &CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeImportBtn)
    ON_BN_CLICKED(IDC_M2DC_THEME_DELETE_BTN, &CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeDeleteBtn)
    ON_LBN_SELCHANGE(IDC_M2DC_THEME_LISTBOX, &CManilla2DConfigThemesDlg::OnLbnSelchangeM2dcThemeListbox)
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

void CManilla2DConfigThemesDlg::OnOK()
{
    if(IsWindowVisible())
    {
        OnBnClickedM2dcThemeApplyBtn();
    }
}

BOOL CManilla2DConfigThemesDlg::OnSetActive()
{
    BOOL retVal = TRUE;

    CPropertyPage::OnSetActive();

    // I used to force users to backup their original theme files, now I no longer do that

    RefreshThemeList();

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

    M2DC::SetActiveTheme(selectedTheme);

    EndDialog(IDOK);
}

#include "unzip.h"

void CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeImportBtn()
{
    CFileTreeDlg fileDlg(this, TEXT("\\"), TEXT("m2dct|zip"));
    if(fileDlg.DoModal() == IDOK)
    {
        CString pathToNewTheme = fileDlg.GetFilePath();

        if(M2DC::FileExists(pathToNewTheme))
        {
            if(!M2DC::FileIsValidM2DCTheme(pathToNewTheme))
            {
                CString msg;
                msg = TEXT("Unable to find HTCHomeSettings.xml or a hh_* file in the selected archive.\n");
                msg += TEXT("Aborting import.");

                AfxMessageBox(msg);

                return;
            }

            M2DC::AddToM2DCThemeList(pathToNewTheme);

            RefreshThemeList();
        }
    }
}

void CManilla2DConfigThemesDlg::RefreshThemeList()
{
    std::vector<CString> themeNames;
    M2DC::GetNamesOfAvailableM2DCThemes(&themeNames);

    m_themeChooserListBox.ResetContent();
    for(size_t i=0; i<themeNames.size(); i++)
    {
        m_themeChooserListBox.AddString(themeNames[i]);
    }

    // Find the longest string in the list box.
    CString str;
    CSize sz;
    int dx=0;
    CDC* pDC = m_themeChooserListBox.GetDC();
    for(int i=0; i < m_themeChooserListBox.GetCount(); i++)
    {
        m_themeChooserListBox.GetText( i, str );
        sz = pDC->GetTextExtent(str);

        if (sz.cx > dx)
        {
            dx = sz.cx;
        }
    }
    m_themeChooserListBox.ReleaseDC(pDC);
    m_themeChooserListBox.SetHorizontalExtent(dx);
}

void CManilla2DConfigThemesDlg::OnBnClickedM2dcThemeDeleteBtn()
{
    CString selectedTheme;
    int index = m_themeChooserListBox.GetCurSel();

    if((index < 0) || (index >= m_themeChooserListBox.GetCount()))
    {
        return;
    }

    m_themeChooserListBox.GetText(index, selectedTheme);

    CString pathToTheme = M2DC::GetPathOfM2DCThemeFromName(selectedTheme);

    if(M2DC::FileExists(pathToTheme))
    {
        CString msg;
        if(pathToTheme.Find(M2DC::GetPathToM2DCThemesDirectory())  == -1)
        {
            msg = TEXT("Are you sure that you would like to remove this theme from your list?");

            int ret = AfxMessageBox(msg, MB_YESNO);

            if(ret == IDYES)
            {
                M2DC::RemoveFromM2DCThemeList(selectedTheme);

                msg = TEXT("Would you like to delete the theme file?");
                ret = AfxMessageBox(msg, MB_YESNO);

                if(ret == IDYES)
                {
                    DeleteFile(pathToTheme);
                }
            }
        }
        else
        {
            msg = TEXT("To remove a file from the themes directory you must delete it.\nAre you sure you want to delete this theme file?");

            int ret = AfxMessageBox(msg, MB_YESNO);

            if(ret == IDYES)
            {
                DeleteFile(pathToTheme);
                M2DC::RemoveFromM2DCThemeList(selectedTheme);
            }
        }
    }
    
    RefreshThemeList();
}

HBITMAP LoadImageThumbnailWithImagingApi(const CString &strFileName, int imgWid, int imgHei)
{
    IImagingFactory *pImgFactory = NULL;
    IImage *pImage = NULL;
    IImage *pImageThumb = NULL;
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    HBITMAP hResult = 0;
    if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IImagingFactory,
        (void **)&pImgFactory)))
    {
        ImageInfo imageInfo;
        if (SUCCEEDED(pImgFactory->CreateImageFromFile(strFileName, &pImage))
            && SUCCEEDED(pImage->GetThumbnail(imgWid, imgHei, &pImageThumb))
            && SUCCEEDED(pImageThumb->GetImageInfo(&imageInfo)))
        {
            CWindowDC dc(0);
            CDC dcBitmap;
            dcBitmap.CreateCompatibleDC(&dc);
            hResult = CreateCompatibleBitmap(dc.GetSafeHdc(), imageInfo.Width, imageInfo.Height);
            if (hResult)
            {
                HGDIOBJ hOldBitmap = dcBitmap.SelectObject(hResult);
                pImage->Draw(dcBitmap.GetSafeHdc(), CRect(0, 0, imageInfo.Width, imageInfo.Height), NULL);
                dcBitmap.SelectObject(hOldBitmap);
            }
            pImageThumb->Release();
            pImage->Release();
        }
        pImgFactory->Release();
    }
    CoUninitialize();

    return hResult;
}

void CManilla2DConfigThemesDlg::OnLbnSelchangeM2dcThemeListbox()
{
    AfxGetApp()->BeginWaitCursor();
    CString selectedTheme;
    int index = m_themeChooserListBox.GetCurSel();

    if((index < 0) || (index >= m_themeChooserListBox.GetCount()))
    {
        return;
    }

    m_themeChooserListBox.GetText(index, selectedTheme);

    CString pathToPreview = M2DC::GetPathOfM2DCThemePreviewFromName(selectedTheme);

    HBITMAP previewBitmap = LoadImageThumbnailWithImagingApi(pathToPreview, m_previewWidth, m_previewHeight);
    m_pictureBox.SetBitmap(previewBitmap);
    AfxGetApp()->EndWaitCursor();
}
