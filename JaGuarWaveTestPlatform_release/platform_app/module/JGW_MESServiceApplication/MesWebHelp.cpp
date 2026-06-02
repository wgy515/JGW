#include "StdAfx.h"
#include "MesWebHelp.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <poco/Environment.h>
#include <poco/Exception.h>
#include "CJGW_FTM_Json.h"

namespace JGW
{
	CMesWebHelp::CMesWebHelp(void) : mstrMesUrl("http:////192.168.8.4:8803//messervice//api"),mstrVer("4.0"),mstrClientType("AOI"),mstrOperationID("-1"),mcCurlHttp("")
	{
		LoadMesWebHelpConfig();
		GetMesWebHelpWorkingUnitHardwareID();
		JGW_FormatString(mstrCookieFilePath,"%s//%u_JSESSIONID.txt",JGW_W2A_W(JGW_GetApplicationFolder()).c_str(),JGW_GetTimeOfDay());
	}


	CMesWebHelp::~CMesWebHelp(void)
	{
	}

	const std::string& CMesWebHelp::GetErrorMessage()
	{
		return mstrErrorMessage;
	}

	bool CMesWebHelp::TestConnect()
	{
		std::string strUrl(mstrMesUrl);
		std::string strResponceUtf8;

		JGW_ReplaceStringA(strUrl,"/api","");
		SendWindowMsgFormat(JGW_A2W(strUrl).c_str());
		if (!mcCurlHttp.SendCurlHttpA(strUrl,false,false,mstrCookieFilePath,strResponceUtf8,mstrErrorMessage))
		{
			mstrResponceLocal = JGW_UTF8ToLocal(strResponceUtf8);
			return false;
		}
		mstrResponceLocal = JGW_UTF8ToLocal(strResponceUtf8);
		// SMT上料系统
		return true;
	}

	bool CMesWebHelp::GeneralUrlMesWeb(const std::string& strGeneralUrl,bool bSaveCookieFile /* = false */,bool bUseCookieFile /* = true */)
	{
		std::string strResponceUtf8;
		SendWindowMsgFormat(JGW_A2W(strGeneralUrl).c_str());
		if (!mcCurlHttp.SendCurlHttpA(strGeneralUrl,bSaveCookieFile,bUseCookieFile,mstrCookieFilePath,strResponceUtf8,mstrErrorMessage))
		{
			mstrResponceLocal = JGW_UTF8ToLocal(strResponceUtf8);
			return false;
		}
		mstrResponceLocal = JGW_UTF8ToLocal(strResponceUtf8);
		return HandleHttpResponseMessage(mstrResponceLocal); 
	}

	bool CMesWebHelp::LoginMesWeb(const std::string& strAccount,const std::string& strPassword)
	{
		mstrAccount = strAccount;
		mstrPassword = strPassword;
		return GeneralUrlMesWeb(GetLogiMesWebUrl(),true,false);
	}

	bool CMesWebHelp::LogoutMesWeb()
	{
		return GeneralUrlMesWeb(GetLogoutMesWebUrl());
	}

	bool CMesWebHelp::LotCheckMesWeb(const std::string& strSerialNumber)
	{
		return GeneralUrlMesWeb(GetLotCheckMesWebUrl(strSerialNumber));
	}

	bool CMesWebHelp::SaveTestRecordMesWeb(const std::string& strStartTimer,const std::string& strEndTimer,const std::string& strTestStatus,const std::string& strSerialNumber,const std::string& strComment /* = "" */,const std::string& strTestName /* = "" */,const std::string& strFixtureNo /* = "" */,const std::string& strCableNo /* = "" */,const std::string& strInstrumentNo /* = "" */,const std::string& strVersion /* = "" */,const std::string& strPlatForm /* = "" */,const std::string& strDefectCode /* = "" */,const std::string& strDefectDesc /* = "" */,const std::string& strDecision /* = "" */)
	{
		return GeneralUrlMesWeb(GetSaveTestRecordMesWebUrl(strStartTimer,strEndTimer,strTestStatus,strSerialNumber,strComment,strTestName,strFixtureNo,strCableNo,strInstrumentNo,strVersion,strPlatForm,strDefectCode,strDefectDesc,strDecision));
	}
#if 0
	bool CMesWebHelp::SaveTestRecordMesWeb(bool bTestPass,const std::string& strStartTimer,const std::string& strSerialNumber,const std::string& strComment /* = "" */,const std::string& strTestName /* = "" */,const std::string& strFixtureNo /* = "" */,const std::string& strCableNo /* = "" */,const std::string& strInstrumentNo /* = "" */,const std::string& strVersion /* = "" */,const std::string& strPlatForm /* = "" */,const std::string& strDefectCode /* = "" */,const std::string& strDefectDesc /* = "" */,const std::string& strDecision /* = "" */)
	{
		std::string strEndTimer = JGW_GetCurrentTimeStampYMDHMSA();
		return GeneralUrlMesWeb(GetSaveTestRecordMesWebUrl(strStartTimer,bTestPass,strEndTimer,strSerialNumber));
	}

