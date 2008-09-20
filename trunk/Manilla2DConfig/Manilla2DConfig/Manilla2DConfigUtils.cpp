#include "StdAfx.h"
#include "Manilla2DConfigUtils.h"

#include <stdio.h>
#include <time.h>

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

CString GetPathToBackupHTCHomeSettingsXmlFile()
{
	CString retVal = GetPathToApplicationDirectory();
	retVal += "\\HTCHomeSettings-BACKUP.xml";

	TRACE(TEXT("GetPathToBackupHTCHomeSettingsXmlFile "));
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

CString GetPathToApplicationDirectory()
{
	CString retVal = GetDirectoryOfFile(GetPathToRunningBinary());

	TRACE(TEXT("GetPathToApplicationDirectory "));
	TRACE(retVal);
	TRACE(TEXT("\n"));

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
	CString retVal = GetPathToApplicationDirectory();
	retVal += "\\ErrorLog.txt";

	TRACE(TEXT("GetPathToErrorLogFile "));
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

void BackupHTCHomeSettingsXmlFileIfNeeded()
{
    if(!FileExists(GetPathToBackupHTCHomeSettingsXmlFile()))
    {
        CopyFile(GetPathToActualHTCHomeSettingsXmlFile(), GetPathToBackupHTCHomeSettingsXmlFile(), FALSE);
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

void RestoreBackupHTCHomeSettingsXmlFileToWindowsDir()
{
    DWORD dwAttributes = GetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile());

    if(dwAttributes == 0xFFFFFFFF)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR Backup-1\nUnable to append to ");
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

    if(SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), FILE_ATTRIBUTE_NORMAL) == 0)
	{
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR Backup-2\nUnable to append to ");
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

    CopyFile(GetPathToBackupHTCHomeSettingsXmlFile(), GetPathToActualHTCHomeSettingsXmlFile(), FALSE);

    if(SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), dwAttributes) == 0)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR Backup-3\nUnable to append to ");
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
