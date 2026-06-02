#include <CTF_QCSubSysVFS/SubSysVFSTDSCDMA.h>

unsigned char iRMC_Type = FTM_TDSCDMA_BER_RMC_TYPE_12_2_SC;
unsigned char iDL_Slot = 4;/*valid value 2~6*/
unsigned char iUL_Slot = 2;/*valid value 1~6*/
unsigned char iUL_MaxTxPwr = 24;/*dbm*/
unsigned char iUL_TPC_StepSize = 1;

unsigned char sys_mode = 5;/*TDSCDMA*/
unsigned char chain_selection = 1;/*chain 0,prx*/


CSubSysVFSBase *CSubSysVFSTDSCDMA::m_lpInstance = NULL;
CSubSysVFSTDSCDMA::CSubSysVFSTDSCDMA(void):CSubSysVFSBase()
{
}

CSubSysVFSTDSCDMA::~CSubSysVFSTDSCDMA(void)
{
}

bool CSubSysVFSTDSCDMA::StartCall()
{
	unsigned long iPhoneMode = SYS_OPRT_MODE_FTM;

	CHECKFUNRES_SUB( Check_NS_Is_Connected() );
	CHECKFUNRES_SUB( QLIB_GetPhoneOperatingMode(m_hResourceContext,&iPhoneMode) && SYS_OPRT_MODE_FTM == iPhoneMode)
	CHECKFUNRES_SUB( QLIB_FTM_SET_MODE_ID(m_hResourceContext,FTM_MODE_ID_TDSCDMA) );

	unsigned long eTimeoutId= 3;/*QMSL_Timeout_Nonsignaling*/
	unsigned long iNewValue_ms = 10 * 1000;
	CHECKFUNRES_SUB( QLIB_ConfigureTimeOut(m_hResourceContext,eTimeoutId,iNewValue_ms) );

	unsigned long iTimeOut_ms = 2000;
	CHECKFUNRES_SUB( QLIB_FTM_TDSCDMA_BER_StartSession(m_hResourceContext,iTimeOut_ms) );


	unsigned short iChannel = m_sBandInfo.m_nUpLinkChannel;
	CHECKFUNRES_SUB( QLIB_FTM_TDSCDMA_BER_AcquirePilot(m_hResourceContext,iChannel) )
	CHECKFUNRES_SUB( QLIB_FTM_TDSCDMA_BER_SetupRMC_DCH(m_hResourceContext,iRMC_Type,iDL_Slot,iUL_Slot,iUL_MaxTxPwr,iUL_TPC_StepSize) );
	CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
	CHECKFUNRES_SUB( QLIB_DIAG_CHANGE_RX_DIVERSITY(m_hResourceContext,sys_mode,chain_selection) );


	SetTargetTxPower(-550);

	return true;
}

bool CSubSysVFSTDSCDMA::HandOverChannel()
{
	CHECKFUNRES_SUB( Check_NS_Is_Connected() );

	unsigned short iChannel = /*10054*/m_sBandInfo.m_nUpLinkChannel;

	CHECKFUNRES_SUB( QLIB_FTM_TDSCDMA_BER_Handover(m_hResourceContext,iRMC_Type,iDL_Slot,iUL_Slot,iUL_MaxTxPwr,iUL_TPC_StepSize,iChannel) );
	CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
	CHECKFUNRES_SUB( QLIB_DIAG_CHANGE_RX_DIVERSITY(m_hResourceContext,sys_mode,chain_selection) );
	SetTargetTxPower(-550);

	return true;
}

bool CSubSysVFSTDSCDMA::SetTargetTxPower(int iUL_Pwr_dBm10,unsigned char ucTxPwrCtrlMode /* = 1 */)
{
	CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

	unsigned short iStatus = 0;
	CHECKFUNRES_SUB( QLIB_FTM_TDSCDMA_BER_SET_UL_POWER(m_hResourceContext,/*bOverride =*/1, /*iUL_Pwr_dBm10 =*/iUL_Pwr_dBm10,&iStatus) && !iStatus );

	return true;
}

bool CSubSysVFSTDSCDMA::TestBER(int nTestCounts,float fBlerPw,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts /* = 10000*/)
{
	SetTargetTxPower(240);
	int nCountIndex = 0;

	for (;nCountIndex < nTestCounts;nCountIndex ++)
	{
		CHECKFUNRES_SUB( Check_NS_Is_Connected() );
		CHECKFUNRES_SUB( QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext) );
		CHECKFUNRES_SUB( QLIB_FTM_SE_BER_Start_Log( m_hResourceContext,
			/*iFramesPerLog =*/ 8, 
			/*iPnSeqType = */1, 
			/*bPnInverted = */0, 
			/*iAcqThreshold =*/ 20, 
			/*iAcqLostWm =*/ 60, 
			/*iSE_BER_Loopback_Type =*/ 8, 
			/*iMCS =*/ 1 ) );
		CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );
		SE_BER_Status sSEBER;
		CHECKFUNRES_SUB( QLIB_FTM_SE_BER_DoMeasurement(m_hResourceContext,&sSEBER,0,8000,nCounts/*10000*/) );
		CHECKFUNRES_SUB( QLIB_FTM_SE_BER_Stop_Log(m_hResourceContext) );
		CHECKFUNRES_SUB( QLIB_DIAG_FLUSH_BUFFER(m_hResourceContext) );

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
	swprintf_s(szErrorBuffer,MAX_PATH, L"TDS_BER(%0.2fdbm--trigger%d) @ 0%% @ %0.2f%% @ %0.2f%%",fBlerPw,nCountIndex , fBlerErr, fBlerErrConfig);
	TESTFUNCOK_SUB( nCountIndex < nTestCounts );

	return true;
}

bool CSubSysVFSTDSCDMA::EndCall()
{
	CHECKFUNRES_SUB( Check_NS_Is_Connected() );
	CHECKFUNRES_SUB( QLIB_FTM_TDSCDMA_BER_DropAll(m_hResourceContext) );
	CHECKFUNRES_SUB( QLIB_FTM_TDSCDMA_BER_StopSession(m_hResourceContext) );

	return true;
}

bool CSubSysVFSTDSCDMA::CalFreqInfo()
{
	m_sBandInfo.m_nDownloadLinkChannel = m_sBandInfo.m_nUpLinkChannel ;
	m_sBandInfo.dFreqDL = m_sBandInfo.dFreqUL = m_sBandInfo.m_nUpLinkChannel/5.00;
	return true;
}

bool CSubSysVFSTDSCDMA::Check_NS_Is_Connected()
{
	CHECKFUNRES_SUB( QLIB_IsPhoneConnected(m_hResourceContext) );

	unsigned char bIsFTMMode;
	CHECKFUNRES_SUB( QLIB_IsFTM_Mode(m_hResourceContext,&bIsFTMMode) && bIsFTMMode);
	return true;
}