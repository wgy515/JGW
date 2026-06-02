#include "StdAfx.h"
#include "ListTestCtrl.h"
#include "..\core\UIManager.h"
#include <algorithm>

namespace UiLib
{
    CListTestCtrlUI::CListTestCtrlUI(void)
    {
    }

    CListTestCtrlUI::~CListTestCtrlUI(void)
    {
    }

    LPCTSTR CListTestCtrlUI::GetClass() const
    {
        return _T("ListTestCtrlUI");
    }

    LPVOID CListTestCtrlUI::GetInterface(LPCTSTR pstrName)
    {
        if(m_pHeader)
            m_ListInfo.nColumns = MIN(m_pHeader->GetCount(), UILIST_MAX_COLUMNS);

        if( _tcscmp(pstrName, DUI_CTR_LIST_TEST_CTRL) == 0 ) return static_cast<CListTestCtrlUI*>(this);
        if( _tcscmp(pstrName, DUI_CTR_LIST) == 0 ) return static_cast<CListUI*>(this);
        if( _tcscmp(pstrName, _T("IList")) == 0 ) return static_cast<IListUI*>(this);
        if( _tcscmp(pstrName, _T("IListOwner")) == 0 ) return static_cast<IListOwnerUI*>(this);
        return CVerticalLayoutUI::GetInterface(pstrName);
    }

    void CListTestCtrlUI::SetHeaderRowHeight(int nHeight)
    {
        if( m_pHeader )
        {
            TCHAR szBuffer[10] = {0};
            _stprintf_s(szBuffer,10,_T("%d"),nHeight);

            m_pHeader->SetAttribute(_T("height"),szBuffer);
        }
    }

    void CListTestCtrlUI::SetHeaderRowWeight(int nCol,int nWeigth)
    {
        if( m_pHeader )
        {
            TCHAR szBuffer[10] = {0};
            _stprintf_s(szBuffer,10,_T("%d"),nWeigth);

            CControlUI *pUI = m_pHeader->GetItemAt(nCol);
            if(pUI) pUI->SetAttribute(_T("width"),szBuffer);
        }
    }

    // void CListTestCtrlUI::SetColumnWeight(int nCol,int nWeigth)
    // {
    // 	TCHAR szBuffer[10] = {0};
    // 	_stprintf_s(szBuffer,10,_T("%d"),nHeight);
    // 
    // 	CListTextElementUI *pUI = GetListTextElementItemClass(0);
    // 	if( !pUI )return ;
    // 	pUI->set
    // 	pUI->SetAttribute(_T("width"),szBuffer);
    // }

    void CListTestCtrlUI::SetColumnFont(int nFontId)
    {
        TCHAR szBuffer[10] = {0};
        _stprintf_s(szBuffer,10,_T("%d"),nFontId);

        /*m_pHeader->*/SetAttribute(_T("itemfont"),szBuffer);
    }

    void CListTestCtrlUI::SetColumnFont(CPaintManagerUI *pPaintManagerUi,int nFontSize,LPCTSTR pStrFontName /* = _T("宋体") */)
    {
        if( !pPaintManagerUi )return ;

        pPaintManagerUi->AddFont(pStrFontName,nFontSize,false,false,false);

        SetColumnFont( pPaintManagerUi->GetCustomFontCount() - 1);
    }

    void CListTestCtrlUI::SetHeaderFont(int nFontId)
    {
        if( m_pHeader )
        {
            TCHAR szBuffer[10] = {0};
            _stprintf_s(szBuffer,10,_T("%d"),nFontId);

            m_pHeader->SetAttribute(_T("font"),szBuffer);
        }
    }

    void CListTestCtrlUI::SetHeaderFont(CPaintManagerUI *pPaintManagerUi,int nFontSize,LPCTSTR pStrFontName /* = _T("宋体") */)
    {
        if( !pPaintManagerUi )return ;

        pPaintManagerUi->AddFont(pStrFontName,nFontSize,false,false,false);

        SetHeaderFont( pPaintManagerUi->GetCustomFontCount() - 1);
    }

