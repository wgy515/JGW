#include "StdAfx.h"
#include "CTSEMultiSplitTestPluginDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CTSETestPluginUI.h"
namespace JGW
{
	CCTSEMultiSplitTestPluginDlg::CCTSEMultiSplitTestPluginDlg(void) : mpPluginPaintManagerUI(NULL),mpMainMultipleVerticalLayout(NULL),mpsViewResolverParam(NULL)
	{
		msPluginSkin.mstrSkinXmlPath = L"tse_multi_plugin_app_plugin.xml";
		msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
	}


	CCTSEMultiSplitTestPluginDlg::~CCTSEMultiSplitTestPluginDlg(void)
	{
	}

	bool CCTSEMultiSplitTestPluginDlg::InitSubWndPlugin(LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam)
	{
		//! Init UI
		{
			mpsViewResolverParam = (LPS_VIEW_RESOLVER_PARAM)psSubWndImplParam;
			mpPluginPaintManagerUI = (CPaintManagerUI*)mpsViewResolverParam->mpsCommonWndMsgInfo->mpPluginPaintManager;
			msMultiPluginAppConfig.mstrModuleFolder = mpsViewResolverParam->mpPluginInfo->mstrModuleFolderName;
			msMultiPluginAppConfig.mstrSuiteName = mpsViewResolverParam->mpPluginInfo->mstrSuiteXmlName;
			//msMultiPluginAppConfig.mhMainWnd = mpPluginPaintManagerUI->GetPaintWindow();
			mpMainMultipleVerticalLayout = static_cast<CVerticalLayoutUI*>(mpPluginPaintManagerUI->FindControl(L"MainMultipleVerticalLayout"));
			if (NULL == mpMainMultipleVerticalLayout) return false;

			//! 上下文菜单栏（右键点击出现菜单栏）
			mpMainMultipleVerticalLayout->SetContextMenuUsed(true);

			//! 加载多路插件测试配置信息
			if (!LoadTSEMultiPluginAppConfig()) return false;

			// 响应发送的消息
			ADD_REG_MSG(mpsViewResolverParam->mpMapRegMsg,WM_MENUCLICK,&CCTSEMultiSplitTestPluginDlg::OnMenuClick);;

			//! 初始化多路测试视图
			InitTSEMultiPluginAppPluginView();
		}

		return true;
	}

	void CCTSEMultiSplitTestPluginDlg::CloseSubWndPlugin()
	{
		if (NULL != mpMainMultipleVerticalLayout) mpMainMultipleVerticalLayout->SetContextMenuUsed(false);
		UnInitTSEMultiPluginAppPluginView();
		CCJGWUiLibViewResolverImpl::CloseSubWndPlugin();
	}

	void CCTSEMultiSplitTestPluginDlg::UnInitTSEMultiPluginAppPluginView()
	{
#if 1
		for (std::map<int,CCTSETestPluginUI*>::iterator it = mmappPluginAppWindowUI.begin();it != mmappPluginAppWindowUI.end();++it)
		{
			//! 关闭TSE测试插件窗口
			it->second->CloseTSETestPluginWnd();
            //! 移除所有控件
			((CVerticalLayoutUI*)it->second->GetParent())->RemoveAll();
		}
		mmappPluginAppWindowUI.clear();
#endif
        mpMainMultipleVerticalLayout->RemoveAll();
	}

	void CCTSEMultiSplitTestPluginDlg::OnNotifySubWndPlugin(LPVOID pMsg)
	{
		TNotifyUI* pNotifyMsg = (TNotifyUI*)pMsg;
		if (pNotifyMsg->sType == DUI_MSGTYPE_MENU)
		{
			//! 鼠标右键点击消息
			if (pNotifyMsg->pSender == mpMainMultipleVerticalLayout)
			{
				OnContextMenuClick(pNotifyMsg);
			}
		}
	}

    CControlUI* CCTSEMultiSplitTestPluginDlg::CreateControl(LPCTSTR pstrClass)
    {
        if(_tcscmp(pstrClass, L"TSETestPlugin") == 0)
            return	new CCTSETestPluginUI();
        return NULL;
    }

