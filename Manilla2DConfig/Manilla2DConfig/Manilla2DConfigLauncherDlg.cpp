
#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigLauncherDlg.h"

//IMPLEMENT_DYNAMIC(CManilla2DConfigLauncherDlg, CM2DCTabPage)

// CManilla2DConfigTabsDlg dialog

CManilla2DConfigLauncherDlg::CManilla2DConfigLauncherDlg(CWnd* pParent /*=NULL*/)
	: CM2DCTabPage(CManilla2DConfigLauncherDlg::IDD, pParent)
{
}

CManilla2DConfigLauncherDlg::~CManilla2DConfigLauncherDlg()
{
}

void CManilla2DConfigLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigLauncherDlg, CDialog)
END_MESSAGE_MAP()

BOOL CManilla2DConfigLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigLauncherDlg::OnOK()
{
    CDialog::OnOK();
}

void CManilla2DConfigLauncherDlg::OnCancel()
{
    CDialog::OnCancel();
}
