// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include <TSE_FoundationLib/TSE_GlobalResources.h>
#include "TSE_Sleep.h"
#include "TSE_SendPipeCommand.h"
#include "TSE_MessageBox.h"
#include "TSE_ComparisonEnvironment.h"
#include "RFCAL_QSEQ\TSE_RFCalVFSTest.h"
#include "QCOMM/TSE_ConnectQcomDiag.h"
#include "QCOMM/TSE_DisConnectQocmDiag.h"
#include "QCOMM/TSE_ReadQcomNV.h"
#include "QCOMM/TSE_SyncQcomNV.h"
#include "QCOMM/TSE_WriteQcomNV.h"
#include "QCOMM/TSE_WriterQcomSN.h"
#include "QCOMM/TSE_WriteQualcommNumber.h"
#include "QCOMM/TSE_ReadQualcommNumber.h"
#include "QCOMM/TSE_ReaderQcomSN.h"
#include "QCOMM/TSE_QualcommFlashImageDownload.h"
#include "QCOMM/TSE_QualcommFlashImageDownloadV2.h"
#include "QCOMM/TSE_QualcommQCN.h"
#include "QCOMM/TSE_SettingFTM.h"
#include "QCOMM/TSE_SettingQualcommMode.h"
#include "QCOMM/TSE_QutsStartTcpServer.h"
#include "QCOMM/TSE_QutsStopTcpServer.h"
#include "WINDOW/TSE_InputWindows.h"
#include "COMMON/TSE_ReadEth0Mac.h"
#include "COMMON/TSE_WriteEth0MacConfig.h"
#include "COMMON/TSE_FTPUpdateFile.h"
#include "COMMON/TSE_SaveTestLog.h"
#include "COMMON/TSE_CheckUsbInterfaceStatus.h"
#include "COMMON/TSE_BuildEnvironmentValue.h"
#include "COMMON/TSE_PingTest.h"
#include "COMMON/TSE_RandomGlobalVariable.h"
#include "COMMON/TSE_CheckFileExist.h"
#include "COMMON/TSE_CheckAdapterSpeed.h"
#include "COMMON/TSE_BuildString.h"
#include "PUMA/TSE_ConnectPeraso.h"
#include "PUMA/TSE_InitPDBFileToPeraso.h"
#include "PUMA/TSE_WriteNumberToPeraso.h"
#include "PUMA/TSE_ReadNumberToPeraso.h"
#include "PUMA/TSE_DownloadPDBToPeraso.h"
#include "PUMA/TSE_ResetPeraso.h"
#include "PUMA/TSE_DisConnectPeraso.h"
#include "PUMA/TSE_SaveProductScreenLogPeraso.h"
#include "PUMA/TSE_PumaCommand.h"
#include "PUMA/TSE_PearsoRSSITest.h"
#include "PUMA/TSE_Connect1hotlinkPeraso.h"
#include "PUMA/TSE_PerasoDMGTRXTest.h"
#include "PUMA/TSE_PerasoDMGGuCalTest.h"
#include "PUMA/TSE_ConnectDMGPeraso.h"
#include "PUMA/TSE_DisConnectDMGPeraso.h"
#include "http/TSE_HttpPost.h"
#include "http/TSE_HttpPostUploadFile.h"
#include "log/TSE_BuildTestLog.h"
#include "FASTMMI/TSE_FTMClearAllTestInfo.h"
#include "FASTMMI/TSE_FTMExecuteSingleTest.h"
#include "FASTMMI/TSE_FTMVersionTest.h"
#include "FASTMMI/TSE_FTMCheckTestResult.h"
#include "FASTMMI/TSE_FTMClearTestResultFile.h"
#include "FASTMMI/TSE_FTMExecuteSingleTestResult.h"
#include "LYNX_FTM/TSE_FTM_ConnectSocket.h"
#include "LYNX_FTM/TSE_FTM_DisConnectSocket.h"
#include "LYNX_FTM/TSE_FTM_CheckVersion.h"
#include "LYNX_FTM/TSE_FTM_CheckKey.h"
#include "LYNX_FTM/TSE_FTM_LcdColorTest.h"
#include "LYNX_FTM/TSE_FTM_LcdBackLightTest.h"
#include "LYNX_FTM/TSE_FTM_CheckMemoryCard.h"
#include "LYNX_FTM/TSE_FTM_CheckSIMDetect.h"
#include "LYNX_FTM/TSE_FTM_CheckBreathingLed.h"
#include "LYNX_FTM/TSE_FTM_CheckLEDRGB.h"
#include "LYNX_FTM/TSE_FTM_HandoverWiFiMode.h"
#include "LYNX_FTM/TSE_FTM_WiFiScanAP.h"
#include "LYNX_FTM/TSE_FTM_GetEth0IPAddress.h"
#include "LYNX_FTM/TSE_FTM_QC3Test.h"
#include "LYNX_FTM/TSE_FTM_MTPTest.h"
#include "LYNX_FTM/TSE_FTM_OTGTest.h"
#include "THROUGHPUT/TSE_ConnectWiFi.h"
#include "THROUGHPUT/TSE_DisConnectWiFi.h"
#include "THROUGHPUT/TSE_CheckThroughputTestResult.h"
#include "THROUGHPUT/TSE_IperfTest.h"
#include "THROUGHPUT/TSE_ReadLynxAPInfo.h"
#include "THROUGHPUT/TSE_RemoteConnectPeraso.h"
#include "THROUGHPUT/TSE_RemoteWriteReadPeraso.h"
#include "THROUGHPUT/TSE_RemoteDisConnectPeraso.h"
#include "THROUGHPUT/TSE_PerasoSSHRSSITest.h"
#include "THROUGHPUT/TSE_CheckConnectWiFi.h"
#include "THROUGHPUT/TSE_RemoteShellExecute.h"
#include "THROUGHPUT/TSE_DisConnectWiFiSocket.h"
#include "SSH/TSE_ConnectSFTP.h"
#include "SSH/TSE_ConnectSSH.h"
#include "SSH/TSE_DisConnectSFTP.h"
#include "SSH/TSE_DisConnectSSH.h"
#include "SSH/TSE_PutFileSFTP.h"
#include "SSH/TSE_ReadWriteSSH.h"
#include "SSH/TSE_PostHttpReqRsp.h"
#include "SSH/TSE_APSReadAPInfo.h"
#include "SSH/TSE_IperfSSHTest.h"
#include "SSH/TSE_ReadAPNumber.h"
#include "SSH/TSE_GetFileSFTP.h"
#include "SSH/TSE_BuildAPBinSFTP.h"
#include "SSH/TSE_ReadAPBinNumber.h"
#include "SSH/TSE_ReadPumasSNSSH.h"
#include "SSH/TSE_ExecSSH.h"
#include "SSH/TSE_GetPerasoMibSSH.h"

