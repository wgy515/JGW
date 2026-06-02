#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_WindowsFuncPlugin/CJGW_DownloadPDBImage.h>
#include "CDownloadPDB_Define.h"

namespace JGW
{
    class CCDownloadPDBThread:public CCMessageThread
    {
    public:
        CCDownloadPDBThread(void);
        ~CCDownloadPDBThread(void);
    private:
        void OnInitWnd( WPARAM wParam,LPARAM lParam );
        void OnRunThread( WPARAM wParam,LPARAM lParam );

        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        void SendListMsgToWnd(const wchar_t* strMsg,bool bInsert = true);

        bool OpenPerasoComPort();

        bool InitDongle();

		bool InputSN();

        bool GetInputSerialNumber();

        bool WriteSerialNumber();

        bool BuildOTPBin();

        bool DownloadPDBImageFiles();

        bool ResetDongle();
    private:
        bool                    mbInitLoadPDBImage;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        LPS_DOWNLOAD_PDB_PARAM mpsDownloadPDBParam;
        std::string            mstrSerialNumber;
        std::wstring            mwstrSerialNumber;
        CCJGW_CSerialComPort    mcSerialComPort;
        CCJGW_DownloadPDBImage mcDownloadPDBImage;
    };
}


