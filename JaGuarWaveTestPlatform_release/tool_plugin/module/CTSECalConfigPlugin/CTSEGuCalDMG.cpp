#include "StdAfx.h"
#include "CTSEGuCalDMG.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <tinyxml/tinyxml.h>
#include <fstream>
#include <algorithm>
#include <numeric>
namespace JGW
{
    CCTSEGuCalDMG::CCTSEGuCalDMG(void)
    {
    }


    CCTSEGuCalDMG::~CCTSEGuCalDMG(void)
    {
    }

    bool CCTSEGuCalDMG::LoadGuCalConfig(const std::wstring& strGuCalConfigPath,PS_TSE_CALCONFIG_TEST_CONFIG_PARAM psCalConfigTestConfigParam)
    {
        CCJGW_ConfigIni configIni;
        std::wstring strTemp;

        configIni.InitIniFilePath(strGuCalConfigPath.c_str());
        strTemp = configIni.GetIniKeyValue(L"DMG",L"dmg_config_path");
        if (strTemp.empty()) return false;

        msDmgCalConfig.mstrCalSuiteXmlFilePath = JGW_RealativePathToAbsPath(strTemp.c_str());
        if (!JGW_FileExistsToFilePath(msDmgCalConfig.mstrCalSuiteXmlFilePath.c_str())) return false;
        LoadTSESuiteConfigEnvironment(msDmgCalConfig.mstrCalSuiteXmlFilePath);

        msDmgCalConfig.meAlgorithm = (E_TSE_GuCal_Algorithm)configIni.GetIniKeyIntValue(L"DMG",L"algorithm");
        msDmgCalConfig.mnLimit = configIni.GetIniKeyIntValue(L"DMG",L"limit",3);
        strTemp = configIni.GetIniKeyValue(L"DMG",L"gu_test_data_file",L"TestLog\\DMG\\dmg_gu_cal.csv");
        msDmgCalConfig.mstrGuTestDataFile = JGW_RealativePathToAbsPath(strTemp.c_str());
        //JGW_DeleteFile(msDmgCalConfig.mstrGuTestDataFile.c_str());
        msDmgCalConfig.mstrDutRxGuTxRssiEnvironment = configIni.GetIniKeyValue(L"DMG",L"DutRxGuTxRssiEnv",L"TSE_TX_RX_RSSI_NORMAL");
        
        msDmgCalConfig.mstrDutTxGuRxRssiEnvironment = configIni.GetIniKeyValue(L"DMG",L"DutTxGuRxRssiEnv",L"TSE_RX_TX_RSSI_NORMAL");

        return true;
    }

    std::wstring CCTSEGuCalDMG::GetDutTestDataFilePath()
    {
        return msDmgCalConfig.mstrGuTestDataFile;
    }

    void CCTSEGuCalDMG::LoadTSESuiteConfigEnvironment(const std::wstring& strCalSuiteXmlFilePath)
    {
        mmapCommonEnvironment.clear();
        mstrGlobalEnvironmentConfigXmlPath = L"";
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* pRoot,*pCommonNode,*pXmlTemp = NULL;
        const char* pszTemp = NULL;
        std::wstring strkey,strvalue;
        if (!ptrXmlDoc->LoadFile(JGW_W2A(strCalSuiteXmlFilePath))) return ;

        pRoot = ptrXmlDoc->RootElement();
        if (!pRoot) return ;

        pCommonNode = pRoot->FirstChildElement("common");
        if (pCommonNode)
        {
            pXmlTemp = pCommonNode->FirstChildElement();
            while (pXmlTemp)
            {
                pszTemp = pXmlTemp->GetText();
                strvalue = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

                pszTemp = pXmlTemp->Value();
                strkey = pszTemp?JGW_A2W_A(pszTemp,CP_UTF8):L"";

                pXmlTemp = pXmlTemp->NextSiblingElement();

                if (0 == JGW_WStrComparenoCaseWStr(L"TSE_GLOBAL_ENVIRONMENT_CONFIG_FILE_PATH",strkey.c_str()))
                {
                    mstrGlobalEnvironmentConfigXmlPath = JGW_RealativePathToAbsPath(strvalue.c_str());
                }
                else
                {
                    mmapCommonEnvironment[strkey] = strvalue;
                } 
            }
        }
    }

