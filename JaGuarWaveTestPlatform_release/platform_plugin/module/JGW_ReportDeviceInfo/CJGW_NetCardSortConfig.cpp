#include "StdAfx.h"
#include "CJGW_NetCardSortConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_NetCardSortConfig::CCJGW_NetCardSortConfig(void)
    {
    }


    CCJGW_NetCardSortConfig::~CCJGW_NetCardSortConfig(void)
    {
    }

    void CCJGW_NetCardSortConfig::LoadNetCardSortConfig(const std::wstring& strSortConfigFilePath)
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        mvstrDescription.clear();
        mConfigIni.InitIniFilePath(strSortConfigFilePath.c_str());
        Log4WD_F(L"Load NetCard Sort Config Path:%s",strSortConfigFilePath.c_str());

        int count = mConfigIni.GetIniKeyIntValue(L"MAIN",L"Count",0);
        std::wstring strTemp;
        for (int i = 0;i < count;i ++)
        {
            JGW_FormatWString(strTemp,L"NetworkInterface_%d",i + 1);
            mvstrDescription.push_back(mConfigIni.GetIniKeyValue(L"MAIN",strTemp.c_str()));
            Log4WD_F(L"%s - %s",strTemp.c_str(),mvstrDescription[mvstrDescription.size() - 1].c_str());
        }
    }

    void CCJGW_NetCardSortConfig::ClearNetCardSortConfig()
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        mConfigIni.SetIniKeyValue(L"MAIN",L"Count",0);
        mvstrDescription.clear();
        Log4WD(L"Clear NetCard Sort Config");
    }

    void CCJGW_NetCardSortConfig::SaveNetCardSortConfig()
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        for (size_t i = 0;i < mvstrDescription.size();i ++)
        {
            WriteNetCardSortConfig(i + 1,mvstrDescription[i].c_str());
        }
        mConfigIni.SetIniKeyValue(L"MAIN",L"Count",(int)mvstrDescription.size());
    }

    void CCJGW_NetCardSortConfig::WriteNetCardSortConfig(int index,const std::wstring& strDescription)
    {
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"NetworkInterface_%d",index);
        mConfigIni.SetIniKeyValue(L"MAIN",strTemp.c_str(),strDescription.c_str());
    }

    int CCJGW_NetCardSortConfig::GetSortIndexToNetCardDescription(const std::wstring& strDescription)
    {
        int emptyIndex = -1;
        std::wstring strTemp;
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        //! 
        for (size_t i = 0;i < mvstrDescription.size();i ++)
        {
            if (0 == JGW_WStrComparenoCaseWStr(mvstrDescription[i].c_str(),strDescription.c_str()))
            {
                return i + 1;
            }
            //! 当描述符为空时候
            if (mvstrDescription[i].empty() && -1 == emptyIndex)
            {
                emptyIndex = i + 1;
            }
        }
        //! 当未空描述符说明还有位置写入
        if (emptyIndex >= 1)
        {
            mvstrDescription[emptyIndex - 1] = strDescription;
            WriteNetCardSortConfig(emptyIndex,strDescription.c_str());
            return emptyIndex;
        }
        
        mvstrDescription.push_back(strDescription);
        WriteNetCardSortConfig(mvstrDescription.size(),strDescription.c_str());
        mConfigIni.SetIniKeyValue(L"MAIN",L"Count",(int)mvstrDescription.size());
        Log4WD_F(L"NetworkInterface_%d - %s",mvstrDescription.size(),strDescription.c_str());
        return mvstrDescription.size();
    }
}
