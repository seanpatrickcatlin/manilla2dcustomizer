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

// Manilla2DConfigHomeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigHomeDlg.h"


// CManilla2DConfigHomeDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigHomeDlg, CPropertyPage)

CManilla2DConfigHomeDlg::CManilla2DConfigHomeDlg(CWnd* pParent /*=NULL*/)
: CPropertyPage(CManilla2DConfigHomeDlg::IDD, CManilla2DConfigHomeDlg::IDS_TAB)
{

}

CManilla2DConfigHomeDlg::~CManilla2DConfigHomeDlg()
{
}

void CManilla2DConfigHomeDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_M2DC_ENABLE_ANALOG_CB, m_enableAnalogClockCheckBox);
    DDX_Control(pDX, IDC_M2DC_ENABLE_DIGITAL_CB, m_enableDigitalClockCheckBox);
    DDX_Control(pDX, IDC_M2DC_ENABLE_MISSEDCALLS_CB, m_enableMissedCallsCheckBox);
    DDX_Control(pDX, IDC_M2DC_ENABLE_CALENDAR_CB, m_enableCalendarCheckBox);
    DDX_Control(pDX, IDC_M2DC_ENABLE_ALARM_CB, m_enableAlarmStatusCheckBox);
    DDX_Control(pDX, IDC_M2DC_ENABLE_LINE1_CB, m_enableLine1CheckBox);
    DDX_Control(pDX, IDC_M2DC_ENABLE_LINE2_CB, m_enableLine2CheckBox);
    DDX_Control(pDX, IDC_M2DC_ENABLE_LINE3_CB, m_enableLine3CheckBox);
}

BOOL CManilla2DConfigHomeDlg::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

    M2DC::GetHomeWidgetSettings(M2DC::GetPathToHTCHomeSettingsXmlFileActual(), &m_homeSettings);

    int check = m_homeSettings.bAnalogClockEnabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableAnalogClockCheckBox.SetCheck(check);

    check = m_homeSettings.bDigitalClockEnabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableDigitalClockCheckBox.SetCheck(check);

    check = m_homeSettings.bMissedCallsEnabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableMissedCallsCheckBox.SetCheck(check);

    check = m_homeSettings.bCalendarEnabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableCalendarCheckBox.SetCheck(check);

    check = m_homeSettings.bAlarmStatusEnabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableAlarmStatusCheckBox.SetCheck(check);

    check = m_homeSettings.bLine1Enabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableLine1CheckBox.SetCheck(check);

    check = m_homeSettings.bLine2Enabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableLine2CheckBox.SetCheck(check);

    check = m_homeSettings.bLine2Enabled ? BST_CHECKED : BST_UNCHECKED;
    m_enableLine3CheckBox.SetCheck(check);

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigHomeDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigHomeDlg::OnQuerySiblings)
END_MESSAGE_MAP()

void CManilla2DConfigHomeDlg::OnPaint()
{
    CString titleStr;
    titleStr.LoadStringW(CManilla2DConfigHomeDlg::IDS_TITLE);

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

LRESULT CManilla2DConfigHomeDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigHomeDlg::OnOK()
{
    HomeWidgetSettings newSettings;

    newSettings.bAnalogClockEnabled = (m_enableAnalogClockCheckBox.GetCheck() == BST_CHECKED);
    newSettings.bDigitalClockEnabled = (m_enableDigitalClockCheckBox.GetCheck() == BST_CHECKED);
    newSettings.bMissedCallsEnabled = (m_enableMissedCallsCheckBox.GetCheck() == BST_CHECKED);
    newSettings.bCalendarEnabled = (m_enableCalendarCheckBox.GetCheck() == BST_CHECKED);
    newSettings.bAlarmStatusEnabled = (m_enableAlarmStatusCheckBox.GetCheck() == BST_CHECKED);
    newSettings.bLine1Enabled = (m_enableLine1CheckBox.GetCheck() == BST_CHECKED);
    newSettings.bLine2Enabled = (m_enableLine2CheckBox.GetCheck() == BST_CHECKED);
    newSettings.bLine3Enabled = (m_enableLine3CheckBox.GetCheck() == BST_CHECKED);

    if((newSettings.bAnalogClockEnabled != m_homeSettings.bAnalogClockEnabled) ||
        (newSettings.bDigitalClockEnabled != m_homeSettings.bDigitalClockEnabled) ||
        (newSettings.bMissedCallsEnabled != m_homeSettings.bMissedCallsEnabled) ||
        (newSettings.bCalendarEnabled != m_homeSettings.bCalendarEnabled) ||
        (newSettings.bAlarmStatusEnabled != m_homeSettings.bAlarmStatusEnabled) ||
        (newSettings.bLine1Enabled != m_homeSettings.bLine1Enabled) ||
        (newSettings.bLine2Enabled != m_homeSettings.bLine2Enabled) ||
        (newSettings.bLine3Enabled != m_homeSettings.bLine3Enabled))
    {
        M2DC::SetHomeWidgetSettings(M2DC::GetPathToHTCHomeSettingsXmlFileWorking(), &newSettings);
    }
}
