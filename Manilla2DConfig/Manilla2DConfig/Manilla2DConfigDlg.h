// Manilla2DConfigDlg.h : header file
//

#pragma once

#include "afxcmn.h"
#include "M2DCTabPage.h"
#include "Manilla2DConfigUtils.h"


// CManilla2DConfigDlg dialog
class CManilla2DConfigDlg : public CDialog
{
// Construction
public:
	CManilla2DConfigDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CManilla2DConfigDlg();

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
    virtual void OnCancel();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
    CTabCtrl m_mainTabControl;
    afx_msg void OnTcnSelchangeMainTabControl(NMHDR *pNMHDR, LRESULT *pResult);

private:
    void SetRectangle();
    vector<CM2DCTabPage*> m_mainTabVector;
    int m_currentTabFocus;
    CCommandBar m_cmdBar;
};
