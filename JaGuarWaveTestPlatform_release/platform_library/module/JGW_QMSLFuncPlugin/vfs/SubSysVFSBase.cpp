#include "stdafx.h"
#include <JGW_QMSLFuncPlugin/SubSysVFSBase.h>

namespace JGW
{
    //CSubSysVFSBase *CSubSysVFSBase::m_lpInstance = NULL;
    CSubSysVFSBase::CSubSysVFSBase(void):m_hResourceContext(NULL)
        ,m_niDL_PriCPICH_Scr_Code(0)
    {
        m_sBandInfo.InitParam();
    }

    CSubSysVFSBase::~CSubSysVFSBase(void)
    {
    }

    bool CSubSysVFSBase::InitBandInfo(int nBnad,int nUpLinkChannel,int nDownLinkChannel,double dFreqUL,double dFreqDL)
    {
        m_sBandInfo.m_nBand = nBnad;
        m_sBandInfo.m_nUpLinkChannel = nUpLinkChannel;
        m_sBandInfo.m_nDownloadLinkChannel = nDownLinkChannel;
        m_sBandInfo.dFreqDL = dFreqDL;
        m_sBandInfo.dFreqUL = dFreqUL;
        return true;
    }

    bool CSubSysVFSBase::InitBandInfo(int m_nBnad,int nUpLinkChannel)
    {
        //CHECKFUNRES_SUB(m_lpInstance);
        m_sBandInfo.m_nBand = m_nBnad;
        m_sBandInfo.m_nUpLinkChannel = nUpLinkChannel;
        CHECKFUNRES_SUB(CalFreqInfo());

        return true;
    }

    bool CSubSysVFSBase::LTE_NS_SetUpLinkWaveform( unsigned char iConfigOverride /* = 1 */, unsigned char iTxChain /* = 0 */, unsigned char iStartRB /* = 0 */, unsigned char iNumRB /* = 50 */, unsigned char iMCS /* = 6 */ )
    {
        //LOG_ERROR("LTE_NS_SetUpLinkWaveform do not supprot");
        return false;
    }

    //// bool CSubSysVFSBase::LTE_SetTargetTxPower(int nPower,unsigned char ucTxPwrCtrlMode /* = 1 */)
    //{
    //	HTLOG_DEBUG("LTE_SetTargetTxPower do not supprot");
    //	return false;
    //}
    //
    //bool CSubSysVFSBase::LTE_TestBER(int nRxChain, float fBerPower, float fBlerErrConfig, float &fBlerErr, char *szErrorBuffer, int nCounts /* = 1000 */ )
    //{
    //	HTLOG_DEBUG("LTE_TestBER do not supprot");
    //	return false;
    //}
}