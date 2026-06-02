#include "StdAfx.h"
#include "CJGW_MESConfig.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <tinyxml/tinyxml.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/RSAKey.h>

#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

namespace JGW
{
    CCJGW_MESConfig::CCJGW_MESConfig(CTSE_LogNet& logNet) : mLogNet(logNet)
    {
    }


    CCJGW_MESConfig::~CCJGW_MESConfig(void)
    {
    }

    bool CCJGW_MESConfig::LoadMESIniConfig(const std::wstring& strMesConfigPath,LPS_MES_INFO psMesInfo)
    {
        if (!psMesInfo) return false;

        CCJGW_ConfigIni configIni;
        if (!configIni.InitIniFilePath(strMesConfigPath.c_str())) return false;

        psMesInfo->mbAutoUpdate = configIni.GetIniKeyBoolValue(L"Update",L"auto",1);
        psMesInfo->mbIsForceTerminateProcess = configIni.GetIniKeyBoolValue(L"MAIN",L"ForceTerminateProcess",1);
        psMesInfo->msMesConnectInfo.meMESModel = configIni.GetIniKeyIntValue(L"MESMode",L"Online",1)?E_ONLINE_MES:E_NOTONLINE_MES;
        //! CS TSE
        psMesInfo->msCSTSEServerConfig.mbOpenCSTSEServerModule = configIni.GetIniKeyBoolValue(L"CSTSE",L"OpenCSTSEServerModule");
        psMesInfo->msCSTSEServerConfig.mnCSTSEServerSocketPort = configIni.GetIniKeyIntValue(L"CSTSE",L"CSTSEServerSocketPort",3008);
        //! MESClient
        std::string strTemp;
        psMesInfo->msMesConnectInfo.mstrMESServiceIP = configIni.GetIniKeyValueA(L"MESClient",L"MESServiceIP",L"127.0.0.1");
        psMesInfo->msMesConnectInfo.mnMESServiceSocketPort = configIni.GetIniKeyIntValue(L"MESClient",L"MESServiceSocketPort",3000);
        psMesInfo->msMesConnectInfo.mnMESConnectTimeOut = configIni.GetIniKeyIntValue(L"MESClient",L"MESConnectTimeOut",10);
        psMesInfo->msMesConnectInfo.mnMESSendTimeOut = configIni.GetIniKeyIntValue(L"MESClient",L"MESSendTimeOut",10);
        psMesInfo->msMesConnectInfo.mnMESRecvTimeOut = configIni.GetIniKeyIntValue(L"MESClient",L"MESRecvTimeOut",10);
        mLogNet.DebugLogFormat(L"MESServiceIP: %s,MESServiceSocketPort: %d",psMesInfo->msMesConnectInfo.mstrMESServiceIP.c_str(),psMesInfo->msMesConnectInfo.mnMESServiceSocketPort);
		
        psMesInfo->msMesConnectInfo.mstrUsername = configIni.GetIniKeyValueA(L"MESClient",L"UserName",L"101");
        strTemp = configIni.GetIniKeyValueA(L"MESClient",L"UserPassword",L"qUP++vNRLk6QuV6M9/i5aw==");
        psMesInfo->msMesConnectInfo.mstrUserpwd = DecryptPassword(strTemp);
        mLogNet.DebugLogFormat(L"UserName: %s,UserPassword: %s",psMesInfo->msMesConnectInfo.mstrUsername.c_str(),strTemp.c_str());

        psMesInfo->msMesConnectInfo.mbSaveAccount = configIni.GetIniKeyBoolValue(L"MESClient",L"SaveAccount");
        psMesInfo->msMesConnectInfo.mnSaveAccountTime = configIni.GetIniKeyIntValue(L"MESClient",L"SaveAccountTime");
        psMesInfo->msMesConnectInfo.mnSaveAccountTimeStamp = configIni.GetIniKeyIntValue(L"MESClient",L"SaveAccountTimeStamp");
        if (!psMesInfo->msMesConnectInfo.mbSaveAccount) psMesInfo->msMesConnectInfo.mstrUserpwd = "";
        psMesInfo->mstrTaskNumber = configIni.GetIniKeyValueA(L"MESClient",L"TaskNumber",L"");
        psMesInfo->mstrWorkOrderNumber = configIni.GetIniKeyValueA(L"MESClient",L"WorkOrderNumber",L"");
        psMesInfo->mstrShiftID = configIni.GetIniKeyValueA(L"MESClient",L"ShiftID",L"");
        psMesInfo->mstrOperationId = configIni.GetIniKeyValueA(L"MESClient",L"DefaultOperationID",L"-1");
        LoadFTPConfig(configIni,psMesInfo);

        return true;
    }

    void CCJGW_MESConfig::LoadFTPConfig(CCJGW_ConfigIni& configIni,LPS_MES_INFO psMesInfo)
    {
        std::wstring strFTPList = configIni.GetIniKeyValue(L"FTPList",L"List");
        if (strFTPList.empty()) return;

		psMesInfo->mmapFTPConnectInfo.clear();
        std::vector<std::wstring> vstrAppName;
        JGW_ParserStrW(strFTPList.c_str(),L",",vstrAppName);
        S_FTP_CONNECT_INFO sFtpConnectInfo;

        for (std::vector<std::wstring>::iterator it = vstrAppName.begin();
            it != vstrAppName.end();
            ++ it)
        {
            sFtpConnectInfo.mstrHost = configIni.GetIniKeyValueA(it->c_str(),L"Host");
            sFtpConnectInfo.mstrUsername = configIni.GetIniKeyValueA(it->c_str(),L"UserName");
            sFtpConnectInfo.mstrUserpwd = configIni.GetIniKeyValueA(it->c_str(),L"UserPassword");
			sFtpConnectInfo.mstrUserpwd = DecryptPassword(sFtpConnectInfo.mstrUserpwd);
            psMesInfo->mmapFTPConnectInfo[it->c_str()] = sFtpConnectInfo;
        }
    }

