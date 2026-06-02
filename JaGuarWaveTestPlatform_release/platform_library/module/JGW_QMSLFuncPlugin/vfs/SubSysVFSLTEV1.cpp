#include "stdafx.h"
#include <JGW_QMSLFuncPlugin/SubSysVFSLTEV1.h>

namespace JGW
{
    CSubSysVFSBase *CSubSysVFSLTEV1::m_lpInstance = NULL;
    CSubSysVFSLTEV1::CSubSysVFSLTEV1(void)
    {
    }

    CSubSysVFSLTEV1::~CSubSysVFSLTEV1(void)
    {
    }

    bool CSubSysVFSLTEV1::StartCall()
    {
        unsigned long iFTM_Error_Code = FALSE;
        unsigned short sStatus = 0;

        TESTFUNCOK_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

        unsigned long PhoneMode = 0;
        TESTFUNCOK_SUB ( QLIB_GetPhoneOperatingMode(m_hResourceContext,&PhoneMode) && SYS_OPRT_MODE_FTM == PhoneMode);

        TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_IDLE(m_hResourceContext,&iFTM_Error_Code) );
        TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_STOP(m_hResourceContext,&iFTM_Error_Code) );
        TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_START(m_hResourceContext, &iFTM_Error_Code) );

        TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_ACQ(m_hResourceContext, m_sBandInfo.m_nBand - 1, m_sBandInfo.m_nDownloadLinkChannel, &iFTM_Error_Code) );

        if ( m_sBandInfo.m_nBand  > (33-1) )
        {
            QLIB_FTM_LTE_NS_TDD_CONFIG(m_hResourceContext,1,4,&iFTM_Error_Code) ;
        }

        TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_START_DP( m_hResourceContext, iCRNTI, iNetworkSigValue, iBSMaxPowerLimit, iDLLCID, iULLCID, &iFTM_Error_Code )/* && FALSE == iFTM_Error_Code*/);
        TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_IS_CONNECTED(m_hResourceContext, &iFTM_Error_Code) );
        TESTFUNCOK_SUB( LTE_NS_SetUpLinkWaveform() );
        TESTFUNCOK_SUB( QLIB_DIAG_LTE_RX_TOOL_CHAIN_COMMAND(m_hResourceContext,3) );
        TESTFUNCOK_SUB( SetTargetTxPower( 280 ) );
        TESTFUNCOK_SUB( HandOverChannel() );

        return true;
    }

    bool CSubSysVFSLTEV1::HandChannel()
    {
        TESTFUNCOK_SUB( QLIB_DIAG_LTE_RX_TOOL_CHAIN_COMMAND(m_hResourceContext,3) );
        unsigned long iFTM_Error_Code = FALSE;
        TESTFUNCOK_SUB( Check_NS_Is_Connected() );
        TESTFUNCOK_SUB( SetTargetTxPower(100) );
        TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_HANDOVER(m_hResourceContext,m_sBandInfo.m_nBand - 1, m_sBandInfo.m_nDownloadLinkChannel, &iFTM_Error_Code) && ( 0 == iFTM_Error_Code || 13 == iFTM_Error_Code) );
        /*TESTFUNCOK_SUB(*/ Check_NS_Is_Connected() /*)*/;

        /*TESTFUNCOK_SUB(*/ QLIB_DIAG_LTE_RX_TOOL_CHAIN_COMMAND(m_hResourceContext,3) /*)*/;
        /*TESTFUNCOK_SUB(*/ SetTargetTxPower(250)/* )*/;
        /*TESTFUNCOK_SUB(*/ LTE_NS_SetUpLinkWaveform() /*)*/;
        /*TESTFUNCOK_SUB(*/ Check_NS_Is_Connected()/* )*/;

        return true;
    }

    bool CSubSysVFSLTEV1::HandOverChannel()
    {
        for (int nIndex = 0;nIndex < 3;nIndex ++)
        {
            if ( HandChannel() )
            {
                return true;
            }
        }

        return false;
    }
}