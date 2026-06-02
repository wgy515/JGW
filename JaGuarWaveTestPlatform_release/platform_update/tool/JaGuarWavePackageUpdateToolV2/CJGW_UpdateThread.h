#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include "JGWPackageUpdateDefine.h"
#include "CJaGuarWavePackageUpdateToolDlgV2.h"

namespace JGW
{
    class CCJGW_UpdateThread : public CCMessageThread
    {
    public:
        CCJGW_UpdateThread(CCJaGuarWavePackageUpdateToolDlgV2 *pMainDlg);
        ~CCJGW_UpdateThread(void);
    private:
        //! download help db
        void OnInitThread(WPARAM wParam,LPARAM lParam);
        //! 处理FTP下载等相关信息
        void OnHandleFtpDownloadMsg(WPARAM wParam,LPARAM lParam);
        //! 处理FTP上传等相关信息
        void OnHandleFtpUploadMsg(WPARAM wParam,LPARAM lParam);
        //!
        void OnHandleAsyncFuncMsg(WPARAM wParam,LPARAM lParam);
    private:
        void PrintLogW(const wchar_t *format, ...);
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        CCJGW_AsyncWndMessage *mpAsyncWndMessage;
        S_PACKAGE_UPDATE_CONFIG *mpsPackageUpdateConfig;
        CCJaGuarWavePackageUpdateToolDlgV2 *mpMainDlg;
    };

}

