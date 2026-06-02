// TSEUDPServerDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UDPEchoServer.h"
#include <tinyxml/tinyxml.h>

int _tmain(int argc, _TCHAR* argv[])
{
#if 0
    UDPEchoServer echoServer(Poco::Net::SocketAddress("localhost", 3003));
    Sleep(22000 * 1000);
#else
#if 0
    TiXmlDocument doc;  
    TiXmlElement* msg;
    TiXmlDeclaration* decl = new TiXmlDeclaration();  
    doc.LinkEndChild( decl );   
    TiXmlElement * root = new TiXmlElement( "MyApp" );  
    doc.LinkEndChild( root );  
    TiXmlComment * comment = new TiXmlComment();
    comment->SetValue(" Settings for MyApp " );  
    root->LinkEndChild( comment );   
    TiXmlElement * msgs = new TiXmlElement( "Messages" );  
    root->LinkEndChild( msgs ); 
    doc.SaveFile( "d:\\123456.xml" );  
#else
    std::string strSuiteConfigPath = "d:\\123456.xml";
    std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
    TiXmlElement* pRoot,*pCommonNode,*pConfigNode = NULL;
    if (!ptrXmlDoc->LoadFile(strSuiteConfigPath.c_str())) return 0;

    pRoot = ptrXmlDoc->RootElement();
    if (!pRoot) return 0;

    SetElementTextValue(pRoot,"Messages11", L"Thank you for using MyApp");

    //pCommonNode = pRoot->FirstChildElement("Messages");
    //pCommonNode->LinkEndChild(new TiXmlText( "Thank you for using MyApp" ));  
    //pCommonNode->SetValue("123");
    ptrXmlDoc->SaveFile(strSuiteConfigPath);

#endif
#endif

    //echoServer.run();
	return 0;
}

