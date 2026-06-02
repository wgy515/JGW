#include "StdAfx.h"
#include "CJGW_ThroughputTestConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <memory>
namespace JGW
{
    CCJGW_ThroughputTestConfig::CCJGW_ThroughputTestConfig(S_ThroughputTest_Confige& sThroughputConfig) : msThroughputConfig(sThroughputConfig)
    {
    }


    CCJGW_ThroughputTestConfig::~CCJGW_ThroughputTestConfig(void)
    {
    }

    bool CCJGW_ThroughputTestConfig::LoadThroughputTestXMLConfig(const std::string& strXMLConfig)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(strXMLConfig))
        {
            return false;
        }
        xmlRoot = ptrXmlDoc->RootElement();
        if (!xmlRoot) return false;
        LoadThroughputNode(xmlRoot);

        return true;
    }

    void CCJGW_ThroughputTestConfig::LoadThroughputNode(TiXmlElement* pThroughputNode)
    {
        std::wstring strTemp;

        GetElementTextValue(pThroughputNode,"SocketTerimalType",strTemp,L"0");
        msThroughputConfig.meSocketTerimalType = (E_SOCKET_TERIMALTYPE)_ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"IPAddress",msThroughputConfig.mstrIPAddress,L"0.0.0.0");
        GetElementTextValue(pThroughputNode,"SocketPort",strTemp,L"5555");
        msThroughputConfig.mnSocketPort = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"TranservicesType",strTemp,L"1");
        msThroughputConfig.meTranservicesType = (E_Transceivers_TYPE)_ttoi(strTemp.c_str());

        GetElementTextValue(pThroughputNode,"EnableTurnTable",strTemp,L"1");
        msThroughputConfig.mbEnableTurnTable = _ttoi(strTemp.c_str())?true:false;
        GetElementTextValue(pThroughputNode,"TurnTableStep",strTemp,L"2");
        msThroughputConfig.mTurnTableStep = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"Velocity",strTemp,L"5");
        msThroughputConfig.mfVelocity = _ttof(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"Acceleration",strTemp,L"100");
        msThroughputConfig.mfAcceleration = _ttof(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"Deceleration",strTemp,L"100");
        msThroughputConfig.mfDeceleration = _ttof(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"ReadDonglesSN",strTemp,L"0");
        msThroughputConfig.mbReadDonglesSN = _ttoi(strTemp.c_str())?true:false;

        GetElementTextValue(pThroughputNode,"TestTime",strTemp,L"100");
        msThroughputConfig.mnTestTimes = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"SocketCount",strTemp,L"100");
        msThroughputConfig.mnSocketCount = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"DataLength",strTemp,L"5000");
        msThroughputConfig.mnDataLength = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"IntervTime",strTemp,L"1");
        msThroughputConfig.mnIntervTime = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"SendPolicy",strTemp,L"0");
        msThroughputConfig.meSendPolicy = (EnSendPolicy)_ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"ServerThreadCount",strTemp,L"0");
        msThroughputConfig.mnServerThreadCount = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"ServerMaxConnSocketCount",strTemp,L"10000");
        msThroughputConfig.mnServerMaxConnSocketCount = _ttoi(strTemp.c_str());
        GetElementTextValue(pThroughputNode,"HtmlFile",msThroughputConfig.mstrHtmlFile,L"html/current_echarts_qt.html");
    }

    bool CCJGW_ThroughputTestConfig::SaveThroughputTestXMLConfig(const std::string& strXMLConfig)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot = NULL;

        if (!ptrXmlDoc->LoadFile(strXMLConfig.c_str())) return false;
        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return false;

        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.meSocketTerimalType);
        SetElementTextValue(pRoot,"SocketTerimalType",strTemp);
        SetElementTextValue(pRoot,"IPAddress",msThroughputConfig.mstrIPAddress);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mnSocketPort);
        SetElementTextValue(pRoot,"SocketPort",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.meTranservicesType);
        SetElementTextValue(pRoot,"TranservicesType",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mnTestTimes);
        SetElementTextValue(pRoot,"TestTime",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mnSocketCount);
        SetElementTextValue(pRoot,"SocketCount",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mnDataLength);
        SetElementTextValue(pRoot,"DataLength",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mnIntervTime);
        SetElementTextValue(pRoot,"IntervTime",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.meSendPolicy);
        SetElementTextValue(pRoot,"SendPolicy",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mnServerThreadCount);
        SetElementTextValue(pRoot,"ServerThreadCount",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mnServerMaxConnSocketCount);
        SetElementTextValue(pRoot,"ServerMaxConnSocketCount",strTemp);
        SetElementTextValue(pRoot,"HtmlFile",msThroughputConfig.mstrHtmlFile);
        //! turntable 
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mbEnableTurnTable?1:0);
        SetElementTextValue(pRoot,"EnableTurnTable",strTemp);
        JGW_FormatWString(strTemp,L"%d",msThroughputConfig.mTurnTableStep);
        SetElementTextValue(pRoot,"TurnTableStep",strTemp);
        JGW_FormatWString(strTemp,L"%.0f",msThroughputConfig.mfVelocity);
        SetElementTextValue(pRoot,"Velocity",strTemp);
        JGW_FormatWString(strTemp,L"%.0f",msThroughputConfig.mfAcceleration);
        SetElementTextValue(pRoot,"Acceleration",strTemp);
        JGW_FormatWString(strTemp,L"%.0f",msThroughputConfig.mfDeceleration);
        SetElementTextValue(pRoot,"Deceleration",strTemp);

        ptrXmlDoc->SaveFile(strXMLConfig);

        return true;
    }
}