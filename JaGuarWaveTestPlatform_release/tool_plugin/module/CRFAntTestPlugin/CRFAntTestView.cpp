#include "StdAfx.h"
#include "CRFAntTestView.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "configwnd/DuiConfig.h"
#include <fstream>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>

namespace JGW
{
    CCRFAntTestView::CCRFAntTestView(void):mpTestThread(NULL),m_nListItemHeight(30),m_bTestThreadStatus(false)
    {
        msPluginSkin.mstrSkinXmlPath = L"test.xml";
        msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
    }


    CCRFAntTestView::~CCRFAntTestView(void)
    {
    }

    void CCRFAntTestView::CloseSubWndPlugin()
    {
        if (mpTestThread)
        {
            delete mpTestThread;
            mpTestThread = NULL;
        }
        CCJGW_CommanTestPlnView::CloseSubWndPlugin();
    }

    bool CCRFAntTestView::OnIdelTestStatus()
    {
        mpDoMainList = static_cast<CListTestCtrlUI*>(mpPluginPaintManagerUI->FindControl(L"domainlist"));
        if (!mpDoMainList) return false;

        {
            //! mpDoMainList->GetHeader()->RemoveAll();
            mpDoMainList->SetAttribute(L"itemalign",L"center");
            mpDoMainList->GetList()->RemoveAll();
            mpDoMainList->GetHeader()->SetScaleHeader(false);

            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetText(_T("测试项"));
            mpDoMainList->GetHeader()->GetItemAt( 0 )->SetAttribute(L"align",L"center");
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetText(_T("Min"));
            mpDoMainList->GetHeader()->GetItemAt( 1 )->SetAttribute(L"align",L"center");
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetText(_T("Value"));
            mpDoMainList->GetHeader()->GetItemAt( 2 )->SetAttribute(L"align",L"center");
            mpDoMainList->InsertColumn(3,_T("Max"),0);
            mpDoMainList->InsertColumn(4,_T("P/F"),0);

            mpDoMainList->SetHeaderRowWeight(0,0);
            mpDoMainList->SetHeaderRowWeight(1,200);
            mpDoMainList->SetHeaderRowWeight(2,200);
            mpDoMainList->SetHeaderRowWeight(3,200);
            mpDoMainList->SetHeaderRowWeight(4,40);
//             mpDoMainList->GetHeader()->GetItemAt( 0 )->SetScale( 30 );
//             mpDoMainList->GetHeader()->GetItemAt( 1 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 2 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 3 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 4 )->SetScale( 10 );

//             mpDoMainList->GetHeader()->GetItemAt( 0 )->SetScale( 30 );
//             mpDoMainList->GetHeader()->GetItemAt( 1 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 2 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 3 )->SetScale( 20 );
//             mpDoMainList->GetHeader()->GetItemAt( 4 )->SetScale( 10 );
        }

        if ( !LoadConfig() ) return false;

        //! WM_HWND_TEST_LIST_MSG
        ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_TEST_LIST_MSG,&CCRFAntTestView::OnTestListMsg);


        if (!mpTestThread) mpTestThread = new CCRFAntTestThread;
        if (!mpTestThread->IsThreadRun()) mpTestThread->CreateMessageThread(0,0,true);

