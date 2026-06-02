#pragma once
#include <string>
#include "TSE_GlobalEnvironmentImpl.h"
#include "TSE_TestStatusListener.h"
#include "TSE_LogListener.h"
#include "TSE_SequenceTestImpl.h"
#define INVALID_TOKEN 0xFFFFFF
/*Interface*/
namespace JGW
{
    class CTSE_SequenceManageImpl
    {
    public:
        virtual ~CTSE_SequenceManageImpl(){}
        //! Get Token ID
        virtual unsigned long GetTokenID() = 0;
        //! 设置Test Status 监听
        virtual void SetOnTestStatusListener(CTSE_TestStatusListener* pListener) = 0;
        //! 移除Test Status 监听
        virtual void RemoveOnTestStatusListener(CTSE_TestStatusListener* pListener) = 0;
        //! 设置log 监听
        virtual void SetOnLogListener(CTSE_LogListener* pListener) = 0;
        //! 移除log 监听
        virtual void RemoveOnLogListener(CTSE_LogListener* pListener) = 0;
        //! 获取环境变量接口
        virtual CTSE_GlobalEnvironmentImpl* GetGlobalEnvironmentImpl() = 0;
        //! 获取测试类接口
        virtual CTSE_SequenceTestImpl* GetSequenceTestImpl() = 0;
    };
}