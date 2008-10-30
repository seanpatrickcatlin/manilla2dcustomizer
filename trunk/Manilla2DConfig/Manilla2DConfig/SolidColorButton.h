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

#include "afxwin.h"
#include "Manilla2DConfig.h"

class CSolidColorButton : public CButton
{
public:
    CSolidColorButton();
    ~CSolidColorButton();

private:
    int m_red;
    int m_green;
    int m_blue;

public:
    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    void GetColorValues(int &red, int &green, int &blue);
    void SetColorValues(int red, int green, int blue);

    DECLARE_MESSAGE_MAP()
};