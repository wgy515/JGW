#include "StdAfx.h"
#include "CJGW_DonglesMonitorDlg.h"
#include "CJGW_DonglesTRxSectorThread.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_WindowsFuncPlugin/CJGW_PerasoSocUsb.h>
#include <JGW_SSHPlugin/CJGW_DonglesSSH.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#define TX_SECTOR_COMMAND "mib q b 12-8-1-c"
#define RX_SECTOR_COMMAND "mib q b 12-8-1-d"

namespace JGW
{
    boost::mutex io_mutex;

    CCJGW_DonglesTRxSectorThread::CCJGW_DonglesTRxSectorThread(void):mbReRead(true),mbExitThread(false),mnPrevRssi(-128),mnPrevMcs(-1)/*,mcDonglesComPort(mcSerialComPort)*/,mpPerasoSocImp(NULL),mnPrevLocalRssi(-128)
    {
     
    }

    CCJGW_DonglesTRxSectorThread::~CCJGW_DonglesTRxSectorThread(void)
    {
        ExitDonglesPortThread();
        TerminateMessageThread();
        if (mpPerasoSocImp)
        {
            delete mpPerasoSocImp;
            mpPerasoSocImp = NULL;
        }
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_DonglesTRxSectorThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_DonglesTRxSectorThread::OnRunTRxSectorThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_DonglesTRxSectorThread::SendCommandToDonglesPort(const char* strCommand)
    {
        boost::mutex::scoped_lock lock(io_mutex);
        mqueue_buffer.push(strCommand);
    }

    void CCJGW_DonglesTRxSectorThread::ExitDonglesPortThread()
    {
        ExitThread(false);
        mpsDonglesMonitroParam->mbOpenDonglesPort = false;
        Sleep(1500);
        mbExitThread = true;
    }

    void CCJGW_DonglesTRxSectorThread::CreateLogFile()
    {
        std::wstring strLogFilePath;
        strLogFilePath = JGW_GetApplicationFolder(); 
        strLogFilePath += JGW_GetFileNameToCurrentTimeStamp();
        strLogFilePath += L"\\";
        JGW_CreateDirectory(strLogFilePath.c_str(),true);
        strLogFilePath += L"trxsector.log";

        mFileStream.imbue( std::locale("chs") );
        mFileStream.open( strLogFilePath.c_str(),std::ios_base::out | std::ios_base::trunc );

        if (!mFileStream.is_open())
            MessageBox(mpsDonglesMonitroParam->mhMainWnd,L"无法保存LOG文件",L"警告",MB_ICONWARNING);
    }

    bool CCJGW_DonglesTRxSectorThread::OpenPerasoComPort()
    {
        std::wstring strComPort;
        while (mpsDonglesMonitroParam->mbOpenDonglesPort)
        {
            if (!mpsDonglesMonitroParam->mbPerasoSocImpSSH)
            {
                strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
                if (strComPort.empty()) { Sleep(1000); continue; }
                if (mpPerasoSocImp->OpenPerasoSocImpToUsb(strComPort.c_str())) break;
            }
            else
            {
                if (mpPerasoSocImp->OpenPerasoSocImpToSSH(mpsDonglesMonitroParam->mstrHost,mpsDonglesMonitroParam->mstrSourceAddr,mpsDonglesMonitroParam->mnSSHPort,mpsDonglesMonitroParam->mstrUserName,mpsDonglesMonitroParam->mstrUserPassword)) 
                    break;
            }
            
            Sleep(1000);
        }        
        return true;
    }

    void CCJGW_DonglesTRxSectorThread::OnRunTRxSectorThread(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsDonglesMonitroParam = (LPS_DONGLES_MONITOR_PARAM)lParam;
        if (!mpsDonglesMonitroParam->mbPerasoSocImpSSH)
        {
            mpPerasoSocImp = new CCJGW_PerasoSocUsb(mcSerialComPort);
        }
        else
        {
            mpPerasoSocImp = new CCJGW_DonglesSSH(mcSSHShell);
        }
        CreateLogFile();
        while (!mbExitThread)
        {
            if (!mpsDonglesMonitroParam->mbOpenDonglesPort) { Sleep(1000);continue;}
            if (!OpenPerasoComPort()) continue;
            OnReportTRxSectorInfo();
        }
    }

    void CCJGW_DonglesTRxSectorThread::SendCommandMsg()
    {
        std::string strCommand,strRead;
        {
            boost::mutex::scoped_lock lock(io_mutex);
            if (mqueue_buffer.empty()) return ;
            strCommand = mqueue_buffer.front();
            mqueue_buffer.pop();
        }
        JGW_EraseLastAndFristTwoChars(strCommand);
        mpPerasoSocImp->ReadRawWriteCommands(strCommand.c_str(),strRead);
        std::wstring strTemp = JGW_A2W(strRead);
        
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_A2W(strCommand).c_str(),NULL);
        if (mpsDonglesMonitroParam->mbPerasoSocImpSSH)
        {
            mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,L"\r\n",NULL);
            strTemp += (L"\r\n>");
        }         
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str(),NULL);
    }

    bool CCJGW_DonglesTRxSectorThread::ReadDonglesInfo()
    {
        if (!mbReRead) return true;
        if (mpsDonglesMonitroParam->mbPerasoSocImpSSH)
        {
            if (!mpPerasoSocImp->IsOpenPerasoSocImpToSSH()) return false;
        }
        else
        {
             if (!mpPerasoSocImp->IsOpenPerasoSocImpToUsb()) return false;
        }
       
        //! 重新打开端口的时候，重新获取dongles信息
        mpsDonglesMonitroParam->mstrFW = L"";
        mpsDonglesMonitroParam->mstrMac = L"";
        mpsDonglesMonitroParam->mstrSN = L"";
        mpsDonglesMonitroParam->mstrUsbSpeed = L"";

        mpPerasoSocImp->ReadFirmwareVersion(mpsDonglesMonitroParam->mstrFW);
        mpPerasoSocImp->ReadMacAddress(mpsDonglesMonitroParam->mstrMac);
        mpPerasoSocImp->ReadSerialNumber(mpsDonglesMonitroParam->mstrSN);
        mpPerasoSocImp->ReadUsbSpeed(mpsDonglesMonitroParam->mstrUsbSpeed);

        if (mpsDonglesMonitroParam->mstrFW.empty()
            || mpsDonglesMonitroParam->mstrMac.empty()
            || mpsDonglesMonitroParam->mstrSN.empty()
            || mpsDonglesMonitroParam->mstrUsbSpeed.empty())
        {
            return false;
        }
        mbReRead = false;
        SendMessage(mpsDonglesMonitroParam->mhMainWnd,WM_HWND_UPDATE_DONGLES_INFO,NULL,NULL);

        return true;
    }

    void CCJGW_DonglesTRxSectorThread::OnReportTRxSectorInfo()
    {
        static std::wstring gstrPrevTxSector,gstrPrevRxSector;
        std::wstring wstrTxSector,wstrRxSector;
        int nCount = 1;
        mbReRead = true;

        while (mpsDonglesMonitroParam->mbOpenDonglesPort)
        {
            if (mpsDonglesMonitroParam->mbPerasoSocImpSSH)
            {
                if (!mpPerasoSocImp->IsOpenPerasoSocImpToSSH()) return ;
            }
            else
            {
                if (!mpPerasoSocImp->IsOpenPerasoSocImpToUsb()) return ;
            }
            //! 读取Dongles相关信息
            SendCommandMsg();
            ReadDonglesInfo();
			UpdateRssiInfo();
            UpdateMcsInfo();

            //! 如果读取Sector失败则可能是Sector端口已经丢失
            if (!mpPerasoSocImp->ReadSectorStructToRxSectorCommand(wstrRxSector)
                || !mpPerasoSocImp->ReadSectorStructToTxSectorCommand(wstrTxSector))
            {
                mpPerasoSocImp->ClosePerasoSocImpToSSH();
                mpPerasoSocImp->ClosePerasoSocImpToUsb();
                gstrPrevTxSector = L"";
                gstrPrevRxSector = L"";
                mpAsyncWndMessage->PutAsyncMessage(WM_HWND_TX_RX_SECTOR_MSG);
                break;
            }
            nCount ++;
            //! 如果相等则不上报
            if (NULL != gstrPrevTxSector.compare(wstrTxSector)
                || NULL != gstrPrevRxSector.compare(wstrRxSector))
            {
                gstrPrevTxSector = wstrTxSector;
                gstrPrevRxSector = wstrRxSector;
                wstrTxSector += L"-";
                wstrTxSector += wstrRxSector;
                if (mFileStream.is_open()) mFileStream << JGW_GetCurrentTimeStamp() << L" " << gstrPrevTxSector << L"\t" << gstrPrevRxSector << std::endl;
                mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TX_RX_SECTOR_MSG,wstrTxSector.c_str(),NULL);
            }
            //! 串口读取一段时间后Sleep下
            if (0 == nCount % 100)
            {
                Sleep(20);
                nCount = 1;
            }
        }
    }

    void CCJGW_DonglesTRxSectorThread::UpdateMcsInfo()
    {
        //! mnPrevRssi
        int mcs = 0;
        if (!mpPerasoSocImp->ReadMcs(mcs)) return ;
        if (mcs == mnPrevMcs) return;
        mnPrevMcs = mcs;
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_UPDATE_MCS_INFO,WPARAM(mnPrevMcs));
    }

    void CCJGW_DonglesTRxSectorThread::UpdateRssiInfo()
    {
        //! mnPrevRssi
        int rssi = 0;

        if (mpPerasoSocImp->ReadOmniRssi(rssi))
        {
            if (rssi != mnPrevRssi)
            {
                mnPrevRssi = rssi;
                mpAsyncWndMessage->PutAsyncMessage(WM_HWND_UPDATE_RSSI_INFO,WPARAM(mnPrevRssi),LPARAM(0));
            } 
        }
        
        if (mpPerasoSocImp->ReadRssiToCommand("mib q b 12-8-1-9\n",rssi))
        {
            if (rssi != mnPrevLocalRssi)
            {
                mnPrevLocalRssi = rssi;
                mpAsyncWndMessage->PutAsyncMessage(WM_HWND_UPDATE_RSSI_INFO,WPARAM(mnPrevLocalRssi),LPARAM(1));
            } 
        }

    }
}