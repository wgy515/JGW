#pragma once
#include "subsysvfsbase.h"

//默认常量
extern unsigned short iCRNTI;
extern unsigned char iNetworkSigValue ;
extern char iBSMaxPowerLimit;
extern unsigned char iDLLCID;
extern unsigned char iULLCID ;

class CSubSysVFSLTE :
	public CSubSysVFSBase
{
public:
	CSubSysVFSLTE(void);
public:
	~CSubSysVFSLTE(void);

	static CSubSysVFSBase *GetInstance()
	{
		if( !m_lpInstance )
			m_lpInstance = new CSubSysVFSLTE;
		return m_lpInstance;
	}

	virtual bool	InitBandInfo(int m_nBnad,int nUpLinkChannel);
	virtual bool	StartCall() ;//开始呼叫
	virtual bool	HandOverChannel() ;//呼叫后切换信息
	virtual bool	EndCall() ;//切断呼叫
	virtual bool	CalFreqInfo() ;//计算频率相关信息
	//检查非信令连接是否OK
	virtual bool    Check_NS_Is_Connected();

	virtual bool	SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode = 1) ;//设置目标发射功率
	virtual bool	TestBER(int nRxChain,float fBerPower,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts = 1000) ;//测试BER
	virtual bool	LTE_NS_SetUpLinkWaveform(
		unsigned char  iConfigOverride = 1,  
		unsigned char  iTxChain = 0,  
		unsigned char  iStartRB = 0,  
		unsigned char  iNumRB = 50,  
		unsigned char  iMCS = 6 );

	static CSubSysVFSBase *m_lpInstance;
};