    CListHeaderItemUI* CListTestCtrlUI::InsertColumn(int nCol,LPCTSTR strColumnName,int nWidth,bool bDefault/* = true*/)
    {
        CListHeaderItemUI* pListEle = new CListHeaderItemUI;
        if(bDefault)
        {
            pListEle->SetAttribute(_T("sepwidth"),_T("2"));
            pListEle->SetAttribute(_T("sepimage"),_T("file='list_header_sep.png'"));
            pListEle->SetAttribute(_T("hotimage"),_T("file='list_header_hot.png'"));
            pListEle->SetAttribute(_T("pushedimage"),_T("file='list_header_pushed.png'"));
        }
        pListEle->SetText(strColumnName);
        pListEle->SetTag(nCol);
        Add(pListEle);
        SetHeaderRowWeight(nCol,nWidth);

        return pListEle;
    }

    //CListTextElementUI* CListTestCtrlUI::InsertItem(int nRow,LPCTSTR strItemText,int nHeight,CPaintManagerUI& paintManagerUI)
    //{
    //   
    //    CListTextElementUI* pListElement = NULL;
    //    if ( 0 == m_aIdelListTextElementUi.GetSize() )
    //    {
    //        pListElement = new CListTextElementUI;
    //        pListElement->SetManager(this->GetManager(),NULL,false);
    //        pListElement->SetTag(nRow);
    //        pListElement->SetAttribute(_T("bkcolor"),_T("#FF000000"));
    //        pListElement->SetAttribute(_T("borderround"),_T("3,3"));
    //    }
    //    else
    //    {
    //        pListElement = (CListTextElementUI*)m_aIdelListTextElementUi.GetAt(0);
    //        m_aIdelListTextElementUi.Remove(0);
    //    }
    //
    //    TCHAR szBuffer[10] = {0};
    //    _stprintf_s(szBuffer,10,_T("%d"),nHeight);
    //    pListElement->SetAttribute(_T("height"),szBuffer);
    //    Add(pListElement);
    //
    //    return pListElement;
    //}

    void CListTestCtrlUI::LineDown(int index,int nHeight)
    {
        SIZE sz = GetScrollPos();
        sz.cy = index * nHeight;
        sz.cy -= (GetHeight() / nHeight / 2 /*- 1*/) * nHeight;
        if (sz.cy < 0) sz.cy = 0;
        SetScrollPos(sz);
    }


    CListContainerElementUIEx* CListTestCtrlUI::GetCListContainerElementUIEx(int nRow)
    {
        return (CListContainerElementUIEx*)GetList()->GetItemAt(nRow);
    }

    CControlUI* CListTestCtrlUI::GetListContainerElementAt(int nRow,int indexHorizontalLayout,int indexCControlUI)
    {
        CListContainerElementUIEx* plsitContainerItem = (CListContainerElementUIEx*)GetList()->GetItemAt(nRow);
        if (!plsitContainerItem) return NULL;
        CHorizontalLayoutUI* pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(plsitContainerItem->GetItemAt(indexHorizontalLayout));
        if (!pHorizontalLayoutUi) return NULL;
        return pHorizontalLayoutUi->GetItemAt(indexCControlUI);
    }

    CListContainerElementUIEx* CListTestCtrlUI::InsertContainerItem(const wchar_t* strXmlFile,int nHeight)
    {
        CListContainerElementUIEx* plsitContainerItem = NULL;
        if ( 0 == m_aIdelListTextElementUi.GetSize() )
        {
            CDialogBuilder builder1;
            plsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(strXmlFile,(UINT)0,NULL,m_pManager));
            if (!plsitContainerItem) return NULL;
        }
        else
        {
            plsitContainerItem = (CListContainerElementUIEx*)m_aIdelListTextElementUi.GetAt(0);
            m_aIdelListTextElementUi.Remove(0);
        }

        TCHAR szBuffer[10] = {0};
        _stprintf_s(szBuffer,10,_T("%d"),nHeight);
        plsitContainerItem->SetAttribute(_T("height"),szBuffer);
        Add(plsitContainerItem);

