#pragma once
#include "CTSEGuCalBase.h"
#include <map>
#include <vector>
namespace JGW
{
    typedef struct  
    {
        std::string mstrSerialNumber;
        std::string mstrTestDataFilePath;
    }S_QIA_WLAN_TESTDATA_FILE_INFO;

	class CCTSEGuCalWlanLoss : public CCTSEGuCalBase
	{
	public:
		CCTSEGuCalWlanLoss(void);
		~CCTSEGuCalWlanLoss(void);
	private:
		//! 加载GuCalConfig.ini配置文件
		virtual bool LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam);
        //! 获取DUT在工厂环境下测试数据路径
		virtual std::wstring GetDutTestDataFilePath();
        //! 生成新的校准后的线损文件路径
		virtual std::wstring SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath);
    private:
        std::wstring mstrGoldenUnitsTestDataFolder;
        std::wstring mstrDutStationCalDataFilePath;
        std::wstring mstrSaveDutStationCalDataFilePath;
        std::wstring mstrDutTestLogFloder;
        std::vector<S_QIA_WLAN_TESTDATA_FILE_INFO> mvGoldenUnitsDataFiles;
	};
}
