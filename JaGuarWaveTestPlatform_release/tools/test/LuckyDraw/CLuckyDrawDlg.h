#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "LuckyDrawDefine.h"

namespace JGW
{
    class CCLuckyDrawDlg : public CCAsyncMsgDialog
    {
    public:
        CCLuckyDrawDlg(void);
        ~CCLuckyDrawDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_LossConfigToolDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        int GetWinID();
        void LoadDrawLuckyConfig();
        void OnReDrawLucky();
        void OnSelectDrawLucky(CButtonUI* pButton);
        bool FindWinIDIsSmoking(int winID);
    private:
        CListUI* mpLuckyDrawVerticalLayout;
        CButtonUI* mpSaveLossBtn;
        S_LUCKY_DRAW_CONFIG msLuckyDrawConfig;
        //! 抽奖抽中的名单
        std::vector<int> mvSmokingList;
    };
}


