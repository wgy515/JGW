#include "StdAfx.h"
#include "JGW_IPQ40xxGuTestData.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <fstream>
#include <TSE_FoundationLib/TSE_FoundationLib_Define.h>

namespace JGW
{
    CJGW_IPQ40xxGuTestData::CJGW_IPQ40xxGuTestData(void) : mstrTxStationCalPathName(L"BH0_LP,BH1_LP")
    {
    }


    CJGW_IPQ40xxGuTestData::~CJGW_IPQ40xxGuTestData(void)
    {
    }

    CJGW_IPQ40xxGuTestData* CJGW_IPQ40xxGuTestData::GetInstance()
    {
        static CJGW_IPQ40xxGuTestData* gpIPQ40xxGuTestData = NULL;
        if (NULL == gpIPQ40xxGuTestData)
        {
            gpIPQ40xxGuTestData = new CJGW_IPQ40xxGuTestData();
        }
        return gpIPQ40xxGuTestData;
    }

    void CJGW_IPQ40xxGuTestData::ClearIPQ40xxGuTestData()
    {
        mmapChainMaskChannelPowerInfo.clear();
        //mmap5gPowerInfo.clear();
        //mmap24gPowerInfo.clear();
    }

    void CJGW_IPQ40xxGuTestData::SetTxStationCalPathName(const std::wstring& strTxStationCalPathName)
    {
        mstrTxStationCalPathName = strTxStationCalPathName;
        JGW_ReplaceStringW(mstrTxStationCalPathName,L",",L"-");
    }

    void CJGW_IPQ40xxGuTestData::UpdateIPQ40xxGuTest24ChannelInfo(const std::wstring& str24GChannelInfo)
    {
        mstr24GChannelInfo = str24GChannelInfo;
        JGW_ReplaceStringW(mstr24GChannelInfo,L",",L"-");
    }

    void CJGW_IPQ40xxGuTestData::UpdateIPQ40xxGuTest5ChannelInfo(const std::wstring& str5GChannelInfo)
    {
        mstr5GChannelInfo = str5GChannelInfo;
        JGW_ReplaceStringW(mstr5GChannelInfo,L",",L"-");
    }

    void CJGW_IPQ40xxGuTestData::UpdateIPQ40xxGuTestData(int nTxChainMask,int nChannel,double dPower)
    {
#if 1
        mmapChainMaskChannelPowerInfo[nTxChainMask][nChannel] = dPower;
#else
        if (nChannel >= 2412 && nChannel <= 2472)
        {
            mmap24gPowerInfo[nTxChainMask][nChannel] = dPower;
            //mmap24gPowerInfo.insert(std::make_pair<int,std::pair<int,double>>(nTxChainMask,std::make_pair<int,double>(nChannel,dPower)));
        }
        if (nChannel >= 4920 && nChannel <= 5920)
        {
            mmap5gPowerInfo[nTxChainMask][nChannel] = dPower;
            //mmap5gPowerInfo.insert(std::make_pair<int,std::pair<int,double>>(nTxChainMask,std::make_pair<int,double>(nChannel,dPower)));
        }
#endif
    }
#if 0
    double CJGW_IPQ40xxGuTestData::GetPowerToTxChainMaskChannel(int nTxChainMask,int nChannel)
    {
        double power = -999.99;
        if (nChannel >= 2412 && nChannel <= 2472)
        {
            std::map<int,std::map<int,double>>::iterator it =  mmap24gPowerInfo.find(nTxChainMask);
            if (it != mmap24gPowerInfo.end())
            {
                std::map<int,double>::iterator itChannelPower = it->second.find(nChannel);
                if (itChannelPower != it->second.end())
                {
                    power = itChannelPower->second;
                }
            }
        }
        if (nChannel >= 4920 && nChannel <= 5920)
        {
            std::map<int,std::map<int,double>>::iterator it =  mmap5gPowerInfo.find(nTxChainMask);
            if (it != mmap5gPowerInfo.end())
            {
                std::map<int,double>::iterator itChannelPower = it->second.find(nChannel);
                if (itChannelPower != it->second.end())
                {
                    power = itChannelPower->second;
                }
            }
        }
        return power;
    }
#endif
    void CJGW_IPQ40xxGuTestData::SaveIPQ40xxGuTestData(const std::wstring& strSN,const std::wstring& strRelativePath)
    {
#if 1
        std::wstring strLog;
        std::wstring strSaveTestDataCsvPath = JGW_RealativePathToAbsPath(strRelativePath.c_str());
        //! 检测金机测试数据保存文件是否存在
        bool bGuTestLogFileExists = JGW_FileExistsToFilePath(strSaveTestDataCsvPath.c_str());

        std::wofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(strSaveTestDataCsvPath.c_str(),std::ios_base::app);
        if (!fileStream.is_open())
        {
            JGW_FormatWString(strLog,L"Warning:Open Gu Test Data CSV(%s - %s) Log File Faill",strSaveTestDataCsvPath.c_str(),JGW_GetSystemErrorString(GetLastError()).c_str());
            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strLog.c_str());
            return;
        }

