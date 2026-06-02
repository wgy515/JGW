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

        bool FmtPumaImageFile(const char* strImageFilePath);

        const std::vector<S_PUMA_IMAGE_DATA_INFO>& GetPumaImageDataInfo() { return mvPumaImageDataInfo;}

        const std::vector<S_PUMA_IMAGE_DATA_INFO>& GetPumaValidateImageDataInfo() { return mvPumaValidateImageDataInfo;}
    private:
        void GetFlashReadDataCmd(char* szCmd,unsigned int a_address, unsigned int a_length);

        void ParseDownloadImageFmtBuffer(char* pData);

        void ParseValidateReadImageDataInfo(char* pData);

        void FreeFmtBuffer();
        //! 格式化发送缓冲区
        int FormatSendBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length);
        //! 格式化接收缓冲区
        int FormatRecvBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length);
    private:
        size_t mnImageFileSize;
        size_t mnImageFileFmtSize;
        std::vector<S_PUMA_IMAGE_DATA_INFO> mvPumaImageDataInfo;    
        std::vector<S_PUMA_IMAGE_DATA_INFO> mvPumaValidateImageDataInfo;
    };
}


