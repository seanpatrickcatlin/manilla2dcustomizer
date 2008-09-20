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

#include "M2DCTabPage.h"
#include "Manilla2DConfigUtils.h"

// CManilla2DConfigTabsDlg dialog

class CManilla2DConfigTabsDlg : public CM2DCTabPage
{
public:
	CManilla2DConfigTabsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigTabsDlg();

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_TABS_DIALOG };
    
    virtual UINT GetIDD() { return IDD_MANILLA2DCONFIG_TABS_DIALOG; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    virtual CString GetTabText();

	DECLARE_MESSAGE_MAP()

    public:
    afx_msg void OnBnClickedMoveUpButton();
    afx_msg void OnBnClickedMoveDownButton();
    afx_msg void OnBnClickedRestoreDefaultsButton();
    afx_msg void OnNMClickMainListControl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedMainListControl(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool m_bPopulatingListControl;

    NameAndEnabledState_vector_t m_newWidgetVector;
    NameAndEnabledState_vector_t m_currentWidgetVector;

    CListCtrl m_mainListControl;
    CButton m_moveUpButton;
    CButton m_moveDownButton;
    CButton m_restoreDefaultsButton;

    int GetSelectedItemIndex();
    void UpdateListControlFromNewWidgetVector();

    void PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile();
	void WriteHTCHomeSettingsXmlFileFromNewWidgetVector();
};
