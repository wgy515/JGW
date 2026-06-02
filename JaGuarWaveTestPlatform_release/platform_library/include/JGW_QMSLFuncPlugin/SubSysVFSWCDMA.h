#pragma once
#include "subsysvfsbase.h"

namespace JGW
{
    class CSubSysVFSWCDMA :
        public CSubSysVFSBase
    {
    private:
        CSubSysVFSWCDMA(void);
    public:
        ~CSubSysVFSWCDMA(void);

        void	InitBandInfo( COMMUNICATIONSSYS eSys,int nUpLinkChannel);//初始化频段信息

        static CSubSysVFSBase *GetInstance(){if( !m_lpInstance )m_lpInstance = new CSubSysVFSWCDMA;return m_lpInstance;}
        bool	StartCall() ;//开始呼叫
        bool	HandOverChannel() ;//呼叫后切换信息
        bool	SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode = 1) ;//设置目标发射功率
        bool	TestBER(int nTestCounts,float fBlerPw,float fBlerErrConfig,float &fBlerErr,wchar_t *szErrorBuffer,int nCounts  = 1000) ;//测试BER
        bool	EndCall() ;//切断呼叫
        bool	CalFreqInfo() ;//计算频率相关信息
        //检查非信令连接是否OK
        bool    Check_NS_Is_Connected();
        static CSubSysVFSBase *m_lpInstance;
    };
}