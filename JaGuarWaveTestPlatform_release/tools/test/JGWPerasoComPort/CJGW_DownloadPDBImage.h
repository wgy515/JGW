#pragma once
#include "PerasoDefine.h"
#include "CJGW_CSerialComPort.h"

#include <string>
//! operation procedure
#define DEBUG_OPERATION_PROCEDURE 
/*
step 1: open com port
step 2: fli    ------read flash info 
step 3: device_size  == total_size 0x100000  manufacturer_id : C2
step 4: check isCliReady  send " \n" to device
step 4: send "mib s 11-42-1-9 00 00 00 00\n" to device
step 5: send "fli\n" to device --- verify Flash is recognized 
step 6: if read not ok or manufacturer_id == 255 --- return cannot read flash info
step 7: check otp config image is exists 
step 8: erase persistent_database_1_config.bin  and persistent_database_0_otp.bin file
*/
//! cli ready
namespace JGW
{
    class CCJGW_DownloadPDBImage
    {
    public:
        CCJGW_DownloadPDBImage(void);
        ~CCJGW_DownloadPDBImage(void);

        //! 加载PDB Image文件
        void LoadPDBImageFile(std::string& strFloder);
        //! 打开Dongle端口
        bool OpenDongleComPort(std::wstring strComName);
        //! 初始化OPT
        bool InitPDBInfoToDongle();
        //! 下载PDB Imgae 文件
        bool DonwloadAllPDBImageFileToDongleFlash();
        //!
        void ChangeSNToImageFileBuf(const char* szSN);
        //!
        bool BuildOTPCRC32();
        //! 
        bool BuildConfigCRC32();
        //!
        bool DownloadPDBImageFileToDongleFlash(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileTypes);
        //! 重启Dongle
        void ResetDongleDevice();
        //! 写SN或者MAC到Dongle的flash中
        bool DownloadSNMacToDongleFlash(const char* szSN,const char* macAddr);
        //!
        bool BuildSNMacToFile(const char* szSN,const char* macAddr);
        //! 从dongle flash中读取SN和MAC
        bool ReadSNMacToDongleFlash(std::string& strSN,std::string& strMAC);
        //！ 关闭Dongle端口
        void CloseDongleComPort();
   public:
        void UnLoadPDBImageFile();
        //! 删除或者下载镜像到Dongle flash
        bool EraseAndDonwloadImageFileToFlash(bool bErase,const UINT32 uFlashAddr,S_FILE_INFO& sFileInfo);
        //! 重新构造CRC32 到Image Buf
        bool GeneratedCRC32ToImageBuf(/*const*/ unsigned char* szImageBuf,size_t buf_len);
        //! 格式化小端 讲4字节镜像转化4字节的UINT32
        UINT32 FormattedLittleEndianInt(const unsigned char* szBuf);
        //! 计算IMAGE BUF的CRC32
        UINT32 CalcChecksum32ToImageBuf (const unsigned char* szBuf,size_t buf_len);
        //!
        void ChangeMacToImageFileBuf(const char* macAddr);
        //! 返回格式化BUF大小 讲4字节数据 格式化为9字节的形式 将0x60010101 转化为"01010160 "
        size_t Flash_Write_Formatted_Buffer(const unsigned char* buf,size_t buf_len);
        //! erase_len = file size()
        bool FlashErase(UINT32 addr,UINT32 erase_len);
        //! flash 写入校验
        bool FlashWriteVerify(PS_FILE_INFO psFileInfo,UINT32 addr);
        //! 读取flash  读取的格式类似于"01010160"
        bool FlashReadFormattedBufferBlock(std::string& strRead,size_t read_len,UINT32 offest);
        //! flash 块写入
        bool FlashWriteBlock(const unsigned char* szFileBuf,size_t bue_len,size_t current_offset);

        void GetPDBAddrToFlash(UINT32 uFlashTotalSize);
        //! 检查端口是否处于Ready状态
        bool IsCliReady();
        //! 检查返回字符串
        bool CheckComportResponce(std::string& strRead);
        //! 
        void VerifyWriteSizeDivisibleFour(size_t& write_size);
    private:
        //!S_DONGLES_PDB_FLASH_ADDR msOptConfigAddr;
        CCJGW_CSerialComPort mscp;
        S_DONGLES_PDB_FILES_INFO msOTPConfigFiles;
        UINT32 muFlashTotalSize;
        UCHAR mnManufacturerID;
        char* mszWriteBuf;
        char* mszReadBuf;
        char mstrChangeSN[128];
    };

}

