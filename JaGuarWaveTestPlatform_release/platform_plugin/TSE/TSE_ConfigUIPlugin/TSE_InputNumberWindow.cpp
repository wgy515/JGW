#include "StdAfx.h"
#include "TSE_InputNumberWindow.h"
#include "CJGW_InputViewResolver.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"

#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

namespace JGW
{
	CTSE_InputNumberWindow::CTSE_InputNumberWindow(void) : mMainHwnd(NULL),mstrMaxChar(L"999"),mstrMinChar(L"0"),mbTSEInit(false),meStringUpperLowCaseType(E_STRING_UPPERCASE_TYPE)
	{
	}


	CTSE_InputNumberWindow::~CTSE_InputNumberWindow(void)
	{
	}

	const wchar_t* CTSE_InputNumberWindow::TSE_GetParamDescription()
	{
		return L"{\
				\"RealName\": \"CTSE_InputNumberWindow\",\
				\"TestName\": \"输入对话框\",\
				\"tip\": \"标题\",\
				\"input_length\": \"输入长度限制\",\
				\"StringUpperLowCaseType\": \"字符串大小写类型,0:不忽略大小写，对大小写敏感,1:默认大写模式,2: 小写模式\",\
				\"maxchar\": \"输入最大字符串\",\
				\"minchar\": \"最少输入字符串\",\
				\"input_tip\": \"输入提示\",\
				\"input_prefix\": \"输入前缀字符串校验\",\
				\"numbertype\": \"号段类型E_SN:0,E_IMEI:1,E_IMEI2:2,E_MEID:3,E_WIFI_MAC:4,E_ETH0_MAC:5,E_BT_MAC:6,E_2_4_SSID:7,E_2_4_PASSWORD:8,E_5_SSID:9,E_5_PASSWORD:10,E_AD_SSID:11,E_AD_PASSWORD:12,E_CLIENT_SN:13\",\
				\"Environment\": \"输入保存环境变量\"\
				}";
	}

	bool CTSE_InputNumberWindow::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
	{
		if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
		if(TSE_PARAM_NAME_EQUAL(_T("tip"),strParamName))
		{
			mstrTip = strParamValue;
		}
		if(TSE_PARAM_NAME_EQUAL(_T("input_length"),strParamName))
		{
			mstrMaxChar = strParamValue;
		}
		if(TSE_PARAM_NAME_EQUAL(_T("StringUpperLowCaseType"),strParamName))
		{
			meStringUpperLowCaseType = (E_STRING_UPPERLOWCASE_TYPE)_ttoi(strParamValue);
		}
		if(TSE_PARAM_NAME_EQUAL(_T("maxchar"),strParamName))
		{
			mstrMaxChar = strParamValue;
		}
		if(TSE_PARAM_NAME_EQUAL(_T("minchar"),strParamName))
		{
			mstrMinChar = strParamValue;
		}
		if(TSE_PARAM_NAME_EQUAL(_T("input_tip"),strParamName))
		{
			mstrInputTip = strParamValue;
		}
		if(TSE_PARAM_NAME_EQUAL(_T("input_prefix"),strParamName))
		{
			mstrInputPrefix = strParamValue;
		}
		if(TSE_PARAM_NAME_EQUAL(_T("numbertype"),strParamName))
		{
			mstrNumberType = strParamValue;
		}
		if(TSE_PARAM_NAME_EQUAL(_T("Environment"),strParamName))
		{
			mstrEnvironmentStr = strParamValue;
		}
		else
		{
			return false;
		}
		return true;
	}

