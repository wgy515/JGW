#include "StdAfx.h"
#include <JGW_QMSLFuncPlugin\JGW_device_api.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    unsigned char jgw_device_api::mbUseQPST = eQPhoneMS;
    int jgw_device_api::m_nNextBootModeTemp = 1;
    bool jgw_device_api::m_bInitQMSL = false;
    bool gbInitQutsTcp = false;
    jgw_device_api::jgw_device_api(HANDLE hResourceContext,unsigned char chUseQPST):m_hResourceContext(hResourceContext)
    {
        if ( !m_bInitQMSL ) InitializeQMSL( chUseQPST );
        mbUseQPST = chUseQPST;
    }


    jgw_device_api::~jgw_device_api(void)
    {
        TerminateQPSTServer();
    }

    void jgw_device_api::TerminateQPSTServer(void)
    {
        if ( eQPSTMode == mbUseQPST )
        {
            QLIB_TerminateQPSTServer();
            HANDLE   hp = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE,FALSE,GetCurrentProcessId());   
            if (hp) TerminateProcess(hp,0);
        }
    }

    void jgw_device_api::DisconnectServerSahara(void)
    {
        if ( m_hResourceContext )
        {
            QLIB_DisconnectServer_Sahara(m_hResourceContext);
            m_hResourceContext = NULL;
        }
    }

    bool jgw_device_api::QUTS_Start_TCP_Server(int type, unsigned int port)
    {
        if (gbInitQutsTcp)
        {
            return true;
        }

        if (1 == QLIB_QUTS_Start_TCP_Server(type,port))
        {
            gbInitQutsTcp = true;
            return true;
        }
        return false;
    }

    void jgw_device_api::QUTS_Stop_TCP_Server(unsigned int port)
    {
        if (gbInitQutsTcp)
        {
            QLIB_QUTS_Stop_TCP_Server(port);
            gbInitQutsTcp = false;
        }
    }

    bool jgw_device_api::ConnetSaharaPort(unsigned int& iComPort,swdlQPHONEMSCB pSWDL_QPHONEMS_CB /* = NULL */)
    {
        unsigned long version = 0 ,sn = 0,ulmsid = 0,ultimeout = 0;
        int nmode = 0;
        char pkHash[200] = {0};
        unsigned char bGetInfo = 1;

        m_hResourceContext = QLIB_QPHONEMS_ConnectServer_Sahara(iComPort, &version, &sn, &ulmsid, pkHash, bGetInfo, nmode, ultimeout, pSWDL_QPHONEMS_CB);

        return ( NULL != m_hResourceContext );
    }

    void jgw_device_api::DisconnectServerFireHose(void)
    {
        if ( m_hResourceContext )
        {
            QLIB_DisconnectServer_FireHose(m_hResourceContext);
            m_hResourceContext = NULL;
        }
    }

    bool jgw_device_api::FTM_GSM_BER_RX_LEVEL_REQ(unsigned short* iStatus, unsigned long *iRSSIValue, short* iRxPwr_in16dBm)
    {
        return (TRUE == QLIB_FTM_GSM_BER_RX_LEVEL_REQ(m_hResourceContext,iStatus,iRSSIValue,iRxPwr_in16dBm));
    }

    bool jgw_device_api::FTM_LTE_NS_GET_DL_LEVEL(unsigned char iRxChain, short* iRxAGC, short* iRxLeveldBm10, unsigned long* pStatus)
    {
        return (TRUE == QLIB_FTM_LTE_NS_GET_DL_LEVEL(m_hResourceContext,iRxChain,iRxAGC,iRxLeveldBm10,pStatus));
    }

    bool jgw_device_api::FTM_TDSCDMA_BER_RX_LEVEL_REQ(unsigned char iRxChain, unsigned short* iStatus, short*iRxAGC, short* iRxPowerdBm)
    {
        return (TRUE == QLIB_FTM_TDSCDMA_BER_RX_LEVEL_REQ(m_hResourceContext,iRxChain,iStatus,iRxAGC,iRxPowerdBm));
    }

    bool jgw_device_api::FTM_WCDMA_BER_RX_LEVEL_REQ(unsigned char iRxChain, unsigned short* iStatus, short*iRxAGC, short* iRxPowerdBm)
    {
        return (TRUE == QLIB_FTM_WCDMA_BER_RX_LEVEL_REQ(m_hResourceContext,iRxChain,iStatus,iRxAGC,iRxPowerdBm));
    }

    bool jgw_device_api::QPHONEMS_Sahara_FlashProgrammer( char* sARMPRG_FileName )
    {
        return ( TRUE == QLIB_QPHONEMS_Sahara_FlashProgrammer( m_hResourceContext,sARMPRG_FileName ) );
    }

    bool jgw_device_api::QPHONEMS_ConnectServer_FireHose( unsigned int iComPort, swdlQPHONEMSCB pSWDL_QPHONEMS_CB )
    {
        m_hResourceContext = QLIB_QPHONEMS_ConnectServer_FireHose(iComPort,pSWDL_QPHONEMS_CB);
        return ( NULL != m_hResourceContext );
    }

    void jgw_device_api::QPHONEMS_FireHoseConfigureCallback( swdlQPHONEMSCB pSWDL_QPHONEMS_CB )
    {
        QLIB_QPHONEMS_FireHoseConfigureCallback(m_hResourceContext,pSWDL_QPHONEMS_CB);
    }

    bool jgw_device_api::QPHONEMS_FireHoseNOP(void)
    {
        return ( TRUE == QLIB_QPHONEMS_FireHoseNOP(m_hResourceContext) );
    }

    bool jgw_device_api::QPHONEMS_FireHoseConfigure(
        char* MaxPayloadSizeToTargetInBytes /* = "49152" */,
        char* memory_name /* = "eMMC" */,
        char* target_name /* = "8916" */,
        char* AckPacketNumber /* = "100" */,
        unsigned char bSpecifyMaxPayloadSizeToTargetInBytes /* = 0 */,
        unsigned char bUseSkipWriteAttr /* = 0 */,
        unsigned char bUseAlwaysValidateAttr /* = 0 */,
        unsigned char bUseVerboseAttr /* = 0 */,
        unsigned char bUseAckRawDataEveryNumPackets /* = 0 */)
    {
        return ( TRUE == QLIB_QPHONEMS_FireHoseConfigure(m_hResourceContext,MaxPayloadSizeToTargetInBytes,memory_name,target_name
            ,AckPacketNumber,bSpecifyMaxPayloadSizeToTargetInBytes,bUseSkipWriteAttr,bUseAlwaysValidateAttr,bUseVerboseAttr,bUseAckRawDataEveryNumPackets) );
    }

    bool jgw_device_api::QPHONEMS_SetSendDataDelay_FireHose( int timeinMilliSeconds /* = 0 */ )
    {
        return ( TRUE == QLIB_QPHONEMS_SetSendDataDelay_FireHose(m_hResourceContext,timeinMilliSeconds) );
    }

    bool jgw_device_api::QPHONEMS_EnableDataLogToFile_FireHose( unsigned char enabled /* = 0 */ )
    {
        return ( TRUE == QLIB_QPHONEMS_EnableDataLogToFile_FireHose(m_hResourceContext,enabled) );
    }

    bool jgw_device_api::QPHONEMS_EnableReadDataValidation_FireHose( unsigned char mode /* = 0 */ )
    {
        return ( TRUE == QLIB_QPHONEMS_EnableReadDataValidation_FireHose(m_hResourceContext,mode) );
    }

    bool jgw_device_api::QPHONEMS_UploadEmmcImage_FireHose( char* rawprogramfile, char* patchfile, float* imageSizeInMB, float* throughput )
    {
        return ( TRUE == QLIB_QPHONEMS_UploadEmmcImage_FireHose(m_hResourceContext,rawprogramfile,patchfile,imageSizeInMB,throughput) );
    }

    void jgw_device_api::QPHONEMS_UploadSBMultiImage_ConfigureCallBack( swdlQPHONEMSCB pSWDL_QPHONEMS_CB )
    {
        QLIB_QPHONEMS_UploadSBMultiImage_ConfigureCallBack(m_hResourceContext, pSWDL_QPHONEMS_CB);
        QLIB_SetLogFlags( m_hResourceContext, LOG_ALL );
        QLIB_ExtendedTextMessage_SetCategoryEnable(m_hResourceContext, QMSL_ExtTextMsgCat_QPHONEMS_SW_Download, 1);
    }

    bool jgw_device_api::QPHONEMS_DLoad_SendNop(void)
    {
        return ( TRUE == QLIB_QPHONEMS_DLoad_SendNop(m_hResourceContext,1000) );
    }

    bool jgw_device_api::SwitchToDownloadMode(void)
    {
        unsigned long iErrorCode = 0;
        return ( TRUE == QLIB_QPHONEMS_SwitchToDownloadMode(m_hResourceContext,3,&iErrorCode));
    }

    bool jgw_device_api::QPHONEMS_DownloadUserPartitions( char* sARMPRG_FileName,char* sPartitionFileName ,SWDL_UserPartitionList* pPartitonList,unsigned char bOverridePrtnTable )
    {
        //QMSL_DownloadAgent_Error_Enum
        //unsigned char bOverridePrtnTable = 1;
        unsigned char bUseTrustedMode = 1;
        unsigned char bSkipGoToDownload = 0;
        unsigned long iSleepTimeToDOWNLOOADmode = 5000;
        unsigned long iSleepTimeToGOcommand = 3000;
        bSkipGoToDownload = 1; //skip switch to download already switched
        //_sSCMM_ARMPRG_FileName = "\0"; //skip programmer already downloaded by Sahara
        unsigned long iErrorCode = QMSL_DOWNLOAD_NO_ERROR;

        unsigned char result = false;
        result = QLIB_QPHONEMS_DownloadUserPartitions(
            m_hResourceContext,
            sARMPRG_FileName, //empty
            sPartitionFileName,
            &pPartitonList,
            bOverridePrtnTable,
            bUseTrustedMode,
            bSkipGoToDownload, //1
            iSleepTimeToDOWNLOOADmode,
            iSleepTimeToGOcommand,
            &iErrorCode);

        if ( QMSL_DOWNLOAD_NO_ERROR == iErrorCode && result ) return true;

        PELOG4WW(GetQMSLDownloadAgentErrorString(iErrorCode));

        return false;
    }
    /*
    QMSL_DOWNLOAD_NO_ERROR                       = 0x0000,
    QMSL_DOWNLOAD_FILE_NOT_FOUND             = 0x0001,
    QMSL_DOWNLOAD_FILE_TYPE_NOT_SUPPORT          = 0x0002,
    QMSL_DOWNLOAD_FAIL_TO_SET_SECURITY_MODE      = 0x0003,
    QMSL_DOWNLOAD_FAIL_TO_SEND_PARTITION     = 0x0004,
    QMSL_DOWNLOAD_FAIL_TO_SEND_DBL               = 0x0005,
    QMSL_DOWNLOAD_FAIL_TO_SEND_OSBL              = 0x0006,
    QMSL_DOWNLOAD_FAIL_TO_SEND_FSBL              = 0x0007,
    QMSL_DOWNLOAD_FAIL_TO_SEND_MODEM         = 0x0008,
    QMSL_DOWNLOAD_FAIL_TO_GET_AMSS_VERSION       = 0x0009,
    QMSL_DOWNLOAD_FAIL_TO_SEND_DIAG_DOWDLOAD_CMD = 0x000a,
    QMSL_DOWNLOAD_FAIL_TO_SEND_DIAG_OFFLINE_CMD   = 0x000b,
    QMSL_DOWNLOAD_FAIL_TO_REPOPEN_PORT           = 0x000c,
    QMSL_DOWNLOAD_FAIL_TO_DOWNLOAD_ARMPRG        = 0x000d,
    QMSL_DOWNLOAD_FAIL_TO_FETCH_DL_PARAM     = 0x000e,
    QMSL_DOWNLOAD_NO_HELLO_COMMAND_RESPONSE      = 0x000f,
    QMSL_DOWNLOAD_FAIL_TO_SEND_DSP1              = 0x0010,
    QMSL_DOWNLOAD_FAIL_TO_SEND_DSP2              = 0x0011,
    QMSL_DOWNLOAD_FAIL_TO_SEND_PBL               = 0x0012,
    QMSL_DOWNLOAD_FAIL_TO_SEND_QCSBL         = 0x0013,
    QMSL_DOWNLOAD_FAIL_TO_SEND_QCSBLH            = 0x0014,
    QMSL_DOWNLOAD_FAIL_TO_SEND_OEMSBL            = 0x0016,
    QMSL_DOWNLOAD_FAIL_TO_SEND_APPS_BOOTLOADER  = 0x0017,
    QMSL_DOWNLOAD_FAIL_TO_SEND_APPS              = 0x0018,
    QMSL_DOWNLOAD_FAIL_TO_SEND_WINMOBILE     = 0x0019,
    QMSL_DOWNLOAD_ERROR_UNKNOWN                  = 0x001a,
    QMSL_DOWNLOAD_FAIL_TO_SEND_CEFS_MODEM        = 0x001b
    */
    const wchar_t* jgw_device_api::GetQMSLDownloadAgentErrorString( long errorCode )
    {
        switch ( errorCode )
        {
        case QMSL_DOWNLOAD_FILE_NOT_FOUND:
            return L"QMSL Download File Not Found";
        case QMSL_DOWNLOAD_FILE_TYPE_NOT_SUPPORT:
            return L"QMSL_DOWNLOAD_FILE_TYPE_NOT_SUPPORT";
        case QMSL_DOWNLOAD_FAIL_TO_SET_SECURITY_MODE:
            return L"QMSL_DOWNLOAD_FAIL_TO_SET_SECURITY_MODE";
        }
        static std::wstring strErrorMsg;
        JGW_FormatWString(strErrorMsg,L"ErrorCode:%ld",errorCode);
        return strErrorMsg.c_str();
    }

    /**********************FAST MMI*************************/
    bool jgw_device_api::FTM_AP_ERASE_ALL_FILES()
    {
        unsigned short iFTM_Error_Code = 0;
        return (TRUE == QLIB_FTM_AP_ERASE_ALL_FILES(m_hResourceContext,&iFTM_Error_Code)) && (0 == iFTM_Error_Code);
    }

    bool jgw_device_api::FTM_AP_EXECUTE_SINGLE_TEST(const char* testCase)
    {
        unsigned short iFTM_Error_Code = 0;
        return (TRUE == QLIB_FTM_AP_EXECUTE_SINGLE_TEST(m_hResourceContext,&iFTM_Error_Code,const_cast<char*>(testCase))) && (0 == iFTM_Error_Code);
    }

    bool jgw_device_api::FTM_AP_READ_FILE(const char* strSavePath /* = "c:mmi.res" */,const char* sRemoteFileName /* = "mmi.res" */)
    {
        unsigned short iFTM_Error_Code = 0;
        return (TRUE == QLIB_FTM_AP_READ_FILE(m_hResourceContext,&iFTM_Error_Code,const_cast<char*>(sRemoteFileName),const_cast<char*>(strSavePath))) && (0 == iFTM_Error_Code);
    }

    bool jgw_device_api::FTM_AP_CLEAR_RESULTS()
    {
        unsigned short iFTM_Error_Code = 0;
        return (TRUE == QLIB_FTM_AP_CLEAR_RESULTS(m_hResourceContext,&iFTM_Error_Code)) && (0 == iFTM_Error_Code); 
    }










    void jgw_device_api::InitializeQMSL( unsigned char iConnectionEnum /* = eQPhoneMS */ )
    {
        if( mbUseQPST == iConnectionEnum ) return;
        QLIB_SetLibraryMode( iConnectionEnum );
        mbUseQPST = iConnectionEnum;
        QLIB_SetTargetType( QLIB_TARGET_TYPE_MSM_MDM );
        m_bInitQMSL = true;
    }

    bool jgw_device_api::ConnectPhone( unsigned int uiTimeOut /* = 60 */,unsigned short comPort /* = QLIB_COM_AUTO_DETECT */ )
    {
        unsigned short iNumPorts = 30,pPortList[30] = {0},iNumIgnorePorts = 0,pIgnorePortList[1] = {0},_iItemStatus = 0;
        bool			bFindPort = false;

        if ( QLIB_COM_AUTO_DETECT != comPort )
        {
            m_hResourceContext = QLIB_ConnectServer(comPort);
            if ( m_hResourceContext ) return true;
        }
#if 1
        for (size_t i = 0;i < uiTimeOut;i ++ )
        {
            iNumPorts = 30;
            //PELOG4WW_F(L"Start QLIB_GetAvailablePhonesPortList:%u",GetTickCount());
            if ( !QLIB_GetAvailablePhonesPortList(&iNumPorts,pPortList,iNumIgnorePorts,pIgnorePortList) )
            {
                //PELOG4WW_F(L"Get Error QLIB_GetAvailablePhonesPortList:%u",GetTickCount());
                Sleep(1000);
                continue;
            }
            if( comPort == QLIB_COM_AUTO_DETECT && iNumPorts > 0 )
            {
                comPort = pPortList[0];
				//PELOG4WW_F(L"QLIB_GetAvailablePhonesPortList:%u,comPort:%d",GetTickCount(),comPort);
                break;
            }
            for ( int y = 0;y < iNumPorts && y < 30;y ++)
            {
                if ( pPortList[y] == comPort )
                {
                    bFindPort = true;
                    break;
                }
            }
            if ( bFindPort )	break;
            Sleep(1000);
        }
        /*if ( QLIB_COM_AUTO_DETECT != comPort )
        {
        m_hResourceContext = QLIB_ConnectServer(comPort);
        if ( m_hResourceContext ) return true;
        }*/
        //PELOG4WW_F(L"QLIB_ConnectServerWithWait:%u,comPort:%d",GetTickCount(),comPort);
        m_hResourceContext = QLIB_ConnectServerWithWait(comPort,2*1000);
#else
        m_hResourceContext = QLIB_ConnectServerWithWait(comPort,uiTimeOut/*2*1000*/);
#endif
        

        return (m_hResourceContext != NULL);
    }

    bool jgw_device_api::SyncNv()
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned char path[2] = {'/','\0'};
        unsigned char piExecutionStatus = FALSE;

        for (int i = 0;i < 3; i ++)
        {
            if(  QLIB_EFS2_SyncWithWait(m_hResourceContext,path,2000,&piExecutionStatus) && 0==piExecutionStatus )
            {
                return true;
            }
            Sleep(200);
        }

        return false;
    }

    bool jgw_device_api::IsMobileConnected()
    {
        if ( !m_hResourceContext ) return false;

        for (int i = 0;i < 10 && m_hResourceContext;i ++)
        {
            if( QLIB_IsPhoneConnected( m_hResourceContext ) )
            {
                return true;
            }
            Sleep(50);
        }
        PELOG4WW(_T("Device disconnect"));
        return true;
    }

    void jgw_device_api::StartLogging( const char* sLogFile /* = "C:\\Factory_log.txt" */,unsigned int uiLogFlags /* = 0xFFFF */ )
    {
#ifdef OPEN_QMSL_LOG
        QLIB_SetLogFlags( m_hResourceContext,uiLogFlags );
        QLIB_StartLogging( m_hResourceContext, const_cast<char*>(sLogFile) );
#endif    
    }

    void jgw_device_api::StopLogging()
    {
#ifdef OPEN_QMSL_LOG
        QLIB_StopLogging( m_hResourceContext );
#endif
    }

    void jgw_device_api::DisconnectServer()
    {
        if ( m_hResourceContext )
        {
            QLIB_DisconnectServer(m_hResourceContext);
            m_hResourceContext = NULL;
        }
    }

    bool jgw_device_api::OpenDMSSSecurityPlan(std::string sSPC /* = "000000" */)
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned char piSPC_Result = FALSE;
        unsigned char bResult =  QLIB_DIAG_SPC_F(m_hResourceContext,(unsigned char*) sSPC.c_str() , &piSPC_Result );
        return bResult && piSPC_Result;
    }

    bool jgw_device_api::ReadNV(word iItemID,byte *pItemData,int iLength)
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned short _iItemStatus = 0;
        if (QLIB_DIAG_NV_READ_F(m_hResourceContext,iItemID,pItemData,iLength,&_iItemStatus) && NV_DONE_S == _iItemStatus) return true;
        PELOG4WW(GetNVStatusAsString(_iItemStatus));
        return false;
    }

    bool jgw_device_api::ReadNV(word iItemID,byte *pItemData,int iLength,word iContextID)
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned short _iItemStatus = 0;

        if (QLIB_DIAG_NV_READ_EXT_F(m_hResourceContext,iItemID,pItemData,iContextID,iLength,&_iItemStatus) && NV_DONE_S == _iItemStatus) return true;

        PELOG4WW(GetNVStatusAsString( _iItemStatus ));
        return false;
    }

    const wchar_t* jgw_device_api::GetRfNVErrorCodeAsString(word errorCode)
    {
        switch(errorCode)
        {
        case RFNV_READ_SUCCESS:
            return L"RFNV_READ_SUCCESS";//L"RFNV_READ_SUCCESS";
        case RFNV_READ_FAILD:
            return L"RFNV_READ_FAILD";//L"RFNV_READ_FAILD";
        case RFNV_WRITE_SUCCESS:
            return L"RFNV_WRITE_SUCCESS";//L"RFNV_WRITE_SUCCESS";
        case RFNV_WRITE_FAILED:
            return L"RFNV_WRITE_FAILED";//L"RFNV_WRITE_FAILED";
        case RFNV_INACTIVE_ITEM:
            return L"RFNV_INACTIVE_ITEM";//L"RFNV_INACTIVE_ITEM";
        default:
            return L"Unknown status";//L"Unknown status: ";
        }
    }

    const wchar_t* jgw_device_api::GetNVStatusAsString( word result )
    {
        switch (result)
        {
        case NV_DONE_S:
            return L"Request completed okay";//L"Request completed okay";
        case NV_BUSY_S:
            return L"Request is queued";//L"Request is queued";
        case NV_BADCMD_S:
            return L"Unrecognizable command field";//L"Unrecognizable command field";
        case NV_FULL_S:
            return L"The NVM is full";//L"The NVM is full";
        case NV_FAIL_S:
            return L"Command failed, reason other than NVM was full";//L"Command failed, reason other than NVM was full";
        case NV_NOTACTIVE_S:
            return L"Variable was not active";//L"Variable was not active";
        case NV_BADPARM_S:
            return L"Bad parameter in command block";//L"Bad parameter in command block";
        case NV_READONLY_S:
            return L"Parameter is write-protected and this read only";//L"Parameter is write-protected and this read only";
        case NV_BADTG_S:
            return L"Item not valid for Target";//L"Item not valid for Target";
        case NV_NOMEM_S:
            return L"Free memory exhausted";//L"Free memory exhausted";
        case NV_NOTALLOC_S:
            return L"Address is not a valid allocation";//L"Address is not a valid allocation";
        default:
            return L"Unknown status: ";//L"Unknown status: ";
        }
    }

    bool jgw_device_api::WriterNV(word iItemID,unsigned char *pItemData,int iLength,word iContextID)
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned short _iItemStatus = 0;

        if( 0 == iContextID )
        {
            if(QLIB_DIAG_NV_WRITE_F(m_hResourceContext,iItemID,pItemData,iLength,&_iItemStatus) && NV_DONE_S == _iItemStatus) return true;
            PELOG4WW(GetNVStatusAsString(_iItemStatus));
            return false;
        }

        if(QLIB_DIAG_NV_WRITE_EXT_F(m_hResourceContext,iItemID,pItemData,iContextID,iLength,&_iItemStatus) && NV_DONE_S == _iItemStatus) return true;

        PELOG4WW(GetNVStatusAsString( _iItemStatus ));
        return false;
    }

    bool jgw_device_api::WriterNV(word iItemID,unsigned char *pItemData,int iLength)
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned short _iItemStatus = 0;

        if (QLIB_DIAG_NV_WRITE_F(m_hResourceContext,iItemID,pItemData,iLength,&_iItemStatus) && NV_DONE_S == _iItemStatus) return true;
        PELOG4WW(GetNVStatusAsString( _iItemStatus ));
        return false;
    }

    bool jgw_device_api::LoadRfNV()
    {
        unsigned short errorCode = 0x1;
        /*
        //Error code:
        RFNV_READ_SUCCESS  = 0
        RFNV_READ_FAILD    = 1
        RFNV_WRITE_SUCCESS = 2
        RFNV_WRITE_FAILED  = 3
        RFNV_INACTIVE_ITEM = 4
        */
        if (QLIB_FTM_LOAD_RF_NV(m_hResourceContext, &errorCode ) && 0 == errorCode) return true;
        PELOG4WW(GetRfNVErrorCodeAsString(errorCode));
        return false;
    }
    bool jgw_device_api::WriteMcwillAtCommand(E_MCWILL_USB_MSG cmd_id,const std::string strNumber,std::string& strReturnValue,int time_out /* = MCWLL_AT_COMMAND_TIME_OUT */)
    {
        TESTFUNCOK(IsMobileConnected());
        std::string strTemp(strNumber);
        ftm_McWill_pkt_type sftm = {0};
        sftm.cmd_code = 75;
        sftm.subsys_id = 11;
        sftm.subsys_cmd_code = 93;
        sftm.cmd_id = cmd_id;
        sftm.cmd_ms_time_out = time_out;
        sftm.cmd_data_len = sizeof(ftm_McWill_pkt_type);
        MakestringToUpper(strTemp);
        memcpy_s(sftm.data,sizeof(sftm.data),strTemp.c_str(),strTemp.length());
        ftm_McWill_generic_res ftm = {0};
        short iszie = sizeof(ftm_McWill_generic_res);

        QLIB_SendSync(m_hResourceContext,sftm.cmd_data_len,(unsigned char *)&sftm,&iszie,(unsigned char *)&ftm,3000);

        if (1 == ftm.result)
        {
            strReturnValue = (char*)ftm.data;
            MakestringToUpper(strReturnValue);
        }

        return ftm.result==1;
    }

    bool jgw_device_api::WriteMcwillAtCommand(E_MCWILL_USB_MSG cmd_id,const std::string strNumber,int time_out /* = 2000 */)
    {
        TESTFUNCOK(IsMobileConnected());
        std::string strTemp(strNumber);
        ftm_McWill_pkt_type sftm = {0};
        sftm.cmd_code = 75;
        sftm.subsys_id = 11;
        sftm.subsys_cmd_code = 93;
        sftm.cmd_id = cmd_id;
        sftm.cmd_ms_time_out = time_out;
        sftm.cmd_data_len = sizeof(ftm_McWill_pkt_type);
        MakestringToUpper(strTemp);
        memcpy_s(sftm.data,sizeof(sftm.data),strTemp.c_str(),strTemp.length());
        ftm_McWill_generic_res ftm = {0};
        short iszie = sizeof(ftm_McWill_generic_res);

        QLIB_SendSync(m_hResourceContext,sftm.cmd_data_len,(unsigned char *)&sftm,&iszie,(unsigned char *)&ftm,3000);

        return ftm.result==1;
    }

    bool jgw_device_api::WriteMcwillPid(const std::string strPid,int time_out /* = 2000 */)
    {
        return WriteMcwillAtCommand(E_MCWILL_WRITE_PID,strPid,time_out);
    }

    bool jgw_device_api::WriteMcwillUSBMac(const std::string strUSBMac,int time_out /* = 2000 */)
    {
        if( MCWILL_MAC_LENGTH != strUSBMac.length() ) return false;
        std::string strformat(strUSBMac);
        for(int i = 2;i < MCWILL_MAC_LENGTH + 4;i+=3)
            strformat.insert(i,1,',');
        return WriteMcwillAtCommand(E_MCWILL_WRITE_USBMAC,strformat,time_out);
    }

    bool jgw_device_api::ReadMcwillAtCommand(E_MCWILL_USB_MSG cmd_id,std::string& strReturnValue,int time_out /* = 2000 */)
    {
        TESTFUNCOK(IsMobileConnected());
        ftm_McWill_pkt_type sftm = {0};
        sftm.cmd_code = 75;
        sftm.subsys_id = 11;
        sftm.subsys_cmd_code = 93;
        sftm.cmd_id = cmd_id;
        sftm.cmd_ms_time_out = time_out;
        sftm.cmd_data_len = sizeof(ftm_McWill_pkt_type);
        ftm_McWill_generic_res ftm = {0};
        short iszie = sizeof(ftm_McWill_generic_res);

        QLIB_SendSync(m_hResourceContext,sftm.cmd_data_len,(unsigned char *)&sftm,&iszie,(unsigned char *)&ftm,3000);
        strReturnValue = (char*)ftm.data;
        MakestringToUpper(strReturnValue);

        return ftm.result==1;
    }

    bool jgw_device_api::ReadMcwillHWType(std::string& strHWType,int time_out /* = MCWLL_AT_COMMAND_TIME_OUT */)
    {
        bool bStatus = false;
        bStatus = ReadMcwillAtCommand(E_MCWILL_READ_HW_TYPE,strHWType,time_out);
        if( !bStatus || 5 != strHWType.length() ) return false;
        ReplaceStringA(strHWType,".","");
        if( !bStatus || 4 != strHWType.length() ) return false;
        return bStatus;
    }

    bool jgw_device_api::ReadMcwillPid(std::string& strPid,int time_out /* = 2000 */)
    {
        return ReadMcwillAtCommand(E_MCWILL_READ_PID,strPid,time_out);
    }

    bool jgw_device_api::ReadMcwillUSBMac(std::string& strUsbMac,int time_out /* = 2000 */)
    {
        bool bStatus = false;
        bStatus = ReadMcwillAtCommand(E_MCWILL_READ_USBMAC,strUsbMac,time_out);
        if( !bStatus || MCWILL_MAC_LENGTH + 5 != strUsbMac.length() ) return false;
        ReplaceStringA(strUsbMac,"-","");
        if( !bStatus || MCWILL_MAC_LENGTH != strUsbMac.length() ) return false;
        return bStatus;
    }

    bool jgw_device_api::JGW_QLIB_SendSync( short iRequestSize, unsigned char* piRequestBytes, short* piResponseSize, unsigned char* piResponseBytes, unsigned long iTimeout )
    {
        return ( TRUE == QLIB_SendSync(m_hResourceContext,iRequestSize,piRequestBytes,piResponseSize,piResponseBytes,iTimeout) );
    }

    bool jgw_device_api::SendMcwillUsbMsgToDevice( E_MCWILL_USB_MSG cmd_id, int time_out /* = 2000 */ )
    {
        TESTFUNCOK(IsMobileConnected());
        ftm_McWill_pkt_type sftm = {0};
        sftm.cmd_code = 75;
        sftm.subsys_id = 11;
        sftm.subsys_cmd_code = 93;
        sftm.cmd_id = cmd_id;
        sftm.cmd_data_len = sizeof(ftm_McWill_pkt_type);
        ftm_McWill_generic_res ftm = {0};
        short iszie = sizeof(ftm_McWill_generic_res);

        QLIB_SendSync(m_hResourceContext,sftm.cmd_data_len,(unsigned char *)&sftm,&iszie,(unsigned char *)&ftm,time_out);
        return ftm.result==1;
    }

    bool jgw_device_api::ReadQcommSubsysVersion( int& sub_version,int time_out /* = 2000 */ )
    {
        TESTFUNCOK(IsMobileConnected());
        ftm_McWill_pkt_type sftm = {0};
        sftm.cmd_code = 75;
        sftm.subsys_id = 11;
        sftm.subsys_cmd_code = 93;
        sftm.cmd_id = E_READ_SUBSYS_VERSION;
        sftm.cmd_data_len = sizeof(ftm_McWill_pkt_type);
        ftm_McWill_generic_res ftm = {0};
        short iszie = sizeof(ftm_McWill_generic_res);
        QLIB_SendSync(m_hResourceContext,sftm.cmd_data_len,(unsigned char *)&sftm,&iszie,(unsigned char *)&ftm,time_out);
        sub_version = ftm.value.platform_subtype_id;
        return ftm.result==1;
    }

    bool jgw_device_api::ReadMcwillNetCSQToDevice( int& csq,int time_out /* = 2000 */ )
    {
        TESTFUNCOK(IsMobileConnected());
        ftm_McWill_pkt_type sftm = {0};
        sftm.cmd_code = 75;
        sftm.subsys_id = 11;
        sftm.subsys_cmd_code = 93;
        sftm.cmd_id = E_READ_MCWILL_CSQ;
        sftm.cmd_data_len = sizeof(ftm_McWill_pkt_type);
        ftm_McWill_generic_res ftm = {0};
        short iszie = sizeof(ftm_McWill_generic_res);
        QLIB_SendSync(m_hResourceContext,sftm.cmd_data_len,(unsigned char *)&sftm,&iszie,(unsigned char *)&ftm,time_out);
        csq = ftm.value.csq;
        return ftm.result==1;
    }

    bool jgw_device_api::SetPhoneMode( mode_enum_type ePhoneMode )
    {
        unsigned char result = 0x1;
        unsigned short status = 0x1;
        const unsigned long diagPhoneModeReadBackTimeout_bak = QLIB_GetTimeOut(m_hResourceContext, QMSL_Timeout_Runtime_Mode_Switching);
        const unsigned long diagPhoneModeReadBackTimeout = 500;//do not want to wait for timeout here.
        QLIB_ConfigureTimeOut(m_hResourceContext, QMSL_Timeout_Runtime_Mode_Switching, diagPhoneModeReadBackTimeout);
        result = QLIB_DIAG_CONTROL_F(m_hResourceContext, ePhoneMode);
        QLIB_ConfigureTimeOut(m_hResourceContext, QMSL_Timeout_Runtime_Mode_Switching, diagPhoneModeReadBackTimeout_bak);
        return result ? true: false;
    }

    bool jgw_device_api::ResetPhone( unsigned int uiTimeOut /* = 5 */ )
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned char result = QLIB_DIAG_CONTROL_F( m_hResourceContext,MODE_RESET_F );
        int i = uiTimeOut * 5;

        while( i-- > 0 )
        {
            if( !IsMobileConnected() )
            {
                DisconnectServer();
                return true;
            }
            Sleep( 200 );
        }
        return false;
    }

    bool jgw_device_api::UploadQcnFile(std::string QcnPath, std::string sSPC /* = "000000" */, bool bAutoRestore /* = true */, bool bSkipReset /* = true */, bool bAllowEsnMismatch /* = true */, bool bIgnorePhoneModel /* = true */)
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned char result = 0x1;
        // Check the extension whether it is xqcn or not. If XQCN, then use QLIB_UploadQcnFile_V2.
        unsigned char bUseXqcn = 0;
        if (QcnPath.length() > 5)
        {
            std::string stExt =  QcnPath.substr(QcnPath.length() - 4);
            if (_stricmp(stExt.c_str(),"xqcn") == 0)
            {
                bUseXqcn = 1;
            }
        }
        if (bUseXqcn)
        {
            result = QLIB_UploadQcnFile_V2
                (
                m_hResourceContext,
                (char*) QcnPath.c_str(),
                (char*) sSPC.c_str(),
                (unsigned char) bAllowEsnMismatch
                );
        }
        else
        {
            result = QLIB_UploadQcnFile
                (
                m_hResourceContext,
                (char*) QcnPath.c_str(),
                (char*) sSPC.c_str(),
                (unsigned char)bAutoRestore,
                (unsigned char) bSkipReset,
                (unsigned char) bAllowEsnMismatch,
                (unsigned char) bIgnorePhoneModel
                );
        }

        return result ? true : false;
    }

    bool jgw_device_api::DownloadQcnFile(std::string backupQcnPath,int*count/* =NULL */)
    {
        TESTFUNCOK(IsMobileConnected());
        unsigned char result =  0x1 ;
        unsigned char bUseXqcn = 0;
        int res = 0;

        if ( backupQcnPath.length() > 5 )
        {
            std::string stExt =  backupQcnPath.substr(backupQcnPath.length() - 4);
            if (_stricmp(stExt.c_str(),"xqcn") == 0)
            {
                bUseXqcn = 1;
            }
        }
        if ( bUseXqcn )
        {
            result = QLIB_DownloadQcnFile_V2(m_hResourceContext, (char*)backupQcnPath.c_str(), "000000");
        }
        else
        {
            result = QLIB_DownloadQcnFile(m_hResourceContext, (char*)backupQcnPath.c_str(), "000000");

        }

        if(NULL!=count)
        {
            QLIB_NV_LoadNVsFromQCN(m_hResourceContext,backupQcnPath.c_str(),count,&res);
        }

        return result ? true : false;
    }

    void jgw_device_api::NVConfigureCallBack(nvToolCB nvToolCBFunc)
    {
        QLIB_NV_ConfigureCallBack(m_hResourceContext,nvToolCBFunc);
    }

    bool jgw_device_api::RestoreQCN(std::string uploadQcnPath)
    {
        int iResultCode = 0,iNumOfNVItemValuesLoaded = 0;
        if (!QLIB_NV_LoadNVsFromQCN(m_hResourceContext,(char*) uploadQcnPath.c_str(),&iNumOfNVItemValuesLoaded, &iResultCode))
        {
            PELOG4WW(L"QLIB_NV_LoadNVsFromQCN fail");
            return false;
        }
        //QLIB_NV_WriteNVsToMobile(m_hResourceContext,  &iResultCode)
        if (TRUE == QLIB_NV_WriteNVsToMobile(m_hResourceContext, &iResultCode)) return true;
        PELOG4WW_F(L"Restore QCN fail code:%d",iResultCode);
        return false; 
    }

    bool jgw_device_api::BackupQCNToMobile(std::string backupQcnPath)
    {
        int iResultCode = 0;
        if (TRUE == QLIB_BackupNVFromMobileToQCN(m_hResourceContext,(char*) backupQcnPath.c_str(), &iResultCode)) return true;
        PELOG4WW(GetQCNResultCodeAsString(iResultCode));
        return false;
    }

    const wchar_t*	jgw_device_api::GetQCNResultCodeAsString(int iResultCode)
    {
        switch (iResultCode)
        {
        case BACKUP_ERROR_FREE:
            break;
        case BACKUP_FAILED_TO_READ_EXT_BUILD_INFO:
            return L"BACKUP_FAILED_TO_READ_EXT_BUILD_INFO";
        case BACKUP_FAILED_TO_READ_FEATURE_MASK:
            return L"BACKUP_FAILED_TO_READ_FEATURE_MASK";
        case BACKUP_FAILED_TO_OPEN_QCN_STORAGE:
            return L"BACKUP_FAILED_TO_OPEN_QCN_STORAGE";
        case BACKUP_FAILED_TO_READ_STORAGE_CONFIGURATION:
            return L"BACKUP_FAILED_TO_READ_STORAGE_CONFIGURATION";
        case BACKUP_FAILED_TO_CREATE_EFS_BACKUP:
            return L"BACKUP_FAILED_TO_CREATE_EFS_BACKUP";
        case BACKUP_FAILED_TO_OPEN_NV_STORAGE:
            return L"BACKUP_FAILED_TO_OPEN_NV_STORAGE";
        case BACKUP_FAILED_TO_OPEN_NV_STORAGE_BY_CONTEXT:
            return L"BACKUP_FAILED_TO_OPEN_NV_STORAGE_BY_CONTEXT";
        case BACKUP_UNKNOWN:
            return L"BACKUP_UNKNOWN";
        }
        return L"BACKUP_UNKNOWN";
    }

    bool jgw_device_api::SwitchPhoneEDL(void)
    {
        __try
        {
            for (int i = 0;i < 3;i ++)
            {
                if(QLIB_QPHONEMS_SwitchToEDL(m_hResourceContext)) break;
                Sleep(300);
            }
            QLIB_DisconnectServer(m_hResourceContext);
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return  false;
        }

        return true;/*(TRUE == QLIB_QPHONEMS_SwitchToEDL(m_hResourceContext));*/
    }

    bool jgw_device_api::SetUEFFBMModel(unsigned char iBootMode,unsigned char iSubBootMode /* = 0 */)
    {
        CHECKFUNRES(IsMobileConnected());
        unsigned char bResult = FALSE;
        unsigned short iFTM_Error_Code;

        bResult = QLIB_FTM_FFBM_SET_MODE(m_hResourceContext,iBootMode,iSubBootMode,&iFTM_Error_Code);

        if (!bResult || iFTM_Error_Code)
        {
            PELOG4WW_F(L"QLIB_FTM_FFBM_SET_MODE ERROR_CODE:%d",iFTM_Error_Code);
            return false;
        }

        return true;
    }

    bool jgw_device_api::CheckUEFFBMModel(int& nCurrentBootMode,int& nNextBootMode/* = m_nNextBootModeTemp*/)
    {
        CHECKFUNRES(m_hResourceContext);
        CHECKFUNRES(QLIB_IsPhoneConnected(m_hResourceContext));

        unsigned short iFTM_Error_Code = 0;
        unsigned char iCurrentBootMode = 0;
        unsigned char iCurrentBootSubMode = 0;
        unsigned char iNextBootMode = 0;
        unsigned char iNextBootSubMode = 0;

        BOOL bResult = QLIB_FTM_FFBM_GET_MODE(m_hResourceContext, 
            &iFTM_Error_Code,
            &iCurrentBootMode,
            &iCurrentBootSubMode,
            &iNextBootMode,
            &iNextBootSubMode);

        if (!bResult || iFTM_Error_Code)
        {
            //g_strJGWMsvcErrorMsg = L"CheckFFBM Fail";
            CHECKFUNRES(false);
        }
        nCurrentBootMode = iCurrentBootMode;
        nNextBootMode = iNextBootMode;

        return true;
    }
}