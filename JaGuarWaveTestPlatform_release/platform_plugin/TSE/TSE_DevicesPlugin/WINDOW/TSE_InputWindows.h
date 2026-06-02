#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JaGuarWaveTestPlatform/CJGW_InputViewResolver_Define.h>
namespace JGW
{
    class CTSE_InputWindows : public CTSE_TestBase
    {
    public:
        CTSE_InputWindows(void);
        ~CTSE_InputWindows(void);

        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Init(void);

        bool TSE_Run();

    private:
        //! 默认是忽略大小写
        E_STRING_UPPERLOWCASE_TYPE meStringUpperLowCaseType;
        //!
        bool mbTSEInit;
        //! 
        HWND mMainHwnd;
        //! 号段类型ID
        std::wstring mstrNumberType;
        //! 提示
        std::wstring mstrTip;
        //! 输入提示
        std::wstring mstrInputTip;
        //! 输入最大的字符串
        std::wstring mstrMaxChar;
        //! 输入最大的字符串
        std::wstring mstrMinChar;
        //! 输入前缀
        std::wstring mstrInputPrefix;
        //! 环境变量名称
        std::wstring mstrEnvironmentStr;
        //!
        S_ALL_INPUT_UI_ATTR_SETTING msAllInputUIAttrSetting;
    };
}