	void CCTSEMultiSplitTestPluginDlg::InitTSEMultiPluginAppPluginView()
	{
		int nIndex = 0;
		//! 卸载多路测试视图
		UnInitTSEMultiPluginAppPluginView();

		//! 初始化多路测试视图
		for (int i = 0;i < mnMultipleTestRow;i++)
		{
			CDialogBuilder builder;
			CHorizontalLayoutUI* pHorizontalLayoutUI = (CHorizontalLayoutUI*)(builder.Create(L"<Window><HorizontalLayout enabled=\"true\" bordersize=\"2\" bordercolor=\"#ffffff\" padding=\"5,5,5,5\"></HorizontalLayout></Window>",(UINT)0,NULL,mpPluginPaintManagerUI));
			if (NULL == pHorizontalLayoutUI) continue;

			for (int y = 0;y < mnMultipleTestCol;y++)
			{
				CDialogBuilder builder1;
				CVerticalLayoutUI* pListContainerItem = (CVerticalLayoutUI*)(builder1.Create(_T("multi_split_app_window.xml"),(UINT)0,this,mpPluginPaintManagerUI));
				nIndex ++;
				if (NULL != pListContainerItem)
				{

					//! 当前插件内部的控件，内部自动删除
					pListContainerItem->SetDelayedDestroy(false);
                    //! 获取TSE插件UI视图实例
					CCTSETestPluginUI* pPluginAppWindowUI = (CCTSETestPluginUI*)pListContainerItem->GetItemAt(0);
					//! 从根配置文件进行复制操作 xx.xml => xx.xml.%d
					std::wstring strSuiteName;
					std::wstring strSuiteNameFilePath = JGW_GetCacheConfigFilePath(msMultiPluginAppConfig.mstrModuleFolder,msMultiPluginAppConfig.mstrSuiteName);
					JGW_FormatWString(strSuiteName,L"%s.%d",strSuiteNameFilePath.c_str(),nIndex);
					//! 检查配置文件是否存在
					if (!JGW_FileExistsToFilePath(strSuiteName.c_str()))
					{
						CopyFileW(strSuiteNameFilePath.c_str(),strSuiteName.c_str(),FALSE );
					}
					if (!JGW_FileExistsToFilePath(strSuiteName.c_str())) 
					{
						MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),JGW_GetFormatWString(L"The configuration file(%s) does not exist",strSuiteName.c_str()).c_str(),L"Error",MB_ICONERROR);
						UnInitTSEMultiPluginAppPluginView();
						return ;
					}
					JGW_FormatWString(strSuiteName,L"%s.%d",msMultiPluginAppConfig.mstrSuiteName.c_str(),nIndex);
					//! 设置目录以及配置文件名
                    pPluginAppWindowUI->SetModuleFolderSuitName(msMultiPluginAppConfig.mstrModuleFolder,strSuiteName);
					mmappPluginAppWindowUI[nIndex] = (pPluginAppWindowUI);
					//! 添加到当前窗口
					pHorizontalLayoutUI->Add(pListContainerItem);
				}
			}
			mpMainMultipleVerticalLayout->Add(pHorizontalLayoutUI);
		}
	}

	void CCTSEMultiSplitTestPluginDlg::OnContextMenuClick(TNotifyUI* pNotifyMsg)
	{
		CMenuWnd* pMenu = new CMenuWnd();
		CPoint point;
		point.x = pNotifyMsg->ptMouse.x;
		point.y = pNotifyMsg->ptMouse.y;
		ClientToScreen(mpPluginPaintManagerUI->GetPaintWindow(), &point);

		mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"] = false;
		mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"] = false;
		mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"] = false;
		mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"] = false;
		switch(meNumberOfMultipleTests)
		{
		case E_NUMBER_OF_MULTIPLE_TEST_1:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"] = true;
			break;
		case E_NUMBER_OF_MULTIPLE_TEST_2:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"] = true;
			break;
		case E_NUMBER_OF_MULTIPLE_TEST_4:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"] = true;
			break;
		case E_NUMBER_OF_MULTIPLE_TEST_8:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"] = true;
			break;
		}

		pMenu->Init(NULL,_T("tse_multi_plugin_app_plugin_menu.xml"),point,mpPluginPaintManagerUI,&mpMenuCheckInfo);
	}

	void CCTSEMultiSplitTestPluginDlg::OnMenuClick(const WPARAM& wParam,const LPARAM& lParam)
	{
#if 0
		const wchar_t* strName = (const wchar_t*)wParam;

		return ;
#else
		//! 取消已单机过的参数
		switch(meNumberOfMultipleTests)
		{
		case E_NUMBER_OF_MULTIPLE_TEST_1:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"] = false;
			break;
		case E_NUMBER_OF_MULTIPLE_TEST_2:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"] = false;
			break;
		case E_NUMBER_OF_MULTIPLE_TEST_4:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"] = false;
			break;
		case E_NUMBER_OF_MULTIPLE_TEST_8:
			mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"] = false;
			break;
		}
		//! 判断是有点击数据
		if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_1"])
		{
			if (!(1 == mnMultipleTestRow && 1 == mnMultipleTestCol))
			{
				mnMultipleTestRow = 1;
				mnMultipleTestCol = 1;
				InitTSEMultiPluginAppPluginView();
				mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",1);
			}
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_1;
		}
		else if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_2"])
		{
			if (!(1 == mnMultipleTestRow && 2 == mnMultipleTestCol))
			{
				mnMultipleTestRow = 1;
				mnMultipleTestCol = 2;
				InitTSEMultiPluginAppPluginView();
				mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",2);
			}
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_2;
		}
		else if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_4"])
		{
			if (!(2 == mnMultipleTestRow && 2 == mnMultipleTestCol))
			{
				mnMultipleTestRow = 2;
				mnMultipleTestCol = 2;
				InitTSEMultiPluginAppPluginView();
				mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",4);
			}
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_4;
		}
		else if (mpMenuCheckInfo[L"tse_multi_plugin_app_plugin_menu_8"])
		{
			if (!(2 == mnMultipleTestRow && 4 == mnMultipleTestCol))
			{
				mnMultipleTestRow = 2;
				mnMultipleTestCol = 4;
				InitTSEMultiPluginAppPluginView();
				mConfigIni.SetIniKeyValue(L"MAIN",L"NumberOfMultipleTests",8);
			}
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_8;
		}
#endif
	}

	bool CCTSEMultiSplitTestPluginDlg::LoadTSEMultiPluginAppConfig()
	{
		std::wstring strRecordConfigPath;
		JGW_GetConfigExecSubFolder(strRecordConfigPath,msMultiPluginAppConfig.mstrModuleFolder.c_str());
		strRecordConfigPath += msMultiPluginAppConfig.mstrSuiteName;
		strRecordConfigPath += L".data";

		//! check_suite.xml.data
		mConfigIni.InitIniFilePath(strRecordConfigPath.c_str());
		msMultiPluginAppConfig.mnNumberOfMultipleTests = mConfigIni.GetIniKeyIntValue(L"MAIN",L"NumberOfMultipleTests",2);

		//msMultiPluginAppConfig.mstrStationID = mpsViewResolverParam->mpPluginInfo->mStationID;
		//msMultiPluginAppConfig.mstrClsid = mConfigIni.GetIniKeyValue(L"MAIN",L"clsid");
		//msMultiPluginAppConfig.mnViewLoaderID = mConfigIni.GetIniKeyIntValue(L"MAIN",L"viewloader_id",1);
		//msMultiPluginAppConfig.mstrModuleFolder = mConfigIni.GetIniKeyValue(L"MAIN",L"module_folder");
		//msMultiPluginAppConfig.mstrPluginName = mConfigIni.GetIniKeyValue(L"MAIN",L"name");
		//msMultiPluginAppConfig.mstrStationName = mConfigIni.GetIniKeyValue(L"MAIN",L"station_name");
		//std::string strStationNameA = JGW_W2A(msMultiPluginAppConfig.mstrStationName,CP_ACP);
		//msMultiPluginAppConfig.mstrStationName = JGW_A2W(strStationNameA,CP_UTF8);
		//msMultiPluginAppConfig.mstrSuiteName = mConfigIni.GetIniKeyValue(L"MAIN",L"suite_name");
		//msMultiPluginAppConfig.mnNumberOfMultipleTests = mConfigIni.GetIniKeyIntValue(L"MAIN",L"NumberOfMultipleTests",2);
#if 1
		switch (msMultiPluginAppConfig.mnNumberOfMultipleTests)
		{
		case 1:
			mnMultipleTestRow = 1;
			mnMultipleTestCol = 1;
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_1;
			break;
		case 2:
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_2;
			mnMultipleTestRow = 1;
			mnMultipleTestCol = 2;
			break;
		case 4:
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_4;
			mnMultipleTestRow = 2;
			mnMultipleTestCol = 2;
			break;
		case 8:
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_8;
			mnMultipleTestRow = 2;
			mnMultipleTestCol = 4;
			break;
		default:
			meNumberOfMultipleTests = E_NUMBER_OF_MULTIPLE_TEST_2;
			mnMultipleTestRow = 2;
			mnMultipleTestCol = 2;
			break;
		}
#endif

		return true;
	}

}
