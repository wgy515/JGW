#pragma once
#include <string>
namespace JGW
{
    class ITSEControlCmdForward
    {
    public: 
        virtual void OnForwardRecvControlCmdInfo(const std::string& strData) = 0;
    };
}