	bool CTSE_InputNumberWindow::TSE_Init()
	{
		msAllInputUIAttrSetting.m_vControlAttrSetting.clear();
		msAllInputUIAttrSetting.m_sGetControlAttrSetting.mvsAttrNameValue.clear();
		//msAllInputUIAttrSetting.m_sWindowAttr.mhParentHwnd = hwnd;
		msAllInputUIAttrSetting.m_sWindowAttr.mstrSkinFolder = _T("skin\\JaGuarWave");
		msAllInputUIAttrSetting.m_sWindowAttr.mstrSkinFile = _T("CommInputUiSkin.xml");
		msAllInputUIAttrSetting.m_sWindowAttr.dwStyle = UI_WNDSTYLE_DIALOG;
		msAllInputUIAttrSetting.m_sWindowAttr.dwExStyle = WS_EX_STATICEDGE /*| WS_EX_APPWINDOW*/ ;
		msAllInputUIAttrSetting.m_sWindowAttr.mstrWindowName = _T("CTSE_InputNumberWindow_Duilib");

		S_ATTR_NAME_VALUE sAttrNameValue;
		S_CONTROL_ATTR_SETTING sControlAttrSetting,sControlAttrSetting1,sControlAttrSetting2,sControlAttrSetting3;
		S_CONTROL_ATTR_SETTING sGetControlAttrSetting;
		{
			sControlAttrSetting.name = _T("inputEdit");

			sAttrNameValue.pstrName = _T("focus");
			sAttrNameValue.pstrValue = _T("");
			sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);

			sAttrNameValue.pstrName = _T("SetSelAll");
			sAttrNameValue.pstrValue = _T("");
			sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);

