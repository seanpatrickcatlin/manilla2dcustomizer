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

#include "StdAfx.h"
#include <atlbase.h>
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigProgressDlg.h"

#include "WinCeFileUtils.h"

#include "zip.h"
#include "unzip.h"

#include "tinyxml.h"
#include "tinystr.h"

#include <stdio.h>
#include <time.h>


std::string g_tempStr;
CString g_installDirectory;
bool g_bRestoreTodayScreenNeeded = false;
bool g_bAlreadyBeganMakingChanges = false;
bool g_bAllowPopupDialogs = true;
TodayScreenRegBackup g_todayScreenRegBackup;

void M2DC::SetAllowPopupDialogs(bool allow)
{
    g_bAllowPopupDialogs = allow;
}

bool M2DC::CompareNameAndEnabledStateVectors(NameAndEnabledState_vector_t* vec1, NameAndEnabledState_vector_t* vec2)
{
    if(vec1->size() != vec2->size())
    {
        return true;
    }

    for(size_t i=0; i<vec1->size(); i++)
    {
        if(((*vec1)[i].name != (*vec2)[i].name) ||
            ((*vec1)[i].enabled != (*vec2)[i].enabled))
        {
            return true;
        }
    }

    return false;
}

CString M2DC::GetPathToNullThemePreview()
{
	CString retVal = GetPathToM2DCThemesDirectory();
	retVal += "\\null.jpeg";

    CString debugStr = TEXT("GetPathToNullThemePreview ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString M2DC::GetPathToHTCHomeSettingsXmlFileActiveTheme()
{
	CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\HTCHomeSettings-ActiveTheme.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileActive Theme: ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    if(!WinCeFileUtils::FileExists(retVal))
    {
        retVal = GetPathToHTCHomeSettingsXmlFileBackup();
    }

    return retVal;
}

CString M2DC::GetPathToHTCHomeSettingsXmlFileWorking()
{
	CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\HTCHomeSettings-WorkingCopy.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileWorking: ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    if(!g_bAlreadyBeganMakingChanges)
    {
        BeginMakingChanges();
    }

    return retVal;
}

CString M2DC::GetPathToHTCHomeSettingsXmlFileActual()
{
    CString retVal = WinCeFileUtils::GetPathToWindowsDirectory();
    retVal += "\\HTCHomeSettings.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileActual ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString M2DC::GetPathToHTCHomeSettingsXmlFileBackup()
{
	CString retVal = GetPathToM2DCInstallDirectory();
	retVal += "\\HTCHomeSettings-BACKUP.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileBackup ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString M2DC::GetPathToManila2DRegistryKeyBackupFile()
{
	CString retVal = GetPathToM2DCInstallDirectory();
	retVal += "\\Manila2DRegistry-Backup.reg";

    CString debugStr = TEXT("GetPathToManila2DRegistryKeyBackupFile ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString M2DC::GetPathToThemeBackupFile()
{
	CString retVal = GetPathToM2DCThemesDirectory();
	retVal += "\\DefaultThemeBACKUP.m2dct";

    CString debugStr = TEXT("GetPathToThemeBackupFile ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString M2DC::GetPathToM2DCInstallDirectory()
{
    CString retVal;

    if(g_installDirectory.GetLength() > 0)
    {
        retVal = g_installDirectory;
    }
    else
    {
        retVal = WinCeFileUtils::GetFileDirNoNameNoExt(WinCeFileUtils::GetPathToRunningBinary());
    }

	TRACE(TEXT("GetPathToApplicationDirectory "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

	return retVal;
}

CString M2DC::GetPathToM2DCThemesDirectory()
{
    CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\";
    retVal += "Themes";

    TRACE(TEXT("GetPathToThemesDirectory "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

    if(!WinCeFileUtils::FileExists(retVal))
    {
        CreateDirectory(retVal, NULL);
    }

	return retVal;
}


void M2DC::RefreshTodayScreen()
{
    // Send message to refresh today screen
    ::SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);
}

void M2DC::BackupManila2DRegistryKey(bool overwritePreviousBackup /*= false*/)
{
    bool fileExists = WinCeFileUtils::FileExists(GetPathToManila2DRegistryKeyBackupFile());

    if(overwritePreviousBackup || !fileExists)
    {
        if(fileExists)
        {
            DeleteFile(GetPathToManila2DRegistryKeyBackupFile());
        }

        HKEY mainHKey;
        CString mainKeyName;

        mainKeyName = "\\Software\\HTC\\Manila2D";

        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, mainKeyName, 0, 0, &mainHKey) == ERROR_SUCCESS)
        {
            //RegSaveKey(mainHKey, GetPathToManila2DRegistryKeyBackupFile(), NULL);
            RegCloseKey(mainHKey);
        }
    }
}

void M2DC::RestoreManila2DRegistryKey()
{
    if(WinCeFileUtils::FileExists(M2DC::GetPathToManila2DRegistryKeyBackupFile()))
    {
        //RegReplaceKey(HKEY_LOCAL_MACHINE, NULL, GetPathToManila2DRegistryKeyBackupFile(), NULL);
    }
}

void M2DC::BackupHTCHomeSettingsXml(bool overwritePreviousBackup)
{
    bool fileExists = WinCeFileUtils::FileExists(GetPathToHTCHomeSettingsXmlFileBackup());

    if(overwritePreviousBackup || !fileExists)
    {
        if(fileExists)
        {
            DeleteFile(GetPathToHTCHomeSettingsXmlFileBackup());
        }

        DWORD dwFileAttributes = GetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual());
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual(), FILE_ATTRIBUTE_NORMAL);
        CopyFile(GetPathToHTCHomeSettingsXmlFileActual(), GetPathToHTCHomeSettingsXmlFileBackup(), FALSE);
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileBackup(), FILE_ATTRIBUTE_NORMAL);  
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual(), dwFileAttributes);
    }
}

void M2DC::BackupActiveTheme(bool overwritePreviousBackup)
{
    AfxGetApp()->BeginWaitCursor();

    bool fileExists = WinCeFileUtils::FileExists(GetPathToThemeBackupFile());

    if(overwritePreviousBackup || !fileExists)
    {
        if(fileExists)
        {
            DeleteFile(GetPathToThemeBackupFile());
        }

        std::vector<CString> hh_strVector;
        GetVectorOfThemeFilesCurrentlyInUse(&hh_strVector, true);

        HZIP hz = CreateZip(GetPathToThemeBackupFile(), 0);

        CString progMsg;
        CManilla2DConfigProgressDlg* pProgDlg = NULL;
        if(g_bAllowPopupDialogs)
        {
            pProgDlg = new CManilla2DConfigProgressDlg(AfxGetApp()->GetMainWnd());
        }

        if(pProgDlg)
        {
            progMsg = TEXT("Backing up theme files");
            pProgDlg->BeginTrackingProgress(progMsg, 0, hh_strVector.size());
        }

        for(size_t i=0; i<hh_strVector.size(); i++)
        {
            CString fullFilePath = hh_strVector[i];

            CString archiveFilePath = fullFilePath;
            archiveFilePath.Replace(TEXT("\\"), TEXT("/"));

            if(pProgDlg)
            {
                progMsg.Format(TEXT("Backing up theme files\nFile %d of %d\n"), i, hh_strVector.size());
                progMsg += fullFilePath;
                pProgDlg->UpdateStatus(progMsg, i);
            }

            ZipAdd(hz, archiveFilePath, fullFilePath);

            // set the files attributes ot normal for ease of use later on
            SetFileAttributes(fullFilePath, FILE_ATTRIBUTE_NORMAL);
        }

        CloseZip(hz);
        TRACE(TEXT("End Zip HH_ files\n"));

        if(pProgDlg)
        {
            pProgDlg->EndTrackingProgress();
            delete pProgDlg;
            pProgDlg = NULL;
        }
    }

    AfxGetApp()->EndWaitCursor();
}

const char* M2DC::GetConstCharStarFromCString(CString str)
{
    CT2CA pszConvertedAnsiString(str);
	g_tempStr = pszConvertedAnsiString;

	return g_tempStr.c_str();
}

CString M2DC::GetWin32ErrorString(DWORD err)
{
    CString Error;
    LPTSTR s;
    ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        err,
        0,
        (LPTSTR)&s,
        0,
        NULL);

    LPTSTR p = _tcschr(s, _T('\r'));
    if(p != NULL)
    { /* lose CRLF */
        *p = _T('\0');
    } /* lose CRLF */
    Error = s;
    ::LocalFree(s);

    return Error;
} // ErrorString

void M2DC::RestoreActiveThemeFromBackup()
{
    AfxGetApp()->BeginWaitCursor();
    M2DC::SetActiveThemeFromPath(M2DC::GetPathToThemeBackupFile(), TEXT("BACKUP"));

    AfxGetApp()->EndWaitCursor();
    M2DC::EndMakingChanges();
}

void M2DC::RestoreHTCHomeSettingsXmlFromBackup()
{
    AfxGetApp()->BeginWaitCursor();

    if(WinCeFileUtils::FileExists(M2DC::GetPathToHTCHomeSettingsXmlFileBackup()))
    {
        CopyFile(M2DC::GetPathToHTCHomeSettingsXmlFileBackup(), M2DC::GetPathToHTCHomeSettingsXmlFileWorking(), FALSE);
    }
    
    AfxGetApp()->EndWaitCursor();
    EndMakingChanges();
}

void M2DC::DisableAllTodayScreenItems()
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


void M2DC::BackupTodayScreenItemsRegHive()
{
    g_todayScreenRegBackup.dateEnabled = FALSE;
    g_todayScreenRegBackup.itemVector.clear();

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

    g_todayScreenRegBackup.dateEnabled = enabledState;

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

                g_todayScreenRegBackup.itemVector.push_back(newRegEntry);
            }

            subKeyIndex++;
            subKeyBufferSize = MAX_PATH;
            retVal = RegEnumKeyEx(mainHKey, subKeyIndex, subKeyBuffer, &subKeyBufferSize, NULL, NULL, NULL, NULL);
        }

        RegCloseKey(mainHKey);
    }
}

void M2DC::RestoreTodayScreenItemsRegHive()
{
    HKEY hKey;
    DWORD enabledState = 0;
    CString keyName, valueName;

    // restore the date enabled state
    keyName = "\\Software\\Microsoft\\Today";
    
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, 0, &hKey) == ERROR_SUCCESS)
    {
        valueName = "Date";
        enabledState = g_todayScreenRegBackup.dateEnabled;
        RegSetValueEx(hKey, valueName, NULL, REG_DWORD, (CONST BYTE*)&enabledState, sizeof(DWORD));
        RegFlushKey(hKey);
        RegCloseKey(hKey);
    }

    for(size_t i=0; i<g_todayScreenRegBackup.itemVector.size(); i++)
    {
        keyName = "\\Software\\Microsoft\\Today\\Items\\";
        keyName += g_todayScreenRegBackup.itemVector[i].name;

        if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, keyName, 0, 0, &hKey) == ERROR_SUCCESS)
        {
            valueName = "Enabled";
            enabledState = g_todayScreenRegBackup.itemVector[i].enabled;
            RegSetValueEx(hKey, valueName, NULL, REG_DWORD, (CONST BYTE*)&enabledState, sizeof(DWORD));
            RegFlushKey(hKey);
            RegCloseKey(hKey);
        }
    }
}

