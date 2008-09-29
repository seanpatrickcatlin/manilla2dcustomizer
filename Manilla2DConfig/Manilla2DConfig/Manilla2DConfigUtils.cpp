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
TodayScreenRegBackup g_todayScreenRegBackup;

void PrintNameAndEnabledStateContents(NameAndEnabledState_vector_t* nameAndStateVector)
{
	if(nameAndStateVector == NULL)
	{
		return;
	}

    char tempStr[1024];
    CString debugStr;

    TRACE(TEXT("---------------"));

    for(size_t i=0; i<nameAndStateVector->size(); i++)
    {
        sprintf(tempStr, "Item: %d, Name: ", i);
        debugStr = tempStr;
        debugStr += (*nameAndStateVector)[i].name;
        sprintf(tempStr, ", Enabled: %d\n", (*nameAndStateVector)[i].enabled);
        debugStr += tempStr;

        TRACE(debugStr);
    }
}

bool CompareNameAndEnabledStateVectors(NameAndEnabledState_vector_t* vec1, NameAndEnabledState_vector_t* vec2)
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

CString GetPathToHTCHomeSettingsXmlFileWorking()
{
	CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\HTCHomeSettings-WorkingCopy.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileWorking ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    if(!g_bAlreadyBeganMakingChanges)
    {
        BeginMakingChanges();
    }

    return retVal;
}

