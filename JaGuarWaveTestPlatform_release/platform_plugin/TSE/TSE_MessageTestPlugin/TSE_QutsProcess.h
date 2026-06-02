#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_QutsProcess : public CTSE_TestBase
    {
    public:
        CTSE_QutsProcess(void);
        ~CTSE_QutsProcess(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    private:
        void StartQutsProcess();
    private:
        bool mbCheckQutsProcessRuning;
         /*
        #define SW_HIDE             0
        #define SW_SHOWNORMAL       1
        #define SW_NORMAL           1
        #define SW_SHOWMINIMIZED    2
        #define SW_SHOWMAXIMIZED    3
        #define SW_MAXIMIZE         3
        #define SW_SHOWNOACTIVATE   4
        #define SW_SHOW             5
        #define SW_MINIMIZE         6
        #define SW_SHOWMINNOACTIVE  7
        #define SW_SHOWNA           8
        #define SW_RESTORE          9
        #define SW_SHOWDEFAULT      10
        #define SW_FORCEMINIMIZE    11
        #define SW_MAX              11
        */
        int mnShowWindow;
        std::wstring mstrQutsExecFilePath;
    };  
}
