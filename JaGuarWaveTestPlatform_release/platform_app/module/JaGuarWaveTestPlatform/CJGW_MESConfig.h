#pragma once
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    class CCJGW_MESConfig
    {
    public:
        CCJGW_MESConfig(void);
        ~CCJGW_MESConfig(void);
        //! 加载MES INI配置文件
        bool LoadMESIniConfig(const std::wstring& strMesConfigPath,LPS_MES_INFO psMesInfo);
        //! 加载MES 工序相关信息
        bool LoadMESWorkStageXmlConfig(const std::wstring& strMesXmlConfigPath,LPS_MES_INFO psMesInfo);
        static bool SaveMESIniConfig(LPS_MES_INFO psMesInfo);
    private:
        void LoadFTPConfig(CCJGW_ConfigIni& configIni,LPS_MES_INFO psMesInfo);
    public:
        //! 密码加密
        static std::string EncryptPassword(const std::string& strPassword);
        //! 密码解密
        static std::string DecryptPassword(const std::string& strEncryptPassword);
    };
}