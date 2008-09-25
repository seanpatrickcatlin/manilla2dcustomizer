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

#include "zip.h"
#include "unzip.h"

#include <stdio.h>
#include <time.h>

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

CString GetPathToActualHTCHomeSettingsXmlFile()
{
	CString retVal = GetPathToWindowsDirectory();
    retVal += "\\HTCHomeSettings.xml";

	TRACE(TEXT("GetPathToActualHTCHomeSettingsXmlFile "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

    return retVal;
}

CString GetPathToHTCHomeSettingsXmlBackup()
{
	CString retVal = GetPathToM2DCInstallDirectory();
	retVal += "\\HTCHomeSettings-BACKUP.xml";

	TRACE(TEXT("GetPathToHTCHomeSettingsXmlBackup "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

    return retVal;
}

CString GetPathToHH_FilesZipBackup()
{
	CString retVal = GetPathToM2DCInstallDirectory();
	retVal += "\\HH_Files-BACKUP.zip";

	TRACE(TEXT("GetPathToHH_FilesZipBackup "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

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
	CString retVal = GetDirectoryOfFile(GetPathToRunningBinary());

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

    TRACE(TEXT("GetPathToM2DCThemesDirectory "));
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

CString GetPathToErrorLogFile()
{
	CString retVal = GetPathToM2DCInstallDirectory();
	retVal += "\\ErrorLog.txt";

	TRACE(TEXT("GetPathToErrorLogFile "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

	return retVal;
}

CString GetPathToM2DCDataDirectory()
{
     CString retVal = GetPathToAppDataDirectory();
     retVal += "\\M2DC";

     return retVal;
}

CString GetPathToAppDataDirectory()
{
     CString retVal("\\AppData");

    TCHAR appDataDirStr[MAX_PATH];
	if(SHGetSpecialFolderPath(NULL, appDataDirStr, CSIDL_APPDATA, 0) == TRUE)
	{
		retVal = appDataDirStr;
	}

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
    CWaitCursor wait;

    if(!FileExists(GetPathToHTCHomeSettingsXmlBackup()))
    {
        CopyFile(GetPathToActualHTCHomeSettingsXmlFile(), GetPathToHTCHomeSettingsXmlBackup(), FALSE);
    }
    
    if(!FileExists(GetPathToHH_FilesZipBackup()))
    {
        TRACE(TEXT("Begin Zip HH_ files\n"));
        CString findString = GetPathToWindowsDirectory();
        findString += "\\HH_*";

        HZIP hz = CreateZip(GetPathToHH_FilesZipBackup(), 0);

        WIN32_FIND_DATA findData;
        HANDLE hFindHandle = FindFirstFile(findString, &findData);

        if(hFindHandle != INVALID_HANDLE_VALUE)
        {
            BOOL retVal = TRUE;

            while(retVal != FALSE)
            {
                if((lstrcmpi(findData.cFileName, TEXT(".")) != 0) && (lstrcmpi(findData.cFileName, TEXT("..")) != 0))
                {
                    CString fullFilePath = GetPathToWindowsDirectory();
                    fullFilePath += "\\";
                    fullFilePath += findData.cFileName;

                    TRACE(TEXT("Zip Add: "));
                    TRACE(fullFilePath);
                    TRACE(TEXT("\n"));

                    ZipAdd(hz, findData.cFileName, fullFilePath);
                }

                retVal = FindNextFile(hFindHandle, &findData);
            }
        }

        FindClose(hFindHandle);
        CloseZipZ(hz);
        TRACE(TEXT("End Zip HH_ files\n"));
    }
}

const char* GetConstCharStarFromCString(CString str)
{
	CT2CA pszConvertedAnsiString(str);
	std::string temp(pszConvertedAnsiString);

	return temp.c_str();
}

void LogFileAttributes(FILE* openFile, DWORD dwAttributes)
{
    if(openFile != NULL)
    {
        if(dwAttributes == 0xFFFFFFFF)
        {
            fprintf(openFile, "\tinvalid dwAttributes\n");
        }
        else
        {
            fprintf(openFile, "\tFILE_ATTRIBUTE_ARCHIVE %d\n", dwAttributes&FILE_ATTRIBUTE_ARCHIVE);
            fprintf(openFile, "\tFILE_ATTRIBUTE_COMPRESSED %d\n", dwAttributes&FILE_ATTRIBUTE_COMPRESSED);
            fprintf(openFile, "\tFILE_ATTRIBUTE_DIRECTORY %d\n", dwAttributes&FILE_ATTRIBUTE_DIRECTORY);
            fprintf(openFile, "\tFILE_ATTRIBUTE_ENCRYPTED %d\n", dwAttributes&FILE_ATTRIBUTE_ENCRYPTED);
            fprintf(openFile, "\tFILE_ATTRIBUTE_HIDDEN %d\n", dwAttributes&FILE_ATTRIBUTE_HIDDEN);
            fprintf(openFile, "\tFILE_ATTRIBUTE_INROM %d\n", dwAttributes&FILE_ATTRIBUTE_INROM);
            fprintf(openFile, "\tFILE_ATTRIBUTE_NORMAL %d\n", dwAttributes&FILE_ATTRIBUTE_NORMAL);
            fprintf(openFile, "\tFILE_ATTRIBUTE_READONLY %d\n", dwAttributes&FILE_ATTRIBUTE_READONLY);
            fprintf(openFile, "\tFILE_ATTRIBUTE_ROMMODULE %d\n", dwAttributes&FILE_ATTRIBUTE_ROMMODULE);
            fprintf(openFile, "\tFILE_ATTRIBUTE_ROMSTATICREF %d\n", dwAttributes&FILE_ATTRIBUTE_ROMSTATICREF);
            fprintf(openFile, "\tFILE_ATTRIBUTE_SYSTEM %d\n", dwAttributes&FILE_ATTRIBUTE_SYSTEM);
            fprintf(openFile, "\tFILE_ATTRIBUTE_TEMPORARY %d\n", dwAttributes&FILE_ATTRIBUTE_TEMPORARY);
        }
    }
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

void RestoreM2DCFiles()
{
    CWaitCursor wait;

    if(FileExists(GetPathToHTCHomeSettingsXmlBackup()))
    {
        CopyFile(GetPathToHTCHomeSettingsXmlBackup(), GetPathToActualHTCHomeSettingsXmlFile(), FALSE);
    }

    if(FileExists(GetPathToHH_FilesZipBackup()))
    {
        HZIP hz = OpenZip(GetPathToHH_FilesZipBackup(), 0);
        ZIPENTRY ze;

        // -1 gives overall information about the zipfile
        GetZipItem(hz,-1,&ze);

        int numitems = ze.index;
        
        for(int zi=0; zi<numitems; zi++)
        {
            ZIPENTRY ze;
            
            GetZipItem(hz, zi, &ze);            // fetch individual details

            CString targetPath = GetPathToWindowsDirectory();
            targetPath += "\\";
            targetPath += ze.name;

            UnzipItem(hz, zi, targetPath);
        }

        CloseZip(hz);
    }
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
        AfxGetApp()->DoWaitCursor(1);
        BackupTodayScreenItemsRegHive();
        DisableAllTodayScreenItems();
        RefreshTodayScreen();
    }
}

void EndMakingChanges()
{
    if(g_bAlreadyBeganMakingChanges)
    {
        AfxGetApp()->DoWaitCursor(1);
        RestoreTodayScreenItemsRegHive();
        RefreshTodayScreen();
        AfxGetApp()->DoWaitCursor(-1);
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

