#include "StdAfx.h"
#include "TSE_WriteEth0MacConfig.h"
#include <fstream>
#include <JGW_FoundationFunc/JGW_FilePath.h>

namespace JGW
{
    CTSE_WriteEth0MacConfig::CTSE_WriteEth0MacConfig(void) : mstrSrcEth0MacConfigPath("linuxpg_123\\8168HEF.src.cfg"),mnEnvironmentSize(12),mstrDestEth0MacConfigPath("linuxpg_123\\8168HEF.cfg")
    {
    }


    CTSE_WriteEth0MacConfig::~CTSE_WriteEth0MacConfig(void)
    {
    }

    const wchar_t* CTSE_WriteEth0MacConfig::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WriteEth0MacConfig\",\
                \"TestName\": \"写以太网MAC到配置中\",\
                \"SrcConfigPath\": \"原配置文件路径(模板)\",\
                \"DestConfigPath\": \"写入到指定配置文件中\",\
                \"Environment\": \"MAC环境变量名\",\
                \"EnvironmentLen\": \"MAC限定长度，一般为12\"\
                }";
    }

    bool CTSE_WriteEth0MacConfig::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Environment"))
        {
            mstrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SrcConfigPath"))
        {
            mstrSrcEth0MacConfigPath = JGW_W2A(JGW_RealativePathToAbsPath(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"DestConfigPath"))
        {
            mstrDestEth0MacConfigPath = JGW_W2A(JGW_RealativePathToAbsPath(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamValue,L"EnvironmentLen"))
        {
            mnEnvironmentSize = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WriteEth0MacConfig::TSE_Init()
    {
        size_t file_size = 0;
        mstrSrcEth0MacConfigPath = JGW_W2A(JGW_RealativePathToAbsPath(JGW_A2W(mstrSrcEth0MacConfigPath).c_str()));
        mstrDestEth0MacConfigPath = JGW_W2A(JGW_RealativePathToAbsPath(JGW_A2W(mstrDestEth0MacConfigPath).c_str()));
        const char* szFileBuf = JGW_GetFileBufToPath_C(mstrSrcEth0MacConfigPath.c_str(),file_size);
        if (szFileBuf) { mstrConfigFileBuf = szFileBuf; free((void*)szFileBuf); return true; }
        return false;
    }

    bool CTSE_WriteEth0MacConfig::TSE_Run()
    {
        std::wstring strMacEnvironment = GetGlobalEnvironment()->GetString(mstrEnvironment);

        if (mnEnvironmentSize != strMacEnvironment.length())
        { 
            LogE_F(L"输入的eth0 Mac长度不等于%d\n",mnEnvironmentSize);
            return false;
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strMacEnvironment.c_str());
        for (int i = 2;i < 15;i += 3)
        {
            strMacEnvironment.insert(i,L" ");
        }

        if (mstrConfigFileBuf.empty())
        {
            LogE_F(L"读取eth0 Mac配置文件失败，请检查文件是否存在(%s)\n",JGW_A2W(mstrSrcEth0MacConfigPath).c_str());
            return false;
        }

        int startPos =  mstrConfigFileBuf.find("NODEID");
        if (std::string::npos == startPos)
        {
            LogE_F(L"未读取eth0 Mac节点，请检查NODEID节点是否存在\n",JGW_A2W(mstrSrcEth0MacConfigPath).c_str());
            return false;
        }
        int endPos = mstrConfigFileBuf.find("\n",startPos);


        std::string strMac;
        JGW_FormatString(strMac,"NODEID = %s",JGW_W2A(strMacEnvironment).c_str());

        std::string strFileBuf = mstrConfigFileBuf.substr(0,startPos);
        strFileBuf += strMac;
        strFileBuf += mstrConfigFileBuf.substr(endPos);

        std::ofstream fileStream;
        fileStream.imbue( std::locale("") );
        fileStream.open(mstrDestEth0MacConfigPath,std::ios_base::out | std::ios_base::trunc );
        if (!fileStream.is_open())
        {
            LogE_F("保存eth0 Mac配置文件失败\n");
            return false;
        }
        fileStream << strFileBuf;
        fileStream.flush();
        fileStream.close();

        return true;
    }
}