#pragma once
#include <poco/ClassLoader.h>
#include <poco/Manifest.h>
#include <poco/SharedPtr.h>
#include <JaGuarWaveTestPlatform/JGWDuiiLlibSubWndImpl.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JGW_FoundationFunc\JGW_FilePath.h>
#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include <JGW_FoundationFunc\CUtf8String.hpp>
#include <JGW_FoundationFunc/CJGW_DllDirectoryStateSaver.h>
#include <JGW_Log4cplusImplPlugin/log4cplusimpl.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

namespace JGW
{
    template < class T >
    class CCJGW_PocoClassLoader
    {
    public:
        CCJGW_PocoClassLoader(void):mbResult(false)
        {

        }

        ~CCJGW_PocoClassLoader(void)
        {
            std::string strPluginPath = JGW_W2A(mstrPluginPath,CP_UTF8);
            if (!mstrPluginPath.empty() && mclassLoaderModule.isLibraryLoaded(strPluginPath))
            {
                try
                {
                    Log4WD_F(L"CCJGW_PocoClassLoader::FreeSubPlugin:unloadlibrary(%s)",mstrPluginPath.c_str());
                    mclassLoaderModule.unloadLibrary(strPluginPath);
                    mstrPluginPath = L"";
                }
                catch (Poco::NotFoundException& e)
                {
                    Log4WE_F(L"CCJGW_PocoClassLoader::FreeSubPlugin:unloadLibrary(%s)",JGW_A2W_A(e.what()).c_str());
                }
            }
        }

        //! 加载子窗体插件
        /*const*/ T *	LoadPocoClassLoader(S_POCO_CLASS_LOADER_INFO& sPocoClassLoaderInfo)
        {
            for ( int i = 0;i < RELOAD_PLUGIN_COUNTS && !mbResult; ++i)
            {
                Log4WD_F(L"LoadPocoClassLoader Plugin: %s,clsid: %s",sPocoClassLoaderInfo.mstrModuleName.c_str(),sPocoClassLoaderInfo.mstrClsid.c_str());
                mbResult =  LoadModule(sPocoClassLoaderInfo);
            }

            return mbResult?CreateClassLoader():NULL;
        }

        /*const*/ T* CreateClassLoader()
        {
            std::string strPluginPath = JGW_W2A(mstrPluginPath,CP_UTF8);
            if ( !mclassLoaderModule.isLibraryLoaded(strPluginPath) ) return NULL;
            try
            {
                mClassPtr = &mclassLoaderModule.instance(mstrClsid);
            }
            catch ( Poco::NotFoundException& e)
            {
                Log4WE_F(L"CCJGW_PocoClassLoader::CreateSubWndImpl:create clsid:%s(error:%s)",JGW_A2W(mstrClsid).c_str(),JGW_A2W_A(e.what()).c_str());
            }
            return mClassPtr;
        }


        bool LoadModule(S_POCO_CLASS_LOADER_INFO& sPocoClassLoaderInfo)
        {
            mstrPluginPath = JGW_RealativePathToAbsPath(sPocoClassLoaderInfo.mstrModuleName.c_str());
            mstrClsid = JGW_W2A(sPocoClassLoaderInfo.mstrClsid);
            std::string strPluginPath = JGW_W2A(mstrPluginPath,CP_UTF8);
            CCJGW_DllDirectoryStateSaver dllDirectoryStateSaver(JGW_GetApplicationFolder());
            //! 加载插件
            try
            {
                Log4WI_F(L"CCJGW_PocoClassLoader::LoadSubWndPlugin loadLibrary(path:%s)",mstrPluginPath.c_str());
                if ( !mclassLoaderModule.isLibraryLoaded(strPluginPath) )
                    mclassLoaderModule.loadLibrary(strPluginPath);
            }
            catch ( Poco::LibraryLoadException& e )
            {
                Log4WE_F(L"CCJGW_PocoClassLoader::LoadSubWndPlugin loadLibrary fail(%s)(%d)",JGW_A2W_A(e.what()).c_str(), GetLastError());
                ::MessageBox( FindWindow(NULL,NULL),mstrPluginPath.c_str()/*e.what()*/,GetString(E_CHILDFORM_INIT_FAIL_TYPE),MB_ICONERROR );
                mstrPluginPath = L"";
                return false;
            }

            return true;
        }

        void SetClsid( std::wstring& strclsid )
        {
            mstrClsid = JGW::JGW_W2A( strclsid );
        }
//   private:
//         std::wstring GetClassLoaderPluginPath(LPCTSTR strPlugin)
//         {
//             std::wstring strPath(JGW_GetApplicationFolder());
//             return strPath + strPlugin;
//         }
    private:
        std::string									mstrClsid;
        std::wstring								mstrPluginPath;
        T*                                          mClassPtr;
        Poco::ClassLoader<T>						mclassLoaderModule;
        bool										mbResult;
    };
}