	bool CMesWebHelp::SaveTestRecordMesWeb(bool bIsStartTimer,bool bTestPass,const std::string& strSerialNumber,const std::string& strComment /* = "" */,const std::string& strTestName /* = "" */,const std::string& strFixtureNo /* = "" */,const std::string& strCableNo /* = "" */,const std::string& strInstrumentNo /* = "" */,const std::string& strVersion /* = "" */,const std::string& strPlatForm /* = "" */,const std::string& strDefectCode /* = "" */,const std::string& strDefectDesc /* = "" */,const std::string& strDecision /* = "" */)
	{
		if (bIsStartTimer)
		{
			mstrStartTimer = JGW_GetCurrentTimeStampYMDHMS();
			return true;
		}

		std::string strEndTimer = JGW_GetCurrentTimeStampYMDHMS();
		std::string strResponse;
		std::string strErrorMessage;

		if (!mcCurlHttp.SendCurlHttp(GetSaveTestRecordMesWebUrl(bTestPass,strEndTimer,strSerialNumber),false,true,mstrCookieFilePath,strResponse,strErrorMessage))
		{
			mstrErrorMessage = strErrorMessage;
			return false;
		}
		return HandleHttpResponseMessage(strResponse);
	}
#endif

	bool CMesWebHelp::LotNodeNextMesWeb(const std::string& strSerialNumber,const std::string& strShiftID)
	{
		return GeneralUrlMesWeb(GetLotNodeNextMesWebUrl(strSerialNumber,strShiftID));
	}

	bool CMesWebHelp::IMEICheckMesWeb(const std::string& strImei)
	{
		return GeneralUrlMesWeb(GetIMEICheckMesWebUrl(strImei));
	}

	bool CMesWebHelp::IMEINextMesWeb(const std::string& strImei,const std::string& strPcbSN,const std::string& strImeiInfo,const std::string& strComment)
	{
		return GeneralUrlMesWeb(GetIMEINextMesWebUrl(strImei,strPcbSN,strImeiInfo,strComment));
	}

	bool CMesWebHelp::MoInfoBySNMesWeb(const std::string& strSerialNumber,LPS_MoInfo psMoInfo,const std::string& strSNType /* = L"SN" */)
	{
		if (!GeneralUrlMesWeb(GetMoInfoBySNMesWebUrl(strSerialNumber,strSNType)))
		{
			return false;
		}

		if (NULL != psMoInfo)
		{
			//{"MONO":"4520094692-10","SONO":"DHW247105","ProductCode":"F205-JW-6151B01","ProductName":"JW-PTP6151-MBOM-JW-无logo 白色","SOProjectName":"C300-JW-CN-CBU-PR","ProjectName":"C300","SN":"LEQ22425001108","CODE":0,"MSG":"成功"}
			psMoInfo->mstrMono = JGW_GetJsonICaseStringFieldDef(mstrResponceLocal,"MONO","");
			psMoInfo->mstrSono = JGW_GetJsonICaseStringFieldDef(mstrResponceLocal,"SONO","");
			psMoInfo->mstrProductCode = JGW_GetJsonICaseStringFieldDef(mstrResponceLocal,"ProductCode","");
			psMoInfo->mstrProductName = JGW_GetJsonICaseStringFieldDef(mstrResponceLocal,"ProductName","");
			psMoInfo->mstrSOProjectName = JGW_GetJsonICaseStringFieldDef(mstrResponceLocal,"SOProjectName","");
			psMoInfo->mstrProjectName = JGW_GetJsonICaseStringFieldDef(mstrResponceLocal,"ProjectName","");
			psMoInfo->mstrSn = JGW_GetJsonICaseStringFieldDef(mstrResponceLocal,"SN","");
		}

		return true;
	}

