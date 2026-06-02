#include "StdAfx.h"
#include "TSE_WriteNumberToPeraso.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_WriteNumberToPeraso::CTSE_WriteNumberToPeraso(void) : mbWriteOptBin(false),meNubmerType(E_SN_PERASO),mbInitPDBInfo(false)
    {
    }


    CTSE_WriteNumberToPeraso::~CTSE_WriteNumberToPeraso(void)
    {
    }

    const wchar_t* CTSE_WriteNumberToPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WriteNumberToPeraso\",\
                \"TestName\": \"写Peraso号段(Peraso)\",\
                \"InitPDBInfo\": \"初始化PDB信息每个测试序列必须初始化一次\",\
                \"WriteOptBin\": \"是否写PDB OTP BIN\",\
                \"NumberType\": \"号段类型E_SN_PERASO:0,E_RSN_MAC_PERASO:1,E_MAC_BUFFER:3 => 写MAC BUF\",\
                \"Environment\": \"写入号段的环境变量\"\
                }";
    }

    bool CTSE_WriteNumberToPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"WriteOptBin"))
        {
            mbWriteOptBin = _ttoi(strParamValue)?true:false;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"NumberType",strParamName))
        {
            meNubmerType = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"InitPDBInfo",strParamName))
        {
            mbInitPDBInfo = _ttoi(strParamValue)?true:false;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Environment"))
        {
            mstrEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WriteNumberToPeraso::TSE_Run()
    {
        if (meNubmerType >= E_UNKNOWN_NUMBER) { LogE(L"未知号段类型写入\n");return false;}
        //! 
        if (mbInitPDBInfo && !CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->InitPDBInfoToDongle())
        {
            LogE(L"Init PDB Info To Dongles Fail\n");
            return false;
        }

        std::wstring strTemp = GetGlobalEnvironment()->GetString(mstrEnvironment);
        std::string strSN,strMac;
        LogI_F(L"Write %s: %s\n",gszPerasoNumberName[meNubmerType],strTemp.c_str());
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
        switch (meNubmerType)
        {
        case E_SN_PERASO:
            CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->ChangeSNToImageFileBuf(JGW_W2A(strTemp).c_str());
            break;
        case E_RSN_MAC_PERASO:
            //! 从Dongle中读取SN号段，防止将以前写入的SN号段覆盖掉
            if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->ReadSNMacToDongleFlash(strSN,strMac)) { LogE(L"Read Serail Number Fail\n"); return false;}
            CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->ChangeSNToImageFileBuf(strSN.c_str());
            CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->ChangeMacToImageFileBuf(JGW_W2A(strTemp).c_str());
            break;
        case E_MAC_BUFFER:
            CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->ChangeMacToImageFileBuf(JGW_W2A(strTemp).c_str());
            break;
        }
        if (!mbWriteOptBin) return true;

        if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->BuildOTPCRC32())
        {
            LogE(L"Build PDB CRC Fail\n");
            return false;
        }

        if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->DownloadPDBImageFileToDongleFlash(E_OTP))
        {
            LogE(L"Download OTP Image File Fails\n");
            return false;
        }

        return true;
    }
}