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

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigUtils.h"
#include "Manilla2DConfigLauncherDlg.h"

#include "tinyxml.h"
#include "tinystr.h"

// CManilla2DConfigTabsDlg dialog

CManilla2DConfigLauncherDlg::CManilla2DConfigLauncherDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CManilla2DConfigLauncherDlg::IDD, IDS_M2DC_LAUNCHER)
{
    m_initialNumberOfColumns = GetNumberOfLauncherColumnsFromHTCHomeSettingsXml();
}

CManilla2DConfigLauncherDlg::~CManilla2DConfigLauncherDlg()
{
}

void CManilla2DConfigLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_LAUNCHER_THREE_COL_RADIO, m_launcherThreeColumnRadioButton);
    DDX_Control(pDX, IDC_LAUNCHER_FOUR_COL_RADIO, m_launcherFourColumnRadioButton);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigLauncherDlg, CPropertyPage)
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigLauncherDlg::OnQuerySiblings)
END_MESSAGE_MAP()

BOOL CManilla2DConfigLauncherDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

    if(m_initialNumberOfColumns == 3)
    {
        m_launcherThreeColumnRadioButton.SetCheck(BST_CHECKED);
        m_launcherFourColumnRadioButton.SetCheck(BST_UNCHECKED);
    }
    else if(m_initialNumberOfColumns == 4)
    {
        m_launcherThreeColumnRadioButton.SetCheck(BST_UNCHECKED);
        m_launcherFourColumnRadioButton.SetCheck(BST_CHECKED);
    }
    else
    {
        m_launcherThreeColumnRadioButton.SetCheck(BST_UNCHECKED);
        m_launcherFourColumnRadioButton.SetCheck(BST_UNCHECKED);
        m_launcherThreeColumnRadioButton.EnableWindow(FALSE);
        m_launcherFourColumnRadioButton.EnableWindow(FALSE);
    }

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_APPLY_CANCEL_MENU);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigLauncherDlg::OnOK()
{
    int currentColumnCount = -1;

    if(m_launcherThreeColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 3;
    }
    else if(m_launcherFourColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 4;
    }

    if(m_initialNumberOfColumns != currentColumnCount)
    {
        BeginMakingChanges();
        SetNumberOfLauncherColumnsFromHTCHomeSettingsXml(currentColumnCount);
    }
}

LRESULT CManilla2DConfigLauncherDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    if(wParam == ID_RESTORE_DEFAULTS)
    {
        BeginMakingChanges();
        SetNumberOfLauncherColumnsFromHTCHomeSettingsXml(3);
    }

    return 0;
}

int CManilla2DConfigLauncherDlg::GetNumberOfLauncherColumnsFromHTCHomeSettingsXml()
{
    int retVal = -1;

    std::string launcherColStr = "IDLAUNCHERWG_COLUMN";

    CT2CA pszConvertedAnsiString(GetPathToActualHTCHomeSettingsXmlFile());

    std::string temp(pszConvertedAnsiString);

    TiXmlDocument doc(temp.c_str());
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlNode* htcHomeNode = NULL;

        htcHomeNode = doc.FirstChild("HTCHome");

        if(htcHomeNode != NULL)
        {
            TiXmlNode* wpNode = htcHomeNode->FirstChild("WidgetProperty");

            if(wpNode != NULL)
            {
                TiXmlNode* lwNode = wpNode->FirstChild("LauncherWidget");

                if(lwNode != NULL)
                {
                    TiXmlElement* lwElement = lwNode->ToElement();

                    if(lwElement != NULL)
                    {
                        TiXmlElement* lwItemElement = lwElement->FirstChildElement();

                        while(lwItemElement != NULL)
                        {
                            std::string currentPropertyName = lwItemElement->Attribute("name");

                            if(currentPropertyName.compare(launcherColStr) == 0)
                            {
                                lwItemElement->QueryIntAttribute("value", &retVal);
                                break;
                            }

                            lwItemElement = lwItemElement->NextSiblingElement();
                        }
                    }
                }
            }
        }
    }

    return retVal;
}

void CManilla2DConfigLauncherDlg::SetNumberOfLauncherColumnsFromHTCHomeSettingsXml(int numberOfColumns)
{
    if((numberOfColumns != 3) && (numberOfColumns != 4))
    {
        AfxMessageBox(TEXT("ERROR numcol != 3 or 4"));
        return;
    }

    std::string launcherColStr = "IDLAUNCHERWG_COLUMN";
    std::string launcherStartPointStr = "IDLAUNCHERWG_START_POINT";

    std::string colStr = "3";
    std::string startPoint = "20, 2";

    if(numberOfColumns == 4)
    {
        colStr = "4";
        startPoint = "5, 2";
    }

	CString filePath = GetPathToActualHTCHomeSettingsXmlFile();

    DWORD dwAttributes = GetFileAttributes(filePath);

    if(dwAttributes == 0xFFFFFFFF)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR WriteNew-1L\nUnable to append to ");
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

    if(SetFileAttributes(filePath, FILE_ATTRIBUTE_NORMAL) == 0)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
            CString msg("ERROR WriteNew-2L\nUnable to append to ");
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

    CT2CA pszConvertedAnsiString(filePath);
    std::string temp(pszConvertedAnsiString);

    TiXmlDocument doc(temp.c_str());
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        TiXmlNode* htcHomeNode = NULL;

        htcHomeNode = doc.FirstChild("HTCHome");

        if(htcHomeNode != NULL)
        {
            TiXmlNode* wpNode = htcHomeNode->FirstChild("WidgetProperty");

            if(wpNode != NULL)
            {
                TiXmlNode* lwNode = wpNode->FirstChild("LauncherWidget");

                if(lwNode != NULL)
                {
                    TiXmlElement* lwElement = lwNode->ToElement();

                    if(lwElement != NULL)
                    {
                        TiXmlElement* lwItemElement = lwElement->FirstChildElement();

                        while(lwItemElement != NULL)
                        {
                            std::string currentPropertyName = lwItemElement->Attribute("name");

                            if(currentPropertyName.compare(launcherColStr) == 0)
                            {
                                lwItemElement->SetAttribute("value", colStr.c_str());
                            }

                            if(currentPropertyName.compare(launcherStartPointStr) == 0)
                            {
                                lwItemElement->SetAttribute("value", startPoint.c_str());
                            }

                            lwItemElement = lwItemElement->NextSiblingElement();
                        }
                    }
                }
            }
        }

        doc.SaveFile();
    }

    if(SetFileAttributes(GetPathToActualHTCHomeSettingsXmlFile(), dwAttributes) == 0)
    {
		FILE* errorDump = fopen(GetConstCharStarFromCString(GetPathToErrorLogFile()), "a");

		if(errorDump == NULL)
		{
			CString msg("ERROR WriteNew-3L\nUnable to append to ");
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
