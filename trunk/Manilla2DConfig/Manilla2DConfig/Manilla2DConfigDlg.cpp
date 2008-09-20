// Manilla2DConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Afxext.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigDlg.h"
#include "Manilla2DConfigTabsDlg.h"
#include "Manilla2DConfigLauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CManilla2DConfigDlg dialog

CManilla2DConfigDlg::CManilla2DConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManilla2DConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    // here is where we add new tabpages, the rest should be done automatically

    CM2DCTabPage* newTabPage = new CManilla2DConfigTabsDlg(); 
    if(newTabPage != NULL)
    {
        m_mainTabVector.push_back(newTabPage);
    }

    newTabPage = new CManilla2DConfigLauncherDlg();
    if(newTabPage != NULL)
    {
        m_mainTabVector.push_back(newTabPage);
    }

    m_currentTabFocus = 0;
}

CManilla2DConfigDlg::~CManilla2DConfigDlg()
{
    for(size_t i=0; i<m_mainTabVector.size(); i++)
    {
        CM2DCTabPage* currentTabPage = m_mainTabVector[i];;

        if(currentTabPage != NULL)
        {
            delete currentTabPage;
        }
    }
}

void CManilla2DConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MAIN_TAB_CONTROL, m_mainTabControl);
}

BEGIN_MESSAGE_MAP(CManilla2DConfigDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
    ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB_CONTROL, &CManilla2DConfigDlg::OnTcnSelchangeMainTabControl)
END_MESSAGE_MAP()

// CManilla2DConfigDlg message handlers

BOOL CManilla2DConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    for(size_t i=0; i<m_mainTabVector.size(); i++)
    {
        CM2DCTabPage* currentTabPage = m_mainTabVector[i];

        if(currentTabPage != NULL)
        {
            currentTabPage->Create(currentTabPage->GetIDD(), &m_mainTabControl);
        }
    }

    for(size_t i=0; i<m_mainTabVector.size(); i++)
    {
        int showState = SW_HIDE;

        if(i==0)
        {
            showState = SW_SHOW;
        }

        CM2DCTabPage* currentTabPage = m_mainTabVector[i];

        if(currentTabPage != NULL)
        {
            currentTabPage->ShowWindow(showState);
            m_mainTabControl.InsertItem(i, currentTabPage->GetTabText());
        }
    }

    SetRectangle();

    CM2DCTabPage* page1 = m_mainTabVector[0];
    if(page1 != NULL)
    {
        page1->ShowWindow(SW_SHOW);
        page1->GetFocus();
    }

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_OKCANCELMENU);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigDlg::OnOK()
{
    CWaitCursor wait;

    BackupTodayScreenItemsRegHive();
    DisableAllTodayScreenItems();
    RefreshTodayScreen();

    for(size_t i=0; i<m_mainTabVector.size(); i++)
    {
        CM2DCTabPage* currentTabPage = m_mainTabVector[i];

        if(currentTabPage != NULL)
        {
            currentTabPage->OnOK();
        }
    }

    RestoreTodayScreenItemsRegHive();
    RefreshTodayScreen();

    CDialog::OnOK();
}

void CManilla2DConfigDlg::OnCancel()
{
    for(size_t i=0; i<m_mainTabVector.size(); i++)
    {
        CM2DCTabPage* currentTabPage = m_mainTabVector[i];

        if(currentTabPage != NULL)
        {
            currentTabPage->OnCancel();
        }
    }

    CDialog::OnCancel();
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CManilla2DConfigDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
        /*
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_MANILLA2DCONFIG_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_MANILLA2DCONFIG_DIALOG));
            */
	}
}
#endif

void CManilla2DConfigDlg::OnTcnSelchangeMainTabControl(NMHDR *pNMHDR, LRESULT *pResult)
{
    (void)pResult;
    (void)pNMHDR;

    if(m_currentTabFocus != m_mainTabControl.GetCurFocus())
    {
        CM2DCTabPage* oldTab = m_mainTabVector[m_currentTabFocus];

        if(oldTab != NULL)
        {
            m_mainTabVector[m_currentTabFocus]->ShowWindow(SW_HIDE);
        }

        m_currentTabFocus = m_mainTabControl.GetCurFocus();

        CM2DCTabPage* newTab = m_mainTabVector[m_currentTabFocus];

        if(newTab != NULL)
        {
            m_mainTabVector[m_currentTabFocus]->ShowWindow(SW_SHOW);
            m_mainTabVector[m_currentTabFocus]->SetFocus();
        }
    }
}

void CManilla2DConfigDlg::SetRectangle()
{
	CRect tabRect, itemRect;
	int nX, nY, nXc, nYc;

	m_mainTabControl.GetClientRect(&tabRect);
	m_mainTabControl.GetItemRect(0, &itemRect);

	nX=tabRect.left+1;
	nY=tabRect.top+1;
	nXc=tabRect.right-2;
    nYc=itemRect.top-tabRect.top-2;
    
	for(size_t i=0; i < m_mainTabVector.size(); i++)
    {
        UINT showFlag = SWP_HIDEWINDOW;

        if(i==0)
        {
            showFlag = SWP_SHOWWINDOW;
        }

        CM2DCTabPage* currentTabPage = m_mainTabVector[i];

        if(currentTabPage != NULL)
        {
		    currentTabPage->SetWindowPos(&m_mainTabControl.wndTop, nX, nY, nXc, nYc, SWP_HIDEWINDOW);
        }
	}
}
