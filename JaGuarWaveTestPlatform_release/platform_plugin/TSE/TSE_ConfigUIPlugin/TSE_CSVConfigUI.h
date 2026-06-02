#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_BaseConfigXML.h>
namespace JGW
{
    /*

    csv 配置
    BEAMCH1Header=OneHotLinkV2RockfinchTxFe0Temp,OneHotLinkV2RockfinchTxFe1Temp,OneHotLinkV2RockfinchTxFe2Temp,OneHotLinkV2RockfinchTxFe3Temp,OneHotLinkV2RockfinchTxFe4Temp,OneHotLinkV2RockfinchTxFe5Temp,OneHotLinkV2RockfinchTxFe6Temp,OneHotLinkV2RockfinchTxFe7Temp
    BEAMCH1Value=
    BEAMCH2Header=OneHotLinkV2RockfinchTxFe0Temp,OneHotLinkV2RockfinchTxFe1Temp,OneHotLinkV2RockfinchTxFe2Temp,OneHotLinkV2RockfinchTxFe3Temp,OneHotLinkV2RockfinchTxFe4Temp,OneHotLinkV2RockfinchTxFe5Temp,OneHotLinkV2RockfinchTxFe6Temp,OneHotLinkV2RockfinchTxFe7Temp
    BEAMCH2Value=

    PrimaryColumnValueIndex=0
    PrimaryColumnNameRowIndex=0
    PrimaryValue=PRM2141-01_PRA7211-01
    csv文件路径
    读取对应参数值：
    在编辑框中显示对应数值：

    校准完成后直接写配置

    设置PST测试标准
    0~7:-30,-31,-32,-33,-34,-35,-36,-37
    */

    typedef struct  
    {
        //! 是否更新到CSV文件中
        bool mbUpdateCsv;
        // 是否只读
        bool mbReadOnly;
        // UI Label tip
        std::wstring mstrLabelName;
        // CSV Column Name
        std::wstring mstrEditValueEnvironment;
        // Global Environment Name
        std::wstring mstrGlobalEnvironmentName;
    }S_TSE_CSV_CONFIG_ITEM,*PS_TSE_CSV_CONFIG_ITEM;


    class CTSE_CSVConfigUI : public CTSE_TestBase
    {
    public:
        CTSE_CSVConfigUI(void);
        ~CTSE_CSVConfigUI(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        size_t mnConfigCount;
        //! 主键行索引
        int mnPrimaryColumnNameRowIndex;
        //! 主键列索引
        int mnPrimaryColumnValueIndex;
        //! CSV 文件路径
        std::wstring mstrCSVFilePathCommand;
        //! 主键值
        std::wstring mstrPrimaryValue;

        std::vector<S_TSE_CSV_CONFIG_ITEM> mvsTSECSVConfigItems;
        std::map<std::wstring,std::wstring> m_mParam;
    };
}


