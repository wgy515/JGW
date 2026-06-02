#pragma once

namespace JGW
{
    //! <entry name="model" size="32"/>
    typedef struct  
    {
        char* mszEntryBuf;
        size_t mnSize;
        bool mbIsHexNumber;
        std::string mstrEntryName;
    }S_EntryInfo,*LPS_EntryInfo;

    class CJGW_DeviceInfoBuf
    {
    public:
        CJGW_DeviceInfoBuf(void);
        ~CJGW_DeviceInfoBuf(void);
    public:
        bool LoadDeviceInfoBuf(const std::wstring& strDeviceInfoFilePath,const std::wstring& strBaseXmlFilePath,bool isEncryptFile = false);
        bool UpdateDeviceInfo(const std::string& strEntryName,const std::string& strEntryValue);
        bool ReadDeviceInfo(const std::string& strEntryName,std::string& strEntryValue);
        bool SaveDeviceInfoBuf(const std::string& strSaveFilePath,bool isEncrypt = false);
    private:
        bool LoadDeviceInfoBaseXmlFilePath(const std::wstring& strBaseXmlFilePath);
    private:
        size_t mnDeviceInfoBinFileSize;
        char* mpDeviceInfoBuf;
        std::vector<S_EntryInfo> mvEntryInfo;
    };

}


