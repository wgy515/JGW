#include "StdAfx.h"
#include "TSE_FTM_CheckMemoryCard.h"
#include "CJGW_FTM_MemoryCardResponceVoJson.h"
namespace JGW
{
    CTSE_FTM_CheckMemoryCard::CTSE_FTM_CheckMemoryCard(void) : mInternalSharedStorageGB(0)
        ,mTFlashStorageGB(0)
    {
    }


    CTSE_FTM_CheckMemoryCard::~CTSE_FTM_CheckMemoryCard(void)
    {
    }

    const wchar_t* CTSE_FTM_CheckMemoryCard::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTM_CheckMemoryCard\",\
                \"TestName\": \"内存信息检测(LYNX FTM)\",\
                \"InternalSharedStorage\": \"内部存储大小(单位GB)为0 不测试\",\
                \"TFlashStorage\": \"T卡存储大小(单位GB) 为0 不测试\",\
                \"TimeOutSec\": \"通信超时时间设置 以S为单位，默认10S\"\
                }";
    }

    bool CTSE_FTM_CheckMemoryCard::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_FTM_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"InternalSharedStorage"))
        {
            mInternalSharedStorageGB = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TFlashStorage"))
        {
            mTFlashStorageGB = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FTM_CheckMemoryCard::TSE_Run()
    {
        std::string strJsonCommand = CTSE_FTM_JSONCommand::GetMemoryJson();
        CCJGW_FTM_Json ftmJson;
        if (!CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(strJsonCommand,E_REQ_MEMORY_CARD_CODE,ftmJson,mTimeOutSec))
        {
            return false;
        }

        CCJGW_FTM_MemoryCardResponceVoJson memroyInfo;
        if (!memroyInfo.FromJosn(ftmJson)) return false;

        if (0 != mInternalSharedStorageGB)
        {
            if (memroyInfo.mvsMemoryCardInfo.empty() || memroyInfo.mvsMemoryCardInfo.at(0).totalBytes/1024/1024/1024 <= 0)
            {
                LogE("未读取到内部存储信息\n");
                return false;
            }
        }

        if (0 == mTFlashStorageGB) return true;

        if (2 > memroyInfo.mvsMemoryCardInfo.size())
        {
            LogE("未读取到T-FLASH信息\n");
            return false;
        }
        /*size_t flashGB = memroyInfo.mvsMemoryCardInfo.at(1).totalBytes/1024/1024/1024;

        if (flashGB != mTFlashStorageGB)
        {
        LogE_F("读取到的T-FLASH大小为: %d,小于配置中的flash大小(%d)\n",flashGB,mTFlashStorageGB);
        return false;
        }*/
        return true;
    }
}