    std::wstring CCTSEGuCalDMG::SaveDutCalibrationLossFile(const std::wstring& strDutTestDataFilePath)
    {
        std::wstring strDutTxGuRxRssi,strDutRxGuTxRssi;
        if (!GetTSERssiStandard(strDutTestDataFilePath,strDutTxGuRxRssi,strDutRxGuTxRssi))
        {
            return L"获取RSSI测试标准失败";
        }

        if (mstrGlobalEnvironmentConfigXmlPath.empty())
        {
            return ChangeTSEConfigXml(msDmgCalConfig.mstrCalSuiteXmlFilePath,strDutTxGuRxRssi,strDutRxGuTxRssi) ? L"" : L"修改CONFIG XML失败";
        }

        return ChangeTSEConfigXml(mstrGlobalEnvironmentConfigXmlPath,strDutTxGuRxRssi,strDutRxGuTxRssi) ? L"" : L"修改CONFIG XML失败";
    }

    bool CCTSEGuCalDMG::ChangeTSEConfigXml(const std::wstring& strTSEConfigXmlPath,std::wstring& strDutTxGuRxRssi,std::wstring& strDutRxGuTxRssi)
    {
        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        bool bLoadXmlOk = true;
        TiXmlElement* pRoot,*pCommonNode,*pXmlTemp = NULL;
        if (!ptrXmlDoc->LoadFile(JGW_W2A(strTSEConfigXmlPath)) || !ptrXmlDoc->RootElement())
        {
            JGW_CreateDirectory(strTSEConfigXmlPath.c_str(),false);
            bLoadXmlOk = false;
        }

        if (!bLoadXmlOk)
        {
            TiXmlDeclaration *pDec = new TiXmlDeclaration("1.0", "utf-8", "");
            ptrXmlDoc->LinkEndChild(pDec);
            TiXmlElement* pSuiteNode = new TiXmlElement("Suite");
            TiXmlElement* pCommonNode1 = new TiXmlElement("common");
            pSuiteNode->LinkEndChild(pCommonNode1);
            ptrXmlDoc->LinkEndChild(pSuiteNode);
            pRoot = pSuiteNode;
            pCommonNode = pCommonNode1;

            for (std::map<std::wstring,std::wstring>::iterator it = mmapCommonEnvironment.begin();
                it != mmapCommonEnvironment.end();++it)
            {
                SetElementTextValue(pCommonNode,JGW_W2A(it->first).c_str(),it->second);
            }
            SetElementTextValue(pCommonNode,JGW_W2A(msDmgCalConfig.mstrDutRxGuTxRssiEnvironment,CP_UTF8).c_str(),strDutRxGuTxRssi);
            SetElementTextValue(pCommonNode,JGW_W2A(msDmgCalConfig.mstrDutTxGuRxRssiEnvironment,CP_UTF8).c_str(),strDutTxGuRxRssi);
        }
        else
        {
            pRoot = ptrXmlDoc->RootElement();
            if (!pRoot) return false;

            pCommonNode = pRoot->FirstChildElement("common");
            if (!pCommonNode) return false;

            SetElementTextValue(pCommonNode,JGW_W2A(msDmgCalConfig.mstrDutRxGuTxRssiEnvironment,CP_UTF8).c_str(),strDutRxGuTxRssi);
            SetElementTextValue(pCommonNode,JGW_W2A(msDmgCalConfig.mstrDutTxGuRxRssiEnvironment,CP_UTF8).c_str(),strDutTxGuRxRssi);
        }
        ptrXmlDoc->SaveFile(JGW_W2A(strTSEConfigXmlPath));
        return true;
    }
    //！ 解析CSV文件
    void csvline_populate(std::vector<std::wstring> &record, const std::wstring& line, wchar_t delimiter = L',')
    {
        int linepos=0;
        int inquotes=false;
        wchar_t c;
        int linemax=line.length();
        std::wstring curstring;
        record.clear();

        while(line[linepos]!=0 && linepos < linemax)
        {

            c = line[linepos];

            if (!inquotes && curstring.length()==0 && c==L'"')
            {
                //beginquotechar
                inquotes=true;
            }
            else if (inquotes && c==L'"')
            {
                //quotechar
                if ( (linepos+1 <linemax) && (line[linepos+1]==L'"') )
                {
                    //encountered 2 double quotes in a row (resolves to 1 double quote)
                    curstring.push_back(c);
                    linepos++;
                }
                else
                {
                    //endquotechar
                    inquotes=false;
                }
            }
            else if (!inquotes && c==delimiter)
            {
                //end of field
                record.push_back( curstring );
                curstring=L"";
            }
            else if (!inquotes && (c==L'\r' || c==L'\n') )
            {
                record.push_back( curstring );
                return;
            }
            else
            {
                curstring.push_back(c);
            }
            linepos++;
        }
        record.push_back( curstring );
        return;
    }