			//
			sAttrNameValue.pstrName = _T("maxchar");
			sAttrNameValue.pstrValue = mstrMaxChar;
			sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);
		}
		{
			sControlAttrSetting3.name = _T("GlobalEnvironment");
			sAttrNameValue.pstrName = _T("minchar");
			sAttrNameValue.pstrValue = mstrMinChar;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("maxchar");
			sAttrNameValue.pstrValue = mstrMaxChar;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("inputprefix");
			sAttrNameValue.pstrValue = mstrInputPrefix;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("numbertype");
			sAttrNameValue.pstrValue = mstrNumberType;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("StringUpperLowCaseType");
			sAttrNameValue.pstrValue = JGW_GetFormatWString(L"%d",meStringUpperLowCaseType);
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);     
		}
		{
			sControlAttrSetting1.mvsAttrNameValue.clear();
			sControlAttrSetting1.name = _T("tipLabel");
			sAttrNameValue.pstrName = _T("text");
			sAttrNameValue.pstrValue = mstrTip;
			sControlAttrSetting1.mvsAttrNameValue.push_back(sAttrNameValue);
		}
		{
			sControlAttrSetting2.mvsAttrNameValue.clear();
			sControlAttrSetting2.name = _T("inputTip");
			sAttrNameValue.pstrName = _T("text");
			sAttrNameValue.pstrValue = mstrInputTip;
			sControlAttrSetting2.mvsAttrNameValue.push_back(sAttrNameValue);
		}
		msAllInputUIAttrSetting.m_vControlAttrSetting.push_back(sControlAttrSetting);
		msAllInputUIAttrSetting.m_vControlAttrSetting.push_back(sControlAttrSetting1);
		msAllInputUIAttrSetting.m_vControlAttrSetting.push_back(sControlAttrSetting2);
		msAllInputUIAttrSetting.m_vControlAttrSetting.push_back(sControlAttrSetting3);
		//! 获取控制字符串
		{
			msAllInputUIAttrSetting.m_sGetControlAttrSetting.name = _T("inputEdit");
			sAttrNameValue.pstrName = _T("text");
			sAttrNameValue.pstrValue = _T("");
			msAllInputUIAttrSetting.m_sGetControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);
		}

		return true;
	}

	LRESULT CTSE_InputNumberWindow::OnResponceInputCommonWindowMsg(WPARAM wParam,LPARAM lParam)
	{
#if 1
		LPS_ALL_INPUT_UI_ATTR_SETTING psAllControlAttrSetting = (LPS_ALL_INPUT_UI_ATTR_SETTING)wParam;
		if (NULL == psAllControlAttrSetting)
		{
			return IDCANCEL;
		}

		CCJGWInputViewResolver jgw(psAllControlAttrSetting);
		if ( IDOK == jgw.DoModal() )
		{
			unUserData nRetData = {{0,0,0,0}};
#if 0
			for ( std::vector<LPS_CONTROL_ATTR_SETTING>::iterator it = vGetControlAttrSetting->begin();
				it != vGetControlAttrSetting->end();
				++ it )
			{
				for ( std::vector<S_ATTR_NAME_VALUE>::iterator itAttr = it[0]->mvsAttrNameValue.begin();
					itAttr != it[0]->mvsAttrNameValue.end();
					++ itAttr )
				{
					nRetData = jgw.GetControlAttr( it[0]->name.c_str(),itAttr->pstrName.c_str() );
					if ( NULL != nRetData.pcwchar ) itAttr->pstrValue = nRetData.pcwchar;
				}
			}
#else
			for ( std::vector<S_ATTR_NAME_VALUE>::iterator itAttr = psAllControlAttrSetting->m_sGetControlAttrSetting.mvsAttrNameValue.begin();
				itAttr != psAllControlAttrSetting->m_sGetControlAttrSetting.mvsAttrNameValue.end();
				++ itAttr )
			{
				nRetData = jgw.GetControlAttr( psAllControlAttrSetting->m_sGetControlAttrSetting.name.c_str(),itAttr->pstrName.c_str() );
				if ( NULL != nRetData.pcwchar ) itAttr->pstrValue = nRetData.pcwchar;
			}
#endif
			return IDOK;
		}
		return IDCANCEL;
#else
		return IDOK;
#endif
	}

	bool CTSE_InputNumberWindow::TSE_Run()
	{
		if (!mbTSEInit)
		{
			TSE_Init();
			mbTSEInit = true;
		}

		mMainHwnd = (HWND)GetGlobalEnvironment()->GetInt(TSE_GLOBAL_MAIN_HWND);
		if ( !IsWindow(mMainHwnd) )
		{
			LogE_F(_T("当前窗口句柄不存在(HWND:%d)\n"),mMainHwnd);
			return false;
		}
		msAllInputUIAttrSetting.m_sWindowAttr.mhParentHwnd = mMainHwnd;
		msAllInputUIAttrSetting.m_sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue = L"";
#if 0
		S_ALL_CONTROL_ATTR_SETTING				m_sAllControlAttrSetting;
		std::vector<LPS_CONTROL_ATTR_SETTING>	m_vGetControlAttrSetting;
		m_sAllControlAttrSetting.m_sWindowAttr.mhParentHwnd = hwnd;
		m_sAllControlAttrSetting.m_sWindowAttr.mstrSkinFolder = _T("skin\\JaGuarWave");
		m_sAllControlAttrSetting.m_sWindowAttr.mstrSkinFile = _T("CommInputUiSkin.xml");
		m_sAllControlAttrSetting.m_sWindowAttr.dwStyle = UI_WNDSTYLE_DIALOG;
		m_sAllControlAttrSetting.m_sWindowAttr.dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW ;
		m_sAllControlAttrSetting.m_sWindowAttr.mstrWindowName = _T("CCJGW_InputWindow_Duilib");

		S_ATTR_NAME_VALUE sAttrNameValue;
		S_CONTROL_ATTR_SETTING sControlAttrSetting,sControlAttrSetting1,sControlAttrSetting2,sControlAttrSetting3;
		S_CONTROL_ATTR_SETTING sGetControlAttrSetting;
		{
			sControlAttrSetting.name = _T("inputEdit");

			sAttrNameValue.pstrName = _T("focus");
			sAttrNameValue.pstrValue = _T("");
			sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);

			sAttrNameValue.pstrName = _T("SetSelAll");
			sAttrNameValue.pstrValue = _T("");
			sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);

			//
			sAttrNameValue.pstrName = _T("maxchar");
			sAttrNameValue.pstrValue = mstrMaxChar;
			sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);
		}
		{
			sControlAttrSetting3.name = _T("GlobalEnvironment");
			sAttrNameValue.pstrName = _T("minchar");
			sAttrNameValue.pstrValue = mstrMinChar;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("maxchar");
			sAttrNameValue.pstrValue = mstrMaxChar;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("inputprefix");
			sAttrNameValue.pstrValue = mstrInputPrefix;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("numbertype");
			sAttrNameValue.pstrValue = mstrNumberType;
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);
			sAttrNameValue.pstrName = _T("StringUpperLowCaseType");
			sAttrNameValue.pstrValue = JGW_GetFormatWString(L"%d",meStringUpperLowCaseType);
			sControlAttrSetting3.mvsAttrNameValue.push_back(sAttrNameValue);     
		}
		{
			sControlAttrSetting1.mvsAttrNameValue.clear();
			sControlAttrSetting1.name = _T("tipLabel");
			sAttrNameValue.pstrName = _T("text");
			sAttrNameValue.pstrValue = mstrTip;
			sControlAttrSetting1.mvsAttrNameValue.push_back(sAttrNameValue);
		}
		{
			sControlAttrSetting2.mvsAttrNameValue.clear();
			sControlAttrSetting2.name = _T("inputTip");
			sAttrNameValue.pstrName = _T("text");
			sAttrNameValue.pstrValue = mstrInputTip;
			sControlAttrSetting2.mvsAttrNameValue.push_back(sAttrNameValue);
		}
		m_sAllControlAttrSetting.m_vControlAttrSetting.push_back(&sControlAttrSetting);
		m_sAllControlAttrSetting.m_vControlAttrSetting.push_back(&sControlAttrSetting1);
		m_sAllControlAttrSetting.m_vControlAttrSetting.push_back(&sControlAttrSetting2);
		m_sAllControlAttrSetting.m_vControlAttrSetting.push_back(&sControlAttrSetting3);
		//! 获取控制字符串
		{
			sGetControlAttrSetting.name = _T("inputEdit");
			sAttrNameValue.pstrName = _T("text");
			sAttrNameValue.pstrValue = _T("");
			sGetControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);
		}
		m_vGetControlAttrSetting.push_back( &sGetControlAttrSetting );

		if ( IDOK == SendMessage(hwnd,WM_HWND_INPUT_COMMON_WINDOW_MSG,WPARAM(&m_sAllControlAttrSetting),LPARAM(&m_vGetControlAttrSetting)) && !sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue.empty() )
		{
			//! mstrEnvironmentStr
			LogI_F(_T("输入:%s"), sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue.c_str());
			GetGlobalEnvironment()->PutString(mstrEnvironmentStr,sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue);
			return true;
		}

		LogE(_T("获取输入框内容失败\n"));
		return false;
#endif
		if ( IDOK == OnResponceInputCommonWindowMsg(WPARAM(&msAllInputUIAttrSetting),LPARAM(NULL)) && !msAllInputUIAttrSetting.m_sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue.empty() )
		{
			std::wstring strTemp (msAllInputUIAttrSetting.m_sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue); 
			switch (meStringUpperLowCaseType)
			{
			case E_STRING_NOT_IGNORE_CASE_TYPE:
				break;
			case E_STRING_UPPERCASE_TYPE:
				JGW_MakeWstringToUpper(strTemp);
				break;
			case E_STRING_LOWCASE_TYPE:
				JGW_MakeWstringToLower(strTemp);
				break;
			}
			//! mstrEnvironmentStr
			LogI_F(_T("Input:%s"), strTemp.c_str());
			GetGlobalEnvironment()->PutString(mstrEnvironmentStr,strTemp);
			return true;
		}

		LogE(_T("获取输入框内容失败\n"));
		return false;
	}

}
