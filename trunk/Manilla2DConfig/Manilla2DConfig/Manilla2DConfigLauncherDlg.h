#pragma once


// CManilla2DConfigLauncherDlg dialog

class CManilla2DConfigLauncherDlg : public CDialog
{
	DECLARE_DYNAMIC(CManilla2DConfigLauncherDlg)

public:
	CManilla2DConfigLauncherDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigLauncherDlg();

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_LAUNCHER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
