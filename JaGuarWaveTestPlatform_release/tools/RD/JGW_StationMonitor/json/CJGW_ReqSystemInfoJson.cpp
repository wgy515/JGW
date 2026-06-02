#include "StdAfx.h"
#include "CJGW_ReqSystemInfoJson.h"
#include "../CJGW_StationMonitorDefine.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_ReqSystemInfoJson::CCJGW_ReqSystemInfoJson(void) : requestCode(REQ_CODE_SYSTEM_REPORT),cpuAll(0.00),cpu0(0.00),cpu1(0.00),cpu2(0.00),cpu3(0.00),mem_free(0),mem_total(0),mem_used(0),cpu_temp(-1),cpu_freq(-1),radio_temp(-1)
    {
    }


    CCJGW_ReqSystemInfoJson::~CCJGW_ReqSystemInfoJson(void)
    {
    }

    std::string CCJGW_ReqSystemInfoJson::BuildSystemJson()
    {
        std::string strJson;
        JGW_FormatString(strJson,"{\"requestCode\":%d,\"mac\":\"%s\",\"cpuAll\":%.2lf,\"cpu0\":%.2lf,\"cpu1\":%.2lf,\"cpu2\":%.2lf,\"cpu3\":%.2lf,\"mem_total\":%I64u,\"mem_used\":%I64u,\"mem_free\":%I64u,\"cpu_temp\":%d,\"cpu_freq\":%d,\"radio_temp\":%d}",requestCode,mac.c_str(),cpuAll,cpu0,cpu1,cpu2,cpu3,mem_total,mem_used,mem_free,cpu_temp,cpu_freq,radio_temp);
        return strJson;
    }
}