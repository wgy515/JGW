#include "stdafx.h"
#include <string>
#include <sstream>
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <poco/Net/FilePartSource.h>
#include "Poco/StreamCopier.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URI.h"

#include <JGW_SSHPlugin/CJGW_SSHSCP.h>

std::string gulp(std::istream &in)
{
    std::string ret;
    char buffer[4096];
    while (in.read(buffer, sizeof(buffer)))
        ret.append(buffer, sizeof(buffer));
    ret.append(buffer, (size_t)in.gcount());
    return ret;
}

int _tmain(int argc, _TCHAR* argv[])
{










    return -1;

    std::string strUrl = "http://mmwlogin.net/cgi-bin/upload.cgi",strRead;
    try
    {
        Poco::URI url(strUrl);
        Poco::Net::HTTPClientSession session(url.getHost(),url.getPort());
        Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST,url.getPath(),Poco::Net::HTTPRequest::HTTP_1_1);

        //request.setChunkedTransferEncoding(false);
        request.setTransferEncoding(Poco::Net::HTTPMessage::IDENTITY_TRANSFER_ENCODING);
        request.set("upload-type", "100");
        /*request.set("Expect","100-continue");*/
       //! request.setContentType("application/octet-stream");

        Poco::Net::HTMLForm form;
        form.setEncoding(Poco::Net::HTMLForm::ENCODING_MULTIPART);
        //! form.set("upload-type", "100");
       // form.set("Content-Type","application//octet-stream");
        form.addPart("sendfile", new Poco::Net::FilePartSource("C://01_19_08_192.168.133.65_to_192.168.133.36.log"));
        form.prepareSubmit(request);
        request.setTransferEncoding(Poco::Net::HTTPMessage::IDENTITY_TRANSFER_ENCODING);

        std::ostringstream ostr;
        form.write(ostr);
        std::string s = ostr.str();

        request.setContentLength(s.length());
        session.sendRequest(request) << s;
//         session.setTimeout(Poco::Timespan(20, 0));
//         form.write(session.sendRequest(request));        

//         Poco::Net::HTTPResponse res;
//         std::istream &is = httpSession->receiveResponse(res);
//         Poco::StreamCopier::copyStream(is, std::cout);

        //         request.setContentLength(mpsStationMonitorUiInfo->mstrStationStatusJson.length());
        //! 如果非空则绑定源地址
        //         if (!strSourceAddr.empty())
        //         {
        //             sockaddr_in sin;
        //             sin.sin_family = AF_INET;
        //             sin.sin_port = 0;
        //             sin.sin_addr.S_un.S_addr = inet_addr(strSourceAddr.c_str());
        //             Poco::Net::SocketAddress sourceAddress((LPSOCKADDR)&sin, sizeof(sin));
        //             session.socket().impl()->bind(sourceAddress);
        //             session.setKeepAlive(true);
        //             Poco::Timespan time(60*60,0);
        //             session.setKeepAliveTimeout(time);
        // 
        //             Poco::Net::SocketAddress addr(url.getHost(),url.getPort());
        //             session.socket().connect(addr,60000000);
        //             session.socket().setReceiveTimeout(60000000);
        //             session.socket().setNoDelay(true);
        //         }
        // session.sendRequest(request) << mpsStationMonitorUiInfo->mstrStationStatusJson;

        Poco::Net::HTTPResponse resp;
        std::istream & is = session.receiveResponse(resp);
        strRead = gulp(is);

        //Log4AI_F("w: %s",mpsStationMonitorUiInfo->mstrStationStatusJson.c_str());

        return true;
    }
    catch(Poco::Net::NetException & ex)
    {
        /*Log4AE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); */
    }
    catch(Poco::Exception& ex)
    {
        //Log4AE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); 
    }
}