void M2DC::BeginMakingChanges()
{
    if(!g_bAlreadyBeganMakingChanges)
    {
        g_bAlreadyBeganMakingChanges = true;

        BackupAndDisableTodayScreen();

        AfxGetApp()->BeginWaitCursor();

        DWORD dwFileAttributes = GetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual());
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual(), FILE_ATTRIBUTE_NORMAL);  
        CopyFile(GetPathToHTCHomeSettingsXmlFileActual(), GetPathToHTCHomeSettingsXmlFileWorking(), FALSE);
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileWorking(), FILE_ATTRIBUTE_NORMAL);
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual(), dwFileAttributes);

        AfxGetApp()->EndWaitCursor();
    }
}

void M2DC::EndMakingChanges()
{
    if(g_bAlreadyBeganMakingChanges)
    {
        DWORD dwFileAttributes = GetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual());
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual(), FILE_ATTRIBUTE_NORMAL);
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileWorking(), FILE_ATTRIBUTE_NORMAL);
        CopyFile(GetPathToHTCHomeSettingsXmlFileWorking(), GetPathToHTCHomeSettingsXmlFileActual(), FALSE);
        SetFileAttributes(GetPathToHTCHomeSettingsXmlFileActual(), dwFileAttributes);
        DeleteFile(GetPathToHTCHomeSettingsXmlFileWorking());

        g_bAlreadyBeganMakingChanges = false;
    }
}

/*
void M2DC::UpdateHTCHomeSettingsXmlWithActualFilePaths(std::vector<CString>* pPathVector, CString xmlFilePath)
{
    if(pPathVector == NULL)
    {
        return;
    }

    TiXmlDocument doc(GetConstCharStarFromCString(xmlFilePath));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome"); 
            htcHomeNode != NULL;
            htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
        {
            for(TiXmlNode* imageListNode = htcHomeNode->FirstChild("ImageList");
                imageListNode != NULL;
                imageListNode = imageListNode->NextSibling("ImageList"))
            {
                TiXmlElement* imageListElement = imageListNode->ToElement();

                if(imageListElement != NULL)
                {
                    CString basePath;
                    basePath = imageListElement->Attribute("path");

                    if(basePath.GetLength() <= 0)
                    {
                        basePath = TEXT("\\Windows");
                    }

                    if(basePath[basePath.GetLength()-1] == '\\')
                    {
                        basePath = basePath.Mid(0, basePath.GetLength()-2);
                    }

                    for(TiXmlElement* imageListItemElement = imageListElement->FirstChildElement("Image");
                        imageListItemElement != NULL;
                        imageListItemElement = imageListItemElement->NextSiblingElement("Image"))
                    {
                        CString currentFilePath(imageListItemElement->Attribute("name"));

                        if((currentFilePath.Find(TEXT("hh_")) != -1) ||
                            (currentFilePath.Find(TEXT("HH_")) != -1))
                        {
                            if(currentFilePath[0] != '\\')
                            {
                                currentFilePath = '\\' + currentFilePath;
                            }

                            currentFilePath = basePath + currentFilePath;

                            CString currentFileName = WinCeFileUtils::GetFileNameNoDirWithExt(currentFilePath);

                            for(size_t i=0; i<pPathVector->size(); i++)
                            {
                                CString oldFilePath = pPathVector->at(i);
                                CString oldFileName = WinCeFileUtils::GetFileNameNoDirWithExt(oldFilePath);

                                if(oldFileName.CompareNoCase(currentFileName) == 0)
                                {
                                    if(oldFilePath.CompareNoCase(currentFilePath) != 0)
                                    {
                                        // the files are the same, but the path in this xml does
                                        // not point to the proper place, FIX IT

                                        CString newFileXmlString = oldFilePath;

                                        if(oldFilePath.Find(basePath) == 0)
                                        {
                                            newFileXmlString = oldFilePath.Mid(basePath.GetLength());
                                        }
                                        else
                                        {
                                            // time to add some leading dots... ".."
                                            CString tempBasePath = basePath;

                                            while(tempBasePath.GetLength() > 0)
                                            {
                                                tempBasePath = tempBasePath.Mid(0, tempBasePath.ReverseFind('\\'));

                                                newFileXmlString = TEXT("\\..") + newFileXmlString;
                                            }
                                        }

                                        imageListItemElement->SetAttribute("name", GetConstCharStarFromCString(newFileXmlString));
                                    }

                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                widgetPropertyNode != NULL;
                widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
            {
                for(TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild();
                    widgetPropertyChildNode != NULL;
                    widgetPropertyChildNode = widgetPropertyChildNode->NextSibling())
                {
                    TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->ToElement();

                    if(widgetPropertyChildElement != NULL)
                    {
                        CString currentwidgetProperty;

                        currentwidgetProperty = widgetPropertyChildElement->Value();

                        CString baseDir;

                        baseDir.Empty();

                        if((currentwidgetProperty.Find(TEXT("PhotoWidget")) != -1) ||
                            (currentwidgetProperty.Find(TEXT("LauncherWidget")) != -1) ||
                            (currentwidgetProperty.Find(TEXT("OperatorWidget")) != -1) ||
                            (currentwidgetProperty.Find(TEXT("LocationWidget")) != -1))
                        {
                            baseDir = TEXT("\\Windows\\");
                        }

                        for(TiXmlElement* widgetPropertyChildPropertyElement = widgetPropertyChildNode->FirstChildElement();
                            widgetPropertyChildPropertyElement != NULL;
                            widgetPropertyChildPropertyElement = widgetPropertyChildPropertyElement->NextSiblingElement())
                        {
                            CString currentFilePath(widgetPropertyChildPropertyElement->Attribute("value"));

                            if((currentFilePath.Find(TEXT("hh_")) != -1) ||
                                (currentFilePath.Find(TEXT("HH_")) != -1))
                            {
                                currentFilePath = baseDir + currentFilePath;

                                CString currentFileName = WinCeFileUtils::GetFileNameNoDirWithExt(currentFilePath);

                                for(size_t i=0; i<pPathVector->size(); i++)
                                {
                                    CString oldFilePath = pPathVector->at(i);
                                    CString oldFileName = WinCeFileUtils::GetFileNameNoDirWithExt(oldFilePath);

                                    if(oldFileName.CompareNoCase(currentFileName) == 0)
                                    {
                                        if(oldFilePath.CompareNoCase(currentFilePath) != 0)
                                        {
                                            // the files are the same, but the path in this xml does
                                            // not point to the proper place, FIX IT

                                            CString newFileXmlString = oldFilePath;

                                            if(oldFilePath.Find(baseDir) == 0)
                                            {
                                                newFileXmlString = oldFilePath.Mid(baseDir.GetLength());
                                            }
                                            else
                                            {
                                                // time to add some leading dots... ".."
                                                CString tempBasePath = baseDir;

                                                while(tempBasePath.GetLength() > 0)
                                                {
                                                    tempBasePath = tempBasePath.Mid(0, tempBasePath.ReverseFind('\\'));

                                                    newFileXmlString = TEXT("\\..") + newFileXmlString;
                                                }
                                            }

                                            widgetPropertyChildPropertyElement->SetAttribute("value", GetConstCharStarFromCString(newFileXmlString));
                                        }

                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        doc.SaveFile();
    }
}
*/

