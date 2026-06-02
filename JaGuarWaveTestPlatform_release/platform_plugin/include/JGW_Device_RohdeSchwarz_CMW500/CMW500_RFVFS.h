#pragma once
#include "CFactoty_RFVFS.h"
#include <JGW_Device_RohdeSchwarz_CMW500/JGW_Device_RohdeSchwarz_CMW500_Define.h>
namespace JGW
{

    //此类Tx Rx  相对于设备来说为Rx Tx 非信令默认值测试10M  以及只测试QPSK
    class JGW_DEVICE_ROHDESCHWARZ_CMW500_API CMW500_RFVFS:public CFactory_RFVFS
    {
    public:
        CMW500_RFVFS();
        ~CMW500_RFVFS();

        E_MONTIOR_TYPE GetMontiorType(){ return E_CMW500;}
        //static void SetLog4IxPtr(void* pIxLogWriter);
        //! inst_number default 0 SubInstrument 1 / 2 : SubInstrument 1
        void SetTCPIP(const char* strIPAddress,int nSubInstrument = 0);

        void SetVisa(const char* visaAddress = "TCPIP0::CMW50050-134371::inst0::INSTR");

        void SetVisatToDeviceName(const char* strDeviceName,int nSubInstrument = 0);

        void SetGPIB(BYTE bGPIBType = NI_GPIB_TYPE, DWORD dwBoardIndex = 0, DWORD dwPrimaryAddress = 20,DWORD secAddress = 96);

        //设置GPIB 仪器接口地址
        //!void SetGPIB(BYTE bGPIBType, DWORD dwBoardIndex, DWORD dwPrimaryAddress);
        //！设置Tx线损
        void SetTxLoss( float fTxLoss = 1.00f );
        //! 设置Rx线损
        void SetRxLoss( float fRxLoss = 1.00f );
        //! 仪器复位以及 删除线损表
        bool ResetMode( );
        //! 初始化仪器
        bool InitMode();
        //! 设置仪器输入输出口
        bool SetInOutPutPort( int nTxNumb,int nRxNumb = ERFAC );
        //! 设GPRF上下行频率
        bool SetGPRFDlUlFreq(double dFreqDL, double dFreqUL,float nCellPower = -65.00f,float nEnPower = 23.00f);
        bool SetGPRFDlUlFreq(S_BAND_CHANNEL_INFO& sBandChannelInfo,float nCellPower = -65.00f ,float nEnPower = 23.00f );

        //! 设置信号发生器小区功率，以及仪器期望功率
        bool SetCellPoweAEnPower( float nCellPower = -65.00f,float nEnPower = 23.00f );
        //! 设Band信息
        bool SetGPRFBand(int nBand);
        //！开启仪器测量
        void OpenMEValuation();

        void CloseMEValuation();

        bool SetCellPower( float fCellPower = -65.00f);

        bool GPS_InitBSE( int nTxNumb = ERFAC,float fCellPower = -130,double dFreqDL = 1575.42);
        /*	LTE制式所独有	*/
        //！设置LTE频段和模式  开启信号发生器
        bool LTE_SetBandAndMode(int nBand);
        //! 初始化LTE测试环境
        bool LTE_InitMeas();
        //! 开始测试所有LTE参数
        bool LTE_NS_MesurmentAll(LTE_NS_MaxPwrTestValue &testValue);

        bool LTE_NS_GetMinPower(float &fMinPower);


        /*  TDSCDMA */
        //! 初始化TDSCDMA仪器设备
        bool TDSCDMA_InitBSE();

        bool TDSCDMA_NS_GetMinPower(double& fMinPower);

        bool TDSCDMA_NS_MesurmentAll(TDS_NS_MaxPwrTestValue& TDtestValue);
        /* WCDMA */
        //! 初始化WCDMA仪器设备
        bool WCDMA_InitBSE();

        bool WCDMA_NS_MinPwr(float& fChannPwr);

        bool WCDMA_NS_MesurmentAll(W_NS_MaxPwrTestValue& TDtestValue);
        /* CDMA */
        //! 初始化CDMA仪器设备
        bool CDMA_InitBSE();

        bool CDMA_NS_MinPwr(float& fChannPwr);

        bool CDMA_NS_ACPR( int &OutOfTolerance );

        bool CDMA_2000_NS_MesurmentAll( C_NS_MaxPwrTestValue &testValue );

        /* GSM */
        bool GSM_InitBSE();

        bool GSM_MesurmentAll(int nPowerLevel,float nCellBerPower,GSM_NS_TestValue &sGsm_TestValue);

        bool GSM_MesurmentBer(float nCellPower,int nBerCount,float &fBer);

        bool FinalizeBSE();
    private:
        //获取LTE波形文件
        const char* LTE_GetWaveFile( E_LTE_MODE eLteMode = E_LTE_FDD );
        //！检查信号发生器是否已经处于开启状态
        bool CheckGPRFGenStateIsOn();
        bool CheckMeasIsComplete();
    };
}