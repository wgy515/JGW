#include "StdAfx.h"
#include "CJGW_PumaImageFmtData.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCJGW_PumaImageFmtData::CCJGW_PumaImageFmtData(void):mnPumaImageTxRxBufIndex(0)
    {
        mnPumaImageTxRxBuf = (char*)memalign (JGW_GetPageSize (), PUMA_MAX_BUF_TX_RX);
        memset(mnPumaImageTxRxBuf,0x00,PUMA_MAX_BUF_TX_RX);
    }


    CCJGW_PumaImageFmtData::~CCJGW_PumaImageFmtData(void)
    {
        /*FreeFmtBuffer();*/
        if (mnPumaImageTxRxBuf)
        {
            ALIGNED_FREE(mnPumaImageTxRxBuf);
            mnPumaImageTxRxBuf = NULL;
        }
    }

    char* CCJGW_PumaImageFmtData::GetPumaImageBuf(size_t nSize)
    {
        char* pTemp = mnPumaImageTxRxBuf + mnPumaImageTxRxBufIndex;
        mnPumaImageTxRxBufIndex += nSize;
        return pTemp;
    }

    void CCJGW_PumaImageFmtData::FreeFmtBuffer()
    {
        mnPumaImageTxRxBufIndex = 0;
        memset(mnPumaImageTxRxBuf,0x00,PUMA_MAX_BUF_TX_RX);
//         for (std::vector<S_PUMA_IMAGE_DATA_INFO>::iterator it = mvPumaImageDataInfo.begin();
//             it != mvPumaImageDataInfo.end();
//             ++ it)
//         {
//             if (it->mpFmtBuffer && it->mDataSize > 0)
//             {
//                 free (it->mpFmtBuffer);
//             }
//         }
        mvPumaImageDataInfo.clear();
//         for (std::vector<S_PUMA_IMAGE_DATA_INFO>::iterator it = mvPumaValidateImageDataInfo.begin();
//             it != mvPumaValidateImageDataInfo.end();
//             ++ it)
//         {
//             if (it->mpFmtBuffer && it->mDataSize > 0)
//             {
//                 free (it->mpFmtBuffer);
//             }
//             if (it->mpDownloadCmd)
//             {
//                 free (it->mpDownloadCmd);
//             }
//         }
        mvPumaValidateImageDataInfo.clear();
        mvPumaEraseImageDataInfo.clear();
    }

    bool CCJGW_PumaImageFmtData::FmtPumaImageFile(const char* strImageFilePath,bool bValidateImage)
    {
        char* pFileBuf = JGW_GetFileBufToPath_C(strImageFilePath,mnImageFileSize);
        if (NULL == pFileBuf || 0 == mnImageFileSize) return false;

        FreeFmtBuffer();
        ParseDownloadImageFmtBuffer(pFileBuf);
        if (bValidateImage) ParseValidateReadImageDataInfo(pFileBuf);
        ParseEraseImageDataInfo(mnImageFileSize,mvPumaEraseImageDataInfo);
        ParseEraseImageDataInfo(PERASO_DEVICE_SIZE - 1/*mnImageFileSize*/,mvPumaEraseAllImageDataInfo);
        free(pFileBuf);
        return true;
    }

    size_t CCJGW_PumaImageFmtData::GetImageFileSize()
    {
        return mnImageFileSize;
    }

    size_t CCJGW_PumaImageFmtData::GetDownloadBufCounts()
    {
         double dCounts = (mnPumaImageTxRxBufIndex + mnPumaImageTxRxBufIndex * 0.2)/1024;
         return static_cast<size_t>(dCounts);
    }

    void CCJGW_PumaImageFmtData::ParseDownloadImageFmtBuffer(char* pData)
    {
        S_PUMA_IMAGE_DATA_INFO sPumaImageDataInfo = {0};
#ifdef FLASH_ENABLE_ARRAY
        S_PUMA_IMAGE_DATA_INFO sPumaFlashEnable = {(char*)flashEnable,NULL,0,0,strlen(flashEnable)};
#endif
        size_t bufferLen = mnImageFileSize,address = FlashImageOffset,dataLen = 128;
        while (bufferLen)
        {
            dataLen = (bufferLen > 128) ? 128 : bufferLen;
            sPumaImageDataInfo.mDataSize = dataLen;
            sPumaImageDataInfo.mAddress = address;
            sPumaImageDataInfo.mpFmtBuffer = GetPumaImageBuf(32 + (dataLen * 3));
            /*(char *)malloc(32 + (dataLen * 3));*/
            sPumaImageDataInfo.mFmtBufferSize = FormatSendBuffer(sPumaImageDataInfo.mpFmtBuffer,address,pData,dataLen) - 1;
            pData += dataLen;
            address += dataLen;
            bufferLen -= dataLen;
#ifdef FLASH_ENABLE_ARRAY
            mvPumaImageDataInfo.push_back(sPumaFlashEnable);
#endif
            mvPumaImageDataInfo.push_back(sPumaImageDataInfo);
        }
    }

    void CCJGW_PumaImageFmtData::ParseValidateReadImageDataInfo(char* pData)
    {
        size_t bufferLen = mnImageFileSize,address = FlashImageOffset,dataLen = FLASH_READ_SIZE;
        S_PUMA_IMAGE_DATA_INFO sPumaImageDataInfo = {0};

        while (bufferLen)
        {
            dataLen = (bufferLen > FLASH_READ_SIZE) ? FLASH_READ_SIZE : bufferLen;
            sPumaImageDataInfo.mAddress = address;
            sPumaImageDataInfo.mDataSize = dataLen;
            sPumaImageDataInfo.mpFmtBuffer = GetPumaImageBuf(788)/*(char *)malloc(788)*/;
            sPumaImageDataInfo.mFmtBufferSize = FormatRecvBuffer(sPumaImageDataInfo.mpFmtBuffer,address,pData,dataLen);
            sPumaImageDataInfo.mpDownloadCmd = GetPumaImageBuf(32)/*(char *)malloc(32)*/;
            memset(sPumaImageDataInfo.mpDownloadCmd,0x00,32);
            GetFlashReadDataCmd(sPumaImageDataInfo.mpDownloadCmd,address,dataLen);
            pData += dataLen;
            address += dataLen;
            bufferLen -= dataLen;
            mvPumaValidateImageDataInfo.push_back(sPumaImageDataInfo);
        }
    }

    void CCJGW_PumaImageFmtData::ParseEraseImageDataInfo(size_t erase_size,std::vector<S_PUMA_IMAGE_DATA_INFO>& veraseImageDataInfos)
    {
        size_t bufferLen = erase_size,address = FlashImageOffset,dataLen = FLASH_READ_SIZE;
        size_t a_address = 0 & 0xff0000;
        S_PUMA_IMAGE_DATA_INFO sPumaImageDataInfo = {0};
        unsigned int nbrOfSectors = (bufferLen/FLASH_SECTOR_SIZE) + ((bufferLen % FLASH_SECTOR_SIZE) ? 1 : 0);
        unsigned int i, index, offset;
        veraseImageDataInfos.clear();
        index = sizeof(scSpiWriteCmd) - 1;
        
        for ( ; nbrOfSectors; nbrOfSectors--)
        {
            sPumaImageDataInfo.mpFmtBuffer = GetPumaImageBuf(32);
            index = sizeof(scSpiWriteCmd) - 1;
            memcpy(sPumaImageDataInfo.mpFmtBuffer, scSpiWriteCmd, index);
            sPumaImageDataInfo.mpFmtBuffer[index++] = '4';
            sPumaImageDataInfo.mpFmtBuffer[index++] = 0x20;
            sPumaImageDataInfo.mpFmtBuffer[index++] = 'D';
            sPumaImageDataInfo.mpFmtBuffer[index++] = '8';
            sPumaImageDataInfo.mpFmtBuffer[index++] = 0x20;

            offset = index;
            for (i=3; i>0; i--)
            {
                sPumaImageDataInfo.mpFmtBuffer[offset++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8 + 4))]; 
                sPumaImageDataInfo.mpFmtBuffer[offset++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8))]; 
                sPumaImageDataInfo.mpFmtBuffer[offset++] = 0x20;
            }
            sPumaImageDataInfo.mpFmtBuffer[offset++] = '\r';
            sPumaImageDataInfo.mpFmtBuffer[offset++] = 0;

            a_address += FLASH_SECTOR_SIZE;
            sPumaImageDataInfo.mFmtBufferSize = strlen(sPumaImageDataInfo.mpFmtBuffer);
            ///mvPumaEraseImageDataInfo.push_back(sPumaImageDataInfo);
            veraseImageDataInfos.push_back(sPumaImageDataInfo);
        }
    }

    void CCJGW_PumaImageFmtData::GetFlashReadDataCmd(char* szCmd,unsigned int a_address, unsigned int a_length)
    {
        int i = 0, index = 0;
        int nbrOfLine = (a_length/16) + ((a_length % 16) ? 1 : 0);
        int rcvLength = (nbrOfLine * 2) + ((a_length * 3) - nbrOfLine) + 3;

        memcpy(szCmd, scSpiReadCmd, sizeof(scSpiReadCmd));    // spi command for read 
        index = sizeof(scSpiReadCmd) - 1;
        szCmd[index++] = '4';     // count of read command byte to follow
        szCmd[index++] = ' ';
        szCmd[index++] = OPCODE_READ_DATA;
        szCmd[index++] = ' ';

        // calculate and format the count of SPI data
        for (i=3; i > 0; i--)
        {
            szCmd[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8 + 4))]; 
            szCmd[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8))]; 
            szCmd[index++] = 0x20;
        }

        szCmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 12)]; 
        szCmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 8)]; 
        szCmd[index++] = scTblHex2Ascii[0x0f & (a_length >> 4)]; 
        szCmd[index++] = scTblHex2Ascii[0x0f & (a_length)]; 
        szCmd[index++] = '\r';
        szCmd[index++] = 0;
    }

    int CCJGW_PumaImageFmtData::FormatSendBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length)
    {
        int i, index;
        memcpy(a_pBuf, scSpiWriteCmd, sizeof(scSpiWriteCmd));    // spi command for page program (2)
        index = sizeof(scSpiWriteCmd) - 1;

        // calculate and format the count of SPI data
        i = a_length + 4;   // opcode(1) + address(3) + a_length
        a_pBuf[index++] = scTblHex2Ascii[0x0f & (i >> 8)];
        a_pBuf[index++] = scTblHex2Ascii[0x0f & (i >> 4)];
        a_pBuf[index++] = scTblHex2Ascii[0x0f & i];
        a_pBuf[index++] = 0x20;
        a_pBuf[index++] = OPCODE_PAGE_PROGRAM;
        a_pBuf[index++] = 0x20;

        for (i=3; i>0; i--)
        {
            a_pBuf[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8 + 4))]; 
            a_pBuf[index++] = scTblHex2Ascii[0x0f & (a_address >> ((i-1)*8))]; 
            a_pBuf[index++] = 0x20;
        }

        for (i=0; i<a_length; i++)
        {
            a_pBuf[index++] = scTblHex2Ascii[0x0f & (*a_pData >> 4)];
            a_pBuf[index++] = scTblHex2Ascii[0x0f & (*a_pData++)];
            a_pBuf[index++] = 0x20;
        }

        a_pBuf[index++] = '\r';
        a_pBuf[index++] = '\0';
        return index;
    }

    int CCJGW_PumaImageFmtData::FormatRecvBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length)
    {
        int index = 0,i = 1;
        a_pBuf[index++] = 0x0A;
        a_pBuf[index++] = 0x0D;

        for (; i <= a_length; i++)
        {
            a_pBuf[index++] = scTblHex2Ascii[0x0f & (*a_pData >> 4)];
            a_pBuf[index++] = scTblHex2Ascii[0x0f & (*a_pData++)];
            if (0 == i%16)
            {
                a_pBuf[index++] = 0x0A;
                a_pBuf[index++] = 0x0D;
            }
            else
            {
                a_pBuf[index++] = 0x20;
            }
        }
        //! 如果不能整除 16 则在删除0x20
        if (0 != (i - 1) % 16)
        {
            a_pBuf[index - 1] = 0x0A; //! 删除0x20末尾标记
            a_pBuf[index++] = 0x0D;
        }
        a_pBuf[index ++] = RESP_TERMINATOR;
        a_pBuf[index] = '\0';
        return index;
    }
}