#include "StdAfx.h"
#include "CJGW_MesTcpServiceDataHandle.h"
#include "CJGW_FTM_Json.h"
namespace JGW
{
	//! 请求登录账号
	const int LOGIN_USER = 0;
	//! 获取工单列表 TDO:未实现
	const int GET_MOLIST = 1;
	//! 获取订单列表 TDO:未实现
	const int GET_SOLIST = 2;
	//! 测试SQL连接
	const int TEST_CONNECT = 3;
	//!获取IMEI信息，并检查当前IMEI是否属于当前工单以及号段信息 绑定PCBA SN
	const int GET_INFO_IMEI = 4;
	//! 获取SN相关信息，检查当前SN与工单的对应关系
	const int GET_INFO_SN = 5;
	//! 获取订单工单信息
	const int GET_INFO_SOMO = 6;
	//! 获取测试状态信息
	const int GET_TEST_STATUS = 7;
	//! 退出用户登录
	const int GET_LOGOUT = 8;
	//! SN过站信息
	const int NEXT_SN = 9;
	//! IMEI过站信息
	const int NEXT_IMEI = 10;
	//! 保存测试记录
	const int SAVE_TEST_RECORDS = 11;

	CCJGW_MesTcpServiceDataHandle::CCJGW_MesTcpServiceDataHandle(void)
	{
	}


	CCJGW_MesTcpServiceDataHandle::~CCJGW_MesTcpServiceDataHandle(void)
	{
	}

