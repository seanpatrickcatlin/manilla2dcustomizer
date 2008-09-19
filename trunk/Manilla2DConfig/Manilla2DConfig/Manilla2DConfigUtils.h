#pragma once

#include <vector>

using std::vector;

struct NameAndEnabledStateItem
{
    CString name;
    BOOL enabled;
};

typedef std::vector<NameAndEnabledStateItem> NameAndEnabledState_vector_t;

struct TodayScreenRegBackup
{
    BOOL dateEnabled;
    NameAndEnabledState_vector_t itemVector;
};

void PrintNameAndEnabledStateVector(NameAndEnabledState_vector_t nameAndStateVector);
bool CompareNameAndEnabledStateVectors(NameAndEnabledState_vector_t* vec1, NameAndEnabledState_vector_t* vec2);

void BackupHTCHomeSettingsXmlFileIfNeeded();
void RestoreBackupHTCHomeSettingsXmlFileToWindowsDir();

void RefreshTodayScreen();
void DisableAllTodayScreenItems();

CString GetWin32ErrorString(DWORD err);

bool FileExists(CString pathToFile);

void LogFileAttributes(FILE* openFile, DWORD dwAttributes);

const char* GetConstCharStarFromCString(CString str);

CString GetDirectoryOfFile(CString fullFilePath);
CString GetPathToErrorLogFile();
CString GetPathToWindowsDirectory();
CString GetPathToApplicationDirectory();
CString GetPathToRunningBinary();
CString GetPathToActualHTCHomeSettingsXmlFile();
CString GetPathToBackupHTCHomeSettingsXmlFile();
