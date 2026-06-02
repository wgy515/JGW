#pragma once
#include <string>

namespace JGW
{
    class CCJGW_ReqSystemInfoJson
    {
    public:
        CCJGW_ReqSystemInfoJson(void);
        ~CCJGW_ReqSystemInfoJson(void);
    public:
        std::string BuildSystemJson();
    public:
        int requestCode;

        int cpu_temp;
        int cpu_freq;
        int radio_temp;
        
        double cpuAll;
        double cpu0;
        double cpu1;
        double cpu2;
        double cpu3;
        ULONGLONG mem_total;
        ULONGLONG mem_used;
        ULONGLONG mem_free;

        std::string mac;
    };
}


