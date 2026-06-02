#pragma once
#include <UiLib/UIlib.h>
#include <JGW_TestInterface/JGW_TestInterface.h>
#include <JGW_TestRecord/JGW_TestRecord_Exports.h>

namespace JGW
{
    class CCJGW_TestView
    {
    public:
        static CCJGW_TestView& GetInstanceTestView()
        {
            static CCJGW_TestView gJGWTestView;
            return gJGWTestView;
        }

    public:
        bool InitTestView(CPaintManagerUI* pPaintManagerUI,const wchar_t* strFolderName,const wchar_t* strRecordConfigName);

        bool OnChangeTestStatus(E_TEST_STATUS_TYPE eTestStatusType);

        void OnUpdateTestTime();

        void OnClearRecordData();
        //! WM_HWND_SAVE_TEST_LOG_SELECTCHANGE_MSG
        bool OnSaveTestLogSelectChangedMsg();
    private:
        void OnTestIDELStatus();
        void OnTestBusyStatus();
        void OnTestFailStatus();
        void OnTestPassStatus();

        void UpdateTestRecordView();
    private:
        CCJGW_TestView(void);
        ~CCJGW_TestView(void);
   
    private:
        CPaintManagerUI* mpPaintManagerUI;
        CButtonUI*      mpStartBtn;
        CButtonUI*      mpTestStatusBtn;
        CCheckBoxUI*      mpSaveTestLog;
        size_t          mdwTestTime;
        CButtonUI*      mpTestTimeBtn;

        CTextUI*        mpPassTextUi;
        CTextUI*        mpFailTextUi;
        CTextUI*        mpCountTextUi;
        CTextUI*        mpPassRateTextUi;
        CHorizontalLayoutUI* mpTestInfoVerticalLayout;

        size_t          mnTestPassCount;
        size_t          mnTestCounts;

        CCJGW_TestRecordImpl* mpTestRecordImpl;
        //std::wstring mstrFolderName;
    };

}