#include "SSH/TSE_ConnectSFTPV2.h"
#include "SSH/TSE_DisConnectSFTPV2.h"
#include "SSH/TSE_PutFileSFTPV2.h"
#include "SSH/TSE_GetFileSFTPV2.h"


#include "Modem/TSE_OpenModemPort.h"
#include "Modem/TSE_ReadWriteModemPort.h"
#include "Modem/TSE_CloseModemPort.h"
#include "AP_NUMBER/TSE_BuildAPInfoTxt.h"
#include "AP_NUMBER/TSE_ParseConfigInfo.h"
#include "AP_NUMBER/TSE_BuildApInfoBinaryTxt.h"
#include "UC600AA/TSE_ReadStringNVX20.h"
#include "UC600AA/TSE_WriteStringNVX20.h"
#include "Serial/TSE_OpenSerialComPort.h"
//#include "Serial/TSE_ReadSerialComPort.h"
#include "Serial/TSE_ReadWriteSerialComPort.h"
//#include "Serial/TSE_WriteSerialComPort.h"
#include "Serial/TSE_CloseSerialComPort.h"
#include "Serial/TSE_LongReadWriteSerialComPort.h"

#include "ssh_scp/TSE_ConnectSSHSCP.h"
#include "ssh_scp/TSE_DisConnectSSHSCP.h"
#include "ssh_scp/TSE_GetFileSSHSCP.h"
#include "ssh_scp/TSE_PutFileSSHSCP.h"
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_StringToIEE754Float.h"
#include "TSE_DevicesGlobalResource.h"
#include "TSE_CompareStringList.h"
#include "TSE_SplitString.h"
#include "TSE_CompareFloat.h"
#include "TSE_CompareInt.h"
#include "TSE_PrintInfo.h"
#include "TSE_JsonParse.h"
#include "TSE_GetComputerName.h"
#include "TSE_GetFileName.h"
#include "TSE_CheckSocketPortOpen.h"
#include "TSE_FindString.h"
#include "TSE_GetUsbPort.h"
#include "TSE_ConsoleAppOperation.h"
#include "TSE_RecordTestTimeLog.h"
#include "TSE_FormatMACAddress.h"
#include "TSE_GetIPQVersion.h"
#include "TSE_SingleToneTxFormat.h"
#include "TSE_ContinousRxCommandFormat.h"
#include "TSE_RFConfigFormat.h"
#include "TSE_CheckUsbDeviceExists.h"
#include "TSE_BuildMESLog.h"
#include "TSE_SaveMESLog.h"
#include "TSE_WriteGuData.h"
#include "TSE_MicroAmmeterRW.h"
#include "TSE_CheckMAVoltageCurrentPower.h"
#include "TSE_SendRecvWindowMessage.h"
#include "TSE_CopyThirdPartyTestLogs.h"
#include "SOCKET/TSE_ConnectAsyncSocket.h"
#include "SOCKET/TSE_DisConnectAsyncSocket.h"
#include "SOCKET/TSE_ReadWriteSocket.h"
#include "SOCKET/TSE_XWGaugeAsyncSocket.h"
#include "COMMON/TSE_BuildDateTimeStamp.h"
#include "TSE_SearchStringRegex.h"
#include "TSE_BuildTestLogV2.h"
#include <poco/ClassLibrary.h>
/*#include "QSEQ_Debug.h"*/
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>