        //! 金机测试数据文件不存在的时候，重新生成头文件
        if (!bGuTestLogFileExists)
        {
            std::wstring strHeader(L"SN,TxStationCalPathName,");
            std::vector<std::wstring> vTemp;
            std::vector<int> v24GChannel,v5GChannel;
            JGW_ParserIntW(mstr24GChannelInfo.c_str(),L"-",v24GChannel);
            JGW_ParserIntW(mstr5GChannelInfo.c_str(),L"-",v5GChannel);
            JGW_ParserStrW(mstrTxStationCalPathName.c_str(),L"-",vTemp);
            size_t nTestDataCol = vTemp.size() * (v24GChannel.size() + v5GChannel.size());
            for (size_t i = 0;i < nTestDataCol;i ++)
            {
                strHeader += L",ChainMaskChannelPower";
            }
            fileStream << strHeader << std::endl;
        }
        //! 记录测试数据
        fileStream << strSN << L",";
        fileStream << mstrTxStationCalPathName;
        //! TxChainMask,Channel,Power
        for (std::map<int,std::map<int,double>>::iterator it = mmapChainMaskChannelPowerInfo.begin();
            it != mmapChainMaskChannelPowerInfo.end();
            ++ it)
        {
            for (std::map<int,double>::iterator itChannelPower = it->second.begin();
                itChannelPower != it->second.end();
                ++ itChannelPower)
            {
                fileStream << JGW_GetFormatWString(L"%d-%d-%.2f",it->first,itChannelPower->first,itChannelPower->second) << L",";
            }
        }
        fileStream << std::endl;
        fileStream.close();
#else
        std::wstring strLog;
        std::wstring strSaveTestDataCsvPath = JGW_RealativePathToAbsPath(strRelativePath.c_str());
        bool bGuTestLogFileExists = JGW_FileExistsToFilePath(strSaveTestDataCsvPath.c_str());
        std::vector<std::wstring> vTemp;
        JGW_ParserStrW(mstrTxStationCalPathName.c_str(),L"-",vTemp);

        std::wofstream fileStream;
        fileStream.imbue(std::locale(""));
        fileStream.open(strSaveTestDataCsvPath.c_str(),std::ios_base::app);
        if (!fileStream.is_open())
        {
            JGW_FormatWString(strLog,L"Warning:Open Gu Test Data CSV(%s - %s) Log File Faill",strSaveTestDataCsvPath.c_str(),JGW_GetSystemErrorString(GetLastError()).c_str());
            LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strLog.c_str());
            return;
        }
        //! 创建头文件
        if (!bGuTestLogFileExists)
        {
            std::wstring strHeader(L"SN,TxStationCalPathName,Channel24G,Channel5G,Power24G_1,Power5G_1");
            for (size_t i = 1;i < vTemp.size();i ++)
            {
                strHeader += JGW_GetFormatWString(L",Power24G_%d,Power5G_%d",i + 1,i + 1);
            }
            fileStream << strHeader << std::endl;
        }
        fileStream << strSN << L",";
        fileStream << mstrTxStationCalPathName << L",";
        fileStream << mstr24GChannelInfo << L",";
        fileStream << mstr5GChannelInfo << L",";

        std::wstring str24GPower,str5GPower;
        std::vector<int> v24GChannel,v5GChannel;
        JGW_ParserIntW(mstr24GChannelInfo.c_str(),L"-",v24GChannel);
        JGW_ParserIntW(mstr5GChannelInfo.c_str(),L"-",v5GChannel);

        for (size_t i = 0;i < vTemp.size();i ++)
        {
            str24GPower = L"";
            for (size_t index = 0;index < v24GChannel.size();index++)
            {
                if (index == v24GChannel.size() - 1)
                {
                    str24GPower += JGW_GetFormatWString(L"%.2f",GetPowerToTxChainMaskChannel(i + 1,v24GChannel[index]));
                }
                else
                {
                    str24GPower += JGW_GetFormatWString(L"%.2f-",GetPowerToTxChainMaskChannel(i + 1,v24GChannel[index]));
                }    
            }
            fileStream << str24GPower << L",";

            str5GPower = L"";
            for (size_t index = 0;index < v5GChannel.size();index++)
            {
                if (index == v5GChannel.size() - 1)
                {
                    str5GPower += JGW_GetFormatWString(L"%.2f",GetPowerToTxChainMaskChannel(i + 1,v5GChannel[index]));
                }
                else
                {
                    str5GPower += JGW_GetFormatWString(L"%.2f-",GetPowerToTxChainMaskChannel(i + 1,v5GChannel[index]));
                }
            }
            if (i == vTemp.size() - 1)
            {
                fileStream << str5GPower;
            }
            else
            {
                fileStream << str5GPower << L",";
            }
        }
        fileStream << std::endl;
#endif
    }
}
