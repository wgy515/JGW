#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_WindowsFuncPlugin/CJGW_DownloadPDBImage.h>
#include "CWriteNumber_Define.h"

namespace JGW
{
    class CCWriteNumberThread:public CCMessageThread
    {
    public:
        CCWriteNumberThread(void);
        ~CCWriteNumberThread(void);
    private:
        void OnInitWnd( WPARAM wParam,LPARAM lParam );
        void OnRunThread( WPARAM wParam,LPARAM lParam );

        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        void SendListMsgToWnd(const wchar_t* strMsg,bool bInsert = true);

        bool OpenPerasoComPortView();

        bool InitDongleView();
        //! 从设备中读取
        bool GetSerialNumberView();

        bool GetMacAddressView();

        bool WriteSerialNumberAndMacAddressView();

        bool ResetDongleView();
    private:
        bool ValidationMacAddressEffectiveness();
        bool GetInputNumber(size_t nMaxChar,const wchar_t* strTipLabel,const wchar_t* strInputTip);
    private:
        bool                    mbInitLoadPDBImage;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;       
        LPS_DOWNLOAD_PDB_PARAM  mpsDownloadPDBParam;
        std::string             mstrSerialNumber;
        std::string             mstrMacAddress;
        std::wstring            mstrInput;     
        std::wstring            mwstrSerialNumber;     
        CCJGW_CSerialComPort    mcSerialComPort;
        CCJGW_DownloadPDBImage  mcDownloadPDBImage;
    };
}

