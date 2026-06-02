#pragma once
#include "SubSysVFSDefine.h"
#include <CTF_QcommFuncLib/QMSL_inc/QLib.h>
#include <CTF_QcommFuncLib/QMSL_inc/QLib_Defines.h>
//涵盖LTE WCDMA TDSCDMA CDMA GSM -> 然后适配不同的接口
//然后初始化不同的实例
//流程->设置端口句柄 -> 初始化频段信息 -> 开始呼叫
class CSubSysVFSBase
{
public:
	CSubSysVFSBase(void);
	~CSubSysVFSBase(void);
	//设置资源句柄
	void	SetResourceContext(HANDLE hResourceContext){m_hResourceContext = hResourceContext;}
	//初始化频段信息
	bool	InitBandInfo( int m_nBnad,int nUpLinkChannel,int nDownLinkChannel,double dFreqUL,double dFreqDL);
	bool	InitBandInfo( int m_nBnad,int nUpLinkChannel);

	static CSubSysVFSBase *GetInstance() {return NULL;}

	/* General Interface */
	//开始呼叫
	virtual bool	StartCall() = 0;
	//呼叫后切换信息
	virtual bool	HandOverChannel() = 0;
	//切断呼叫
	virtual bool	EndCall() = 0;
	//计算频率相关信息
	virtual bool	CalFreqInfo() = 0;
	//检查非信令连接是否OK
	virtual bool    Check_NS_Is_Connected() = 0;
	//设置目标发射功率
	virtual	bool	SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode = 1) = 0;
	//测试BER
	virtual	bool	TestBER(int nTestCounts,float fBlerPw,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts  = 10000) = 0;
	//设置上行波形
	virtual bool	LTE_NS_SetUpLinkWaveform(
		unsigned char  iConfigOverride = 1,  
		unsigned char  iTxChain = 0,  
		unsigned char  iStartRB = 0,  
		unsigned char  iNumRB = 50,  
		unsigned char  iMCS = 6 );

	/*WCDMA Class Interface */

public:
	unsigned short m_niDL_PriCPICH_Scr_Code;
	SBAND_INFO m_sBandInfo;
	//static CSubSysVFSBase *m_lpInstance;
	HANDLE m_hResourceContext;
};
