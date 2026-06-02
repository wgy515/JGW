#pragma once
#include <tinyxml/tinyxml.h>
#include <JGW_Device_WCNTester_CommonSCPIWLAN/WCNTester_CommonSCPIWLAN.h>

namespace JGW
{
    class CCStationCalDataFile
    {
    public:
        CCStationCalDataFile(void);
        ~CCStationCalDataFile(void);

    public:
        bool LoadStationCalDataFile(const std::string& strFilePath,std::vector<S_STATION_CAL_PATH>& vsStationCalPath);
    private:
        bool PareseAllPathParamNode(const TiXmlElement* xmlRoot,std::vector<S_STATION_CAL_PATH>& vsStationCalPath);
    };
}


