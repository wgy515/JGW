#pragma once
#include "CTSEGuCalBase.h"
#include "CTSECalConfigDefine.h"
#include <map>
#include <vector>

namespace JGW
{
    //typedef struct
    //{
    //    //! 功率集合
    //    std::vector<float> mvPower;
    //}S_GU_CHANNEL_POWERS,*LPS_GU_CHANNEL_POWERS;

    typedef struct
    {
        //! chain mask
        //! int mnChainMask;
        //! channel,power collection
        //std::vector<S_GU_CHANNEL_POWERS> mvChannelPowerCollection;
        std::map<int, std::vector<float>> mmapChannelPowerCollection;
    }S_GU_CHANNLE_POWER_LIST;


    /*typedef struct
    {
        //! sn
        //! std::wstring mstrSerialNumber;
        //! sn 对应的GU WIFI测试数据
        std::map<int, std::vector<S_GU_CHAINMASK_CHANNLE_POWER_MAP>>
        //std::vector<S_GU_CHAINMASK_CHANNLE_POWER_MAP> mvGuChainMaskChannelPowerMap;
    }S_WIFI_GU_TEST_PARAM;*/

    class CCTSEGuCalWiFiIPQ40xx : public CCTSEGuCalBase
    {
    public:
        CCTSEGuCalWiFiIPQ40xx(void);
        ~CCTSEGuCalWiFiIPQ40xx(void);
    private:
        //! 加载金机校准配置文件
        bool LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam);
        //! 保存金机校准配置
        std::wstring SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath);
        //! 获取金机测试数据路径
        std::wstring GetDutTestDataFilePath();
    private:
        //! 生成金机线损配置文件
        //! BH0_LP - BH1_LP - BH2_LP - BH3_LP
        //! 2412 - 2437 - 2462 - 4920 - 5180 - 5500 - 5825
        bool GenGuLossConfigFile(const std::wstring& strLossConfigFilePath,std::map<int,std::map<int,float>> mapChainMaskChannelLoss);
        //! 加载WIFI金机测试数据
        bool LoadWiFiGuTestData(const std::wstring& strDutTestDataFilePath,std::map<int,S_GU_CHANNLE_POWER_LIST>& mapChainMaskChannelPowerList);
        //! 将金机数据转换为线损数据
        bool MapChainMaskChannelPowerListConvertWiFiLossData (std::map<int,S_GU_CHANNLE_POWER_LIST>& mapChainMaskChannelPowerList,std::map<int,std::map<int,float>>& mapChainMaskChannelLoss);
    private:
        bool GetLossToPowersGuLoss(int nChannel,std::vector<float>& vfPower,float fGuLoss,float& fLoss);
    private:
        int mn5GExpectPower;
        int mn24GExpectPower;
        float mfGuLoss;
        std::wstring mstrDutTestDataFilePath;
        std::wstring mstrWiFiLossConfigFilePath;
        std::wstring mstrDefaultGuLossConfigFilePath;
        //! TxChainMask,Channel,Loss
        //std::map<int,std::map<int,float>> mmapChainMaskChannelLoss;
    };
}