void M2DC::GetVectorOfThemeFilesFromHTCHomeSettingsXml(std::vector<CString>* pPathVector, CString xmlFilePath)
{
    if(pPathVector == NULL)
    {
        return;
    }

    pPathVector->clear();

    TiXmlDocument doc(GetConstCharStarFromCString(xmlFilePath));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome"); 
            htcHomeNode != NULL;
            htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
        {
            for(TiXmlNode* imageListNode = htcHomeNode->FirstChild("ImageList");
                imageListNode != NULL;
                imageListNode = imageListNode->NextSibling("ImageList"))
            {
                TiXmlElement* imageListElement = imageListNode->ToElement();

                if(imageListElement != NULL)
                {
                    CString basePath;
                    basePath = imageListElement->Attribute("path");

                    if(basePath.GetLength() <= 0)
                    {
                        basePath = TEXT("\\Windows");
                    }

                    if(basePath[basePath.GetLength()-1] == '\\')
                    {
                        basePath = basePath.Mid(0, basePath.GetLength()-2);
                    }

                    for(TiXmlElement* imageListItemElement = imageListElement->FirstChildElement("Image");
                        imageListItemElement != NULL;
                        imageListItemElement = imageListItemElement->NextSiblingElement("Image"))
                    {
                        CString currentFilePath(imageListItemElement->Attribute("name"));

                        if((currentFilePath.Find(TEXT("hh_")) != -1) ||
                            (currentFilePath.Find(TEXT("HH_")) != -1))
                        {
                            if(currentFilePath[0] != '\\')
                            {
                                currentFilePath = '\\' + currentFilePath;
                            }

                            currentFilePath = basePath + currentFilePath;

                            TRACE(TEXT("Adding File: "));
                            TRACE(currentFilePath);
                            TRACE(TEXT("\n"));

                            /*
                            if(pProgDlg != NULL)
                            {
                                progMsg = TEXT("Building M2D file list\nStep 1 of 3\n");
                                progMsg += currentFilePath;
                                pProgDlg->UpdateStatus(progMsg, (progMax/4)*1);
                            }
                            */

                            pPathVector->push_back(currentFilePath);
                        }
                    }
                }
            }

            for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                widgetPropertyNode != NULL;
                widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
            {
                for(TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild();
                    widgetPropertyChildNode != NULL;
                    widgetPropertyChildNode = widgetPropertyChildNode->NextSibling())
                {
                    TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->ToElement();

                    if(widgetPropertyChildElement != NULL)
                    {
                        CString currentwidgetProperty;

                        currentwidgetProperty = widgetPropertyChildElement->Value();

                        CString baseDir;

                        baseDir.Empty();

                        if((currentwidgetProperty.Find(TEXT("PhotoWidget")) != -1) ||
                            (currentwidgetProperty.Find(TEXT("LauncherWidget")) != -1) ||
                            (currentwidgetProperty.Find(TEXT("OperatorWidget")) != -1) ||
                            (currentwidgetProperty.Find(TEXT("LocationWidget")) != -1))
                        {
                            baseDir = TEXT("\\Windows\\");
                        }

                        for(TiXmlElement* widgetPropertyChildPropertyElement = widgetPropertyChildNode->FirstChildElement();
                            widgetPropertyChildPropertyElement != NULL;
                            widgetPropertyChildPropertyElement = widgetPropertyChildPropertyElement->NextSiblingElement())
                        {
                            CString currentFilePath(widgetPropertyChildPropertyElement->Attribute("value"));

                            if((currentFilePath.Find(TEXT("hh_")) != -1) ||
                                (currentFilePath.Find(TEXT("HH_")) != -1))
                            {
                                currentFilePath = baseDir + currentFilePath;

                                TRACE(TEXT("Adding File: "));
                                TRACE(currentFilePath);
                                TRACE(TEXT("\n"));

                                /*
                                if(pProgDlg != NULL)
                                {
                                    progMsg = TEXT("Building M2D file list\nStep 2 of 3\n");
                                    progMsg += currentFilePath;
                                    pProgDlg->UpdateStatus(progMsg, (progMax/4)*2);
                                }
                                */

                                pPathVector->push_back(currentFilePath);
                            }
                        }
                    }
                }
            }
        }
    }
}

void M2DC::GetVectorOfThemeFilesCurrentlyInUse(std::vector<CString>* pPathVector, bool includeNonXmlFiles)
{
    int progMax = 12;
    CString progMsg;
    CManilla2DConfigProgressDlg* pProgDlg = NULL;

    CString windowsDirPath = WinCeFileUtils::GetPathToWindowsDirectory();

    if(pPathVector != NULL)
    {
        if(g_bAllowPopupDialogs)
        {
            pProgDlg = new CManilla2DConfigProgressDlg(AfxGetApp()->GetMainWnd());
        }

        if(pProgDlg != NULL)
        {
            progMsg = TEXT("Building M2D file list");
            pProgDlg->BeginTrackingProgress(progMsg, 0, progMax);
        }

        GetVectorOfThemeFilesFromHTCHomeSettingsXml(pPathVector, GetPathToHTCHomeSettingsXmlFileActual());

        if(includeNonXmlFiles)
        {
            pPathVector->push_back(GetPathToHTCHomeSettingsXmlFileActual());

            WIN32_FIND_DATA findData;
            CString findString = windowsDirPath;
            findString += "\\*";
            HANDLE hFindHandle = FindFirstFile(findString, &findData);

            if(hFindHandle != INVALID_HANDLE_VALUE)
            {
                BOOL keepSearching = TRUE;

                while(keepSearching == TRUE)
                {
                    CString currentFile = findData.cFileName;

                    if((currentFile.Find(TEXT("hh_")) != -1) ||
                        (currentFile.Find(TEXT("HH_")) != -1))
                    {
                        CString currentFilePath = windowsDirPath;
                        currentFilePath += "\\";
                        currentFilePath += currentFile;

                        bool addFileToVector = true;

                        for(size_t i=0; i<pPathVector->size(); i++)
                        {
                            if((*pPathVector)[i].Find(currentFile) != -1)
                            {
                                addFileToVector = false;
                                break;
                            }
                        }

                        if(addFileToVector)
                        {
                            TRACE(TEXT("Adding File: "));
                            TRACE(currentFilePath);
                            TRACE(TEXT("\n"));

                            if(pProgDlg != NULL)
                            {
                                progMsg = TEXT("Building M2D file list\nStep 3 of 3\n");
                                progMsg += currentFilePath;
                                pProgDlg->UpdateStatus(progMsg, (progMax/4)*3);
                            }

                            pPathVector->push_back(currentFilePath);
                        }
                    }

                    keepSearching = FindNextFile(hFindHandle, &findData);
                }
            }

            FindClose(hFindHandle);
        }
    }

    if(pProgDlg != NULL)
    {
        pProgDlg->EndTrackingProgress();
        delete pProgDlg;
        pProgDlg = NULL;
    }
}

void M2DC::BackupAndDisableTodayScreen()
{
    if(!g_bRestoreTodayScreenNeeded)
    {
        AfxGetApp()->BeginWaitCursor();
        BackupTodayScreenItemsRegHive();
        DisableAllTodayScreenItems();
        RefreshTodayScreen();
        AfxGetApp()->EndWaitCursor();

        g_bRestoreTodayScreenNeeded = true;
    }
}

void M2DC::RestoreAndReEnableTodayScreen()
{
    if(g_bRestoreTodayScreenNeeded)
    {
        AfxGetApp()->BeginWaitCursor();

        RestoreTodayScreenItemsRegHive();
        RefreshTodayScreen();

        AfxGetApp()->EndWaitCursor();
    }
}

void M2DC::SetInstallDirectory(CString installDirectory)
{
    g_installDirectory = installDirectory;
}

int M2DC::SetActiveTheme(CString themeName)
{
    CString pathToTheme = M2DC::GetPathOfM2DCThemeFromName(themeName);
    return SetActiveThemeFromPath(pathToTheme, themeName);
}

