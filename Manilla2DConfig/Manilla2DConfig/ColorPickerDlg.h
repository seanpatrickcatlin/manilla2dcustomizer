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

// ColorPickerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>

#include "Manilla2DConfig.h"
#include "Manilla2DConfigAbstractDlg.h"
#include "afxwin.h"

#include "SolidColorButton.h"

// CColorPickerDlg dialog
class CColorPickerDlg : public CManilla2DConfigAbstractDlg
{
// Construction
public:
	CColorPickerDlg(CWnd* pParent);	// standard constructor
    ~CColorPickerDlg();

// Dialog Data
	enum
    {
        IDD = IDD_COLOR_PICKER_DLG,
        IDS_TAB   = -1,
        IDS_TITLE = IDS_COLOR_PICKER_TITLE_STR
    };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    void OnOK();
    void OnCancel();

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
    CSliderCtrl m_colorRedSlider;
    CSliderCtrl m_colorGreenSlider;
    CSliderCtrl m_colorBlueSlider;
    CStatic m_colorValueStaticText;
    CSolidColorButton m_colorExampleButton;
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

private:
    bool bHasBeenInitialized;

    int m_red;
    int m_green;
    int m_blue;

    void UpdateColorControls();

public:
    void SetColorValues(int red, int green, int blue);
    void GetColorValues(int &red, int &green, int &blue);
    CComboBox m_colorBasicCombo;
    afx_msg void OnCbnSelchangeColorBasicCombo();
};
