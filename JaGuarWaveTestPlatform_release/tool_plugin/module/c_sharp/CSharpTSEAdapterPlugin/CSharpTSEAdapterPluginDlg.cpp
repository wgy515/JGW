#include "StdAfx.h"
#include "CSharpTSEAdapterPluginDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
namespace JGW
{
	typedef struct  
	{
		int mnTypeID;
		int mnLogID;
		char mstrLog[1024];
	}S_TSE_ADPATER_LOG,*LPS_TSE_ADPATER_LOG;

	CCSharpTSEAdapterPluginDlg::CCSharpTSEAdapterPluginDlg(void) : mhCSharpAdapterExecWindowHandle(NULL),mpEmptySubLayout(NULL),mpThroughtputTestEdt(NULL),mhTSEAdapterHInstance(NULL)
	{
		msPluginSkin.mstrSkinXmlPath = L"tse_csharp_apdate_skin.xml";
		msPluginSkin.mstrPluginVersions = SUB_PLUGIN_VERSION;
		memset(&mCSharpAdapterHwndrect,0x00,sizeof(mCSharpAdapterHwndrect));
        mbIsShowLogHwnd = false;
	}


	CCSharpTSEAdapterPluginDlg::~CCSharpTSEAdapterPluginDlg(void)
	{
        if (NULL != mhCSharpAdapterExecWindowHandle)
        {
            PostMessage(mhCSharpAdapterExecWindowHandle,WM_CLOSE_CSHARP_ADAPTER_VIEW,NULL,NULL);
            JGW_KillProcessToName(L"JGW.TSE.WinForm.exe");
            //TerminateProcess(mhTSEAdapterHInstance,-1);
        }   
	}

	void CCSharpTSEAdapterPluginDlg::CloseSubWndPlugin()
	{
		if (NULL != mhCSharpAdapterExecWindowHandle)
		{
			SendMessage(mhCSharpAdapterExecWindowHandle,WM_CLOSE_CSHARP_ADAPTER_VIEW,NULL,NULL);
            JGW_KillProcessToName(L"JGW.TSE.WinForm.exe");
            //TerminateProcess(mhTSEAdapterHInstance,-1);
			mhCSharpAdapterExecWindowHandle = NULL;
		}
		CCJGW_CommanTestPlnView::CloseSubWndPlugin();
	}

	bool CCSharpTSEAdapterPluginDlg::StartCSharpTSEAdapterExec()
	{
		HWND hPluginWnd = mpPluginPaintManagerUI->GetPaintWindow();
		JGW_KillProcessToName(L"JGW.TSE.WinForm.exe");
		//! rect = mpViewResolverParam->mpMainParentContainer->GetPos();
		//! QiaSampleApp.exe
		std::wstring strApplicationFile;
		JGW_FormatWString(strApplicationFile, L"%s\\JGW.TSE.WinForm.exe",JGW_GetApplicationFolder());
		std::wstring strParam;
		std::wstring strConfigXmlPath = JGW_GetTSEConfigFilePath(mpViewResolverParam->mpPluginInfo->mstrModuleFolderName,mpViewResolverParam->mpPluginInfo->mstrSuiteXmlName);
		JGW_FormatWString(strParam,L"%d \"%s\"",(int)hPluginWnd,strConfigXmlPath.c_str());
        mhTSEAdapterHInstance = ShellExecute(NULL,L"open",strApplicationFile.c_str(),strParam.c_str(),JGW_GetApplicationFolder(),SW_NORMAL);
		return NULL != mhTSEAdapterHInstance;
	}

