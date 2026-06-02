#include "StdAfx.h"
#include "TSE_PostHttpReqRsp.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>

namespace JGW
{
    CTSE_PostHttpReqRsp::CTSE_PostHttpReqRsp(void) : mstrURL("http://192.168.1.202/cgi-bin/leopard.fcgi"),mstrParam("{\"requestCode\":170,\"fileName\":\"update.zip\"}"),mstrContentType("application/json"),mstrSourceAddrEnvironment(L"TSE_SSH_SOURCE_IP")
    {
    }


    CTSE_PostHttpReqRsp::~CTSE_PostHttpReqRsp(void)
    {
    }

    const wchar_t* CTSE_PostHttpReqRsp::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_PostHttpReqRsp\",\
                \"TestName\": \"发送HTTP Post请求(AP)\",\
                \"SourceAddrEnvironment\": \"发送url路由出口\",\
                \"Url\": \"http url\",\
                \"Param\": \"url请求参数\",\
                \"ParamEnvrionment\": \"url param environment\",\
                \"ContentType\": \"http请求消息类型\",\
                \"SaveReceiveEnvironment\": \"Save Receive Environment\",\
                \"FindResult\": \"比对请求发送后返回的结果\"\
                }";
    }

    bool CTSE_PostHttpReqRsp::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Url",strParamName))
        {
            mstrURL = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Param",strParamName))
        {
            mstrParam = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ParamEnvrionment",strParamName))
        {
            mstrParamEnvrionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ContentType",strParamName))
        {
            mstrContentType = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"FindResult",strParamName))
        {
            mstrFindString = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SourceAddrEnvironment",strParamName))
        {
            mstrSourceAddrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveReceiveEnvironment",strParamName))
        {
            mstrSaveReceiveEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    std::string gulp(std::istream &in)
    {
        std::string ret;
        char buffer[4096];
        while (in.read(buffer, sizeof(buffer)))
            ret.append(buffer, sizeof(buffer));
        ret.append(buffer, (size_t)in.gcount());
        return ret;
    }

    bool CTSE_PostHttpReqRsp::TSE_Run()
    {
        std::string strRead;
        std::string strSourceAddr = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        std::wstring strUrl(JGW_A2W(mstrURL));
        GetEnvironmentCommandString(strUrl);
        try
        {
            Poco::URI url(JGW_W2A(strUrl,CP_UTF8));
            Poco::Net::HTTPClientSession session(url.getHost(),url.getPort());
            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST,url.getPath(),Poco::Net::HTTPRequest::HTTP_1_1);

            request.setChunkedTransferEncoding(false);
            request.setContentType(mstrContentType);
            request.setContentLength(mstrParam.length());
            //! 如果非空则绑定源地址
            if (!strSourceAddr.empty())
            {
                sockaddr_in sin;
                sin.sin_family = AF_INET;
                sin.sin_port = 0;
                sin.sin_addr.S_un.S_addr = inet_addr(strSourceAddr.c_str());
                Poco::Net::SocketAddress sourceAddress((LPSOCKADDR)&sin, sizeof(sin));
                session.socket().impl()->bind(sourceAddress);
                session.setKeepAlive(true);
                Poco::Timespan time(60*60,0);
                session.setKeepAliveTimeout(time);

                Poco::Net::SocketAddress addr(url.getHost(),url.getPort());
                session.socket().connect(addr,60000000);
                session.socket().setReceiveTimeout(60000000);
                session.socket().setNoDelay(true);
            }
            if (mstrParamEnvrionment.empty())
            {
                session.sendRequest(request) << mstrParam;
            }
            else
            {
                std::string strParam = GetGlobalEnvironment()->GetAnsiString(mstrParamEnvrionment);
                LogI_F("Param:%s",strParam.c_str());
                session.sendRequest(request) << strParam;
            }
            
            if (mstrFindString.empty())
            {
                return true;
            }
            Poco::Net::HTTPResponse resp;
            std::istream & is = session.receiveResponse(resp);
            strRead = gulp(is);
        }
        catch(Poco::Net::NetException & ex)
        {
            LogE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); 
            return false;
        }
        catch(Poco::Exception& ex)
        {
            LogE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); 
            return false;
        }

        if (NULL == JGW_StrCaseStr(strRead.c_str(),mstrFindString.c_str()))
        {
            LogE_F("result : %s , expected results : %s\n",strRead.c_str(),mstrFindString.c_str());
            return false;
        }

        LogI_F("HTTP Post : %s\n" ,strRead.c_str());
        if (!mstrSaveReceiveEnvironment.empty())
        {
            GetGlobalEnvironment()->PutAnsiString(mstrSaveReceiveEnvironment,strRead);
        }

        return true;
    }
}