/*
int M2DC::SetActiveThemeFromPath(CString themePath, CString themeName)
{
    int retVal = 0;

    // unzip the files to their appropriate destinations according to the
    // paths from the current XML file
    if(WinCeFileUtils::FileExists(themePath))
    {
        if(!FileIsValidM2DCTheme(themePath))
        {
            CString msg;
            msg = TEXT("Unable to find HTCHomeSettings.xml or a hh_* file in the selected theme.\n");
            msg += TEXT("Unable to apply");

            AfxMessageBox(msg, MB_OK);

            return -1;
        }

        // delete the last active theme file
        DeleteFile(GetPathToHTCHomeSettingsXmlFileActiveTheme());

        BeginMakingChanges();
        AfxGetApp()->BeginWaitCursor();

        // backup launcher settings
        int launcherColumns = 0;
        int launcherRows = 0;

        M2DC::ReadLauncherValuesFromXml(launcherColumns, launcherRows);

        // backup tab settings
        NameAndEnabledState_vector_t tabWidgetVector;
        tabWidgetVector.clear();
        M2DC::ReadTabValuesFromXml(&tabWidgetVector);

        // 
        std::vector<CString> originalThemeFilePathsList;
        GetVectorOfThemeFilesCurrentlyInUse(&originalThemeFilePathsList, true);

        HZIP hz = OpenZip(themePath, 0);
        ZIPENTRY ze;
        // -1 gives overall information about the zipfile
        GetZipItem(hz,-1,&ze);

        int numitems = ze.index;

        // rip through the theme file and extract the HtcHomeSettings.xml file is possible
        for(int zi=0; zi<numitems; zi++)
        {
            ZIPENTRY ze;

            GetZipItem(hz, zi, &ze);            // fetch individual details

            CString destString;
            CString filePathFromZip = ze.name;
            CString fileNameNoPath = filePathFromZip.Mid(filePathFromZip.ReverseFind('/')+1);

            if(fileNameNoPath.CompareNoCase(TEXT("HTCHomeSettings.xml")) == 0)
            {
                destString = M2DC::GetPathToHTCHomeSettingsXmlFileWorking();
                DWORD dwAttributes =  GetFileAttributes(destString);
                SetFileAttributes(destString, FILE_ATTRIBUTE_NORMAL);
                UnzipItem(hz, zi, destString);
                SetFileAttributes(destString, dwAttributes);
            }
        }

        std::vector<CString> newThemeFilePathsList;
        GetVectorOfThemeFilesFromHTCHomeSettingsXml(&newThemeFilePathsList, GetPathToHTCHomeSettingsXmlFileWorking());

        CString progMsg;
        CManilla2DConfigProgressDlg* pProgDlg = NULL;
        
        if(g_bAllowPopupDialogs)
        {
            pProgDlg = new CManilla2DConfigProgressDlg(AfxGetApp()->GetMainWnd());
        }

        if(pProgDlg != NULL)
        {
            progMsg = TEXT("Applying theme: ");
            progMsg += themeName;
            pProgDlg->BeginTrackingProgress(progMsg, 0, numitems);
        }

        for(int zi=0; zi<numitems; zi++)
        {
            ZIPENTRY ze;

            GetZipItem(hz, zi, &ze);            // fetch individual details

            CString destString;
            CString filePathFromZip = ze.name;
            CString fileNameNoPath = filePathFromZip.Mid(filePathFromZip.ReverseFind('/')+1);
            CString fileExt = fileNameNoPath.Mid(fileNameNoPath.ReverseFind('.')+1);

            if(fileNameNoPath.CompareNoCase(TEXT("HTCHomeSettings.xml")) == 0)
            {
                destString = GetPathToHTCHomeSettingsXmlFileActiveTheme();
            }

            if(fileExt.CompareNoCase(TEXT("tsk")) == 0)
            {
                destString = WinCeFileUtils::GetPathToWindowsDirectory();
                destString += "\\";
                destString += fileNameNoPath;
            }

            for(size_t i=0; ((i<originalThemeFilePathsList.size()) && (destString.GetLength() <= 0)); i++)
            {
                CString currentThemeFile = originalThemeFilePathsList[i];
                CString currentThemeFileNoPath = WinCeFileUtils::GetFileNameNoDirWithExt(currentThemeFile);
                if(currentThemeFileNoPath.CompareNoCase(fileNameNoPath) == 0)
                {
                    destString = currentThemeFile;
                }
            }

            for(size_t i=0; ((i<newThemeFilePathsList.size()) && (destString.GetLength() <= 0)); i++)
            {
                CString currentThemeFile = newThemeFilePathsList[i];
                CString currentThemeFileNoPath = WinCeFileUtils::GetFileNameNoDirWithExt(currentThemeFile);
                if(currentThemeFileNoPath.CompareNoCase(fileNameNoPath) == 0)
                {
                    destString = currentThemeFile;
                }
            }

            if(destString.GetLength() <= 0)
            {
                if(((fileNameNoPath[0] == 'H') || (fileNameNoPath[0] == 'h')) &&
                    ((fileNameNoPath[1] == 'H') || (fileNameNoPath[1] == 'h')) &&
                    (fileNameNoPath[2] == '_'))
                {
                    destString = WinCeFileUtils::GetPathToWindowsDirectory() + '\\';
                    destString += fileNameNoPath;
                }
            }

            if(destString.GetLength() > 0)
            {
                if(pProgDlg != NULL)
                {
                    progMsg.Format(TEXT("Applying theme: %s\nFile %d of %d\n"), themeName.GetBuffer(), zi, numitems);
                    progMsg += fileNameNoPath;
                    pProgDlg->UpdateStatus(progMsg, zi);
                }

                DWORD dwAttributes = GetFileAttributes(destString);
                SetFileAttributes(destString, FILE_ATTRIBUTE_NORMAL);
                UnzipItem(hz, zi, destString);
                SetFileAttributes(destString, dwAttributes);

                if(fileExt.CompareNoCase(TEXT("tsk")) == 0)
                {
                    SetNewTskTheme(destString);
                }
            }
            else
            {
                TRACE(TEXT("Skipped "));
                TRACE(fileNameNoPath);
                TRACE(TEXT("\n"));
            }
        }

        if(pProgDlg != NULL)
        {
            pProgDlg->EndTrackingProgress();
            delete pProgDlg;
            pProgDlg = NULL;
        }

        CloseZip(hz);

        UpdateHTCHomeSettingsXmlWithActualFilePaths(&originalThemeFilePathsList, GetPathToHTCHomeSettingsXmlFileWorking());
        // Read specific values from the HTCHomeSettings.xml file in the ActiveThemeDirectory
        // write the values to the working xml file
        //HTCHomeSettingsStruct xmlSettings;

        //ReadValuesFromXml(GetPathToHTCHomeSettingsXmlFileActiveTheme(), &xmlSettings);
        //WriteValuesToXml(GetPathToHTCHomeSettingsXmlFileWorking(), &xmlSettings);

        // restore launcher settings
        M2DC::WriteLauncherValuesToXml(launcherColumns, launcherRows);

        // restore tab settings
        M2DC::WriteTabValuesToXml(&tabWidgetVector);

        AfxGetApp()->EndWaitCursor();
        EndMakingChanges();
    }

    return retVal;
}
*/

int M2DC::SetActiveThemeFromPath(CString themePath, CString themeName)
{
    int retVal = 0;

    // unzip the files to their appropriate destinations according to the
    // paths from the current XML file
    if(WinCeFileUtils::FileExists(themePath))
    {
        // add a 5 second wait ot make sure that the files can be overwritten
        Sleep(5000);

        if(!FileIsValidM2DCTheme(themePath))
        {
            CString msg;
            msg = TEXT("Unable to find HTCHomeSettings.xml or a hh_* file in the selected theme.\n");
            msg += TEXT("Unable to apply");

            AfxMessageBox(msg, MB_OK);

            return -1;
        }

        // delete the last active theme file
        DeleteFile(GetPathToHTCHomeSettingsXmlFileActiveTheme());

        BeginMakingChanges();
        AfxGetApp()->BeginWaitCursor();

        std::vector<CString> pathsToThemeFiles;

        GetVectorOfThemeFilesCurrentlyInUse(&pathsToThemeFiles, true);

        HZIP hz = OpenZip(themePath, 0);
        ZIPENTRY ze;

        // -1 gives overall information about the zipfile
        GetZipItem(hz,-1,&ze);

        CString progMsg;
        CManilla2DConfigProgressDlg* pProgDlg = NULL;
        
        if(g_bAllowPopupDialogs)
        {
            pProgDlg = new CManilla2DConfigProgressDlg(AfxGetApp()->GetMainWnd());
        }

        int numitems = ze.index;

        if(pProgDlg != NULL)
        {
            progMsg = TEXT("Applying theme: ");
            progMsg += themeName;
            pProgDlg->BeginTrackingProgress(progMsg, 0, numitems);
        }

        for(int zi=0; zi<numitems; zi++)
        {
            ZIPENTRY ze;

            GetZipItem(hz, zi, &ze);            // fetch individual details

            CString destString;
            CString filePathFromZip = ze.name;
            CString fileNameNoPath = filePathFromZip.Mid(filePathFromZip.ReverseFind('/')+1);
            CString fileExt = fileNameNoPath.Mid(fileNameNoPath.ReverseFind('.')+1);

            if(fileNameNoPath.Compare(TEXT("HTCHomeSettings.xml")) == 0)
            {
                destString = GetPathToHTCHomeSettingsXmlFileActiveTheme();
            }

            if(fileExt.CompareNoCase(TEXT("tsk")) == 0)
            {
                destString = WinCeFileUtils::GetPathToWindowsDirectory();
                destString += "\\";
                destString += fileNameNoPath;
            }

            for(size_t i=0; ((i<pathsToThemeFiles.size()) && (destString.GetLength() <= 0)); i++)
            {
                CString currentThemeFile = pathsToThemeFiles[i];
                if(currentThemeFile.Find(fileNameNoPath) != -1)
                {
                    destString = currentThemeFile;
                }
            }

            if(destString.GetLength() > 0)
            {
                if(pProgDlg != NULL)
                {
                    progMsg.Format(TEXT("Applying theme: %s\nFile %d of %d\n"), themeName.GetBuffer(), zi, numitems);
                    progMsg += fileNameNoPath;
                    pProgDlg->UpdateStatus(progMsg, zi);
                }

                DWORD dwAttributes =  GetFileAttributes(destString);
                SetFileAttributes(destString, FILE_ATTRIBUTE_NORMAL);
                UnzipItem(hz, zi, destString);
                SetFileAttributes(destString, dwAttributes);

                if(fileExt.CompareNoCase(TEXT("tsk")) == 0)
                {
                    SetNewTskTheme(destString);
                }
            }
            else
            {
                TRACE(TEXT("Skipped "));
                TRACE(fileNameNoPath);
                TRACE(TEXT("\n"));
            }
        }

        if(pProgDlg != NULL)
        {
            pProgDlg->EndTrackingProgress();
            delete pProgDlg;
            pProgDlg = NULL;
        }

        CloseZip(hz);

        // Read specific values from the HTCHomeSettings.xml file in the ActiveThemeDirectory
        // write the values to the working xml file
        HTCHomeSettingsStruct xmlSettings;

        ReadValuesFromXml(GetPathToHTCHomeSettingsXmlFileActiveTheme(), &xmlSettings);
        WriteValuesToXml(GetPathToHTCHomeSettingsXmlFileWorking(), &xmlSettings);

        AfxGetApp()->EndWaitCursor();
        EndMakingChanges();
    }

    return retVal;
}

void M2DC::GetPathsOfM2DCThemeFolders(std::vector<CString>* pFolderNameVector)
{
    if(pFolderNameVector != NULL)
    {
        CString currentThemeFolderPath;

        TiXmlDocument doc(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            for(TiXmlNode* m2dcNode = doc.FirstChild("M2DC");
                m2dcNode != NULL;
                m2dcNode = m2dcNode->NextSibling("M2DC"))
            {
                for(TiXmlElement* themeFolderElement = m2dcNode->FirstChildElement("ThemeFolder");
                    themeFolderElement != NULL;
                    themeFolderElement = themeFolderElement->NextSiblingElement("ThemeFolder"))
                {
                    currentThemeFolderPath = themeFolderElement->Attribute("path");

                    if(WinCeFileUtils::IsDir(currentThemeFolderPath))
                    {
                        pFolderNameVector->push_back(currentThemeFolderPath);
                    }
                    else
                    {
                        m2dcNode->RemoveChild(themeFolderElement);
                    }
                }
            }
        }
    }
}

