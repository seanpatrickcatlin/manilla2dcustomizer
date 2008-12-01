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

// Manilla2DConfigAboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "version.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigAboutDlg.h"


// CManilla2DConfigAboutDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigAboutDlg, CM2DCTabPage)

CManilla2DConfigAboutDlg::CManilla2DConfigAboutDlg(CWnd* pParent /*=NULL*/)
: CManilla2DConfigAbstractDlg(pParent, CManilla2DConfigAboutDlg::IDD, CManilla2DConfigAboutDlg::IDS_TAB, CManilla2DConfigAboutDlg::IDS_TITLE)
{
}

CManilla2DConfigAboutDlg::~CManilla2DConfigAboutDlg()
{
}

void CManilla2DConfigAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_M2DC_ABOUT_VERSION, m_versionStringStaticText);
    DDX_Control(pDX, IDC_M2DC_ABOUT_DONATORS_LIST, m_donatorsListBox);
}

BOOL CManilla2DConfigAboutDlg::OnInitDialog()
{
    CManilla2DConfigAbstractDlg::OnInitDialog();

    CString versionString;
    versionString.Format(TEXT("Version %d.%d.%d"), PRODUCT_VERSION_MAJOR, PRODUCT_VERSION_MINOR, PRODUCT_VERSION_QFE);
    
    m_versionStringStaticText.SetWindowTextW(versionString);

    m_donatorsListBox.ResetContent();
    m_donatorsListBox.AddString(TEXT("My wife and daughter - Love and support"));
    m_donatorsListBox.AddString(TEXT("Myself - Countless hours of programming"));
    m_donatorsListBox.AddString(TEXT("TWolf - Program Icon"));
    m_donatorsListBox.AddString(TEXT("MobileMatt (aka mchapman007) - Theme files for development, $$$"));
    m_donatorsListBox.AddString(TEXT("kwill - $$$"));
    m_donatorsListBox.AddString(TEXT("LoriInWa - $$$"));
    m_donatorsListBox.AddString(TEXT("Mattster_spv - $$$"));
    m_donatorsListBox.AddString(TEXT("yongchoon - $$$"));
    m_donatorsListBox.AddString(TEXT("tyguy - $$$"));
    m_donatorsListBox.AddString(TEXT("G. (aka guido11) - $$$"));
    m_donatorsListBox.AddString(TEXT("Jstar (aka JstarLuvsTina) - $$$"));
    m_donatorsListBox.AddString(TEXT("Whosdaman - $$$"));
    m_donatorsListBox.AddString(TEXT("jsoneng - $$$, Beta Tester v14"));
    m_donatorsListBox.AddString(TEXT("AllTheWay - $$$"));
    m_donatorsListBox.AddString(TEXT("showaco - $$$, Beta Tester v14, $$$"));
    m_donatorsListBox.AddString(TEXT("debarron - Beta Tester v14"));
    m_donatorsListBox.AddString(TEXT("dashmaster1000 - Beta Tester v14"));
    m_donatorsListBox.AddString(TEXT("ldostang - $$$"));
    m_donatorsListBox.AddString(TEXT("micman[lalo] - $$$"));
    m_donatorsListBox.AddString(TEXT("Slaphappy100 - $$$"));
    m_donatorsListBox.AddString(TEXT("robertus3d - $$$"));
    m_donatorsListBox.AddString(TEXT("sschlesinger - $$$"));
    m_donatorsListBox.AddString(TEXT("Zak from ZaksEnterprises.com - $$$"));
    m_donatorsListBox.AddString(TEXT("scotchua - $$$"));
    m_donatorsListBox.AddString(TEXT("jesusishere - $$$"));
    m_donatorsListBox.AddString(TEXT("Jaxboater - $$$"));
    m_donatorsListBox.AddString(TEXT("RaNg3r - $$$"));
    m_donatorsListBox.AddString(TEXT("scrosler - $$$"));
    m_donatorsListBox.AddString(TEXT("Pablo Villasuso - $$$"));
    m_donatorsListBox.AddString(TEXT("epimazzo - $$$"));
    m_donatorsListBox.AddString(TEXT("skr_xd - $$$"));
    m_donatorsListBox.AddString(TEXT("..."));
    m_donatorsListBox.AddString(TEXT("<your name here for a donation of $1 or more>"));

    // Find the longest string in the list box.
    CString str;
    CSize sz;
    int dx=0;
    CDC* pDC = m_donatorsListBox.GetDC();
    for(int i=0; i < m_donatorsListBox.GetCount(); i++)
    {
        m_donatorsListBox.GetText( i, str );
        sz = pDC->GetTextExtent(str);

        if (sz.cx > dx)
        {
            dx = sz.cx;
        }
    }
    m_donatorsListBox.ReleaseDC(pDC);
    m_donatorsListBox.SetHorizontalExtent(dx);
    
    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigAboutDlg, CManilla2DConfigAbstractDlg)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigAboutDlg::OnQuerySiblings)
END_MESSAGE_MAP()

LRESULT CManilla2DConfigAboutDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

