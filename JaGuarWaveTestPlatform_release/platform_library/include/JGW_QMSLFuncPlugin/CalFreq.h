#pragma once
#include "SubSysVFSDefine.h"
#include "JGW_QMSLFuncPlugin_Define.h"
namespace JGW
{
    class JGWQMSLFUNCPLUGIN_CLASS CCalFreq
    {
    public:
        CCalFreq(void);
        ~CCalFreq(void);
        //sBandInfo 输入输出结构体
        static bool	CalFreq(COMMUNICATIONSSYS nsys,SBAND_INFO& sBandInfo);
    private:
        static bool	CalLteFreq(SBAND_INFO& sBandInfo);
        static bool	CalWcdmaFreq(SBAND_INFO& sBandInfo);
        static bool	CalTdscdmaFreq(SBAND_INFO& sBandInfo);
        static bool	CalCdmaFreq(SBAND_INFO& sBandInfo);
        static bool	CalGsmFreq(SBAND_INFO& sBandInfo);
    };
}