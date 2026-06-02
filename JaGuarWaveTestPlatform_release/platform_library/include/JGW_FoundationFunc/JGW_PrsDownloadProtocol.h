#pragma once
#include <vector>
#include <map>
#include "JGW_FoundationFuncLib_Define.h"
#include "PrsCommonDefs.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    extern "C" 
    {
#endif
        /// 获取PRS 读取命令,命令缓冲区长度32 
        JGW_FOUNDATIONFUNC_LIB_API void JGW_GetPrsFlashReadDataCmd(char* szCmd,unsigned int a_address, unsigned int a_length);
        /// 
        JGW_FOUNDATIONFUNC_LIB_API int JGW_PrsUnformatReceiveBuffer(char* a_pBuf, char* a_pData, int a_length);
        /// 
        JGW_FOUNDATIONFUNC_LIB_API int JGW_PrsFormatSendBuffer(char *a_pBuf, int a_address, char *a_pData, int a_length);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_PrsSetSerailNumberMacAddressToOtpFile(unsigned char* szFileBuf,size_t fileSize,const std::string& serailNumber,const std::string& macAddress);
        //! 大端转小端 INT
        JGW_FOUNDATIONFUNC_LIB_API UINT32 FormattedLittleEndianInt(const unsigned char* szBuf);
        //! 计算CRC
        JGW_FOUNDATIONFUNC_LIB_API UINT32 JGW_PrsCalcChecksum32ToImageBuf(const unsigned char* szBuf,size_t buf_len);
        // 生成CRC,并将生成的CRC保存到szImageBuf
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_PrsGeneratedCRC32ToImageBuf(unsigned char* szImageBuf,size_t buf_len);

#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    }
#endif
}