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

// Manilla2DConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Afxext.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigDlg.h"
#include "Manilla2DConfigHomeDlg.h"
#include "Manilla2DConfigTabsDlg.h"
#include "Manilla2DConfigAboutDlg.h"
#include "Manilla2DConfigThemesDlg.h"
#include "Manilla2DConfigHomeRegDlg.h"
#include "Manilla2DConfigRestoreDlg.h"
#include "Manilla2DConfigSoftKeysDlg.h"
#include "Manilla2DConfigLauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CManilla2DConfigDlg property sheet
	
CManilla2DConfigDlg::CManilla2DConfigDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CManilla2DConfigDlg::CManilla2DConfigDlg(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CManilla2DConfigDlg::~CManilla2DConfigDlg()
{
    for(size_t i=0; i<m_mainTabVector.size(); i++)
    {
        CPropertyPage* currentTabPage = m_mainTabVector[i];;

        if(currentTabPage != NULL)
        {
            delete currentTabPage;
        }
    }
}

BEGIN_MESSAGE_MAP(CManilla2DConfigDlg, CPropertySheet)
END_MESSAGE_MAP()

// CManilla2DConfigDlg message handlers

BOOL CManilla2DConfigDlg::OnInitDialog()
{
    CPropertySheet::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigDlg::SetupPages()
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_M2DC_ICO);

    // here is where we add new tabpages, the rest should be done automatically
    AddNewTabPage(new CManilla2DConfigTabsDlg(this));
    AddNewTabPage(new CManilla2DConfigHomeDlg(this));
    AddNewTabPage(new CManilla2DConfigHomeRegDlg(this));
    AddNewTabPage(new CManilla2DConfigSoftKeysDlg(this));
    AddNewTabPage(new CManilla2DConfigLauncherDlg(this));
    AddNewTabPage(new CManilla2DConfigThemesDlg(this));
    AddNewTabPage(new CManilla2DConfigRestoreDlg(this));
    AddNewTabPage(new CManilla2DConfigAboutDlg(this));

    for(size_t i=0; i<m_mainTabVector.size(); i++)
    {
        CPropertyPage* currentTabPage = m_mainTabVector[i];

        if(currentTabPage != NULL)
        {
            AddPage(currentTabPage);
        }
    }
}

void CManilla2DConfigDlg::AddNewTabPage(CPropertyPage* newTabPage)
{
    if(newTabPage != NULL)
    {
        m_mainTabVector.push_back(newTabPage);
    }
}