    //! 
    bool CCTSEGuCalDMG::GetTSERssiStandard(const std::wstring& strDutTestDataFilePath,std::wstring& strDutTxGuRxRssi,std::wstring& strDutRxGuTxRssi)
    {
        std::map<int,std::vector<int>> mapChannelDutTxGuRxRssi,mapChannelDutRxGuTxRssi;
        std::wifstream guTestDatafstream;
        wchar_t szReadBuf[2048] = {0};
        int nTestStatusPos = 11;
        int nDutTxGuRxRssiColumns[MAX_DMG_CHANNEL] = {12,15,18,21,24,27};
        int nDutRxGuTxRssiColumns[MAX_DMG_CHANNEL] = {30,33,36,39,42,45};
        guTestDatafstream.open(strDutTestDataFilePath.c_str());
        strDutTxGuRxRssi = L"";
        strDutRxGuTxRssi = L"";
        if (!guTestDatafstream.is_open())
        {
            PELOG4WW_F(L"Open Gu Test Data File Path(%s) Fail\n",strDutTestDataFilePath.c_str());
            return false;
        }
        std::vector<std::wstring> vHeader;
        std::vector<int> vTemp;
        std::wstring strTemp;
        for (int x = 0;x < MAX_DMG_CHANNEL;x ++)
        {
            mapChannelDutTxGuRxRssi[x] = vTemp;
            mapChannelDutRxGuTxRssi[x] = vTemp;
        }

        guTestDatafstream.getline(szReadBuf,1024);
        JGW_ParserStrW(szReadBuf,L",",vHeader);
        for (size_t i = 0;i < vHeader.size();i ++)
        {
            if (0 == JGW_WStrComparenoCaseWStr(L"TestStatus",vHeader[i].c_str()))
            {
                nTestStatusPos = i;
                continue;
            }
            for (int x = 0;x < MAX_DMG_CHANNEL;x ++)
            {
                JGW_FormatWString(strTemp,L"DutTxGuRxCh%dRssi",x + 1);
                if (0 == JGW_WStrComparenoCaseWStr(strTemp.c_str(),vHeader[i].c_str()))
                {
                    nDutTxGuRxRssiColumns[x] = i;
                    continue;
                }
            }
            for (int x = 0;x < MAX_DMG_CHANNEL;x ++)
            {
                JGW_FormatWString(strTemp,L"DutRxGuTxCh%dRssi",x + 1);
                if (0 == JGW_WStrComparenoCaseWStr(strTemp.c_str(),vHeader[i].c_str()))
                {
                    nDutRxGuTxRssiColumns[x] = i;
                    continue;
                }
            }
        }

        while (guTestDatafstream.getline(szReadBuf,1024))
        {
            vHeader.clear();
            csvline_populate(vHeader,szReadBuf,L',');
            if (nTestStatusPos > (int)vHeader.size() || 0 != JGW_WStrComparenoCaseWStr(L"PASS",vHeader[nTestStatusPos].c_str()))
            {
                continue;
            }
            for (int x = 0;x < MAX_DMG_CHANNEL;x ++)
            {
                if (nDutTxGuRxRssiColumns[x] > (int)vHeader.size())
                {
                    continue;
                }
                if (!vHeader[nDutTxGuRxRssiColumns[x]].empty())
                {
                    mapChannelDutTxGuRxRssi[x].push_back(_ttoi(vHeader[nDutTxGuRxRssiColumns[x]].c_str()));
                }
            }

            for (int x = 0;x < MAX_DMG_CHANNEL;x ++)
            {
                if (nDutRxGuTxRssiColumns[x] > (int)vHeader.size())
                {
                    continue;
                }
                if (!vHeader[nDutRxGuTxRssiColumns[x]].empty())
                {
                    mapChannelDutRxGuTxRssi[x].push_back(_ttoi(vHeader[nDutRxGuTxRssiColumns[x]].c_str()));
                }
            }
        }

        for (int x = 0;x < MAX_DMG_CHANNEL;x ++)
        {
            int value = GetTSERssiStandardValue(mapChannelDutTxGuRxRssi[x]);
            if (value < -1)
            {
                strDutTxGuRxRssi += JGW_GetFormatWString(L"%d,",value);
            }

            value = GetTSERssiStandardValue(mapChannelDutRxGuTxRssi[x]);
            if (value < -1)
            {
                strDutRxGuTxRssi += JGW_GetFormatWString(L"%d,",value);
            }
        }
        JGW_EraseLastAndFristChars(strDutTxGuRxRssi);
        JGW_EraseLastAndFristChars(strDutRxGuTxRssi);

        return !strDutTxGuRxRssi.empty() && !strDutRxGuTxRssi.empty();
    }
    /*
    double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);  
    double mean =  sum / resultSet.size(); //均值  

    double accum  = 0.0;  
    std::for_each (std::begin(resultSet), std::end(resultSet), [&](const double d) {  
    accum  += (d-mean)*(d-mean);  
    });  

    double stdev = sqrt(accum/(resultSet.size()-1)); //方差  
    */
    int CCTSEGuCalDMG::GetTSERssiStandardValue(std::vector<int>& vValue)
    {
        if (vValue.empty())
        {
            return -1;
        }
        double sum = 0,mean = 0;
        std::vector<int>::iterator it = vValue.end();
        std::vector<int> vTemp(vValue.begin(),vValue.end());

        switch (msDmgCalConfig.meAlgorithm)
        {  
        case E_TSE_GuCal_MIN_LIMIT:
            it = std::min_element(vValue.begin(),vValue.end());
            return it[0] - msDmgCalConfig.mnLimit;
        case E_TSE_GuCal_RMINMAX_AVG:
            if (vTemp.size() >= 3)
            {
                it = std::min_element(vTemp.begin(),vTemp.end());
                vTemp.erase(it);
                it = std::max_element(vTemp.begin(),vTemp.end());
                vTemp.erase(it);
                sum = std::accumulate(std::begin(vTemp), std::end(vTemp), 0.0);  
                mean =  sum / vTemp.size(); //均值
            }
            else
            {
                sum = std::accumulate(std::begin(vValue), std::end(vValue), 0.0);  
                mean =  sum / vValue.size(); //均值 
            }
            return (int)mean - msDmgCalConfig.mnLimit;
        case E_TSE_GuCal_RMINMAX_MIN_LIMIT:
            if (vTemp.size() >= 2)
            {
                it = std::min_element(vTemp.begin(),vTemp.end());
                vTemp.erase(it);
                it = std::min_element(vTemp.begin(),vTemp.end());
            }
            else
            {
                it = vTemp.begin();
            }
            return it[0] - msDmgCalConfig.mnLimit;
        case E_TSE_GuCal_AVG:
        default:
            sum = std::accumulate(std::begin(vValue), std::end(vValue), 0.0);  
            mean =  sum / vValue.size(); //均值
            return (int)mean - msDmgCalConfig.mnLimit;
        }

        return -1;
    }
}
