// Manilla2DConfigProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigProgressDlg.h"
#include "Manilla2DConfigUtils.h"

#define PROGRESS_TIMER_INTERVAL 250

// CManilla2DConfigProgressDlg dialog

IMPLEMENT_DYNAMIC(CManilla2DConfigProgressDlg, CDialog)

CManilla2DConfigProgressDlg::CManilla2DConfigProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManilla2DConfigProgressDlg::IDD, pParent)
{
    // tell CDialog that we don't want a fullscreen dialog
    m_bFullScreen = false;

    // setup defualt values
    m_minVal = 0;
    m_maxVal = 100;
    m_curVal = 0;
    m_userPressedCancel = false;
    m_updateMessage = false;
    m_updateProgress = false;
    m_pumpingMessages = false;
    m_parent = pParent;
}

CManilla2DConfigProgressDlg::~CManilla2DConfigProgressDlg()
{
    DestroyWindow();
}

BOOL CManilla2DConfigProgressDlg::OnInitDialog()
{
    BOOL retVal = CDialog::OnInitDialog();
    return retVal;
}

void CManilla2DConfigProgressDlg::OnDestroy()
{
    if(m_timerId != 0)
    {
        KillTimer(m_timerId);
    }

    CDialog::OnDestroy();
}

void CManilla2DConfigProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_M2DC_PROGRESS_MESSAGE_ST, m_messageText);
    DDX_Control(pDX, IDC_M2DC_PROGRESS_CTRL, m_progressCtrl);
}

BEGIN_MESSAGE_MAP(CManilla2DConfigProgressDlg, CDialog)
    ON_BN_CLICKED(IDC_M2DC_PROGRESS_CANCEL_BTN, &CManilla2DConfigProgressDlg::OnBnClickedM2dcProgressCancelBtn)
    ON_COMMAND(IDCANCEL, &CManilla2DConfigProgressDlg::OnBnClickedM2dcProgressCancelBtn)
    ON_WM_ACTIVATE()
	ON_WM_SETTINGCHANGE()
    ON_WM_DESTROY()
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CManilla2DConfigProgressDlg message handlers

void CManilla2DConfigProgressDlg::OnBnClickedM2dcProgressCancelBtn()
{
    m_userPressedCancel = true;
}

void CManilla2DConfigProgressDlg::OnTimer(UINT nIDEvent)
{
    if((nIDEvent == m_timerId) && (!m_userPressedCancel))
    {
        if(m_curMsg != m_lastMsg)
        {
            m_messageText.SetWindowText(m_curMsg);
            m_lastMsg = m_curMsg;
        }

        if(m_curVal != m_lastVal)
        {
            m_progressCtrl.SetPos(m_curVal);
            m_lastVal = m_curVal;
        }
    }

    CDialog::OnTimer(nIDEvent);
}

void CManilla2DConfigProgressDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CManilla2DConfigProgressDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CWnd::OnSettingChange(uFlags, lpszSection);
}

void CManilla2DConfigProgressDlg::BeginTrackingProgress(CString message, int min, int max)
{
    m_minVal = min;
    m_maxVal = max;

    m_curVal = min;
    m_curMsg = message;

    Create(CManilla2DConfigProgressDlg::IDD, m_parent);

    m_progressCtrl.SetRange32(m_minVal, m_maxVal);
    
    m_timerId = SetTimer(IDT_M2DC_PROGRESS_TIMER, PROGRESS_TIMER_INTERVAL, 0);

    SetWindowText(message);

    ShowWindow(SW_SHOW);

    PumpMessages();
}

void CManilla2DConfigProgressDlg::EndTrackingProgress()
{
    ShowWindow(SW_HIDE);
}

int CManilla2DConfigProgressDlg::UpdateStatus(CString message, int newVal)
{ 
    int retVal = 0;

    m_curMsg = message;
    m_curVal = newVal;

    if((m_curMsg != m_lastMsg) || (m_curVal != m_lastVal))
    {
        PumpMessages();
    }

    if(m_userPressedCancel)
    {
        retVal = -1;
    }

    return retVal;
}

void CManilla2DConfigProgressDlg::PumpMessages()
{
    // Must call Create() before using the dialog
    ASSERT(m_hWnd != NULL);

    MSG msg;
    // Handle dialog messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if(!IsDialogMessage(&msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);  
        }
    }
}
