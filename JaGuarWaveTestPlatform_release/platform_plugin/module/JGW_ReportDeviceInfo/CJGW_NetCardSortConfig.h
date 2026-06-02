#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
namespace JGW
{
    class CCJGW_NetCardSortConfig
    {
    public:
        CCJGW_NetCardSortConfig(void);
        ~CCJGW_NetCardSortConfig(void);
    public:
        //! 
        void LoadNetCardSortConfig(const std::wstring& strSortConfigFilePath);
        //! 
        void SaveNetCardSortConfig();
        //! 
        void ClearNetCardSortConfig();
        //!
        int GetSortIndexToNetCardDescription(const std::wstring& strDescription);
    private:
        void WriteNetCardSortConfig(int index,const std::wstring& strDescription);
    private:
        CCJGW_ConfigIni mConfigIni;
        std::vector<std::wstring> mvstrDescription;
        CCJGW_CriticalSectionLock mCriticalSectionLock;
    };

}

