#include "stdafx.h"
#include <JGW_Device_RohdeSchwarz_CMW500/CFactoty_RFVFS.h>

namespace JGW
{
    CFactory_RFVFS::CFactory_RFVFS():m_pEquipInter(NULL)
        ,m_eLteChBWType(E_LTE_CH_BW_10_MHZ)
        ,m_eSysName(E_LTE_NAME)
    {

    }

    CFactory_RFVFS::~CFactory_RFVFS()
    {
        if ( m_pEquipInter )
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
        }
    }

    //！设置Tx线损
    void CFactory_RFVFS::SetTxLoss( float fTxLoss /*= 1.00f*/ )
    {

    }
    //! 设置Rx线损
    void CFactory_RFVFS::SetRxLoss( float fRxLoss/* = 1.00f*/ )
    {

    }
    //! 仪器复位以及 删除线损表
    bool CFactory_RFVFS::ResetMode( )
    {
        return true;
    }
    //! 初始化仪器
    bool CFactory_RFVFS::InitMode()
    {
        return true;
    }
    //! 设置仪器输入输出口
    bool CFactory_RFVFS::SetInOutPutPort( int nTxNumb,int nRxNumb/* = ERFAC*/ )
    {
        return true;
    }

    //! 设GPRF上下行频率
    bool CFactory_RFVFS::SetGPRFDlUlFreq(double dFreqDL, double dFreqUL,float nCellPower/* = -57.00f*/,float nEnPower/* = 23.00f*/)
    {
        return true;
    }

    bool CFactory_RFVFS::SetGPRFDlUlFreq(S_BAND_CHANNEL_INFO& sBandChannelInfo,float nCellPower /* = -57.00f */,float nEnPower /* = 23.00f */)
    {
        return true;
    }

    //! 设置信号发生器小区功率，以及仪器期望功率
    bool CFactory_RFVFS::SetCellPoweAEnPower( float nCellPower/* = -57.00f*/,float nEnPower /*= 23.00f*/ )
    {
        return true;
    }

    //! 设Band信息
    bool CFactory_RFVFS::SetGPRFBand(int nBand)
    {
        return true;
    }
    //！开启仪器测量
    void CFactory_RFVFS::OpenMEValuation()
    {
        return ;
    }

    void CFactory_RFVFS::CloseMEValuation()
    {
        return ;
    }

    bool CFactory_RFVFS::SetCellPower( float fCellPower/* = -57.00f*/)
    {
        return true;
    }

    bool CFactory_RFVFS::GPS_InitBSE( int nTxNumb/* = ERFAC*/,float fCellPower/* = -130*/,double dFreqDL/* = 1575.42*/)
    {
        return true;
    }
    /*	LTE制式所独有	*/
    //！设置LTE频段和模式  开启信号发生器
    bool CFactory_RFVFS::LTE_SetBandAndMode(int nBand)
    {
        return true;
    }
    //! 初始化LTE测试环境
    bool CFactory_RFVFS::LTE_InitMeas()
    {
        return true;
    }
    //! 开始测试所有LTE参数
    bool CFactory_RFVFS::LTE_NS_MesurmentAll(LTE_NS_MaxPwrTestValue &testValue)
    {
        return true;
    }
    bool CFactory_RFVFS::LTE_NS_GetMinPower(float &fMinPower)
    {
        return true;
    }

    /*  TDSCDMA */
    //! 初始化TDSCDMA仪器设备
    bool CFactory_RFVFS::TDSCDMA_InitBSE()
    {
        return true;
    }
    bool CFactory_RFVFS::TDSCDMA_NS_GetMinPower(double& fMinPower)
    {
        return true;
    }
    bool CFactory_RFVFS::TDSCDMA_NS_MesurmentAll(TDS_NS_MaxPwrTestValue& TDtestValue)
    {
        return true;
    }
    /* WCDMA */
    //! 初始化WCDMA仪器设备
    bool CFactory_RFVFS::WCDMA_InitBSE()
    {
        return true;
    }
    bool CFactory_RFVFS::WCDMA_NS_MinPwr(float& fChannPwr)
    {
        return true;
    }
    bool CFactory_RFVFS::WCDMA_NS_MesurmentAll(W_NS_MaxPwrTestValue& TDtestValue)
    {
        return true;
    }
    /* CDMA */
    //! 初始化CDMA仪器设备
    bool CFactory_RFVFS::CDMA_InitBSE()
    {
        return true;
    }
    bool CFactory_RFVFS::CDMA_NS_MinPwr(float& fChannPwr)
    {
        return true;
    }

    bool CFactory_RFVFS::CDMA_NS_ACPR( int &OutOfTolerance )
    {
        return true;
    }

    bool CFactory_RFVFS::CDMA_2000_NS_MesurmentAll( C_NS_MaxPwrTestValue &testValue )
    {
        return true;
    }

    /* GSM */
    bool CFactory_RFVFS::GSM_InitBSE()
    {
        return true;
    }

    bool CFactory_RFVFS::GSM_MesurmentAll(int nPowerLevel,float nCellBerPower,GSM_NS_TestValue &sGsm_TestValue)
    {
        return true;
    }

    bool CFactory_RFVFS::GSM_MesurmentBer(float nCellPower,int nBerCount,float &fBer)
    {
        return true;
    }

    bool CFactory_RFVFS::FinalizeBSE()
    {
        return true;
    }

    //获取LTE波形文件
    const char* CFactory_RFVFS::LTE_GetWaveFile( E_LTE_MODE eLteMode/* = E_LTE_FDD*/ )
    {
        return "";
    }
    //！检查信号发生器是否已经处于开启状态
    bool CFactory_RFVFS::CheckGPRFGenStateIsOn()
    {
        return true;
    }
    bool CFactory_RFVFS::CheckMeasIsComplete()
    {
        return true;
    }
}