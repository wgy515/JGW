#include "StdAfx.h"
#include "TSE_XWGaugeAsyncSocket.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_XWGaugeAsyncSocket::CTSE_XWGaugeAsyncSocket(void) : mstrConnectAsyncSocketInstanceEnvironment(L"TSE_ASYNC_SOCKET"),mnCheckResultTimeOutSec(30)
    {
    }


    CTSE_XWGaugeAsyncSocket::~CTSE_XWGaugeAsyncSocket(void)
    {
    }

    const wchar_t* CTSE_XWGaugeAsyncSocket::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_XWGaugeAsyncSocket\",\
                \"TestName\": \"异步SOCKET断开\",\
                \"ConnectAsyncSocketInstanceEnvironment\": \"保存连接SOCKET实例变量名称\"\
                }";
    }

    bool CTSE_XWGaugeAsyncSocket::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ConnectAsyncSocketInstanceEnvironment"))
        {
            mstrConnectAsyncSocketInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckResultTimeOutSec"))
        {
            mnCheckResultTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_XWGaugeAsyncSocket::TSE_Init()
    {
        return true;
    }

    bool CTSE_XWGaugeAsyncSocket::TSE_Run()
    {
        if (mstrConnectAsyncSocketInstanceEnvironment.empty())
        {
            LogE(L"Error : Socket Instance Config Environment Name is empty");
            return false;
        }

        CCJGW_AsyncSocket* pAsyncSocket = (CCJGW_AsyncSocket*)(GetGlobalEnvironment()->GetInt(mstrConnectAsyncSocketInstanceEnvironment));
        if (NULL == pAsyncSocket)
        {
            LogE(L"Error : Socket Instance is empty");
            return false;
        }
        /*
        CONF INIT
        -1|0|OK
        CONF READ 0
        -1|0|OK
        START READ
        -1|0|OK
        0|13|连接测试仪表失败!
        -1|15|STOP == 当前测试完成


		2025-03-17 10:17:40 ERROR - Test Failure : 0|10|PASS
		2025-03-17 10:17:40 ERROR - Test Failure : -1|15|STOP
		2025-03-17 10:17:40 ERROR - Test Failure : -1|15|STOP
        */
        char szBuf[1024] = {0};
        jgw_timer jt;
        //! 10 测试PASS
        std::vector<std::string> vstrReadResult;
        while (jt.elapsed() <= mnCheckResultTimeOutSec)
        {
            memset(szBuf,0x00,1024);
            if (pAsyncSocket->AsyncReadSocket(szBuf,1024))
            {
                vstrReadResult.push_back(szBuf);
                if (JGW_StrCaseStr(szBuf,"-1|15|"))
                {
                    break;
                }
#if 0
                vstrReadResult.empty();
                JGW_ParserStrA(szBuf,"|",vstrReadResult);
                if (vstrReadResult.size() > 2)
                {
                    int nResulCode = atoi(vstrReadResult[1].c_str());

                    if (E_XWGAUGE_ALL_TEST_PASS == nResulCode)
                    {

                    }

                    if (E_XWGAUGE_TEST_COMPLETE == nResulCode)
                    {
                        return true;
                    }
                }  
#endif
            }
        }

        for (std::vector<std::string>::iterator it = vstrReadResult.begin();it != vstrReadResult.end();++it)
        {
			if (0 == JGW_StrCaseStr(it->c_str(),"|")) continue;
            std::vector<std::string> vstrTemp;
            JGW_ParserStrA(it->c_str(),"|",vstrTemp);
			if (vstrTemp.size() < 2) continue;

            int nResulCode = atoi(vstrTemp[1].c_str());
            if (E_XWGAUGE_TEST_COMPLETE == nResulCode) continue;

            if (E_XWGAUGE_ALL_TEST_PASS == nResulCode)
            {
                LogI_F("Test Successful : %s",it->c_str());
                return true;
            }
            else
            {
                LogE_F("Test Failure : %s",it->c_str());
            }
        }
        return false;
    }
}

