#pragma once
#include "LYNX_FTM\CJGW_FTM_ClientSocket.h"
#include <JGW_QMSLFuncPlugin/jgw_wr_nv_api.h>
#include <JGW_WindowsFuncPlugin/CJGW_DownloadPDBImage.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include <JGW_WindowsFuncPlugin/CJGW_PrsDownloadFlash.h>
#include <JGW_SSHPlugin/CJGW_SecureShell.h>
#include <JGW_SSHPlugin/CJGW_SSHShell.h>
#include <JGW_SSHPlugin/CJGW_SSHSFTP.h>
#include <JGW_SSHPlugin/CJGW_SSHSCP.h>

namespace JGW
{
    class CTSE_DevicesGlobalResource
    {
    public:
        ~CTSE_DevicesGlobalResource();

        static jgw_wr_nv_api* GetConnetQcommDiagActionIsntance(HANDLE hResourceContext = NULL,unsigned char chUseQPST = FALSE);
        static jgw_wr_nv_api* GetQcommDiagActionIsntance();
        //! 
        static CCJGW_PrsFlashDownloadBase* GetDownloadPDBInstance();
        //!
        static void SettingPrsDeviceBootMode(eDeviceBootMode deviceBootMode);
        //!
        static CCJGW_PrsDownloadFlash* GetPrsDownloadFlashInstance();
        //!
        static CCJGW_DonglesComPort* GetDonglesComPortInstance();
        //! 
        static CCJGW_FTM_ClientSocket* GetFTMClientSocket();
        //!
        static CCJGW_CSerialComPort& GetSerialComPort();
        //!
        //! static CCJGW_SecureShell* GetSecureShell();
        //!
        static CCJGW_SSHShell* GetSSHShell();
        //!
        static CCJGW_SSHSFTP* GetSSHSFTP();

        static CCJGW_SSHSCP* GetSSHSCP();
        //!
        static void DestroyFTMClientSocket();
        //!
        static CCJGW_ProcessPipe* GetProcessCMDPipe();
        //!
        static bool ReadCMDProcessPipeEOF(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec = 10);
    private:
        static CCJGW_FTM_ClientSocket* mFTM_ClientSocket;
    };
}
