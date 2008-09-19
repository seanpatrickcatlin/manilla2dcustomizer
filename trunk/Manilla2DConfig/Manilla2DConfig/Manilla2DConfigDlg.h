// Manilla2DConfigDlg.h : header file
//

#pragma once

#include "Manilla2DConfigUtils.h"

// CManilla2DConfigDlg dialog
class CManilla2DConfigDlg : public CDialog
{
// Construction
public:
	CManilla2DConfigDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
    virtual void OnOK();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
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
