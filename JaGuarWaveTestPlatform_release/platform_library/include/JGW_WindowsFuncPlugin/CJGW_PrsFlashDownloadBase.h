#pragma once
#include "PerasoDefine.h"
namespace JGW
{
    class CCJGW_PrsFlashDownloadBase
    {
    public:
        //! 
        virtual bool InitPDBInfoToDongle() = 0;
        //! 
        virtual bool LoadPDBImageFile(std::string& strFloder) = 0;
        //!
        virtual void ChangeSNToImageFileBuf(const char* szSN) = 0;
        //!
        virtual void ChangeMacToImageFileBuf(const char* macAddr) = 0;
        //! 从dongle flash中读取SN和MAC
        virtual bool ReadSNMacToDongleFlash(std::string& strSN,std::string& strMAC) = 0;
        //! 
        virtual bool BuildOTPCRC32() = 0;
        //! 
        virtual bool DownloadPDBImageFileToDongleFlash(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileTypes) = 0;
        //! strSavePDBBobyFilePath 为空则只保存一个PDB文件否则，保存两个文件
        virtual bool SavePDBImageFileToDongleFlash(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileType,const std::wstring& strSavePDBHeaderFilePath,const std::wstring& strSavePDBBobyFilePath) = 0;
        //! 重启Dongle
        virtual void ResetDongleDevice() = 0;
        
    };
}