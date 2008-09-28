/*
    This file is part of Manilla2DCustomizer.

    Manilla2DCustomizer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Manilla2DCustomizer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Manilla2DCustomizer.  If not, see <http://www.gnu.org/licenses/>.
*/

// FileTreeDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include <vector>

#include "Manilla2DConfig.h"

// CFileTreeDlg dialog
class CFileTreeDlg : public CDialog
{
// Construction
public:
	CFileTreeDlg(CWnd* pParent, CString initialDirectory, CString fileExtension);	// standard constructor
    ~CFileTreeDlg();

// Dialog Data
	enum { IDD = IDD_FILE_TREE_DLG };

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
