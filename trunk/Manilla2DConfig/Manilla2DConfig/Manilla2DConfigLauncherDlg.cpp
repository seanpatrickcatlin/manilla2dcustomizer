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
	: CPropertyPage(CManilla2DConfigLauncherDlg::IDD, IDS_M2DC_LAUNCHER_STR)
{
    ReadValuesFromXml();
}

CManilla2DConfigLauncherDlg::~CManilla2DConfigLauncherDlg()
{
}

void CManilla2DConfigLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_M2DC_LAUNCHER_3_COL_RB, m_launcher3ColumnRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_4_COL_RB, m_launcher4ColumnRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_5_COL_RB, m_launcher5ColumnRadioButton);

    DDX_Control(pDX, IDC_M2DC_LAUNCHER_3_ROW_RB, m_launcher3RowRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_6_ROW_RB, m_launcher6RowRadioButton);
    DDX_Control(pDX, IDC_M2DC_LAUNCHER_9_ROW_RB, m_launcher9RowRadioButton);
}


BEGIN_MESSAGE_MAP(CManilla2DConfigLauncherDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_MESSAGE(PSM_QUERYSIBLINGS, CManilla2DConfigLauncherDlg::OnQuerySiblings)
END_MESSAGE_MAP()

BOOL CManilla2DConfigLauncherDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

    m_launcher3ColumnRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher4ColumnRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher5ColumnRadioButton.SetCheck(BST_UNCHECKED);

    if(m_numberOfColumns == 3)
    {
        m_launcher3ColumnRadioButton.SetCheck(BST_CHECKED);
    }
    else if(m_numberOfColumns == 4)
    {
        m_launcher4ColumnRadioButton.SetCheck(BST_CHECKED);
    }
    else if(m_numberOfColumns == 5)
    {
        m_launcher5ColumnRadioButton.SetCheck(BST_CHECKED);
    }
    else
    {
        m_launcher3ColumnRadioButton.EnableWindow(FALSE);
        m_launcher4ColumnRadioButton.EnableWindow(FALSE);
        m_launcher5ColumnRadioButton.EnableWindow(FALSE);
    }

    m_launcher3RowRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher6RowRadioButton.SetCheck(BST_UNCHECKED);
    m_launcher9RowRadioButton.SetCheck(BST_UNCHECKED);

    if(m_numberOfRows == 3)
    {
        m_launcher3RowRadioButton.SetCheck(BST_CHECKED);
    }
    else if(m_numberOfRows == 6)
    {
        m_launcher6RowRadioButton.SetCheck(BST_CHECKED);
    }
    else if(m_numberOfRows == 9)
    {
        m_launcher9RowRadioButton.SetCheck(BST_CHECKED);
    }
    else
    {
        m_launcher3RowRadioButton.EnableWindow(FALSE);
        m_launcher6RowRadioButton.EnableWindow(FALSE);
        m_launcher9RowRadioButton.EnableWindow(FALSE);
    }

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CManilla2DConfigLauncherDlg::OnPaint()
{
    CPaintDC dc(this);

    int nWidth = dc.GetDeviceCaps(HORZRES);
    const int nHeaderHeight = 24;

    // paint title
    CFont *pCurrentFont = dc.GetCurrentFont();
    LOGFONT lf;
    pCurrentFont->GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;

    CFont newFont;
    newFont.CreateFontIndirect(&lf);
    CFont *pSave = dc.SelectObject(&newFont);
    dc.SetTextColor(RGB(0, 0, 156));
    dc.DrawText(TEXT("Launcher Settings"), CRect(8, 0, nWidth, nHeaderHeight), DT_VCENTER | DT_SINGLELINE); dc.SelectObject(pSave);

    // paint line
    CPen blackPen(PS_SOLID, 1, RGB(0,0,0));
    CPen *pOldPen = dc.SelectObject(&blackPen);

    dc.MoveTo(0, nHeaderHeight);
    dc.LineTo(nWidth, nHeaderHeight);

    dc.SelectObject(pOldPen); 
}

LRESULT CManilla2DConfigLauncherDlg::OnQuerySiblings(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CManilla2DConfigLauncherDlg::OnOK()
{
    int currentColumnCount = -1;
    if(m_launcher3ColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 3;
    }
    else if(m_launcher4ColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 4;
    }
    else if(m_launcher5ColumnRadioButton.GetCheck() == BST_CHECKED)
    {
        currentColumnCount = 5;
    }

    int currentRowCount = -1;
    if(m_launcher3RowRadioButton.GetCheck() == BST_CHECKED)
    {
        currentRowCount = 3;
    }
    else if(m_launcher6RowRadioButton.GetCheck() == BST_CHECKED)
    {
        currentRowCount = 6;
    }
    else if(m_launcher9RowRadioButton.GetCheck() == BST_CHECKED)
    {
        currentRowCount = 9;
    }

    if((m_numberOfColumns != currentColumnCount) || (m_numberOfRows != currentRowCount))
    {
        m_numberOfRows = currentRowCount;
        m_numberOfColumns = currentColumnCount;

        M2DC::BeginMakingChanges();
        WriteValuesToXml();
    }
}

void CManilla2DConfigLauncherDlg::ReadValuesFromXml()
{
    m_numberOfRows = -1;
    m_numberOfColumns = -1;
    
    std::string launcherRowStr = "IDLAUNCHERWG_ROW";
    std::string launcherColStr = "IDLAUNCHERWG_COLUMN";

    TiXmlDocument doc(M2DC::GetConstCharStarFromCString(M2DC::GetPathToHTCHomeSettingsXmlFileActual()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
            htcHomeNode != NULL;
            htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
        {
            for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                widgetPropertyNode != NULL;
                widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
            {
                for(TiXmlNode* launcherWidgetNode = widgetPropertyNode->FirstChild("LauncherWidget");
                    launcherWidgetNode != NULL;
                    launcherWidgetNode = launcherWidgetNode->NextSibling("LauncherWidget"))
                {
                    for(TiXmlElement* launcherWidgetChildElement = launcherWidgetNode->FirstChildElement();
                        launcherWidgetChildElement != NULL;
                        launcherWidgetChildElement = launcherWidgetChildElement->NextSiblingElement())
                    {
                        std::string currentPropertyName = launcherWidgetChildElement->Attribute("name");

                        if(currentPropertyName.compare(launcherColStr) == 0)
                        {
                            launcherWidgetChildElement->QueryIntAttribute("value", &m_numberOfColumns);
                        }
                        else if(currentPropertyName.compare(launcherRowStr) == 0)
                        {
                            launcherWidgetChildElement->QueryIntAttribute("value", &m_numberOfRows);
                        }
                    }
                }
            }
        }
    }
}

void CManilla2DConfigLauncherDlg::WriteValuesToXml()
{
    if((m_numberOfColumns < 3) || (m_numberOfColumns > 5))
    {
        AfxMessageBox(TEXT("ERROR numcol != 3, 4, or 5"));
        return;
    }

    if((m_numberOfRows%3 != 0) || (m_numberOfColumns < 3) || (m_numberOfColumns > 9))
    {
        AfxMessageBox(TEXT("ERROR numrow != 3, 6, or 9"));
        return;
    }

    std::string launcherRowStr = "IDLAUNCHERWG_ROW";
    std::string launcherColStr = "IDLAUNCHERWG_COLUMN";
    std::string launcherStartPointStr = "IDLAUNCHERWG_START_POINT";
    std::string launcherYIntervalStr = "IDLAUNCHERWG_Y_INTERVAL";

    char intVal[2];
    intVal[0] = '0' + m_numberOfRows;
    intVal[1] = '\0';
    std::string rowStr = intVal;

    intVal[0] = '0' + m_numberOfColumns;
    intVal[1] = '\0';
    std::string colStr = intVal;

    std::string startPoint = "20, 2";
    if(m_numberOfColumns == 4)
    {
        startPoint = "5, 2";
    }
    else if(m_numberOfColumns == 5)
    {
        startPoint = "0, 0";
    }

    std::string yIntStr = "2";
    if(m_numberOfColumns == 5)
    {
        yIntStr = "0";
    }

    TiXmlDocument doc(M2DC::GetConstCharStarFromCString(M2DC::GetPathToHTCHomeSettingsXmlFileWorking()));
    bool loadOkay = doc.LoadFile();

    if(loadOkay)
    {
        for(TiXmlNode* htcHomeNode = doc.FirstChild("HTCHome");
            htcHomeNode != NULL;
            htcHomeNode = htcHomeNode->NextSibling("HTCHome"))
        {
            for(TiXmlNode* widgetPropertyNode = htcHomeNode->FirstChild("WidgetProperty");
                widgetPropertyNode != NULL;
                widgetPropertyNode = widgetPropertyNode->NextSibling("WidgetProperty"))
            {
                for(TiXmlNode* launcherWidgetNode = widgetPropertyNode->FirstChild("LauncherWidget");
                    launcherWidgetNode != NULL;
                    launcherWidgetNode = launcherWidgetNode->NextSibling("LauncherWidget"))
                {
                    for(TiXmlElement* launcherWidgetChildElement = launcherWidgetNode->FirstChildElement();
                        launcherWidgetChildElement != NULL;
                        launcherWidgetChildElement = launcherWidgetChildElement->NextSiblingElement())
                    {
                        std::string currentPropertyName = launcherWidgetChildElement->Attribute("name");

                        if(currentPropertyName.compare(launcherColStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", colStr.c_str());
                        }
                        else if(currentPropertyName.compare(launcherStartPointStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", startPoint.c_str());
                        }
                        else if(currentPropertyName.compare(launcherRowStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", rowStr.c_str());
                        }
                        else if(currentPropertyName.compare(launcherYIntervalStr) == 0)
                        {
                            launcherWidgetChildElement->SetAttribute("value", yIntStr.c_str());
                        }
                    }
                }
            }
        }

        doc.SaveFile();
    }
}
