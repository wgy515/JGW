#include "StdAfx.h"
#include "CJGW_PositioningSystemProtocol.h"
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_PositioningSystemMemberVariable.h"
#include "WMM_2025/wmm.h"
#include "latlon_ellipsoidal_vincenty.h"
#include "CJGW_PSRabbitMQ.h"

#include "CJGW_Cef3BrowserEventHelp.h"

namespace JGW
{
	/// <summary>
	/// start scan id
	/// </summary>
	const int CLICK_START_SCAN_ID = 0;
	/// <summary>
	/// stop scan id
	/// </summary>
	//const int SWITCH_BLE_DISCOVERY = 1;
	const int STOP_SCAN_ID = 1;
	/// <summary>
	/// 连接BLE
	/// </summary>
	const int CONNECT_BLE_ID = 2;
	/// <summary>
	/// 连接Gatt
	/// </summary>
	const int CONNECT_GATT_ID = 3;
	/// <summary>
	/// 连接Characteristics
	/// </summary>
	const int CONNECT_Characteristics_ID = 4;
	/// <summary>
	/// dxcs 操作ID
	/// </summary>
	const int DXCS_OPERATION_ID = 5;
	/// <summary>
	/// BLE断开连接
	/// </summary>
	const int DISCONNECT_BLE_ID = 6;
	// 定位系统实例
	CCJGW_PositioningSystemProtocol* gpPositioningSystemProtocol;
	//! 获取指定字段的double数据
	double GetNotifDoubleField(const std::string& strNotif,const std::string& strField)
	{
		std::string strFloat = JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		return atof(strFloat.c_str());
	}
	//! 获取指定字段的double数据
	double GetNotifDoubleFieldDef(const std::string& strNotif,const std::string& strField,double def = 0.00)
	{
		std::string strFloat = JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		if (strFloat.empty()) return def;
		if (NULL != JGW_StrCaseStr(strFloat.c_str(),"NaN")) return def;
		return atof(strFloat.c_str());
	}

	//! 获取指定字段的int数据
	int GetNotifIntField(const std::string& strNotif,const std::string& strField)
	{
		std::string strFloat = JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		return atoi(strFloat.c_str());
	}
	//! 获取指定字段的int数据
	int GetNotifIntFieldDef(const std::string& strNotif,const std::string& strField,int def)
	{
		std::string strFloat = JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		if (strFloat.empty()) return def;
		if (NULL != JGW_StrCaseStr(strFloat.c_str(),"NaN")) return def;
		return atoi(strFloat.c_str());
	}

	// 获取指定字段的string数据
	std::string GetNotifStringField(const std::string& strNotif,const std::string& strField)
	{
		std::string strValue =  JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		JGW_ReplaceStringA(strValue,"\"","");
		return strValue;
	}
	//! 获取指定字段的string数据
	std::string GetNotifStringFieldDef(const std::string& strNotif,const std::string& strField,const std::string& strDes = "NaN")
	{
		std::string strValue =  JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		JGW_ReplaceStringA(strValue,"\"","");
		if (strValue.empty()) strValue = strDes;
		return strValue;
	}
	//! 获取指定字段的float数据
	float GetNotifFloatField(const std::string& strNotif,const std::string& strField)
	{
		std::string strFloat = JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		return static_cast<float>(atof(strFloat.c_str()));
	}
	//! 获取指定字段的float数据
	float GetNotifFloatFieldDef(const std::string& strNotif,const std::string& strField,float def = 0.00f)
	{
		std::string strFloat = JGW_GetSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		if (strFloat.empty()) return def;
		if (NULL != JGW_StrCaseStr(strFloat.c_str(),"NaN")) return def;
		return static_cast<float>(atof(strFloat.c_str()));
	}

