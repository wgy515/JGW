#ifndef __UILISTTESTCTRL_H__
#define __UILISTTESTCTRL_H__
#pragma once
#include "UIList.h"

//#define  GRAY_BK_COLOR 0xffb4b4b4
//#define GREEN_BK_COLOR 0xff007d7d
//#define RED_BK_COLOR  0xffff0000
//#define BULE_BK_COLOR 0xff0000fa
//#define BALCK_BK_COLOR 0x00000000
//#define WHITE_BK_COLOR 0xffffffff
//#define PASS_COLOR 0x00ff00

typedef bool (*LPSortItemFunc)(UiLib::CListTextElementUI* pListTextEleUi1,UiLib::CListTextElementUI* pListTextEleUi2);
extern bool SortItemFuncAse(UiLib::CListTextElementUI* pListTextEleUi1,UiLib::CListTextElementUI* pListTextEleUi2);
extern bool SortItemFuncDesc(UiLib::CListTextElementUI* pListTextEleUi1,UiLib::CListTextElementUI* pListTextEleUi2);
namespace UiLib {
    class CPaintManagerUI;

    //class CListHeaderItemUI;
    class UILIB_API CListTestCtrlUI : public CListUI
    {
    public:
        CListTestCtrlUI(void);
        ~CListTestCtrlUI(void);

        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        //! 设置头行高
        void SetHeaderRowHeight(int nHeight);
        //! 设置头宽
        void SetHeaderRowWeight(int nCol,int nWeigth);
        //! 设置头字体
        void SetHeaderFont(CPaintManagerUI *pPaintManagerUi,int nFontSize,LPCTSTR pStrFontName = _T("宋体"));
        //! 设置头字体ID
        void SetHeaderFont(int nFontId);
        //! 设置列字体
        void SetColumnFont(CPaintManagerUI *pPaintManagerUi,int nFontSize,LPCTSTR pStrFontName = _T("宋体"));
        //! 设置列字体ID
        void SetColumnFont(int nFontId);
        //! 设置列高
        void SetColumnHeight(int nHeight);
        // 	//! 设置列宽
        //	void SetColumnWeight(int nCol,int nWeigth);

        CListContainerElementUIEx* InsertContainerItem(const wchar_t* strXmlFile,int nHeight);

        CListContainerElementUIEx* GetCListContainerElementUIEx(int nRow);

        void LineDown(int index,int nHeight);

        CControlUI* GetListContainerElementAt(int nRow,int indexHorizontalLayout,int indexCControlUI);
        //! 增加列
        CListHeaderItemUI* InsertColumn(int nCol,LPCTSTR strColumnName,int nWidth,bool bDefault = true);
        //！增加Item
        CListTextElementUI* InsertItem(int nRow,LPCTSTR strItemText,int nHeight);
        //! 增加Item
        //! CListTextElementUI* InsertItem(int nRow,LPCTSTR strItemText,int nHeight,CPaintManagerUI& paintManagerUI);

        CListContainerElementUIEx* InsertContainerItem(int nHeight);
        void		InsertContainerSubItem(CListContainerElementUIEx* pListContainerItem,int nSubItem,CControlUI* pSubControlUi);
        //! 设置选中颜色
        //void SetSelectedItemBkColor(DWORD dwBkColor);
        void SetSelectedItemIndex(int nRow);
        //! 取消选中项
        void SetUnSelectedItemIndex(int nRow);
        //! 全选
        void SetAllSelectedItem();
        //! 取消全选
        void SetAllUnSelectedItem();
        //! 选择下一行
        void EndItemDown();

        CListHeaderItemUI* GetHeadItemClass(int nCol);

        CListTextElementUI* GetListTextElementItemClass(int nRow);

        int	GetListCtrlCol(CListHeaderItemUI* pListHeaderItemUi);

        int GetListCtrlRow(CListTextElementUI* pListTextEleUi);

        int InsertItem(int nItem, int nHeight, CListContainerElementUIEx *pListItem);

        void SortItem(int nCol,LPSortItemFunc pSortItemFunc = SortItemFuncAse);
    };
}
#endif