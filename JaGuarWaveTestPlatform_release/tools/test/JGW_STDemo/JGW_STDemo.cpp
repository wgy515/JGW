// JGW_STDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SCLLibHelper.h"
#include <string>
#include <unordered_map>

int _tmain(int argc, _TCHAR* argv[])
{
#if 1
    std::unordered_map<int,std::wstring> mmapPropertyConfig;
    mmapPropertyConfig[854545] = L"5555";
    std::wstring& strTemp = mmapPropertyConfig[854545];
    strTemp = L"zhaoqiefsdfjlsfsl";
    mmapPropertyConfig[854545] = L"zhaowc";

    for (int i = 0;i < 854545;i ++)
    {
        //! mmapPropertyConfig[i] = L"44";
        mmapPropertyConfig.insert(std::make_pair(i,L"44"));
    }

    strTemp = L"zhaoqi";
#else
    double velocity = 5.00; //! 周转速度 max 10 default 5 rps
    double acceleration = 100.00; //! 加速度 rps/s
    double deceleration = 100.00; //! 减速度 rps/5
    int nRelDI = 20000;

    int m_nNodeID = 32; //! 默认 1
    SCLLibHelper scl;
 
    if (!scl.Open(2,9600))
    {
        return 0;
    }
    scl.WriteMotorEnable(m_nNodeID);
    scl.RelMove(m_nNodeID, nRelDI, &velocity, &acceleration, &deceleration);


    scl.Close();
#endif
	return 0;
}