        return plsitContainerItem;
    }

    CListTextElementUI* CListTestCtrlUI::InsertItem(int nRow,LPCTSTR strItemText,int nHeight)
    {
        CListTextElementUI* pListElement = NULL;
        if ( 0 == m_aIdelListTextElementUi.GetSize() )
        {
            pListElement = new CListTextElementUI;
            pListElement->SetTag(nRow);
            pListElement->SetAttribute(_T("bkcolor"),_T("#FF000000"));
            pListElement->SetAttribute(_T("borderround"),_T("3,3"));
        }
        else
        {
            pListElement = (CListTextElementUI*)m_aIdelListTextElementUi.GetAt(0);
            m_aIdelListTextElementUi.Remove(0);
        }

        TCHAR szBuffer[10] = {0};
        _stprintf_s(szBuffer,10,_T("%d"),nHeight);
        pListElement->SetAttribute(_T("height"),szBuffer);
        Add(pListElement);

        return pListElement;
    }

    void CListTestCtrlUI::SetColumnHeight(int nHeight)
    {
        TCHAR szBuffer[10] = {0};

        _stprintf_s(szBuffer,10,_T("%d"),nHeight);

        for (int i = 0;i < GetList()->GetCount();i ++)
        {
            GetList()->GetItemAt(i)->SetAttribute(_T("height"),szBuffer);
        }

    }

    void CListTestCtrlUI::SetSelectedItemIndex(int nRow)
    {
        GetListTextElementItemClass(nRow)->Select(true);
    }

    void CListTestCtrlUI::SetUnSelectedItemIndex(int nRow)
    {
        GetListTextElementItemClass(nRow)->Select(false);
    }

    void CListTestCtrlUI::SetAllUnSelectedItem()
    {
        for (int i = 0;i < GetList()->GetCount();i ++)
        {
            if( _tcscmp(GetList()->GetItemAt(i)->GetClass(), L"ListContainerElementUIEx") == 0 )
                ((CListContainerElementUIEx*)GetList()->GetItemAt(i))->Select(false);
            else
                ((CListTextElementUI*)GetList()->GetItemAt(i))->Select(false);
        }
    }

    void CListTestCtrlUI::SetAllSelectedItem()
    {
        for (int i = 0;i < GetList()->GetCount();i ++)
        {
            if( _tcscmp(GetList()->GetItemAt(i)->GetClass(), L"ListContainerElementUIEx") == 0 )
                ((CListContainerElementUIEx*)GetList()->GetItemAt(i))->Select(true);
            else
                ((CListTextElementUI*)GetList()->GetItemAt(i))->Select(true);
        }
    }

    CListTextElementUI* CListTestCtrlUI::GetListTextElementItemClass(int nRow)
    {
        if(nRow < 0 || nRow >= GetList()->GetCount())return NULL;

        return (CListTextElementUI*)GetList()->GetItemAt(nRow);
    }

    CListHeaderItemUI* CListTestCtrlUI::GetHeadItemClass(int nCol)
    {
        if(nCol < 0 || nCol >= m_pHeader->GetCount())return NULL;

        return (CListHeaderItemUI*)m_pHeader->GetItemAt(nCol);
    }

    int CListTestCtrlUI::GetListCtrlCol(CListHeaderItemUI* pListHeaderItemUi)
    {
        for (int i = 0;i < m_pHeader->GetCount();i ++)
        {
            if( pListHeaderItemUi == m_pHeader->GetItemAt(i) ) return i;
        }

        return -1;
    }

    int CListTestCtrlUI::GetListCtrlRow(CListTextElementUI* pListTextEleUi)
    {
        for (int i = 0;i < GetList()->GetCount();i ++)
        {
            if( pListTextEleUi == GetList()->GetItemAt(i) ) return i;
        }

        return -1;
    }

    int g_nCol  = 0;

    void CListTestCtrlUI::SortItem( int nCol,LPSortItemFunc pSortItemFunc )
    {
        if( nCol < 0 || nCol >= m_pHeader->GetCount() || !pSortItemFunc )return ;
        std::vector<CListTextElementUI*> _vListTextUi;
        g_nCol = nCol;

        for (int i = 0;i < GetList()->GetCount();i ++)
        {
            _vListTextUi.push_back((CListTextElementUI*)GetList()->GetItemAt(i));
        }

        sort( _vListTextUi.begin(),_vListTextUi.end(),pSortItemFunc );
        int nIndex = 0;

        for (std::vector<CListTextElementUI*>::iterator it = _vListTextUi.begin();
            it != _vListTextUi.end();
            it++,nIndex++)
        {
            /*(CControlUI*)it[0];*/
            SetItemIndex(it[0],nIndex);
        }
    }

    bool SortItemFuncAse(UiLib::CListTextElementUI* pListTextEleUi1,UiLib::CListTextElementUI* pListTextEleUi2)
    {
        TCHAR strTextUi1[512] = {0};
        TCHAR strTextUi2[512] = {0};

        _tcscpy_s(strTextUi1,512,pListTextEleUi1->GetText(g_nCol));
        _tcscpy_s(strTextUi2,512,pListTextEleUi1->GetText(g_nCol));

        if( _tcscmp(strTextUi1,strTextUi2) < 0 ) 
            return true;

        return false;
    }

    bool SortItemFuncDesc(UiLib::CListTextElementUI* pListTextEleUi1,UiLib::CListTextElementUI* pListTextEleUi2)
    {
        TCHAR strTextUi1[512] = {0};
        TCHAR strTextUi2[512] = {0};

        _tcscpy_s(strTextUi1,512,pListTextEleUi1->GetText(g_nCol));
        _tcscpy_s(strTextUi2,512,pListTextEleUi1->GetText(g_nCol));

        if( _tcscmp(strTextUi1,strTextUi2) < 0 ) 
            return false;

        return true;
    }

    void CListTestCtrlUI::EndItemDown()
    {
        SIZE sz = GetScrollPos();
        sz.cy = GetScrollRange().cy  + 300;
        GetVerticalScrollBar()->SetScrollRange( GetVerticalScrollBar()->GetScrollRange() + 300);
        SetScrollPos(sz);
    }

    CListContainerElementUIEx* CListTestCtrlUI::InsertContainerItem(int nHeight)
    {
        CListContainerElementUIEx *pListItem = new CListContainerElementUIEx;
        pListItem->SetFixedHeight(nHeight);/*固定一个行高*/
        pListItem->m_pHeader = CListUI::GetHeader();
        if (NULL !=  pListItem->m_pHeader )
        {
            int nHeaderCount =  pListItem->m_pHeader->GetCount();
            for (int i = 0; i < nHeaderCount; i++)
            {
                CHorizontalLayoutUI* lphui = new CHorizontalLayoutUI;
                lphui->SetAttribute(L"inset",L"5,5,5,5");
                pListItem->Add(lphui);
            }
        }
        if ( !CListUI::AddAt(pListItem,GetCount()) )
        {
            delete pListItem;
            pListItem = NULL;
            return NULL;
        }
        return pListItem;
    }

    void CListTestCtrlUI::InsertContainerSubItem(CListContainerElementUIEx* pListContainerItem,int nSubItem,CControlUI* pSubControlUi)
    {
        if( !pListContainerItem ) return;
        if( nSubItem >= CListUI::GetHeader()->GetCount() ) return ;

        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;
        pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(pListContainerItem->GetItemAt(nSubItem));

        if( !pHorizontalLayoutUi ) return;

        pHorizontalLayoutUi->Add(pSubControlUi);
    }

    int CListTestCtrlUI::InsertItem(int nItem, int nHeight, CListContainerElementUIEx *pListItem)
    {
        pListItem->SetFixedHeight(nHeight);	
        pListItem->m_pHeader =  CListUI::GetHeader();
        if ( !CListUI::AddAt(pListItem, nItem) )
        {
            delete pListItem;
            pListItem = NULL;
            return -1;
        }

        return nItem;
    }
}