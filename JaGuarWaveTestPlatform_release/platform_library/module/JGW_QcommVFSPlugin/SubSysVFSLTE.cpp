#include <CTF_QCSubSysVFS/SubSysVFSLTE.h>
//默认常量
unsigned short iCRNTI = 14;
unsigned char iNetworkSigValue = 1;
char iBSMaxPowerLimit = 30;
unsigned char iDLLCID = 1;
unsigned char iULLCID = 2;
/*unsigned long iEARFCN = 14;*/
/**********************************/
CSubSysVFSBase *CSubSysVFSLTE::m_lpInstance = NULL;
CSubSysVFSLTE::CSubSysVFSLTE(void):CSubSysVFSBase()
{
}

CSubSysVFSLTE::~CSubSysVFSLTE(void)
{
}

bool CSubSysVFSLTE::InitBandInfo(int m_nBnad,int nUpLinkChannel)
{
	//CHECKFUNRES_SUB(m_lpInstance);
	m_sBandInfo.m_nBand = m_nBnad;
	m_sBandInfo.m_nUpLinkChannel = nUpLinkChannel;
	CHECKFUNRES_SUB(CalFreqInfo());

	return true;
}

bool CSubSysVFSLTE::StartCall()
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
	TESTFUNCOK_SUB( SetTargetTxPower( 250 ) );
	//TESTFUNCOK_SUB( HandOverChannel() );

	return true;
}

bool CSubSysVFSLTE::HandOverChannel()
{
	unsigned long status = 0;
	unsigned char bIsFTMMode;
	unsigned long iFTM_Error_Code = FALSE;

	TESTFUNCOK_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
	//TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_IS_CONNECTED(m_hResourceContext, &iFTM_Error_Code) );
	TESTFUNCOK_SUB( QLIB_IsFTM_Mode(m_hResourceContext,&bIsFTMMode) && bIsFTMMode );
	TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_START(m_hResourceContext, &status)/* && (0 == status)*/);
	TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_ACQ(m_hResourceContext, m_sBandInfo.m_nBand - 1, m_sBandInfo.m_nDownloadLinkChannel, &iFTM_Error_Code) );
	if ( m_sBandInfo.m_nBand  > (33-1) )
	{
		QLIB_FTM_LTE_NS_TDD_CONFIG(m_hResourceContext,1,4,&iFTM_Error_Code) ;
	}


	TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_START_DP( m_hResourceContext, iCRNTI, iNetworkSigValue, iBSMaxPowerLimit, iDLLCID, iULLCID, &iFTM_Error_Code )/* && FALSE == iFTM_Error_Code*/);
	Sleep( 100 );

	TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_IS_CONNECTED(m_hResourceContext, &iFTM_Error_Code) );
	TESTFUNCOK_SUB( LTE_NS_SetUpLinkWaveform() );
	TESTFUNCOK_SUB( QLIB_DIAG_LTE_RX_TOOL_CHAIN_COMMAND(m_hResourceContext,3) );
	TESTFUNCOK_SUB( SetTargetTxPower( 250) );
	Sleep( 100 );

	return true;
}

