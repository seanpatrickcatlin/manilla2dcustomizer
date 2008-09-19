#pragma once

// CM2DCTabPage dialog

class CM2DCTabPage : public CDialog
{
public:
    CM2DCTabPage(UINT nIDTemplate, CWnd* pParent = NULL) { CDialog(nIDTemplate, pParent); };
    virtual ~CM2DCTabPage() { };

	enum { IDD = 0 };

protected:
    virtual void DoDataExchange(CDataExchange* pDX) { CDialog::DoDataExchange(pDX); };    // DDX/DDV support

    virtual BOOL OnInitDialog() = 0;
    virtual void OnOK() = 0;
    virtual void OnCancel() = 0;
    virtual CString GetTabText() = 0;
};
