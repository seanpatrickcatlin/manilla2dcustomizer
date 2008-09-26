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

#include "tinyxml.h"
#include "tinystr.h"

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

CString GetPathToWorkingHTCHomeSettingsXmlFile()
{
	CString retVal = GetPathToM2DCInstallDirectory();
    retVal += "\\HTCHomeSettings-WorkingCopy.xml";

	TRACE(TEXT("GetPathToWorkingHTCHomeSettingsXmlFile "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

    return retVal;
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
    BackupHTCHomeSettingsXml(false);
    BackupHH_Files(false);
}

void BackupHTCHomeSettingsXml(bool onlyIfNeeded)
{
    bool fileExists = FileExists(GetPathToHTCHomeSettingsXmlBackup());

    if(!(fileExists && onlyIfNeeded))
    {
        if(fileExists)
        {
            DeleteFile(GetPathToHTCHomeSettingsXmlBackup());
        }

        CopyFile(GetPathToActualHTCHomeSettingsXmlFile(), GetPathToHTCHomeSettingsXmlBackup(), FALSE);
    }
}

void BackupHH_Files(bool onlyIfNeeded)
{
    bool fileExists = FileExists(GetPathToHH_FilesZipBackup());

    if(!(fileExists && onlyIfNeeded))
    {
        if(fileExists)
        {
            DeleteFile(GetPathToHH_FilesZipBackup());
        }

        TRACE(TEXT("Begin Zip HH_ files\n"));

        HZIP hz = CreateZip(GetPathToHH_FilesZipBackup(), 0);

        CString htcHomeXml = GetPathToActualHTCHomeSettingsXmlFile();
        CString archiveXml = htcHomeXml;
        archiveXml.Replace(TEXT("\\"), TEXT("/"));

        ZipAdd(hz, archiveXml, htcHomeXml);

        std::vector<CString> hh_strVector;
        GetVectorOfHH_FilesCurrentlyInUse(&hh_strVector);

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

                for(size_t i=0; i<hh_strVector.size(); i++)
                {
                    if(hh_strVector[i].Compare(currentHH_file) == 0)
                    {
                        addFileToVector = false;
                        break;
                    }
                }

                if(addFileToVector)
                {
                    hh_strVector.push_back(currentHH_file);
                }

                keepSearching = FindNextFile(hFindHandle, &findData);
            }
        }

        FindClose(hFindHandle);

        for(size_t i=0; i<hh_strVector.size(); i++)
        {
            CString fullFilePath = hh_strVector[i];

            CString archiveFilePath = fullFilePath;
            archiveFilePath.Replace(TEXT("\\"), TEXT("/"));

            ZipAdd(hz, archiveFilePath, fullFilePath);
        }

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
            UnzipItem(hz, zi, ze.name);
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

        CopyFile(GetPathToActualHTCHomeSettingsXmlFile(), GetPathToWorkingHTCHomeSettingsXmlFile(), FALSE);
        SetFileAttributes(GetPathToWorkingHTCHomeSettingsXmlFile(), FILE_ATTRIBUTE_NORMAL);
    }
}

void EndMakingChanges()
{
    if(g_bAlreadyBeganMakingChanges)
    {
        CopyFile(GetPathToWorkingHTCHomeSettingsXmlFile(), GetPathToActualHTCHomeSettingsXmlFile(), FALSE);
        DeleteFile(GetPathToWorkingHTCHomeSettingsXmlFile());

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

void GetVectorOfHH_FilesCurrentlyInUse(std::vector<CString>* pPathVector)
{
    if(pPathVector != NULL)
    {
        CString basePath = GetPathToHH_ImageFilesFromActualXml();

        if(basePath[basePath.GetLength()-1] != '\\')
        {
            basePath += '\\';
        }

        TiXmlDocument doc(GetConstCharStarFromCString(GetPathToActualHTCHomeSettingsXmlFile()));
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
                    CString currentFilePath(imageListItemElement->Attribute("name"));

                    if(currentFilePath.Find(TEXT("HH_")) != -1)
                    {
                        if(currentFilePath.Find('\\') == -1)
                        {
                            currentFilePath = basePath + currentFilePath;

                            pPathVector->push_back(currentFilePath);
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
    }
}

CString GetPathToHH_ImageFilesFromActualXml()
{
    CString retVal;

    TiXmlDocument doc(GetConstCharStarFromCString(GetPathToActualHTCHomeSettingsXmlFile()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlHandle docHandle(&doc);

        TiXmlElement* imageListElement = docHandle.FirstChild("HTCHome").FirstChild("ImageList").Element();

        if(imageListElement != NULL)
        {
            {
                retVal = imageListElement->Attribute("path");
            }
        }
    }

    return retVal;
}

bool IsM2DCThemeSupportEnabled()
{
    // check to see if the Zip file exists

    // check to see if the local themes folder exists and is not empty

    // check to make sure that the xml file in \Windows references the local theme folder

    return false;
}

bool EnableM2DCThemeSupport()
{
    bool retVal = false;

    CString enableMessage = TEXT("Enabling M2DC theme support may take 3-5 minutes to complete.");
    enableMessage += "\nWould you like to continue?";

    if(MessageBox(NULL, enableMessage, TEXT("Enable M2DC Themes?"), MB_YESNO) == IDYES)
    {
        //BackupHH_Files(true);

        // Make a directory in the install folder to contain the new themeRe 

        // unzip the HH backup zip file to the new CurrentTheme folder

        // 
    }

    return retVal;
}
