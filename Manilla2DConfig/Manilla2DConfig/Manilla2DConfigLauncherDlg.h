#pragma once

#include "M2DCTabPage.h"

// CManilla2DConfigLauncherDlg dialog

class CManilla2DConfigLauncherDlg : public CM2DCTabPage
{
public:
	CManilla2DConfigLauncherDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CManilla2DConfigLauncherDlg();

// Dialog Data
	enum { IDD = IDD_MANILLA2DCONFIG_LAUNCHER_DIALOG };

    virtual UINT GetIDD() { return IDD_MANILLA2DCONFIG_LAUNCHER_DIALOG; };
    virtual CString GetTabText() { return TEXT("Launcher"); };

    virtual void OnOK();
    virtual void OnCancel();

protected:
    virtual BOOL CManilla2DConfigLauncherDlg::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
    int m_initialNumberOfColumns;

    CButton m_launcherThreeColumnRadioButton;
    CButton m_launcherFourColumnRadioButton;

    int GetNumberOfLauncherColumnsFromHTCHomeSettingsXml();
    void SetNumberOfLauncherColumnsFromHTCHomeSettingsXml(int numberOfColumns);
};
