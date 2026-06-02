#include "stdafx.h"
#include <JGW_QMSLFuncPlugin/SubSysVFSCDMA.h>
#pragma warning(disable:4018)

namespace JGW
{

    CSubSysVFSBase *CSubSysVFSCDMA::m_lpInstance = NULL;
    CSubSysVFSCDMA::CSubSysVFSCDMA(void):CSubSysVFSBase()
    {
    }

    CSubSysVFSCDMA::~CSubSysVFSCDMA(void)
    {
    }

    bool CSubSysVFSCDMA::StartCall()
    {
        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

        unsigned long iPhoneMode;
        CHECKFUNRES_SUB( QLIB_GetPhoneOperatingMode(m_hResourceContext,&iPhoneMode) && SYS_OPRT_MODE_FTM == iPhoneMode );
        CHECKFUNRES_SUB( Check_NS_Is_Connected() );

        short eModeId = FTM_MODE_ID_CDMA_1X;
        CHECKFUNRES_SUB( QLIB_FTM_SET_MODE_ID(m_hResourceContext,eModeId) );
        CHECKFUNRES_SUB( QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext) );
        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_NS_ClearStatus(m_hResourceContext,TRUE) );

        unsigned long eTimeoutId= 3;/*QMSL_Timeout_Nonsignaling*/
        unsigned long iNewValue_ms = 5 * 1000;
        CHECKFUNRES_SUB( QLIB_ConfigureTimeOut(m_hResourceContext,eTimeoutId,iNewValue_ms) );

        unsigned char eDR_Type = 1;
        double dTurnAroundConstant = -73;
        unsigned char bEnableDiversityAGCLog = 0;
        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_NS_Start(
            m_hResourceContext,
            eDR_Type,
            dTurnAroundConstant,
            bEnableDiversityAGCLog
            ) );

        unsigned long iBand_Class = 0;
        unsigned short iCDMA_Freq = m_sBandInfo.m_nUpLinkChannel;
        unsigned long iAcq_Mode = 0;
        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_PILOT_ACQ( 
            m_hResourceContext,
            iBand_Class, 
            iCDMA_Freq, 
            iAcq_Mode
            ) );

        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_DEMOD_SYNC(m_hResourceContext) );

        unsigned long iRadio_Cfg_DEMOD = 1;
        unsigned char iWalsh = 10;
        unsigned char iQOF_Mask_ID = 0;
        unsigned char iBypass_LC = 0;
        unsigned char iFrame_Offset_DEMOD = 0;
        unsigned char iSubChan_Gain = 0;
        unsigned char iFFPC_Enable = 0;
        unsigned long iFPC_Mode = 0;
        unsigned char iInit_Setpt = 0;
        unsigned char iOLC_Params_Target_FER = 0;
        unsigned char iOLC_Params_Min_Setpt = 0;
        unsigned char iOLC_Params_Max_Setpt = 0;
        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_DEMOD_FCH( 
            m_hResourceContext,
            iRadio_Cfg_DEMOD,
            iWalsh, 
            iQOF_Mask_ID, 
            iBypass_LC, 
            iFrame_Offset_DEMOD, 
            iSubChan_Gain, 
            iFFPC_Enable, 
            iFPC_Mode, 
            iInit_Setpt, 
            iOLC_Params_Target_FER, 
            iOLC_Params_Min_Setpt, 
            iOLC_Params_Max_Setpt 
            ) );

        unsigned long iRadio_Cfg_MOD = 1;
        unsigned char iFrame_Offset_MOD = 0;
        unsigned long iFCH_Rate = 0;
        unsigned char iEnable_RPC = 1;
        unsigned short iNum_Preambles = 0;
        unsigned long iTX_Pattern = 0;
        CHECKFUNRES_SUB(  QLIB_FTM_CDMA2000_MOD_FCH( 
            m_hResourceContext,
            iRadio_Cfg_MOD, 
            iFrame_Offset_MOD, 
            iFCH_Rate, 
            iEnable_RPC , 
            iNum_Preambles, 
            iTX_Pattern
            ) );
        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
        CHECKFUNRES_SUB(  QLIB_FTM_CDMA2000_NS_Stop(m_hResourceContext) );
        //	CDMA_2000_NS_SetTargetTxPower(0);
        SetTargetTxPower(0);
        return true;
    }

    bool CSubSysVFSCDMA::HandOverChannel()
    {
        CHECKFUNRES_SUB( Check_NS_Is_Connected() );
        unsigned long iBand_Class = 0;
        unsigned short iCDMA_Freq = m_sBandInfo.m_nUpLinkChannel;
        unsigned char iEnable_RPC = 1;
        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_FTM_FWD_HHO_SC( 
            m_hResourceContext,
            iBand_Class, 
            iCDMA_Freq, 
            iEnable_RPC
            ) );

        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

        return true;
    }

    bool CSubSysVFSCDMA::SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode /* = 1 */)
    {
        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
        FTM_CDMA2000_NS_Set_Rev_Power_Type_Enum iPower_Type = FTM_1X_TX_POWER_TYPE_AGC_SETPOINT;
        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_SET_REV_PWR(m_hResourceContext,  iPower_Type ,nPower) );	
        return true;
    }

    bool CSubSysVFSCDMA::TestBER(int nTestCounts,float fBlerPw,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts /* = 10000*/)
    {
        CHECKFUNRES_SUB( SetTargetTxPower(280) );

        int nCountIndex = 0;
        for( ;nCountIndex < nTestCounts;nCountIndex ++)
        {
            CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

            CHECKFUNRES_SUB( QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext) );
            CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_NS_ClearStatus(m_hResourceContext,TRUE) );
            unsigned long eTimeOutId = 3;
            unsigned long iNewValue_ms = 5000;
            CHECKFUNRES_SUB( QLIB_ConfigureTimeOut( m_hResourceContext, eTimeOutId, iNewValue_ms ) );
            unsigned char eDR_Type = 1;
            double dTurnAroundConstant = -73.00;
            unsigned char bEnableDiversityAGCLog = 0;
            CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_NS_Start( m_hResourceContext, eDR_Type, dTurnAroundConstant, bEnableDiversityAGCLog ) );
            CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
            unsigned long eCurrentCDMA2000_RC = 1;
            unsigned long eCurrentCDMA2000_DataRate = 0;
            CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_NS_SetSER_RCandRS( m_hResourceContext, eCurrentCDMA2000_RC, eCurrentCDMA2000_DataRate) );
            CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
            CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_RESET_FCH_STAT(m_hResourceContext) );

            Sleep(300);

            unsigned long iNumFrame = 0;
            unsigned long iFrameError = 0;
            double dFER = 0;
            unsigned long iNumSymbol = 0;
            unsigned long iSymbolError = 0;
            double dSER = 0;
            CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_GET_FCH_STAT( m_hResourceContext, &iNumFrame, &iFrameError, &dFER, &iNumSymbol, &iSymbolError, &dSER ) );
            CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_RESET_FCH_STAT(m_hResourceContext) );
            Sleep(2000);
            CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_GET_FCH_STAT( m_hResourceContext, &iNumFrame, &iFrameError, &dFER, &iNumSymbol, &iSymbolError, &dSER ) );
            CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
            CHECKFUNRES_SUB( QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext) );

            fBlerErr = static_cast<float>(dFER);

            if ((iNumFrame < 50) || (iNumSymbol < nCounts))
            {
                continue;
            }

            if (( fBlerErr < 0 ) || ( fBlerErr > fBlerErrConfig ))
            {
                CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_NS_Stop(m_hResourceContext) );
            }
            else
            {
                break;
            }
        }
        swprintf_s(szErrorBuffer, MAX_PATH,L"C_FER(%0.2fdbm--trigger%d) @ 0 %% @ %0.4f %% @ %0.2f %%",fBlerPw, nCountIndex, fBlerErr, fBlerErrConfig);
        TESTFUNCOK_SUB( nCountIndex < nTestCounts)

            return true;
    }

    bool CSubSysVFSCDMA::EndCall()
    {
        CHECKFUNRES_SUB( Check_NS_Is_Connected() );
        CHECKFUNRES_SUB( QLIB_FTM_CDMA2000_CMD_RELEASE(m_hResourceContext) );

        return true;
    }

    bool CSubSysVFSCDMA::CalFreqInfo()
    {
        CHECKFUNRES_SUB( 0 == m_sBandInfo.m_nBand );
        m_sBandInfo.dFreqUL = 825 + ( m_sBandInfo.m_nUpLinkChannel * 0.03 );
        m_sBandInfo.dFreqDL = 870 + ( m_sBandInfo.m_nUpLinkChannel * 0.03 );
        m_sBandInfo.m_nDownloadLinkChannel =  m_sBandInfo.m_nUpLinkChannel;

        return true;
    }

    bool CSubSysVFSCDMA::Check_NS_Is_Connected()
    {
        CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

        unsigned char bIsFTMMode;
        CHECKFUNRES_SUB( QLIB_IsFTM_Mode(m_hResourceContext,&bIsFTMMode) && bIsFTMMode );

        return true;
    }
}