#pragma once


#include "Manilla2DConfigUtils.h"

// CManilla2DConfigTabsDlg dialog

class CManilla2DConfigTabsDlg : public CDialog
{
	DECLARE_DYNAMIC(CManilla2DConfigTabsDlg)

public:
	CManilla2DConfigTabsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigTabsDlg();

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_TABS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    virtual void OnOK();

	DECLARE_MESSAGE_MAP()

    public:
    afx_msg void OnBnClickedMoveUpButton();
    afx_msg void OnBnClickedMoveDownButton();
    afx_msg void OnBnClickedCancelButton();
    afx_msg void OnBnClickedRestoreDefaultsButton();
    afx_msg void OnNMClickMainListControl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnItemchangedMainListControl(NMHDR *pNMHDR, LRESULT *pResult);

private:
	bool m_bPopulatingListControl;

    NameAndEnabledState_vector_t m_newWidgetVector;
    NameAndEnabledState_vector_t m_currentWidgetVector;

    TodayScreenRegBackup m_todayScreenRegBackup;

    CListCtrl m_mainListControl;
    CButton m_moveUpButton;
    CButton m_moveDownButton;
    CButton m_cancelButton;
    CButton m_restoreDefaultsButton;

    int GetSelectedItemIndex();
    void UpdateListControlFromNewWidgetVector();

    void PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile();
	void WriteHTCHomeSettingsXmlFileFromNewWidgetVector();
	
	void BackupTodayScreenItemsRegHive();
	void RestoreTodayScreenItemsRegHive();
};
