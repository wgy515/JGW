#pragma once
#include <UiLib/UIlib.h>
/*#include <JGW_TestRecord/CJGW_TestRecordImpl.h>*/
#ifdef JGW_TESTINTERFACE_API
#undef JGW_TESTINTERFACE_API
#endif

#ifdef JGW_TESTINTERFACE_EXPORTS
#define JGW_TESTINTERFACE_API __declspec(dllexport)
#else
#define JGW_TESTINTERFACE_IMPORTS
#define JGW_TESTINTERFACE_API __declspec(dllimport)
#endif

#ifdef JGW_TESTINTERFACE_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"JGW_TestInterface_d.lib")
#   else
#       pragma comment(lib,"JGW_TestInterface.lib")
#   endif
#endif

namespace JGW
{
    enum E_TEST_STATUS_TYPE
    {
        E_TEST_IDEL,
        E_TEST_BUSY,
        E_TEST_PASS,
        E_TEST_FAIL
    };
}

#ifdef __cplusplus
extern "C" 
{
#endif
    //! 初始化测试接口
    JGW_TESTINTERFACE_API bool InitTestInterface(CPaintManagerUI* pPaintManagerUI,const wchar_t* strModuleFolder,const wchar_t* strRecordConfigName);
    //! 测试状态改变
    JGW_TESTINTERFACE_API void ChangeTestStatus(JGW::E_TEST_STATUS_TYPE eTestStatusType);
    //! 更新测试时间
    JGW_TESTINTERFACE_API void UpdateTestTime();
    //! 清空测试记录
    JGW_TESTINTERFACE_API void ClearRecordData();

    JGW_TESTINTERFACE_API bool SaveTestLogSelectChangedMsg();
         
#ifdef __cplusplus
};
#endif