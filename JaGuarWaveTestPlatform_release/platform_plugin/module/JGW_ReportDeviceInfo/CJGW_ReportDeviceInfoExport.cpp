#include "StdAfx.h"
#include <JGW_ReportDeviceInfo/CJGW_ReportDeviceInfoExport.h>
#include "CJGW_ReportNetCardThread.h"
using namespace JGW;

CCJGW_ReportNetCardThread* gpReportNetCardThread = NULL;

bool StartNetCardDeviceThread(int nIntervalTimeMS/* = 200*/)
{
    if (!gpReportNetCardThread)
    {
        gpReportNetCardThread = new CCJGW_ReportNetCardThread;
        gpReportNetCardThread->CreateMessageThread(0,0,true);
    }
    Log4WI_F(L"NETCARD Start NetCard Thread Interval Time MS: %d",nIntervalTimeMS);
    gpReportNetCardThread->SetIntervalTimeMS(nIntervalTimeMS);
    return gpReportNetCardThread->IsThreadRun();
}

bool RegNetCardDeviceMsg(HWND hWindow,const wchar_t* strIPAddressSegment /* = L"192.168.1." */)
{
    if (gpReportNetCardThread)
    {
        Log4WI_F(L"NETCARD Reg NetCard Msg Window: %d,IP:%s",hWindow,strIPAddressSegment);
        return gpReportNetCardThread->RegNetCardDeviceMsg(hWindow,strIPAddressSegment);
    }
    return false;
}

bool RegNetCardDeviceThreadMsg(HANDLE hThread,const wchar_t* strIPAddressSegment/* = L"192.168.1."*/)
{
    if (gpReportNetCardThread)
    {
        Log4WI_F(L"NETCARD Reg NetCard Msg Thread: %d,IP:%s",hThread,strIPAddressSegment);
        return gpReportNetCardThread->RegNetCardDeviceThreadMsg(hThread,strIPAddressSegment);
    }
    return false;
}

void ClearNetCardSortConfig()
{
    if (gpReportNetCardThread)
    {
        Log4WI(L"NETCARD ClearNetCardSortConfig");
        gpReportNetCardThread->ClearNetCardSortConfig();
    }
}

void RemoveNetCardDeviceMsg(HWND hWindow)
{
    if (gpReportNetCardThread)
    {
        Log4WI_F(L"NETCARD Remove NetCard Msg Window:%d",hWindow);
        gpReportNetCardThread->RemoveNetCardDeviceMsg(hWindow);
    }
}

void CloseNetCardDeviceThread()
{
    if (gpReportNetCardThread)
    {
        Log4WI(L"NETCARD Close NetCard Thread");
        gpReportNetCardThread->ExitThread(true);
        gpReportNetCardThread->TerminateMessageThread();
        delete gpReportNetCardThread;
        gpReportNetCardThread = NULL;
    }
}
