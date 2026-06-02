#pragma once
#include <JGW_NIGPIBControllImpl/iequipcommtl.h>
#include <JGW_Device_RohdeSchwarz_CMW500/JGW_Device_RohdeSchwarz_CMW500_Define.h>

#include "../../../platform_include/JGW_WIFI_Define.h"

namespace JGW
{
    enum E_MONTIOR_TYPE
    {
        E_CMW500,
        E_CMU200,
        E_8960,
    };
    //! Tx  是仪器的发射 Rx是仪器的接收
    class JGW_DEVICE_ROHDESCHWARZ_CMW500_API JGW_CMW500EquipmentControll
    {
    public:
        JGW_CMW500EquipmentControll(void);
        ~JGW_CMW500EquipmentControll(void);
    public:
        virtual E_MONTIOR_TYPE GetMontiorType() { return E_CMW500;}

        virtual void SetInstrumentSys(E_SYS_NAME esysName) {m_eSysName = esysName;}
        //! inst_number default 0 SubInstrument 1 / 2 : SubInstrument 1
        void SetTCPIP(const char* strIPAddress,int nSubInstrument = 0);

        void SetVisa(const char* visaAddress = "TCPIP0::CMW50050-134371::inst0::INSTR");

        void SetVisatToDeviceName(const char* strDeviceName,int nSubInstrument = 0);

        void SetGPIB(BYTE bGPIBType = NI_GPIB_TYPE, DWORD dwBoardIndex = 0, DWORD dwPrimaryAddress = 20,DWORD secAddress = 96);
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
        //! 设置仪器的RX端口
        void SetRxInPutPort(int nRxNumb = ERFAC);
        //!
        void SetTxOutPutPort(int nTxNumb = ERFAC);
        //!
        bool CheckWriteEquipComm(std::string& strCommand);

        void OpenMEValuation();

        void StopMEValuation();

        void CloseMEValuation();

        bool CheckMeasIsComplete();

        bool GetRawData(const char *pbuf,char pData[][20]);

        BOOL GetRawData(const char *pbuf,float *dData);

        bool CheckGPRFGenStateIsOn();
        //! WIFI
    public:
        bool WLAN_InitMode();

        void WLAN_SetFrequency(int dFreq);
        void WLAN_SetWlanMode(/*E_WLAN_MODE eWlanMode*/E_WLAN_HT_MODE eWlanHtMode);
        void WLAN_SetExpectedPowerAndUserMarg(float fPower,float fUsermarg);
        void WLAN_InitMeasurement();
        bool WLAN_GetTestResult(E_WLAN_MODE eWlanMode,float *fModuaData,float *fSpeMask);
        //bool WLAN_GetTestResult(E_WLAN_MODE eWlanMode,float *fModuaData,float *fSpeMask,float* fPeakModuaData,float* fPeakSpeMask);

        const char* WLAN_GetWaveform(E_WLAN_RX_DATA_RATE nDataRate);
        bool WLAN_InitRxTest(int nChannel,float fLevel,E_WLAN_RX_DATA_RATE nDataRate);
        void WLAN_CloseRxTest();

    public:
        //! 设GPRF上下行频率
        bool SetGPRFDlUlFreq(double dFreqDL, double dFreqUL,float nCellPower = -65.00f,float nEnPower = 23.00f);
        bool SetGPRFDlUlFreq(S_BAND_CHANNEL_INFO& sBandChannelInfo,float nCellPower = -65.00f ,float nEnPower = 23.00f );

        //! 设置信号发生器小区功率，以及仪器期望功率
        bool SetCellPoweAEnPower( float nCellPower = -65.00f,float nEnPower = 23.00f );
        //! 设Band信息
        bool SetGPRFBand(int nBand);

    public:
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
    private:
        E_SYS_NAME		m_eSysName;
        E_LTE_CH_BW_TYPE m_eLteChBWType;
        IEquipCommTL* mpEquipCommTL;
    };
}