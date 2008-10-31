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
    m_red = m_green = m_blue = 192;
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

    Invalidate();
}

BEGIN_MESSAGE_MAP(CSolidColorButton, CButton)
END_MESSAGE_MAP()

void CSolidColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);     //Get device context object
    CRect rt;
    rt = lpDrawItemStruct->rcItem;        //Get button rect

    if(IsWindowEnabled() == TRUE)
    {
        dc.FillSolidRect(rt, RGB(m_red, m_green, m_blue));
    }
    else
    {
        dc.FillSolidRect(rt, RGB(192, 192, 192));

    }

    UINT state = lpDrawItemStruct->itemState; //Get state of the button
    if ( (state & ODS_SELECTED) )            // If it is pressed
    {
        dc.DrawEdge(rt,EDGE_SUNKEN,BF_RECT);    // Draw a sunken face
    }
    else
    {
        dc.DrawEdge(rt,EDGE_RAISED,BF_RECT);    // Draw a raised face
    }

    dc.SetTextColor(RGB(255,255,120));
    // Set the color of the caption to be yellow
    CString strTemp;
    GetWindowText(strTemp);
    // Get the caption which have been set
    dc.DrawText(strTemp,rt,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    // Draw out the caption
    if ( (state & ODS_FOCUS ) )       // If the button is focused
    {
        // Draw a focus rect which indicates the user
        // that the button is focused
        int iChange = 3;
        rt.top += iChange;
        rt.left += iChange;
        rt.right -= iChange;
        rt.bottom -= iChange;
        dc.DrawFocusRect(rt);
    }
    dc.Detach();
}
