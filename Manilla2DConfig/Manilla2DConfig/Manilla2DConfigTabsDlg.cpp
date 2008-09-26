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

// CManilla2DConfigTabsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigTabsDlg.h"
#include "FileTreeDlg.h"

#include "tinyxml.h"
#include "tinystr.h"

// CManilla2DConfigTabsDlg dialog

CManilla2DConfigTabsDlg::CManilla2DConfigTabsDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CManilla2DConfigTabsDlg::IDD, IDS_M2DC_TABS_STR)
{
    m_bPopulatingListControl = false;
}

CManilla2DConfigTabsDlg::~CManilla2DConfigTabsDlg()
{
}

void CManilla2DConfigTabsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_M2DC_TABS_MAIN_LC, m_mainListControl);
    DDX_Control(pDX, IDC_M2DC_TABS_UP_BTN, m_moveUpButton);
    DDX_Control(pDX, IDC_M2DC_TABS_DOWN_BTN, m_moveDownButton);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigTabsDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_MESSAGE(PSM_QUERYSIBLINGS, &CManilla2DConfigTabsDlg::OnQuerySiblings)
    ON_BN_CLICKED(IDC_M2DC_TABS_UP_BTN, &CManilla2DConfigTabsDlg::OnBnClickedMoveUpButton)
    ON_NOTIFY(NM_CLICK, IDC_M2DC_TABS_MAIN_LC, &CManilla2DConfigTabsDlg::OnNMClickMainListControl)
    ON_BN_CLICKED(IDC_M2DC_TABS_DOWN_BTN, &CManilla2DConfigTabsDlg::OnBnClickedMoveDownButton)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_M2DC_TABS_MAIN_LC, &CManilla2DConfigTabsDlg::OnLvnItemchangedMainListControl)
END_MESSAGE_MAP()

BOOL CManilla2DConfigTabsDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
    m_mainListControl.SetExtendedStyle(m_mainListControl.GetExtendedStyle()|LVS_EX_CHECKBOXES);

    PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile();
    UpdateListControlFromNewWidgetVector();

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigTabsDlg::OnOK()
{
    if(CompareNameAndEnabledStateVectors(&m_newWidgetVector, &m_currentWidgetVector))
    {
        BeginMakingChanges();
        WriteHTCHomeSettingsXmlFileFromNewWidgetVector();
    }
}

void CManilla2DConfigTabsDlg::OnPaint()
{
    CPaintDC dc(this);

    int nWidth = dc.GetDeviceCaps(HORZRES);
    const int nHeaderHeight = 24;

    // paint title
    CFont *pCurrentFont = dc.GetCurrentFont();
    LOGFONT lf;
    pCurrentFont->GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;

    CFont newFont;
    newFont.CreateFontIndirect(&lf);
    CFont *pSave = dc.SelectObject(&newFont);
    dc.SetTextColor(RGB(0, 0, 156));
    dc.DrawText(TEXT("Tab Settings"), CRect(8, 0, nWidth, nHeaderHeight), DT_VCENTER | DT_SINGLELINE); dc.SelectObject(pSave);

    // paint line
    CPen blackPen(PS_SOLID, 1, RGB(0,0,0));
    CPen *pOldPen = dc.SelectObject(&blackPen);

    dc.MoveTo(0, nHeaderHeight);
    dc.LineTo(nWidth, nHeaderHeight);

    dc.SelectObject(pOldPen); 
}

void CManilla2DConfigTabsDlg::OnBnClickedMoveUpButton()
{
    int selectedItemIndex = GetSelectedItemIndex();

    if(selectedItemIndex >= 0)
    {
        NameAndEnabledStateItem temp = m_newWidgetVector[selectedItemIndex-1];
        m_newWidgetVector[selectedItemIndex-1] = m_newWidgetVector[selectedItemIndex];
        m_newWidgetVector[selectedItemIndex] = temp;

        m_bPopulatingListControl = true;
        m_mainListControl.SetItemState(selectedItemIndex-1, LVIS_SELECTED, LVIS_SELECTED);
        m_bPopulatingListControl = false;

        UpdateListControlFromNewWidgetVector();
    }
}

void CManilla2DConfigTabsDlg::OnBnClickedMoveDownButton()
{
    int selectedItemIndex = GetSelectedItemIndex();

    if((selectedItemIndex >= 0) && (selectedItemIndex < (m_mainListControl.GetItemCount()-1)))
    {
        NameAndEnabledStateItem temp = m_newWidgetVector[selectedItemIndex+1];
        m_newWidgetVector[selectedItemIndex+1] = m_newWidgetVector[selectedItemIndex];
        m_newWidgetVector[selectedItemIndex] = temp;

        m_bPopulatingListControl = true;
        m_mainListControl.SetItemState(selectedItemIndex+1, LVIS_SELECTED, LVIS_SELECTED);
        m_bPopulatingListControl = false;

        UpdateListControlFromNewWidgetVector();
    }
}

LRESULT CManilla2DConfigTabsDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigTabsDlg::OnLvnItemchangedMainListControl(NMHDR *pNMHDR, LRESULT *pResult)
{
    (void)pResult;

    if(!m_bPopulatingListControl)
    {
        NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

        int changedItemIndex = pNMListView->iItem;

        if((changedItemIndex >= 0) && (changedItemIndex < (int)m_newWidgetVector.size()))
        {
            BOOL checkState = m_mainListControl.GetCheck(changedItemIndex);

            if(checkState != m_newWidgetVector[changedItemIndex].enabled)
            {
                m_newWidgetVector[changedItemIndex].enabled = checkState;

                m_bPopulatingListControl = true;
                m_mainListControl.SetItemState(changedItemIndex, LVIS_SELECTED, LVIS_SELECTED);
                m_bPopulatingListControl = false;
            }
        }        
    }
}

