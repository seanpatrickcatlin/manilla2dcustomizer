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

// Manilla2DConfigAbstractDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Manilla2DConfig.h"
#include "Manilla2DConfigAbstractDlg.h"

#define TITLE_HEADER_HEIGHT 24

// CManilla2DConfigAbstractDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigAbstractDlg, CM2DCTabPage)

CManilla2DConfigAbstractDlg::CManilla2DConfigAbstractDlg(CWnd* pParent /*= NULL*/, UINT dlgID, UINT tabStrID, UINT titleStrID)
: CPropertyPage(dlgID, tabStrID)
{
    m_dlgId = dlgID;

    m_titleStr.LoadStringW(titleStrID);

    m_bScrollBarHasBeenInitialized = FALSE;
    m_nScrollPos = 0;
}

CManilla2DConfigAbstractDlg::~CManilla2DConfigAbstractDlg()
{
    if(IsWindow(m_ctrlScrollBar))
    {
        m_ctrlScrollBar.DestroyWindow();
    }
}

BOOL CManilla2DConfigAbstractDlg::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    TRACE(TEXT("CManilla2DConfigAbstractDlg OnInitDialog\n"));

    m_cmdBar.Create(this);
    m_cmdBar.InsertMenuBar(IDR_M2DC_APPLY_CANCEL_MENU);

    initScrollBarSupport();

    return FALSE;
}

BEGIN_MESSAGE_MAP(CManilla2DConfigAbstractDlg, CPropertyPage)
    ON_WM_PAINT()
    ON_WM_SIZE()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()

// CManilla2DConfigAbstractDlg message handlers

void CManilla2DConfigAbstractDlg::OnPaint()
{
    CPaintDC dc(this);

    //if((TITLE_HEADER_HEIGHT-m_nScrollPos) > 0)
    {
        int nWidth = dc.GetDeviceCaps(HORZRES);

        // paint title
        CFont *pCurrentFont = dc.GetCurrentFont();
        LOGFONT lf;
        pCurrentFont->GetLogFont(&lf);
        lf.lfWeight = FW_BOLD;

        CFont newFont;
        newFont.CreateFontIndirect(&lf);
        CFont *pSave = dc.SelectObject(&newFont);
        dc.SetTextColor(RGB(0, 0, 156));
        dc.DrawText(m_titleStr, CRect(8, -m_nScrollPos, nWidth, TITLE_HEADER_HEIGHT-m_nScrollPos), DT_VCENTER | DT_SINGLELINE);
        dc.SelectObject(pSave);

        // paint line
        CPen blackPen(PS_SOLID, 1, RGB(0,0,0));
        CPen *pOldPen = dc.SelectObject(&blackPen);

        dc.MoveTo(0, TITLE_HEADER_HEIGHT-m_nScrollPos);
        dc.LineTo(nWidth, TITLE_HEADER_HEIGHT-m_nScrollPos);

        dc.SelectObject(pOldPen);
    }
}

void CManilla2DConfigAbstractDlg::initScrollBarSupport()
{
    m_boundingRect = GetDlgControlBounds();

    CRect currentDlgRect;
    GetWindowRect(&currentDlgRect);

    DWORD dwStyle = SBS_VERT | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS;
    m_ctrlScrollBar.Create(dwStyle, CRect(0, 0, 10, m_boundingRect.Height()), this, CE_DLGSCROLLBAR);

    CRect scrollBarRect;

    m_ctrlScrollBar.GetWindowRect(&scrollBarRect);
    m_ctrlScrollBar.MoveWindow(currentDlgRect.Width()-scrollBarRect.Width(), 0, scrollBarRect.Width(), scrollBarRect.Height());
    m_ctrlScrollBar.BringWindowToTop();
    m_ctrlScrollBar.ShowScrollBar();

    m_ctrlScrollBar.SetScrollRange(0, m_boundingRect.Height());

    m_nScrollPos = 0;
    m_ctrlScrollBar.SetScrollPos(m_nScrollPos);

    // we don't need it right away, so hide it from the user
    m_ctrlScrollBar.ShowWindow(SW_HIDE);

    m_bScrollBarHasBeenInitialized = TRUE;
}

BOOL CManilla2DConfigAbstractDlg::OnKillActive() 
{
   SHSipPreference(m_hWnd, SIP_FORCEDOWN); 
   return CPropertyPage::OnKillActive();
}

