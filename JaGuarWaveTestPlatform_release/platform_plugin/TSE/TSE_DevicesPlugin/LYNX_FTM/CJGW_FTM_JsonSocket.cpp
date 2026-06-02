#include "StdAfx.h"
#include "CJGW_FTM_JsonSocket.h"
#include "..\TSE_DevicesGlobalResource.h"
#include "TSE_FTM_Define.h"
#include <TSE_FoundationLib/TSE_FoundationLib_Define.h>

namespace JGW
{
    bool CCJGW_FTM_JsonSocket::WriteJsonSocket(const std::string& strWriteJson,size_t timeOutSec /* = 10 */,int retryCount /* = 3 */)
    {
        LogD_F("%s\n",strWriteJson.c_str());
        for (int i = 0;i < retryCount;i ++)
        {
            if(CTSE_DevicesGlobalResource::GetFTMClientSocket()->AsyncWriterSocket(strWriteJson,timeOutSec))
            return true;
        }
        
        LogE_F("Send Json Command(%s) Fail\n",strWriteJson.c_str());
        return false;
    }

    bool CCJGW_FTM_JsonSocket::ReadJsonSocketToWriteSocket(const std::string& strWriteJson,size_t nReqCode,CCJGW_FTM_Json& ftmJson,size_t timeOutSec /* = 10 */,int retryCount /* = 3 */)
    {
        char szReadBuf[TSE_FTM_MAX_READ_BUF] = {0};
        std::stringstream msg_ss;
        int nRsponseCode = -1;
        //! 写入的JSON为空则直接读
        if (!WriteJsonSocket(strWriteJson,timeOutSec,retryCount)) return false;

        for (int i = 0;i < retryCount;i ++)
        {
            if (!CTSE_DevicesGlobalResource::GetFTMClientSocket()->AsyncReadSocket(szReadBuf,TSE_FTM_MAX_READ_BUF,timeOutSec))
            {
                LogE_F("Recv Json Fail\n");
                continue;
            }
            if (!ftmJson.FromJsonToString(szReadBuf)) {LogD_F("ReadSocket:%s\n",szReadBuf);continue;}
            //! log print
            for(boost::property_tree::ptree::iterator msg_it = ftmJson.GetJsonTree().begin(); msg_it != ftmJson.GetJsonTree().end(); ++msg_it)
            {
                LogI_F("%s : %s\n",msg_it->first.c_str(),ftmJson.GetJsonTree().get<std::string>(msg_it->first).c_str());
            }
            //! 判断请求code和响应code是否一致
            if (!ftmJson.GetJsonValueToKey<int>("reqCode",nRsponseCode)) continue;
            if (nRsponseCode == nReqCode) return true;
            i --;
        }
        return false;
    }
}