void CManilla2DConfigTabsDlg::OnNMClickMainListControl(NMHDR *pNMHDR, LRESULT *pResult)
{
    pNMHDR;
    pResult;

    int selectedItemIndex = GetSelectedItemIndex();

    BOOL upButtonState = TRUE;
    BOOL downButtonState = TRUE;

    if(selectedItemIndex == (m_mainListControl.GetItemCount()-1))
    {
        downButtonState = FALSE;
    }

    if(selectedItemIndex == 0)
    {
        upButtonState = FALSE;
    }

    m_moveDownButton.EnableWindow(downButtonState);
    m_moveUpButton.EnableWindow(upButtonState);
}

int CManilla2DConfigTabsDlg::GetSelectedItemIndex()
{
    int retVal = -1;

    POSITION pos = m_mainListControl.GetFirstSelectedItemPosition();

    if(pos != NULL)
    {
        retVal = m_mainListControl.GetNextSelectedItem(pos);
    }

    return retVal;
}

void CManilla2DConfigTabsDlg::UpdateListControlFromNewWidgetVector()
{
    m_bPopulatingListControl = true;
    int selectedIndex = GetSelectedItemIndex();

    m_mainListControl.DeleteAllItems();

    CHeaderCtrl* pListHeaderCtrl = m_mainListControl.GetHeaderCtrl();

    if(pListHeaderCtrl != NULL)
    {
        int headerItemCount = pListHeaderCtrl->GetItemCount();

        if(headerItemCount != 1)
        {
            // remove all items
            for(int i=headerItemCount; i > 0; i--)
            {
                pListHeaderCtrl->DeleteItem(i);
            }

            CRect lcRect;
            m_mainListControl.GetClientRect(&lcRect);
            m_mainListControl.InsertColumn(0, _T("Item Name"), LVCFMT_LEFT, lcRect.Width());
        }
    }

    for(size_t i=0; i<m_newWidgetVector.size(); i++)
    {
        LPCTSTR p(m_newWidgetVector[i].name);

        m_mainListControl.InsertItem(i, p);
        
        m_mainListControl.SetCheck(i, m_newWidgetVector[i].enabled);
    }

    if((selectedIndex >= 0) && (selectedIndex < m_mainListControl.GetItemCount()))
    {
        m_mainListControl.SetItemState(selectedIndex, LVIS_SELECTED, LVIS_SELECTED);
    }

    OnNMClickMainListControl(NULL, NULL);
    m_mainListControl.SetFocus();
    m_bPopulatingListControl = false;
}

void CManilla2DConfigTabsDlg::PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile()
{
    TiXmlDocument doc(GetConstCharStarFromCString(GetPathToActualHTCHomeSettingsXmlFile()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlHandle docHandle(&doc);

        TiXmlElement* tabElement = docHandle.FirstChild("HTCHome").FirstChild("Tabs").Element();

        if(tabElement != NULL)
        {
            TiXmlElement* tabItemElement = NULL;

            tabItemElement = tabElement->FirstChildElement();

            while(tabItemElement != NULL)
            {
                NameAndEnabledStateItem newTabEntry;
                newTabEntry.name = tabItemElement->Value();

                int enabled;
                tabItemElement->QueryIntAttribute("enable", &enabled);
                newTabEntry.enabled = (enabled == 0 ? FALSE : TRUE);

                m_currentWidgetVector.push_back(newTabEntry);

                tabItemElement = tabItemElement->NextSiblingElement();
            }
        }
    }

    m_newWidgetVector = m_currentWidgetVector;
}

void CManilla2DConfigTabsDlg::WriteHTCHomeSettingsXmlFileFromNewWidgetVector()
{
    TiXmlDocument doc(GetConstCharStarFromCString(GetPathToWorkingHTCHomeSettingsXmlFile()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlHandle docHandle(&doc);

        TiXmlElement* tabElement = docHandle.FirstChild("HTCHome").FirstChild("Tabs").Element();

        if(tabElement != NULL)
        {
            TiXmlElement* tabItemElement = NULL;

            tabItemElement = tabElement->FirstChildElement();

            vector<TiXmlElement> elementList;

            // go through, set the attribute for enabled, copy the element to the vector, remove the element
            while(tabItemElement != NULL)
            {
                CString elementName(tabItemElement->Value());

                for(size_t i=0; i<m_newWidgetVector.size(); i++)
                {
                    if(elementName == m_newWidgetVector[i].name)
                    {
                        tabItemElement->SetAttribute("enable", m_newWidgetVector[i].enabled);
                        elementList.push_back(*tabItemElement);
                        tabElement->RemoveChild(tabItemElement);
                        break;
                    }
                }

                tabItemElement = tabItemElement->NextSiblingElement();
            }

            // go through, set the attribute for enabled, copy the element to the vector, remove the element

            for(size_t i=0; i<m_newWidgetVector.size(); i++)
            {
                for(size_t j=0; j<elementList.size(); j++)
                {
                    CString elementName(elementList[j].Value());

                    if(elementName == m_newWidgetVector[i].name)
                    {
                        tabElement->InsertEndChild(elementList[j]);
                        break;
                    }
                }
            }
        }

        doc.SaveFile();
    }

    m_currentWidgetVector = m_newWidgetVector;
}
