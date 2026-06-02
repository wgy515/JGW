#pragma once
#include <JGW_NIGPIBControllImpl/iequipcommtl.h>
#include <JGW_Device_RohdeSchwarz_CMW500/JGW_Device_RohdeSchwarz_CMW500_Define.h>
namespace JGW
{
    enum E_MONTIOR_TYPE
    {
        E_CMW500,
        E_CMU200,
        E_8960,
    };
    class JGW_DEVICE_ROHDESCHWARZ_CMW500_API CFactory_RFVFS
    {
    public:
        CFactory_RFVFS();
        virtual ~CFactory_RFVFS();

        //static void SetLog4IxPtr(void* pIxLogWriter);
        //设置GPIB 仪器接口地址
       // virtual void SetGPIB(BYTE bGPIBType, DWORD dwBoardIndex, DWORD dwPrimaryAddress) = 0;
        virtual void SetGPIB(BYTE bGPIBType = NI_GPIB_TYPE, DWORD dwBoardIndex = 0, DWORD dwPrimaryAddress = 20,DWORD secAddress = 96) = 0;

        virtual void SetTCPIP(const char* strIPAddress,int nSubInstrument = 0) = 0;

        virtual void SetVisa(const char* visaAddress = "TCPIP0::CMW50050-134371::inst0::INSTR") = 0;

        virtual void SetVisatToDeviceName(const char* strDeviceName,int nSubInstrument = 0) = 0;
        //! 
        virtual E_MONTIOR_TYPE GetMontiorType() = 0;
        //！设置Tx线损
        virtual void SetTxLoss( float fTxLoss = 1.00f );
        //! 设置Rx线损
        virtual void SetRxLoss( float fRxLoss = 1.00f );
        //! 仪器复位以及 删除线损表
        virtual bool ResetMode( );
        //! 初始化仪器
        virtual bool InitMode();
        //! 设置仪器输入输出口
        virtual bool SetInOutPutPort( int nTxNumb,int nRxNumb = ERFAC);

        virtual void SetInstrumentSys(E_SYS_NAME esysName) {m_eSysName = esysName;}
        //! 设GPRF上下行频率
        virtual bool SetGPRFDlUlFreq(double dFreqDL, double dFreqUL,float nCellPower = -65.00f,float nEnPower = 23.00f);
        virtual bool SetGPRFDlUlFreq(S_BAND_CHANNEL_INFO& sBandChannelInfo,float nCellPower = -65.00f ,float nEnPower  = 23.00f );

        //! 设置信号发生器小区功率，以及仪器期望功率
        virtual bool SetCellPoweAEnPower( float nCellPower = -65.00f,float nEnPower = 23.00f );
        //! 设Band信息
        virtual bool SetGPRFBand(int nBand);
        //！开启仪器测量
        virtual void OpenMEValuation();

        virtual void CloseMEValuation();

        virtual bool SetCellPower( float fCellPower = -65.00f);

        virtual bool GPS_InitBSE( int nTxNumb = ERFAC,float fCellPower = -130,double dFreqDL = 1575.42);
        /*	LTE制式所独有	*/
        //！设置LTE频段和模式  开启信号发生器
        virtual bool LTE_SetBandAndMode(int nBand);
        //! 初始化LTE测试环境
        virtual bool LTE_InitMeas();
        //! 开始测试所有LTE参数
        virtual bool LTE_NS_MesurmentAll(LTE_NS_MaxPwrTestValue &testValue);

        virtual bool LTE_NS_GetMinPower(float &fMinPower);


        /*  TDSCDMA */
        //! 初始化TDSCDMA仪器设备
        virtual bool TDSCDMA_InitBSE();

        virtual bool TDSCDMA_NS_GetMinPower(double& fMinPower);

        virtual bool TDSCDMA_NS_MesurmentAll(TDS_NS_MaxPwrTestValue& TDtestValue);
        /* WCDMA */
        //! 初始化WCDMA仪器设备
        virtual bool WCDMA_InitBSE();

        virtual bool WCDMA_NS_MinPwr(float& fChannPwr);

        virtual bool WCDMA_NS_MesurmentAll(W_NS_MaxPwrTestValue& TDtestValue);
        /* CDMA */
        //! 初始化CDMA仪器设备
        virtual bool CDMA_InitBSE();

        virtual bool CDMA_NS_MinPwr(float& fChannPwr);

        virtual bool CDMA_NS_ACPR( int &OutOfTolerance );

        virtual bool CDMA_2000_NS_MesurmentAll( C_NS_MaxPwrTestValue &testValue );

        /* GSM */
        virtual bool GSM_InitBSE();

        virtual bool GSM_MesurmentAll(int nPowerLevel,float nCellBerPower,GSM_NS_TestValue &sGsm_TestValue);

        virtual bool GSM_MesurmentBer(float nCellPower,int nBerCount,float &fBer);

        virtual bool FinalizeBSE();

        //获取LTE波形文件
        virtual const char* LTE_GetWaveFile( E_LTE_MODE eLteMode = E_LTE_FDD );
        //！检查信号发生器是否已经处于开启状态
        virtual bool CheckGPRFGenStateIsOn();
        virtual bool CheckMeasIsComplete();
    protected:
        IEquipCommTL*	m_pEquipInter;
        E_LTE_CH_BW_TYPE m_eLteChBWType; 
        E_SYS_NAME		m_eSysName;
    };
}