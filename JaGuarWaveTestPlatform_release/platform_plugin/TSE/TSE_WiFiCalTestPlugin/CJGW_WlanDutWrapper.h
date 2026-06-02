#pragma once
#include <JGW_Device_WCNTester_CommonSCPIWLAN/JGW_IWlanInstrumentInterface.h>

namespace JGW
{
    class CCJGW_WlanDutWrapper
    {
    public:
        CCJGW_WlanDutWrapper(void);
        ~CCJGW_WlanDutWrapper(void);
    public:
		//!
		void SwitchLossPathToChainMask(bool tx,int chainMask);
        //!
        bool GetCWFreqErrPower(double chFreqMHz, double powerLevelDBm,double& cwFreqErrorHz,double& cwPower);
        //! 
        double GetCWFreqErr(double chFreqMHz, double powerLevelDBm);
        //! 
        double GetAvgPowerDelegate(WLAN_RATE rate, double chFreqMHz, double powerLevelDBm,int iTriggerLevel,int vsaAverages = 1);
		//!
		double GetCurrentConfigAvgPowerDelegate();
        //!
        double GetCWAvgPowerDelegate(double chFreqMHz, double powerLevelDBm,int iTriggerLevel,int vsaAverages = 1);
    };
}