using namespace JGW;
POCO_BEGIN_MANIFEST(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_TestBase)
    POCO_EXPORT_CLASS(CTSE_GlobalResources)
    POCO_EXPORT_CLASS(CTSE_Sleep)
    POCO_EXPORT_CLASS(CTSE_SendPipeCommand)
    POCO_EXPORT_CLASS(CTSE_MessageBox)
    POCO_EXPORT_CLASS(CTSE_ComparisonEnvironment)
    POCO_EXPORT_CLASS(CTSE_RFCalVFSTest)
    POCO_EXPORT_CLASS(CTSE_ConnectQcomDiag)
    POCO_EXPORT_CLASS(CTSE_DisConnectQocmDiag)
    POCO_EXPORT_CLASS(CTSE_ReadQcomNV)
    POCO_EXPORT_CLASS(CTSE_SyncQcomNV)
    POCO_EXPORT_CLASS(CTSE_WriteQcomNV)
    POCO_EXPORT_CLASS(CTSE_WriterQcomSN)
    POCO_EXPORT_CLASS(CTSE_ReaderQcomSN)
    POCO_EXPORT_CLASS(CTSE_WriteQualcommNumber)
    POCO_EXPORT_CLASS(CTSE_ReadQualcommNumber)
    POCO_EXPORT_CLASS(CTSE_QualcommFlashImageDownload)
    POCO_EXPORT_CLASS(CTSE_QualcommFlashImageDownloadV2)
    POCO_EXPORT_CLASS(CTSE_QualcommQCN)
    POCO_EXPORT_CLASS(CTSE_QutsStartTcpServer)
    POCO_EXPORT_CLASS(CTSE_QutsStopTcpServer)
    POCO_EXPORT_CLASS(CTSE_SettingFTM)
    POCO_EXPORT_CLASS(CTSE_SettingQualcommMode)
    POCO_EXPORT_CLASS(CTSE_InputWindows)
    POCO_EXPORT_CLASS(CTSE_ReadEth0Mac)
    POCO_EXPORT_CLASS(CTSE_HttpPost)
    POCO_EXPORT_CLASS(CTSE_HttpPostUploadFile)
    POCO_EXPORT_CLASS(CTSE_WriteEth0MacConfig)
    POCO_EXPORT_CLASS(CTSE_BuildEnvironmentValue)
    POCO_EXPORT_CLASS(CTSE_PingTest)
    POCO_EXPORT_CLASS(CTSE_CheckFileExist)
    POCO_EXPORT_CLASS(CTSE_CheckAdapterSpeed)
    POCO_EXPORT_CLASS(CTSE_RandomGlobalVariable)
    POCO_EXPORT_CLASS(CTSE_BuildTestLog)
    POCO_EXPORT_CLASS(CTSE_FTPUpdateFile)
    POCO_EXPORT_CLASS(CTSE_SaveTestLog)
    POCO_EXPORT_CLASS(CTSE_CheckUsbInterfaceStatus)
    POCO_EXPORT_CLASS(CTSE_ConnectPeraso)
    POCO_EXPORT_CLASS(CTSE_InitPDBFileToPeraso)
    POCO_EXPORT_CLASS(CTSE_WriteNumberToPeraso)
    POCO_EXPORT_CLASS(CTSE_ReadNumberToPeraso)
    POCO_EXPORT_CLASS(CTSE_DownloadPDBToPeraso)
    POCO_EXPORT_CLASS(CTSE_ResetPeraso)
    POCO_EXPORT_CLASS(CTSE_DisConnectPeraso)
    POCO_EXPORT_CLASS(CTSE_SaveProductScreenLogPeraso)
    POCO_EXPORT_CLASS(CTSE_PumaCommand)
    POCO_EXPORT_CLASS(CTSE_Connect1hotlinkPeraso)
    POCO_EXPORT_CLASS(CTSE_PerasoDMGTRXTest)
    POCO_EXPORT_CLASS(CTSE_PerasoDMGGuCalTest)
    POCO_EXPORT_CLASS(CTSE_ConnectDMGPeraso)
    POCO_EXPORT_CLASS(CTSE_DisConnectDMGPeraso)
    POCO_EXPORT_CLASS(CTSE_PearsoRSSITest)
    POCO_EXPORT_CLASS(CTSE_BuildString)
    POCO_EXPORT_CLASS(CTSE_FTMClearAllTestInfo)
    POCO_EXPORT_CLASS(CTSE_FTMClearTestResultFile)
    POCO_EXPORT_CLASS(CTSE_FTMExecuteSingleTest)
    POCO_EXPORT_CLASS(CTSE_FTMVersionTest)
    POCO_EXPORT_CLASS(CTSE_FTMCheckTestResult)
    POCO_EXPORT_CLASS(CTSE_FTMExecuteSingleTestResult)
    POCO_EXPORT_CLASS(CTSE_FTM_ConnectSocket)
    POCO_EXPORT_CLASS(CTSE_FTM_DisConnectSocket)
    POCO_EXPORT_CLASS(CTSE_FTM_CheckVersion)
    POCO_EXPORT_CLASS(CTSE_FTM_CheckKey)
    POCO_EXPORT_CLASS(CTSE_FTM_LcdColorTest)
    POCO_EXPORT_CLASS(CTSE_FTM_LcdBackLightTest)
    POCO_EXPORT_CLASS(CTSE_FTM_CheckMemoryCard)
    POCO_EXPORT_CLASS(CTSE_FTM_CheckSIMDetect)
    POCO_EXPORT_CLASS(CTSE_FTM_CheckBreathingLed)
    POCO_EXPORT_CLASS(CTSE_FTM_CheckLEDRGB)
    POCO_EXPORT_CLASS(CTSE_FTM_HandoverWiFiMode)
    POCO_EXPORT_CLASS(CTSE_FTM_WiFiScanAP)
    POCO_EXPORT_CLASS(CTSE_FTM_GetEth0IPAddress)
    POCO_EXPORT_CLASS(CTSE_FTM_QC3Test)
    POCO_EXPORT_CLASS(CTSE_FTM_MTPTest)
    POCO_EXPORT_CLASS(CTSE_FTM_OTGTest)
    POCO_EXPORT_CLASS(CTSE_ConnectWiFi)
    POCO_EXPORT_CLASS(CTSE_DisConnectWiFi)
    POCO_EXPORT_CLASS(CTSE_CheckThroughputTestResult)
    POCO_EXPORT_CLASS(CTSE_DisConnectSFTP)
    POCO_EXPORT_CLASS(CTSE_DisConnectSSH)
    POCO_EXPORT_CLASS(CTSE_ConnectSFTP)
    POCO_EXPORT_CLASS(CTSE_ConnectSSH)
    POCO_EXPORT_CLASS(CTSE_ReadWriteSSH)
    POCO_EXPORT_CLASS(CTSE_GetPerasoMibSSH)
    POCO_EXPORT_CLASS(CTSE_PutFileSFTP)
    POCO_EXPORT_CLASS(CTSE_PostHttpReqRsp)
    POCO_EXPORT_CLASS(CTSE_APSReadAPInfo)
    POCO_EXPORT_CLASS(CTSE_IperfSSHTest)
    POCO_EXPORT_CLASS(CTSE_ReadAPBinNumber)
    POCO_EXPORT_CLASS(CTSE_ReadAPNumber)
    POCO_EXPORT_CLASS(CTSE_GetFileSFTP)
    POCO_EXPORT_CLASS(CTSE_ReadPumasSNSSH)
    POCO_EXPORT_CLASS(CTSE_BuildAPBinSFTP)
    POCO_EXPORT_CLASS(CTSE_ExecSSH)
    POCO_EXPORT_CLASS(CTSE_IperfTest)
    POCO_EXPORT_CLASS(CTSE_ReadLynxAPInfo)
    POCO_EXPORT_CLASS(CTSE_RemoteConnectPeraso)
    POCO_EXPORT_CLASS(CTSE_RemoteWriteReadPeraso)
    POCO_EXPORT_CLASS(CTSE_RemoteDisConnectPeraso)
    POCO_EXPORT_CLASS(CTSE_DisConnectWiFiSocket)
    POCO_EXPORT_CLASS(CTSE_RemoteShellExecute)
    POCO_EXPORT_CLASS(CTSE_CheckConnectWiFi)
    POCO_EXPORT_CLASS(CTSE_PerasoSSHRSSITest)
    POCO_EXPORT_CLASS(CTSE_BuildAPInfoTxt)
    POCO_EXPORT_CLASS(CTSE_BuildApInfoBinaryTxt)
    POCO_EXPORT_CLASS(CTSE_OpenSerialComPort)
    POCO_EXPORT_CLASS(CTSE_OpenModemPort)
    POCO_EXPORT_CLASS(CTSE_ReadWriteModemPort)
    POCO_EXPORT_CLASS(CTSE_CloseModemPort)
    POCO_EXPORT_CLASS(CTSE_SplitString)
    POCO_EXPORT_CLASS(CTSE_CompareStringList)
    POCO_EXPORT_CLASS(CTSE_CompareInt)
    POCO_EXPORT_CLASS(CTSE_CompareFloat)
    POCO_EXPORT_CLASS(CTSE_FindString)
    POCO_EXPORT_CLASS(CTSE_ReadStringNVX20)
    //POCO_EXPORT_CLASS(CTSE_ReadSerialComPort)
    POCO_EXPORT_CLASS(CTSE_ReadWriteSerialComPort)
    POCO_EXPORT_CLASS(CTSE_LongReadWriteSerialComPort)
    POCO_EXPORT_CLASS(CTSE_WriteStringNVX20)
    //POCO_EXPORT_CLASS(CTSE_WriteSerialComPort)
    POCO_EXPORT_CLASS(CTSE_DisConnectSSHSCP)
    POCO_EXPORT_CLASS(CTSE_StringToIEE754Float)
    POCO_EXPORT_CLASS(CTSE_GetFileSSHSCP)
    POCO_EXPORT_CLASS(CTSE_PutFileSSHSCP)
    POCO_EXPORT_CLASS(CTSE_ConnectSSHSCP)
	POCO_EXPORT_CLASS(CTSE_BuildTestLogV2)
    POCO_EXPORT_CLASS(CTSE_ConnectSFTPV2)
    POCO_EXPORT_CLASS(CTSE_DisConnectSFTPV2)
    POCO_EXPORT_CLASS(CTSE_PutFileSFTPV2)
    POCO_EXPORT_CLASS(CTSE_GetFileSFTPV2)
	POCO_EXPORT_CLASS(CTSE_CopyThirdPartyTestLogs)
    POCO_EXPORT_CLASS(CTSE_CheckSocketPortOpen)
    POCO_EXPORT_CLASS(CTSE_PrintInfo)
    POCO_EXPORT_CLASS(CTSE_JsonParse)
    POCO_EXPORT_CLASS(CTSE_GetFileName)
    POCO_EXPORT_CLASS(CTSE_GetComputerName)
    POCO_EXPORT_CLASS(CTSE_GetUsbPort)
    POCO_EXPORT_CLASS(CTSE_RecordTestTimeLog)
    POCO_EXPORT_CLASS(CTSE_ConsoleAppOperation)
    POCO_EXPORT_CLASS(CTSE_CloseSerialComPort)
    POCO_EXPORT_CLASS(CTSE_FormatMACAddress)
    POCO_EXPORT_CLASS(CTSE_ParseConfigInfo)
    POCO_EXPORT_CLASS(CTSE_GetIPQVersion)
    POCO_EXPORT_CLASS(CTSE_SingleToneTxFormat)
    POCO_EXPORT_CLASS(CTSE_ContinousRxCommandFormat)
    POCO_EXPORT_CLASS(CTSE_BuildDateTimeStamp)
    POCO_EXPORT_CLASS(CTSE_RFConfigFormat)
    POCO_EXPORT_CLASS(CTSE_CheckUsbDeviceExists)
    POCO_EXPORT_CLASS(CTSE_BuildMESLog)
    POCO_EXPORT_CLASS(CTSE_SaveMESLog)
    POCO_EXPORT_CLASS(CTSE_WriteGuData)
    POCO_EXPORT_CLASS(CTSE_MicroAmmeterRW)
    POCO_EXPORT_CLASS(CTSE_CheckMAVoltageCurrentPower)
    POCO_EXPORT_CLASS(CTSE_SendRecvWindowMessage)
    POCO_EXPORT_CLASS(CTSE_ConnectAsyncSocket)
    POCO_EXPORT_CLASS(CTSE_DisConnectAsyncSocket)
    POCO_EXPORT_CLASS(CTSE_ReadWriteSocket)
    POCO_EXPORT_CLASS(CTSE_XWGaugeAsyncSocket)
	POCO_EXPORT_CLASS(CTSE_SearchStringRegex)
POCO_END_MANIFEST

#ifdef _DEBUG
#pragma comment(lib,"TSE_FoundationLib_d.lib")
#else
#pragma comment(lib,"TSE_FoundationLib.lib")
#endif
void pocoUninitializeLibrary()
{
    CTSE_DevicesGlobalResource::DestroyFTMClientSocket();
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifdef IS_ENCRYPT_MODULE
        return JGW::EncryptModuleIsLoaded(hModule) ? TRUE : FALSE;
#endif  
    default:
        break;
    }
#ifdef QSEQ_MEMORY_LEAK_CHECK
    _CrtSetReportMode(_CRT_WARN,  _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_DETACH:
        {
            _RPT0 ( _CRT_ERROR, "***** Memory Leak Check - QSEQ_Core.dll *****\n" );
            _CrtDumpMemoryLeaks();
        }
        break;

    default:
        break;
    }
#endif
	return TRUE;


}

