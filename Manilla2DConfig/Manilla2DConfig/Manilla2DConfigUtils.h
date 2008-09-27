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

void BackupM2DCFiles();
void BackupAndDisableTodayScreen();
void BackupHH_Files(bool overwritePreviousBackup = false);
void BackupHTCHomeSettingsXml(bool overwritePreviousBackup = false);

void RestoreAndReEnableTodayScreen();
void RestoreM2DCFiles();

void RefreshTodayScreen();
void DisableAllTodayScreenItems();

CString GetWin32ErrorString(DWORD err);

bool FileExists(CString pathToFile);
bool IsDirEmpty(CString dirPath);
void RecursivelyDeleteDirectory(CString sDirPath);

const char* GetConstCharStarFromCString(CString str);

CString GetDirectoryOfFile(CString fullFilePath);




CString GetPathToWindowsDirectory();


CString GetPathToM2DCInstallDirectory();
CString GetPathToCurrentThemeDirectory();
CString GetPathToRunningBinary();

CString GetPathToHTCHomeSettingsXmlFileActual();
CString GetPathToHTCHomeSettingsXmlFileBackup();
CString GetPathToHTCHomeSettingsXmlFileWorking();

CString GetPathToHH_ZipFileBackup();

void BackupTodayScreenItemsRegHive();
void RestoreTodayScreenItemsRegHive();

void BeginMakingChanges();
void EndMakingChanges();

bool IsM2DCThemeSupportEnabled();
bool EnableM2DCThemeSupport();

void GetVectorOfHH_FilesCurrentlyInUse(std::vector<CString>* pPathVector);
void SetHH_FileDirectoryInXmlSettingsFile(CString newDirectory);

void SetInstallDirectory(CString installDirectory);
