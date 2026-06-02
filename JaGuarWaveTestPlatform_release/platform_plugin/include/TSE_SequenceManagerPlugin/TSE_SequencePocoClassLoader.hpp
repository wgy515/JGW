#pragma once
#include <poco/ClassLoader.h>
#include <poco/Manifest.h>
#include <poco/SharedPtr.h>
#include <JGW_FoundationFunc\JGW_FilePath.h>
#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include <JGW_FoundationFunc\CUtf8String.hpp>
#include <JGW_FoundationFunc/CJGW_DllDirectoryStateSaver.h>
#include <vector>
#include <JGW_Log4cplusImplPlugin/log4cplusimpl.h>

namespace JGW
{
    template <class T>
    class CTSE_SequencePocoClassLoader
    {
    public:
        ~CTSE_SequencePocoClassLoader()
        {
        }

        void UnloadTestSuiteModule()
        {
            for (std::vector<std::wstring>::iterator it = mvLoaderLibraryFilePath.begin();
                it != mvLoaderLibraryFilePath.end();
                ++ it)
            {
                std::string strTestSuiteModulePath = JGW_W2A(it[0],CP_UTF8);
                if (mclassLoaderModule.isLibraryLoaded(strTestSuiteModulePath)) mclassLoaderModule.unloadLibrary(strTestSuiteModulePath);
            }
            mvLoaderLibraryFilePath.clear();
        }

        bool LoadTestSuiteModule(const std::wstring& strTestSuiteModule)
        {
            CCJGW_DllDirectoryStateSaver dllDirectoryStateSaver(JGW_GetApplicationFolder());
            std::string strTestSuiteModulePath = JGW_W2A(strTestSuiteModule,CP_UTF8);
            //! ¼ÓÔØ²å¼þ
            try
            {
                JGW_FormatWString(mstrLog,L"loadTestSuiteModule(path:%s)",strTestSuiteModule.c_str());
                if (mclassLoaderModule.isLibraryLoaded(strTestSuiteModulePath)) return true;
                mclassLoaderModule.loadLibrary(strTestSuiteModulePath);
                mvLoaderLibraryFilePath.push_back(strTestSuiteModule);
                return true;
            }
            catch (Poco::LibraryLoadException& e)
            {
                JGW_FormatWString(mstrLog,L"loadTestSuiteModule fail(%s)(%s)",JGW_A2W(e.message(),CP_UTF8).c_str(),JGW_GetSystemErrorString(GetLastError()).c_str());
				Log4WE(mstrLog.c_str());
                return false;
            }
        }

        T* InstanceTestSuitePtrClass(const std::string& strClassName)
        {
            try
            {
                return &mclassLoaderModule.instance(strClassName);
            }
            catch ( Poco::NotFoundException& e)
            {
                //JGW_FormatString(mstrLog,"CreateTestSuitePtrClass : %s(error:%s)",strClassName.c_str(),e.what());
                std::string strTemp;
                JGW_FormatString(strTemp,"CreateTestSuitePtrClass : %s(error:%s)",strClassName.c_str(),e.what());
                mstrLog = JGW_A2W(strTemp,CP_UTF8);
				Log4WE(mstrLog.c_str());
                return NULL;
            }
        }

        T* CreateAutoDeleteTestSuitePtrClassToModuleName(const std::string& strClassName,const std::string& strModuleName)
        {
            const Poco::Manifest<CTSE_TestBase>* pManif = mclassLoaderModule.findManifest(strModuleName);
            if (!pManif) return NULL;
            Poco::Manifest<CTSE_TestBase>::Iterator itm = pManif->find(strClassName);
            if (pManif->end() == itm) return NULL;
            T* ptr = itm->create();
            itm->autoDelete(ptr);
            return ptr;
        }

        T* CreateTestSuitePtrClassToModuleName(const std::string& strClassName,const std::string& strModuleName)
        {
            const Poco::Manifest<CTSE_TestBase>* pManif = mclassLoaderModule.findManifest(strModuleName);
            if (!pManif) return NULL;
            Poco::Manifest<CTSE_TestBase>::Iterator itm = pManif->find(strClassName);
            if (pManif->end() == itm) return NULL;
            T* ptr = itm->create();
            //! itm->autoDelete(ptr);
            return ptr;
        }

        T* CreateTestSuitePtrClassNotAutoDelete(const std::string& strClassName)
        {
            try
            {
                T* ptr = mclassLoaderModule.create(strClassName);
                //! mclassLoaderModule.classFor(strClassName).autoDelete(ptr);
                return ptr;
            }
            catch ( Poco::NotFoundException& e)
            {
                std::string strTemp;
                JGW_FormatString(strTemp,"CreateTestSuitePtrClass : %s(error:%s)",strClassName.c_str(),e.what());
                mstrLog = JGW_A2W(strTemp,CP_UTF8);
				Log4WE(mstrLog.c_str());
                return NULL;
            }
        }

        T* CreateTestSuitePtrClass(const std::string& strClassName)
        {
            try
            {
                T* ptr = mclassLoaderModule.create(strClassName);
                mclassLoaderModule.classFor(strClassName).autoDelete(ptr);
                return ptr;
            }
            catch ( Poco::NotFoundException& e)
            {
                std::string strTemp;
                JGW_FormatString(strTemp,"CreateTestSuitePtrClass : %s(error:%s)",strClassName.c_str(),e.what());
                mstrLog = JGW_A2W(strTemp,CP_UTF8);
				Log4WE(mstrLog.c_str());
                return NULL;
            }
        }

        Poco::ClassLoader<T>& GetClassLoaderModule()
        {
            return mclassLoaderModule;
        }

        const std::wstring& GetLog()
        {
            return mstrLog;
        }

    private:
        std::wstring mstrLog;
        std::vector<std::wstring> mvLoaderLibraryFilePath;
        //! std::vector<std::shared_ptr<T>> mvPtrClass;
        Poco::ClassLoader<T> mclassLoaderModule;
    };
}