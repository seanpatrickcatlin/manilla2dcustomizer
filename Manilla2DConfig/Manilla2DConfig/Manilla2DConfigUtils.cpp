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
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigProgressDlg.h"

#include "zip.h"
#include "unzip.h"

#include "tinyxml.h"
#include "tinystr.h"

#include <stdio.h>
#include <time.h>


std::string g_tempStr;
CString g_installDirectory;
bool g_bThemeSupportEnabled = false;
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

CString M2DC::GetPathToHTCHomeSettingsXmlFileActiveTheme()
{
	CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\HTCHomeSettings-ActiveTheme.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileActive Theme: ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    if(!g_bAlreadyBeganMakingChanges)
    {
        BeginMakingChanges();
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
	CString retVal = GetPathToWindowsDirectory();
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

CString M2DC::GetDirectoryOfFile(CString fullFilePath)
{
	CString retVal = fullFilePath;

	int pos = retVal.ReverseFind('\\');

	if((pos > 0) && (pos < retVal.GetLength()))
	{
		retVal = retVal.Mid(0, pos);
	}

	TRACE(TEXT("GetDirectoryOfFile "));
	TRACE(fullFilePath);
	TRACE(TEXT(", "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

	return retVal;
}


CString M2DC::GetPathToWindowsDirectory()
{
	CString retVal("\\Windows");

	TCHAR winDirStr[MAX_PATH];
	if(SHGetSpecialFolderPath(NULL, winDirStr, CSIDL_WINDOWS, 0) == TRUE)
	{
		retVal = winDirStr;
	}

	TRACE(TEXT("GetPathToWindowsDirectory "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

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
	    retVal = GetDirectoryOfFile(GetPathToRunningBinary());
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

    if(!M2DC::FileExists(retVal))
    {
        CreateDirectory(retVal, NULL);
    }

	return retVal;
}

bool M2DC::IsDirEmpty(CString dirPath)
{
    bool retVal = true;

    CString searchString = dirPath;
    searchString += "\\*";

    WIN32_FIND_DATA findData;
    HANDLE hFindHandle = FindFirstFile(searchString, &findData);

    if(hFindHandle != INVALID_HANDLE_VALUE)
    {
        BOOL keepSearching = TRUE;

        while(keepSearching == TRUE)
        {
            if((lstrcmp(findData.cFileName, TEXT(".")) != 0) && (lstrcmp(findData.cFileName, TEXT("..")) != 0))
            {
                retVal = false;
                break;
            }

            keepSearching = FindNextFile(hFindHandle, &findData);
        }
    }

    FindClose(hFindHandle);

    return retVal;
}

CString M2DC::GetPathToM2DCOldActiveThemeDirectory()
{
    CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\";
    retVal += "ActiveTheme";

    TRACE(TEXT("GetPathToM2DCOldActiveThemeDirectory "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

	return retVal;
}

CString M2DC::GetPathToRunningBinary()
{
	CString retVal("\\");

	TCHAR binaryPath[MAX_PATH];
	if(GetModuleFileName(NULL, binaryPath, MAX_PATH) > 0)
	{
		retVal = binaryPath;
	}

	TRACE(TEXT("GetPathToRunningBinary "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

	return retVal;
}

bool M2DC::FileExists(CString pathToFile)
{
    bool retVal = false;

    WIN32_FIND_DATA emptyStruct;

    HANDLE fileHndl = FindFirstFile(pathToFile, &emptyStruct);
    if(fileHndl != INVALID_HANDLE_VALUE)
    {
        FindClose(fileHndl);
        retVal = true;
    }

    return retVal;
}

bool M2DC::DirExists(CString pathToDir)
{
    bool retVal = false;

    WIN32_FIND_DATA emptyStruct;

    HANDLE fileHndl = FindFirstFile(pathToDir, &emptyStruct);
    if(fileHndl != INVALID_HANDLE_VALUE)
    {
        if(emptyStruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            retVal = true;
        }

        FindClose(fileHndl);
    }

    return retVal;
}

void M2DC::RefreshTodayScreen()
{
    // Send message to refresh today screen
    ::SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);
}

void M2DC::BackupM2DCFiles()
{
    BackupHTCHomeSettingsXml(true);
    BackupActualTheme(true);
}

void M2DC::BackupHTCHomeSettingsXml(bool overwritePreviousBackup)
{
    bool fileExists = FileExists(GetPathToHTCHomeSettingsXmlFileBackup());

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

int M2DC::BackupActualTheme(bool overwritePreviousBackup)
{
    int retVal = 0;
    AfxGetApp()->BeginWaitCursor();

    bool fileExists = FileExists(GetPathToThemeBackupFile());

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

    return retVal;
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

void M2DC::RestoreM2DCFiles()
{
    BeginMakingChanges();
    AfxGetApp()->BeginWaitCursor();
    
    CString zipBackupPath = GetPathToThemeBackupFile();
    CString workingXmlPath = GetPathToHTCHomeSettingsXmlFileWorking();
    CString backupXmlPath = GetPathToHTCHomeSettingsXmlFileBackup();

    if(FileExists(zipBackupPath))
    {
        HZIP hz = OpenZip(zipBackupPath, 0);
        ZIPENTRY ze;

        // -1 gives overall information about the zipfile
        GetZipItem(hz, -1, &ze);

        int numitems = ze.index;

        CString progMsg;
        CManilla2DConfigProgressDlg* pProgDlg = NULL;

        if(g_bAllowPopupDialogs)
        {
            pProgDlg = new CManilla2DConfigProgressDlg(AfxGetApp()->GetMainWnd());
        }

        if(pProgDlg)
        {
            progMsg = TEXT("Restoring theme from backup");
            pProgDlg->BeginTrackingProgress(progMsg, 0, numitems);
        }

        int retVal = 0;
        
        for(int zi=0; zi<numitems; zi++)
        {
            ZIPENTRY ze;
            
            GetZipItem(hz, zi, &ze);            // fetch individual details

            if(pProgDlg)
            {
                progMsg.Format(TEXT("Restoring theme from backup\nFile %d of %d\n%s"), zi, numitems, ze.name);            
                pProgDlg->UpdateStatus(progMsg, zi);
            }

            UnzipItem(hz, zi, ze.name);
        }

        if(pProgDlg != NULL)
        {
            pProgDlg->EndTrackingProgress();
            delete pProgDlg;
            pProgDlg = NULL;
        }

        CloseZip(hz);
    }

    if(FileExists(backupXmlPath))
    {
        CopyFile(backupXmlPath, workingXmlPath, FALSE);
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

void M2DC::RecursivelyDeleteDirectory(CString sDirPath)
{
    // Declare variables
    WIN32_FIND_DATA wfd;
    HANDLE hFile;
    DWORD dwFileAttr;
    CString sFile;

    CString sPathFile;

    if(sDirPath[sDirPath.GetLength()-1] != '\\')
    {
        sDirPath += '\\';
    }

    CString sSpec = sDirPath;
    sSpec += "*.*";

    // Find the first file
    hFile = FindFirstFile(sSpec, &wfd);

    if(hFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            sFile = wfd.cFileName;
            sPathFile = sDirPath + sFile;
            // Get the file attributes
            dwFileAttr = GetFileAttributes(sPathFile);

            // See if file is read-only : if so unset read-only
            if (dwFileAttr & FILE_ATTRIBUTE_READONLY)
            {
                dwFileAttr &= ~FILE_ATTRIBUTE_READONLY;
                SetFileAttributes(sPathFile, dwFileAttr);
            }

            // See if the file is a directory
            if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // Make sure it isn't current or parent directory
                if (sFile != "." && sFile != "..")
                {
                    sPathFile += "\\";

                    // Recursively delete all files in this folder
                    RecursivelyDeleteDirectory(sPathFile);

                    // Remove the directory
                    RemoveDirectory(sPathFile);
                }
            }
            else
            {
                // Delete the file
                DeleteFile(sPathFile);
            }
        }
        while(FindNextFile(hFile, &wfd));
    }

    // Close handle to file
    FindClose(hFile);

    RemoveDirectory(sDirPath);
}

void M2DC::GetVectorOfThemeFilesCurrentlyInUse(std::vector<CString>* pPathVector, bool includeNonXmlFiles)
{
    int progMax = 10;
    CString progMsg;
    CManilla2DConfigProgressDlg* pProgDlg = NULL;

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

        TiXmlDocument doc(GetConstCharStarFromCString(GetPathToHTCHomeSettingsXmlFileActual()));
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

                                if(pProgDlg != NULL)
                                {
                                    progMsg = TEXT("Building M2D file list\nStep 1 of 3\n");
                                    progMsg += currentFilePath;
                                    pProgDlg->UpdateStatus(progMsg, (progMax/4)*1);
                                }

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

                                    if(pProgDlg != NULL)
                                    {
                                        progMsg = TEXT("Building M2D file list\nStep 2 of 3\n");
                                        progMsg += currentFilePath;
                                        pProgDlg->UpdateStatus(progMsg, (progMax/4)*2);
                                    }

                                    pPathVector->push_back(currentFilePath);
                                }
                            }
                        }
                    }
                }
            }
        }

        if(includeNonXmlFiles)
        {
            pPathVector->push_back(GetPathToHTCHomeSettingsXmlFileActual());

            WIN32_FIND_DATA findData;
            CString findString = GetPathToWindowsDirectory();
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
                        CString currentFilePath = GetPathToWindowsDirectory();
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

bool M2DC::IsM2DCThemeSupportEnabled()
{
    if(!g_bThemeSupportEnabled)
    {
        AfxGetApp()->BeginWaitCursor();

        if(DirExists(GetPathToM2DCOldActiveThemeDirectory()))
        {
            CString msg;
            msg = "Oops! M2DC v0.6.97 and v0.7.104 had a bug in the theme support implementation.";
            msg += "  Disabling theme support and restoring default theme now";
            AfxMessageBox(msg);

            RestoreM2DCFiles();
            DeleteFile(GetPathToThemeBackupFile());
            RecursivelyDeleteDirectory(GetPathToM2DCOldActiveThemeDirectory());
        }

        // check to see if the Zip file exists
        g_bThemeSupportEnabled = FileExists(GetPathToThemeBackupFile());

        AfxGetApp()->EndWaitCursor();
    }

    return g_bThemeSupportEnabled;
}

int M2DC::EnableM2DCThemeSupport()
{
    int retVal = BackupActualTheme(true);

    g_bThemeSupportEnabled = (retVal == 0);

    return retVal;
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

int M2DC::SetActiveTheme(CString pathToTheme)
{
    int retVal = 0;    

    CString themeName = pathToTheme.Mid(pathToTheme.Find('\\')+1);;
    themeName = themeName.Mid(0, themeName.Find('.')-1);

    // unzip the files to their appropriate destinations according to the
    // paths from the current XML file
    if(FileExists(pathToTheme))
    {
        BeginMakingChanges();
        AfxGetApp()->BeginWaitCursor();

        std::vector<CString> pathsToThemeFiles;

        GetVectorOfThemeFilesCurrentlyInUse(&pathsToThemeFiles, true);

        HZIP hz = OpenZip(pathToTheme, 0);
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

            if(fileNameNoPath.Compare(TEXT("HTCHomeSettings.xml")) == 0)
            {
                destString = GetPathToHTCHomeSettingsXmlFileActiveTheme();
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
        CString pathToThemeXml = GetPathToHTCHomeSettingsXmlFileActiveTheme();

        HTCHomeSettingsStruct xmlSettings;

        ReadValuesFromXml(pathToThemeXml, &xmlSettings);
        WriteValuesToXml(GetPathToHTCHomeSettingsXmlFileWorking(), &xmlSettings);

        AfxGetApp()->EndWaitCursor();
        EndMakingChanges();
    }

    return retVal;
}

CString M2DC::GetFileBaseName(CString filePath)
{
    CString retVal = filePath;

    retVal = retVal.Mid(retVal.ReverseFind('\\'));
    retVal = retVal.Mid(0, retVal.Find('.'));

    return retVal;
}

void M2DC::GetNamesOfInstalledThemes(std::vector<CString>* pThemeNameVector)
{
    if(pThemeNameVector != NULL)
    {
        CString searchString = GetPathToM2DCThemesDirectory();
        searchString += "\\*.m2dct";

        WIN32_FIND_DATA findData;
        HANDLE hFindHandle = FindFirstFile(searchString, &findData);

        if(hFindHandle != INVALID_HANDLE_VALUE)
        {
            BOOL keepSearching = TRUE;

            while(keepSearching == TRUE)
            {
                if((lstrcmp(findData.cFileName, TEXT(".")) != 0) && (lstrcmp(findData.cFileName, TEXT("..")) != 0))
                {
                    pThemeNameVector->push_back(GetFileBaseName(findData.cFileName));
                }

                keepSearching = FindNextFile(hFindHandle, &findData);
            }
        }

        FindClose(hFindHandle);
    }
}

void M2DC::ReadValuesFromXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings)
{
    if((xmlSettings != NULL) && (FileExists(xmlFilePath)))
    {
        GetClockEnabledState(xmlFilePath, &xmlSettings->bIsAnalogClockEnabled, &xmlSettings->bIsDigitalClockEnabled);

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
                    for(TiXmlNode* homeWidgetNode = widgetPropertyNode->FirstChild("HomeWidget");
                        homeWidgetNode != NULL;
                        homeWidgetNode = homeWidgetNode->NextSibling("HomeWidget"))
                    {
                        for(TiXmlElement* homeWidgetChildElement = homeWidgetNode->FirstChildElement();
                            homeWidgetChildElement != NULL;
                            homeWidgetChildElement = homeWidgetChildElement->NextSiblingElement())
                        {
                            xmlSettings->homeWidgetPropertyElements.push_back(*homeWidgetChildElement);
                        }
                    }

                    for(TiXmlNode* tabWidgetNode = widgetPropertyNode->FirstChild("TabWidget");
                        tabWidgetNode != NULL;
                        tabWidgetNode = tabWidgetNode->NextSibling("TabWidget"))
                    {
                        for(TiXmlElement* tabWidgetChildElement = tabWidgetNode->FirstChildElement();
                            tabWidgetChildElement != NULL;
                            tabWidgetChildElement = tabWidgetChildElement->NextSiblingElement())
                        {
                            xmlSettings->tabWidgetPropertyElements.push_back(*tabWidgetChildElement);
                        }
                    }
                }
            }
        }
    }
}

void M2DC::WriteValuesToXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings)
{
    if((xmlSettings != NULL) && (FileExists(xmlFilePath)))
    {
        SetClockEnabledState(xmlFilePath, xmlSettings->bIsAnalogClockEnabled, xmlSettings->bIsDigitalClockEnabled);

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
                    for(TiXmlNode* homeWidgetNode = widgetPropertyNode->FirstChild("HomeWidget");
                        homeWidgetNode != NULL;
                        homeWidgetNode = homeWidgetNode->NextSibling("HomeWidget"))
                    {
                        for(TiXmlElement* homeWidgetChildElement = homeWidgetNode->FirstChildElement();
                            homeWidgetChildElement != NULL;
                            homeWidgetChildElement = homeWidgetChildElement->NextSiblingElement())
                        {
                            currentNameAttribute = homeWidgetChildElement->Attribute("name");

                            for(size_t i=0; i<xmlSettings->homeWidgetPropertyElements.size(); i++)
                            {
                                vectorNameAttribute = xmlSettings->homeWidgetPropertyElements[i].Attribute("name");
                                vectorValueAttribute = xmlSettings->homeWidgetPropertyElements[i].Attribute("value");

                                if(vectorNameAttribute.Compare(currentNameAttribute) == 0)
                                {
                                    homeWidgetChildElement->SetAttribute("value",
                                        GetConstCharStarFromCString(vectorValueAttribute));
                                    break;
                                }
                            }
                        }
                    }

                    for(TiXmlNode* tabWidgetNode = widgetPropertyNode->FirstChild("TabWidget");
                        tabWidgetNode != NULL;
                        tabWidgetNode = tabWidgetNode->NextSibling("TabWidget"))
                    {
                        for(TiXmlElement* tabWidgetChildElement = tabWidgetNode->FirstChildElement();
                            tabWidgetChildElement != NULL;
                            tabWidgetChildElement = tabWidgetChildElement->NextSiblingElement())
                        {
                            currentNameAttribute = tabWidgetChildElement->Attribute("name");

                            for(size_t i=0; i<xmlSettings->tabWidgetPropertyElements.size(); i++)
                            {
                                vectorNameAttribute = xmlSettings->tabWidgetPropertyElements[i].Attribute("name");
                                vectorValueAttribute = xmlSettings->tabWidgetPropertyElements[i].Attribute("value");

                                if(vectorNameAttribute.Compare(currentNameAttribute) == 0)
                                {
                                    tabWidgetChildElement->SetAttribute("value",
                                        GetConstCharStarFromCString(vectorValueAttribute));
                                    break;
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

bool M2DC::ArchiveContainsHTCHomeSettingsXml(CString filePath)
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
        retVal = (filePathFromZip.Find(TEXT("HTCHomeSettings.xml")) != -1);
    }

    CloseZip(hz);
    AfxGetApp()->EndWaitCursor();

    return retVal;
}

void M2DC::GetClockEnabledState(CString pathToXmlFile, bool* pIsAnalogEnabled, bool* pIsDigitalEnabled)
{
    int analogEnabledCount = 0;
    int analogDisabledCount = 0;
    int digitalEnabledCount = 0;
    int digitalDisabledCount = 0;

    if((pIsAnalogEnabled != NULL) && (pIsDigitalEnabled != NULL) && (FileExists(pathToXmlFile)))
    {
        TiXmlDocument doc(GetConstCharStarFromCString(pathToXmlFile));
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
                    for(TiXmlNode* imageListImageNode = imageListNode->FirstChild();
                        imageListImageNode != NULL;
                        imageListImageNode = imageListImageNode->NextSibling())
                    {
                        CString strVal;

                        TiXmlComment* imageListImageComment = imageListImageNode->ToComment();

                        if(imageListImageComment != NULL)
                        {
                            strVal = imageListImageComment->Value();
                        }

                        TiXmlElement* imageListImageElement = imageListImageNode->ToElement();

                        if(imageListImageElement != NULL)
                        {
                            strVal = imageListImageElement->Attribute("name");
                        }

                        if(strVal.Find(TEXT("hh_hm_aclock")) != -1)
                        {
                            if((imageListImageComment != NULL) || (strVal[strVal.GetLength()-1] == 'x'))
                            {
                                analogDisabledCount++;
                            }
                            else
                            {
                                analogEnabledCount++;
                            }
                        }

                        if((strVal.Find(TEXT("hh_hm_digit")) != -1) ||
                            (strVal.Find(TEXT("hh_hm_am")) != -1) ||
                            (strVal.Find(TEXT("hh_hm_pm")) != -1) ||
                            (strVal.Find(TEXT("hh_hm_colon")) != -1))
                        {
                            if((imageListImageComment != NULL) || (strVal[strVal.GetLength()-1] == 'x'))
                            {
                                digitalDisabledCount++;
                            }
                            else
                            {
                                digitalEnabledCount++;
                            }
                        }
                    }
                }
            }
        }
    }

    *pIsAnalogEnabled = (analogEnabledCount != 0);
    *pIsDigitalEnabled = (digitalEnabledCount != 0);
}

/*
    <Image index="43" name="hh_hm_am.png" />
    <Image index="44" name="hh_hm_pm.png" />
    <Image index="47" name="hh_hm_colon.png" />
    <Image index="48" name="hh_hm_digit_0.png" />
    <Image index="49" name="hh_hm_digit_1.png" />
    <Image index="50" name="hh_hm_digit_2.png" />
    <Image index="51" name="hh_hm_digit_3.png" />
    <Image index="52" name="hh_hm_digit_4.png" />
    <Image index="53" name="hh_hm_digit_5.png" />
    <Image index="54" name="hh_hm_digit_6.png" />
    <Image index="55" name="hh_hm_digit_7.png" />
    <Image index="56" name="hh_hm_digit_8.png" />
    <Image index="57" name="hh_hm_digit_9.png" />
*/

/*
    <Image index="121" name="hh_hm_aclock_alarm.bmpx" />
    <Image index="122" name="hh_hm_aclock_alarm_mask.bmpx" />
    <Image index="123" name="hh_hm_aclock_alarm_shadow.bmpx" />
    <Image index="124" name="hh_hm_aclock_alarm_shadow_mask.bmpx" />
    <Image index="125" name="hh_hm_aclock_hour.bmpx" />
    <Image index="126" name="hh_hm_aclock_hour_mask.bmpx" />
    <Image index="127" name="hh_hm_aclock_hour_shadow.bmpx" />
    <Image index="128" name="hh_hm_aclock_hour_shadow_mask.bmpx" />
    <Image index="209" name="hh_hm_aclock_Minute.bmpx" />
    <Image index="210" name="hh_hm_aclock_Minute_mask.bmpx" />
    <Image index="211" name="hh_hm_aclock_Minute_shadow.bmpx" />
    <Image index="212" name="hh_hm_aclock_Minute_shadow_mask.bmpx" />
    <Image index="213" name="hh_hm_aclock_Dot.bmpx" />
    <Image index="214" name="hh_hm_aclock_Dot_mask.bmpx" />
    <Image index="215" name="hh_hm_aclock_Dot_shadow.bmpx" />
    <Image index="216" name="hh_hm_aclock_Dot_shadow_mask.bmpx" />
*/

void M2DC::SetClockEnabledState(CString pathToXmlFile, bool analogEnabled, bool digitalEnabled)
{
    if(FileExists(pathToXmlFile))
    {
        TiXmlDocument doc(GetConstCharStarFromCString(pathToXmlFile));
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
                    for(TiXmlNode* imageListImageNode = imageListNode->FirstChild();
                        imageListImageNode != NULL;
                        imageListImageNode = imageListImageNode->NextSibling())
                    {
                        CString strVal;

                        TiXmlComment* imageListImageComment = imageListImageNode->ToComment();

                        if(imageListImageComment != NULL)
                        {
                            //strVal = imageListImageComment->Value();
                        }

                        TiXmlElement* imageListImageElement = imageListImageNode->ToElement();

                        if(imageListImageElement != NULL)
                        {
                            strVal = imageListImageElement->Attribute("name");

                            if(strVal.Find(TEXT("hh_hm_aclock")) != -1)
                            {
                                if(analogEnabled)
                                {

                                    if(strVal[strVal.GetLength()-1] == 'x')
                                    {
                                        strVal = strVal.Mid(0, strVal.GetLength()-1);
                                    }
                                }
                                else
                                {
                                    if(strVal[strVal.GetLength()-1] != 'x')
                                    {
                                        strVal += 'x';
                                    }
                                }

                                imageListImageElement->SetAttribute("name", GetConstCharStarFromCString(strVal));
                            }
                        }

                        if((strVal.Find(TEXT("hh_hm_digit")) != -1) ||
                            (strVal.Find(TEXT("hh_hm_am")) != -1) ||
                            (strVal.Find(TEXT("hh_hm_pm")) != -1) ||
                            (strVal.Find(TEXT("hh_hm_colon")) != -1))
                        {
                            if(digitalEnabled)
                            {
                                if(strVal[strVal.GetLength()-1] == 'x')
                                {
                                    strVal = strVal.Mid(0, strVal.GetLength()-1);
                                }
                            }
                            else
                            {
                                if(strVal[strVal.GetLength()-1] != 'x')
                                {
                                    strVal += 'x';
                                }
                            }

                            imageListImageElement->SetAttribute("name", GetConstCharStarFromCString(strVal));
                        }
                    }
                }
            }
        }

        doc.SaveFile();
    }
}
