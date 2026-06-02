// stdafx.cpp : 只包括标准包含文件的源文件
// JGWPerasoComPort.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <conio.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <iostream>
#include <JGW_FoundationFunc/JGW_PrsDownloadProtocol.h>
#include <JGW_WindowsFuncPlugin/CJGW_CSerialModemPort.h>
//#include "CSTSE_DesCrypto.h"

#include <JGW_QMSLFuncPlugin/jgw_device_api.h>
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用


//! Resource context used for all calls in this file
HANDLE g_hResourceContext = NULL;

//! Flag to indicate whether async message callback should print things to the screen
unsigned char g_bAsyncCallbackVerbose = 0;

//! Flag to indicate whether text message callback should print things to the screen
unsigned char g_bTextCallbackVerbose = 0;
void asyncMessageCallBack
    (
    unsigned short iMessageSize,
    unsigned char* iMessageBuffer,
    HANDLE hContextID
    )
{
    // If verbose is enabled, then print message
    if ( g_bAsyncCallbackVerbose )
    {
        printf("\nAsync message received by C callback.  Context = %d, Size = %d", hContextID, iMessageSize );

        LogMessage* _pLogMessage = (LogMessage*) iMessageBuffer;

        // First check to see if it is a log command response
        if ( _pLogMessage->CMD_CODE == _DIAG_LOG_F )
        {
            // Now look for the specical QLIB log type
            if ( _pLogMessage->log_item == QLIB_TEXT_LOG_CODE )
            {
                // Decode the text log structure
                QMSL_TextLog_struct* _pTextLog = (QMSL_TextLog_struct*) _pLogMessage->iLogMsgData;

                // Print the log type
                printf("\n  QMSL Text Log found...");
                printf("\n      Log Type: %d", _pTextLog->iLogLevel );
                printf("\n      Log Text: %s", _pTextLog->sLogText  );
            }  //  if ( _pLogMessage->log_item == QLIB_TEXT_LOG_CODE )
        } // if ( _pLogMessage->CMD_CODE == _DIAG_LOG_F )

        // Decode normal text messages
    }
}

//! Callback for generic download events
unsigned char generalDownloadEventCallBack
    (
    unsigned char* pGeneralSwDownloadEvent_union
    )
{



    return 1;
}

// Token: 0x0200000B RID: 11
enum DiagType
{
    // Token: 0x040000B0 RID: 176
    QLIB_DIAG_TYPE_STANDARD,
    // Token: 0x040000B1 RID: 177
    QLIB_DIAG_TYPE_LITE,
    // Token: 0x040000B2 RID: 178
    QLIB_DIAG_TYPE_MAX_INVALID
};

int _tmain(int argc, _TCHAR* argv[])
{
    // This the default COM port, but a COM port can be specified as a command line parameter.
    unsigned short iComPort = 30001;
    unsigned short iTargetType = QLIB_TARGET_TYPE_MSM_MDM;
    unsigned char bContinue = true;      // Variable for menu
    char cKey = ' ';                 // Key pressed in menu
    unsigned short iConnectServerWithHandShake = 0; // 0 = QLIB_ConnectServer, 1 = QLIB_ConnectServerWithHandShake

    char sLibraryVersion[50];

    printf("\n QLIB Demo--No COM port specified from command line!\n\nPlease enter the COM port (-1 = auto detect), or enter -2 for Femto tool, or enter -3 for WLAN x86, or enter -4 for Bluetooth x86 #: ");
     scanf("%hd", & iComPort );

    // This will get filled out based on command line arguments
    unsigned short _bUserSelectQPST_short = true;
    unsigned char _bUserSelectQPST = true;

    // These will get filled in by the library interface
    unsigned char _bSupportsDiag = false;
    unsigned char _bSupportsEFS = false;
    unsigned char _bSupportsSwDownload = false;
    unsigned char _bUsingQPST = false;

    unsigned short iNumPorts = 30,pPortList[30] = {0},iNumIgnorePorts = 0,pIgnorePortList[1] = {0},_iItemStatus = 0;
    QLIB_SetLibraryMode(QLIB_LIB_MODE_QPST);
    unsigned char sss = QLIB_GetAvailablePhonesPortList(&iNumPorts,pPortList,iNumIgnorePorts,pIgnorePortList);
    printf("result %d,iNumPorts：%d\n",sss,iNumPorts);
    g_hResourceContext  = QLIB_ConnectServer(QLIB_COM_AUTO_DETECT);
    printf("g_hResourceContext：%d\n",g_hResourceContext);
    QLIB_DisconnectServer(g_hResourceContext);
    return -1;

    // Set the library mode--QPST or QPHONEMS
    QLIB_SetLibraryMode( _bUserSelectQPST ? QLIB_LIB_MODE_QPST : QLIB_LIB_MODE_QPHONEMS );

    // Set TargetType
    QLIB_SetTargetType((unsigned char)iTargetType);

    QLIB_SetDiagType(QLIB_DIAG_TYPE_STANDARD);

    // Get the library version
    QLIB_GetLibraryVersion(sLibraryVersion);

    printf("QLibrary Demo\n\n");
    printf("QLIB DLL Version: %s\n\n", sLibraryVersion );

    if ( iComPort == QLIB_COM_AUTO_DETECT )
        printf("User Requested COM Port: AUTO\n" );
    else if ( iComPort == QLIB_COM_NULL )
        printf("User Requested COM Port: NULL\n" );
    else
        printf("User Requested COM Port: %d\n", iComPort );

    // Get the capabilities
    QLIB_GetLibraryCapabilities( &_bSupportsDiag, &_bSupportsEFS, &_bSupportsSwDownload, &_bUsingQPST );

    printf("\n       Supports DIAG: %s", _bSupportsDiag          ? "YES" : "NO" );
    printf("\n        Supports EFS: %s", _bSupportsEFS           ? "YES" : "NO" );
    printf("\nSupports SW Download: %s", _bSupportsSwDownload    ? "YES" : "NO" );
    printf("\n          Using QPST: %s", _bUsingQPST         ? "YES" : "NO" );

    //unsigned short iNumPorts = 30,pPortList[30] = {0},iNumIgnorePorts = 0,pIgnorePortList[1] = {0},_iItemStatus = 0;

    sss = QLIB_GetAvailablePhonesPortList(&iNumPorts,pPortList,iNumIgnorePorts,pIgnorePortList);

    // Connect to the desired COM port
    if(  iConnectServerWithHandShake == 0)
        // Brew, Linux, Windows Mobile full boot target
    {
        g_hResourceContext  = QLIB_ConnectServer( iComPort );
    }
    else
        // Win Mobile boots into FTM mode directly
    {
        g_hResourceContext  = QLIB_ConnectServerWithHandShake( iComPort );
    }


    //if ( _bUsingQPST)
    //{
    //    // Configure call back functions
    //    QLIB_ConfigureCallBacks( g_hResourceContext, NULL, generalDownloadEventCallBack, asyncMessageCallBack  );
    //}
    //else
    //{
    //    // Configure call back functions
    //    QLIB_ConfigureCallBacks( g_hResourceContext, NULL, NULL, asyncMessageCallBack  );
    //}
    unsigned long iPhoneOperatingMode = static_cast<unsigned long>(SYS_OPRT_MODE_NONE);//init to invalid value
    unsigned char result = QLIB_GetPhoneOperatingMode( g_hResourceContext, &iPhoneOperatingMode );

    if (g_hResourceContext)
    {
        QLIB_DisconnectServer(g_hResourceContext);
    }

    return 0;
}