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

    if(!FileExists(retVal))
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
    int progMax = 12;
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

    CString themeName = pathToTheme.Mid(pathToTheme.ReverseFind('\\')+1);;
    themeName = themeName.Mid(0, themeName.Find('.')-1);

    // unzip the files to their appropriate destinations according to the
    // paths from the current XML file
    if(FileExists(pathToTheme))
    {
        DeleteFile(GetPathToHTCHomeSettingsXmlFileActiveTheme());

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
    if(FileExists(xmlFilePath) && (xmlSettings != NULL))
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
    if(FileExists(xmlFilePath) && (xmlSettings != NULL))
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

bool M2DC::ArchiveIsValidM2DCTheme(CString filePath)
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
    if(FileExists(pathToXmlFile) && (pHomeWidgetSettings != NULL))
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
    if(FileExists(pathToXmlFile) && (pHomeWidgetSettings != NULL))
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
    if(FileExists(xmlFilePath) && (pElementVector != NULL))
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
