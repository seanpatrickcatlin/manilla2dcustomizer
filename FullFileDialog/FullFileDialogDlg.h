// FullFileDialogDlg.h : header file
//

#pragma once
#include "afxcmn.h"

// CFullFileDialogDlg dialog
class CFullFileDialogDlg : public CDialog
{
// Construction
public:
	CFullFileDialogDlg(CWnd* pParent, CString initialDirectory, CString fileExtension);	// standard constructor
    ~CFullFileDialogDlg();

// Dialog Data
	enum { IDD = IDD_FULLFILEDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    CString GetFilePath();

    void OnOK();
    void OnCancel();

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
    CTreeCtrl m_fileTreeControl;

    void PopulateFileTree();
    void SetTreeSelection(CString targetDirectory);
    CString GetTreeSelection();

    CCommandBar m_cmdBar;

    CString m_targetFileExtension;
    CString m_selectedFilePath;
    CString m_initialDirectory;

    CImageList m_systemImageList;

    void AddDirectoryToFileSystemTree(HTREEITEM parentItem, CString directoryName);
};
