#pragma once
#include <JGW_FoundationFunc/JGW_PrsDownloadProtocol.h>
/*#include "CJGW_PrsSerial.h"*/
#include "CJGW_CSerialComPort.h"
#include "CJGW_PrsFlashDownloadBase.h"

namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_PrsDownloadFlash : public CCJGW_PrsFlashDownloadBase
    {
    public:
        CCJGW_PrsDownloadFlash(CCJGW_CSerialComPort& seialComPort);
        ~CCJGW_PrsDownloadFlash(void);
    public: //! CCJGW_PrsFlashDownloadBase
        //! 
        bool InitPDBInfoToDongle();
        //! 
        bool LoadPDBImageFile(std::string& strFloder);
        //!
        void ChangeSNToImageFileBuf(const char* szSN);
        //!
        void ChangeMacToImageFileBuf(const char* macAddr);
        //!
        bool ReadSNMacToDongleFlash(std::string& strSN,std::string& strMAC);
        //!
        bool DownloadPDBImageFileToDongleFlash(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileTypes);
        //!
        bool SavePDBImageFileToDongleFlash(E_DONGLES_PDB_FILE_TYPE eDonglesPDBFileType,const std::wstring& strSavePDBHeaderFilePath,const std::wstring& strSavePDBBobyFilePath);
        //!
        void ResetDongleDevice();
    private:
        //! 初始化flash address
        void InitializeFlashAddresses(uint32_t a_flashSizeKb = PRS_FLASH_DEFAULT_SIZE_KB);
        //! 释放flash iamge buf
        void FreeDownloadFlashImage();
        //! header + boby 文件合成一个文件用于下载
        bool GetPrsFileInfo(S_PRS_FILE_INFO& sFileInfo,const std::string& strPDBHeaderFilePath,const std::string& strPDBBobyFilePath);
        //! check file info is vailiad
        inline bool CheckPrsFileInfoIsValid(S_PRS_FILE_INFO& sFileInfo);
        //! 验证FLASH是否为空
        int FlashDataValidate(char *a_pData, unsigned int a_address, unsigned int a_length);
        //! 验证flash data和prs file buf是否一致
        bool ValidateFlashDataEqualToPrsFileBuf(S_PRS_FILE_INFO& sFileInfo);
        //! 擦除flash数据
        int FlashEraseData(unsigned int a_address, unsigned int a_length);
        //! 读flash数据
        int FlashReadData(char *a_pData, unsigned int a_address, unsigned int a_length);
        //! 写入flash数据
        int FlashWriteData(char *a_pData, unsigned int a_address, unsigned int a_length);
        //! 发送flash cmd 读取flash响应 0: 成功 1: 失败
        int FlashSendReceiveIsError(const char* a_pData, char* a_pResp, unsigned int a_length);
        //! 发送flash cmd 读取flash响应  0: 成功 1: 失败
        int FlashSendReceiveIsError(const char* a_pData/*, char* a_pResp, unsigned int a_length*/);
        //! 定位SN MAC在OTP FILE BUF文件位置
        bool GetSerialNumberMacAddressPosToOtpBinBuf(S_PRS_FILE_INFO& sFileInfo);
        //! 检查PDB文件类型
        //uint32_t CheckPDBFileTypeGetFlashOffset(unsigned char* szFileBuf,size_t fileSize);
    public:
        //! 根据目录加载下载BIN文件信息
        bool InitDownloadFlashImage(const std::string& strFolder,bool bInitFireware);
        //! 生成PDB文件根据PDB BUF
        bool BuildPDBFileToPDBBuf(const std::string& saveFolder,bool bDownFireware = true,bool bDownloadOtp = true,bool bDownloadConfig = true);
        //! 下载所有PDB文件
        bool DonwloadAllPDBImageFileToDongleFlash(bool bDownFireware = true,bool bDownloadOtp = true,bool bDownloadConfig = true);
        //! 下载FLASH镜像根据PRS FILE INFO
        bool DownloadFlashWriteImage(S_PRS_FILE_INFO& psPrsFileInfo,bool isFireware = false);
    public:
        //! 重新生成或者校验OTP CRC 
        bool BuildOTPCRC32();
        //! 设置SN和MAC地址
        bool SetSerailNumberMacAddressToOtpFile(const std::string& serailNumber,const std::string& macAddress);
        /// 
        //bool OpenPrsPort(const std::wstring& strComPort);
        /// 
        //bool DownloadPrsProgramFlash(SPrsFlashCmdParms sPrsFlashCmdParms);
		//! check device is bootloader mode
		//! 0 BootloaderMode
		//! 1 Falcon UMAC
		//! -1 error
		eDeviceBootMode CheckDeviceIsBootloaderMode();
    private:
        eDeviceBootMode meDeviceBootMode;
        //! 
        char* mRecvBuf;
        //!
        UINT32 muSerialNumberPosToBin;
        //! 
        UINT32 muMacAddressPosToBin;
        //! 
        SPrsFlashCmdParms msSPrsFlashCmdParms;
        //!
        PrsFlashAddress msPrsFlashAddress;
        //!
        CCJGW_CSerialComPort& mcPrsSerial;
    };
}
