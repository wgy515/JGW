#pragma once
#include <vector>
#include "PumaDownload_Define.h"

namespace JGW
{
    class CCJGW_PumaImageFmtData
    {
    public:
        CCJGW_PumaImageFmtData(void);
        ~CCJGW_PumaImageFmtData(void);

        bool FmtPumaImageFile(const char* strImageFilePath,bool bValidateImage);

        const std::vector<S_PUMA_IMAGE_DATA_INFO>& GetPumaImageDataInfo() { return mvPumaImageDataInfo;}

        const std::vector<S_PUMA_IMAGE_DATA_INFO>& GetPumaValidateImageDataInfo() { return mvPumaValidateImageDataInfo;}

        const std::vector<S_PUMA_IMAGE_DATA_INFO>& GetPumaEraseImageDataInfo() { return mvPumaEraseImageDataInfo;}

        const std::vector<S_PUMA_IMAGE_DATA_INFO>& GetPumaEraseAllImageDataInfo() { return mvPumaEraseAllImageDataInfo;}

        size_t GetDownloadBufCounts(); 

        size_t GetImageFileSize();
    private:
        void GetFlashReadDataCmd(char* szCmd,unsigned int a_address, unsigned int a_length);

        void ParseDownloadImageFmtBuffer(char* pData);

        void ParseValidateReadImageDataInfo(char* pData);
        /// 
        void ParseEraseImageDataInfo(size_t erase_size,std::vector<S_PUMA_IMAGE_DATA_INFO>& veraseImageDataInfos);

        void FreeFmtBuffer();
        //! 格式化发送缓冲区
        int FormatSendBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length);
        //! 格式化接收缓冲区
        int FormatRecvBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length);
    private:
        char* GetPumaImageBuf(size_t nSize);
    private:
        size_t mnImageFileSize;
        //! size_t mnImageFileFmtSize;
        std::vector<S_PUMA_IMAGE_DATA_INFO> mvPumaImageDataInfo;    
        std::vector<S_PUMA_IMAGE_DATA_INFO> mvPumaValidateImageDataInfo;
        std::vector<S_PUMA_IMAGE_DATA_INFO> mvPumaEraseImageDataInfo;
        //! 擦除所有镜像文件
        std::vector<S_PUMA_IMAGE_DATA_INFO> mvPumaEraseAllImageDataInfo;

        size_t mnPumaImageTxRxBufIndex;
        char* mnPumaImageTxRxBuf;
    };
}