void M2DC::GetNamesOfAvailableM2DCThemes(std::vector<CString>* pThemeNameVector)
{
    if(pThemeNameVector != NULL)
    {
        // call a function that reads the theme folders from the xml file
        std::vector<CString> themeFoldersVector;
        GetPathsOfM2DCThemeFolders(&themeFoldersVector);

        AfxGetApp()->BeginWaitCursor();

        for(size_t i=0; i<themeFoldersVector.size(); i++)
        {
            //CString searchString = GetPathToM2DCThemesDirectory();
            CString searchString = themeFoldersVector[i];
            searchString += "\\*";

            WIN32_FIND_DATA findData;
            HANDLE hFindHandle = FindFirstFile(searchString, &findData);

            CString currentFileName;
            CString currentFileExtension;
            CString currentFilePath;

            if(hFindHandle != INVALID_HANDLE_VALUE)
            {
                BOOL keepSearching = TRUE;

                while(keepSearching == TRUE)
                {
                    currentFileName = findData.cFileName;

                    if((currentFileName.CompareNoCase(TEXT(".")) != 0) && (currentFileName.CompareNoCase(TEXT("..")) != 0))
                    {
                        currentFileExtension = currentFileName.Mid(currentFileName.ReverseFind('.')+1);

                        if((currentFileExtension.CompareNoCase(TEXT("zip")) == 0) ||
                            (currentFileExtension.CompareNoCase(TEXT("m2dct")) == 0))
                        {
                            currentFilePath = themeFoldersVector[i];
                            currentFilePath += "\\";
                            currentFilePath += currentFileName;

                            M2DC::AddToM2DCThemeList(currentFilePath);
                        }
                    }

                    keepSearching = FindNextFile(hFindHandle, &findData);
                }
            }

            FindClose(hFindHandle);
        }
        AfxGetApp()->EndWaitCursor();

        CString currentThemeName;
        CString currentThemePath;

        TiXmlDocument doc(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            for(TiXmlNode* m2dcNode = doc.FirstChild("M2DC");
                m2dcNode != NULL;
                m2dcNode = m2dcNode->NextSibling("M2DC"))
            {
                for(TiXmlElement* themeElement = m2dcNode->FirstChildElement("Theme");
                    themeElement != NULL;
                    themeElement = themeElement->NextSiblingElement("Theme"))
                {
                    currentThemeName = themeElement->Attribute("name");
                    currentThemePath = themeElement->Attribute("path");

                    if(WinCeFileUtils::FileExists(currentThemePath))
                    {
                        pThemeNameVector->push_back(currentThemeName);
                    }
                    else
                    {
                        m2dcNode->RemoveChild(themeElement);
                    }
                }
            }
        }
    }
}

