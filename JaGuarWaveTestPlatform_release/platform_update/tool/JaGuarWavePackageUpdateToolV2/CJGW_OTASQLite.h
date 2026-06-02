#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
#include <vector>
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    class CCJGW_OTASQLite
    {
    public:
        CCJGW_OTASQLite(void);
        ~CCJGW_OTASQLite(void);
    public:
        bool LoadOTASQLite(CCJGW_SQLite& sqliteDB);
        std::vector<S_OTA>& GetOTASQLiteInfos();
        //! 
        bool GetOTAInfosToInstallVersionID(CCJGW_SQLite& sqliteDB,const int installVersionID,std::vector<S_OTA>& vsOta);
        //!
        bool UpdateOTADBToOTAInfo(CCJGW_SQLite& sqliteDB,const S_OTA& sOta);
        //! 
        bool UpdateOTAIsLatestVersionToOTAInfo(CCJGW_SQLite& sqliteDB,const int installVersionID,bool isLatestVersion);
        //! 
        bool InsertOTADBToOTAInfo(CCJGW_SQLite& sqliteDB,const S_OTA& sOta);
    private:
        std::vector<S_OTA> mvsOta;
    };
}


