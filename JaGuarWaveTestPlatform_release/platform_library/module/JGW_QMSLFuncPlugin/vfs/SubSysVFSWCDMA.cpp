#include "stdafx.h"
#include <JGW_QMSLFuncPlugin/SubSysVFSWCDMA.h>

namespace JGW
{
    CSubSysVFSBase *CSubSysVFSWCDMA::m_lpInstance = NULL;
    CSubSysVFSWCDMA::CSubSysVFSWCDMA(void):CSubSysVFSBase()
    {
    }

    CSubSysVFSWCDMA::~CSubSysVFSWCDMA(void)
    {
    }

    bool CSubSysVFSWCDMA::StartCall()
    {
        unsigned long iPhoneMode;

        CHECKFUNRES_SUB( QLIB_GetPhoneOperatingMode(m_hResourceContext,&iPhoneMode)&& SYS_OPRT_MODE_FTM == iPhoneMode );
        CHECKFUNRES_SUB( Check_NS_Is_Connected() );

        QLIB_FTM_WCDMA_BER_StopSession(m_hResourceContext);

        short eModeId = FTM_MODE_ID_WCDMA;
        CHECKFUNRES_SUB(  QLIB_FTM_SET_MODE_ID(m_hResourceContext,eModeId) );

        unsigned char iMode = 0;
        CHECKFUNRES_SUB(  QLIB_FTM_SECOND_CHAIN_TEST_CALL(m_hResourceContext,iMode) );

        unsigned long eTimeoutId= 3;/*QMSL_Timeout_Nonsignaling*/
        unsigned long iNewValue_ms = /*10 * */1000;

        CHECKFUNRES_SUB(  QLIB_ConfigureTimeOut(m_hResourceContext,eTimeoutId,iNewValue_ms) );

        unsigned long iTimeOut_ms = 10 *1000/*FTM_NONSIG_DEFAULT_TIMEOUT*/;
        CHECKFUNRES_SUB(  QLIB_FTM_WCDMA_BER_StartSession(m_hResourceContext,iTimeOut_ms) );

        unsigned short iChannel = /*10700*/m_sBandInfo.m_nDownloadLinkChannel;
        CHECKFUNRES_SUB(  QLIB_FTM_WCDMA_BER_AcquirePilot(m_hResourceContext,iChannel) );

        unsigned char eTFCS_Select1 = FTM_WCDMA_BER_UL_TFCS_CONFIG;
        unsigned char aiTFCS_Config[4] = {0,1,2,3};
        CHECKFUNRES_SUB(  QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(m_hResourceContext,eTFCS_Select1,aiTFCS_Config) );
        unsigned char eTFCS_Select0 = FTM_WCDMA_BER_DL_TFCS_CONFIG;
        CHECKFUNRES_SUB(  QLIB_FTM_WCDMA_BER_RMC_DCH_TFCS_CONFIG_REQ(m_hResourceContext,eTFCS_Select0,aiTFCS_Config) );

        //QLIB_FTM_WCDMA_BER_Handover( iRMC_Type = 3, iUL_MaxTxPwr = 33, iUL_DPCH_ScrCode = 0, iUL_PCA = 1, iUL_TPC_StepSize = 0, iDL_PriCPICH_Scr_Code = 8, iDL_ChanCode = 9, iBetaC = 8, iBetaD = 15, iChannel  = 4357 )
        CHECKFUNRES_SUB(HandOverChannel());
        // 	unsigned char iRMC_Type = FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric;
        // 	unsigned char iUL_MaxTxPwr = 28;
        // 	unsigned long iUL_DPCH_ScrCode = 0;
        // 	unsigned char iUL_PCA = FTM_WCDMA_BER_PCA_Type_Alg1;
        // 	unsigned char iUL_TPC_StepSize = FTM_WCDMA_BER_PCA_Size_1dB;
        // 	unsigned short iDL_PriCPICH_Scr_Code = 8;
        // 	// For this parameter, Agilent default is 9, R&S default is 2
        // 	unsigned short iDL_ChanCode = 9;
        // 	CHECKFUNRES_SUB(  QLIB_FTM_WCDMA_BER_SetupRMC_DCH(
        // 				m_hResourceContext,
        // 				iRMC_Type,
        // 				iUL_MaxTxPwr,
        // 				iUL_DPCH_ScrCode,
        // 				iUL_PCA,
        // 				iUL_TPC_StepSize,
        // 				m_niDL_PriCPICH_Scr_Code,
        // 				iDL_ChanCode
        // 				) );

        Sleep(200);
        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

        SetTargetTxPower(0);

        return true;
    }
    //QLIB_FTM_WCDMA_BER_Handover( iRMC_Type = 3, iUL_MaxTxPwr = 33, iUL_DPCH_ScrCode = 0, iUL_PCA = 1, iUL_TPC_StepSize = 0, iDL_PriCPICH_Scr_Code = 8, iDL_ChanCode = 9, iBetaC = 8, iBetaD = 15, iChannel  = 4357 )
    bool CSubSysVFSWCDMA::HandOverChannel()
    {
        CHECKFUNRES_SUB( Check_NS_Is_Connected() );

        unsigned char iRMC_Type = /*FTM_WCDMA_BER_RMC_Type12_2kpbs*/FTM_WCDMA_BER_RMC_Type12_2kpbs_Symmetric;
        unsigned char iUL_MaxTxPwr = 33;
        unsigned long iUL_DPCH_ScrCode = 0;
        unsigned char iUL_PCA = FTM_WCDMA_BER_PCA_Type_Alg1;
        unsigned char iUL_TPC_StepSize = FTM_WCDMA_BER_PCA_Size_1dB;
        //unsigned short iDL_PriCPICH_Scr_Code = 0;
        //unsigned short iDL_PriCPICH_Scr_Code = 8;
        // For this parameter, Agilent default is 9, R&S default is 2
        unsigned short iDL_ChanCode = 9;
        unsigned char iBetaC = 8;
        unsigned char iBetaD = 15;
        short iDPCCH_PowerOffset = -60;
        unsigned char n_312 = FTM_WCDMA_BER_N312_s1;
        unsigned char n_313 = FTM_WCDMA_BER_N313_s20;
        unsigned char n_315 = FTM_WCDMA_BER_N315_s1;
        unsigned char t_312 = 1;
        unsigned char t_313 = 3;
        unsigned short iChannel = m_sBandInfo.m_nDownloadLinkChannel;
        CHECKFUNRES_SUB( QLIB_FTM_WCDMA_BER_Handover_V2B(
            m_hResourceContext,
            iRMC_Type,
            iUL_MaxTxPwr,
            iUL_DPCH_ScrCode,
            iUL_PCA,
            iUL_TPC_StepSize,
            m_niDL_PriCPICH_Scr_Code,
            iDL_ChanCode,
            iBetaC,
            iBetaD,
            iDPCCH_PowerOffset,
            n_312,
            n_313,
            n_315,
            t_312,
            t_313,
            iChannel 
            ) );

        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
        CHECKFUNRES_SUB( Check_NS_Is_Connected() );

        return true;
    }

