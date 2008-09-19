// CManilla2DConfigTabsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigTabsDlg.h"

#include "tinyxml.h"
#include "tinystr.h"

// CManilla2DConfigTabsDlg dialog

IMPLEMENT_DYNAMIC(CManilla2DConfigTabsDlg, CDialog)

CManilla2DConfigTabsDlg::CManilla2DConfigTabsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManilla2DConfigTabsDlg::IDD, pParent)
{
    m_bPopulatingListControl = false;
    m_todayScreenRegBackup.dateEnabled = 0;
    m_todayScreenRegBackup.itemVector.clear();
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
    DDX_Control(pDX, IDC_CANCEL_BUTTON, m_cancelButton);
    DDX_Control(pDX, IDC_RESTORE_DEFAULTS_BUTTON, m_restoreDefaultsButton);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigTabsDlg, CDialog)
    ON_BN_CLICKED(IDC_MOVE_UP_BUTTON, &CManilla2DConfigTabsDlg::OnBnClickedMoveUpButton)
    ON_BN_CLICKED(IDC_MOVE_DOWN_BUTTON, &CManilla2DConfigTabsDlg::OnBnClickedMoveDownButton)
    ON_BN_CLICKED(IDC_CANCEL_BUTTON, &CManilla2DConfigTabsDlg::OnBnClickedCancelButton)
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
        CWaitCursor wait;

        BackupTodayScreenItemsRegHive();
        DisableAllTodayScreenItems();
        RefreshTodayScreen();
        WriteHTCHomeSettingsXmlFileFromNewWidgetVector();
        RestoreTodayScreenItemsRegHive();
        RefreshTodayScreen();
    }

    if(FileExists(GetPathToErrorLogFile()))
    {
        CString msg("A log file of errors has been generated ");
        msg += GetPathToErrorLogFile();
        msg += "\nPlease attach this file when reporting errors.";

        AfxMessageBox(msg);
    }

    CDialog::OnOK();
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

void CManilla2DConfigTabsDlg::OnBnClickedCancelButton()
{
    OnCancel();
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

    if(FileExists(GetPathToErrorLogFile()))
    {
        CString msg("A log file of errors has been generated ");
        msg += GetPathToErrorLogFile();
        msg += "\nPlease attach this file when reporting errors.";

        AfxMessageBox(msg);
    }

    OnCancel();
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

void CManilla2DConfigTabsDlg::BackupTodayScreenItemsRegHive()
{
    m_todayScreenRegBackup.dateEnabled = FALSE;
    m_todayScreenRegBackup.itemVector.clear();

    HKEY mainHKey;
    DWORD enabledState = 0;
    DWORD varSizeDword = sizeof(DWORD);
    DWORD varTypeDword = REG_DWORD;
    CString mainKeyName, subKeyName, valueName;

    // backup the date enabled state
    mainKeyName = "\\Software\\Microsoft\\Today";
    
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainKeyName, 0, 0, &mainHKey) == ERROR_SUCCESS)
    {
        valueName = "Date";

        RegQueryValueEx(mainHKey, valueName, NULL, &varTypeDword, (LPBYTE)&enabledState, &varSizeDword);

        RegCloseKey(mainHKey);
    }

    m_todayScreenRegBackup.dateEnabled = enabledState;

    // enumerate the sub items of the today screen
    mainKeyName = "\\Software\\Microsoft\\Today\\Items";

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainKeyName, 0, 0, &mainHKey) == ERROR_SUCCESS)
    {
        HKEY subHKey;
        DWORD subKeyIndex = 0;
        DWORD subKeyBufferSize = MAX_PATH;
        TCHAR subKeyBuffer[MAX_PATH];

        LONG retVal = RegEnumKeyEx(mainHKey, subKeyIndex, subKeyBuffer, &subKeyBufferSize, NULL, NULL, NULL, NULL);

        while(retVal == ERROR_SUCCESS)
        {
            enabledState = 0;

            subKeyName = "\\Software\\Microsoft\\Today\\Items\\";
            subKeyName += subKeyBuffer;

            if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKeyName, 0, 0, &subHKey) == ERROR_SUCCESS)
            {
                valueName = "Enabled";

                RegQueryValueEx(subHKey, valueName, NULL, &varTypeDword, (LPBYTE)&enabledState, &varSizeDword);

                RegCloseKey(subHKey);

                NameAndEnabledStateItem newRegEntry;
                newRegEntry.name = subKeyBuffer;
                newRegEntry.enabled = enabledState;

                m_todayScreenRegBackup.itemVector.push_back(newRegEntry);
            }

            subKeyIndex++;
            subKeyBufferSize = MAX_PATH;
            retVal = RegEnumKeyEx(mainHKey, subKeyIndex, subKeyBuffer, &subKeyBufferSize, NULL, NULL, NULL, NULL);
        }

        RegCloseKey(mainHKey);
    }
}

void CManilla2DConfigTabsDlg::RestoreTodayScreenItemsRegHive()
{
    HKEY hKey;
    DWORD enabledState = 0;
    CString keyName, valueName;

    // restore the date enabled state
    keyName = "\\Software\\Microsoft\\Today";
    
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, 0, &hKey) == ERROR_SUCCESS)
    {
        valueName = "Date";
        enabledState = m_todayScreenRegBackup.dateEnabled;
        RegSetValueEx(hKey, valueName, NULL, REG_DWORD, (CONST BYTE*)&enabledState, sizeof(DWORD));
        RegFlushKey(hKey);
        RegCloseKey(hKey);
    }

    for(size_t i=0; i<m_todayScreenRegBackup.itemVector.size(); i++)
    {
        keyName = "\\Software\\Microsoft\\Today\\Items\\";
        keyName += m_todayScreenRegBackup.itemVector[i].name;

        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, 0, &hKey) == ERROR_SUCCESS)
        {
            valueName = "Enabled";
            enabledState = m_todayScreenRegBackup.itemVector[i].enabled;
            RegSetValueEx(hKey, valueName, NULL, REG_DWORD, (CONST BYTE*)&enabledState, sizeof(DWORD));
            RegFlushKey(hKey);
            RegCloseKey(hKey);
        }
    }
}