        mpTestThread->SetTestSuiteXmlFilePath(JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName));
        mpTestThread->SetAsyncMessageClassInstance((CCJGW_AsyncWndMessage*)(mpViewResolverParam->mpsCommonWndMsgInfo->masync_message_class));

        return mpTestThread->IsThreadRun() && mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpPluginPaintManagerUI->GetPaintWindow()),LPARAM(&m_configFile.m_sRfConfigInfo));
        
    }

    bool CCRFAntTestView::OnBusyTestStatus()
    {
        m_bTestThreadStatus = true;
        mpDoMainList->HomeUp();
        mpDoMainList->SetAllUnSelectedItem();
        mpDoMainList->RemoveAllShow();
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xff00ff00");
        mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
        return true;
    }

    bool CCRFAntTestView::OnFailTestStatus()
    {
        m_bTestThreadStatus = false;
        mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        //mpDoMainList->EndDown();
		OnSaveTest(false);
        return true;
    }

    bool CCRFAntTestView::OnPassTestStatus()
    {
        m_bTestThreadStatus = false;
		OnSaveTest(true);
        return true;
    }
    //! 5个数值
    void CCRFAntTestView::OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        const wchar_t* strListCtrlMsg = (const wchar_t*)wParam;
        bool bInsertItem = (1 == lParam);

        static std::vector<std::wstring> _vListCtrlMsg;
        _vListCtrlMsg.clear();
        CListTextElementUI* m_pListTextElementUi = NULL;
        JGW_ParserStrW(strListCtrlMsg,_T("@"),_vListCtrlMsg);
        if (5 > _vListCtrlMsg.size()) return ;

        if ( bInsertItem )
        {
            m_pListTextElementUi = mpDoMainList->InsertItem( mpDoMainList->GetList()->GetCount(),_vListCtrlMsg.at(0).c_str(),m_nListItemHeight);
            mpDoMainList->SetNeedEndDown(true);
        }
        else
        {
            m_pListTextElementUi = mpDoMainList->GetListTextElementItemClass( mpDoMainList->GetList()->GetCount() - 1);
        }

        if( !m_pListTextElementUi ) return ;

        for (size_t i = 0;i < _vListCtrlMsg.size();i ++)
        {
            m_pListTextElementUi->SetText(i,_vListCtrlMsg.at(i).c_str());
        }

        if (std::wstring::npos != _vListCtrlMsg[4].find(L"F") || std::wstring::npos != _vListCtrlMsg[4].find(L"f"))
        {
            m_pListTextElementUi->Select();
            mpDoMainList->SetAttribute(L"itemselectedbkcolor",L"0xffff0000");
        } 
    }

    bool CCRFAntTestView::LoadConfig()
    {
        std::wstring strDataCacheFilePath,strConfigFilePath;
           
        JGW_GetCacheExecSubFolder( strDataCacheFilePath,mpViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str() );
        strDataCacheFilePath += L"rf_test.ini";
        JGW_GetConfigExecSubFolder( strConfigFilePath,mpViewResolverParam->mpPluginInfo->mstrModuleFolderName.c_str() );
        strConfigFilePath += L"Model.ini";

        m_configFile.SetCacheConfigFilePath( strDataCacheFilePath.c_str() );
        m_configFile.SetConfigFilePath( strConfigFilePath.c_str() );

        if ( !m_configFile.LoadConfigFile() )
        {
            ::MessageBox( mpPluginPaintManagerUI->GetPaintWindow(),m_configFile.GetErrorMsg(),L"配置文件加载错误",MB_ICONERROR );
            return false;
        }

        return true;
    }

    void CCRFAntTestView::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
    {
        if( m_bTestThreadStatus ) { MessageBox( mpPluginPaintManagerUI->GetPaintWindow(),L"测试过程中不允许修改配置",L"访问错误",MB_ICONERROR ); return;}
        CDuiConfig duiconfig(&m_configFile);
        duiconfig.DoModal( mpPluginPaintManagerUI->GetPaintWindow() );
        if ( mpTestThread && !mpTestThread->CheckMonitorConfigIsSame() ) mpTestThread->PostThreadMessage(WM_TEST_THREAD_MSG_INIT,WPARAM(mpPluginPaintManagerUI->GetPaintWindow()),LPARAM(&m_configFile.m_sRfConfigInfo));
    }

	void CCRFAntTestView::OnSaveTest(bool bTestStatus)
	{
		std::wofstream fileStream;
		std::wstring strExcelContent;
		CListTextElementUI* pListTextElementUi = NULL;

		std::wstring strTemp,strFilePath;
		JGW_GetExecSubFolder(strTemp,L"log");
		JGW_FormatWString(strFilePath,L"%s%u.log",strTemp.c_str(),JGW_GetTimeOfDay());
		//! GetTickCount
		fileStream.imbue( std::locale("") );
		fileStream.open( strFilePath.c_str(),std::ios_base::out | std::ios_base::trunc );

		if (!fileStream.is_open()) return ;

		fileStream << L"测试项\tMin\tValue\tMax\tP/F" << std::endl;

		for (int i = 0;i < mpDoMainList->GetList()->GetCount();i ++)
		{
			pListTextElementUi = (CListTextElementUI*)mpDoMainList->GetList()->GetItemAt(i);

			for (int index = 0;index < 5;index ++)
			{
				fileStream << pListTextElementUi->GetText(index) << L"\t";
			}
			fileStream <<  std::endl;
		}

		fileStream.flush();
		fileStream.close();

		CCJGW_MoveTestLogging moveTestLogging;
		moveTestLogging.MoveTestLogging(L"ANT_TEST\\",mpTestThread->GetSerialNumber().c_str(),strFilePath.c_str(),bTestStatus);

		return ;
	}
}

