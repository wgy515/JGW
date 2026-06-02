#pragma once
#include <string>
#include "CTSECalConfigDefine.h"
namespace JGW
{
    /*
    GoldenUnitsTestLogPath 表示金机在是实验室环境下测试数据路径(测试数据的格式每个项目定义可能都不同)
    DutStationCalDataFilePath 表示DUT在工厂测试环境下使用的线损
    */
    class CCTSEGuCalBase
    {
    public:
        //! 加载GuCalConfig.ini配置文件
        virtual bool LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam) = 0; //! 加载金机校准相关配置
        //! 获取DUT在工厂环境下测试数据路径
        virtual std::wstring GetDutTestDataFilePath() = 0;
        //! 生成新的校准后的线损文件路径
        //！strDutTestDataFilePath DUT测试数据文件
        virtual std::wstring SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath) = 0; //! 保存金机校准配置,加载保存金机数据配置文件路径
        // 当点击开始测试时候，回调
        virtual bool OnStartDutTestThread() { return true;}
    };
}