	bool CCSharpTSEAdapterPluginDlg::OnIdelTestStatus()
	{
		mpEmptySubLayout = static_cast<CHorizontalLayoutUI*>(mpPluginPaintManagerUI->FindControl(L"emptysubLayout"));
		mpThroughtputTestEdt = static_cast<CRichEditUI*>(mpPluginPaintManagerUI->FindControl(L"throughtputTestEdt"));
		ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_PAINT,&CCSharpTSEAdapterPluginDlg::OnPaint);
		ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_CSHARP_ADAPTER_VIEW_REPORT_HWND,&CCSharpTSEAdapterPluginDlg::OnCsharpAdapterViewReportHwnd);
		ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_HWND_INIT_TSE_TEST_FAIL_MSG,&CCSharpTSEAdapterPluginDlg::OnTSEInitFail);
		ADD_REG_MSG(mpViewResolverParam->mpMapRegMsg,WM_COPYDATA,&CCSharpTSEAdapterPluginDlg::OnTSEAdapterLog);

		if (!StartCSharpTSEAdapterExec())
		{
			return false;
		}
		return true;
	}

	void CCSharpTSEAdapterPluginDlg::UpdateMESInfoToCSharpAdapterHwnd()
	{
		static std::string strOperationID,strUserName,strPassword;
		LPS_MES_INFO psMESInfo = GetMESDataBaseAdo()->GetMESServiceSocketConnect();

		if (0 != strOperationID.compare(psMESInfo->mstrOperationId)
			|| 0 != strUserName.compare(psMESInfo->msMesConnectInfo.mstrUsername)
			|| 0 != strPassword.compare(psMESInfo->msMesConnectInfo.mstrUserpwd))
		{
			std::wstring strTemp;
			std::string value;
			JGW_FormatString(value,"%s,%s,%s",psMESInfo->mstrOperationId.c_str(),psMESInfo->msMesConnectInfo.mstrUsername.c_str(),psMESInfo->msMesConnectInfo.mstrUserpwd.c_str());
			strTemp = JGW_A2W(value);

			COPYDATASTRUCT copyData;
			copyData.lpData = (LPVOID)strTemp.c_str();
			copyData.cbData = strTemp.size() * 2;
			::SendMessage(mhCSharpAdapterExecWindowHandle, WM_COPYDATA, NULL, (LPARAM)&copyData);
			strOperationID = psMESInfo->mstrOperationId;
			strUserName = psMESInfo->msMesConnectInfo.mstrUsername;
			strPassword = psMESInfo->msMesConnectInfo.mstrUserpwd;
		}
	}

	bool CCSharpTSEAdapterPluginDlg::OnBusyTestStatus()
	{
		UpdateMESInfoToCSharpAdapterHwnd();
		mpThroughtputTestEdt->SetText(L"");
		SendMessage(mhCSharpAdapterExecWindowHandle,WM_TEST_THREAD_MSG_RUN,WPARAM(NULL),0);
		return true;
	}
	//! WM_TEST_PASS_MSG
	bool CCSharpTSEAdapterPluginDlg::OnPassTestStatus()
	{
		if (mbIsSaveTestLog) CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",true,mpThroughtputTestEdt->GetText().GetData());
		SendMessage(mhCSharpAdapterExecWindowHandle,WM_TEST_THREAD_MSG_AUTO_TEST,WPARAM(NULL),0);
		return true;
	}
	//! WM_TEST_FAIL_MSG
	bool CCSharpTSEAdapterPluginDlg::OnFailTestStatus()
	{
		 if (mbIsSaveTestLog) CCJGW_SaveTestLog::SaveTestLog(mstrSaveDownloadLogFolder,L"",false,mpThroughtputTestEdt->GetText().GetData());
		return true;
	}

	void CCSharpTSEAdapterPluginDlg::OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam)
	{
		SendMessage(mhCSharpAdapterExecWindowHandle,WM_TEST_THREAD_MSG_CONFIG_VIEW_TEST,WPARAM(NULL),0);
	}

	void CCSharpTSEAdapterPluginDlg::OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam)
	{
		MessageBox(mpPluginPaintManagerUI->GetPaintWindow(),L"初始化配置失败，请检查配置是否正常,详情见Log",L"TSE INTI",MB_ICONERROR);
		PostMessage(mhCSharpAdapterExecWindowHandle,WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS,NULL,NULL);
	}

	void CCSharpTSEAdapterPluginDlg::OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam)
	{
		PostMessage(mhCSharpAdapterExecWindowHandle,WM_HWND_SHOW_LOG_LAYOUT_CLICK_MSG,NULL,NULL);
		//! ReSizeCSharpAdapterHwnd();
	}

	void CCSharpTSEAdapterPluginDlg::OnCsharpAdapterViewReportHwnd(const WPARAM& wParam,const LPARAM& lParam)
	{
		mhCSharpAdapterExecWindowHandle = (HWND)wParam;
	}

	//! 更新
	void CCSharpTSEAdapterPluginDlg::OnPaint(const WPARAM& wParam,const LPARAM& lParam)
	{
		//mpEmptySubLayout->GetRelativePos()
		//if (NULL != mhCSharpAdapterExecWindowHandle)
		//{
		//	SendMessage(mhCSharpAdapterExecWindowHandle,WM_NCPAINT,0,0);
		//}
		//RECT containerRect = mpEmptySubLayout->GetPos();
		//if ( mCSharpAdapterHwndrect.bottom != containerRect.bottom || mCSharpAdapterHwndrect.top != containerRect.top
		//	|| mCSharpAdapterHwndrect.left != containerRect.left || mCSharpAdapterHwndrect.right != containerRect.right )
		//{
			ReSizeCSharpAdapterHwnd();
		//}
	}

	void CCSharpTSEAdapterPluginDlg::ReSizeCSharpAdapterHwnd()
    {
        if (NULL != mhCSharpAdapterExecWindowHandle)
        {
            std::wstring strTemp;
			RECT containerRect = mpEmptySubLayout->GetPos();
            JGW_FormatWString(strTemp,L"%d,%d,%d,%d",containerRect.left
                ,containerRect.top
                ,containerRect.right - containerRect.left 
                ,containerRect.bottom - containerRect.top);

            COPYDATASTRUCT copyData;
            copyData.lpData = (LPVOID)strTemp.c_str();
            copyData.cbData = strTemp.size() * 2;

            ::SendMessage(mhCSharpAdapterExecWindowHandle, WM_COPYDATA, NULL, (LPARAM)&copyData);

            //! SendMessage(mhCSharpAdapterExecWindowHandle,WM_RESIZE_CSHARP_ADAPTER_VIEW,WPARAM(strTemp.c_str()),NULL);
            /*
            MoveWindow(GetHWND(),mpViewResolverParam->mpMainParentContainer->GetPos().left + 5 ,mpViewResolverParam->mpMainParentContainer->GetPos().top + 2
            ,mpViewResolverParam->mpMainParentContainer->GetPos().right - mpViewResolverParam->mpMainParentContainer->GetPos().left - 10
            ,mpViewResolverParam->mpMainParentContainer->GetPos().bottom - mpViewResolverParam->mpMainParentContainer->GetPos().top - 10,TRUE );
            */
            mCSharpAdapterHwndrect = containerRect;
        }  
    }
	//public enum LogType : int
	//{
	//	Debug = 0,
	//	Info = 1,
	//	Warn = 2,
	//	Error = 3,
	//	UserDefine = 4
	//}
    void CCSharpTSEAdapterPluginDlg::AppendRicheditLog(std::wstring& strLog)
    {
        JGW_EraseLastAndFristTwoCharsW(strLog);
        mpThroughtputTestEdt->AppendText(strLog.c_str());
        mpThroughtputTestEdt->AppendText(L"\r\n");
        mpThroughtputTestEdt->EndDown();
    }

	void CCSharpTSEAdapterPluginDlg::OnTSEAdapterLog(const WPARAM& wParam,const LPARAM& lParam)
	{
		COPYDATASTRUCT* pCopyDateStruct = (COPYDATASTRUCT*)lParam;
		LPS_TSE_ADPATER_LOG pAdapterLog = (LPS_TSE_ADPATER_LOG)pCopyDateStruct->lpData;
		std::wstring strLog = JGW_A2W(pAdapterLog->mstrLog,CP_UTF8);
		switch (pAdapterLog->mnTypeID)
		{
		case 0:
			Log4WD(strLog.c_str());
			break;
		case 1:
		case 2:
			PDLOG4WW(strLog.c_str());
			if (NULL != JGW_WStrCaseWStr(strLog.c_str(),L" fail "))
            {
                AppendRicheditLog(strLog);
                PELOG4WW(strLog.c_str());
            }
			break;
		case 3:
			PELOG4WW(strLog.c_str());
            AppendRicheditLog(strLog);
			break;
		case 4:
			switch ((E_USER_DEFINE_LOG_TYPE)pAdapterLog->mnLogID)
			{
			case E_USER_DEFINE_HTMLFILEPATH:
				PHLOG4WW(strLog.c_str());
				break;
			case E_TSE_SHOW_CONTENT_INFO:
				//mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TSE_SHOW_CONTENT,pAdapterLog.mstrLog);
				break;
			case E_TSE_SHOW_TEST_INFO_TO_RICHEDIT:
				AppendRicheditLog(strLog);
				break;
			}
			break;
		default:
			break;
		}
	}
}
