#pragma once
#include "CCJGW_WCNTestDefine.h"
#include <vector>
namespace JGW
{
    class CCJGW_WCNTestBoxInterface
    {
    public:
        /*virtual bool ConnectWCNTestBox(const std::string& host,uint port,const std::string& source)
        {
        return true;
        }*/

        virtual double getCWFreqErr(double chFreqMHz, double powerLevelDBm, unsigned int dutPortMask = 1) = 0;

        virtual bool GetPower2(double chFreqMHz, uint chainMask, WLAN_DataRate rate, double* powerLevelDBm, uint numAvg,  double* oPower) = 0;

        virtual bool MeasurePower(WLAN_RATE rate, double channelFreqMhz, std::vector<double>& vPowerLevelDbm, ChainMask txChainMask, TriggerType triggerType, double* pathLossDb, std::vector<double>& vsaTriggerLevelOffsetDbr, bool shortGuardInterval, double* powerAvgEachBurst, double* powerPkEachBurst, int vsaAverages = 1) = 0;

        virtual uint sigGenTransmit(int inputSignalStrength_dBm, uint freq, WLAN_DataRate rate, uint numPackets, uint chain) = 0;

        virtual void mapWlanRate(WLAN_DataRate wlanRate, bool short11b_nGuard,WLAN_RATE& rate,WLAN_Gen6_PHYDBG_PreambleRate& preambleType, uint channel2 = 0u) = 0;
        //void DisconnectFromSwitch();
        //void ResetPaths();
        ////void SetBTPath();
        ////void SetFMPath(FM_ANTENNA antennaPath);

        //void SetWLANPath(WLAN_CHAIN_MASK2 wlanChainMask, WLAN_DIVERSITY_ANTENNA diversityAntenna = WLAN_DIVERSITY_ANTENNA.A, WLAN_TX_RX_PORT wlanPort = WLAN_TX_RX_PORT.TX);
    };
}