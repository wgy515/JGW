#pragma once
#include <poco/ClassLoader.h>
#include <poco/Manifest.h>
#include <JaGuarWaveTestPlatform/JGWDuiiLlibSubWndImpl.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JGW_FoundationFunc\JGW_FilePath.h>
#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include <JGW_FoundationFunc\CUtf8String.hpp>
#include <JGW_FoundationFunc/CJGW_DllDirectoryStateSaver.h>
#include <JGW_Log4cplusImplPlugin/log4cplusimpl.h>


namespace JGW
{
    template < class T >
    class CJGWLoadSubPlugin
    {
    public:
        CJGWLoadSubPlugin(void):mpSubWndimpl(NULL)
        {

        }

        ~CJGWLoadSubPlugin(void)
        {
            CloseSubPluginView();
            DestroySubPlugin();
        }

        //! 加载子窗体插件
        T *	LoadSubPluginSubWndImpl( LPS_PLUGIN_INFO pPluginInfo )
        {
            for ( int i = 0;i < RELOAD_PLUGIN_COUNTS; ++i)
            {
                Log4WI_F(L"LoadSubPluginSubWndImpl : %s",pPluginInfo->mstrPluginName.c_str());
                if (LoadSubWndPlugin(pPluginInfo)) return CreateSubWndImpl(JGW::JGW_W2A(pPluginInfo->mstrClsid));
            }
            return NULL;
        }

        void CloseSubPluginView()
        {
            if (mpSubWndimpl) 
            { 
                Log4WI_F(L"CloseSubPluginView : %s",mstrPluginPath.c_str());
                mpSubWndimpl->CloseSubWndPlugin(); 
                mpSubWndimpl = NULL;
            }
        }

        void DestroyClassPtr(const std::string& strClsid,T* ptrClass)
        {
            Log4WI_F(L"DestroyClassPtr clsid : %s,ptr : %08X",JGW_A2W(strClsid).c_str(),ptrClass);
            if (0 != msubWndClassLoader.findClass(strClsid))
                msubWndClassLoader.classFor(strClsid).destroy(ptrClass);
            if (ptrClass == mpSubWndimpl) mpSubWndimpl = NULL;
        }

        T* CreateSubWndImpl(const std::string& strClsid)
        {
            std::string strPluginPath = JGW_W2A(mstrPluginPath,CP_UTF8);
            if (!msubWndClassLoader.isLibraryLoaded(strPluginPath)) return NULL;
            Log4WI_F(L"CreateSubWndImpl : %s,clsid: %s",mstrPluginPath.c_str(),JGW_A2W(strClsid).c_str());
            try
            {
                mpSubWndimpl = msubWndClassLoader.create(strClsid);
                msubWndClassLoader.classFor(strClsid).autoDelete(mpSubWndimpl);
            }
            catch ( Poco::NotFoundException& e)
            {
                Log4WE_F(L"CJGWLoadSubPlugin::CreateSubWndImpl:create clsid:%s(error:%s)",JGW_A2W(strClsid).c_str(),JGW_A2W_A(e.what()).c_str());
            }
            return mpSubWndimpl;
        }

        void DestroySubPlugin()
        {
            std::string strPluginPat = JGW_W2A(mstrPluginPath,CP_UTF8);
            if (mstrPluginPath.empty() || !msubWndClassLoader.isLibraryLoaded(strPluginPat)) return;
            try
            {
                Log4WI_F(L"CJGWLoadSubPlugin::FreeSubPlugin:unloadlibrary %s",mstrPluginPath.c_str());
                msubWndClassLoader.unloadLibrary(strPluginPat);
                mstrPluginPath = L"";
            }
            catch (Poco::NotFoundException& e)
            {
                Log4WE_F(L"CJGWLoadSubPlugin::FreeSubPlugin:unloadLibrary(%s)",JGW_A2W_A(e.what(),CP_UTF8).c_str());
            }
        }

        //! 加载子窗体插件，视图加载器也属于子窗体
        bool LoadSubWndPlugin(LPS_PLUGIN_INFO pPluginInfo)
        {
            if (!pPluginInfo) return NULL;
            std::wstring strPluginName = pPluginInfo->mstrPluginName;
            mstrPluginPath = GetSubPluginPath(strPluginName.c_str());
            std::string strPluginPath = JGW_W2A(mstrPluginPath,CP_UTF8);
            CCJGW_DllDirectoryStateSaver dllDirectoryStateSaver(JGW_GetApplicationFolder());
            //! 加载插件
            try
            {
                Log4WI_F(L"LoadSubWndPlugin loadLibrary(path:%s)",mstrPluginPath.c_str());
                //! if (!msubWndClassLoader.isLibraryLoaded(mstrPluginPath))
                
                msubWndClassLoader.loadLibrary(strPluginPath);
            }
            catch ( Poco::LibraryLoadException& e )
            {
                Log4WE_F(L"CJGWLoadSubPlugin::LoadSubWndPlugin loadLibrary fail(%s)(%d)",JGW_A2W_A(e.what()).c_str(), GetLastError());
                ::MessageBox( FindWindow(NULL,NULL),mstrPluginPath.c_str()/*e.what()*/,L"Subform initialization failed",MB_ICONERROR );
                mstrPluginPath = L"";
            }
            return msubWndClassLoader.isLibraryLoaded(strPluginPath);
        }

    private:
        std::wstring GetSubPluginPath( LPCTSTR strPlugin )
        {
            std::wstring strPath;
            JGW::JGW_GetExecSubFolder(strPath,L"plugin");
            return strPath + strPlugin;
        }
    private:
        T* mpSubWndimpl;
        std::wstring	mstrPluginPath;
        Poco::ClassLoader<T> msubWndClassLoader;
    };
}