	CCJGW_PositioningSystemProtocol::CCJGW_PositioningSystemProtocol(CCJGW_Cef3BrowserEventHelp* pCef3BrowserEventHelp,CCJGW_Cef3PositioningSystemUDPScokect* pCef3PositioningSystemUDPScokect) : mpCef3BrowserEventHelp(pCef3BrowserEventHelp),mpCef3PositioningSystemUDPScokect(pCef3PositioningSystemUDPScokect),mbConnectBLE(false),mbIsStartThread(false)/*,mnPositioningSystemID(-1)*/
	{
		// 重新校准地磁 {"type":1,"func":"recalibration_magnetic"}
		RegisterCppFuncToJs(L"recalibration_magnetic",OnClickRecalibrationMagnetic);
		//! 手动修改指南针数据 {"type":1,"func":"change_compass","compass":12}
		RegisterCppFuncToJs(L"change_compass",OnSettingCompassAngle);
		//! 手动更新经纬度 {"type":1,"func":"manual_update_latlng","index":0,"latitude":22,"latitude":113}
		RegisterCppFuncToJs(L"manual_update_latlng",OnManualUpdateLatLng);
		//! 取消手动修改指南针数据 {"type":1,"func":"cancel_compass"}
		RegisterCppFuncToJs(L"cancel_compass",OnCancelManualUpdateCompassAngle);
		//! 重新连接BLE {"type":1,"func":"reconnect_dxcs"}
		RegisterCppFuncToJs(L"reconnect_dxcs",OnReConnectDxcs);
		//! 登录房间 {"type":1,"func":"login_room","room_id":123456,"flag":A}
		RegisterCppFuncToJs(L"login_room",OnLoginRabbitMQ);
		//! 连接BLE设备
		RegisterCppFuncToJs(L"connect_ble",OnConnectBleDevice);

		gpPositioningSystemProtocol = this;
		// WMM2025 磁偏角计算初始化
		wmm_init();
		
	}


	CCJGW_PositioningSystemProtocol::~CCJGW_PositioningSystemProtocol(void)
	{
		StopPositioningSystemProtocolThread();
	}

	std::wstring CCJGW_PositioningSystemProtocol::GetErrorMessage()
	{
		return mstrErrorMessage;
	}

	bool CCJGW_PositioningSystemProtocol::StartPositioningSystemProtocolThread()
	{
		try
		{
			mbIsStartThread = true;
			//! 开启线程
			_thread.start(*this);
			return mbIsStartThread;
		}
		catch (Poco::Exception& ec)
		{
			mbIsStartThread = false;
			JGW_FormatWString(mstrErrorMessage,L"Start PS Thread Error : %s",JGW_A2W(ec.message()).c_str());
		}
		return false;
	}

	void CCJGW_PositioningSystemProtocol::StopPositioningSystemProtocolThread(void)
	{
		if (mbIsStartThread)
		{
			mbIsStartThread = false;
			mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(GetDisConnectBleUWPCommand());
			mbConnectBLE = false;
			_thread.join();
		}
	}

	void CCJGW_PositioningSystemProtocol::OnRecvRabbitMQMsg(const std::string& strMessage)
	{
		Log4WD_F(L"RMQ : %s",JGW_A2W(strMessage,CP_UTF8).c_str());
		//! {"altitude":608.901363822208,"tag":1,"type":1}
		std::string strTag = GetNotifStringField(strMessage,"tag");
		std::string strType = GetNotifStringField(strMessage,"type");
		if (strTag.empty() || strType.empty()) return;

		E_TAG_RMQ eTagRmq = (E_TAG_RMQ)atoi(strTag.c_str());
		if (eTagRmq == TAG_RMQ_VIEW || eTagRmq == TAG_RMQ_UNKNOWN) return;

		CCJGW_PositioningSystemMemberVariable *pMemberVariable = &CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable();
		//! TAG ID 不一致则为远程终端设备
		if (eTagRmq != CCJGW_PSRabbitMQ::GetTagRmq())
		{
			E_RabbitMQDataType eRmqType = (E_RabbitMQDataType)atoi(strType.c_str());
			switch (eRmqType)
			{
			case LATITUDELONGITUDE_TYPE:
				{
					//! {"latitude":22.664027,"longitude":113.794979,"tag":2,"type":0}
					pMemberVariable->UpdateRemoteGpsLngLat(GetNotifDoubleFieldDef(strMessage,"longitude",INVALID_LONGITUDE),GetNotifDoubleFieldDef(strMessage,"latitude",INVALID_LATITUDE));
				}
				break;
			case ALTITUDE_TYPE:
				//! {"altitude":608.9387677371966,"tag":1,"type":1}
				break;
			case EULERANGLE_TYPE:
				{
					//! {"pitch":-0.9121788,"roll":-2.0600238,"tag":1,"type":2,"yaw":152.30522}
					pMemberVariable->UpdateRemoteYaw(GetNotifFloatFieldDef(strMessage,"yaw",INVALID_YAW_F));
				}
				break;
			}
		}
	}

