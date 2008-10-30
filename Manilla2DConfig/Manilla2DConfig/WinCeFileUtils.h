#pragma once

#include "stdafx.h"

namespace WinCeFileUtils
{
    CString GetPathToRunningBinary();
    CString GetPathToWindowsDirectory();

    bool IsDirEmpty(CString dirPath);
    bool IsDir(CString pathToDir);
    bool FileExists(CString pathToFile);

    void RecursivelyDeleteDirectory(CString sDirPath);

    CString GetFileDirNoNameNoExt(CString filePath);
    CString GetFileNameNoDirNoExt(CString filePath);
    CString GetFileExtNoDirNoName(CString filePath);
    CString GetFileNameNoDirWithExt(CString filePath);
};