	bool CMesWebHelp::MoInfoMesWeb(std::string& strResponceLocal,const std::string& strMONO)
	{
		if (!GeneralUrlMesWeb(GetMoInfoMesWebUrl(strMONO)))
		{
			return false;
		}
#if 0
		// SONO 以及MONO
		//{"code":0,"data":{"androidKeyFile":"","cartonWeightLow":"0.00","cartonWeightTop":"0.00","color":"","dASWSECBinFile":"","databaseFTPUrl":" ","eFuseUrl":"","eFuseXmlFile":"","efuse":"","elabelPicPath":"","featureOrSmart":"","ftppwd":"9DNJXIZD9i2A","ftpuser":"readonly","giftWeightLow":"0.00","giftWeightTop":"0.00","iMEIProtect":"N","iSIMEISecret":"","iSSIMLock":"","isAutoRestore":"","isCardSetting":"","isDualCard":"1","isIMEISignature":"","isReadEMMCFlash":"","isSIMLock":"","isWeChatRegist":"","isWriteAndroidKeyFile":"N","isWriteBT":"Y","isWriteDRMKey":"N","isWriteETH0":"","isWriteElabelPic":"","isWriteEsn":"N","isWriteMEID":"N","isWriteMFD":"N","isWriteMSL":"N","isWriteOTKSL":"N","isWriteSKU":"","isWriteSN":"Y","isWriteSWSKU":"","isWriteTAcode":"","isWriteWallpaperID":"","isWriteWifi":"Y","item":"10","misc2FilePath":"","platform":"SM6225","preLoaderBinFile":"","productName":"T577A-MBOM-SKU1_32+4_黑色_国内版_A1_一供","qty":"377","sIMLockFile":"","sONO":"MTS251445","sOProjectName":"T577A-HCDH-CN-32+4-SS-Retail-SKU1-MP","scatterTxtFile":"","softName":"","softWareVarCustomer":"","softWareVarTinno":"","uniqueAndroidKey":"","xQCNFilePath":""},"message":"succeed"}
		strSono = JGW_GetJsonICaseStringFieldDef(mstrResponceUtf8,"sONO","");
		if (strSono.empty())
		{
			mstrErrorMessage = "Failed to get SONO";
			mstrErrorMessage += " Error : ";
			mstrErrorMessage += mstrResponceUtf8;
			return false;
		}
		//if (NULL != psMoInfo)
		//{
		//	//{"MONO":"4520094692-10","SONO":"DHW247105","ProductCode":"F205-JW-6151B01","ProductName":"JW-PTP6151-MBOM-JW-无logo 白色","SOProjectName":"C300-JW-CN-CBU-PR","ProjectName":"C300","SN":"LEQ22425001108","CODE":0,"MSG":"成功"}
		//	psMoInfo->mstrMono = GetNotifStringField(strResponse,L"MONO");
		//	psMoInfo->mstrSono = GetNotifStringField(strResponse,L"SONO");
		//	psMoInfo->mstrProductCode = GetNotifStringField(strResponse,L"ProductCode");
		//	psMoInfo->mstrProductName = GetNotifStringField(strResponse,L"ProductName");
		//	psMoInfo->mstrSOProjectName = GetNotifStringField(strResponse,L"SOProjectName");
		//	psMoInfo->mstrProjectName = GetNotifStringField(strResponse,L"ProjectName");
		//}
#endif
		strResponceLocal = mstrResponceLocal;

		return true;
	}

	bool CMesWebHelp::GetProductInfoBySNMesWeb(std::string& strResponceLocal,const std::string& strProductSN,const std::string& strRefSN,const std::string& strProductType /* = L"IMEI" */)
	{
		if (!GeneralUrlMesWeb(GetProductInfoBySNMesWebUrl(strProductSN,strRefSN,strProductType)))
		{
			return false;
		}
		strResponceLocal = mstrResponceLocal;
		// {"MSG":"OK","CODE":0,"SnInfo":{}}
		return true;
	}


