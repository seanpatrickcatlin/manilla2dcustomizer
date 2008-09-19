// Manilla2DConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CManilla2DConfigDlg dialog

CManilla2DConfigDlg::CManilla2DConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManilla2DConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CManilla2DConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB1, m_mainTabControl);
}

BEGIN_MESSAGE_MAP(CManilla2DConfigDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CManilla2DConfigDlg message handlers

BOOL CManilla2DConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigDlg::OnOK()
{
    CDialog::OnOK();
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