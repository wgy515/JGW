#pragma once
#include <string>
#include <JGW_CurlPlugin/CJGW_CurlHttp.h>
#define MES_WEB_HELP_CONFIG_NAME "all_MES.ini"
namespace JGW
{
	// {"MONO":"4520094692-10","SONO":"DHW247105","ProductCode":"F205-JW-6151B01","ProductName":"JW-PTP6151-MBOM-JW-无logo 白色","SOProjectName":"C300-JW-CN-CBU-PR","ProjectName":"C300","SN":"LEQ22425001108","CODE":0,"MSG":"成功"}
	typedef struct  
	{
		std::string mstrMono;
		std::string mstrSono;
		std::string mstrProductCode;
		std::string mstrProductName;
		std::string mstrSOProjectName;
		std::string mstrProjectName;
		std::string mstrSn;
	}S_MoInfo,*LPS_MoInfo;


	class CMesWebHelp
	{
	public:
		CMesWebHelp(void);
		~CMesWebHelp(void);
	private:
		// 加载MES配置文件
		void LoadMesWebHelpConfig();
		// 获取电脑名称、硬件ID ，网卡MAC
		void GetMesWebHelpWorkingUnitHardwareID();
	public:
		bool TestConnect();
		// 登录MES WEB
		bool LoginMesWeb(const std::string& strAccount,const std::string& strPassword);
		// 生产前校验SN
		bool LotCheckMesWeb(const std::string& strSerialNumber);
		// 上传生产数据 记录测试时间
		// 设备完成作业后提交处理结果，并上传相关日志和图片。
		/// @brief 上传生产数据 记录测试时间
		/// @param bTestPass 测试状态
		/// @param strEndTimer 作业结束时间，格式:yyyyMMddHHmmss
		/// @param strSerialNumber 产品序号，规则同 SN 校验接口
		/// @param strComment 备注，1.拼板检测工序，如 AOI，SPI 等，有多个 SN，使用此参数返回每个 SN 的作业结果，示例：SN1:PASS,SN2:SKIP,SN3:UNTEST,SN4:FAIL；
		/// @param strTestName 作业名，当前作业功能名称
		/// @param strFixtureNo 治具编号，作业使用的治具编号，治具需要在系统中注册，没有则使用固定值 NA
		/// @param strCableNo RF 线编号，作业使用的接口线材编号，没有则使用固定值 NA
		/// @param strInstrumentNo 仪器的资产编号或仪器编号，如果涉及多个仪器，使用逗号分开；编号需要在系统中注册；没有则使用固定值 NA
		/// @param strVersion 软件版本
		/// @param strPlatForm 平台，没有则使用固定值 NA
		/// @param strDefectCode 设备定义不良代码，如果有多个不良，这里传主要或第一个不良的代码
		/// @param strDefectDesc 不良描述，如果有多个不良，这里传主要或第一个不良的描述，例如：C21:少锡
		/// @param strDecision 人工判断结果，可选参数：REPAIR 人工判断不良，转维修RETEST 可以重测，不进入不良
		/// @return 
		bool SaveTestRecordMesWeb(const std::string& strStartTimer,const std::string& strEndTimer,const std::string& strTestStatus,const std::string& strSerialNumber,const std::string& strComment = "",const std::string& strTestName = "",const std::string& strFixtureNo = "",const std::string& strCableNo = "",const std::string& strInstrumentNo = "",const std::string& strVersion = "",const std::string& strPlatForm = "",const std::string& strDefectCode = "",const std::string& strDefectDesc = "",const std::string& strDecision = "");
		//bool SaveTestRecordMesWeb(bool bIsStartTimer,bool bTestPass,const std::string& strSerialNumber,const std::string& strComment = "",const std::string& strTestName = "",const std::string& strFixtureNo = "",const std::string& strCableNo = "",const std::string& strInstrumentNo = "",const std::string& strVersion = "",const std::string& strPlatForm = "",const std::string& strDefectCode = "",const std::string& strDefectDesc = "",const std::string& strDecision = "");
		//bool SaveTestRecordMesWeb(bool bTestPass,const std::string& strStartTimer,const std::string& strSerialNumber,const std::string& strComment = "",const std::string& strTestName = "",const std::string& strFixtureNo = "",const std::string& strCableNo = "",const std::string& strInstrumentNo = "",const std::string& strVersion = "",const std::string& strPlatForm = "",const std::string& strDefectCode = "",const std::string& strDefectDesc = "",const std::string& strDecision = "");
		// 完成生产(SN 过站) 作业完成后，需将产品序号过站。班次, 此参数可选，取值如下白班: 310001夜班: 310002
		bool LotNodeNextMesWeb(const std::string& strSerialNumber,const std::string& strShiftID);
		// 校验当前作业的 IMEI/整机 SN 是否合法，和工单是否匹配以及是否允许当前工序作业，当接口返回成功时，才能执行操作
		bool IMEICheckMesWeb(const std::string& strImei);
		// 作业完成后，需将 IMEI 过站
		bool IMEINextMesWeb(const std::string& strImei,const std::string& strPcbSN = "",const std::string& strImeiInfo = "",const std::string& strComment = "");
		// 通用URL
		bool GeneralUrlMesWeb(const std::string& strGeneralUrl,bool bSaveCookieFile = false,bool bUseCookieFile = true);
		// 根据产品 SN(PCBSN,整机 SN,IMEI 等)获取关联的工单、订单相关信息
		// 产品 SN(PCBSN,整机 SN,IMEI 等)
		// SN 类型，可选参数，IMEI、SN、PCBSN、BIND
		/*
		{"MONO":"4520094692-10","SONO":"DHW247105","ProductCode":"F205-JW-6151B01","ProductName":"JW-PTP6151-MBOM-JW-无logo 白色","SOProjectName":"C300-JW-CN-CBU-PR","ProjectName":"C300","SN":"LEQ22425001108","CODE":0,"MSG":"成功"}
		*/
		bool MoInfoBySNMesWeb(const std::string& strSerialNumber,LPS_MoInfo psMoInf,const std::string& strSNType = "SN");
		// 根据工单号获取关联的工单、订单、项目、软件等信息
		bool MoInfoMesWeb(std::string& strResponceLocal,const std::string& strMONO/*,std::string& strSono*/);
		// 根据成品 SN、IMEI、MAC 或主板 SN 获取产品信息
		// strProductSN : 产 品 SN, 根 据 产 品 性 质 可 能 是 IMEI/ 成 品SN/WIFI/BT 等号码
		// RefSN 关联 SN, 一般为 PCBASN
		// ProductType PCBASN/IMEI（PCBASN：主板，IMEI：整机）；可选参数，默认值为 IMEI
		bool GetProductInfoBySNMesWeb(std::string& strResponceLocal,const std::string& strProductSN,const std::string& strRefSN,const std::string& strProductType = "IMEI");
		//登出MES WEB
		bool LogoutMesWeb();
	public:
		void SetOperationID(const std::string& strOperationID);
		void SetMono(const std::string& strMono);
		const std::string& GetErrorMessage();
	private:
		// http:////api.example.com:8085/messervice/api/login?account=806003&password=123456&WorkingUnitID=PCName&HardwareID=E8982F09&Ver=1.3&ClientType=AOI&OperationId=AOI
		std::string GetLogiMesWebUrl();
		// http:////api.example.com:8085/messervice/api/LotCheck?SN=LM141546000006&MONO=dhw131492-060&OperationID=001
		std::string GetLotCheckMesWebUrl(const std::string& strSerialNumber);
		// http://api.example.com:8085/messervice/api/logout
		std::string GetLogoutMesWebUrl();
		// /messervice/api/SaveTestRecord
		// strComment
		/*
		1.SN1:PASS,SN2:SKIP,SN3:UNTEST,SN4:FAIL；
		2.镭雕或打印等有模板文件的工序，需回传模
		板文件名称；示例：
		{"TemplateFile":"P325A20231108.lab"}
		3.以JSON字符串上传不良单板SN及其位号信
		息：[
		{
		"LotSN" : "111395547K006988",
		"PositionCode" : "u01"
		},... ... ]*/
		/// @brief 
		/// @param bTestPass 测试状态
		/// @param strEndTimer 作业结束时间，格式:yyyyMMddHHmmss
		/// @param strSerialNumber 产品序号，规则同 SN 校验接口
		/// @param strComment 备注，1.拼板检测工序，如 AOI，SPI 等，有多个 SN，使用此参数返回每个 SN 的作业结果，示例：SN1:PASS,SN2:SKIP,SN3:UNTEST,SN4:FAIL；
		/// @param strTestName 作业名，当前作业功能名称
		/// @param strFixtureNo 治具编号，作业使用的治具编号，治具需要在系统中注册，没有则使用固定值 NA
		/// @param strCableNo RF 线编号，作业使用的接口线材编号，没有则使用固定值 NA
		/// @param strInstrumentNo 仪器的资产编号或仪器编号，如果涉及多个仪器，使用逗号分开；编号需要在系统中注册；没有则使用固定值 NA
		/// @param strVersion 软件版本
		/// @param strPlatForm 平台，没有则使用固定值 NA
		/// @param strDefectCode 设备定义不良代码，如果有多个不良，这里传主要或第一个不良的代码
		/// @param strDefectDesc 不良描述，如果有多个不良，这里传主要或第一个不良的描述，例如：C21:少锡
		/// @param strDecision 人工判断结果，可选参数：REPAIR 人工判断不良，转维修RETEST 可以重测，不进入不良
		/// @return 
		//std::string GetSaveTestRecordMesWebUrl(bool bTestPass,const std::string& strEndTimer,const std::string& strSerialNumber,const std::string& strComment = "",const std::string& strTestName = "",const std::string& strFixtureNo = "",const std::string& strCableNo = "",const std::string& strInstrumentNo = "",const std::string& strVersion = "",const std::string& strPlatForm = "",const std::string& strDefectCode = "",const std::string& strDefectDesc = "",const std::string& strDecision = "");
		//std::string GetSaveTestRecordMesWebUrl(const std::string& strStartTimer,bool bTestPass,const std::string& strEndTimer,const std::string& strSerialNumber,const std::string& strComment = "",const std::string& strTestName = "",const std::string& strFixtureNo = "",const std::string& strCableNo = "",const std::string& strInstrumentNo = "",const std::string& strVersion = "",const std::string& strPlatForm = "",const std::string& strDefectCode = "",const std::string& strDefectDesc = "",const std::string& strDecision = "");
		std::string GetSaveTestRecordMesWebUrl(const std::string& strStartTimer,const std::string& strEndTimer,const std::string& strTestStatus,const std::string& strSerialNumber,const std::string& strComment = "",const std::string& strTestName = "",const std::string& strFixtureNo = "",const std::string& strCableNo = "",const std::string& strInstrumentNo = "",const std::string& strVersion = "",const std::string& strPlatForm = "",const std::string& strDefectCode = "",const std::string& strDefectDesc = "",const std::string& strDecision = "");
		// http:////api.example.com:8085/messervice/api/LotNodeNext?SN=LM141546000006&MONO=dhw131492-060&OperationID=001&ShiftID=310001
		std::string GetLotNodeNextMesWebUrl(const std::string& strSerialNumber,const std::string& strShiftID);
		// URL: /messervice/api/IMEICheck
		std::string GetIMEICheckMesWebUrl(const std::string& strImei);
		// URL: /messervice/api/IMEINext
		std::string GetIMEINextMesWebUrl(const std::string& strImei,const std::string& strPcbSN = "",const std::string& strImeiInfo = "",const std::string& strComment = "");
		// URL: /messervice/api/MoInfoBySN
		std::string GetMoInfoBySNMesWebUrl(const std::string& strSerialNumber,const std::string& strSNType = "SN");
		// URL: /messervice/api/MoInfo
		std::string GetMoInfoMesWebUrl(const std::string& strMONO);
		// URL: /messervice/api/ProductInfo
		std::string GetProductInfoBySNMesWebUrl(const std::string& strProductSN,const std::string& strRefSN,const std::string& strProductType = "IMEI");
		// Handle the returned message
		bool HandleHttpResponseMessage(const std::string& strResponceLocal);
	public:
		//std::string mstrResponceUtf8;
	private:
		std::string mstrResponceLocal;
		std::string mstrSono;
		std::string mstrTaskNumber;
		std::string mstrMono; // 工单号（产线输入）
		std::string mstrErrorMessage;
		std::string mstrCookieFilePath;
		std::string mstrVer; // 4.0
		std::string mstrClientType; /* 客户端类型，具体类型在设备或工具对接系
									 统时确认，参考取值:
									 AOI 设备：AOI
									 SPI 设备：SPI
									 X-Ray 设备：XRAY
									 镭雕设备：LASER
									 写号设备：SWTOOL
									 内观检设备：IVC
									 */
		std::string mstrHardwareID; // 硬件ID ，网卡MAC
		std::string mstrWorkingUnitID; // 电脑名称
		//yb
		//Mes_URL=http://172.19.98.25:8803/messervice/api
		//jx
		//Mes_URL=http://messervice.jxmaxon.com/messervice/api
		//hy
		//Mes_URL=http://192.168.8.4:8803/messervice/api
		//eng
		//Mes_URL=http://bpmtestb.tinno.com:8083/messervice/api
		//Vietnam
		// Mes_URL= http://172.27.77.32:8808/messervice/api
		std::string mstrMesUrl;
		std::string mstrOperationID; // 作业工序，系统配置或操作员选择输入 如果登录时不确定工序，可以不提供
		std::string mstrAccount;
		std::string mstrPassword;

		CCJGW_CurlHttp mcCurlHttp;
	};
}

