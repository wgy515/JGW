// JGWLitePointPowerTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <JGW_NIGPIBControllImpl/equipcommtl_VISA.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_tcp_ip.h>
#include <Windows.h>
IEquipCommTL* pEquipMent = NULL;
std::string ip_address = "192.168.133.111";
int inst_number = 0;
std::string visaAddress = "TCPIP::192.168.100.254::24000::SOCKET";
std::fstream fileStream;

void exit_func()
{
    if (fileStream.is_open())
    {
        fileStream.flush();
        fileStream.close();
    }
    if (pEquipMent)
    {
        delete pEquipMent;
        pEquipMent = NULL;
    }
}
int _tmain(int argc, _TCHAR* argv[])
{
    atexit(exit_func);
    pEquipMent = new CEquipCommTL_VISA(visaAddress);
    std::string strCommand,strRetVal;
#if 1
    strCommand = "*RST";
    pEquipMent->write(strCommand);
    strCommand = "*IDN?;";
    pEquipMent->query(strCommand,strRetVal);
    strCommand = "*CLS";
    pEquipMent->write(strCommand);
    std::cout << strRetVal << std::endl;
    strCommand = "FORM:READ:DATA ASC;";
    pEquipMent->write(strCommand);
    strCommand = "*WAI; ERR:ALL?";
    pEquipMent->query(strCommand,strRetVal);
    std::cout << strRetVal << std::endl;

    strCommand = "ROUT1;";
    pEquipMent->write(strCommand);
    strCommand = "PORT:RES     RF1, VSA1;";
    pEquipMent->write(strCommand);
//     strCommand = "PORT:RES:ILO RF1;";
//     pEquipMent->write(strCommand);
//     strCommand = "*WAI; ERR:ALL?";
//     pEquipMent->query(strCommand,strRetVal);
//     std::cout << strRetVal << std::endl;

    strCommand = "VSG1;";
    pEquipMent->write(strCommand);
    strCommand = "FREQ 60.48E9;";
    pEquipMent->write(strCommand);
    strCommand = "VSA1;RLEVel:AUTO;";
    pEquipMent->write(strCommand);
    strCommand = "CAPT:TIME 0.000300;";
    pEquipMent->write(strCommand);
    strCommand = "INIT;";
    pEquipMent->write(strCommand);
    strCommand = "*WAI; ERR:ALL?";
    pEquipMent->query(strCommand,strRetVal);
    std::cout << strRetVal << std::endl;
//     strCommand = "VSA1;";
//     pEquipMent->write(strCommand);



    strCommand = "GPRF;";
    pEquipMent->write(strCommand);
    strCommand = "CONF:pow:phas:spec:ILEN 0.0003";
    pEquipMent->write(strCommand);
    strCommand = "CONF:pow:phas:spec:MLEN 0.0003";
    pEquipMent->write(strCommand);
    strCommand = "CALC:POW 0, 300e-6;";
    pEquipMent->write(strCommand);

    
//     strCommand = "INIT;";
//     pEquipMent->write(strCommand);
// 
//     strCommand = "GPRF;";
//     pEquipMent->write(strCommand);
//     strCommand = "CALC:POW 0, 50e-6;";
//     pEquipMent->write(strCommand);
//     strCommand = "CALC:SPEC 0, 200e-6;";
//     pEquipMent->write(strCommand);

/*#else*/
    fileStream.imbue( std::locale("chs") );
    fileStream.open( "power.log",std::ios_base::out | std::ios_base::trunc );
    if (!fileStream.is_open()) return -1;

//     while (1)
//     {
//         strCommand = "GPRF;";
//         pEquipMent->write(strCommand);

        strCommand = "FETC:POW?";
        pEquipMent->query(strCommand,strRetVal);
        std::cout << strRetVal << std::endl;
        if (!strRetVal.empty() &&'0' == strRetVal.at(0)) 
        {
            const char* pTemp = strRetVal.c_str();
            pTemp += 2;
            fileStream.precision(6);
            fileStream << std::fixed << atof(pTemp)/* << std::endl*/;
        }
        else
        {
            fileStream << "-99999" /*<< std::endl*/;
        }
        strCommand = "*WAI; ERR:ALL?";
        pEquipMent->query(strCommand,strRetVal);
        std::cout << strRetVal << std::endl;
         Sleep(100);
/*     }*/
    
#endif
    delete pEquipMent;
    pEquipMent = NULL;
    fileStream.flush();
    fileStream.close();
	return 0;
}

