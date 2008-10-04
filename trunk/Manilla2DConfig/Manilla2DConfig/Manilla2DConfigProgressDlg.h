#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "Manilla2DConfig.h"

// CManilla2DConfigProgressDlg dialog

class CManilla2DConfigProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CManilla2DConfigProgressDlg)

public:
	CManilla2DConfigProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigProgressDlg();

// Dialog Data
	enum { IDD = IDD_M2DC_PROGRESS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnBnClickedM2dcProgressCancelBtn();
    afx_msg void OnTimer(UINT nIDEvent);
    void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

    BOOL OnInitDialog();
    void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
    CStatic m_messageText;
    CProgressCtrl m_progressCtrl;

    int m_lastVal;
    int m_curVal;

    CString m_curMsg;
    CString m_lastMsg;

    int m_minVal;
    int m_maxVal;
    
    UINT m_timerId;
    bool m_updateMessage;
    bool m_updateProgress;
    bool m_userPressedCancel;
    bool m_pumpingMessages;
    

    CWnd* m_parent;

    void PumpMessages();

public:
    void BeginTrackingProgress(CString message, int min, int max);
    void EndTrackingProgress();
    int UpdateStatus(CString message, int newVal);
};
