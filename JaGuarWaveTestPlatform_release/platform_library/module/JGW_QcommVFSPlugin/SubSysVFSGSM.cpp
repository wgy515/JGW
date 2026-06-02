#include <CTF_QCSubSysVFS/SubSysVFSGSM.h>

CSubSysVFSBase *CSubSysVFSGSM::m_lpInstance = NULL;
CSubSysVFSGSM::CSubSysVFSGSM(void):CSubSysVFSBase()
{
}

CSubSysVFSGSM::~CSubSysVFSGSM(void)
{
}

bool CSubSysVFSGSM::StartCall()
{
	int iBand = m_sBandInfo.m_nBand;
	GetGsmBand(iBand);
	CHECKFUNRES_SUB(QLIB_IsPhoneConnected(m_hResourceContext));
	QLIB_FTM_GSM_BER_ClearStatus(m_hResourceContext);
	QLIB_FTM_GSM_BER_ReleaseTCH(m_hResourceContext);
	QLIB_FTM_GSM_BER_StopSession(m_hResourceContext);


	CHECKFUNRES_SUB(QLIB_FTM_SET_MODE_ID(m_hResourceContext,FTM_MODE_GSM_BER));
	CHECKFUNRES_SUB(QLIB_FTM_GSM_BER_StartSession( m_hResourceContext,10000 ));
	CHECKFUNRES_SUB(QLIB_FTM_GSM_BER_AssignBCCH( m_hResourceContext,m_sBandInfo.m_nUpLinkChannel, iBand));

	unsigned char iTSC = 0,iSlot = FTM_GSM_BER_SLOT_3,iMode = FTM_GSM_BER_SPEECH_FULL_RATE;
	CHECKFUNRES_SUB(QLIB_FTM_GSM_BER_Handover_NoLoopbackConfig(m_hResourceContext, m_sBandInfo.m_nUpLinkChannel, iBand , iTSC, iSlot, iMode, /*iPwrLevel*/m_sBandInfo.m_iPwrIndex));

	unsigned short iLoopbackType = FTM_GSM_BER_Loopback_Type_C;
	CHECKFUNRES_SUB(QLIB_FTM_GSM_ConfigLoopbackType(m_hResourceContext, iLoopbackType) );

	for (int nIndex = 0;nIndex < 100;nIndex ++)
	{
		if (QLIB_FTM_GSM_BER_IsTrafficValid(m_hResourceContext))
		{
			break;
		}
		Sleep(100);
	}

	//m_bFristGSMHandOver = FALSE;
	return true;
}

bool CSubSysVFSGSM::HandOverChannel()
{
	return true;
}

bool CSubSysVFSGSM::SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode /* = 1 */)
{
	int iBand = m_sBandInfo.m_nBand;
	GetGsmBand(iBand);

	CHECKFUNRES_SUB(QLIB_IsPhoneConnected(m_hResourceContext));
	CHECKFUNRES_SUB(QLIB_FTM_SET_MODE_ID(m_hResourceContext,FTM_MODE_GSM_BER));
	CHECKFUNRES_SUB(QLIB_FTM_GSM_BER_ClearStatus(m_hResourceContext));

	unsigned char iTSC = 0,iSlot = FTM_GSM_BER_SLOT_3,iMode = FTM_GSM_BER_SPEECH_FULL_RATE;
	CHECKFUNRES_SUB(QLIB_FTM_GSM_BER_Handover_NoLoopbackConfig(m_hResourceContext, m_sBandInfo.m_nUpLinkChannel, iBand , iTSC, iSlot, iMode, /*iPwrLevel*/nPower));
	CHECKFUNRES_SUB(QLIB_IsPhoneConnected(m_hResourceContext));
	Sleep(200);

	return true;
}

bool CSubSysVFSGSM::TestBER(int nRxChain,float fBerPower,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts /* = 1000 */)
{
	return true;
}

bool CSubSysVFSGSM::EndCall()
{
	QLIB_IsPhoneConnected(m_hResourceContext);
	QLIB_FTM_GSM_BER_ReleaseTCH(m_hResourceContext);
	QLIB_FTM_GSM_BER_StopSession(m_hResourceContext);
	return true;
}

bool CSubSysVFSGSM::CalFreqInfo()
{
	switch(m_sBandInfo.m_nBand)
	{
	case 2://1900
	case 1900:
		m_sBandInfo.dFreqUL = 1850 + (m_sBandInfo.m_nUpLinkChannel - 511) * 0.2;

		m_sBandInfo.dFreqDL = 1930 + (m_sBandInfo.m_nUpLinkChannel - 511) * 0.2;
		break;
	case 3://1800
	case 1800:
		m_sBandInfo.dFreqUL = 1710 + (m_sBandInfo.m_nUpLinkChannel - 511) * 0.2;
		m_sBandInfo.dFreqDL = 1805 + (m_sBandInfo.m_nUpLinkChannel - 511) * 0.2;
		break;
	case 5://850
	case 850:
		m_sBandInfo.dFreqUL = 824 + 0.2 * (m_sBandInfo.m_nUpLinkChannel - 127);
		m_sBandInfo.dFreqDL = 869 + 0.2 * (m_sBandInfo.m_nUpLinkChannel - 127);;
		break;
	case 8://900
	case 900:
		if (m_sBandInfo.m_nUpLinkChannel >= 975)
		{
			m_sBandInfo.dFreqUL = 890 + 0.2 * (m_sBandInfo.m_nUpLinkChannel - 1024);
			m_sBandInfo.dFreqDL = 935 + 0.2 * (m_sBandInfo.m_nUpLinkChannel - 1024);;
		}
		else
		{
			m_sBandInfo.dFreqUL = 890 + 0.2 * m_sBandInfo.m_nUpLinkChannel;
			m_sBandInfo.dFreqDL = 935 + 0.2 * m_sBandInfo.m_nUpLinkChannel;;
		}
		break;
	default:
		return false;
	}

	return true;
}

bool CSubSysVFSGSM::Check_NS_Is_Connected()
{
	return true;
}