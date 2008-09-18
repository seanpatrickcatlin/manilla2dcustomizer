// Manilla2DConfigDlg.h : header file
//

#pragma once

#include <vector>

using std::vector;

struct NameAndEnabledStateItem
{
    CString name;
    BOOL enabled;
};

typedef std::vector<NameAndEnabledStateItem> NameAndEnabledState_vector_t;

struct TodayScreenRegBackup
{
    BOOL dateEnabled;
    NameAndEnabledState_vector_t itemVector;
};

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
    CListCtrl m_mainListControl;

    CButton m_moveUpButton;
    CButton m_moveDownButton;
    CButton m_cancelButton;
    CButton m_restoreDefaultsButton;

    NameAndEnabledState_vector_t m_newWidgetVector;
    NameAndEnabledState_vector_t m_currentWidgetVector;

    TodayScreenRegBackup m_todayScreenRegBackup;

    bool HasTheStateChanged();

    int GetSelectedItemIndex();

    void UpdateListControlFromNewWidgetVector();

    void PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile();
    void PopulateNewHTCHomeSettingsXmlFileFromNewWidgetVector();

    void CopyNewHTCHomeSettingsXmlFileToWindowsDir();
    void BackupHTCHomeSettingsXmlFileIfNeeded();
    void RestoreBackupHTCHomeSettingsXmlFileToWindowsDir();

    void RefreshTodayScreen();
    void BackupTodayScreenItemsRegHive();
    void RestoreTodayScreenItemsRegHive();
    void DisableAllTodayScreenItems();

    bool FileExists(CString pathToFile);

    bool m_bPopulatingListControl;

    CString GetPathToActualHTCHomeSettingsXmlFile();
    CString GetPathToBackupHTCHomeSettingsXmlFile();
    CString GetPathToNewHTCHomeSettingsXmlFile();

    void PrintNewWidgetVectorContents();
};