void CManilla2DConfigAbstractDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if(pScrollBar == &m_ctrlScrollBar)
    {
        int nDelta = 0;
        int nMaxPos = m_ctrlScrollBar.GetScrollLimit();

        switch (nSBCode)
        {
        case SB_LINEDOWN:
        case SB_PAGEDOWN:
            if (m_nScrollPos >= nMaxPos)
            {
                return;
            }
            nDelta = min(max(nMaxPos / 2, 30), nMaxPos - m_nScrollPos);
            break;

        case SB_LINEUP:
        case SB_PAGEUP:
            if (m_nScrollPos <= 0)
            {
                return;
            }
            nDelta = -min(max(nMaxPos / 2, 30), m_nScrollPos);
            break;

        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
            nDelta = (int)nPos - m_nScrollPos;
            break;

        case SB_BOTTOM:
            nDelta = 50;
            break;

        case SB_TOP:
            if (m_nScrollPos <= 0)
            {
                return;
            }
            nDelta = -m_nScrollPos;
            break;

        case SB_ENDSCROLL:
        default:
            break;
        }

        m_nScrollPos += nDelta;
        pScrollBar->SetScrollPos(m_nScrollPos, TRUE);

        CRect currentDlgRect;
        GetWindowRect(&currentDlgRect);

        CRect scrollBarRect;
        m_ctrlScrollBar.GetWindowRect(&scrollBarRect);

        ScrollWindowEx(0, -nDelta, NULL, &currentDlgRect,  NULL, NULL, SW_ERASE|SW_INVALIDATE|SW_SCROLLCHILDREN);
        pScrollBar->MoveWindow(currentDlgRect.Width()-scrollBarRect.Width(), 0, scrollBarRect.Width(), scrollBarRect.Height());
        RedrawWindow(NULL, NULL, RDW_ERASENOW);
    }
}

void CManilla2DConfigAbstractDlg::OnSize(UINT nType, int cx, int cy)
{
    CPropertyPage::OnSize(nType, cx, cy);

    if(m_bScrollBarHasBeenInitialized)
    {
        CRect currentDlgRect;
        GetWindowRect(&currentDlgRect);

        ScrollWindowEx(0, m_ctrlScrollBar.GetScrollPos(), NULL, &currentDlgRect,  NULL, NULL, SW_SCROLLCHILDREN);
        m_nScrollPos = 0;
        m_ctrlScrollBar.SetScrollPos(m_nScrollPos);

        if(cy < m_boundingRect.Height())
        {
            CRect scrollBarRect;
            m_ctrlScrollBar.GetWindowRect(&scrollBarRect);

            scrollBarRect.bottom = scrollBarRect.top + cy; 
            m_ctrlScrollBar.MoveWindow(cx - scrollBarRect.Width(), 0, scrollBarRect.Width(), scrollBarRect.Height());

            m_ctrlScrollBar.SetScrollRange(0, m_boundingRect.Height() - cy);

            m_ctrlScrollBar.ShowWindow(SW_SHOW);
        }
        else
        {
            m_ctrlScrollBar.ShowWindow(SW_HIDE);
            RedrawWindow();
        }
    }
}

CRect CManilla2DConfigAbstractDlg::GetDlgControlBounds()
{
    CRect boundingRect;
    CRect currentObjectRect;
    
    boundingRect.SetRect(0, 0, 0, 0);

    HWND hwnd = ::GetTopWindow(this->GetSafeHwnd());

    // while we have a valid hwnd loop through all child windows
    while (hwnd)
    {
        ::GetWindowRect(hwnd, &currentObjectRect);
        ScreenToClient(&currentObjectRect);

        CPoint bottomRightPoint = boundingRect.BottomRight();

        if(bottomRightPoint.x < currentObjectRect.BottomRight().x)
        {
            bottomRightPoint.x = currentObjectRect.BottomRight().x;
        }

        if(bottomRightPoint.y < currentObjectRect.BottomRight().y)
        {
            bottomRightPoint.y = currentObjectRect.BottomRight().y;
        }

        if(bottomRightPoint != boundingRect.BottomRight())
        {
            boundingRect.SetRect(0, 0, bottomRightPoint.x, bottomRightPoint.y);
        }

        // and get the next child control's hwnd
        hwnd = ::GetNextWindow(hwnd, GW_HWNDNEXT);
    }

    return boundingRect;
}