	void CCJGW_PositioningSystemProtocol::OnHandlePositioningSystemProtocolMsg(int nID,const std::string& strMessage)
	{
		std::string strJson(strMessage);
		JGW_ReplaceStringA(strJson," :",":");
		JGW_ReplaceStringA(strJson," :",":");
		JGW_ReplaceStringA(strJson," :",":");
		JGW_ReplaceStringA(strJson,": ",":");
		JGW_ReplaceStringA(strJson,": ",":");
		JGW_ReplaceStringA(strJson,": ",":");
		//gpPositioningSystemProtocol->mnPositioningSystemID = nID;
		Log4WD_F(L"DXCS : %s",JGW_A2W(strJson,CP_UTF8).c_str());

		switch (nID)
		{
		case CLICK_START_SCAN_ID:
			{
				//! 扫描到的蓝牙以及BLE ID信息 "id": 0 ,"name" : "ai-Thinker","ble_id" : "BluetoothLE#BluetoothLE00:1a:7d:da:71:11-94:c9:60:88:e8:7c"}
				std::string strName = GetNotifStringField(strJson,"name");
				if (0 == JGW_StrComparenoCaseStr(strName.c_str(),"ai-Thinker"))
				{
					std::string strBleId = GetNotifStringField(strJson,"ble_id");
					mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_GetFormatString("BLE Name : %s,ID : %s",strName.c_str(),strBleId.c_str()));
					mpCef3BrowserEventHelp->AddBLEDeviceInfoToMap(strName,strBleId);
				}
				// 扫描完成或者停止事件 {"id": 0 ,"scan_completed" : 1}
				if (1 == GetNotifIntField(strJson,"scan_completed"))
				{
					mpCef3BrowserEventHelp->AddConnectMessageToMap("ble scan completed");
					mpCef3BrowserEventHelp->DisableFalseReScanBleBtnToMap();
				}
			}
			break;
		case STOP_SCAN_ID:
			//mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(JGW_GetFormatString("{\"id\": 2 ,\"name\" : \"ai-Thinker\",\"ble_id\" : \"%s\"}",mstrBleId.c_str()));
			//AddConnectMessageToMap(JGW_GetFormatString("Switch BLE ID : %s",mstrBleId.c_str()));
			break;
		case CONNECT_BLE_ID:
			//! {"id": 2 ,"result" : 1}
			{
				// 连接成功
				// {"id": 2 ,"result" : 1}
				// 连接失败
				// {"id": 2 ,"result" : 0}
				int result = GetNotifIntFieldDef(strJson,"result",-1);
				if (1 == result)
				{
					mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_GetFormatString("Connect BLE result : true"));
				}
				else if (0 == result)
				{
					mpCef3BrowserEventHelp->DisableFalseReScanBleBtnToMap();
				}
			}
			break;
		case CONNECT_GATT_ID:
			//! {"id": 3 ,"gatt_uuid" : 5833ff01-9b8b-5191-6142-22a4536ef123}
			//! {"id":3 ,"gatt_uuid":5833ff01-9b8b-5191-6142-22a4536ef123}
			//! {"id":3 ,"result":1}
			{
				// 连接GATT UUID {"id": 3 ,"uuid" : "55535343-fe7d-4ae5-8fa9-9fafd205e455"}
				std::string strGattUuid = GetNotifStringFieldDef(strJson,"gatt_uuid","");
				//! 检查UUID是否为空
				if (!strGattUuid.empty())
				{
					mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_GetFormatString("gatt uuid : %s",strGattUuid.c_str()));
					// 
					if (0 == JGW_StrComparenoCaseStr(strGattUuid.c_str(),DXCS_GATT_UUID))
					{
						mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket("{\"id\": 3 ,\"uuid\" : \"55535343-fe7d-4ae5-8fa9-9fafd205e455\"}");
						mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_GetFormatString("connect gatt uuid : %s",strGattUuid.c_str()));
					}
				}
				else
				{
					// 连接GATT UUID 结果 {"id": 3 ,"result" : 1}
					if (1 == GetNotifIntField(strJson,"result"))
					{
						mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket("{\"id\":4}");
						mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_GetFormatString("connect gatt uuid result :true"));
					}
					else
					{
						mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_GetFormatString("connect gatt uuid result : false"));
					}
				}
			}
			break;
		case CONNECT_Characteristics_ID:
			// {"id": 4 ,"result" : 1}
			// {"id":4 ,"characteristics_uuid":Custom Characteristic:49535343-1e4d-4bd9-ba61-23c647249616}
			{
				std::string strResult = GetNotifStringFieldDef(strJson,"result","");
				if (strResult.empty())
				{
					mpCef3BrowserEventHelp->AddConnectMessageToMap(GetNotifStringFieldDef(strJson,"characteristics_uuid","unknown characteristics_uuid"));
				}
				else
				{
					mbConnectBLE = 1 == atoi(strResult.c_str());
					mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_GetFormatString("connect characteristics result :%s",mbConnectBLE ? "true" : "false"));
					if (mbConnectBLE) mpCef3BrowserEventHelp->CloseConnectModalToMap();
				}
			}
			break;
		case DXCS_OPERATION_ID:
			OnHandleDxcsOperationMsg(strJson);
			break;
		case DISCONNECT_BLE_ID:
			mbConnectBLE = false;
			mpCef3BrowserEventHelp->OnReConnectDXCSToMap();
			break;
		}
	}

	void CCJGW_PositioningSystemProtocol::OnRecvPositioningSystemProtocolMsg(const std::string& strMessage)
	{
		// 判断ID是否存在
		std::string strID = GetNotifStringField(strMessage,"id");
		if (strID.empty())
		{
			Log4WW_F(L"Invalid ID dxcs message : %s",JGW_A2W(strMessage,CP_UTF8).c_str());
			return ;
		}	

		if (NULL != gpPositioningSystemProtocol)
		{
			gpPositioningSystemProtocol->OnHandlePositioningSystemProtocolMsg(atoi(strID.c_str()),strMessage);
		}
	}

	void CCJGW_PositioningSystemProtocol::OnHandleDxcsOperationMsg(const std::string& strMessage)
	{
		std::string strOperationID = GetNotifStringField(strMessage,"operation_id");
		if (strOperationID.empty())
		{
			Log4WW_F(L"Invalid Operation ID dxcs message : %s",JGW_A2W(strMessage,CP_UTF8).c_str());
			return;
		}

		CCJGW_PositioningSystemMemberVariable *pMemberVariable = &CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable();
		int nOperationID = atoi(strOperationID.c_str());
		switch (nOperationID)
		{
		case GNSS_MODULE_LONGITUDELATITUDE:
			//! {"id":5,"operation_id":33,"longitude":NaN,"latitude":NaN,"magneticDec":NaN}
			{
				pMemberVariable->UpdateLocalGpsLngLat(GetNotifDoubleFieldDef(strMessage,"longitude",INVALID_LONGITUDE),GetNotifDoubleFieldDef(strMessage,"latitude",INVALID_LATITUDE));
			}
			break;
		case GNSS_MODULE_WMM: //! 不使用，只使用自身算法
			break;
		case GNSS_MODULE_LONGITUDELATITUDE_RAW://! 不使用
			//! {"id":5,"operation_id":36,"longitude":113.941775,"latitude":22.5791105,"magneticDec":NaN,"PositionFixIndicator":1,"SatellitesUsed":3,"hdop":4.28,"MslAltitude":183.5 M,"GeoidalSeparation":-2.5}
			{
				pMemberVariable->UpdateLocalGpsLngLat(GetNotifDoubleFieldDef(strMessage,"longitude",INVALID_LONGITUDE),GetNotifDoubleFieldDef(strMessage,"latitude",INVALID_LATITUDE));
				int nPositionFixIndicator = GetNotifIntFieldDef(strMessage,"PositionFixIndicator",0);
				int nSatellitesUsed = GetNotifIntFieldDef(strMessage,"SatellitesUsed",0);
				float fhdop = GetNotifFloatFieldDef(strMessage,"PositionFixIndicator",0.00f);
				std::string strMslAltitude = GetNotifStringFieldDef(strMessage,"MslAltitude"," M");
				float fGeoidalSeparation = GetNotifFloatFieldDef(strMessage,"GeoidalSeparation",0.00f);
				pMemberVariable->UpdateGpsDes(nPositionFixIndicator,nSatellitesUsed,fhdop,fGeoidalSeparation,strMslAltitude);
			}
			//{"id":5,"operation_id":36,"longitude":113.9403115,"latitude":22.5803133333333,"magneticDec":NaN,"PositionFixIndicator":0,"SatellitesUsed":0,"hdop":NaN,"MslAltitude":NaN M,"GeoidalSeparation":NaN}
			break;
		case MOTION_MODULE_MFX:
			// {"id":5,"operation_id":65,"yaw":220.7431,"pitch":-0.5523738,"roll":0.1927947,"calibrationed":1}
			{

				if (1 == GetNotifIntField(strMessage,"calibrationed"))
				{
					mpCef3BrowserEventHelp->DismissEcompassNotCalibratePromptToMap();
					pMemberVariable->UpdateLocalYaw(GetNotifFloatFieldDef(strMessage,"yaw",INVALID_YAW_F));
				}
				else
				{
					pMemberVariable->UpdateLocalYaw(INVALID_YAW_F);
					if (mbConnectBLE) mpCef3BrowserEventHelp->ShowEcompassNotCalibratePromptToMap();
				}
			}
			break;
		case MOTION_MODULE_MTL:
			break;
		case MOTION_MODULE_ALTITUDE:
			break;
		}
	}

	const wchar_t* CCJGW_PositioningSystemProtocol::OnClickRecalibrationMagnetic(const wchar_t* szMessage)
	{
		gpPositioningSystemProtocol->mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(GetReCalibrationMagneticUWPCommand());
		return L"";
	}

	const wchar_t* CCJGW_PositioningSystemProtocol::OnSettingCompassAngle(const wchar_t* szMessage)
	{
		float fCompass = GetNotifFloatField(JGW_W2A(szMessage),"compass");
		CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable().ManualUpdateLocalYaw(fCompass);
		return L"";
	}

	const wchar_t* CCJGW_PositioningSystemProtocol::OnManualUpdateLatLng(const wchar_t* szMessage)
	{
		//! {"type":1,"func":"manual_update_latlng","index":0,"latitude":22,"longitude":113}
		int dindex = GetNotifIntField(JGW_W2A(szMessage),"index");
		double dlatitude = GetNotifDoubleField(JGW_W2A(szMessage),"latitude");
		double dlongitude = GetNotifDoubleField(JGW_W2A(szMessage),"longitude");

		if (dlatitude <= 90.0 && dlatitude >= -90.0 && dlongitude >= -180.0 && dlongitude <= 180.0)
		{
			if (0 == dindex) 
			{
				CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable().ManualUpdateLocalLatLng(dlongitude,dlatitude);
			}
			else
			{
				CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable().ManualUpdateRemoteLatLng(dlongitude,dlatitude);
			}


			if (TAG_RMQ_VIEW == CCJGW_PSRabbitMQ::GetPSRabbitMQInstance().GetTagRmq())
			{
				CCJGW_PositioningSystemMemberVariable* pMemberVariable = &CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable();
				pMemberVariable->CalculateVincentyAzimuthPitchAndMagneticDec();

				gpPositioningSystemProtocol->mpCef3BrowserEventHelp->SetLocalYawInfoToMap(pMemberVariable->msLocalYaws.mfCalYaw,pMemberVariable->msLocalYaws.mfYaw,pMemberVariable->msLocalYaws.mfTrueYaw);
				gpPositioningSystemProtocol->mpCef3BrowserEventHelp->SetRemoteYawInfoToMap(pMemberVariable->msRemoteYaws.mfCalYaw,pMemberVariable->msRemoteYaws.mfYaw,pMemberVariable->msRemoteYaws.mfTrueYaw);
				gpPositioningSystemProtocol->mpCef3BrowserEventHelp->SetLocalGpsInfoToMap(pMemberVariable->msLocalGpsLngLat.mdLatitude,pMemberVariable->msLocalGpsLngLat.mdLongitude,pMemberVariable->mfDistance);
				gpPositioningSystemProtocol->mpCef3BrowserEventHelp->SetRemoteGpsInfoToMap(pMemberVariable->msRemoteGpsLngLat.mdLatitude,pMemberVariable->msRemoteGpsLngLat.mdLongitude);
				gpPositioningSystemProtocol->mpCef3BrowserEventHelp->UpdateAmapWebView();
			}	
		}
		else
		{
			if (0 == dindex) 
			{
				CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable().CancelManualUpdateLocalLatLng();
			}
			else
			{
				CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable().CancelManualUpdateRemoteLatLng();
			}
		}
		return L"";
	}

	const wchar_t* CCJGW_PositioningSystemProtocol::OnCancelManualUpdateCompassAngle(const wchar_t* szMessage)
	{
		CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable().CancelManualUpdateLocalYaw();
		//OutputDebugStringW(L"OnCancelManualUpdateCompassAngle : ");
		//OutputDebugStringW(szMessage);
		//OutputDebugStringW(L"\r\n");
		return L"";
	}

	const wchar_t* CCJGW_PositioningSystemProtocol::OnReConnectDxcs(const wchar_t* szMessage)
	{
		gpPositioningSystemProtocol->mbConnectBLE = false;
		//gpPositioningSystemProtocol->mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket("{\"id\":6}");
		gpPositioningSystemProtocol->mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(GetStartScanUWPCommand());
		gpPositioningSystemProtocol->mpCef3BrowserEventHelp->AddConnectMessageToMap(JGW_W2A(L"Start Scan BLE ....",CP_UTF8));
		return L"";
	}

	const wchar_t* CCJGW_PositioningSystemProtocol::OnLoginRabbitMQ(const wchar_t* szMessage)
	{
		//! "{"type":1,"func":"login_room","room_id":123456,"flag":A}"
		std::string strRoomID = GetNotifStringField(JGW_W2A(szMessage),"room_id");
		std::string strRoomFlag = GetNotifStringField(JGW_W2A(szMessage),"flag");

		E_TAG_RMQ eTagRmq = TAG_RMQ_VIEW;
		if (strRoomFlag.length() >= 1)
		{
			if (strRoomFlag[0] == L'A')
			{
				eTagRmq = TAG_RMQ_A;
			}
			else if (strRoomFlag[0] == L'B')
			{
				eTagRmq = TAG_RMQ_B;
			}
		}

		CCJGW_PSRabbitMQ::AddRabbitMQRoomInfo(eTagRmq,strRoomID);
		if (eTagRmq == TAG_RMQ_A || eTagRmq == TAG_RMQ_B)
		{
			CCJGW_PSRabbitMQ::GetPSRabbitMQInstance().StartRabbitMQThread(OnRecvRabbitMQMsg);
		}

		return L"";
	}

	const wchar_t* CCJGW_PositioningSystemProtocol::OnConnectBleDevice(const wchar_t* szMessage)
	{
		//! "{"type":1,"func":"connect_ble","ble_name":"ai-Thinker","ble_id":"BluetoothLE#BluetoothLE00:1a:7d:da:71:11-94:c9:60:88:e8:7c"}"
		std::string strBleName = GetNotifStringField(JGW_W2A(szMessage),"ble_name");
		std::string strBleId = GetNotifStringField(JGW_W2A(szMessage),"ble_id");
		gpPositioningSystemProtocol->mstrBleId = strBleId;

		gpPositioningSystemProtocol->mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(JGW_GetFormatString("{\"id\": 2 ,\"name\" : \"%s\",\"ble_id\" : \"%s\"}",strBleName.c_str(),strBleId.c_str()));

		return L"";
	}

	void CCJGW_PositioningSystemProtocol::run()
	{
		int nCountIndex = 0;
		CCJGW_PositioningSystemMemberVariable* pMemberVariable = &CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable();
		while (mbIsStartThread)
		{
			if (mbConnectBLE)
			{
				if (nCountIndex++ > UPDATE_YAW_TIMERS)
				{
					mpCef3BrowserEventHelp->SetLocalYawInfoToMap(pMemberVariable->msLocalYaws.mfCalYaw,pMemberVariable->msLocalYaws.mfYaw,pMemberVariable->msLocalYaws.mfTrueYaw);
					mpCef3BrowserEventHelp->SetRemoteYawInfoToMap(pMemberVariable->msRemoteYaws.mfCalYaw,pMemberVariable->msRemoteYaws.mfYaw,pMemberVariable->msRemoteYaws.mfTrueYaw);
					mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(GetMfxUWPCommand());
				}
				if (nCountIndex == UPDATE_LNGLAT_TIMERS)
				{
					pMemberVariable->CalculateVincentyAzimuthPitchAndMagneticDec();

					mpCef3BrowserEventHelp->SetLocalGpsInfoToMap(pMemberVariable->msLocalGpsLngLat.mdLatitude,pMemberVariable->msLocalGpsLngLat.mdLongitude,pMemberVariable->mfDistance);
					mpCef3BrowserEventHelp->SetRemoteGpsInfoToMap(pMemberVariable->msRemoteGpsLngLat.mdLatitude,pMemberVariable->msRemoteGpsLngLat.mdLongitude);
					mpCef3BrowserEventHelp->SetLocalGpsDesToMap(pMemberVariable->msLocalGpsDes.mnPositionFixIndicator,pMemberVariable->msLocalGpsDes.mnSatellitesUsed,pMemberVariable->msLocalGpsDes.mfHdop,pMemberVariable->msLocalGpsDes.mstrMslAltitude,pMemberVariable->msLocalGpsDes.mfGeoidalSeparation);
					//mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(GetLongitudeLatitudeUWPCommand());
					mpCef3PositioningSystemUDPScokect->SendMsgToUdpSocket(GetGNSSRawInfoUWPCommand());
				}
				if (UPDATE_RABBITMQ_MSG == nCountIndex)
				{
					if (IsValidLatitudeLongitude(pMemberVariable->msLocalGpsLngLat.mdLatitude,pMemberVariable->msLocalGpsLngLat.mdLongitude))
					{
						CCJGW_PSRabbitMQ::GetPSRabbitMQInstance().SendRabbitMQMessage(GetLongitudeLatitudeRabbitMQCommand());
					}

					if (pMemberVariable->IsValidYaw(pMemberVariable->msLocalYaws.mfYaw)/*INVALID_YAW != pMemberVariable->msLocalYaws.mfYaw*/)
					{
						CCJGW_PSRabbitMQ::GetPSRabbitMQInstance().SendRabbitMQMessage(GetEulerAngleRabbitMQCommand());
					}

					nCountIndex = 0;
				}

				mpCef3BrowserEventHelp->UpdateAmapWebView();
			}
			else
			{
#if 0
				if (UPDATE_RABBITMQ_MSG == nCountIndex++)
				{
					if (TAG_RMQ_VIEW == CCJGW_PSRabbitMQ::GetPSRabbitMQInstance().GetTagRmq())
					{
						pMemberVariable->CalculateVincentyAzimuthPitchAndMagneticDec();

						mpCef3BrowserEventHelp->SetLocalYawInfoToMap(pMemberVariable->msLocalYaws.mfCalYaw,pMemberVariable->msLocalYaws.mfYaw,pMemberVariable->msLocalYaws.mfTrueYaw);
						mpCef3BrowserEventHelp->SetRemoteYawInfoToMap(pMemberVariable->msRemoteYaws.mfCalYaw,pMemberVariable->msRemoteYaws.mfYaw,pMemberVariable->msRemoteYaws.mfTrueYaw);
						mpCef3BrowserEventHelp->SetLocalGpsInfoToMap(pMemberVariable->msLocalGpsLngLat.mdLatitude,pMemberVariable->msLocalGpsLngLat.mdLongitude,pMemberVariable->mfDistance);
						mpCef3BrowserEventHelp->SetRemoteGpsInfoToMap(pMemberVariable->msRemoteGpsLngLat.mdLatitude,pMemberVariable->msRemoteGpsLngLat.mdLongitude);
						mpCef3BrowserEventHelp->UpdateAmapWebView();
					}	
					nCountIndex = 0;
				}
#endif
			}
#if 0
			else
			{
				if (-1 == mnPositioningSystemID)
				{
					OnReConnectDXCSToMap("");
					//gpPositioningSystemProtocol->mnPositioningSystemID = 0;
				}
			}
#endif
			
			Sleep(UPDATE_DELAY_UNIT_MSEC);
		}
	}

	std::string CCJGW_PositioningSystemProtocol::GetStartScanUWPCommand()
	{
		return "{\"id\":0}";
	}

	std::string CCJGW_PositioningSystemProtocol::GetStopScanUWPCommand()
	{
		return "{\"id\":1}";
	}

	std::string CCJGW_PositioningSystemProtocol::GetConnectBleUWPCommand(const std::string& strBleName,const std::string& strBleId)
	{
		return JGW_GetFormatString("{\"id\": 2 ,\"name\" : \"%s\",\"ble_id\" : \"%s\"}",strBleName.c_str(),strBleId.c_str());
	}

	std::string CCJGW_PositioningSystemProtocol::GetLongitudeLatitudeUWPCommand()
	{
		return "{\"id\":5,\"operation_id\":33}";
	}

	std::string CCJGW_PositioningSystemProtocol::GetMfxUWPCommand()
	{
		return "{\"id\":5,\"operation_id\":65}";
	}

	std::string CCJGW_PositioningSystemProtocol::GetGNSSRawInfoUWPCommand()
	{
		return "{\"id\":5,\"operation_id\":36}";
	}

	std::string CCJGW_PositioningSystemProtocol::GetDisConnectBleUWPCommand()
	{
		return "{\"id\":6}";
	}

	std::string CCJGW_PositioningSystemProtocol::GetReCalibrationMagneticUWPCommand()
	{
		return "{\"id\":5,\"operation_id\":69}";
	}

	std::string CCJGW_PositioningSystemProtocol::GetLongitudeLatitudeRabbitMQCommand()
	{
		CCJGW_PositioningSystemMemberVariable *pMemberVariable = &CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable();
		S_LNGLAT sLocalGpsLngLat = {INVALID_LATITUDE,INVALID_LONGITUDE};
		pMemberVariable->GetLocalGpsLngLat(sLocalGpsLngLat);
		//! {"latitude":-0.9092038,"longitude":-2.0494094,"tag":1,"type":0}
		return JGW_GetFormatString("{\"latitude\":%.6lf,\"longitude\":%.6lf,\"tag\":%d,\"type\":%d}",sLocalGpsLngLat.mdLatitude,sLocalGpsLngLat.mdLongitude,(int)CCJGW_PSRabbitMQ::GetTagRmq(),LATITUDELONGITUDE_TYPE);
	}

	std::string CCJGW_PositioningSystemProtocol::GetEulerAngleRabbitMQCommand()
	{
		CCJGW_PositioningSystemMemberVariable *pMemberVariable = &CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable();
		S_YAW sLocalYaws = {INVALID_YAW_F,INVALID_YAW_F,INVALID_YAW_F};
		pMemberVariable->GetLocalYaws(sLocalYaws);
		//! {"pitch":-0.90287554,"roll":-2.0348217,"tag":1,"type":2,"yaw":152.30745}
		return JGW_GetFormatString("{\"pitch\":%.6f,\"roll\":%.6f,\"yaw\":%.6f,\"tag\":%d,\"type\":%d}",90.0,0.0,sLocalYaws.mfYaw,(int)CCJGW_PSRabbitMQ::GetTagRmq(),EULERANGLE_TYPE);
	}
}

