#pragma once
#include "CTSEGuCalBase.h"
#include "CTSECalConfigDefine.h"
#include <map>
#include <vector>

#define INVAID_POWER -99999
namespace JGW
{
    typedef std::map<std::wstring,std::map<int,std::vector<float>>> _mapFreqPathIDPower;
    typedef std::map<int,std::vector<float>> _mapPathIDPowerVec;

    class CCTSEGuCalRF : public CCTSEGuCalBase
    {
    public:
        CCTSEGuCalRF(void);
        ~CCTSEGuCalRF(void);
    private:
        //! 加载GuCalConfig.ini配置文件
        virtual bool LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam); //! 加载金机校准相关配置
        //! 获取DUT在工厂环境下测试数据路径
        virtual std::wstring GetDutTestDataFilePath();
        //! 生成新的校准后的线损文件路径
        virtual std::wstring SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath); //! 保存金机校准配置,加载保存金机数据配置文件路径
        virtual bool OnStartDutTestThread();
    private:
        //！根据频率以及PATH ID获取金机功率
        float GetPowerToGuData(int pathID,const std::wstring& strFreq);
        //! 生成DUT线损配置文件
        bool GenDutLossConfigFile(const std::wstring& strLossConfigFilePath,std::map<int,std::map<std::wstring,float>> mapChainMaskChannelLoss);
        //! 加载DUT测试数据文件
        bool LoadDutTestDataFile(const std::wstring& strDutTestDataFilePath,_mapFreqPathIDPower& mmapTestData);
        //! 将金机数据转换为线损数据
        bool MapChainMaskChannelPowerListConvertWiFiLossData (_mapFreqPathIDPower& mmapTestData,std::map<int,std::map<std::wstring,float>>& mapChainMaskChannelLoss);
        bool GetLossToPowersGuLoss(std::vector<float>& vfPower,float fGuPower,float fGuLoss,float& fLoss);
    private:
        size_t mnMaxPath;
        std::wstring mstrDutTestDataFilePath;
        std::wstring mstrWiFiLossConfigFilePath;
        std::wstring mstrGuDefaultFreq;
        PS_TSE_CALCONFIG_TEST_CONFIG_PARAM mpsCalConfigTestConfigParam;
        std::wstring mstrDefaultGuLossConfigFilePath;
    };
}