	bool CMesWebHelp::HandleHttpResponseMessage(const std::string& strResponceLocal)
	{
		int nCode = JGW_GetJsonIntICaseFieldDef(strResponceLocal,"CODE",998989);
		std::string strMsg = JGW_GetJsonICaseStringFieldDef(strResponceLocal,"MSG","");
		if (nCode != 0)
		{
			SendWindowMsgFormat(JGW_A2W(strResponceLocal).c_str());
			if (strMsg.empty()) strMsg = strResponceLocal;
		}

		switch (nCode)
		{
		case 0:
			mstrErrorMessage = "";
			return true;
		case 1:
			mstrErrorMessage = strMsg;
			break;
		case 100:
			mstrErrorMessage = "未登录，当客户端收到此错误代码后需要重新登录系统。";
			break;
		case 101:
			mstrErrorMessage = "账号密码错误";
			break;
		case 102:
			mstrErrorMessage = "作业单元未登记";
			break;
		case 103:
			mstrErrorMessage = "作业单元无本工序权限";
			break;
		case 104:
			mstrErrorMessage = "硬件标识错误";
			break;
		case 200:
			mstrErrorMessage = strMsg;
			break;
		default:
			mstrErrorMessage = strMsg;
			break;
		}

		return false;
	}



	void CMesWebHelp::GetMesWebHelpWorkingUnitHardwareID()
	{
		try
		{
			mstrWorkingUnitID = JGW_UTF8ToLocal(Poco::Environment::nodeName());
			mstrHardwareID = JGW_UTF8ToLocal(Poco::Environment::nodeId());
#ifdef _DEBUG
			//mstrWorkingUnitID = "TEST-DL-G1-01";
			mstrWorkingUnitID = "C0500";
			//mstrWorkingUnitID = "MXTP-M4-LHMJ-01";
			//mstrWorkingUnitID = "TEST-MUJIAN-04";
			//mstrWorkingUnitID = "TEST-BT-E5-02";	
			//mstrWorkingUnitID = "MX-MES-B101";
			//mstrWorkingUnitID = "DESKTOP-PSMA2H4";
#endif
		}
		catch (Poco::Exception&)
		{

		}
	}

	void CMesWebHelp::LoadMesWebHelpConfig()
	{
		CCJGW_ConfigIni cConfigIni;
		std::wstring strMesWebHelpConfigFilePath;

		JGW_FormatWString(strMesWebHelpConfigFilePath,L"%s//config//%s",JGW_GetApplicationFolder(),_T(MES_WEB_HELP_CONFIG_NAME));
		cConfigIni.InitIniFilePath(strMesWebHelpConfigFilePath.c_str());

		mstrMesUrl = cConfigIni.GetIniKeyValueA(L"MES",L"Mes_URL",L"http:////192.168.8.4:8803//messervice//api");
		mstrClientType = cConfigIni.GetIniKeyValueA(L"MES",L"ClientType",L"AOI");
		mstrVer = cConfigIni.GetIniKeyValueA(L"MES",L"MES_Ver",L"4.0");
		mstrWorkingUnitID = cConfigIni.GetIniKeyValueA(L"MES",L"HostName",JGW_A2W(mstrWorkingUnitID).c_str());
		mstrHardwareID = cConfigIni.GetIniKeyValueA(L"MES",L"ComputerMAC",JGW_A2W(mstrHardwareID).c_str());

		//Poco::AutoPtr<Poco::Util::IniFileConfiguration> pConf(new Poco::Util::IniFileConfiguration(strMesWebHelpConfigFilePath));

		//mstrMesUrl = pConf->getString("Mes_URL","http:////192.168.8.4:8803//messervice//api");
		//mstrClientType = pConf->getString("ClientType","AOI");
		//mstrVer = pConf->getString("MES_Ver","4.0");
	}

	void CMesWebHelp::SetOperationID(const std::string& strOperationID)
	{
		mstrOperationID = strOperationID;
	}

	void CMesWebHelp::SetMono(const std::string& strMono)
	{
		if (!strMono.empty())
		{
			mstrMono = strMono;
		}
	}

