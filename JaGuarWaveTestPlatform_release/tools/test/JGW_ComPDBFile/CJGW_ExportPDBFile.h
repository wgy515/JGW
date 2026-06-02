#pragma once
#include "PerasoDefine.h"
#include "CJGW_CSerialComPort.h"

#include <string>

namespace JGW
{
    class CCJGW_ExportPDBFile
    {
    public:
        CCJGW_ExportPDBFile(void);
        ~CCJGW_ExportPDBFile(void);

        //! 加载PDB Image文件
        bool LoadPDBImageFile(std::string& strFloder,std::string& strExportFolder);
        //! 初始化PDB
        bool InitPDBInfoToDongle();
        //! 打开Dongle端口
        bool OpenDongleComPort(std::wstring strComName);
        //!
        bool ReadSerialNumber(std::wstring& strSerialNumber);
        //!
        bool ReadSerialNumberToBoardInfo(std::wstring& strSerialNumber);
        //!
        bool ReadMacAddress(std::wstring& strMacAddress);
        //!
        bool ReadMacAddressToBoardInfo(std::wstring& strMacAddress);
        //! close dongles comport
        void CloseDongleComPort();

        bool ExportPDBAllFile();

        bool ComparePDBAllFile();
        //!
        bool ComparePDBFileToFileType(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileType);
        //!
        bool ExportPDBFileToFileType(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileType);
        //!
        bool GetOtpPDBImageSize();
        //!
        bool GetConfigPDBImageSize();
    private:
        //!
        bool PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount);

        bool PareseDonglesReadBitBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBitCount);
        //!
        void EraseCliString(std::string& strRead);

        void FormattedReadLittleEndianBit(unsigned char* file_buf,const char* read_buf);

        bool FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf);

        bool FlashReadFormattedBufferBlock(std::string& strRead,size_t read_len,UINT32 offest);
        bool FlashReadBlock(char* file_buf,size_t read_size,UINT32 offest);

        void VerifyReadSizeDivisibleFour(size_t& write_size);
        //! 卸载PDB IMAGE
        void UnLoadPDBImageFile();

        void GetPDBAddrToFlash(UINT32 uFlashTotalSize);
        //! 检查端口是否处于Ready状态
        bool IsCliReady();
        //! 检查返回字符串
        bool CheckComportResponce(std::string& strRead);
    private:
        //!S_DONGLES_PDB_FLASH_ADDR msOptConfigAddr;
        CCJGW_CSerialComPort mscp;
        S_DONGLES_PDB_FILES_INFO msOTPConfigFiles;
        std::string mstrExportFolder;
        UINT32 muFlashTotalSize;
        UCHAR mnManufacturerID;
        char* mszWriteBuf;
        char* mszReadBuf;
    };
}


