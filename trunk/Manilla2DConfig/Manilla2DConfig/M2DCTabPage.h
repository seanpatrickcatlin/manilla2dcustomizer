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

// CM2DCTabPage dialog

class CM2DCTabPage : public CPropertyPage
{
public:
    CM2DCTabPage(UINT nIDTemplate, UINT nIDCaption) { CPropertyPage(nIDTemplate, nIDCaption); };
    virtual ~CM2DCTabPage() { };

    virtual void OnOK() = 0;
    virtual void OnCancel() = 0;
    virtual UINT GetTabStringID() = 0;
    virtual void RestoreDefaults() = 0;
};