    bool CSubSysVFSWCDMA::SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode /* = 1 */)
    {
        CHECKFUNRES_SUB( Check_NS_Is_Connected() );

        unsigned short iStatus = 0;
        CHECKFUNRES_SUB( QLIB_FTM_WCDMA_BER_SET_UL_POWER(m_hResourceContext,/*bOverride =*/1, /*iUL_Pwr_dBm10 =*/nPower,&iStatus) && !iStatus );

        return true;
    }

    bool CSubSysVFSWCDMA::TestBER(int nTestCounts,float fBlerPw,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts /* = 1000*/)
    {
        //SetTargetTxPower(280);
        int nCountIndex = 0;
        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

        for (;nCountIndex < nTestCounts;nCountIndex ++)
        {
            /*CHECKFUNRES_SUB( */QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext) /*)*/;

            for (int i = 0;i < 20;i ++)
            {
                if(QLIB_FTM_SE_BER_Start_Log( m_hResourceContext,
                    /*iFramesPerLog =*/ 8, 
                    /*iPnSeqType = */1, 
                    /*bPnInverted = */0, 
                    /*iAcqThreshold =*/ 20, 
                    /*iAcqLostWm =*/ 60, 
                    /*iSE_BER_Loopback_Type =*/ 0, 
                    /*iMCS =*/ 1 ))
                    break;
                Sleep(200);
            }
            // 		CHECKFUNRES_SUB( QLIB_FTM_SE_BER_Start_Log( m_hResourceContext,
            // 			/*iFramesPerLog =*/ 8, 
            // 			/*iPnSeqType = */1, 
            // 			/*bPnInverted = */0, 
            // 			/*iAcqThreshold =*/ 20, 
            // 			/*iAcqLostWm =*/ 60, 
            // 			/*iSE_BER_Loopback_Type =*/ 0, 
            // 			/*iMCS =*/ 1 ) );
            CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
            SE_BER_Status sSEBER = {0};
            CHECKFUNRES_SUB( QLIB_FTM_SE_BER_DoMeasurement(m_hResourceContext,&sSEBER,0,nCounts,10 * 1000) && (sSEBER.bits_counted > 0) );
            CHECKFUNRES_SUB( QLIB_FTM_SE_BER_Stop_Log(m_hResourceContext) );

            fBlerErr = (float)sSEBER.ber_stats.BER*100;
            if (( fBlerErr < 0 ) || ( fBlerErr > fBlerErrConfig ))
            {
                Sleep(300);
            }
            else
            {
                break;
            }
        }
        swprintf_s(szErrorBuffer,MAX_PATH, L"WCDMA_BER(%0.2fdbm--trigger%d) @ 0%% @ %0.2f%% @ %0.2f%%",fBlerPw,nCountIndex , fBlerErr, fBlerErrConfig);
        TESTFUNCOK_SUB( nCountIndex < nTestCounts );

        return true;
    }

    bool CSubSysVFSWCDMA::EndCall()
    {
        TESTFUNCOK_SUB( Check_NS_Is_Connected() );
        TESTFUNCOK_SUB( QLIB_FTM_SE_BER_Stop_Log(m_hResourceContext) );
        TESTFUNCOK_SUB( QLIB_FTM_WCDMA_BER_StopSession(m_hResourceContext) );

        return true;
    }

    bool CSubSysVFSWCDMA::CalFreqInfo()
    {
        CHECKFUNRES_SUB(m_sBandInfo.m_nBand < 12 && m_sBandInfo.m_nBand > 0);

        m_sBandInfo.m_nDownloadLinkChannel = m_sBandInfo.m_nUpLinkChannel + g_dWcdmaFreqTable[m_sBandInfo.m_nBand].UARFCNoffset;
        m_sBandInfo.dFreqUL = ((double)m_sBandInfo.m_nUpLinkChannel / (double)5.0) + (double)g_dWcdmaFreqTable[m_sBandInfo.m_nBand].nUpLinkOffset;
        m_sBandInfo.dFreqDL = ((double)m_sBandInfo.m_nDownloadLinkChannel  / (double)5.0) + (double)g_dWcdmaFreqTable[m_sBandInfo.m_nBand].nDownloadLinkOffset;

        return true;
    }

    bool CSubSysVFSWCDMA::Check_NS_Is_Connected()
    {
        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

        unsigned char bIsFTMMode;
        CHECKFUNRES_SUB( QLIB_IsFTM_Mode(m_hResourceContext,&bIsFTMMode) && bIsFTMMode);
        return true;
    }
}