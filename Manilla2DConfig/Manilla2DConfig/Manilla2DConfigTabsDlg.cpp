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

#include "tinyxml.h"
#include "tinystr.h"

// CManilla2DConfigTabsDlg dialog

CManilla2DConfigTabsDlg::CManilla2DConfigTabsDlg(CWnd* pParent /*=NULL*/)
	: CM2DCTabPage(CManilla2DConfigTabsDlg::IDD, pParent)
{
    m_bPopulatingListControl = false;
}

CManilla2DConfigTabsDlg::~CManilla2DConfigTabsDlg()
{
}

void CManilla2DConfigTabsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_MAIN_LIST_CONTROL, m_mainListControl);
    DDX_Control(pDX, IDC_MOVE_UP_BUTTON, m_moveUpButton);
    DDX_Control(pDX, IDC_MOVE_DOWN_BUTTON, m_moveDownButton);
    DDX_Control(pDX, IDC_RESTORE_DEFAULTS_BUTTON, m_restoreDefaultsButton);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigTabsDlg, CDialog)
    ON_BN_CLICKED(IDC_MOVE_UP_BUTTON, &CManilla2DConfigTabsDlg::OnBnClickedMoveUpButton)
    ON_BN_CLICKED(IDC_MOVE_DOWN_BUTTON, &CManilla2DConfigTabsDlg::OnBnClickedMoveDownButton)
    ON_BN_CLICKED(IDC_RESTORE_DEFAULTS_BUTTON, &CManilla2DConfigTabsDlg::OnBnClickedRestoreDefaultsButton)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAIN_LIST_CONTROL, &CManilla2DConfigTabsDlg::OnLvnItemchangedMainListControl)
    ON_NOTIFY(NM_CLICK, IDC_MAIN_LIST_CONTROL, &CManilla2DConfigTabsDlg::OnNMClickMainListControl)
END_MESSAGE_MAP()

BOOL CManilla2DConfigTabsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
    m_mainListControl.SetExtendedStyle(m_mainListControl.GetExtendedStyle()|LVS_EX_CHECKBOXES);

    BackupHTCHomeSettingsXmlFileIfNeeded();
    PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile();
    UpdateListControlFromNewWidgetVector();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigTabsDlg::OnOK()
{
	if(CompareNameAndEnabledStateVectors(&m_newWidgetVector, &m_currentWidgetVector))
    {
        WriteHTCHomeSettingsXmlFileFromNewWidgetVector();
    }

    CDialog::OnOK();
}

void CManilla2DConfigTabsDlg::OnCancel()
{
    CDialog::OnCancel();
}

CString CManilla2DConfigTabsDlg::GetTabText()
{
    return TEXT("Tabs");
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

void CManilla2DConfigTabsDlg::OnBnClickedRestoreDefaultsButton()
{
    CWaitCursor wait;

    BackupTodayScreenItemsRegHive();
    DisableAllTodayScreenItems();
    RefreshTodayScreen();
    RestoreBackupHTCHomeSettingsXmlFileToWindowsDir();
    RestoreTodayScreenItemsRegHive();
    RefreshTodayScreen();

    ::SendMessage(GetParent()->GetSafeHwnd(), WM_COMMAND, IDCANCEL, NULL);
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
    CT2CA pszConvertedAnsiString(GetPathToActualHTCHomeSettingsXmlFile());

    std::string temp(pszConvertedAnsiString);

    TiXmlDocument doc(temp.c_str());
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlNode* htcHomeNode = NULL;

        htcHomeNode = doc.FirstChild("HTCHome");

        if(htcHomeNode != NULL)
        {
            TiXmlNode* tabNode = NULL;
            TiXmlElement* tabElement = NULL;

            tabNode = htcHomeNode->FirstChild("Tabs");

            if(tabNode != NULL)
            {
                tabElement = tabNode->ToElement();

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
        }
    }

    m_newWidgetVector = m_currentWidgetVector;
}

void CManilla2DConfigTabsDlg::WriteHTCHomeSettingsXmlFileFromNewWidgetVector()
{
	CString filePath = GetPathToActualHTCHomeSettingsXmlFile();

    DWORD dwAttributes = GetFileAttributes(filePath);

    if(dwAttributes == 0xFFFFFFFF)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR WriteNew-1\nUnable to append to ");
			msg += GetPathToErrorLogFile();
			AfxMessageBox(msg);
		}
		else
		{
            fprintf(errorDump, "Unabled to get file attribute of ");
            fprintf(errorDump, GetConstCharStarFromCString(GetPathToActualHTCHomeSettingsXmlFile()));
            fprintf(errorDump, "\n");

            fprintf(errorDump, "ERROR: ");
            fprintf(errorDump, GetConstCharStarFromCString(GetWin32ErrorString(GetLastError())));
            fprintf(errorDump, "\n");
            fflush(errorDump);
            fclose(errorDump);
		}
    }

    if(SetFileAttributes(filePath, FILE_ATTRIBUTE_NORMAL) == 0)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR WriteNew-2\nUnable to append to ");
			msg += GetPathToErrorLogFile();
			AfxMessageBox(msg);
		}
		else
		{
            fprintf(errorDump, "Unabled to set file attribute of ");
            fprintf(errorDump, GetConstCharStarFromCString(GetPathToActualHTCHomeSettingsXmlFile()));
            fprintf(errorDump, " to Normal\n");

            fprintf(errorDump, "ERROR: ");
            fprintf(errorDump, GetConstCharStarFromCString(GetWin32ErrorString(GetLastError())));
            fprintf(errorDump, "\n");

            LogFileAttributes(errorDump, dwAttributes);
            fflush(errorDump);
            fclose(errorDump);
		}
    }

    CT2CA pszConvertedAnsiString(filePath);
    std::string temp(pszConvertedAnsiString);

    TiXmlDocument doc(temp.c_str());
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlNode* htcHomeNode = NULL;

        htcHomeNode = doc.FirstChild("HTCHome");

        if(htcHomeNode != NULL)
        {
            TiXmlNode* tabNode = NULL;
            TiXmlElement* tabElement = NULL;

            tabNode = htcHomeNode->FirstChild("Tabs");

            if(tabNode != NULL)
            {
                tabElement = tabNode->ToElement();

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
            }
        }

        doc.SaveFile();
    }

    m_currentWidgetVector = m_newWidgetVector;

    if(SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), dwAttributes) == 0)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR WriteNew-3\nUnable to append to ");
			msg += GetPathToErrorLogFile();
			AfxMessageBox(msg);
		}
		else
		{
            fprintf(errorDump, "Unabled to retore set file attribute of ");
            fprintf(errorDump, GetConstCharStarFromCString(GetPathToActualHTCHomeSettingsXmlFile()));
            fprintf(errorDump, "\n");

            fprintf(errorDump, "ERROR: ");
            fprintf(errorDump, GetConstCharStarFromCString(GetWin32ErrorString(GetLastError())));
            fprintf(errorDump, "\n");

            LogFileAttributes(errorDump, dwAttributes);
            fflush(errorDump);
            fclose(errorDump);
		}
    }
}
