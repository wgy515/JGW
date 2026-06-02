#include "StdAfx.h"
#include "CJGW_WlanDutWrapper.h"
#include "JGW_Phone.h"
namespace JGW
{
    CCJGW_WlanDutWrapper::CCJGW_WlanDutWrapper(void)
    {
    }


    CCJGW_WlanDutWrapper::~CCJGW_WlanDutWrapper(void)
    {
    }

	void CCJGW_WlanDutWrapper::SwitchLossPathToChainMask(bool tx,int chainMask)
	{
		GETINSTRUMENT->SwitchLossPath(tx,chainMask);
	}

    bool CCJGW_WlanDutWrapper::GetCWFreqErrPower(double chFreqMHz, double powerLevelDBm,double& cwFreqErrorHz,double& cwPower)
    {
        cwFreqErrorHz = INVAILD_WLAN_RESULT;
        cwPower = INVAILD_WLAN_RESULT;
        GETINSTRUMENT->MeasureCWFreqErrPower(chFreqMHz,powerLevelDBm,1,cwFreqErrorHz,cwPower);
        return cwFreqErrorHz != INVAILD_WLAN_RESULT && cwPower != INVAILD_WLAN_RESULT;
    }

    double CCJGW_WlanDutWrapper::GetCWFreqErr(double chFreqMHz, double powerLevelDBm)
    {
        double cwFreqErrorHz = INVAILD_WLAN_RESULT;
        GETINSTRUMENT->MeasureCWFreqErr(chFreqMHz,powerLevelDBm,1,cwFreqErrorHz);
        return cwFreqErrorHz;
    }

    double CCJGW_WlanDutWrapper::GetAvgPowerDelegate(WLAN_RATE rate, double chFreqMHz, double powerLevelDBm,int iTriggerLevel,int vsaAverages /* = 1 */)
    {
        uint dutPortMask = PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)PHONE->m_txChainMask);
        return GETINSTRUMENT->GetAvgPower(dutPortMask,rate,static_cast<uint>(chFreqMHz),powerLevelDBm,iTriggerLevel,vsaAverages);  
    }

	double CCJGW_WlanDutWrapper::GetCurrentConfigAvgPowerDelegate()
	{
		return GETINSTRUMENT->GetCurrentConfigAvgPower();
	}

    double CCJGW_WlanDutWrapper::GetCWAvgPowerDelegate(double chFreqMHz, double powerLevelDBm,int iTriggerLevel,int vsaAverages/* = 1*/)
    {
        uint dutPortMask = PHONE->GetSISOChainNumber((WLAN_SINGLE_CHAIN_MASK)PHONE->m_txChainMask);
        return GETINSTRUMENT->GetAvgPower(dutPortMask,RATE_11A_6_MBPS,static_cast<uint>(chFreqMHz),powerLevelDBm,iTriggerLevel,vsaAverages);  
    }
}

