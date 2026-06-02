#pragma once

#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>
#include "TSE_IPQ807xDefine.h"
#include <JGW_QMSLFuncPlugin/CCJGW_WCNTestDefine.h>
//#include <QMSL_Connectivity/inc/QMSL_Connectivity_WLAN/QMSL_Connectivity_WLAN_Atheros.h>
namespace JGW
{
    class CCJGW_IPQ807xPhone
    {
     private:
        CCJGW_IPQ807xPhone(void);
        ~CCJGW_IPQ807xPhone(void);
    public:
        static CCJGW_IPQ807xPhone* GetIPQ807xPhone();
    public:
        bool FindConnectAvailablePhonesHandleID(const std::string& strComPort);
        void InitQMSLLibrary(QLIB_TargetType_Enum type);
        void DisConnectDut();
        bool WlanSetInstance(WLAN_INSTANCE instance);
        bool WlanSetLocalMac(const std::string& macAddr1 = "000000000000", 
            const std::string& macAddr2 = "000000000000", 
            const std::string& macAddr3 = "000000000000", 
            const std::string& macAddr4 = "000000000000", 
            const std::string& macAddr5 = "000000000000", 
            const std::string& macAddr6 = "000000000000", 
            const std::string& macAddr7 = "000000000000", 
            const std::string& macAddr8 = "000000000000", 
            const std::string& macAddr9 = "000000000000");
        //! 
        bool WlanXtalCalInit2(PhyID phyId, int dXtalTargetPPM, size_t dXtalTolerancePPM, WLAN_XTAL_STORAGE_OTP dXtalCalOtpSaveOption, size_t settleTime = 1000, size_t delayTime = 2000,bool pmic = false);
        //! 
        bool WlanXtalCalibration2(PhyID phyId, size_t channel, WLAN_SINGLE_CHAIN_MASK txChainMask, size_t gainIndex, int dacGain, int paCfg, size_t numAverages, size_t& xtalCapIn,  size_t& xtalCapOut,  double& freqErrPPM,  bool& status, bool ppmCurveDetection = false,bool pmic = false);
        //! 
        bool WlanTxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, WLAN_TpcCalScheme calScheme, uint totalNumOfChains, uint multiChainMask, WLAN_TpcBand band, uint numAverages, bool& status, bool VsaOnePort = true, double rangeDb1ptCalUL = 3.0, double rangeDb1ptCalLL = -3.0);
        //! 
        bool WlanRxCalibrationFWControlled2(WLAN_UniversalPhyID phyId, uint chainToCalibrate, BAND band, WLAN_Action PromiscuousMode,bool& status, const std::string& rxMacAddress = "010000C0FFEE", const std::string& bssId = "020000C0FFEE");
        //! 
        bool WlanNoiseFloorCalGenBDF(PhyID PhyId, const std::vector<unsigned short>& vChannel, WLAN_CHAIN_MASK2 chainMask, WLAN_CHAIN_MASK2 chainMaskH, bool updateNfToBdf);
        //! 
        bool BoardDataGetSize(std::string& BoardDataSize);
        //!
        bool WlanSaveBDF2(BDF_OPTION saveOption, const std::string& bdfFileName);
        //
        bool WlanSetDutTxMacAddressBssId(const std::string& rxStation, const std::string& txStation, const std::string& bssId, uint deviceIndex = 0);
    private:
        void NfCal2(PhyID phyId, uint chainMask, unsigned short freq,  uint* nfcal_values);
        ///!
        void NfCalSave(PhyID phyId, const std::vector<unsigned short>& vChannel, unsigned short NFdB2_Value[16][16]);

        bool SetPhyId(PhyID newPhyId);

        bool tlv2AddRspParam(const std::string& key,char data[]);

        bool tlv2AddRspParam(const std::string& key,const std::string& data);

        bool tlvAddRspParam(const std::string& key, const std::string& data);

        bool tlvAddRspParam(const std::string& key, char data[]);

        void tlv2AddRspParam(std::string key, const std::vector<unsigned short>& data, int dataLength);

        std::string tlv2GetRspParam(const std::string& key);

        std::string tlvGetRspParam(const std::string& key);
    public:
        HANDLE mhResourceContext;
        size_t mXtalTolerancePPM;
        int mXtalTargetPPM;
        int m_fc_index;
        //size_t m_txChainMask;
        _FREQ_CAP m_freqCap[16];
    };
}
