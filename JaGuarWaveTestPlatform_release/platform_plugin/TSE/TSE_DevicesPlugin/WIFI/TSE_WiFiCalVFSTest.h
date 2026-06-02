#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_WiFiCalVFSTest : public CTSE_TestBase
    {
    public:
        CTSE_WiFiCalVFSTest(void);
        ~CTSE_WiFiCalVFSTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 测试错误是否停止当前测试项目
        bool mbErrorStop;
        //! 单项错误重测次数
        size_t mnErrorReTest;
        //！WIFI校准综测DLL文件路径
        std::wstring mstrLibraryFilePath;
        //！WIFI校准综测配置文件路径
        std::wstring mstrConfigFilePath;
        //！WIFI校准综测仪器配置文件路径
        std::wstring mstrEquipConfig;
        //! WIFI校准综测 线损配置文件路径
        std::wstring mstrLossConfig;
        //! SN环境变量值
        std::wstring mstrSNEnv;
    };
}