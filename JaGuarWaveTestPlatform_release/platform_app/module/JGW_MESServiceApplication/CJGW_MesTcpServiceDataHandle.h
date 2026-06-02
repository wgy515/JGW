#pragma once
#include <string>
#include "MesWebHelp.h"

namespace JGW
{
	class CCJGW_MesTcpServiceDataHandle
	{
	public:
		CCJGW_MesTcpServiceDataHandle(void);
		~CCJGW_MesTcpServiceDataHandle(void);
	public:
		std::string OnHandleMsg(const std::string& strMsg);
	private:
		// 登录
		std::string OnLoginUser(const std::string& strUserName,const std::string& strUserPassword,const std::string& strOperationId);
		// 测试连接
		std::string OnTestConnect();
		// 获取产品信息 根据成品 SN、IMEI、MAC 或主板 SN 获取产品信息
		std::string OnGetInfoToImei(const std::string& strLotPKSN,const std::string& strPCBSN,const std::string& strMono);
		// 产品 SN 获取工单相关信息
		std::string OnGetInfoToSn(const std::string& strPcbaSN,const std::string& strMono,int nRequestType,const std::string& strSNType = "SN");
		// 获取工单信息
		std::string OnGetInfoToMono(const std::string& strMono,bool bIsSo);
		std::string OnGetTestStatus(const std::string& strLotSn,const std::string& strOperationId);
		std::string OnLogout();
		std::string OnNextSn(const std::string& strLotSn,const std::string& strShiftID);
		std::string OnNextImei(const std::string& strIMEI_M_S_SN,const std::string& strSN,const std::string& strShiftID,const std::string& strSoftWareVarTinno = "",const std::string& strSoftWareVarCustomer = "",const std::string& strRemark = "",const std::string& strUnlockcode = "",const std::string& strImeiInfo = "");
		std::string OnSaveTestRecords(const std::string& sn,
			const std::string& shiftID,
			const std::string& startDate,
			const std::string& endDate,
			const std::string& status,
			const std::string& comment,
			const std::string& type,
			const std::string& testName = "",
			const std::string& fixtureNo = "",
			const std::string& cableNo = "",
			const std::string& instrumentNo = "",
			const std::string& version = "",
			const std::string& platform = "",
			const std::string& failCode = "",
			const std::string& failCodeDesc = "");
	private:
		CMesWebHelp mcMesWebHelp;
	};
}