void M2DC::ReadValuesFromXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings)
{
    if(WinCeFileUtils::FileExists(xmlFilePath) && (xmlSettings != NULL))
    {
        TiXmlDocument doc(GetConstCharStarFromCString(xmlFilePath));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
                htcHomeNode != NULL;
                htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
            {
                for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                    widgetPropertyNode != NULL;
                    widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
                {
                    for(TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild();
                        widgetPropertyChildNode != NULL;
                        widgetPropertyChildNode = widgetPropertyChildNode->NextSibling())
                    {
                        CString currentNodeName;
                        currentNodeName = widgetPropertyChildNode->Value();

                        std::vector<TiXmlElement>* pRelevantVector = NULL;

                        if(currentNodeName.Find(TEXT("HomeWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->homeWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("TabWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->tabWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("PeopleWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->peopleWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("MyFavesWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->myfavesWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("WeatherWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->weatherWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("LauncherWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->launcherWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("OperatorWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->operatorWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("MusicWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->musicWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("PhotoWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->photoWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("MessagingWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->messagingWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("EmailWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->emailWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("SettingsWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->settingsWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("InternetWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->internetWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("LocationWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->locationWidgetPropertyElements);
                        }

                        if(pRelevantVector != NULL)
                        {
                            for(TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->FirstChildElement();
                                widgetPropertyChildElement != NULL;
                                widgetPropertyChildElement = widgetPropertyChildElement->NextSiblingElement())
                            {
                                CString currentNameAttribute;

                                currentNameAttribute = widgetPropertyChildElement->Attribute("name");

                                if((currentNameAttribute.Find(TEXT("POS")) != -1) ||
                                    (currentNameAttribute.Find(TEXT("RECT")) != -1))
                                {
                                    pRelevantVector->push_back(*widgetPropertyChildElement);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void M2DC::WriteValuesToXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings)
{
    if(WinCeFileUtils::FileExists(xmlFilePath) && (xmlSettings != NULL))
    {
        CString vectorNameAttribute;
        CString vectorValueAttribute;
        CString currentNameAttribute;

        TiXmlDocument doc(GetConstCharStarFromCString(xmlFilePath));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
                htcHomeNode != NULL;
                htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
            {
                for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                    widgetPropertyNode != NULL;
                    widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
                {
                    for(TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild();
                        widgetPropertyChildNode != NULL;
                        widgetPropertyChildNode = widgetPropertyChildNode->NextSibling())
                    {
                        CString currentNodeName;
                        currentNodeName = widgetPropertyChildNode->Value();

                        std::vector<TiXmlElement>* pRelevantVector = NULL;

                        if(currentNodeName.Find(TEXT("HomeWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->homeWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("TabWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->tabWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("PeopleWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->peopleWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("MyFavesWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->myfavesWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("WeatherWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->weatherWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("LauncherWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->launcherWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("OperatorWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->operatorWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("MusicWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->musicWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("PhotoWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->photoWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("MessagingWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->messagingWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("EmailWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->emailWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("SettingsWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->settingsWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("InternetWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->internetWidgetPropertyElements);
                        }
                        else if(currentNodeName.Find(TEXT("LocationWidget")) != -1)
                        {
                            pRelevantVector = &(xmlSettings->locationWidgetPropertyElements);
                        }

                        if(pRelevantVector != NULL)
                        {
                            for(TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->FirstChildElement();
                                widgetPropertyChildElement != NULL;
                                widgetPropertyChildElement = widgetPropertyChildElement->NextSiblingElement())
                            {
                                currentNameAttribute = widgetPropertyChildElement->Attribute("name");

                                for(size_t i=0; i<pRelevantVector->size(); i++)
                                {
                                    vectorNameAttribute = (*pRelevantVector)[i].Attribute("name");
                                    vectorValueAttribute = (*pRelevantVector)[i].Attribute("value");

                                    if(vectorNameAttribute.Compare(currentNameAttribute) == 0)
                                    {
                                        widgetPropertyChildElement->SetAttribute("value",
                                            GetConstCharStarFromCString(vectorValueAttribute));
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            doc.SaveFile();
        }
    }
}

bool M2DC::FileIsValidM2DCTheme(CString filePath)
{
    bool retVal = false;

    AfxGetApp()->BeginWaitCursor();
    HZIP hz = OpenZip(filePath, 0);
    ZIPENTRY ze;

    // -1 gives overall information about the zipfile
    GetZipItem(hz,-1,&ze);

    int numitems = ze.index;

    for(int zi=0; ((zi < numitems) && (retVal == false)); zi++)
    {
        ZIPENTRY ze;
        GetZipItem(hz, zi, &ze);            // fetch individual details

        CString filePathFromZip(ze.name);

        CString fileExtension = WinCeFileUtils::GetFileExtNoDirNoName(filePathFromZip);

        if(fileExtension.CompareNoCase(_T("m2dct")) == 0)
        {
            CString warningString = _T("M2DC found a m2dct file in this theme file.\n");
            warningString += _T("You may need to extract the m2dct file from this archive");
            warningString += _T("if you have problems applying this theme.");

            MessageBox(NULL, warningString, WinCeFileUtils::GetFileNameNoDirNoExt(filePathFromZip), MB_OK);
        }

        retVal = ((filePathFromZip.Find(TEXT("HTCHomeSettings.xml")) != -1) ||
                    (filePathFromZip.Find(TEXT("HH_")) != -1) ||
                    (filePathFromZip.Find(TEXT("hh_")) != -1));
    }

    CloseZip(hz);
    AfxGetApp()->EndWaitCursor();

    return retVal;
}

void M2DC::GetHomeWidgetSettings(CString pathToXmlFile, HomeWidgetSettings* pHomeWidgetSettings)
{
    if(WinCeFileUtils::FileExists(pathToXmlFile) && (pHomeWidgetSettings != NULL))
    {
        pHomeWidgetSettings->bAnalogClockEnabled = true;
        pHomeWidgetSettings->bDigitalClockEnabled = true;
        pHomeWidgetSettings->bMissedCallsEnabled = true;
        pHomeWidgetSettings->bCalendarEnabled = true;
        pHomeWidgetSettings->bAlarmStatusEnabled = true;
        pHomeWidgetSettings->bLine1Enabled = true;
        pHomeWidgetSettings->bLine2Enabled = true;
        pHomeWidgetSettings->bLine3Enabled = true;

        // prepare a vector of elements from the current theme file home elements
        std::vector<TiXmlElement> homeWidgetPropertyElements;
        GetVectorOfWidgetPropertyRectPosElements(
            pathToXmlFile,
            TEXT("HomeWidget"),
            &homeWidgetPropertyElements);

        CString currentNameAttribute;
        CString currentValueAttribute;

        for(size_t i=0; i<homeWidgetPropertyElements.size(); i++)
        {
            currentNameAttribute = homeWidgetPropertyElements[i].Attribute("name");
            currentValueAttribute = homeWidgetPropertyElements[i].Attribute("value");

            if((currentNameAttribute.Find(TEXT("POS")) != -1) ||
                (currentNameAttribute.Find(TEXT("RECT")) != -1))
            {
                if(currentValueAttribute.Find(TEXT("-1")) != -1)
                {
                    if(currentNameAttribute.Find(TEXT("CLOCKBTN")) != -1)
                    {
                        pHomeWidgetSettings->bDigitalClockEnabled = false;
                    }
                    else if(currentNameAttribute.Find(TEXT("ANALOGCLOCK")) != -1)
                    {
                        pHomeWidgetSettings->bAnalogClockEnabled = false;
                    }
                    else if(currentNameAttribute.Find(TEXT("CALENDARBTN")) != -1)
                    {
                        pHomeWidgetSettings->bCalendarEnabled = false;
                    }
                    else if(currentNameAttribute.Find(TEXT("MISSEDCALLBTN")) != -1)
                    {
                        pHomeWidgetSettings->bMissedCallsEnabled = false;
                    }
                    else if(currentNameAttribute.Find(TEXT("ALARMSTATUS")) != -1)
                    {
                        pHomeWidgetSettings->bAlarmStatusEnabled = false;
                    }
                    else if(currentNameAttribute.Find(TEXT("LINE1")) != -1)
                    {
                        pHomeWidgetSettings->bLine1Enabled = false;
                    }
                    else if(currentNameAttribute.Find(TEXT("LINE2")) != -1)
                    {
                        pHomeWidgetSettings->bLine2Enabled = false;
                    }
                    else if(currentNameAttribute.Find(TEXT("LINE3")) != -1)
                    {
                        pHomeWidgetSettings->bLine3Enabled = false;
                    }
                }
            }
        }
    }
}



void M2DC::SetHomeWidgetSettings(CString pathToXmlFile, HomeWidgetSettings* pHomeWidgetSettings)
{
    if(WinCeFileUtils::FileExists(pathToXmlFile) && (pHomeWidgetSettings != NULL))
    {
        // prepare a vector of elements from the current theme file home elements
        std::vector<TiXmlElement> themeHomeWidgetPropertyElements;
        GetVectorOfWidgetPropertyRectPosElements(
            M2DC::GetPathToHTCHomeSettingsXmlFileActiveTheme(),
            TEXT("HomeWidget"),
            &themeHomeWidgetPropertyElements);

        TiXmlDocument doc(GetConstCharStarFromCString(pathToXmlFile));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
                htcHomeNode != NULL;
                htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
            {
                for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                    widgetPropertyNode != NULL;
                    widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
                {
                    for(TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild("HomeWidget");
                        widgetPropertyChildNode != NULL;
                        widgetPropertyChildNode = widgetPropertyChildNode->NextSibling("HomeWidget"))
                    {
                        for(TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->FirstChildElement();
                            widgetPropertyChildElement != NULL;
                            widgetPropertyChildElement = widgetPropertyChildElement->NextSiblingElement())
                        {
                            CString currentNameAttribute;
                            CString themeNameAttribute;
                            CString themeValueAttribute;

                            currentNameAttribute = widgetPropertyChildElement->Attribute("name");

                            CString posValue = TEXT("-1,-1");
                            CString rectValue = TEXT("-1,-1,-1,-1");

                            if((currentNameAttribute.Find(TEXT("POS")) != -1) ||
                                (currentNameAttribute.Find(TEXT("RECT")) != -1))
                            {
                                CString enabledValue;
                                CString disabledValue = posValue;

                                if(currentNameAttribute.Find(TEXT("RECT")) != -1)
                                {
                                    disabledValue = rectValue;
                                }

                                for(size_t i=0; i<themeHomeWidgetPropertyElements.size(); i++)
                                {
                                    themeNameAttribute = themeHomeWidgetPropertyElements[i].Attribute("name");

                                    if(themeNameAttribute.Compare(currentNameAttribute) == 0)
                                    {
                                        enabledValue = themeHomeWidgetPropertyElements[i].Attribute("value");
                                        break;
                                    }
                                }

                                CString valueToWrite;

                                if(currentNameAttribute.Find(TEXT("CLOCKBTN")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bDigitalClockEnabled ? enabledValue : disabledValue;
                                }
                                else if(currentNameAttribute.Find(TEXT("ANALOGCLOCK")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bAnalogClockEnabled ? enabledValue : disabledValue;
                                }
                                else if(currentNameAttribute.Find(TEXT("CALENDARBTN")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bCalendarEnabled ? enabledValue : disabledValue;
                                }
                                else if(currentNameAttribute.Find(TEXT("MISSEDCALLBTN")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bMissedCallsEnabled ? enabledValue : disabledValue;
                                }
                                else if(currentNameAttribute.Find(TEXT("ALARMSTATUS")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bAlarmStatusEnabled ? enabledValue : disabledValue;
                                }
                                else if(currentNameAttribute.Find(TEXT("LINE1")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bLine1Enabled ? enabledValue : disabledValue;
                                }
                                else if(currentNameAttribute.Find(TEXT("LINE2")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bLine2Enabled ? enabledValue : disabledValue;
                                }
                                else if(currentNameAttribute.Find(TEXT("LINE3")) != -1)
                                {
                                    valueToWrite = pHomeWidgetSettings->bLine3Enabled ? enabledValue : disabledValue;
                                }

                                if(valueToWrite.GetLength() > 0)
                                {
                                    widgetPropertyChildElement->SetAttribute("value", GetConstCharStarFromCString(valueToWrite));
                                }
                            }
                        }
                    }
                }
            }
        }

        doc.SaveFile();
    }
}

void M2DC::GetVectorOfWidgetPropertyRectPosElements(CString xmlFilePath, CString nodeName, std::vector<TiXmlElement>* pElementVector)
{
    if(WinCeFileUtils::FileExists(xmlFilePath) && (pElementVector != NULL))
    {
        TiXmlDocument doc(GetConstCharStarFromCString(xmlFilePath));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
                htcHomeNode != NULL;
                htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
            {
                for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                    widgetPropertyNode != NULL;
                    widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
                {
                    for(TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild(GetConstCharStarFromCString(nodeName));
                        widgetPropertyChildNode != NULL;
                        widgetPropertyChildNode = widgetPropertyChildNode->NextSibling(GetConstCharStarFromCString(nodeName)))
                    {
                        for(TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->FirstChildElement();
                            widgetPropertyChildElement != NULL;
                            widgetPropertyChildElement = widgetPropertyChildElement->NextSiblingElement())
                        {
                            CString currentNameAttribute;
                            currentNameAttribute = widgetPropertyChildElement->Attribute("name");

                            if((currentNameAttribute.Find(TEXT("POS")) != -1) ||
                                (currentNameAttribute.Find(TEXT("RECT")) != -1))
                            {
                                pElementVector->push_back(*widgetPropertyChildElement);
                            }
                        }
                    }
                }
            }
        }
    }
}


CString M2DC::GetPathToM2DCThemeListXml()
{
	CString retVal = GetPathToM2DCThemesDirectory();
	retVal += "\\AvailableThemes.xml";

    CString debugStr = TEXT("GetPathToM2DCThemeListXml ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;

}

void M2DC::AddFolderToM2DCThemeList(CString pathToFolder)
{
    CString currentFolderPath;

    if(!WinCeFileUtils::FileExists(M2DC::GetPathToM2DCThemeListXml()))
    {
        TiXmlDocument newdoc;
        TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
        TiXmlElement* element = new TiXmlElement("M2DC");
        newdoc.LinkEndChild(decl);
        newdoc.LinkEndChild(element);

        newdoc.SaveFile(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
    }

    TiXmlDocument doc(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {        
        for(TiXmlNode* m2dcNode = doc.FirstChild("M2DC");
            m2dcNode != NULL;
            m2dcNode = m2dcNode->NextSibling("M2DC"))
        {
            for(TiXmlElement* themeElement = m2dcNode->FirstChildElement("ThemeFolder");
                themeElement != NULL;
                themeElement = themeElement->NextSiblingElement("ThemeFolder"))
            {
                currentFolderPath = themeElement->Attribute("path");

                if(currentFolderPath.CompareNoCase(pathToFolder) == 0)
                {
                    // don't bother to add the theme, its path already exists
                    return;
                }
            }
        }

        TiXmlElement* m2dcElement = doc.FirstChildElement("M2DC");

        if(m2dcElement == NULL)
        {
            m2dcElement = new TiXmlElement("M2DC");
            doc.InsertEndChild(*m2dcElement);
        }
        
        // - getting default null preview path
        TiXmlElement* newThemeFolderElement = new TiXmlElement("ThemeFolder");
        newThemeFolderElement->SetAttribute("path", GetConstCharStarFromCString(pathToFolder));

        m2dcElement->LinkEndChild(newThemeFolderElement);

        doc.SaveFile();
    }
}

void M2DC::AddToM2DCThemeList(CString pathToTheme)
{
    int duplicateCount = 0;
    CString currentThemeName;
    CString currentThemePath;

    CString themeBaseName = WinCeFileUtils::GetFileNameNoDirNoExt(pathToTheme);

    if(!WinCeFileUtils::FileExists(M2DC::GetPathToM2DCThemeListXml()))
    {
        TiXmlDocument newdoc;
        TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
        TiXmlElement* element = new TiXmlElement("M2DC");
        newdoc.LinkEndChild(decl);
        newdoc.LinkEndChild(element);

        newdoc.SaveFile(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
    }

    TiXmlDocument doc(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* m2dcNode = doc.FirstChild("M2DC");
            m2dcNode != NULL;
            m2dcNode = m2dcNode->NextSibling("M2DC"))
        {
            for(TiXmlElement* themeElement = m2dcNode->FirstChildElement("Theme");
                themeElement != NULL;
                themeElement = themeElement->NextSiblingElement("Theme"))
            {
                currentThemeName = themeElement->Attribute("name");
                currentThemePath = themeElement->Attribute("path");

                if(currentThemePath.CompareNoCase(pathToTheme) == 0)
                {
                    // don't bother to add the theme, its path already exists
                    return;
                }

                if(currentThemeName.Find(themeBaseName) != -1)
                {
                    duplicateCount++;
                }
            }
        }

        TiXmlElement* m2dcElement = doc.FirstChildElement("M2DC");

        if(m2dcElement == NULL)
        {
            m2dcElement = new TiXmlElement("M2DC");
            doc.InsertEndChild(*m2dcElement);
        }

        if(duplicateCount > 0)
        {
            CString tmpStr;
            tmpStr.Format(TEXT("-%d"), duplicateCount);

            themeBaseName += tmpStr;
        }

        CString previewPath = GetPathToNullThemePreview();;

        // check to see if a file named "preview.jpeg" exists
        // if it does exist extract it and sav the path
        // get information about the zip file
        HZIP hz = OpenZip(pathToTheme, 0);
        ZIPENTRY ze;

        // -1 gives overall information about the zipfile
        GetZipItem(hz, -1, &ze);

        int numitems = ze.index;

        for(int zi=0; zi < numitems; zi++)
        {
            ZIPENTRY ze;
            GetZipItem(hz, zi, &ze);            // fetch individual details

            CString filePathFromZip(ze.name);

            CString fileName = filePathFromZip.Mid(filePathFromZip.ReverseFind('/') + 1);

            if((fileName.CompareNoCase(TEXT("preview.jpg")) == 0) ||
                (fileName.CompareNoCase(TEXT("preview.jpeg")) == 0))
            {
                CString destString = M2DC::GetPathToM2DCThemesDirectory();
                destString += "\\";
                destString += themeBaseName;
                destString += ".jpeg";

                UnzipItem(hz, zi, destString);

                previewPath = destString;
            }
        }

        CloseZip(hz);
        
        // - getting default null preview path

        TiXmlElement* newThemeElement = new TiXmlElement("Theme");
        newThemeElement->SetAttribute("name", GetConstCharStarFromCString(themeBaseName));
        newThemeElement->SetAttribute("path", GetConstCharStarFromCString(pathToTheme));
        newThemeElement->SetAttribute("preview", GetConstCharStarFromCString(previewPath));

        m2dcElement->LinkEndChild(newThemeElement);

        doc.SaveFile();
    }
}

void M2DC::RemoveFromM2DCThemeList(CString themeName)
{
    CString currentThemeName;

    TiXmlDocument doc(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* m2dcNode = doc.FirstChild("M2DC");
            m2dcNode != NULL;
            m2dcNode = m2dcNode->NextSibling("M2DC"))
        {
            for(TiXmlElement* themeElement = m2dcNode->FirstChildElement("Theme");
                themeElement != NULL;
                themeElement = themeElement->NextSiblingElement("Theme"))
            {
                currentThemeName = themeElement->Attribute("name");

                if(currentThemeName.Compare(themeName) == 0)
                {
                    m2dcNode->RemoveChild(themeElement);
                    doc.SaveFile();
                    return;
                }
            }
        }
    }
}

CString M2DC::GetPathOfM2DCThemeFromName(CString themeName)
{
    CString retVal;

    CString currentThemeName;

    TiXmlDocument doc(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* m2dcNode = doc.FirstChild("M2DC");
            m2dcNode != NULL;
            m2dcNode = m2dcNode->NextSibling("M2DC"))
        {
            for(TiXmlElement* themeElement = m2dcNode->FirstChildElement("Theme");
                themeElement != NULL;
                themeElement = themeElement->NextSiblingElement("Theme"))
            {
                currentThemeName = themeElement->Attribute("name");

                if(currentThemeName.Compare(themeName) == 0)
                {
                    retVal = themeElement->Attribute("path");
                    break;
                }
            }
        }
    }

    return retVal;
}

CString M2DC::GetPathOfM2DCThemePreviewFromName(CString themeName)
{
    CString retVal = M2DC::GetPathToNullThemePreview();

    CString currentThemeName;

    TiXmlDocument doc(GetConstCharStarFromCString(M2DC::GetPathToM2DCThemeListXml()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* m2dcNode = doc.FirstChild("M2DC");
            m2dcNode != NULL;
            m2dcNode = m2dcNode->NextSibling("M2DC"))
        {
            for(TiXmlElement* themeElement = m2dcNode->FirstChildElement("Theme");
                themeElement != NULL;
                themeElement = themeElement->NextSiblingElement("Theme"))
            {
                currentThemeName = themeElement->Attribute("name");

                if(currentThemeName.Compare(themeName) == 0)
                {
                    retVal = themeElement->Attribute("preview");
                    break;
                }
            }
        }
    }

    if(retVal.GetLength() <= 0)
    {
        retVal = M2DC::GetPathToNullThemePreview();
    }

    return retVal;
}



void M2DC::ReadLauncherValuesFromXml(int &numberOfColumns, int &numberOfRows)
{
    numberOfRows = 0;
    numberOfColumns = 0;
    
    std::string launcherRowStr = "IDLAUNCHERWG_ROW";
    std::string launcherColStr = "IDLAUNCHERWG_COLUMN";

    TiXmlDocument doc(M2DC::GetConstCharStarFromCString(M2DC::GetPathToHTCHomeSettingsXmlFileActual()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
            htcHomeNode != NULL;
            htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
        {
            for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                widgetPropertyNode != NULL;
                widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
            {
                for(TiXmlNode* launcherWidgetNode = widgetPropertyNode->FirstChild("LauncherWidget");
                    launcherWidgetNode != NULL;
                    launcherWidgetNode = launcherWidgetNode->NextSibling("LauncherWidget"))
                {
                    for(TiXmlElement* launcherWidgetChildElement = launcherWidgetNode->FirstChildElement();
                        launcherWidgetChildElement != NULL;
                        launcherWidgetChildElement = launcherWidgetChildElement->NextSiblingElement())
                    {
                        std::string currentPropertyName = launcherWidgetChildElement->Attribute("name");

                        if(currentPropertyName.compare(launcherColStr) == 0)
                        {
                            launcherWidgetChildElement->QueryIntAttribute("value", &numberOfColumns);
                        }
                        else if(currentPropertyName.compare(launcherRowStr) == 0)
                        {
                            launcherWidgetChildElement->QueryIntAttribute("value", &numberOfRows);
                        }
                    }
                }
            }
        }
    }
}

void M2DC::WriteLauncherValuesToXml(int numberOfColumns, int numberOfRows)
{
    if((numberOfColumns < 3) || (numberOfColumns > 5))
    {
        AfxMessageBox(TEXT("ERROR numcol != 3, 4, or 5"));
        return;
    }

    if((numberOfRows%3 != 0) || (numberOfColumns < 3) || (numberOfColumns > 9))
    {
        AfxMessageBox(TEXT("ERROR numrow != 3, 6, or 9"));
        return;
    }

    std::string launcherRowStr = "IDLAUNCHERWG_ROW";
    std::string launcherColStr = "IDLAUNCHERWG_COLUMN";
    std::string launcherStartPointStr = "IDLAUNCHERWG_START_POINT";
    std::string launcherYIntervalStr = "IDLAUNCHERWG_Y_INTERVAL";

    char intVal[2];
    intVal[0] = '0' + numberOfRows;
    intVal[1] = '\0';
    std::string rowStr = intVal;

    intVal[0] = '0' + numberOfColumns;
    intVal[1] = '\0';
    std::string colStr = intVal;

    std::string startPoint = "20, 2";
    if(numberOfColumns == 4)
    {
        startPoint = "5, 2";
    }
    else if(numberOfColumns == 5)
    {
        startPoint = "0, 0";
    }

    std::string yIntStr = "2";
    if(numberOfColumns == 5)
    {
        yIntStr = "0";
    }

    TiXmlDocument doc(M2DC::GetConstCharStarFromCString(M2DC::GetPathToHTCHomeSettingsXmlFileWorking()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
            htcHomeNode != NULL;
            htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
        {
            for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                widgetPropertyNode != NULL;
                widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
            {
                for(TiXmlNode* launcherWidgetNode = widgetPropertyNode->FirstChild("LauncherWidget");
                    launcherWidgetNode != NULL;
                    launcherWidgetNode = launcherWidgetNode->NextSibling("LauncherWidget"))
                {
                    for(TiXmlElement* launcherWidgetChildElement = launcherWidgetNode->FirstChildElement();
                        launcherWidgetChildElement != NULL;
                        launcherWidgetChildElement = launcherWidgetChildElement->NextSiblingElement())
                    {
                        std::string currentPropertyName = launcherWidgetChildElement->Attribute("name");

                        if(currentPropertyName.compare(launcherColStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", colStr.c_str());
                        }
                        else if(currentPropertyName.compare(launcherStartPointStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", startPoint.c_str());
                        }
                        else if(currentPropertyName.compare(launcherRowStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", rowStr.c_str());
                        }
                        else if(currentPropertyName.compare(launcherYIntervalStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", yIntStr.c_str());
                        }
                    }
                }
            }
        }

        doc.SaveFile();
    }
}

void M2DC::ReadTabValuesFromXml(NameAndEnabledState_vector_t* tabValues)
{
    TiXmlDocument doc(M2DC::GetConstCharStarFromCString(M2DC::GetPathToHTCHomeSettingsXmlFileActual()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
            htcHomeNode != NULL;
            htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
        {
            for(TiXmlNode* tabsNode = htcHomeNode->FirstChild("Tabs");
                tabsNode != NULL;
                tabsNode = tabsNode->NextSibling("Tabs"))
            {
                for(TiXmlElement* tabsNodeChildElement = tabsNode->FirstChildElement();
                    tabsNodeChildElement != NULL;
                    tabsNodeChildElement = tabsNodeChildElement->NextSiblingElement())
                {
                    NameAndEnabledStateItem newTabEntry;
                    newTabEntry.name = tabsNodeChildElement->Value();

                    int enabled;
                    tabsNodeChildElement->QueryIntAttribute("enable", &enabled);
                    newTabEntry.enabled = (enabled == 0 ? FALSE : TRUE);

                    tabValues->push_back(newTabEntry);
                }
            }
        }
    }
}

void M2DC::WriteTabValuesToXml(NameAndEnabledState_vector_t* tabValues)
{

    TiXmlDocument doc(M2DC::GetConstCharStarFromCString(M2DC::GetPathToHTCHomeSettingsXmlFileWorking()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlNode* firstHtcHomeNode = doc.FirstChild("HTCHome");

        if(firstHtcHomeNode != NULL)
        {
            for(TiXmlNode* nextHtcHomeNode = firstHtcHomeNode->NextSibling("HTCHome");
                nextHtcHomeNode != NULL;
                nextHtcHomeNode = nextHtcHomeNode->NextSibling("HTCHome"))
            {
                // copy any child nodes to the first htchome node
                for(TiXmlNode* nextHtcHomeNodeChild = nextHtcHomeNode->FirstChild();
                    nextHtcHomeNodeChild != NULL;
                    nextHtcHomeNodeChild = nextHtcHomeNodeChild->NextSibling())
                {
                    firstHtcHomeNode->InsertEndChild(*nextHtcHomeNodeChild);
                }

                // remove this secondary htchomenode
                doc.RemoveChild(nextHtcHomeNode);
            }

            TiXmlNode* firstTabsNode = firstHtcHomeNode->FirstChild("Tabs");
            
            if(firstTabsNode != NULL)
            {
                for(TiXmlNode* nextTabsNode = firstTabsNode->NextSibling("Tabs");
                    nextTabsNode != NULL;
                    nextTabsNode = nextTabsNode->NextSibling("Tabs"))
                {
                    for(TiXmlNode* nextTabsNodeChild = nextTabsNode->FirstChild();
                        nextTabsNodeChild != NULL;
                        nextTabsNodeChild = nextTabsNodeChild->NextSibling())
                    {
                        firstTabsNode->InsertEndChild(*nextTabsNodeChild);
                    }

                    firstHtcHomeNode->RemoveChild(nextTabsNode);
                }

                // now we are guaranteed to be in the only tabs node that exists
                vector<TiXmlElement> elementList;

                // go through, set the attribute for enabled, copy the element to the vector, remove the element
                for(TiXmlElement* tabsNodeChildElement = firstTabsNode->FirstChildElement();
                    tabsNodeChildElement != NULL;
                    tabsNodeChildElement = tabsNodeChildElement->NextSiblingElement())
                {
                    CString elementName(tabsNodeChildElement->Value());

                    for(size_t i=0; i<tabValues->size(); i++)
                    {
                        if(elementName == (*tabValues)[i].name)
                        {
                            tabsNodeChildElement->SetAttribute("enable", (*tabValues)[i].enabled);
                            elementList.push_back(*tabsNodeChildElement);
                            firstTabsNode->RemoveChild(tabsNodeChildElement);
                            break;
                        }
                    }
                }

                for(size_t i=0; i<tabValues->size(); i++)
                {
                    for(size_t j=0; j<elementList.size(); j++)
                    {
                        CString elementName(elementList[j].Value());

                        if(elementName == (*tabValues)[i].name)
                        {
                            firstTabsNode->InsertEndChild(elementList[j]);
                            break;
                        }
                    }
                }
            }
        }

        doc.SaveFile();
    }
}

void M2DC::SetNewTskTheme(CString pathToTskTheme)
{
    return;

    /*
    if(!FileExists(TEXT("\\Windows\\cusTSK.exe")))
    {
        return;
    }

    PROCESS_INFORMATION pi;
    HKEY hKey;
    LONG lRet;

    //
    // Set the theme
    //
    lRet = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Today"), 0, 0, &hKey);

    if(ERROR_SUCCESS == lRet)
    {
        RegDeleteValue(hKey, _T("UseStartImage"));

        CString wceloadCmdLine = TEXT("/safe /noui /nouninstall /delete 0 ");

        if(pathToTskTheme[0] != '\"')
        {
            wceloadCmdLine += '\"';
        }

        wceloadCmdLine += pathToTskTheme;

        if(pathToTskTheme[pathToTskTheme.GetLength()-1] != '\"')
        {
            wceloadCmdLine += '\"';
        }

        if(::CreateProcess(_T("\\Windows\\wceload.exe"), 
            wceloadCmdLine.GetBuffer(),
            NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi))
        {
            ::WaitForSingleObject(pi.hProcess, INFINITE);

            RegSetValueEx(hKey, _T("Skin"), 0, REG_SZ, (BYTE*)pathToTskTheme.GetBuffer(),
                sizeof(TCHAR) * (wcslen(pathToTskTheme.GetBuffer()) + 1));

            RegCloseKey(hKey);

            CString custskCmdLine;

            if(pathToTskTheme[0] != '\"')
            {
                custskCmdLine += '\"';
            }

            custskCmdLine += pathToTskTheme;

            if(pathToTskTheme[pathToTskTheme.GetLength()-1] != '\"')
            {
                custskCmdLine += '\"';
            }

            if(::CreateProcess(_T("\\Windows\\cusTSK.exe"), 
                custskCmdLine.GetBuffer(),
                NULL, NULL, FALSE, INHERIT_CALLER_PRIORITY, NULL, NULL, NULL, &pi))
            {
                ::WaitForSingleObject(pi.hProcess, 10000);
            }
 
            ::SendMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0); 
            g_bRestoreTodayScreenNeeded = true;
        }
    }
    */
}

void M2DC::ReadManilla2DFontFromRegistry(Manilla2DFontObject* pM2dfo)
{
    if(pM2dfo == NULL)
    {
        return;
    }

    pM2dfo->fontBold = 0;
    pM2dfo->fontItalic = 0;
    pM2dfo->fontFmt = 3;
    pM2dfo->fontRc = _T("0,0,0,0");
    pM2dfo->fontColor = _T("255,255,255");
    pM2dfo->fontSize = 0;
    pM2dfo->fontFace = _T("");
    pM2dfo->fontDefault = true;

    LONG retVal;
    CRegKey rKey;
    TCHAR valueBuffer[MAX_PATH];
    DWORD valueBufferSize = MAX_PATH;

    DWORD dwordValue;

    retVal = rKey.Open(HKEY_LOCAL_MACHINE, pM2dfo->registryKey);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }

    valueBufferSize = MAX_PATH;
    retVal = rKey.QueryStringValue(_T("Face"), valueBuffer, &valueBufferSize);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }
    pM2dfo->fontFace = valueBuffer;

    retVal = rKey.QueryDWORDValue(_T("Size"), dwordValue);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }
    pM2dfo->fontSize = dwordValue;

    retVal = rKey.QueryDWORDValue(_T("Fmt"), dwordValue);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }
    pM2dfo->fontFmt = dwordValue;

    valueBufferSize = MAX_PATH;
    retVal = rKey.QueryStringValue(_T("Clr"), valueBuffer, &valueBufferSize);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }
    pM2dfo->fontColor = valueBuffer;
    
    retVal = rKey.QueryDWORDValue(_T("Bold"), dwordValue);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }
    pM2dfo->fontBold = dwordValue;
    
    retVal = rKey.QueryDWORDValue(_T("Italic"), dwordValue);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }
    pM2dfo->fontItalic = dwordValue;
    
    valueBufferSize = MAX_PATH;
    retVal = rKey.QueryStringValue(_T("Rc"), valueBuffer, &valueBufferSize);
    if(retVal != ERROR_SUCCESS)
    {
        pM2dfo->fontDefault = true;
        return;
    }
    pM2dfo->fontRc = valueBuffer;

    pM2dfo->fontDefault = false;
}

void M2DC::WriteManilla2DFontToRegistry(Manilla2DFontObject* pM2dfo)
{
    if(pM2dfo == NULL)
    {
        return;
    }

    CRegKey rKey;

    if(pM2dfo->fontDefault)
    {
        LONG retVal = rKey.Open(HKEY_LOCAL_MACHINE, _T(""));

        if(retVal == ERROR_SUCCESS)
        {
            rKey.DeleteSubKey(pM2dfo->registryKey);

            CString debugMsg = _T("Deleted key ");
            debugMsg += pM2dfo->registryKey;
        }
    }
    else
    {
        LONG retVal = rKey.Open(HKEY_LOCAL_MACHINE, pM2dfo->registryKey);

        if(retVal == ERROR_SUCCESS)
        {
            rKey.SetStringValue(_T("Face"), pM2dfo->fontFace);
            rKey.SetDWORDValue(_T("Size"), pM2dfo->fontSize);
            rKey.SetDWORDValue(_T("Fmt"), pM2dfo->fontFmt);
            rKey.SetStringValue(_T("Clr"), pM2dfo->fontColor);
            rKey.SetDWORDValue(_T("Bold"), pM2dfo->fontBold);
            rKey.SetDWORDValue(_T("Italic"), pM2dfo->fontItalic);
            rKey.SetStringValue(_T("Rc"), pM2dfo->fontRc);
        }
    }
}

CString M2DC::ReadBrowserExeFromRegistry()
{
    CString retString;

    CRegKey rKey;
    TCHAR valueBuffer[MAX_PATH];
    DWORD valueBufferSize = MAX_PATH;

    LONG retVal = rKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\HTC\\Manila2D\\InternetWidget"));

    if(retVal == ERROR_SUCCESS)
    {
        valueBufferSize = MAX_PATH;
        retVal = rKey.QueryStringValue(_T("OperaEXEFile"), valueBuffer, &valueBufferSize);
        if(retVal != ERROR_SUCCESS)
        {
            retString = _T("");
        }
        else
        {
            retString = valueBuffer;
        }
    }

    return retString;
}

void M2DC::WriteBrowserExeToRegistry(CString newBrowserPath)
{
    CRegKey rKey;

    LONG retVal = rKey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\HTC\\Manila2D\\InternetWidget"));

    if(retVal == ERROR_SUCCESS)
    {
        rKey.SetStringValue(_T("OperaEXEFile"), newBrowserPath);
    }
}
