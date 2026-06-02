#include "StdAfx.h"
#include "CJGW_MESGetSNInfoResponseJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>


namespace JGW
{
	CCJGW_MESGetSNInfoResponseJson::CCJGW_MESGetSNInfoResponseJson(void) : mstrMONOType("MONOSMT")
	{
	}


	CCJGW_MESGetSNInfoResponseJson::~CCJGW_MESGetSNInfoResponseJson(void)
	{
	}

	bool CCJGW_MESGetSNInfoResponseJson::FromJosn(CCJGW_FTM_Json& msgJson)
	{
		CCJGW_MESResponseBaseJson::FromJosn(msgJson);
        //! MONOPack SONO
        std::vector<std::string> vstrParamNames, vstrParamValues;
        msgJson.GetJsonValueToKey("paraName",vstrParamNames);
        msgJson.GetJsonValueToKey("paraValue",vstrParamValues);
#if 0
        if (vstrParamNames.size() != vstrParamValues.size())
        {
            bStatus = false;
            return false;
        }
#else
		size_t uParamLen = vstrParamNames.size() > vstrParamValues.size() ? vstrParamValues.size() : vstrParamNames.size();
#endif

        for (size_t y = 0;y < uParamLen;y ++)
        {
            if (NULL == JGW_StrComparenoCaseStr(mstrMONOType.c_str(),vstrParamNames[y].c_str()))
            {
                mstrWorkOrderNumber = vstrParamValues[y];
                continue;
            }
            if (NULL == JGW_StrComparenoCaseStr("SONO",vstrParamNames[y].c_str()))
            {
                mstrTaskNumber = vstrParamValues[y];
            }
        }
        //! {"paraName":["IMEI_M","IMEI_S","SN","MAC","BT","SpecialSN","OTPpwd","RoutingStep","RoutingStep_Pre","RoutingStep_Next","imeimo","MONOSMT","MONOAssy","MONOPack","SONO"],"paraValue":["","","","","","","","","","","","4520077455-10","4520077487-10","","DHW205221"],"bStatus":true,"responseType":5,"returnMessage":"MES_ErrCode1:此批号［LAUU2045000306］所在站点与当前操作站点一致,\r\n校验正确!"}
		//JGW_ParserStrA(paramval.c_str(),",",vParamVal);
		//if (vParamVal.size() >= 12) mstrWorkOrderNumber = vParamVal[11];
		//if (vParamVal.size() >= 15) mstrTaskNumber = vParamVal[14];
		return true;
	}
}