	std::string CMesWebHelp::GetLogiMesWebUrl()
	{
		// http:////api.example.com:8085/messervice/api/login?account=806003&password=123456&WorkingUnitID=PCName&HardwareID=E8982F09&Ver=1.3&ClientType=AOI&OperationId=AOI
		std::string strLogiMesWebUrl;
		if (std::string::npos != mstrOperationID.find("-1"))
		{
			JGW_FormatString(strLogiMesWebUrl,"%s/login?account=%s&password=%s&WorkingUnitID=%s&HardwareID=%s&Ver=%s&ClientType=%s&OperationId=%s",mstrMesUrl.c_str(),mstrAccount.c_str(),mstrPassword.c_str(),mstrWorkingUnitID.c_str(),mstrHardwareID.c_str(),mstrVer.c_str(),mstrClientType.c_str(),mstrOperationID.c_str());
		}
		else
		{
			JGW_FormatString(strLogiMesWebUrl,"%s/login?account=%s&password=%s&WorkingUnitID=%s&HardwareID=%s&Ver=%s&ClientType=%s",mstrMesUrl.c_str(),mstrAccount.c_str(),mstrPassword.c_str(),mstrWorkingUnitID.c_str(),mstrHardwareID.c_str(),mstrVer.c_str(),mstrClientType.c_str());
		}
		return strLogiMesWebUrl;
	}

	std::string CMesWebHelp::GetLogoutMesWebUrl()
	{
		// http://api.example.com:8085/messervice/api/logout
		return mstrMesUrl + "/logout";
	}

	std::string CMesWebHelp::GetLotCheckMesWebUrl(const std::string& strSerialNumber)
	{
		// http:////api.example.com:8085/messervice/api/LotCheck?SN=LM141546000006&MONO=dhw131492-060&OperationID=001
		std::string strUrl;
		JGW_FormatString(strUrl,"%s/LotCheck?SN=%s&MONO=%s&OperationID=%s",mstrMesUrl.c_str(),strSerialNumber.c_str(),mstrMono.c_str(),mstrOperationID.c_str());
		return strUrl;
	}

	std::string CMesWebHelp::GetSaveTestRecordMesWebUrl(const std::string& strStartTimer,const std::string& strEndTimer,const std::string& strTestStatus,const std::string& strSerialNumber,const std::string& strComment /* = "" */,const std::string& strTestName /* = "" */,const std::string& strFixtureNo /* = "" */,const std::string& strCableNo /* = "" */,const std::string& strInstrumentNo /* = "" */,const std::string& strVersion /* = "" */,const std::string& strPlatForm /* = "" */,const std::string& strDefectCode /* = "" */,const std::string& strDefectDesc /* = "" */,const std::string& strDecision /* = "" */)
	{
		// yyyyMMddHHmmss
		// http:////api.example.com:8085/messervice/api/SaveTestRecord?SN=LM141546000006&MONO=dhw131492-060&OperationID=001&StartTime=20250818170733&EndTime=20250818170833&Status=PASS
		std::string strUrl;
		JGW_FormatString(strUrl,"%s/SaveTestRecord?SN=%s&MONO=%s&OperationID=%s&StartTime=%s&EndTime=%s&Status=%s",mstrMesUrl.c_str(),strSerialNumber.c_str(),mstrMono.c_str(),mstrOperationID.c_str(),strStartTimer.c_str(),strEndTimer.c_str(),strTestStatus.c_str());

		if (!strComment.empty())
		{
			strUrl += "&Comment=";
			strUrl += strComment;
		}
		if (!strTestName.empty())
		{
			strUrl += "&TestName=";
			strUrl += strTestName;
		}
		if (!strFixtureNo.empty())
		{
			strUrl += "&FixtureNo=";
			strUrl += strFixtureNo;
		}
		if (!strCableNo.empty())
		{
			strUrl += "&CableNo=";
			strUrl += strCableNo;
		}
		if (!strInstrumentNo.empty())
		{
			strUrl += "&InstrumentNo=";
			strUrl += strInstrumentNo;
		}
		if (!strVersion.empty())
		{
			strUrl += "&Version=";
			strUrl += strVersion;
		}
		if (!strPlatForm.empty())
		{
			strUrl += "&Platform=";
			strUrl += strPlatForm;
		}
		if (!strDefectCode.empty())
		{
			strUrl += "&DefectCode=";
			strUrl += strDefectCode;
		}
		if (!strDefectDesc.empty())
		{
			strUrl += "&DefectDesc=";
			strUrl += strDefectDesc;
		}
		if (!strDecision.empty())
		{
			strUrl += "&Decision=";
			strUrl += strDecision;
		}

		return strUrl;
	}