CString GetPathToHTCHomeSettingsXmlFileActual()
{
	CString retVal = GetPathToWindowsDirectory();
    retVal += "\\HTCHomeSettings.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileActual ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString GetPathToHTCHomeSettingsXmlFileBackup()
{
	CString retVal = GetPathToM2DCInstallDirectory();
	retVal += "\\HTCHomeSettings-BACKUP.xml";

    CString debugStr = TEXT("GetPathToHTCHomeSettingsXmlFileBackup ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString GetPathToThemeBackupFile()
{
	CString retVal = GetPathToM2DCThemesDirectory();
	retVal += "\\DefaultThemeBACKUP.m2dct";

    CString debugStr = TEXT("GetPathToThemeBackupFile ");
    debugStr += retVal;
    debugStr += "\n";

    TRACE(debugStr);

    return retVal;
}

CString GetDirectoryOfFile(CString fullFilePath)
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


CString GetPathToWindowsDirectory()
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

CString GetPathToM2DCInstallDirectory()
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

CString GetPathToM2DCThemesDirectory()
{
    CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\";
    retVal += "Themes";

    TRACE(TEXT("GetPathToThemesDirectory "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

    if(!FileExists(retVal))
    {
        CreateDirectory(retVal, NULL);
    }

	return retVal;
}

bool IsDirEmpty(CString dirPath)
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

CString GetPathToM2DCActiveThemeDirectory()
{
    CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\";
    retVal += "ActiveTheme";

    TRACE(TEXT("GetPathToThemesDirectory "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

    if(!FileExists(retVal))
    {
        CreateDirectory(retVal, NULL);
    }

	return retVal;
}

CString GetPathToRunningBinary()
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

bool FileExists(CString pathToFile)
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

void RefreshTodayScreen()
{
    // Send message to refresh today screen
    ::SendMessage(HWND_BROADCAST, WM_WININICHANGE, 0xF2, 0);
}

void BackupM2DCFiles()
{
    BackupHTCHomeSettingsXml(true);
    BackupActualTheme(true);
}

void BackupHTCHomeSettingsXml(bool overwritePreviousBackup)
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

int BackupActualTheme(bool overwritePreviousBackup)
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

        TRACE(TEXT("Begin Zip HH_ files\n"));

        std::vector<CString> hh_strVector;
        GetVectorOfThemeFilesCurrentlyInUse(&hh_strVector, true);

        HZIP hz = CreateZip(GetPathToThemeBackupFile(), 0);

        CManilla2DConfigProgressDlg progDlg(AfxGetApp()->GetMainWnd());

        progDlg.BeginTrackingProgress(TEXT("Backing up theme files."), 0, hh_strVector.size());

        for(size_t i=0; ((i<hh_strVector.size()) && (retVal == 0)); i++)
        {
            CString fullFilePath = hh_strVector[i];

            CString archiveFilePath = fullFilePath;
            archiveFilePath.Replace(TEXT("\\"), TEXT("/"));

            CString msg;
            msg.Format(TEXT("Backing Up Current HH_ Files.\nFile %d of %d\n%s"), i, hh_strVector.size(), fullFilePath.GetBuffer());
            retVal = progDlg.UpdateStatus(msg, i);

            ZipAdd(hz, archiveFilePath, fullFilePath);
        }

        progDlg.EndTrackingProgress();

        CloseZip(hz);
        TRACE(TEXT("End Zip HH_ files\n"));
    }

    AfxGetApp()->EndWaitCursor();

    return retVal;
}

const char* GetConstCharStarFromCString(CString str)
{
    CT2CA pszConvertedAnsiString(str);
	g_tempStr = pszConvertedAnsiString;

	return g_tempStr.c_str();
}

CString GetWin32ErrorString(DWORD err)
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

void RestoreM2DCFiles(bool showProgress)
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
        GetZipItem(hz,-1,&ze);

        int numitems = ze.index;

        CString msg;
        CManilla2DConfigProgressDlg* pProgDlg = NULL;

        if(showProgress)
        {
            pProgDlg = new CManilla2DConfigProgressDlg(NULL);

            msg = TEXT("Restoring theme from backup");
            pProgDlg->BeginTrackingProgress(msg, 0, numitems);
        }

        int retVal = 0;
        
        for(int zi=0; ((zi<numitems) && (retVal == 0)); zi++)
        {
            ZIPENTRY ze;
            
            GetZipItem(hz, zi, &ze);            // fetch individual details

            if(showProgress && pProgDlg)
            {
                msg.Format(TEXT("Restoring theme from backup\nFile %d of %d\n%s"), zi, numitems, ze.name);            
                retVal = pProgDlg->UpdateStatus(msg, zi);
            }

            UnzipItem(hz, zi, ze.name);
        }

        if(showProgress && pProgDlg)
        {
            delete pProgDlg;
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

void DisableAllTodayScreenItems()
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


void BackupTodayScreenItemsRegHive()
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

void RestoreTodayScreenItemsRegHive()
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

void BeginMakingChanges()
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

void EndMakingChanges()
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

void RecursivelyDeleteDirectory(CString sDirPath)
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

void GetVectorOfThemeFilesCurrentlyInUse(std::vector<CString>* pPathVector, bool includeNonXmlFiles)
{
    if(pPathVector != NULL)
    {
        CString basePath = TEXT("\\Windows\\");

        TiXmlDocument doc(GetConstCharStarFromCString(GetPathToHTCHomeSettingsXmlFileActual()));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            TiXmlHandle docHandle(&doc);

            TiXmlElement* imageListElement = docHandle.FirstChild("HTCHome").FirstChild("ImageList").Element();

            if(imageListElement != NULL)
            {
                basePath = imageListElement->Attribute("path");

                if(basePath[basePath.GetLength()-1] != '\\')
                {
                    basePath += '\\';
                }

                TiXmlElement* imageListItemElement = imageListElement->FirstChildElement();

                while(imageListItemElement != NULL)
                {
                    CString currentFilePath(imageListItemElement->Attribute("name"));

                    if(currentFilePath.Find(TEXT("HH_")) != -1)
                    {
                        if(currentFilePath.Find('\\') == -1)
                        {
                            currentFilePath = basePath + currentFilePath;
                        }

                        pPathVector->push_back(currentFilePath);
                    }

                    imageListItemElement = imageListItemElement->NextSiblingElement();
                }
            }

            TiXmlNode* widgetPropertyNode = docHandle.FirstChild("HTCHome").FirstChild("WidgetProperty").Node();

            if(widgetPropertyNode != NULL)
            {
                TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild();

                while(widgetPropertyChildNode != NULL)
                {
                    TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->FirstChildElement();

                    while(widgetPropertyChildElement != NULL)
                    {
                        CString currentValue(widgetPropertyChildElement->Attribute("value"));

                        if(currentValue.Find(TEXT("HH_")) != -1)
                        {
                            if(currentValue.Find('\\') == -1)
                            {
                                currentValue = basePath + currentValue;
                            }

                            pPathVector->push_back(currentValue);
                        }

                        widgetPropertyChildElement = widgetPropertyChildElement->NextSiblingElement();
                    }

                    widgetPropertyChildNode = widgetPropertyChildNode->NextSibling();
                }
            }
        }

        if(includeNonXmlFiles)
        {
            pPathVector->push_back(GetPathToHTCHomeSettingsXmlFileActual());

            WIN32_FIND_DATA findData;
            CString findString = GetPathToWindowsDirectory();
            findString += "\\HH_*";
            HANDLE hFindHandle = FindFirstFile(findString, &findData);

            if(hFindHandle != INVALID_HANDLE_VALUE)
            {
                BOOL keepSearching = TRUE;

                while(keepSearching == TRUE)
                {
                    CString currentHH_file = GetPathToWindowsDirectory();
                    currentHH_file += "\\";
                    currentHH_file += findData.cFileName;

                    bool addFileToVector = true;

                    for(size_t i=0; i<pPathVector->size(); i++)
                    {
                        if((*pPathVector)[i].Find(findData.cFileName) != -1)
                        {
                            addFileToVector = false;
                            break;
                        }
                    }

                    if(addFileToVector)
                    {
                        pPathVector->push_back(currentHH_file);
                    }

                    keepSearching = FindNextFile(hFindHandle, &findData);
                }
            }

            FindClose(hFindHandle);
        }
    }
}

bool IsM2DCThemeSupportEnabled()
{
    if(!g_bThemeSupportEnabled)
    {
        AfxGetApp()->BeginWaitCursor();

        // check to see if the Zip file exists
        bool HH_FilesZipBackupExists = FileExists(GetPathToThemeBackupFile());

        // check to see if the local themes folder exists and is not empty
        bool isCurrentThemeDirEmpty = IsDirEmpty(GetPathToM2DCActiveThemeDirectory());

        // check to make sure that the xml file in \Windows references the local theme folder
        bool isXmlFileSetToUseCurrentThemeDir = false;

        std::vector<CString> currentHH_FilePaths;

        GetVectorOfThemeFilesCurrentlyInUse(&currentHH_FilePaths, false);

        for(size_t i=0; i<currentHH_FilePaths.size(); i++)
        {
            isXmlFileSetToUseCurrentThemeDir = (currentHH_FilePaths[i].Find(GetPathToM2DCActiveThemeDirectory()) == 0);

            if(!isXmlFileSetToUseCurrentThemeDir)
            {
                break;
            }
        }

        g_bThemeSupportEnabled = (HH_FilesZipBackupExists && !isCurrentThemeDirEmpty && isXmlFileSetToUseCurrentThemeDir);

        AfxGetApp()->EndWaitCursor();
    }

    return g_bThemeSupportEnabled;
}

int EnableM2DCThemeSupport()
{
    int retVal = -1;

    CString enableMessage = TEXT("Enabling M2DC theme support may take 3-5 minutes to complete.");
    enableMessage += "\nWould you like to continue?";

    if(MessageBox(NULL, enableMessage, TEXT("Enable M2DC Themes?"), MB_YESNO) == IDYES)
    {
        int retVal2 = BackupActualTheme(true);

        if(retVal2 == 0)
        {
            retVal2 = SetActiveTheme(GetPathToThemeBackupFile());
        }

        if(retVal2 == 0)
        {
            SetThemeDirectoryInActualXmlSettingsFile(GetPathToM2DCActiveThemeDirectory());
        }

        g_bThemeSupportEnabled = (retVal2 == 0);
        retVal = retVal2;
    }

    return retVal;
}

void SetThemeDirectoryInActualXmlSettingsFile(CString newDirectory)
{
    BeginMakingChanges();
    AfxGetApp()->BeginWaitCursor();

    // change all file paths from the xml settings file to point to the current theme directory
    TiXmlDocument doc(GetConstCharStarFromCString(GetPathToHTCHomeSettingsXmlFileWorking()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlHandle docHandle(&doc);

        TiXmlElement* imageListElement = docHandle.FirstChild("HTCHome").FirstChild("ImageList").Element();

        if(imageListElement != NULL)
        {
            CT2CA pszConvertedAnsiString(newDirectory);
            std::string tempStr(pszConvertedAnsiString);

            imageListElement->SetAttribute("path", tempStr.c_str());

            TiXmlElement* imageListItemElement = imageListElement->FirstChildElement();

            while(imageListItemElement != NULL)
            {
                CString currentFilePath(imageListItemElement->Attribute("name"));

                if(currentFilePath.Find(TEXT("HH_")) != -1)
                {
                    if(currentFilePath.Find('\\') != -1)
                    {
                        CString fileName = newDirectory;
                        fileName += currentFilePath.Mid(currentFilePath.ReverseFind('\\'));

                        imageListItemElement->SetAttribute("name", GetConstCharStarFromCString(fileName));
                    }
                }

                imageListItemElement = imageListItemElement->NextSiblingElement();
            }
        }

        TiXmlNode* widgetPropertyNode = docHandle.FirstChild("HTCHome").FirstChild("WidgetProperty").Node();

        if(widgetPropertyNode != NULL)
        {
            TiXmlNode* widgetPropertyChildNode = widgetPropertyNode->FirstChild();

            while(widgetPropertyChildNode != NULL)
            {
                TiXmlElement* widgetPropertyChildElement = widgetPropertyChildNode->FirstChildElement();

                while(widgetPropertyChildElement != NULL)
                {
                    CString currentValue(widgetPropertyChildElement->Attribute("value"));

                    if(currentValue.Find(TEXT("HH_")) != -1)
                    {
                        if(currentValue.Find('\\') != -1)
                        {
                            CString fileName = newDirectory;
                            fileName += currentValue.Mid(currentValue.ReverseFind('\\'));

                            widgetPropertyChildElement->SetAttribute("value", GetConstCharStarFromCString(fileName));
                        }
                    }

                    widgetPropertyChildElement = widgetPropertyChildElement->NextSiblingElement();
                }

                widgetPropertyChildNode = widgetPropertyChildNode->NextSibling();
            }
        }

        doc.SaveFile();
    }

    AfxGetApp()->EndWaitCursor();
    EndMakingChanges();
}

void BackupAndDisableTodayScreen()
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

void RestoreAndReEnableTodayScreen()
{
    if(g_bRestoreTodayScreenNeeded)
    {
        AfxGetApp()->BeginWaitCursor();

        RestoreTodayScreenItemsRegHive();
        RefreshTodayScreen();

        AfxGetApp()->EndWaitCursor();
    }
}

void SetInstallDirectory(CString installDirectory)
{
    g_installDirectory = installDirectory;
}

int SetActiveTheme(CString pathToTheme)
{
    int retVal = 0;    

    CString themeName = pathToTheme.Mid('\\');;
    themeName = themeName.Mid(0, themeName.Find('.')-1);

    CString msg;

    CString activeThemeDirectory = GetPathToM2DCActiveThemeDirectory();
    // unzip the files to the local theme directory
    if(FileExists(pathToTheme))
    {
        BeginMakingChanges();
        AfxGetApp()->BeginWaitCursor();

        HZIP hz = OpenZip(pathToTheme, 0);
        ZIPENTRY ze;

        // -1 gives overall information about the zipfile
        GetZipItem(hz,-1,&ze);

        CManilla2DConfigProgressDlg progDlg(AfxGetApp()->GetMainWnd());

        int numitems = ze.index;

        msg.Format(TEXT("Applying theme: %s"), themeName);

        progDlg.BeginTrackingProgress(msg, 0, numitems);

        for(int zi=0; ((zi<numitems) && (retVal == 0)); zi++)
        {
            ZIPENTRY ze;

            GetZipItem(hz, zi, &ze);            // fetch individual details

            CString filePathFromZip = ze.name;

            CString destString = activeThemeDirectory;
            destString += "\\";
            destString += filePathFromZip.Mid(filePathFromZip.ReverseFind('/')+1);

            msg.Format(TEXT("Applying theme: %s\nFile %d of %d\n%s"), themeName, zi, numitems, destString);

            retVal = progDlg.UpdateStatus(msg, zi);

            UnzipItem(hz, zi, destString);

            SetFileAttributes(destString, FILE_ATTRIBUTE_NORMAL);
        }

        CloseZip(hz);

        // Read specific values from the HTCHomeSettings.xml file in the ActiveThemeDirectory
        // write the values to the working xml file
        CString pathToThemeXml = GetPathToM2DCActiveThemeDirectory();
        pathToThemeXml += "\\HTCHomeSettings.xml";

        HTCHomeSettingsStruct xmlSettings;

        ReadClockValuesFromXml(pathToThemeXml, &xmlSettings);
        WriteClockValuesToXml(GetPathToHTCHomeSettingsXmlFileWorking(), &xmlSettings);

        AfxGetApp()->EndWaitCursor();
        EndMakingChanges();
    }

    return retVal;
}

CString GetFileBaseName(CString filePath)
{
    CString retVal = filePath;

    retVal = retVal.Mid(retVal.ReverseFind('\\'));
    retVal = retVal.Mid(0, retVal.Find('.'));

    return retVal;
}

void GetNamesOfInstalledThemes(std::vector<CString>* pThemeNameVector)
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

void ReadClockValuesFromXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings)
{
    if((xmlSettings != NULL) && (FileExists(xmlFilePath)))
    {
        TiXmlDocument doc(GetConstCharStarFromCString(xmlFilePath));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            TiXmlHandle docHandle(&doc);

            TiXmlElement* imageListElement = docHandle.FirstChild("HTCHome").FirstChild("ImageList").Element();
            if(imageListElement != NULL)
            {
                TiXmlElement* imageListItemElement = imageListElement->FirstChildElement();

                while(imageListItemElement != NULL)
                {
                    int currentIndex = -1;
                    imageListItemElement->QueryIntAttribute("index", &currentIndex);

                    if(((currentIndex >= 40) && (currentIndex <= 57)) ||
                        ((currentIndex >= 121) && (currentIndex <= 128)) ||
                        ((currentIndex >= 209) && (currentIndex <= 216)))
                    {
                        CString nameStr(imageListItemElement->Attribute("name"));

                        ImageListImage ili;
                        ili.index = currentIndex;
                        ili.name = nameStr;

                        xmlSettings->imageListImages.push_back(ili);
                    }

                    imageListItemElement = imageListItemElement->NextSiblingElement();
                }
            }

            TiXmlElement* homeWidgetElement = docHandle.FirstChild("HTCHome").FirstChild("WidgetProperty").FirstChild("HomeWidget").Element();
            if(homeWidgetElement != NULL)
            {
                TiXmlElement* homeWidgetPropertyElement = homeWidgetElement->FirstChildElement();

                while(homeWidgetPropertyElement != NULL)
                {
                    int currentId = -1;
                    homeWidgetPropertyElement->QueryIntAttribute("id", &currentId);

                    CString valueStr(homeWidgetPropertyElement->Attribute("value"));

                    WidgetProperty homeWidgetProp;
                    homeWidgetProp.id = currentId;
                    homeWidgetProp.value = valueStr;

                    xmlSettings->homeWidgetProperties.push_back(homeWidgetProp);

                    homeWidgetPropertyElement = homeWidgetPropertyElement->NextSiblingElement();
                }
            }

            TiXmlElement* tabWidgetElement = docHandle.FirstChild("HTCHome").FirstChild("WidgetProperty").FirstChild("TabWidget").Element();
            if(tabWidgetElement != NULL)
            {
                TiXmlElement* tabWidgetPropertyElement = tabWidgetElement->FirstChildElement();

                while(tabWidgetPropertyElement != NULL)
                {
                    int currentId = -1;
                    tabWidgetPropertyElement->QueryIntAttribute("id", &currentId);

                    CString valueStr(tabWidgetPropertyElement->Attribute("value"));

                    WidgetProperty tabWidgetProp;
                    tabWidgetProp.id = currentId;
                    tabWidgetProp.value = valueStr;

                    xmlSettings->tabWidgetProperties.push_back(tabWidgetProp);

                    tabWidgetPropertyElement = tabWidgetPropertyElement->NextSiblingElement();
                }
            }
        }
    }
}

void WriteClockValuesToXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings)
{
    if((xmlSettings != NULL) && (FileExists(xmlFilePath)))
    {
        TiXmlDocument doc(GetConstCharStarFromCString(xmlFilePath));
        bool loadOkay = doc.LoadFile();

        if(loadOkay)
        {
            TiXmlHandle docHandle(&doc);

            TiXmlElement* imageListElement = docHandle.FirstChild("HTCHome").FirstChild("ImageList").Element();
            if(imageListElement != NULL)
            {
                TiXmlElement* imageListItemElement = imageListElement->FirstChildElement();

                while(imageListItemElement != NULL)
                {
                    int currentIndex = -1;
                    imageListItemElement->QueryIntAttribute("index", &currentIndex);

                    for(size_t i=0; i<xmlSettings->imageListImages.size(); i++)
                    {
                        if(xmlSettings->imageListImages[i].index == currentIndex)
                        {
                            imageListItemElement->SetAttribute("name",
                                GetConstCharStarFromCString(xmlSettings->imageListImages[i].name));
                            break;
                        }
                    }

                    imageListItemElement = imageListItemElement->NextSiblingElement();
                }
            }

            TiXmlElement* homeWidgetElement = docHandle.FirstChild("HTCHome").FirstChild("WidgetProperty").FirstChild("HomeWidget").Element();
            if(homeWidgetElement != NULL)
            {
                TiXmlElement* homeWidgetPropertyElement = homeWidgetElement->FirstChildElement();

                while(homeWidgetPropertyElement != NULL)
                {
                    int currentId = -1;
                    homeWidgetPropertyElement->QueryIntAttribute("id", &currentId);

                    for(size_t i=0; i<xmlSettings->homeWidgetProperties.size(); i++)
                    {
                        if(xmlSettings->homeWidgetProperties[i].id == currentId)
                        {
                            homeWidgetPropertyElement->SetAttribute("value",
                                GetConstCharStarFromCString(xmlSettings->homeWidgetProperties[i].value));
                            break;
                        }
                    }

                    homeWidgetPropertyElement = homeWidgetPropertyElement->NextSiblingElement();
                }
            }

            TiXmlElement* tabWidgetElement = docHandle.FirstChild("HTCHome").FirstChild("WidgetProperty").FirstChild("TabWidget").Element();
            if(tabWidgetElement != NULL)
            {
                TiXmlElement* tabWidgetPropertyElement = tabWidgetElement->FirstChildElement();

                while(tabWidgetPropertyElement != NULL)
                {
                    int currentId = -1;
                    tabWidgetPropertyElement->QueryIntAttribute("id", &currentId);

                    for(size_t i=0; i<xmlSettings->tabWidgetProperties.size(); i++)
                    {
                        if(xmlSettings->tabWidgetProperties[i].id == currentId)
                        {
                            tabWidgetPropertyElement->SetAttribute("value",
                                GetConstCharStarFromCString(xmlSettings->tabWidgetProperties[i].value));
                            break;
                        }
                    }

                    tabWidgetPropertyElement = tabWidgetPropertyElement->NextSiblingElement();
                }
            }

            doc.SaveFile();
        }
    }
}

bool ArchiveContainsHTCHomeSettingsXml(CString filePath)
{
    bool retVal = false;

    HZIP hz = OpenZip(filePath, 0);
    ZIPENTRY ze;

    // -1 gives overall information about the zipfile
    GetZipItem(hz,-1,&ze);

    CManilla2DConfigProgressDlg progDlg(AfxGetApp()->GetMainWnd());

    int numitems = ze.index;

    for(int zi=0; ((zi<numitems) && (!retVal)); zi++)
    {
        ZIPENTRY ze;
        GetZipItem(hz, zi, &ze);            // fetch individual details

        CString filePathFromZip(ze.name);
        retVal = (filePathFromZip.Find(TEXT("HTCHomeSettings.xml")) != -1);
    }

    CloseZip(hz);

    return retVal;
}