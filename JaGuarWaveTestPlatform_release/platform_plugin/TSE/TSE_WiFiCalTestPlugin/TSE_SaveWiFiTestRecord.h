#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_WlanExcelTemplateConfig.h"

namespace JGW
{
    typedef enum
    {
        E_TEMP_NORMAL_STATUS = 0,
        E_TEMP_HIGH_STATUS = 1,
        E_TEMP_LOW_STATUS = 2
    }E_TEMP_STATUS;
    //! 保存EXCEL目录
    class CTSE_SaveWiFiTestRecord : public CTSE_TestBase
    {
    public:
        CTSE_SaveWiFiTestRecord(void);
        ~CTSE_SaveWiFiTestRecord(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
        bool TSE_Exit();
    private:
        //! excel 解析配置文件
        std::wstring mstrExcelParseIniFilePath;
        //! excel 模板文件
        std::wstring mstrExcelTemplateFilePath;
        //! 温度状态 0:常温 1:高温 2:低温
        std::wstring mstrTemperatureStatusEnvironment;
        //! WIFI测试数据记录实例环境变量
        std::wstring mstrWiFiTestResultRecordEnvironment;
        //! TSE_DATE_TIME 2022-04-08 TSE_TIME_STAMP 1356897892
        std::wstring mstrSaveExcelFilePathCommand;
        //! 
        std::vector<S_EXCEL_WIFI_POS> mvsExcelWiFiPos;
        //!
        CCJGW_ExcelLibXL mcExcelLibXL;
    };

}

