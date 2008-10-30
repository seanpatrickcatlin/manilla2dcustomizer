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

#include "Manilla2DConfigUtils.h"

#define CE_DLGSCROLLBAR 64000

// CManilla2DConfigAbstractDlg dialog

class CManilla2DConfigAbstractDlg : public CPropertyPage
{
	//DECLARE_DYNAMIC(CManilla2DConfigAbstractDlg)

public:
	CManilla2DConfigAbstractDlg(CWnd* pParent = NULL, UINT dlgID = -1, UINT tabStrID = -1, UINT titleStrID = -1, bool useOkCancelMenu = false);   // standard constructor
	virtual ~CManilla2DConfigAbstractDlg();

protected:
    bool m_useOkCancelMenu;
    CString m_titleStr;
    void OnPaint();

public:
    BOOL OnInitDialog();
    BOOL OnKillActive();
    
    void OnSize(UINT nType, int cx, int cy);
    void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()

protected:
    CScrollBar m_ctrlScrollBar;
    CRect      m_boundingRect;
    CRect      m_scrollBarRect;
    BOOL       m_bScrollBarHasBeenInitialized;

    int        m_nScrollPos;
    
    void initScrollBarSupport();    // Call this function from derived class to setup the scrollbar. 

    CRect GetDlgControlBounds();

private:
    CCommandBar m_cmdBar;
    UINT m_dlgId;
};
