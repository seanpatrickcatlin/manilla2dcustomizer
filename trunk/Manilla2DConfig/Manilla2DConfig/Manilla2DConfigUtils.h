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

struct ImageListImage
{
    int index;
    CString name;
};

struct HomeWidgetProperty
{
    int id;
    CString value;
};

struct HTCHomeSettingsClockSettings
{
    std::vector<HomeWidgetProperty> homeWidgetProperties;
    std::vector<ImageListImage> imageListImages;

};

void PrintNameAndEnabledStateVector(NameAndEnabledState_vector_t nameAndStateVector);
bool CompareNameAndEnabledStateVectors(NameAndEnabledState_vector_t* vec1, NameAndEnabledState_vector_t* vec2);

void BackupM2DCFiles();
void BackupAndDisableTodayScreen();
int BackupActualTheme(bool overwritePreviousBackup = false);
void BackupHTCHomeSettingsXml(bool overwritePreviousBackup = false);

void RestoreAndReEnableTodayScreen();
void RestoreM2DCFiles(bool showProgress = true);

void RefreshTodayScreen();
void DisableAllTodayScreenItems();

CString GetWin32ErrorString(DWORD err);

CString GetFileBaseName(CString filePath);
bool FileExists(CString pathToFile);
bool IsDirEmpty(CString dirPath);
void RecursivelyDeleteDirectory(CString sDirPath);

const char* GetConstCharStarFromCString(CString str);

CString GetPathToRunningBinary();
CString GetDirectoryOfFile(CString fullFilePath);

CString GetPathToWindowsDirectory();

CString GetPathToM2DCInstallDirectory();
CString GetPathToM2DCThemesDirectory();
CString GetPathToM2DCActiveThemeDirectory();

CString GetPathToThemeBackupFile();
CString GetPathToHTCHomeSettingsXmlFileActual();
CString GetPathToHTCHomeSettingsXmlFileBackup();
CString GetPathToHTCHomeSettingsXmlFileWorking();

void BackupTodayScreenItemsRegHive();
void RestoreTodayScreenItemsRegHive();

void BeginMakingChanges();
void EndMakingChanges();

bool IsM2DCThemeSupportEnabled();
int EnableM2DCThemeSupport();

void GetVectorOfThemeFilesCurrentlyInUse(std::vector<CString>* pPathVector, bool includeNonXmlFiles);
void SetThemeDirectoryInActualXmlSettingsFile(CString newDirectory);

void SetInstallDirectory(CString installDirectory);

int SetActiveTheme(CString pathToTheme);

void GetNamesOfInstalledThemes(std::vector<CString>* pThemeNameVector);

void ReadClockValuesFromXml(CString xmlFilePath, HTCHomeSettingsClockSettings* clockSettings);
void WriteClockValuesToXml(CString xmlFilePath, HTCHomeSettingsClockSettings* clockSettings);