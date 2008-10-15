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

#include "Manilla2DConfigUtils.h"
#include "afxwin.h"


// CManilla2DConfigHomeDlg dialog

class CManilla2DConfigHomeDlg : public CPropertyPage
{
	//DECLARE_DYNAMIC(CManilla2DConfigHomeDlg)

public:
	CManilla2DConfigHomeDlg(CWnd* pParent = NULL);
	virtual ~CManilla2DConfigHomeDlg();

// Dialog Data
    enum
    {
        IDD       = IDD_M2DC_HOME_DLG,
        IDS_TAB   = IDS_M2DC_HOME_TAB_STR,
        IDS_TITLE = IDS_M2DC_HOME_TITLE_STR
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    LRESULT OnQuerySiblings(WPARAM wParam, LPARAM lParam);

    void OnPaint();

public:
    BOOL OnInitDialog();
    void OnOK();

	DECLARE_MESSAGE_MAP()

private:
    CCommandBar m_cmdBar;

    HomeWidgetSettings m_homeSettings;

public:
    CButton m_enableAnalogClockCheckBox;
    CButton m_enableDigitalClockCheckBox;
    CButton m_enableMissedCallsCheckBox;
    CButton m_enableCalendarCheckBox;
    CButton m_enableAlarmStatusCheckBox;
    CButton m_enableLine1CheckBox;
    CButton m_enableLine2CheckBox;
    CButton m_enableLine3CheckBox;
};
