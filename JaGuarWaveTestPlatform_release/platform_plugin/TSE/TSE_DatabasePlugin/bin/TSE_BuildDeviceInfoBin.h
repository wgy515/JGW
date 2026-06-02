#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "JGW_DeviceInfoBuf.h"
namespace JGW
{
    class CTSE_BuildDeviceInfoBin : public CTSE_TestBase
    {
    public:
        CTSE_BuildDeviceInfoBin(void);
        ~CTSE_BuildDeviceInfoBin(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIsEncryptFile;
        CJGW_DeviceInfoBuf mDeviceInfoBuf;
        //! device_info.bin 文件路径环境变量
        std::wstring mstrDeviceInfoFilePathEnvironment;
        //! 保存DEVICE INFO BIN环境变量
        std::wstring mstrDeviceInfoEnvironment;
        //! device_info_base.xml 文件路径
        std::wstring mstrDeviceInfoBaseXmlFilePath;
    };
}

