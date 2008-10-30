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
#include "SolidColorButton.h"

CSolidColorButton::CSolidColorButton()
: CButton()
{
    m_red = m_green = m_blue = 0;
}


CSolidColorButton::~CSolidColorButton()
{

}

void CSolidColorButton::GetColorValues(int &red, int &green, int &blue)
{
    red = m_red;
    green = m_green;
    blue = m_blue;
}

void CSolidColorButton::SetColorValues(int red, int green, int blue)
{
    if(red < 0)
    {
        m_red = 0;
    }
    else if(red > 255)
    {
        m_red = 255;
    }
    else
    {
        m_red = red;
    }

    if(green < 0)
    {
        m_green = 0;
    }
    else if(green > 255)
    {
        m_green = 255;
    }
    else
    {
        m_green = green;
    }

    if(blue < 0)
    {
        m_blue = 0;
    }
    else if(blue > 255)
    {
        m_blue = 255;
    }
    else
    {
        m_blue = blue;
    }
}

BEGIN_MESSAGE_MAP(CSolidColorButton, CButton)
    
END_MESSAGE_MAP()

void CSolidColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CDC* pDC   = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect = lpDrawItemStruct->rcItem;
    UINT state = lpDrawItemStruct->itemState;

    CString strText;
    GetWindowText(strText);

    // draw the control edges (DrawFrameControl is handy!)

    if (state & ODS_SELECTED)
        pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED);
    else
        pDC->DrawFrameControl(rect, DFC_BUTTON, DFCS_BUTTONPUSH);

    // Deflate the drawing rect by the size of the button's edges

    rect.DeflateRect( CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));

    // Fill the interior color if necessary
    pDC->FillSolidRect(rect, RGB(m_red, m_green, m_blue)); // yellow
}
