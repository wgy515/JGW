// JGW_QutsDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>
#include "usb/usbioctl.h"
#include <algorithm>
#include <regex>

HANDLE g_hResourceContext;
static bool ConnectStandaloneWlanCardQUTS(std::string strCom)  // Deafult to use
{
    const unsigned int maxListSize = 10;
    unsigned int iNumListSize = maxListSize;
    long long devHandleIdList[maxListSize];
    long long protocolIdList[maxListSize];
    unsigned int index, i;

    char* pDescriptionList[maxListSize];

    unsigned int iDescriptionLength = 200;
    unsigned int iErrorCode = 0;
    for (unsigned int i = 0; i < iNumListSize; i++)
    {
        pDescriptionList[i] = new char[iDescriptionLength];
        if (pDescriptionList[i])
        {
            memset(pDescriptionList[i], 0, sizeof(char)*iDescriptionLength);
        }
    }
    QLIB_SetLibraryMode(QLIB_LIB_MODE_QUTS);
    QLIB_SetTargetType(QLIB_TARGET_TYPE_MSM_MDM);

    unsigned char bOK =  QLIB_GetAvailablePhonesHandleIDList((QUTS_PROTOCOL_TYPE)0, &iNumListSize,
        devHandleIdList,
        protocolIdList,
        &iDescriptionLength, pDescriptionList,
        &iErrorCode);

    if (bOK)
    {
        for (i = 0; i < iNumListSize; i++)
        {
            std::string description(pDescriptionList[i]);
            if(description.find(strCom) != std::string::npos)
            { index = i;
            break;
            }
        }


        g_hResourceContext = QLIB_ConnectServerWithWaitByHandleID((QUTS_PROTOCOL_TYPE)0, devHandleIdList[index], protocolIdList[index], 2000);

    }

    if (g_hResourceContext == NULL)
        return false;

    //CaptureQMSLTraceLog();
    return true;

}

/// Control code for IOCTL that gets device serial number
#define ADB_CTL_GET_SERIAL_NUMBER                 16

/// IOCTL that gets device serial number
#define ADB_IOCTL_GET_SERIAL_NUMBER \
    CTL_CODE(FILE_DEVICE_UNKNOWN, \
    ADB_CTL_GET_SERIAL_NUMBER, \
    METHOD_BUFFERED, \
    FILE_READ_ACCESS)

int _tmain(int argc, _TCHAR* argv[])
{
    double freqErrPPM = 0.518;
    int mXtalTargetPPM = 0,mXtalTolerancePPM = 1;
    double min = mXtalTargetPPM - mXtalTolerancePPM;
    if (freqErrPPM < (double)(mXtalTargetPPM - mXtalTolerancePPM) || freqErrPPM > (double)(mXtalTargetPPM + mXtalTolerancePPM))
    {
        return 0;
    }
    min = (double)(mXtalTargetPPM - mXtalTolerancePPM);
    bool result = freqErrPPM < (double)(mXtalTargetPPM - mXtalTolerancePPM);
    result = freqErrPPM > (double)(mXtalTargetPPM + mXtalTolerancePPM);
        //if (freqErrPPM < (double)(mXtalTargetPPM - mXtalTolerancePPM)
        //
        //    || freqErrPPM > (double)(mXtalTargetPPM + mXtalTolerancePPM))


    //std::regex reg("([0-9]{1,3}[.]){3}[0-9]{1,3}");
    //std::regex pattern("((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
    std::regex ipPattern("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
    if (regex_search("2500->::ffff:192.168.1.1", ipPattern))
    {
        return 0;
    }

    return 1;







    int a = 100;
    a = 200;
    int *p = &a;
    char syncPkt[]  =
        "\x00\x08\x24\x00\x00\x00\x00\x00\x07\x07\x12\x20"
        "UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU";

    double pressure = 1003.00 * 100.0;
    double temperature = 29.0;
    double alt = ((std::pow((101325 / pressure), 1.00/5.257) - 1.0) * (temperature + 273.15)) / 0.0065;

#if 0
    unsigned int isss = ADB_IOCTL_GET_SERIAL_NUMBER;
    std::wstring strCom(L"\\\\.\\COM8");
    //JGW_FormatWString(strCom,L"\\\\.\\COM%d",6);
   HANDLE hFile = CreateFile (strCom.c_str() ,GENERIC_WRITE | GENERIC_READ,0,0, OPEN_EXISTING ,NULL,0);
   DCB dcb;
   dcb.DCBlength =sizeof( DCB);
   GetCommState (hFile,&dcb);

   char lpBuffer[256] = {0};
   DWORD nNumberOfBytesToRead = 256;
   DWORD nNumberOfBytesRead = 0;
   char* command = "OUTP ON\r\n";
   
   bool result = (TRUE == WriteFile(hFile,command,strlen(command),&nNumberOfBytesRead,NULL));
   result = (TRUE == ReadFile(hFile,lpBuffer,nNumberOfBytesToRead,&nNumberOfBytesRead,NULL));
   //mSerialPort.GetControlCommState(&dcb);

   CloseHandle(hFile);
#else
    unsigned char status = QLIB_QUTS_Start_TCP_Server(0,2500);
    //status = QLIB_QUTS_Start_TCP_Server(0,5000);
    QLIB_SetLibraryMode(QLIB_LIB_MODE_QUTS);
    QLIB_SetTargetType(QLIB_TARGET_TYPE_MSM_MDM);
    unsigned short iNumPorts = 30,pPortList[30] = {0},iNumIgnorePorts = 0,pIgnorePortList[1] = {0},_iItemStatus = 0;
    unsigned char deviceinfo[1024] = {0};
    unsigned int deviceinfo_size = 1024;
    unsigned int error_code = 0;
    iNumPorts = 30;

    ConnectStandaloneWlanCardQUTS("192.168.225.1");

    unsigned int iNumListSize[30] = {0};
     QUTS_DEVICE_HANDLE_ID_TYPE iDeviceHandleList[30];
        QUTS_PROTOCOL_ID_TYPE iProtocolHandleList[30];
        unsigned int iDescriptionLength = 256;
        char pDescriptionList[1024] = {0};
        unsigned int iErrorCode;
        while (1)
        {
            if (QLIB_GetAvailablePhonesHandleIDList(0,iNumListSize,iDeviceHandleList,iProtocolHandleList,&iDescriptionLength,(char**)&pDescriptionList,&iErrorCode) == 0 || iNumListSize == 0)
            {
                Sleep(1000);
            }
            else
            {
                break;
            }
        }
    


    //PELOG4WW_F(L"Start QLIB_GetAvailablePhonesPortList:%u",GetTickCount());
    if ( !QLIB_GetAvailablePhonesPortList(&iNumPorts,pPortList,iNumIgnorePorts,pIgnorePortList) )
    {
        //PELOG4WW_F(L"Get Error QLIB_GetAvailablePhonesPortList:%u",GetTickCount());
        Sleep(1000);
    }
    if (iNumPorts > 0)
    {
        HANDLE hResourceHandle = QLIB_ConnectServer(pPortList[0]);

        QLIB_QUTS_Get_Device_Info(hResourceHandle,&deviceinfo_size,deviceinfo,&error_code);
        QLIB_DisconnectServer(hResourceHandle);
    }
#endif

    //QLIB_QUTS_Stop_TCP_Server(5000);
	return 0;
}

