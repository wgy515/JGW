#include "StdAfx.h"
#include <JGW_FoundationFunc/JGW_PrsDownloadProtocol.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{

    void JGW_GetPrsFlashReadDataCmd(char* szCmd,unsigned int a_address, unsigned int a_length)
    {
        int i = 0, index = 0;
        //int nbrOfLine = (a_length/16) + ((a_length % 16) ? 1 : 0);
        //int rcvLength = (nbrOfLine * 2) + ((a_length * 3) - nbrOfLine) + 3;

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

    int JGW_PrsUnformatReceiveBuffer(char* a_pBuf, char* a_pData, int a_length)
    {
        int i = 0;
        unsigned char data, byteHi, byteLo;
        unsigned char * pData;

        // The loops repacks ASCII data string from Falcon-B bootloader into hex string
        // The data returns from Falcon-B bootloader has a specific pattern:
        // [\n\r,[byte1,byte2,space]x15,[byte1,byte2]]x N_lines, ends with \n\r>
        while (a_length)
        {
            int length = a_length > 16 ? 16 : a_length;
            pData = (unsigned char *)(a_pData + 2);
            a_pData += (2 + (length * 3)-1);     // move buffer head to next line
            a_length -= length;

            // pack 2 ASCII data to 1 hex value
            for (i = 1; i <= length; i++)
            {
                data = *pData++;
                byteHi = (data < 'A') ? (data - '0') : (data - 'A' + 0xa);
                data = *pData++;
                byteLo = (data < 'A') ? (data - '0') : (data - 'A' + 0xa);        
                *a_pBuf++ = (0xf0 & (byteHi << 4)) | (0x0f & (byteLo));
                pData++;  // skip space
            }
        }
        return i;
    }

    int JGW_PrsFormatSendBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length)
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

    bool JGW_PrsSetSerailNumberMacAddressToOtpFile(unsigned char* szFileBuf,size_t fileSize,const std::string& serailNumber,const std::string& macAddress)
    {
        std::string strMac(macAddress);

        JGW_ReplaceStringA(strMac,":","");
        if (!strMac.empty() && MAC_ADDRESS_LENGTH != strMac.length() && SERIALNUMBER_MAX_BITS / 2 > serailNumber.length())
        {
            return false;
        }
        unsigned char buf = '\0';
        char szTemp[3] = {0};
        for (size_t i = 0;i < fileSize - 13 - SERIALNUMBER_MAX_BITS;i ++)
        {
            if (szFileBuf[i] == 0x60 
                && szFileBuf[i + 1] == 0x01
                && szFileBuf[i + 2] == 0x01
                && szFileBuf[i + 3] == 0x01
                && szFileBuf[i + 4] == 0x06)
            {
                if (!macAddress.empty())
                {
                    unsigned char szMacAddr[14] = {0};
                    sscanf_s(strMac.c_str(),"%02X%02X%02X%02X%02X%02X",&szMacAddr[0],&szMacAddr[1],&szMacAddr[2],&szMacAddr[3],&szMacAddr[4],&szMacAddr[5]);
                    for (size_t index = 0;index < 6;index ++)
                    {
                        buf = szFileBuf[i + 5 + index];
                        szFileBuf[i + 5 + index] = szMacAddr[index];
                        buf = szFileBuf[i + 5 + index];
                    }
                }

                if (!serailNumber.empty())
                {
                    if (szFileBuf[i + 4 + 6 + 1] == 0x02 && szFileBuf[i + 12] == 0x80)
                    {
                        //! 清空
                        for (int index = 0;index < SERIALNUMBER_MAX_BITS;index ++)
                        {
                            szFileBuf[i + 13 + index] = '\0';
                        }
                        /// 
                        for (size_t index = 0;index < serailNumber.length();index ++)
                        {
                            buf = szFileBuf[i + 13 + index * 2];
                            szFileBuf[i + 13 + index * 2] = serailNumber[index];
                            buf = szFileBuf[i + 13 + index * 2];
                        }
                        return JGW_PrsGeneratedCRC32ToImageBuf(szFileBuf,fileSize);
                    }
                    return false;
                }
            }
        }
        return false;
    }

    UINT32 FormattedLittleEndianInt(const unsigned char* szBuf)
    {
        UINT32 word = 0;
        unsigned char* szBufUINT32 = (unsigned char *)&word;
        memcpy(szBufUINT32,szBuf,4);
        return word;
    }

    UINT32 JGW_PrsCalcChecksum32ToImageBuf(const unsigned char* szBuf,size_t buf_len) 
    {
        UINT64 sum = 0;
        size_t pos = 0;
        if (buf_len % 4 ) return 0;

        while (pos < buf_len)
        {
            sum += FormattedLittleEndianInt(szBuf + pos);
            pos += 4;
        }

        while (sum >> 32)
            sum = (sum & MASK_32BIT) + (sum >> 32);

        return ~sum & MASK_32BIT;
    }

    bool JGW_PrsGeneratedCRC32ToImageBuf(unsigned char* szImageBuf,size_t buf_len)
    {
        //! 计算当前文件缓冲区CRC 0xE11F1735
        UINT32 ucrc = JGW_PrsCalcChecksum32ToImageBuf(szImageBuf,buf_len - 4);
        if (0 == ucrc) return false;
        char szBuffer[9] = {0};
        unsigned char szCRC[10] = {0};

        sprintf_s(szBuffer,9,"%08X",ucrc);
        sscanf_s(szBuffer,"%02X%02X%02X%02X",&szCRC[0],&szCRC[1],&szCRC[2],&szCRC[3]);
        //! 后四位为CRC值，按照小端存储
        szImageBuf[buf_len - 4] = szCRC[3];
        szImageBuf[buf_len - 3] = szCRC[2];
        szImageBuf[buf_len - 2] = szCRC[1];
        szImageBuf[buf_len - 1] = szCRC[0];
        printf("%02X%02X%02X%02X",szCRC[3],szCRC[2],szCRC[1],szCRC[0]);

        return true;
    }
}