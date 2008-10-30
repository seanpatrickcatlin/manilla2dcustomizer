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

#include "TinyXml.h"
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

struct HomeWidgetSettings
{
    bool bAnalogClockEnabled;
    bool bDigitalClockEnabled;
    bool bCalendarEnabled;
    bool bMissedCallsEnabled;
    bool bAlarmStatusEnabled;
    bool bLine1Enabled;
    bool bLine2Enabled;
    bool bLine3Enabled;
};

struct HTCHomeSettingsStruct
{
    std::vector<TiXmlElement> homeWidgetPropertyElements;
    std::vector<TiXmlElement> tabWidgetPropertyElements;
    std::vector<TiXmlElement> peopleWidgetPropertyElements;
    std::vector<TiXmlElement> myfavesWidgetPropertyElements;
    std::vector<TiXmlElement> weatherWidgetPropertyElements;
    std::vector<TiXmlElement> launcherWidgetPropertyElements;
    std::vector<TiXmlElement> operatorWidgetPropertyElements;
    std::vector<TiXmlElement> musicWidgetPropertyElements;
    std::vector<TiXmlElement> photoWidgetPropertyElements;
    std::vector<TiXmlElement> messagingWidgetPropertyElements;
    std::vector<TiXmlElement> emailWidgetPropertyElements;
    std::vector<TiXmlElement> settingsWidgetPropertyElements;
    std::vector<TiXmlElement> internetWidgetPropertyElements;
    std::vector<TiXmlElement> locationWidgetPropertyElements;
};

namespace M2DC
{
    void SetAllowPopupDialogs(bool allow);

    void PrintNameAndEnabledStateVector(NameAndEnabledState_vector_t nameAndStateVector);
    bool CompareNameAndEnabledStateVectors(NameAndEnabledState_vector_t* vec1, NameAndEnabledState_vector_t* vec2);

    void BackupAndDisableTodayScreen();

    void BackupHTCHomeSettingsXml(bool overwritePreviousBackup = false);
    void RestoreHTCHomeSettingsXmlFromBackup();
    void BackupActiveTheme(bool overwritePreviousBackup = false);
    void RestoreActiveThemeFromBackup();

    void RestoreAndReEnableTodayScreen();

    void RefreshTodayScreen();
    void DisableAllTodayScreenItems();

    CString GetWin32ErrorString(DWORD err);

    const char* GetConstCharStarFromCString(CString str);

    CString GetPathToM2DCInstallDirectory();
    CString GetPathToM2DCThemesDirectory();

    CString GetPathToThemeBackupFile();
    CString GetPathToHTCHomeSettingsXmlFileActual();
    CString GetPathToHTCHomeSettingsXmlFileBackup();
    CString GetPathToHTCHomeSettingsXmlFileWorking();
    CString GetPathToHTCHomeSettingsXmlFileActiveTheme();

    void BackupTodayScreenItemsRegHive();
    void RestoreTodayScreenItemsRegHive();

    void BeginMakingChanges();
    void EndMakingChanges();

    void GetVectorOfThemeFilesCurrentlyInUse(std::vector<CString>* pPathVector, bool includeNonXmlFiles);
    void GetVectorOfThemeFilesFromHTCHomeSettingsXml(std::vector<CString>* pPathVector, CString xmlFilePath);
    void UpdateHTCHomeSettingsXmlWithActualFilePaths(std::vector<CString>* pPathVector, CString xmlFIlePath);

    void SetInstallDirectory(CString installDirectory);

    void ReadValuesFromXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings);
    void WriteValuesToXml(CString xmlFilePath, HTCHomeSettingsStruct* xmlSettings);

    void GetHomeWidgetSettings(CString pathToXmlFile, HomeWidgetSettings* pHomeWidgetSettings);
    void SetHomeWidgetSettings(CString pathToXmlFile, HomeWidgetSettings* pHomeWidgetSettings);

    void GetVectorOfWidgetPropertyRectPosElements(CString xmlFilePath, CString nodeName, std::vector<TiXmlElement>* pElementVector);

    CString GetPathToNullThemePreview();
    CString GetPathToM2DCThemeListXml();
    
    void GetNamesOfAvailableM2DCThemes(std::vector<CString>* pThemeNameVector);
    CString GetPathOfM2DCThemeFromName(CString themeName);
    CString GetPathOfM2DCThemePreviewFromName(CString themeName);
    bool FileIsValidM2DCTheme(CString filePath);

    int SetActiveTheme(CString themeName);
    int SetActiveThemeFromPath(CString themePath, CString themeName);

    void AddToM2DCThemeList(CString pathToTheme);
    void RemoveFromM2DCThemeList(CString themeName);

    void ReadLauncherValuesFromXml(int &numberOfColumns, int &numberOfRows);
    void WriteLauncherValuesToXml(int numberOfColumns, int numberOfRows);

    void ReadTabValuesFromXml(NameAndEnabledState_vector_t* tabValues);
    void WriteTabValuesToXml(NameAndEnabledState_vector_t* tabValues);

    void SetNewTskTheme(CString pathToTskTheme);
};