	//std::string CMesWebHelp::GetSaveTestRecordMesWebUrl(const std::string& strStartTimer,bool bTestPass,const std::string& strEndTimer,const std::string& strSerialNumber,const std::string& strComment /* = "" */,const std::string& strTestName /* = "" */,const std::string& strFixtureNo /* = "" */,const std::string& strCableNo /* = "" */,const std::string& strInstrumentNo /* = "" */,const std::string& strVersion /* = "" */,const std::string& strPlatForm /* = "" */,const std::string& strDefectCode /* = "" */,const std::string& strDefectDesc /* = "" */,const std::string& strDecision /* = "" */)
	//{
	//	// bTestPass ? L"PASS":L"FAIL"
	//	return GetSaveTestRecordMesWebUrl(strStartTimer,strEndTimer,bTestPass ? L"PASS":L"FAIL",strSerialNumber,strComment,strTestName,strFixtureNo,strCableNo,strInstrumentNo,strVersion,strPlatForm,strDefectCode,strDefectDesc,strDecision);
	//}

	std::string CMesWebHelp::GetLotNodeNextMesWebUrl(const std::string& strSerialNumber,const std::string& strShiftID)
	{
		return JGW_GetFormatString("%s/LotNodeNext?SN=%s&MONO=%s&OperationID=%s&ShiftID=%s",mstrMesUrl.c_str(),strSerialNumber.c_str(),mstrMono.c_str(),mstrOperationID.c_str(),strShiftID.c_str());
	}

	std::string CMesWebHelp::GetIMEICheckMesWebUrl(const std::string& strImei)
	{
		return JGW_GetFormatString("%s/IMEICheck?IMEI=%s&MONO=%s&OperationID=%s",mstrMesUrl.c_str(),strImei.c_str(),mstrMono.c_str(),mstrOperationID.c_str());
	}

	std::string CMesWebHelp::GetIMEINextMesWebUrl(const std::string& strImei,const std::string& strPcbSN /* = "" */,const std::string& strImeiInfo /* = "" */,const std::string& strComment /* = "" */)
	{
		// URL: /messervice/api/IMEINext
		std::string strUrl;
		JGW_FormatString(strUrl,"%s/IMEINext?IMEI=%s&MONO=%s&OperationID=%s",mstrMesUrl.c_str(),strImei.c_str(),mstrMono.c_str(),mstrOperationID.c_str());

		if (!strPcbSN.empty())
		{
			strUrl += "&PCBSN=";
			strUrl += strPcbSN;
		}
		if (!strImeiInfo.empty())
		{
			strUrl += "&ImeiInfo=";
			strUrl += strImeiInfo;
		}
		if (!strComment.empty())
		{
			strUrl += "&Comment=";
			strUrl += strComment;
		}

		return strUrl;
	}

	std::string CMesWebHelp::GetMoInfoBySNMesWebUrl(const std::string& strSerialNumber,const std::string& strSNType /* = L"SN" */)
	{
		// URL:/messervice/api/MoInfoBySN
		std::string strUrl;
		JGW_FormatString(strUrl,"%s/MoInfoBySN?SN=%s&OperationID=%s",mstrMesUrl.c_str(),strSerialNumber.c_str(),mstrOperationID.c_str());

		if (!strSNType.empty())
		{
			strUrl += "&SNType=";
			strUrl += strSNType;
		}
		return strUrl;
	}

	std::string CMesWebHelp::GetMoInfoMesWebUrl(const std::string& strMONO)
	{
		// URL: /messervice/api/MoInfo
		return JGW_GetFormatString("%s/MoInfo?&MONO=%s&OperationID=%s",mstrMesUrl.c_str(),strMONO.c_str(),mstrOperationID.c_str());
	}

	std::string CMesWebHelp::GetProductInfoBySNMesWebUrl(const std::string& strProductSN,const std::string& strRefSN,const std::string& strProductType /* = L"IMEI" */)
	{
		// URL: /messervice/api/ProductInfo

		std::string strUrl;
		JGW_FormatString(strUrl,"%s/ProductInfo?ProductSN=%s",mstrMesUrl.c_str(),strProductSN.c_str(),mstrOperationID.c_str());

		if (!mstrMono.empty())
		{
			strUrl += "&MONO=";
			strUrl += mstrMono;
		}

		if (!strRefSN.empty())
		{
			strUrl += "&RefSN=";
			strUrl += strRefSN;
		}

		if (!mstrOperationID.empty() && std::string::npos == mstrOperationID.find("-1"))
		{
			strUrl += "&OperationID=";
			strUrl += mstrOperationID;
		}

		if (!strProductType.empty())
		{
			strUrl += "&ProductType=";
			strUrl += strProductType;
		}
		return strUrl;
	}
}