bool CSubSysVFSLTE::SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode /* = 1 */)
{
	unsigned long status;

	CHECKFUNRES_SUB( QLIB_FTM_LTE_NS_CONFIG_UL_POWER(m_hResourceContext, 0, ucTxPwrCtrlMode, nPower, &status) &&
		(0 == status || 13 == status ));

	return true;
}
//PRX 主  DRX 辅
bool CSubSysVFSLTE::TestBER( int nRxChain,float fBerPower,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts /* = 1000 */ )
{
	unsigned long status;
	unsigned long iTotalBlockReceived, iTotalBlockError, iThroughput;
	unsigned long iFTM_Error_Code = 0;


	swprintf_s(szErrorBuffer,MAX_PATH,L"CONNECTED ERROR@---@Flash Buffer Err@---");
	TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_IS_CONNECTED(m_hResourceContext, &iFTM_Error_Code) );
	TESTFUNCOK_SUB( SetTargetTxPower( 250 ) );
	//TESTFUNCOK_SUB( QLIB_DIAG_LTE_RX_TOOL_CHAIN_COMMAND(m_hResourceContext,_LTE_RX_CHAIN_TYPE_NO_ACTION + nRxChain + 1) );
	Sleep(300);
	TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_IS_CONNECTED(m_hResourceContext, &iFTM_Error_Code) );

	swprintf_s(szErrorBuffer,MAX_PATH,L"FTM BLER@---@Flush Buffer Err@---");
	TESTFUNCOK_SUB( QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext) );

	swprintf_s(szErrorBuffer,MAX_PATH, L"FTM %s BLER(%0.2f)@---@RESET_DL_BLER_REPORT Failed@---",nRxChain==0?L"PRX":L"DRX",fBerPower);
	for(int i = 0;i < 30;i ++)
	{
		if(QLIB_FTM_LTE_NS_RESET_DL_BLER_REPORT(m_hResourceContext,&status) && (!status || status == 13))
		{
			break;
		}
		Sleep(100);
	}

	unsigned char bResult = FALSE;
	int nIndex = 0;
	//测试次数 10 次
	for ( ;nIndex < 35;nIndex ++)
	{
		bResult = QLIB_FTM_LTE_NS_GET_DL_BLER_REPORT( m_hResourceContext, &iTotalBlockReceived, &iTotalBlockError, &iThroughput, &status );
		swprintf_s(szErrorBuffer,260,L"QLIB_FTM_LTE_NS_GET_DL_BLER_REPORT( iTotalBlockReceived=%d, iTotalBlockError=%d, iThroughput=%d,status=%d) = %d",iTotalBlockReceived, iTotalBlockError, iThroughput, status,bResult);
		//LOG_INFO(szErrorBuffer);

		if( !bResult || status != 0 )
		{
			swprintf_s(szErrorBuffer, MAX_PATH,L"FTM %s BLER(%0.2f)@---@GET_DL_BLER_REPORT Failed@---",nRxChain==0?L"PRX":L"DRX", fBerPower);
			return false;
		}

		if ( iTotalBlockReceived < 1000)
		{
			Sleep( 200 );
			continue;
		}

		fBlerErr =  ((float)iTotalBlockError /(float)iTotalBlockReceived) * 100.00f;
// 		sprintf_s(szErrorBuffer,MAX_PATH,"%.2f - %.2f",fBlerErrConfig,fBlerErr);
// 		HTLOG_INFO(szErrorBuffer);

		if ( fBlerErr < fBlerErrConfig )
		{
			break;
		}

		TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_RESET_DL_BLER_REPORT(m_hResourceContext,&status) && !status );
		Sleep( 200 );
	}
	if ( iTotalBlockReceived  == 0 )
	{
		swprintf_s(szErrorBuffer,MAX_PATH, L"FTM %s BLER(%0.2f)@0.0@%.2f@%0.2f",nRxChain==0?L"PRX":L"DRX",fBerPower,100.00f,fBlerErrConfig);
	}
	else
	{
		swprintf_s(szErrorBuffer,MAX_PATH, L"FTM %s BLER(%0.2f)@0.0@%.2f@%0.2f",nRxChain==0?L"PRX":L"DRX",fBerPower,fBlerErr,fBlerErrConfig);
	}

	TESTFUNCOK_SUB( nIndex < 35 );
	QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext);

	return true;
}

bool CSubSysVFSLTE::EndCall()
{
	unsigned long status = 0;

	CHECKFUNRES_SUB( QLIB_FTM_LTE_NS_STOP(m_hResourceContext, &status ) &&(0 == status || 13 == status ));

	return true;
}

bool CSubSysVFSLTE::CalFreqInfo()
{
	CHECKFUNRES_SUB( m_sBandInfo.m_nBand >= 1 && m_sBandInfo.m_nBand <= 44 );

	if (m_sBandInfo.m_nBand >= 1 && m_sBandInfo.m_nBand <= 32)
	{
		m_sBandInfo.m_nDownloadLinkChannel = g_dLteFreqTable[m_sBandInfo.m_nBand].nDownloadLinkOffset + (m_sBandInfo.m_nUpLinkChannel - g_dLteFreqTable[m_sBandInfo.m_nBand].nUpLinkOffset);
	}
	else 
	{
		m_sBandInfo.m_nDownloadLinkChannel = m_sBandInfo.m_nUpLinkChannel;
	}

	m_sBandInfo.dFreqDL = g_dLteFreqTable[m_sBandInfo.m_nBand].dDownloadLinkFreqLow + 0.1 * (m_sBandInfo.m_nUpLinkChannel - g_dLteFreqTable[m_sBandInfo.m_nBand].nUpLinkOffset);
	m_sBandInfo.dFreqUL = g_dLteFreqTable[m_sBandInfo.m_nBand].dUpLinkFreqLow + 0.1 * (m_sBandInfo.m_nUpLinkChannel - g_dLteFreqTable[m_sBandInfo.m_nBand].nUpLinkOffset);

	return true;
}

bool CSubSysVFSLTE::LTE_NS_SetUpLinkWaveform( unsigned char iConfigOverride, unsigned char iTxChain, unsigned char iStartRB, unsigned char iNumRB, unsigned char iMCS )
{
	unsigned long status = 0;

	CHECKFUNRES_SUB( QLIB_FTM_LTE_NS_CONFIG_UL_WAVEFORM(m_hResourceContext, iConfigOverride, iTxChain, iStartRB, iNumRB, iMCS, &status) &&
		(0 == status || 13 == status) );

	return true;
}

bool CSubSysVFSLTE::Check_NS_Is_Connected()
{
	unsigned long status = 0;
	TESTFUNCOK_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
	TESTFUNCOK_SUB( QLIB_FTM_LTE_NS_IS_CONNECTED(m_hResourceContext,&status) && !status);
	
	return true;
}