	std::string GetMESResponseBaseJson(bool bStatus,int nResponseType,const std::string& strReturnMessageLocal)
	{
		boost::property_tree::ptree ptWriteReadPerasoJson;
		ptWriteReadPerasoJson.put("bStatus",bStatus);
		ptWriteReadPerasoJson.put("responseType",nResponseType);
		ptWriteReadPerasoJson.put("returnMessage",strReturnMessageLocal);
		std::stringstream sstream;
		boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
		std::string strJson = sstream.str();
		SendWindowMsgFormat(L"write : %s",JGW_A2W(strJson).c_str());
		SendWindowMsgFormat(L"write : bStatus : %s, responseType : %d,returnMessage : %s",bStatus ? L"true" : L"false",nResponseType,JGW_A2W(strReturnMessageLocal).c_str());
		return strJson;
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnHandleMsg(const std::string& strMsg)
	{
		//! requestType
		int nRequestType = -1;
		CCJGW_FTM_Json cFtmJson;

		if (!cFtmJson.FromJsonToString(strMsg)) return GetMESResponseBaseJson(false,-1,"invalid command");
		cFtmJson.GetJsonValueToKey("requestType",nRequestType);

		switch (nRequestType)
		{
		case LOGIN_USER:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"username\":\"%s\",\"userpwd\":\"%s\",\"operationId\":\"%s\"}",E_LOGIN_USER,strUsername.c_str(),strUserpwd.c_str(),strOperationId.c_str());
			{
				std::string strUserName,strUserPassword,strOperationId;
				cFtmJson.GetJsonValueToKey("username",strUserName);
				cFtmJson.GetJsonValueToKey("userpwd",strUserPassword);
				cFtmJson.GetJsonValueToKey("operationId",strOperationId);
				return OnLoginUser(strUserName,strUserPassword,strOperationId);
			}
			break;
		case GET_MOLIST:
			return GetMESResponseBaseJson(false,nRequestType,"未实现此功能");
		case GET_SOLIST:
			return GetMESResponseBaseJson(false,nRequestType,"未实现此功能");
		case TEST_CONNECT:
			return OnTestConnect();
		case GET_INFO_IMEI:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"LotPKSN\":\"%s\",\"PCBSN\":\"%s\",\"MONO\":\"%s\"}",E_GET_INFO_IMEI,strLotPKSN.c_str(),strPCBSN.c_str(),strMONO.c_str());
			{
				std::string strLotPKSN,strPCBSN,strMono;
				cFtmJson.GetJsonValueToKey("LotPKSN",strLotPKSN);
				cFtmJson.GetJsonValueToKey("PCBSN",strPCBSN);
				cFtmJson.GetJsonValueToKey("MONO",strMono);
				return OnGetInfoToImei(strLotPKSN,strPCBSN,strMono);
			}
			break;
		case GET_INFO_SN:
			{
				// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"pcbasn\":\"%s\",\"mono\":\"%s\"}",E_GET_INFO_SN,strPCBASN.c_str(),strMono.c_str());
				std::string strPcbaSN,strMono;
				cFtmJson.GetJsonValueToKey("pcbasn",strPcbaSN);
				cFtmJson.GetJsonValueToKey("mono",strMono);
				return OnGetInfoToSn(strPcbaSN,strMono,GET_INFO_SN);
			}
			break;
		case GET_INFO_SOMO:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"mono\":\"%s\",\"isso\":\"%s\"}",E_GET_INFO_SOMO,strmono.c_str(),strisso.c_str());
			{
				std::string strMono;
				std::string strIsso;
				cFtmJson.GetJsonValueToKey("mono",strMono);
				cFtmJson.GetJsonValueToKey("isso",strIsso);
				return OnGetInfoToMono(strMono,NULL != JGW_StrCaseStr(strIsso.c_str(),"Y"));
			}
			break;
		case GET_TEST_STATUS:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"lotSN\":\"%s\",\"operationId\":\"%s\"}",E_GET_TEST_STATUS,strlotSN.c_str(),stroperationId.c_str());
			{
				std::string strLotSn,strOperationId;
				cFtmJson.GetJsonValueToKey("lotSN",strLotSn);
				cFtmJson.GetJsonValueToKey("operationId",strOperationId);
				return OnGetTestStatus(strLotSn,strOperationId);
			}
			break;
		case GET_LOGOUT:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d}",E_LOGOUT);
			{
				return OnLogout();
			}
			break;
		case NEXT_SN:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"LotSN\":\"%s\",\"ShiftID\":\"%s\"}",E_NEXT_SN,strLotSN.c_str(),strShiftID.c_str());
			{
				std::string strLotPKSN,strShiftID,strMono;
				cFtmJson.GetJsonValueToKey("LotSN",strLotPKSN);
				cFtmJson.GetJsonValueToKey("ShiftID",strShiftID);
				return OnNextSn(strLotPKSN,strShiftID);
			}

			break;
		case NEXT_IMEI:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"IMEI_M_S_SN\":\"%s\",\"LotSN\":\"%s\",\"ShiftID\":\"%s\",\"SoftWareVarTinno\":\"%s\",\"SoftWareVarCustomer\":\"%s\",\"Remark\":\"%s\",\"Unlockcode\":\"%s\",\"ImeiInfo\":\"%s\"}",E_NEXT_IMEI,strIMEI_M_S_SN.c_str(),strLotSN.c_str(),strShiftID.c_str(),strSoftWareVarTinno.c_str(),strSoftWareVarCustomer.c_str(),strRemark.c_str(),strUnlockcode.c_str(),strImeiInfo.c_str());
			{
				std::string strIMEI_M_S_SN,strLotSN,strShiftID,strSoftWareVarTinno,strSoftWareVarCustomer,strRemark,strUnlockcode,strImeiInfo;
				cFtmJson.GetJsonValueToKey("IMEI_M_S_SN",strIMEI_M_S_SN);
				cFtmJson.GetJsonValueToKey("LotSN",strLotSN);
				cFtmJson.GetJsonValueToKey("ShiftID",strShiftID);
				cFtmJson.GetJsonValueToKey("SoftWareVarTinno",strSoftWareVarTinno);
				cFtmJson.GetJsonValueToKey("SoftWareVarCustomer",strSoftWareVarCustomer);
				cFtmJson.GetJsonValueToKey("Remark",strRemark);
				cFtmJson.GetJsonValueToKey("Unlockcode",strUnlockcode);
				cFtmJson.GetJsonValueToKey("ImeiInfo",strImeiInfo);
				return OnNextImei(strIMEI_M_S_SN
					,strLotSN
					,strShiftID
					,strSoftWareVarTinno
					,strSoftWareVarCustomer
					,strRemark
					,strUnlockcode
					,strImeiInfo);
			}
			break;
		case SAVE_TEST_RECORDS:
			// JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"sn\":\"%s\",\"shiftID\":\"%s\",\"startDate\":\"%s\",\"endDate\":\"%s\",\"status\":\"%s\",\"comment\":\"%s\",\"type\":\"%s\",\"testName\":\"%s\",\"fixtureNo\":\"%s\",\"cableNo\":\"%s\",\"instrumentNo\":\"%s\",\"version\":\"%s\",\"platform\":\"%s\",\"failCode\":\"%s\",\"failCodeDesc\":\"%s\"}",E_SAVE_TEST_RECORDS,sn.c_str(),shiftID.c_str(),startDate.c_str(),endDate.c_str(),status.c_str(),comment.c_str(),type.c_str(),testName.c_str(),fixtureNo.c_str(),cableNo.c_str(),instrumentNo.c_str(),version.c_str(),platform.c_str(),failCode.c_str(),failCodeDesc.c_str());
			{
				std::string sn,shiftID,startDate, endDate, status,comment,type,testName = "",fixtureNo = "",cableNo = "",instrumentNo = "",version = "",platform = "",failCode = "", failCodeDesc = "";
				cFtmJson.GetJsonValueToKey("sn",sn);
				cFtmJson.GetJsonValueToKey("shiftID",shiftID);
				cFtmJson.GetJsonValueToKey("startDate",startDate);
				cFtmJson.GetJsonValueToKey("endDate",endDate);
				cFtmJson.GetJsonValueToKey("status",status);
				cFtmJson.GetJsonValueToKey("comment",comment);
				cFtmJson.GetJsonValueToKey("type",type);
				cFtmJson.GetJsonValueToKey("testName",testName);
				cFtmJson.GetJsonValueToKey("fixtureNo",fixtureNo);
				cFtmJson.GetJsonValueToKey("cableNo",cableNo);
				cFtmJson.GetJsonValueToKey("instrumentNo",instrumentNo);
				cFtmJson.GetJsonValueToKey("version",version);
				cFtmJson.GetJsonValueToKey("platform",platform);
				cFtmJson.GetJsonValueToKey("failCode",failCode);
				cFtmJson.GetJsonValueToKey("failCodeDesc",failCodeDesc);
				// 2025-08-21 11:22:46
				JGW_ReplaceStringA(startDate,"-","");
				JGW_ReplaceStringA(startDate," ","");
				JGW_ReplaceStringA(startDate,":","");

				JGW_ReplaceStringA(endDate,"-","");
				JGW_ReplaceStringA(endDate," ","");
				JGW_ReplaceStringA(endDate,":","");

				return OnSaveTestRecords(sn,shiftID,startDate,endDate,status,
					comment,
					type,
					testName,
					fixtureNo,
					cableNo,
					instrumentNo,
					version,
					platform,
					failCode,
					failCodeDesc);

			}
			break;
		}
		return GetMESResponseBaseJson(false,nRequestType,"invalid command");
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnLoginUser(const std::string& strUserName,const std::string& strUserPassword,const std::string& strOperationId)
	{
		mcMesWebHelp.SetOperationID(strOperationId);
		return GetMESResponseBaseJson(mcMesWebHelp.LoginMesWeb(strUserName,strUserPassword),LOGIN_USER,mcMesWebHelp.GetErrorMessage());
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnTestConnect()
	{
		return GetMESResponseBaseJson(mcMesWebHelp.TestConnect(),TEST_CONNECT,mcMesWebHelp.GetErrorMessage());
	}

	boost::property_tree::ptree AddChildPtreeToArray(std::vector<std::string>& vstrValue)
	{
		boost::property_tree::ptree ptreeChildArray;
		for (std::vector<std::string>::iterator it = vstrValue.begin();
			it != vstrValue.end();
			++ it)
		{
			boost::property_tree::ptree array1;
			array1.put_value(it[0]);
			ptreeChildArray.push_back(std::make_pair("", array1));
		}
		return ptreeChildArray;
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnGetInfoToSn(const std::string& strPcbaSN,const std::string& strMono,int nRequestType,const std::string& strSNType /* = L"SN" */)
	{
		S_MoInfo sMoInfo;
		mcMesWebHelp.SetMono(strMono);
		if (!mcMesWebHelp.MoInfoBySNMesWeb(strPcbaSN,&sMoInfo,""))
		{
			return GetMESResponseBaseJson(false,nRequestType,mcMesWebHelp.GetErrorMessage());
		}
		// "MONOSMT","MONOAssy","MONOPack",SONO
		// {"MONO":"4520094692-10","SONO":"DHW247105","ProductCode":"F205-JW-6151B01","ProductName":"JW-PTP6151-MBOM-JW-无logo 白色","SOProjectName":"C300-JW-CN-CBU-PR","ProjectName":"C300","SN":"LEQ22425001108","CODE":0,"MSG":"成功"}
		// public string[] paraName;
		// public string[] paraValue;
		boost::property_tree::ptree ptWriteReadPerasoJson;
		ptWriteReadPerasoJson.put("bStatus",true);
		ptWriteReadPerasoJson.put("responseType",nRequestType);
		
#if 0
		ptWriteReadPerasoJson.put("MONOSMT",JGW_W2A(sMoInfo.mstrMono,CP_UTF8));
		ptWriteReadPerasoJson.put("MONOAssy",JGW_W2A(sMoInfo.mstrMono,CP_UTF8));
		ptWriteReadPerasoJson.put("MONOPack",JGW_W2A(sMoInfo.mstrMono,CP_UTF8));
		ptWriteReadPerasoJson.put("SONO",JGW_W2A(sMoInfo.mstrSono,CP_UTF8));
#else
		std::vector<std::string> vstrParamName;
		vstrParamName.push_back("MONOSMT");
		vstrParamName.push_back("MONOAssy");
		vstrParamName.push_back("MONOPack");
		vstrParamName.push_back("SONO");
		vstrParamName.push_back("ProductCode");
		vstrParamName.push_back("ProductName");
		vstrParamName.push_back("SOProjectName");
		vstrParamName.push_back("SN");
		ptWriteReadPerasoJson.put_child("paraName",AddChildPtreeToArray(vstrParamName));

		std::vector<std::string> vstrParamValue;
		vstrParamValue.push_back(sMoInfo.mstrMono);
		vstrParamValue.push_back(sMoInfo.mstrMono);
		vstrParamValue.push_back(sMoInfo.mstrMono);
		vstrParamValue.push_back(sMoInfo.mstrSono);
		mcMesWebHelp.SetMono(sMoInfo.mstrMono);

		vstrParamValue.push_back(sMoInfo.mstrProductCode);
		vstrParamValue.push_back(sMoInfo.mstrProductName);
		vstrParamValue.push_back(sMoInfo.mstrSOProjectName);
		vstrParamValue.push_back(sMoInfo.mstrSn);
		ptWriteReadPerasoJson.put_child("paraValue",AddChildPtreeToArray(vstrParamValue));

		// AddChildPtreeToArray
#endif
		ptWriteReadPerasoJson.put("returnMessage","");
		std::stringstream sstream;
		boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
		std::string strJson = sstream.str();
		SendWindowMsgFormat(L"write : %s",JGW_A2W(strJson).c_str());
		return strJson;
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnGetInfoToImei(const std::string& strLotPKSN,const std::string& strPCBSN,const std::string& strMono)
	{
		if (strMono.empty())
		{
			return OnGetInfoToSn(strLotPKSN,"",GET_INFO_IMEI,"IMEI");
		}
		else
		{
			std::string strResponceLocal;
			mcMesWebHelp.SetMono(strMono);

			if (!mcMesWebHelp.IMEICheckMesWeb(strLotPKSN))
			{
				return GetMESResponseBaseJson(false,GET_INFO_IMEI,mcMesWebHelp.GetErrorMessage());
			}

			if (!mcMesWebHelp.GetProductInfoBySNMesWeb(strResponceLocal,strLotPKSN,strPCBSN))
			{
				return GetMESResponseBaseJson(false,GET_INFO_IMEI,mcMesWebHelp.GetErrorMessage());
			}

			CCJGW_FTM_Json cFtmJson;
			if (!cFtmJson.FromJsonToString(strResponceLocal))
			{
				return GetMESResponseBaseJson(false,GET_INFO_IMEI,std::string("invalid json : ") + strResponceLocal);;
			}

			// {"MSG":"OK","CODE":0,"SnInfo":{}}
			boost::property_tree::ptree ptWriteReadPerasoJson;
			ptWriteReadPerasoJson.put("bStatus",true);
			ptWriteReadPerasoJson.put("responseType",GET_INFO_IMEI);
			// MONOPack SONO

			std::map<std::string,std::string> mmapKeyValue;
			std::vector<std::string> vstrParamName;
			std::vector<std::string> vstrParamValue;

			cFtmJson.GetJsonKeyValueToChildKey("SnInfo",mmapKeyValue);
			for (std::map<std::string,std::string>::iterator it = mmapKeyValue.begin();it != mmapKeyValue.end();++it)
			{
				vstrParamName.push_back(it->first);
				vstrParamValue.push_back(it->second);
			}

			ptWriteReadPerasoJson.put_child("paraName",AddChildPtreeToArray(vstrParamName));
			ptWriteReadPerasoJson.put_child("paraValue",AddChildPtreeToArray(vstrParamValue));
			ptWriteReadPerasoJson.put("returnMessage","");
#if 0
			public string[] paraName;
			public string[] paraValue;
			// "MONOSMT","MONOAssy","MONOPack",SONO
			boost::property_tree::ptree ptWriteReadPerasoJson;
			ptWriteReadPerasoJson.put("bStatus",true);
			ptWriteReadPerasoJson.put("responseType",GET_INFO_IMEI);
			ptWriteReadPerasoJson.put("MONOSMT",JGW_W2A(sMoInfo.mstrMono,CP_UTF8));
			ptWriteReadPerasoJson.put("MONOAssy",JGW_W2A(sMoInfo.mstrMono,CP_UTF8));
			ptWriteReadPerasoJson.put("MONOPack",JGW_W2A(sMoInfo.mstrMono,CP_UTF8));
			ptWriteReadPerasoJson.put("SONO",JGW_W2A(sMoInfo.mstrSono,CP_UTF8));
			ptWriteReadPerasoJson.put("returnMessage","");
#endif
			std::stringstream sstream;
			boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
			std::string strJson = sstream.str();
			SendWindowMsgFormat(L"write : %s",JGW_A2W(strJson).c_str());
			return strJson;
		}
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnGetInfoToMono(const std::string& strMono,bool bIsSo)
	{
#if 0
		// {"MSG":"OK","CODE":0,"SnInfo":{}}
		boost::property_tree::ptree ptWriteReadPerasoJson;
		ptWriteReadPerasoJson.put("bStatus",true);
		ptWriteReadPerasoJson.put("responseType",GET_INFO_SOMO);

		std::map<std::string,std::string> mmapKeyValue;
		std::vector<std::string> vstrParamName;
		vstrParamName.push_back("MONOSMT");
		vstrParamName.push_back("MONOAssy");
		vstrParamName.push_back("MONOPack");
		vstrParamName.push_back("SONO");

		std::vector<std::string> vstrParamValue;
		vstrParamValue.push_back(strMono);
		vstrParamValue.push_back(strMono);
		vstrParamValue.push_back(strMono);
		vstrParamValue.push_back(strMono);

		ptWriteReadPerasoJson.put_child("paraName",AddChildPtreeToArray(vstrParamName));
		ptWriteReadPerasoJson.put_child("paraValue",AddChildPtreeToArray(vstrParamValue));
		ptWriteReadPerasoJson.put("returnMessage","");

		std::stringstream sstream;
		boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
		std::string strJson = sstream.str();
		SendWindowMsgFormat(L"write : %s",JGW_A2W(strJson).c_str());
		return strJson;
#else
		std::string strResponceLocal;

		if (!mcMesWebHelp.MoInfoMesWeb(strResponceLocal,strMono))
		{
			return GetMESResponseBaseJson(false,GET_INFO_SOMO,mcMesWebHelp.GetErrorMessage());
		}

		CCJGW_FTM_Json cFtmJson;
		if (!cFtmJson.FromJsonToString(strResponceLocal))
		{
			return GetMESResponseBaseJson(false,GET_INFO_SOMO,std::string("invalid json : ") + strResponceLocal);;
		}
		//! "code":0,"data":{"androidKeyFile":"","cartonWeightLow":"0.00","cartonWeightTop":"0.00","color":"","dASWSECBinFile":"","databaseFTPUrl":" ","eFuseUrl":"","eFuseXmlFile":"","efuse":"","elabelPicPath":"","featureOrSmart":"","ftppwd":"9DNJXIZD9i2A","ftpuser":"readonly","giftWeightLow":"0.00","giftWeightTop":"0.00","iMEIProtect":"N","iSIMEISecret":"","iSSIMLock":"","isAutoRestore":"","isCardSetting":"","isDualCard":"1","isIMEISignature":"","isReadEMMCFlash":"","isSIMLock":"","isWeChatRegist":"","isWriteAndroidKeyFile":"N","isWriteBT":"Y","isWriteDRMKey":"N","isWriteETH0":"","isWriteElabelPic":"","isWriteEsn":"N","isWriteMEID":"N","isWriteMFD":"N","isWriteMSL":"N","isWriteOTKSL":"N","isWriteSKU":"","isWriteSN":"Y","isWriteSWSKU":"","isWriteTAcode":"","isWriteWallpaperID":"","isWriteWifi":"Y","item":"10","misc2FilePath":"","platform":"SM6225","preLoaderBinFile":"","productName":"T577A-MBOM-SKU1_32+4_黑色_国内版_A1_一供","qty":"377","sIMLockFile":"","sONO":"MTS251445","sOProjectName":"T577A-HCDH-CN-32+4-SS-Retail-SKU1-MP","scatterTxtFile":"","softName":"","softWareVarCustomer":"","softWareVarTinno":"","uniqueAndroidKey":"","xQCNFilePath":""},"message":"succeed"}
		boost::property_tree::ptree ptWriteReadPerasoJson;
		ptWriteReadPerasoJson.put("bStatus",true);
		ptWriteReadPerasoJson.put("responseType",GET_INFO_SOMO);
		// MONOPack SONO

		std::map<std::string,std::string> mmapKeyValue;
		std::vector<std::string> vstrParamName;
		vstrParamName.push_back("MONOSMT");
		vstrParamName.push_back("MONOAssy");
		vstrParamName.push_back("MONOPack");

		std::vector<std::string> vstrParamValue;
		vstrParamValue.push_back(strMono);
		vstrParamValue.push_back(strMono);
		vstrParamValue.push_back(strMono);
		if (!strMono.empty()) mcMesWebHelp.SetMono(strMono);

		cFtmJson.GetJsonKeyValueToChildKey("data",mmapKeyValue);
		for (std::map<std::string,std::string>::iterator it = mmapKeyValue.begin();it != mmapKeyValue.end();++it)
		{
			vstrParamName.push_back(it->first);
			vstrParamValue.push_back(it->second);
		}

		ptWriteReadPerasoJson.put_child("paraName",AddChildPtreeToArray(vstrParamName));
		ptWriteReadPerasoJson.put_child("paraValue",AddChildPtreeToArray(vstrParamValue));
		ptWriteReadPerasoJson.put("returnMessage","");

		std::stringstream sstream;
		boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
		std::string strJson = sstream.str();
		SendWindowMsgFormat(L"write : %s",JGW_A2W(strJson).c_str());
		return strJson;
#endif
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnGetTestStatus(const std::string& strLotSn,const std::string& strOperationId)
	{
		mcMesWebHelp.SetOperationID(strOperationId);
		return GetMESResponseBaseJson(mcMesWebHelp.LotCheckMesWeb(strLotSn),GET_TEST_STATUS,mcMesWebHelp.GetErrorMessage());
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnLogout()
	{
		return GetMESResponseBaseJson(mcMesWebHelp.LogoutMesWeb(),GET_LOGOUT,mcMesWebHelp.GetErrorMessage());
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnNextSn(const std::string& strLotSn,const std::string& strShiftID)
	{
		//mcMesWebHelp.SetOperationID(strOperationId);
		return GetMESResponseBaseJson(mcMesWebHelp.LotNodeNextMesWeb(strLotSn,strShiftID),NEXT_SN,mcMesWebHelp.GetErrorMessage());
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnNextImei(const std::string& strIMEI_M_S_SN,const std::string& strSN,const std::string& strShiftID,const std::string& strSoftWareVarTinno /* = L"" */,const std::string& strSoftWareVarCustomer /* = L"" */,const std::string& strRemark /* = L"" */,const std::string& strUnlockcode /* = L"" */,const std::string& strImeiInfo /* = L"" */)
	{
		//mcMesWebHelp.SetOperationID(strOperationId);
		// // 作业完成后，需将 IMEI 过站
		//bool IMEINextMesWeb(const std::wstring& strImei,const std::wstring& strPcbSN = L"",const std::wstring& strImeiInfo = L"",const std::wstring& strComment = L"");
		return GetMESResponseBaseJson(mcMesWebHelp.IMEINextMesWeb(strIMEI_M_S_SN,strSN,strImeiInfo,strShiftID),NEXT_IMEI,mcMesWebHelp.GetErrorMessage());
	}

	std::string CCJGW_MesTcpServiceDataHandle::OnSaveTestRecords(const std::string& sn, const std::string& shiftID, const std::string& startDate, const std::string& endDate, const std::string& status, const std::string& comment, const std::string& type, const std::string& testName /* = L"" */, const std::string& fixtureNo /* = L"" */, const std::string& cableNo /* = L"" */, const std::string& instrumentNo /* = L"" */, const std::string& version /* = L"" */, const std::string& platform /* = L"" */, const std::string& failCode /* = L"" */, const std::string& failCodeDesc /* = L"" */)
	{
		// ?"PASS":"FAIL"
		// bool SaveTestRecordMesWeb(bool bTestPass,const std::wstring& strStartTimer,const std::wstring& strSerialNumber,const std::wstring& strComment = L"",const std::wstring& strTestName = L"",const std::wstring& strFixtureNo = L"",const std::wstring& strCableNo = L"",const std::wstring& strInstrumentNo = L"",const std::wstring& strVersion = L"",const std::wstring& strPlatForm = L"",const std::wstring& strDefectCode = L"",const std::wstring& strDefectDesc = L"",const std::wstring& strDecision = L"");
		return GetMESResponseBaseJson(mcMesWebHelp.SaveTestRecordMesWeb(startDate,endDate,status,sn,comment,testName,fixtureNo,cableNo,instrumentNo,version,platform,failCode,failCodeDesc),SAVE_TEST_RECORDS,mcMesWebHelp.GetErrorMessage());
	}
}
