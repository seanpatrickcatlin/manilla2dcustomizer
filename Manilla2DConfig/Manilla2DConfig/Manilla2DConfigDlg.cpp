// Manilla2DConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigDlg.h"

#include "tinyxml.h"
#include "tinystr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CManilla2DConfigDlg dialog

CManilla2DConfigDlg::CManilla2DConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManilla2DConfigDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_bPopulatingListControl = false;
    m_todayScreenRegBackup.dateEnabled = 0;
    m_todayScreenRegBackup.itemVector.clear();
}

void CManilla2DConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_MAIN_LIST_CONTROL, m_mainListControl);
    DDX_Control(pDX, IDC_MOVE_UP_BUTTON, m_moveUpButton);
    DDX_Control(pDX, IDC_MOVE_DOWN_BUTTON, m_moveDownButton);
    DDX_Control(pDX, IDC_CANCEL_BUTTON, m_cancelButton);
    DDX_Control(pDX, IDC_RESTORE_DEFAULTS_BUTTON, m_restoreDefaultsButton);
}

BEGIN_MESSAGE_MAP(CManilla2DConfigDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
    ON_WM_CLOSE()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_MOVE_UP_BUTTON, &CManilla2DConfigDlg::OnBnClickedMoveUpButton)
    ON_BN_CLICKED(IDC_MOVE_DOWN_BUTTON, &CManilla2DConfigDlg::OnBnClickedMoveDownButton)
    ON_BN_CLICKED(IDC_CANCEL_BUTTON, &CManilla2DConfigDlg::OnBnClickedCancelButton)
    ON_BN_CLICKED(IDC_RESTORE_DEFAULTS_BUTTON, &CManilla2DConfigDlg::OnBnClickedRestoreDefaultsButton)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_MAIN_LIST_CONTROL, &CManilla2DConfigDlg::OnLvnItemchangedMainListControl)
    ON_NOTIFY(NM_CLICK, IDC_MAIN_LIST_CONTROL, &CManilla2DConfigDlg::OnNMClickMainListControl)
END_MESSAGE_MAP()


// CManilla2DConfigDlg message handlers

BOOL CManilla2DConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
    m_mainListControl.SetExtendedStyle(m_mainListControl.GetExtendedStyle()|LVS_EX_CHECKBOXES);

    BackupHTCHomeSettingsXmlFileIfNeeded();
    PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile();
    UpdateListControlFromNewWidgetVector();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigDlg::OnOK()
{
    if(HasTheStateChanged())
    {
        CWaitCursor wait;

        BackupTodayScreenItemsRegHive();
        DisableAllTodayScreenItems();
        RefreshTodayScreen();
        PopulateNewHTCHomeSettingsXmlFileFromNewWidgetVector();
        CopyNewHTCHomeSettingsXmlFileToWindowsDir();
        RestoreTodayScreenItemsRegHive();
        RefreshTodayScreen();
    }

    CDialog::OnOK();
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CManilla2DConfigDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_MANILLA2DCONFIG_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_MANILLA2DCONFIG_DIALOG));
	}
}
#endif


void CManilla2DConfigDlg::OnBnClickedMoveUpButton()
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

void CManilla2DConfigDlg::OnBnClickedMoveDownButton()
{
    int selectedItemIndex = GetSelectedItemIndex();

    if((selectedItemIndex >= 0) && (selectedItemIndex < (m_mainListControl.GetItemCount()-1)))
    {
        PrintNewWidgetVectorContents();
        NameAndEnabledStateItem temp = m_newWidgetVector[selectedItemIndex+1];
        m_newWidgetVector[selectedItemIndex+1] = m_newWidgetVector[selectedItemIndex];
        m_newWidgetVector[selectedItemIndex] = temp;
        PrintNewWidgetVectorContents();

        m_bPopulatingListControl = true;
        m_mainListControl.SetItemState(selectedItemIndex+1, LVIS_SELECTED, LVIS_SELECTED);
        m_bPopulatingListControl = false;

        UpdateListControlFromNewWidgetVector();
    }
}

void CManilla2DConfigDlg::OnBnClickedCancelButton()
{
    OnCancel();
}

void CManilla2DConfigDlg::OnBnClickedRestoreDefaultsButton()
{
    CWaitCursor wait;

    BackupTodayScreenItemsRegHive();
    DisableAllTodayScreenItems();
    RefreshTodayScreen();
    RestoreBackupHTCHomeSettingsXmlFileToWindowsDir();
    RestoreTodayScreenItemsRegHive();
    RefreshTodayScreen();

    OnCancel();
}

void CManilla2DConfigDlg::OnLvnItemchangedMainListControl(NMHDR *pNMHDR, LRESULT *pResult)
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


                PrintNewWidgetVectorContents();
            }
        }        
    }
}

bool CManilla2DConfigDlg::HasTheStateChanged()
{
    if(m_currentWidgetVector.size() != m_newWidgetVector.size())
    {
        return true;
    }

    for(size_t i=0; i<m_currentWidgetVector.size(); i++)
    {
        if((m_currentWidgetVector[i].name != m_newWidgetVector[i].name) ||
            (m_currentWidgetVector[i].enabled != m_newWidgetVector[i].enabled))
        {
            return true;
        }
    }

    return false;
}

void CManilla2DConfigDlg::OnNMClickMainListControl(NMHDR *pNMHDR, LRESULT *pResult)
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

int CManilla2DConfigDlg::GetSelectedItemIndex()
{
    int retVal = -1;

    POSITION pos = m_mainListControl.GetFirstSelectedItemPosition();

    if(pos != NULL)
    {
        retVal = m_mainListControl.GetNextSelectedItem(pos);
    }

    return retVal;
}

