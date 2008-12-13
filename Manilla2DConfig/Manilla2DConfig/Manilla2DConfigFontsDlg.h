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

#pragma once

#include "SolidColorButton.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigAbstractDlg.h"
#include "afxwin.h"
#include <vector>

// CManilla2DConfigFontsDlg dialog

class CManilla2DConfigFontsDlg : public CManilla2DConfigAbstractDlg
{
	//DECLARE_DYNAMIC(CManilla2DConfigFontsDlg)

public:
	CManilla2DConfigFontsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigFontsDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_FONTS_DLG,
        IDS_TAB   = IDS_M2DC_FONTS_TAB_STR,
        IDS_TITLE = IDS_M2DC_FONTS_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

public:
    BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
    CComboBox m_fontPurposeCombo;
    CComboBox m_fontFaceCombo;
    CComboBox m_fontSizeCombo;
    CButton m_fontBoldCheck;
    CButton m_fontItalicCheck;
    CSolidColorButton m_fontColorBtn;
    CComboBox m_fontAlignCombo;
    CButton m_fontDefaultCheck;
    CButton m_fontResetAllButton;

    afx_msg void OnBnClickedFontColorBtn();
    afx_msg void OnBnClickedFontResetAllButton();
    afx_msg void OnBnClickedFontDefaultCheck();

    void OnOK();

    void AddFont(CString fontFaceName);

private:
    void EnableFontControls(BOOL bEnable = TRUE);
    void InitializeFontControls();

    std::vector<Manilla2DFontObject> m_m2dFontObjects;

    Manilla2DFontObject* GetCurrentFontSelection();
public:
    afx_msg void OnBnClickedFontBoldCheck();
    afx_msg void OnBnClickedFontItalicCheck();
    afx_msg void OnCbnSelchangeFontAlignCombo();
    afx_msg void OnCbnSelchangeFontSizeCombo();
    afx_msg void OnCbnSelchangeFontFaceCombo();
    afx_msg void OnCbnSelchangeFontPurposeCombo();
    CButton m_fontApplyAllButton;
    afx_msg void OnBnClickedFontAppyAllButton();
};
