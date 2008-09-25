// FullFileDialogDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>

// CFullFileDialogDlg dialog
class CFullFileDialogDlg : public CDialog
{
// Construction
public:
	CFullFileDialogDlg(CWnd* pParent, CString initialDirectory, CString fileExtension);	// standard constructor
    ~CFullFileDialogDlg();

// Dialog Data
	enum { IDD = IDD_FULLFILEDIALOG_DIALOG };

    CString GetFilePath() { return m_selectedFilePath; };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    void OnOK();
    void OnCancel();

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
    std::vector<CString> m_processedDirectories;
    CTreeCtrl m_fileTreeControl;

    bool HasDirectoryBeenProcessed(CString dirPath);

    void PopulateFileTree();
    void SetTreeSelection(CString targetDirectory);
    CString GetTreeSelection();

    CCommandBar m_cmdBar;

    CString m_targetFileExtension;
    CString m_selectedFilePath;
    CString m_initialDirectory;

    CImageList m_systemImageList;

    void AddDirectoryToFileSystemTree(HTREEITEM parentItem, CString directoryPath);

    CString GetFullFilePathFromItem(HTREEITEM hItem);

public:
    afx_msg void OnTvnItemexpandingFileTreeControl(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkFileTreeControl(NMHDR *pNMHDR, LRESULT *pResult);
};
