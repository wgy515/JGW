// JGWInstrumentTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <iostream>
/*#include <JGW_NIGPIBControllImpl/equipcommtl_tcp_ip.h>*/
#include <JGW_NIGPIBControllImpl/equipcommtl_VISA.h>
int _tmain(int argc, _TCHAR* argv[])
{
    std::string ip_address = "TCPIP0::CMW50050-134371::inst0::INSTR";
    int inst_number = 1;
    CEquipCommTL_VISA visa(ip_address);

    std::string cmd = "*IDN?";
    std::string rec;
    visa.query(cmd,rec);
    cmd = "ROUTe:GPRF:GENerator:SCENario:SALone RFAC,TX1";
    visa.write(cmd);
    cmd = "SOURce:GPRF:GEN:RFSettings:EATTenuation 1.0";
    visa.write(cmd);

    while (1)
    {
        getline(std::cin,cmd);
        if (std::string::npos != cmd.find("QUIT") || std::string::npos != cmd.find("quit") || std::string::npos != cmd.find("exit") || std::string::npos != cmd.find("EXIT")) break;
        //sp.ReadRespCommandToWriteCommand(strOut.c_str(),strRead);
        cmd += ";*OPC?;:SYST:ERR?;";
        visa.query(cmd,rec);
      
        std::cout << rec << std::endl;
        std::cin.clear();
    }


	return 0;
}

