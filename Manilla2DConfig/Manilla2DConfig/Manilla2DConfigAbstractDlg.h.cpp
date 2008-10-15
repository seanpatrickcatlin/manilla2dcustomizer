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


// CManilla2DConfigAbstractDlg dialog

//IMPLEMENT_DYNAMIC(CManilla2DConfigAbstractDlg, CM2DCTabPage)

CManilla2DConfigAbstractDlg::CManilla2DConfigAbstractDlg(CWnd* pParent /*= NULL*/, UINT dlgID, UINT tabStrID, UINT titleStrID)
: CPropertyPage(dlgID, tabStrID)
{
    m_titleStr.LoadStringW(titleStrID);

    m_bAutoScroll = FALSE;
    m_bNeedScrollBar = FALSE;
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
    ON_MESSAGE(WM_SETTINGCHANGE, CManilla2DConfigAbstractDlg::OnSettingChange)
END_MESSAGE_MAP()

// CManilla2DConfigAbstractDlg message handlers

void CManilla2DConfigAbstractDlg::OnPaint()
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
    dc.DrawText(m_titleStr, CRect(8, 0, nWidth, nHeaderHeight), DT_VCENTER | DT_SINGLELINE); dc.SelectObject(pSave);

    // paint line
    CPen blackPen(PS_SOLID, 1, RGB(0,0,0));
    CPen *pOldPen = dc.SelectObject(&blackPen);

    dc.MoveTo(0, nHeaderHeight);
    dc.LineTo(nWidth, nHeaderHeight);

    dc.SelectObject(pOldPen); 
}

void CManilla2DConfigAbstractDlg::initScrollBarSupport()
{
    m_bNeedScrollBar = TRUE;
    GetClientRect(&m_DlgClientRect);

    CString debugMsg;
    debugMsg.Format(TEXT("CManilla2DConfigAbstractDlg::initScrollBarSupport (%d, %d)\n"), m_DlgClientRect.Width(), m_DlgClientRect.Height());
    TRACE(debugMsg);

    // create the scrollbar - the initial size is up to you,
    // but when everything is said and done, you want it
    // to be as narrow as possible. The "142" is 
    // how big the scrollbar would actually be if
    // you didn't dynamically size it.

    DWORD dwStyle = SBS_VERT | WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS;
    m_ctrlScrollBar.Create(dwStyle, CRect(0, 0, 10, 142),  this, CE_DLGSCROLLBAR);

    m_ctrlScrollBar.GetWindowRect(&m_ScrollBarRect);
    m_ctrlScrollBar.MoveWindow(m_DlgClientRect.Width()-m_ScrollBarRect.Width(),  0, m_ScrollBarRect.Width(), m_ScrollBarRect.Height());
    m_ctrlScrollBar.BringWindowToTop();
    m_ctrlScrollBar.ShowScrollBar();
    m_ctrlScrollBar.GetWindowRect(&m_ScrollBarRect);

    m_nScrollPos = 0;
    m_nCurHeight = m_ScrollBarRect.Height() + 30;
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_ALL; 
    si.nMin   = 0;
    si.nMax   = m_DlgClientRect.Height();
    si.nPage  = (int)(m_DlgClientRect.Height()* 0.75);
    si.nPos   = 0;
    m_ctrlScrollBar.SetScrollInfo(&si, TRUE);  

    // we don't need it right away, so hide it from the user
    m_ctrlScrollBar.ShowWindow(SW_HIDE);
}

BOOL CManilla2DConfigAbstractDlg::OnKillActive() 
{
   SHSipPreference(m_hWnd, SIP_FORCEDOWN); 
   return CPropertyPage::OnKillActive();
}

void CManilla2DConfigAbstractDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    if (pScrollBar == &m_ctrlScrollBar)
    {
        int nDelta  = 0;
        int nMaxPos = m_DlgClientRect.Height() - m_nCurHeight;

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
            return;

        default:
            return;
        }

        m_nScrollPos += nDelta;
        pScrollBar->SetScrollPos(m_nScrollPos, TRUE);
        ScrollWindowEx(0, -nDelta, NULL, &m_DlgClientRect,  NULL, NULL, SW_SCROLLCHILDREN);
        pScrollBar->MoveWindow(m_DlgClientRect.Width()-m_ScrollBarRect.Width(), 0, m_ScrollBarRect.Width(), m_ScrollBarRect.Height());
    }
}

LRESULT CManilla2DConfigAbstractDlg::OnSettingChange(WPARAM wParam, LPARAM lParam)
{
    if (m_bNeedScrollBar)
    {
        SIPINFO si;
        memset(&si, 0, sizeof(si));
        si.cbSize = sizeof(si);
        if (SHSipInfo(SPI_GETSIPINFO, 0, &si, 0)) 
        {
            BOOL bShowScrollBar = ((si.fdwFlags & SIPF_ON) == SIPF_ON);
            if (!bShowScrollBar)
            {
                if (m_nScrollPos > 0)
                {
                    OnVScroll(SB_TOP, 0, &m_ctrlScrollBar);
                }
                m_ctrlScrollBar.ShowWindow(SW_HIDE);
            }
            else
            {
                // During testing, I discovered that the
                // standard CE SIP is always the same 
                // height, no matter which SIP mode you're using.
                // This means that if you're
                // truly interested in performance
                // (albeit a negligible gain), you can comment 
                // out this code so that the height of the
                // scrollbar is always 142 pixels (as 
                // originally set when you called Create on the scrollbar).

                CRect sipRect(si.rcSipRect);
                CRect clientRect;
                GetClientRect(&clientRect);
                m_ctrlScrollBar.GetWindowRect(&m_ScrollBarRect);
                m_ScrollBarRect.bottom = m_ScrollBarRect.top + clientRect.Height(); 
                m_ctrlScrollBar.MoveWindow(m_DlgClientRect.Width()-m_ScrollBarRect.Width(),
                                            0, m_ScrollBarRect.Width(), m_ScrollBarRect.Height());

                // show the scrollbar
                m_ctrlScrollBar.ShowWindow(SW_SHOW);
                if (m_bAutoScroll)
                {
                    OnVScroll(SB_BOTTOM, 0, &m_ctrlScrollBar);
                }
            }
        }
    }

    return 1L;
}

void CManilla2DConfigAbstractDlg::OnSize(UINT nType, int cx, int cy)
{
    CPropertyPage::OnSize(nType, cx, cy);

    CString debugMsg;
    debugMsg.Format(TEXT("CManilla2DConfigAbstractDlg::OnSize (%d, %d)\n"), cx, cy);

    TRACE(debugMsg);
}
