#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_SequenceManagerPlugin/TSE_GlobalEnvironment.h>
#include "TSE_LogNet.h"

namespace JGW
{
    class CTSE_SequenceTestNet
    {
    public:
        CTSE_SequenceTestNet(CTSE_LogNet& logServices,CTSE_GlobalEnvironment& globalEnvironment);
        ~CTSE_SequenceTestNet(void);
    public:
        //! 加载TSE模块
        bool LoadTSEModule(const wchar_t* moduleName);
        //! 创建TSE实例
        CTSE_TestBase* CreateTSEInstance(const wchar_t* realName, const wchar_t* moduleName);
        //! 更新TSE
        bool UpdateConfigParamToTSEInstance(CTSE_TestBase* pTestBase,const std::wstring& paramName,const std::wstring& paramValue);
        //！ 
        bool InitTestToTSEInstance(CTSE_TestBase* pTestBase);
        //!
        bool ExecuteTSEInstance(CTSE_TestBase* pTestBase);
        //!
        bool ExitTestToTSEInstance(CTSE_TestBase* pTestBase);
        //! 
        bool UnLoadTSEModule(const std::wstring& moduleName);
    private:
        bool SetTSEModuleGlobalResourcesInfo(const std::wstring& strTSEModuleFilePath);
    private:
        //! TSE 注册的类指针供 TSE DLL回调使用
        CTSE_LogNet& mLogServices;
        //! 
        CTSE_GlobalEnvironment& mGlobalEnvironment;
        //! 共享的全局资源组
        //std::vector<std::shared_ptr<CTSE_TestBase>> mvSuiteGlobalResources;
        //! 
        //std::vector<std::shared_ptr<CTSE_TestBase>> mvAllSuiteTestBase;
        //! TSE DLL加载实例
        CTSE_SequencePocoClassLoader<CTSE_TestBase> mTestSuitePocoClassLoader;
    };
}
