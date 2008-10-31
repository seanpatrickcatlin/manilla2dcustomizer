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

// FileTreeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "ColorPickerDlg.h"
#include "WinCeFileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CColorPickerDlg dialog

CColorPickerDlg::CColorPickerDlg(CWnd* pParent)
: CManilla2DConfigAbstractDlg(pParent, CColorPickerDlg::IDD, CColorPickerDlg::IDS_TAB, CColorPickerDlg::IDS_TITLE, true)
{
    m_red = m_green = m_blue = 255;
    bHasBeenInitialized = false;
}

CColorPickerDlg::~CColorPickerDlg()
{
}

void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COLOR_RED_SLIDER, m_colorRedSlider);
    DDX_Control(pDX, IDC_COLOR_GREEN_SLIDER, m_colorGreenSlider);
    DDX_Control(pDX, IDC_COLOR_BLUE_SLIDER, m_colorBlueSlider);
    DDX_Control(pDX, IDC_COLOR_VALUE_TEXT, m_colorValueStaticText);
    DDX_Control(pDX, IDC_COLOR_EXAMPLE_BUTTON, m_colorExampleButton);
}

BEGIN_MESSAGE_MAP(CColorPickerDlg, CManilla2DConfigAbstractDlg)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CColorPickerDlg message handlers

BOOL CColorPickerDlg::OnInitDialog()
{
	CManilla2DConfigAbstractDlg::OnInitDialog();

    m_colorRedSlider.SetRange(0, 255);
    m_colorGreenSlider.SetRange(0, 255);
    m_colorBlueSlider.SetRange(0, 255);

    bHasBeenInitialized = true;
    UpdateColorControls();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CColorPickerDlg::OnOK()
{

    CDialog::OnOK();
}

void CColorPickerDlg::OnCancel()
{

    CDialog::OnCancel();
}


void CColorPickerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    SetColorValues(m_colorRedSlider.GetPos(), m_colorGreenSlider.GetPos(), m_colorBlueSlider.GetPos());

    CManilla2DConfigAbstractDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CColorPickerDlg::SetColorValues(int red, int green, int blue)
{
    if(red < 0)
    {
        m_red = 0;
    }
    else if(red > 255)
    {
        m_red = 255;
    }
    else
    {
        m_red = red;
    }

    if(green < 0)
    {
        m_green = 0;
    }
    else if(green > 255)
    {
        m_green = 255;
    }
    else
    {
        m_green = green;
    }

    if(blue < 0)
    {
        m_blue = 0;
    }
    else if(blue > 255)
    {
        m_blue = 255;
    }
    else
    {
        m_blue = blue;
    }

    UpdateColorControls();
}

void CColorPickerDlg::GetColorValues(int &red, int &green, int &blue)
{
    red = m_red;
    green = m_green;
    blue = m_blue;
}

void CColorPickerDlg::UpdateColorControls()
{
    if(bHasBeenInitialized)
    {
        m_colorRedSlider.SetPos(m_red);
        m_colorGreenSlider.SetPos(m_green);
        m_colorBlueSlider.SetPos(m_blue);

        m_colorExampleButton.SetColorValues(m_red, m_green, m_blue);

        CString colorDisplayString;
        colorDisplayString.Format(_T("%d,%d,%d"), m_red, m_green, m_blue);
        m_colorValueStaticText.SetWindowTextW(colorDisplayString);
    }
}