void CManilla2DConfigDlg::UpdateListControlFromNewWidgetVector()
{
    PrintNewWidgetVectorContents();
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

void CManilla2DConfigDlg::PopulateWidgetVectorsFromCurrentHTCHomeSettingsXmlFile()
{
    CopyFile(GetPathToActualHTCHomeSettingsXmlFile(), GetPathToNewHTCHomeSettingsXmlFile(), FALSE);

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

void CManilla2DConfigDlg::PopulateNewHTCHomeSettingsXmlFileFromNewWidgetVector()
{
    DWORD dwAttributes = GetFileAttributes(GetPathToNewHTCHomeSettingsXmlFile());

    SetFileAttributes(GetPathToNewHTCHomeSettingsXmlFile(), FILE_ATTRIBUTE_NORMAL);

    CT2CA pszConvertedAnsiString(GetPathToNewHTCHomeSettingsXmlFile());

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

    SetFileAttributes(GetPathToNewHTCHomeSettingsXmlFile(), dwAttributes);
}

void CManilla2DConfigDlg::CopyNewHTCHomeSettingsXmlFileToWindowsDir()
{
    DWORD dwAttributes = GetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile());

    SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), FILE_ATTRIBUTE_NORMAL);

    CopyFile(GetPathToNewHTCHomeSettingsXmlFile(), GetPathToActualHTCHomeSettingsXmlFile(), FALSE);

    SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), dwAttributes);
}

void CManilla2DConfigDlg::BackupHTCHomeSettingsXmlFileIfNeeded()
{
    if(!FileExists(GetPathToBackupHTCHomeSettingsXmlFile()))
    {
        CopyFile(GetPathToActualHTCHomeSettingsXmlFile(), GetPathToBackupHTCHomeSettingsXmlFile(), FALSE);
    }
}

void CManilla2DConfigDlg::RestoreBackupHTCHomeSettingsXmlFileToWindowsDir()
{
    DWORD dwAttributes = GetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile());

    SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), FILE_ATTRIBUTE_NORMAL);

    CopyFile(GetPathToBackupHTCHomeSettingsXmlFile(), GetPathToActualHTCHomeSettingsXmlFile(), FALSE);

    SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), dwAttributes);
}

void CManilla2DConfigDlg::RefreshTodayScreen()
{
    // Send message to refresh today screen
    ::SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);
}

void CManilla2DConfigDlg::BackupTodayScreenItemsRegHive()
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

void CManilla2DConfigDlg::RestoreTodayScreenItemsRegHive()
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

void CManilla2DConfigDlg::DisableAllTodayScreenItems()
{
    HKEY mainHKey;
    DWORD enabledState = 0;
    CString mainKeyName, subKeyName, valueName;

    // backup the date enabled state
    mainKeyName = "\\Software\\Microsoft\\Today";
    
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainKeyName, 0, 0, &mainHKey) == ERROR_SUCCESS)
    {
        valueName = "Date";
        enabledState = FALSE;
        RegSetValueEx(mainHKey, valueName, NULL, REG_DWORD, (CONST BYTE*)&enabledState, sizeof(DWORD));
        RegFlushKey(mainHKey);
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
                enabledState = FALSE;
                RegSetValueEx(subHKey, valueName, NULL, REG_DWORD, (CONST BYTE*)&enabledState, sizeof(DWORD));
                RegFlushKey(subHKey);
                RegCloseKey(subHKey);
            }

            subKeyIndex++;
            subKeyBufferSize = MAX_PATH;
            retVal = RegEnumKeyEx(mainHKey, subKeyIndex, subKeyBuffer, &subKeyBufferSize, NULL, NULL, NULL, NULL);
        }

        RegFlushKey(mainHKey);
        RegCloseKey(mainHKey);
    }
}

CString CManilla2DConfigDlg::GetPathToActualHTCHomeSettingsXmlFile()
{
    CString retVal("\\Windows\\HTCHomeSettings.xml");
    return retVal;
}

CString CManilla2DConfigDlg::GetPathToBackupHTCHomeSettingsXmlFile()
{
    CString retVal("\\Program Files\\M2DC\\HTCHomeSettings-BACKUP.xml");
    return retVal;
}

CString CManilla2DConfigDlg::GetPathToNewHTCHomeSettingsXmlFile()
{
    CString retVal("\\Program Files\\M2DC\\HTCHomeSettings-Current.xml");
    return retVal;
}

bool CManilla2DConfigDlg::FileExists(CString pathToFile)
{
    bool retVal = false;

    WIN32_FIND_DATA emptyStruct;

    HANDLE fileHndl = FindFirstFile(GetPathToBackupHTCHomeSettingsXmlFile(), &emptyStruct);
    if(fileHndl != INVALID_HANDLE_VALUE)
    {
        FindClose(fileHndl);
        retVal = true;
    }

    return retVal;
}

void CManilla2DConfigDlg::PrintNewWidgetVectorContents()
{
    char tempStr[1024];
    CString debugStr;

    TRACE(TEXT("---------------"));

    for(size_t i=0; i<m_newWidgetVector.size(); i++)
    {
        sprintf(tempStr, "Item: %d, Name: ", i);
        debugStr = tempStr;
        debugStr += m_newWidgetVector[i].name;
        sprintf(tempStr, ", Enabled: %d\n", m_newWidgetVector[i].enabled);
        debugStr += tempStr;

        TRACE(debugStr);
    }
}
