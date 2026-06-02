#pragma once
#include <poco/ClassLoader.h>
#include <poco/Manifest.h>
#include <poco/SharedPtr.h>
// #include <JaGuarWaveTestPlatform/JGWDuiiLlibSubWndImpl.h>
// #include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JGW_FoundationFunc\JGW_FilePath.h>
#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include <JGW_FoundationFunc\CUtf8String.hpp>
#include <JGW_FoundationFunc/CJGW_DllDirectoryStateSaver.h>
#include <JGW_Log4cplusImplPlugin/log4cplusimpl.h>

namespace JGW
{
    template <class T>
    class CCJGW_TestSuitePocoClassLoader
    {
    public:
        CCJGW_TestSuitePocoClassLoader()
        {

        }

        bool LoadTestSuiteModule(std::string strTestSuiteModule)
        {
            CCJGW_DllDirectoryStateSaver dllDirectoryStateSaver(JGW_GetApplicationFolder());
            std::string strTestSuiteModulePath = U(strTestSuiteModule.c_str());
            //! ¼ÓÔØ²å¼þ
            try
            {
                PDLOG4WA_F( "loadTestSuiteModule(path:%s)",strTestSuiteModule.c_str());
                if (mclassLoaderModule.isLibraryLoaded(strTestSuiteModulePath)) return true;
                mclassLoaderModule.loadLibrary(strTestSuiteModulePath);
                return true;
            }
            catch (Poco::LibraryLoadException& e)
            {
                PELOG4WA_F("loadTestSuiteModule fail(%s)(%s)",e.what(),JGW_GetSystemErrorStringA(GetLastError()).c_str());
                return false;
            }
        }

        T& InstanceTestSuitePtrClass(std::string& strClassName)
        {
            try
            {
                return mclassLoaderModule.instance(strClassName);
            }
            catch ( Poco::NotFoundException& e)
            {
                PELOG4WA_F("CreateTestSuitePtrClass : %s(error:%s)",strClassName.c_str(),e.what());
                return NULL;
            }
        }

        T* CreateTestSuitePtrClass(std::string& strClassName)
        {
            try
            {
                return mclassLoaderModule.create(strClassName);
            }
            catch ( Poco::NotFoundException& e)
            {
                PELOG4WA_F("CreateTestSuitePtrClass : %s(error:%s)",strClassName.c_str(),e.what());
                return NULL;
            }
        }

        Poco::ClassLoader<T>& GetClassLoaderModule()
        {
            return mclassLoaderModule;
        }

    private:
         Poco::ClassLoader<T> mclassLoaderModule;
    };
}