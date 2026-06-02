#pragma once
#include "TSE_FTM_Define.h"
#include "CJGW_FTM_Json.h"
namespace JGW
{
    class CCJGW_FTM_SocketResponceVo
    {
    public:
        CCJGW_FTM_SocketResponceVo();
        bool FromJosn(const std::string& strJson);
        bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        //! 请求code
        int mReqCode;    
        //! 业务处理逻辑中的Code 1  : OK -1 : failed      
        int mResultCode;
        //! 业务处理逻辑结果
        std::string mstrResult;
        //! 业务处理逻辑失败后的提示消息
        std::string mResponseErrorMsg;
    };
}