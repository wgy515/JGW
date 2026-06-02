#include "StdAfx.h"
#include "TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/ctrim.h>
namespace JGW
{
    CCJGW_CSerialComPort gcSerialComPort;
    CCJGW_FTM_ClientSocket* CTSE_DevicesGlobalResource::mFTM_ClientSocket = NULL;
    eDeviceBootMode geDeviceBootMode = eDeviceBootMode_Unknown;
    static CCJGW_ProcessPipe gProcessPipe;
    jgw_wr_nv_api* gpQcommWRNVApi = NULL;

    jgw_wr_nv_api* CTSE_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(HANDLE hResourceContext/* = NULL*/,unsigned char chUseQPST/* = FALSE*/)
    {
        static unsigned char gOldchUseQPST = FALSE;
        if (gOldchUseQPST != chUseQPST)
        {
            GetQcommDiagActionIsntance()->DisConnectDevice();
            GetQcommDiagActionIsntance()->InitializeQMSL(chUseQPST);
            gOldchUseQPST = chUseQPST;
        }
        return GetQcommDiagActionIsntance();
    }

    CTSE_DevicesGlobalResource::~CTSE_DevicesGlobalResource()
    {
        DestroyFTMClientSocket();
    }

    jgw_wr_nv_api* CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()
    {
        if (NULL == gpQcommWRNVApi)
        {
            gpQcommWRNVApi = new jgw_wr_nv_api();
        }
        //! if (!gpQcommWRNVApi) gpQcommWRNVApi = new jgw_wr_nv_api(NULL,false);
        return gpQcommWRNVApi;
    }

    void CTSE_DevicesGlobalResource::SettingPrsDeviceBootMode(eDeviceBootMode deviceBootMode)
    {
        geDeviceBootMode = deviceBootMode;
    }

    CCJGW_PrsFlashDownloadBase* CTSE_DevicesGlobalResource::GetDownloadPDBInstance()
    {
        static CCJGW_DownloadPDBImage gDownloadPDBImage(gcSerialComPort);
        if (eDeviceBootMode_BootLoader == geDeviceBootMode || eDeviceBootMode_Unknown == geDeviceBootMode)
        {
            return GetPrsDownloadFlashInstance();
        }
        return &gDownloadPDBImage;
    }

    CCJGW_DonglesComPort* CTSE_DevicesGlobalResource::GetDonglesComPortInstance()
    {
        static CCJGW_DonglesComPort gDonglesComPort(gcSerialComPort);
        return &gDonglesComPort;
    }

    CCJGW_PrsDownloadFlash* CTSE_DevicesGlobalResource::GetPrsDownloadFlashInstance()
    {
        static CCJGW_PrsDownloadFlash prsDownloadFlash(gcSerialComPort);
        return &prsDownloadFlash;
    }

    //     CCJGW_SecureShell* CTSE_DevicesGlobalResource::GetSecureShell()
    //     {
    //         static CCJGW_SecureShell gSecureShell;
    //         return &gSecureShell;
    //     }

    CCJGW_SSHSFTP* CTSE_DevicesGlobalResource::GetSSHSFTP()
    {
        static CCJGW_SSHSFTP gSSHSFTP;
        return &gSSHSFTP;
    }

    CCJGW_SSHSCP* CTSE_DevicesGlobalResource::GetSSHSCP()
    {
        static CCJGW_SSHSCP gSSHSCP;
        return &gSSHSCP;
    }

    CCJGW_SSHShell* CTSE_DevicesGlobalResource::GetSSHShell()
    {
        static CCJGW_SSHShell gSSHShell;
        return &gSSHShell;
    }

    CCJGW_CSerialComPort& CTSE_DevicesGlobalResource::GetSerialComPort()
    {
        return gcSerialComPort;
    }

    CCJGW_FTM_ClientSocket* CTSE_DevicesGlobalResource::GetFTMClientSocket()
    {
        if (NULL == mFTM_ClientSocket) mFTM_ClientSocket = new CCJGW_FTM_ClientSocket;
        return mFTM_ClientSocket;
    }

    void CTSE_DevicesGlobalResource::DestroyFTMClientSocket()
    {
        if (NULL != gpQcommWRNVApi)
        {
            delete gpQcommWRNVApi;
            gpQcommWRNVApi = NULL;
        }
        if (mFTM_ClientSocket)
        {
            delete mFTM_ClientSocket;
            mFTM_ClientSocket = NULL;
        }
        if (gProcessPipe.CheckProcessIsRun())
        {
            gProcessPipe.CloseProcessPipe();
        }
    }

    bool CTSE_DevicesGlobalResource::ReadCMDProcessPipeEOF(CCJGW_ProcessPipe* pProcessPipe,std::wstring& strRead,size_t nTimeOutSec /* = 10 */)
    {
        if (!pProcessPipe) return false;
        strRead = L"";
        int readSize = 0;
        jgw_timer tm;
        std::wstring strTemp;

        while(tm.elapsed() < nTimeOutSec)
        {
            readSize = pProcessPipe->recv(strTemp);
            if( -1 == readSize )
            {
                LogE(_T("error: process closed!!!"));
                return false;
            }
            else if ( readSize > 0 )
            {
                //trim::ctrim(strTemp);  //! JGW_ReplaceStringW(strTemp,_T("\r"),_T("\t")); //! JGW_ReplaceStringW(strTemp,_T("\n"),_T("\t"));
                if( strTemp.length() <= 0 ) continue;
                strRead += strTemp;
                LogI_F(_T("%s"),strTemp.c_str());
                if (strRead.length() > 1 && (strRead.at(strRead.length() - 1) == _T('>') || strRead.at(strRead.length() - 1) == _T('$') || strRead.at(strRead.length() - 1) == _T('#')))
                {
                    return true;
                }
            }
        }
        if (strTemp.length() > 0)
        {
            return true;
        }
        pProcessPipe->CloseProcessPipe();
        GetProcessCMDPipe();
        return false;
    }

    CCJGW_ProcessPipe* CTSE_DevicesGlobalResource::GetProcessCMDPipe()
    {
        if (gProcessPipe.CheckProcessIsRun()) return &gProcessPipe;

        std::wstring strApplicationName,strApplicationFilePath;
        strApplicationName = GetGlobalEnvironment()->GetString(L"ApplicationCMD");
        if (strApplicationName.empty()) strApplicationName = L"cmd.exe";
        strApplicationFilePath = JGW_RealativePathToAbsPath(strApplicationName.c_str());

        if (!gProcessPipe.InitProcessPipe(strApplicationFilePath.c_str(),L"",JGW_GetApplicationFolder())) { LogE(L"初始化CMD进程失败\n"); return NULL;}
        ReadCMDProcessPipeEOF(&gProcessPipe,strApplicationName);

        return &gProcessPipe;
    }
}