    bool CCJGW_MESConfig::SaveMESIniConfig(LPS_MES_INFO psMesInfo)
    {
        if (!psMesInfo || E_MES_OK_STATUS != psMesInfo->msMesConnectInfo.meMESStatus) return false;
        CCJGW_ConfigIni configIni;
        if (!configIni.InitIniFilePath(psMesInfo->msMesConnectInfo.mstrMESConfigFilePath.c_str())) return false;
        configIni.SetIniKeyValue(L"MESClient",L"TaskNumber",psMesInfo->mstrTaskNumber);
        configIni.SetIniKeyValue(L"MESClient",L"WorkOrderNumber",psMesInfo->mstrWorkOrderNumber);
        configIni.SetIniKeyValue(L"MESClient",L"ShiftID",psMesInfo->mstrShiftID);
        configIni.SetIniKeyValue(L"MESClient",L"UserName",JGW_A2W(psMesInfo->msMesConnectInfo.mstrUsername).c_str());

        if (!psMesInfo->msMesConnectInfo.mbSaveAccount)
        {
            configIni.SetIniKeyValue(L"MESClient",L"UserPassword",L"");
            configIni.SetIniKeyValue(L"MESClient",L"SaveAccount",0);
            return true;
        }
        else
        {
            configIni.SetIniKeyValue(L"MESClient",L"UserPassword",JGW_A2W(EncryptPassword(psMesInfo->msMesConnectInfo.mstrUserpwd)).c_str());
            configIni.SetIniKeyValue(L"MESClient",L"SaveAccount",1);
        }
        configIni.SetIniKeyValue(L"MESClient",L"SaveAccountTime",psMesInfo->msMesConnectInfo.mnSaveAccountTime);
        configIni.SetIniKeyValue(L"MESClient",L"SaveAccountTimeStamp",psMesInfo->msMesConnectInfo.mnSaveAccountTimeStamp);

        return true;
    }

    bool CCJGW_MESConfig::LoadMESWorkStageXmlConfig(const std::wstring& strMesXmlConfigPath,LPS_MES_INFO psMesInfo)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot,*pWorkstageNode = NULL;
        psMesInfo->mmapMESWorkStage.clear();

        mLogNet.DebugLogFormat(L"LoadMESWorkStageXmlConfig : %s",strMesXmlConfigPath.c_str());
        if (!ptrXmlDoc->LoadFile(JGW_W2A(strMesXmlConfigPath).c_str())) return false;
  
        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;

        S_MSE_WORKSTAGE sMesWorkStage;
        std::wstring strTemp;
		psMesInfo->mmapMESWorkStage.clear();
        pWorkstageNode = pRoot->FirstChildElement("workstage");
        while (pWorkstageNode)
        {
            GetElementAttributeValue(pWorkstageNode,"id",strTemp,L"");
            GetElementAttributeValue(pWorkstageNode,"name",sMesWorkStage.mstrWorkStageName,L"");
            GetElementAttributeValue(pWorkstageNode,"des",sMesWorkStage.mstrWorkStageDes,L"");
            mLogNet.DebugLogFormat(L"id: %s,name: %s,des: %s",strTemp.c_str(),sMesWorkStage.mstrWorkStageName.c_str(),sMesWorkStage.mstrWorkStageDes.c_str());
            psMesInfo->mmapMESWorkStage[JGW_W2A(strTemp)] = sMesWorkStage;
            pWorkstageNode = pWorkstageNode->NextSiblingElement("workstage");
        }
        return true;
    }

    std::string CCJGW_MESConfig::EncryptPassword(const std::string& strPassword)
    {
        std::string encrypted= "";
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("AES-128-ECB","JaGuarWave","JaGuarWave"));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
            encrypted = pCipher->encryptString(strPassword, Poco::Crypto::Cipher::ENC_BASE64);
        }
        catch (...)
        {
            mLogNet.ErrorLog(GetString(E_PASSWORD_ENCRYPTION_FAILED_TYPE));
        } 
        return encrypted;
    }

    std::string CCJGW_MESConfig::DecryptPassword(const std::string& strEncryptPassword)
    {
        std::string decrypted= "";
        try
        {
            Poco::Crypto::CipherFactory& factory = Poco::Crypto::CipherFactory::defaultFactory();
            // Creates a 256-bit AES cipher
            Poco::Crypto::Cipher* pCipher = factory.createCipher(Poco::Crypto::CipherKey("AES-128-ECB","JaGuarWave","JaGuarWave"));
            Poco::Crypto::Cipher* pRSACipher = factory.createCipher(Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL));
            decrypted = pCipher->decryptString(strEncryptPassword, Poco::Crypto::Cipher::ENC_BASE64);
        }
        catch (...)
        {
            //PELOG4WW(L"√‹¬ÎΩ‚√‹ ß∞‹");
            mLogNet.ErrorLog(GetString(E_PASSWORD_DECRYPTION_FAILED_TYPE));
        } 
        return decrypted;
    }
}