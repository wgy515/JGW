#include "StdAfx.h"
#include "CJGW_DonglesStatusDlg.h"
#include "CJGW_DonglesTRxSectorThread.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#define TX_SECTOR_COMMAND "mib q b 12-8-1-c"
#define RX_SECTOR_COMMAND "mib q b 12-8-1-d"

namespace JGW
{
    boost::mutex io_mutex;

    CCJGW_DonglesTRxSectorThread::CCJGW_DonglesTRxSectorThread(void):mbReRead(true),mbExitThread(false)
    {
      
    }

    CCJGW_DonglesTRxSectorThread::~CCJGW_DonglesTRxSectorThread(void)
    {
        ExitDonglesPortThread();
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

    bool CCJGW_DonglesTRxSectorThread::OpenPerasoComPort()
    {
        std::wstring strComPort;
        while (mpsDonglesMonitroParam->mbOpenDonglesPort)
        {
            strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
            if (strComPort.empty()) { Sleep(1000); continue; }
            if (mcSerialComPort.OpenDonglesComPort(strComPort.c_str())) break;
            Sleep(1000);
        }        
        return true;
    }

    void CCJGW_DonglesTRxSectorThread::OnRunTRxSectorThread(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsDonglesMonitroParam = (LPS_DONGLES_MONITOR_PARAM)lParam;
        while (!mbExitThread)
        {
            if (!mpsDonglesMonitroParam->mbOpenDonglesPort) { Sleep(1000);continue;}
            if (!OpenPerasoComPort()) continue;
            OnReportTRxSectorInfo();  
            mpsDonglesMonitroParam->mstrFW = L"";
            mpsDonglesMonitroParam->mstrMac = L"";
            mpsDonglesMonitroParam->mstrSN = L"";
            mpsDonglesMonitroParam->mstrUsbSpeed = L"";
            SendMessage(mpsDonglesMonitroParam->mhMainWnd,WM_HWND_UPDATE_DONGLES_INFO,NULL,NULL);
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
        mcSerialComPort.ReadRawWriteCommands(strCommand.c_str(),strRead);
        std::wstring strTemp = JGW_A2W(strRead);
        
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_A2W(strCommand).c_str(),NULL);
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strTemp.c_str(),NULL);
    }

    bool CCJGW_DonglesTRxSectorThread::ReadDonglesInfo()
    {
        if (!mbReRead) return true;
        if (!mcSerialComPort.IsOpenDonglesComPort()) return false;
        //! 重新打开端口的时候，重新获取dongles信息
        mpsDonglesMonitroParam->mstrFW = L"";
        mpsDonglesMonitroParam->mstrMac = L"";
        mpsDonglesMonitroParam->mstrSN = L"";
        mpsDonglesMonitroParam->mstrUsbSpeed = L"";

        mcSerialComPort.ReadFirmwareVersion(mpsDonglesMonitroParam->mstrFW);
        mcSerialComPort.ReadMacAddress(mpsDonglesMonitroParam->mstrMac);
        mcSerialComPort.ReadSerialNumber(mpsDonglesMonitroParam->mstrSN);
        mcSerialComPort.ReadUsbSpeed(mpsDonglesMonitroParam->mstrUsbSpeed);

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
        std::string strRead;
        mbReRead = true;

        while (mpsDonglesMonitroParam->mbOpenDonglesPort)
        {
            if (!mcSerialComPort.IsOpenDonglesComPort()) return;
            //! 读取Dongles相关信息
            SendCommandMsg();
            ReadDonglesInfo();
            Sleep(1000);
            if (!mcSerialComPort.ReadWriteCommands("\r",strRead)) mcSerialComPort.CloseDonglesComPort();
            /*
            //! 如果读取Sector失败则可能是Sector端口已经丢失
            if (!mcSerialComPort.ReadSectorStructToRxSectorCommand(wstrRxSector)
                || !mcSerialComPort.ReadSectorStructToTxSectorCommand(wstrTxSector))
            {
                mcSerialComPort.CloseDonglesComPort();
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
            }*/
        }
    }
}