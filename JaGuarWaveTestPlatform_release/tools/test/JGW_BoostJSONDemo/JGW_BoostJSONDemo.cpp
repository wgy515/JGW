// JGW_BoostJSONDemo.cpp : 定义控制台应用程序的入口点。
//
#include <stdafx.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "CJGW_FTM_ClientSocket.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_MESDatabaseJsonCmd.h"
#include <time.h>
#include <boost/uuid/sha1.hpp>  
#include <iostream>  
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <ShellAPI.h>

#include <regex>
using namespace std;
using namespace boost::property_tree;


const string file_path = "c:\\test.txt";

void write_json_data_into_file(void)
{
    printf("%s\n","write json data");

    boost::property_tree::ptree root, items;

    boost::property_tree::ptree item1;
    item1.put("ID","1");
    item1.put("Name","wang");
    items.push_back(std::make_pair("1",item1));

    boost::property_tree::ptree item2;
    item2.put("ID","2");
    item2.put("Name","zhang");
    items.push_back(std::make_pair("2",item2));

    root.put_child("users",items);
    boost::property_tree::write_json(file_path,root);
}

void read_json_data_from_file(void)
{
    printf("%s\n","read json data");
    boost::property_tree::ptree root;
    boost::property_tree::ptree items;
    boost::property_tree::read_json<boost::property_tree::ptree>(file_path,root);

    items = root.get_child("users");
    for(boost::property_tree::ptree::iterator it=items.begin(); it != items.end(); ++it)
    {
        string key=it->first;//key ID
        string ID=it->second.get<string>("ID");
        string Name=it->second.get<string>("Name");
        cout<<"key: "<<key.c_str()<<'\t';
        printf("ID: %s    Name: %s",ID.c_str(), Name.c_str());
        cout<<endl;
        cout<<"ID:"<<ID<<'\t'<<"Name:"<<Name<<endl;
    }
    cout<<"success"<<endl;
}

void write_json_data_into_string(void)
{
    boost::property_tree::ptree item;
    item.put("a","2");
    std::stringstream is;
    boost::property_tree::write_json(is,item);
    std::string s = is.str();
    cout<<"json s:"<<s<<endl;
}

void read_json_data_from_string(void)
{
    /*
    C++ 中 字符串形式的json串 需要使用 \ 转义 双引号
    */
    std::string str_json = "{\"count\":10,\"people\":[{ \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" },{ \"firstName\": \"Jason\", \"lastName\":\"Hunter\", \"email\": \"bbbb\"},{ \"firstName\": \"Elliotte\", \"lastName\":\"Harold\", \"email\": \"cccc\" }]}";
    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);
    root.put("upid","001");

    // 插入一个数组
    boost::property_tree::ptree exif_array;
    boost::property_tree::ptree array1, array2, array3;
    array1.put("Make", "NIKON");
    array2.put("DateTime", "2011:05:31 06:47:09");
    array3.put("Software", "Ver.1.01");

    //   exif_array.push_back(std::make_pair("Make", "NIKON"));
    //   exif_array.push_back(std::make_pair("DateTime", "2011:05:31 06:47:09"));
    //   exif_array.push_back(std::make_pair("Software", "Ver.1.01"));

    exif_array.push_back(std::make_pair("", array1));
    exif_array.push_back(std::make_pair("", array2));
    exif_array.push_back(std::make_pair("", array3));

    root.put_child("exifs", exif_array);
    std::stringstream str_stream_temp;
    boost::property_tree::write_json(str_stream_temp, root);
    //write_json(str_stream_temp, root);
    std::string str = str_stream_temp.str();
    cout<<str<<endl;
}


void testMsg(void)
{
    string str_json = "{\"TEST\":\"\",\"MSG\":\"130\",\"TKT\":[{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]}]}";
    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);
    boost::property_tree::ptree::iterator root_it = root.begin();
    for(; root_it != root.end(); ++root_it)
    {
        string key = root_it->first;
        if("TKT" == key)
        {
            boost::property_tree::ptree tkt_node = root.get_child(key);
            boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
            for(; tkt_node_it != tkt_node.end(); ++tkt_node_it)
            {
                boost::property_tree::ptree tkt = tkt_node_it->second;
                boost::property_tree::ptree::iterator tkt_it = tkt.begin();
                for(; tkt_it != tkt.end(); ++tkt_it)
                {
                    string tkt_key = tkt_it->first;
                    if("CPN" == tkt_key)
                    {
                        boost::property_tree::ptree cpn_node = tkt.get_child(tkt_key);
                        boost::property_tree::ptree::iterator cpn_node_it = cpn_node.begin();
                        for(; cpn_node_it != cpn_node.end(); ++cpn_node_it)
                        {
                            boost::property_tree::ptree cpn = cpn_node_it->second;
                            boost::property_tree::ptree::iterator cpn_it = cpn.begin();
                            for(; cpn_it != cpn.end();++cpn_it)
                            {
                                string cpn_key = cpn_it->first;
                                string cpn_val = cpn.get<string>(cpn_key);
                                cout<<cpn_key<<":"<<cpn_val<<endl;
                            }
                        }
                    }
                    else
                    {
                        string tkt_val = tkt.get<string>(tkt_key);
                        cout << tkt_key << ":"<<tkt_val<<endl;
                    }
                }
            }
        }
        else
        {
            string val = root.get<string>(key);
            cout << key <<":"<< val <<endl;
            //Sleep(1000);
        }

    }
}

void bianli_json(void)
{
    string json_string="{\"-f\": \"/usr/reservedfff_dir\", \"-s\": \"/usr/reservedddd_dir\"}";

    string str_head;
    string str_node_val;
    boost::property_tree::ptree pt,p1,p2;

    stringstream stream(json_string);
    try
    {
        boost::property_tree::read_json<boost::property_tree::ptree>(stream, pt);
        cout<<"parsing ok\n"<<endl;
        for (boost::property_tree::ptree::iterator ita = pt.begin(); ita != pt.end(); ++ita)
        {
            cout<<"first:"<<ita->first<<endl;
            str_node_val = pt.get<string>(ita->first);
            cout<<str_node_val<<endl;
        }
    }
    catch(std::runtime_error& e)
    {
        std::cout<<e.what()<<endl;
    }
}

void test(void)
{
    string str_json = "{\"TEST\":\"\",\"MSG\":\"130\",\"TKT\":[{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]}]}";
    std::stringstream str_stream(str_json);
    boost::property_tree::ptree root;
    boost::property_tree::read_json(str_stream,root);

    boost::property_tree::ptree tkt = root.get_child("TKT");
    stringstream tkt_stream;
    boost::property_tree::write_json(tkt_stream, tkt);
    cout<<tkt_stream.str()<<endl; //通过打印可以看到 first 为空

    for(boost::property_tree::ptree::iterator it = tkt.begin(); it!=tkt.end();++it)
    {
        boost::property_tree::ptree inner = it->second; //first为空
        //string DTYP_val = inner.get<string>("DTYP");
        //cout << "DTYP" <<":"<< DTYP_val<<endl;

        boost::property_tree::ptree::iterator it_tkt = inner.begin();
        for(; it_tkt != inner.end(); ++it_tkt)
        {
            string innet_key = it_tkt->first;
            cout<<innet_key<<":"<<inner.get<string>(innet_key)<<endl;
        }
        break;
    }

    //    boost::property_tree::ptree pt,pptt,ttt;
    //    string s = "{\"data\":[{\"id\":1,\"name\":\"chen\"},{\"id\":2,\"name\":\"zhang\"}]}";
    //    stringstream stream(s);
    //    read_json(stream, pt);

    //    pptt = pt.get_child("data");
    //    for (boost::property_tree::ptree::iterator it = pptt.begin(); it != pptt.end(); ++it)
    //    {
    //        ttt = it->second; //first为空
    //        cout<<"id="<<ttt.get<string>("id")<<endl;
    //        cout<<"name="<<ttt.get<string>("name")<<endl;
    //    }
}

void parseMsg_1(void)
{
    string msg_str = "{\"MSG\":\"130\",\"CRSC\":\"MU\",\"CRSL\":\"BJS\",\"TAID\":\"08692057\",\"IHID\":\"PEK112\",\"ORGT\":\"A\",\"CRSN\":\"9995\",\"IHIB\":\"PEK112\",\"IDTP\":\"B\",\"PNR1\":\"NYFY37\",\"PNR2\":\"MU\",\"SURN\":\"zhang\",\"GIVN\":\"san\",\"FMID\":\"PPG19739941\",\"DTIS\":\"030117\",\"JORG\":\"TPE\",\"JDST\":\"MAD\",\"FAMT\":\"1560.00\",\"FCUR\":\"CNY\",\"TAMT\":\"1660.00\",\"TCUR\":\"CNY\",\"FANF\":\"H/1560.00/CNY+G/00:00/CNY\",\"FOPF\":\"CC\",\"TAXF\":\"T/CNY/50.00/CN+T/CNY/50.00/YQ\",\"FACF\":\"M/09JAN17TSA FM PVG MU JFK/-GRU JJ MAD1560.00CNY1560.00END\",\"TKT\":[{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"},{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]},{\"DTYP\":\"T\",\"STOT\":\"1\",\"SNUM\":\"1\",\"CPN\":[{\"CNBR\":\"1\",\"DDAT\":\"090117\",\"DTME\":\"1205\",\"ADAT\":\"090117\",\"ATME\":\"1340\",\"ORIG\":\"TSA\",\"DEST\":\"PVG\",\"ALC1\":\"FM\",\"FLTN\":\"802\",\"CLAS\":\"Y\",\"FSTN\":\"OK\",\"FBAS\":\"Y\",\"BAGA\":\"20K\"}]}]}";
    stringstream msg_ss(msg_str);
    boost::property_tree::ptree msg;
    boost::property_tree::read_json(msg_ss, msg);
    boost::property_tree::ptree::iterator msg_it = msg.begin();
    for(; msg_it != msg.end(); ++msg_it)
    {
        string msg_key = msg_it->first;
        string val = "";
        if("TKT" == msg_key)
        {
            boost::property_tree::ptree tkt_node = msg.get_child(msg_key);
            boost::property_tree::ptree::iterator tkt_node_it = tkt_node.begin();
            for(; tkt_node_it != tkt_node.end(); ++tkt_node_it)
            {
                boost::property_tree::ptree tkt = tkt_node_it->second;
                boost::property_tree::ptree::iterator tkt_it = tkt.begin();
                for(; tkt_it != tkt.end(); ++tkt_it)
                {
                    string tkt_key = tkt_it->first;
                    if("CPN" == tkt_key)
                    {
                        boost::property_tree::ptree cpn_node = tkt.get_child(tkt_key);
                        boost::property_tree::ptree::iterator cpn_node_it = cpn_node.begin();
                        for(; cpn_node_it != cpn_node.end(); ++cpn_node_it)
                        {
                            boost::property_tree::ptree cpn = cpn_node_it->second;
                            boost::property_tree::ptree::iterator cpn_it = cpn.begin();
                            for(; cpn_it != cpn.end(); ++cpn_it)
                            {
                                string cpn_key = cpn_it->first;
                                val = cpn.get<string>(cpn_key);
                                cout << cpn_key << ":" << val <<endl;
                            }
                        }
                    }
                    else
                    {
                        val = tkt.get<string>(tkt_key);
                        cout<< tkt_key << ":" << val <<endl;
                    }
                }
            }
        }
        else
        {
            val = msg.get<string>(msg_key);
            cout<< msg_key<<":"<<val<<endl;
        }
    }

}

void parseMsg(void)
{
    string msg_str = "{\"MSG\":\"MSG\",\"CRSC\":\"CRSC\",\"CRSL\":\"CRSL\",\"TAID\":\"TAID\",\"IHID\":\"IHID\",\"ORGT\":\"ORGT\",\"CRSN\":\"CRSN\",\"IHIB\":\"IHIB\",\"IDTP\":\"IDTP\",\"PNR1\":\"PNR1\",\"PNR2\":\"PNR2\",\"TIF\":[{\"SURN\":\"SURN\",\"GIVN\":\"GIVN\",\"FMID\":\"FMID\",\"INFP\":\"INFP\",\"UMCH\":\"UMCH\",\"JORG\":\"JORG\",\"JDST\":\"JDST\",\"FAMT\":\"FAMT\",\"FCUR\":\"FCUR\",\"TAMT\":\"TAMT\",\"TCUR\":\"TCUR\",\"FANF\":\"FANF\",\"FOPF\":\"FOPF\",\"TAXF\":\"TAXF\",\"TKT\":[{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]},{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]}]},{\"SURN\":\"SURN\",\"GIVN\":\"GIVN\",\"FMID\":\"FMID\",\"INFP\":\"INFP\",\"UMCH\":\"UMCH\",\"JORG\":\"JORG\",\"JDST\":\"JDST\",\"FAMT\":\"FAMT\",\"FCUR\":\"FCUR\",\"TAMT\":\"TAMT\",\"TCUR\":\"TCUR\",\"FANF\":\"FANF\",\"FOPF\":\"FOPF\",\"TAXF\":\"TAXF\",\"TKT\":[{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]},{\"TKNB\":\"TKNB\",\"STOT\":\"STOT\",\"SNUM\":\"SNUM\",\"CPN\":[{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"},{\"CNBR\":\"CNBR\",\"DDAT\":\"DDAT\",\"DTME\":\"DTME\",\"ADAT\":\"ADAT\",\"ATME\":\"ATME\",\"ORIG\":\"ORIG\",\"DEST\":\"DEST\",\"ALC1\":\"ALC1\",\"FLTN\":\"FLTN\",\"CLAS\":\"CLAS\",\"FSTN\":\"FSTN\",\"FBAS\":\"FBAS\",\"BAGA\":\"BAGA\"}]}]}]}";
    stringstream msg_ss(msg_str);
    ptree msg;
    read_json(msg_ss, msg);
    string val = "";
    for(ptree::iterator msg_it = msg.begin(); msg_it != msg.end(); ++msg_it)
    {
        string msg_key = msg_it->first;
        if("TIF" == msg_key)
        {
            ptree tif_node = msg.get_child(msg_key);
            for(ptree::iterator tif_node_it = tif_node.begin(); tif_node_it != tif_node.end(); ++tif_node_it)
            {
                ptree tif = tif_node_it->second;
                for(ptree::iterator tif_it = tif.begin(); tif_it != tif.end(); ++tif_it)
                {
                    string tif_key = tif_it->first;
                    if("TKT" == tif_key)
                    {
                        ptree tkt_node = tif.get_child(tif_key);
                        for(ptree::iterator tkt_node_it = tkt_node.begin(); tkt_node_it != tkt_node.end(); ++tkt_node_it)
                        {
                            ptree tkt = tkt_node_it->second;
                            for(ptree::iterator tkt_it = tkt.begin(); tkt_it != tkt.end(); ++tkt_it)
                            {
                                string tkt_key = tkt_it->first;
                                if("CPN" == tkt_key)
                                {
                                    ptree cpn_node = tkt.get_child(tkt_key);
                                    for(ptree::iterator cpn_node_it = cpn_node.begin(); cpn_node_it != cpn_node.end(); ++cpn_node_it)
                                    {
                                        ptree cpn = cpn_node_it->second;
                                        for(ptree::iterator cpn_it = cpn.begin(); cpn_it != cpn.end(); ++cpn_it)
                                        {

                                            string cpn_key = cpn_it->first;
                                            val = cpn.get<string>(cpn_key);
                                            cout<< cpn_key <<":"<<val<<endl;
                                        }
                                    }
                                }
                                else
                                {

                                    val = tkt.get<string>(tkt_key);
                                    cout<<tkt_key<<":"<<val<<endl;
                                }
                            }
                        }
                    }
                    else
                    {

                        val = tif.get<string>(tif_key);
                        cout<< tif_key << ":" <<val <<endl;
                    }
                }
            }
        }
        else
        {

            val = msg.get<string>(msg_key);
            cout<< msg_key << ":" <<val<<endl;
        }
    }
}

// #define FORMAT_STRING(strLog,szFormat)\
//     int nListCount = 0;\
// {\
//     va_list pArgList;\
//     va_start(pArgList,szFormat);\
//     int nLength = _vscprintf(szFormat,pArgList) + 1;\
//     strLog.resize(nLength);\
//     nListCount +=_vsnprintf_s(const_cast<char *>(strLog.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);\
//     va_end(pArgList);\
// }\
//     strLog.reserve(nListCount + 1);

void LogF(const char* szBuf,...)
{
    std::string strLog;
    FORMAT_STRING(strLog,szBuf);
    std::cout << strLog;
}

void Log(const char* szBuf)
{
    std::cout << szBuf;
}

std::string GetLoginJson(const std::string& strUsername,const std::string& strUserpwd,const std::string& strOperationId)
{
    std::string strTemp;
    JGW::JGW_FormatString(strTemp,"{\"requestType\":0,\"username\":%s,\"userpwd\":%s,\"operationId\":%s}",strUsername.c_str(),strUserpwd.c_str(),strOperationId.c_str());
    return strTemp;
}
#define MAX_ALLSECTIONS 300  //定义最大的段长度

#define MAX_SECTION 20     //段的最大长度


void EnumIniFile(LPCTSTR pFilePath)  
{  
    // TODO: Add your control notification handler code here  
    TCHAR strAppNameTemp[1024];//所有AppName的返回值  
    TCHAR strKeyNameTemp[1024];//对应每个AppName的所有KeyName的返回值  
    TCHAR strReturnTemp[1024];//返回值  
    DWORD dwKeyNameSize;//对应每个AppName的所有KeyName的总长度  
    //所有AppName的总长度  
    DWORD dwAppNameSize = GetPrivateProfileString(NULL,NULL,NULL,strAppNameTemp,1024,pFilePath);  
    if(dwAppNameSize>0)  
    {  
        TCHAR *pAppName = new TCHAR[dwAppNameSize];  
        int nAppNameLen=0;  //每个AppName的长度  
        for(int i = 0;i<dwAppNameSize;i++)  
        {  
            pAppName[nAppNameLen++]=strAppNameTemp[i];  
            if(strAppNameTemp[i]=='\0')  
            {  
                OutputDebugString(pAppName);  
                OutputDebugString(_T("\r\n"));  
                dwKeyNameSize = GetPrivateProfileString(pAppName,NULL,NULL,strKeyNameTemp,102,pFilePath);  
                if(dwAppNameSize>0)  
                {  
                    TCHAR *pKeyName = new TCHAR[dwKeyNameSize];  
                    int nKeyNameLen=0;    //每个KeyName的长度  
                    for(int j = 0;j<dwKeyNameSize;j++)  
                    {  

                        pKeyName[nKeyNameLen++]=strKeyNameTemp[j];  
                        if(strKeyNameTemp[j]=='\0')  
                        {  
                            OutputDebugString(pKeyName);  
                            OutputDebugString(_T("="));  
                            if(GetPrivateProfileString(pAppName,pKeyName,NULL,strReturnTemp,1024,pFilePath))  
                                OutputDebugString(strReturnTemp);  
                            memset(pKeyName,0,dwKeyNameSize);  
                            nKeyNameLen=0;  
                            OutputDebugString(_T("\r\n"));  
                        }  
                    }  
                    delete[]pKeyName;  

                }  
                memset(pAppName,0,dwAppNameSize);  
                nAppNameLen=0;  
            }  
        }  
        delete[]pAppName;  
    }  
}


typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef   signed short  int16;
typedef unsigned int   uint32;
typedef   signed int    int32;
typedef unsigned int   uint;
typedef int (CALLBACK* DHCPNOTIFYPROC)(LPWSTR, LPWSTR, BOOL, DWORD, DWORD, DWORD, int);  
//! {7C9F1D3A-7960-44CE-9B17-CE024A257D49}
BOOL   SetIP(LPCSTR   lpszAdapterName,   int   nIndex,   LPCSTR   pIPAddress,   LPCSTR   pNetMask,   LPCSTR   pNetGate)
{
    HKEY   hKey;
    std::string strKeyName = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\";
    strKeyName += lpszAdapterName;
    if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        strKeyName.c_str(),
        0,
        KEY_WRITE,
        &hKey)   !=   ERROR_SUCCESS)
        return   FALSE;

    char   mszIPAddress[100];
    char   mszNetMask[100];
    char   mszNetGate[100];

    strncpy(mszIPAddress,   pIPAddress,   98);
    strncpy(mszNetMask,   pNetMask,   98);
    strncpy(mszNetGate,   pNetGate,   98);

    int   nIP,   nMask,   nGate;

    nIP   =   strlen(mszIPAddress);
    nMask   =   strlen(mszNetMask);
    nGate   =   strlen(mszNetGate);

    *(mszIPAddress   +   nIP   +   1)   =   0x00;
    nIP   +=   2;

    *(mszNetMask   +   nMask   +   1)   =   0x00;
    nMask   +=   2;

    *(mszNetGate   +   nGate   +   1)   =   0x00;
    nGate   +=   2;

    RegSetValueExA(hKey,   "IPAddress",   0,   REG_MULTI_SZ,   (unsigned   char*)mszIPAddress,   nIP);
    RegSetValueExA(hKey,   "SubnetMask",   0,   REG_MULTI_SZ,   (unsigned   char*)mszNetMask,   nMask);
    RegSetValueExA(hKey,   "DefaultGateway",   0,   REG_MULTI_SZ,   (unsigned   char*)mszNetGate,   nGate);

    RegCloseKey(hKey);

    //通知IP地址改变
    BOOL bResult   =   FALSE;
    HINSTANCE hDhcpDll;
    DHCPNOTIFYPROC pDhcpNotifyProc;
    WCHAR   wcAdapterName[256];

    MultiByteToWideChar(CP_ACP,   0,   lpszAdapterName,   -1,   wcAdapterName,256);

    if((hDhcpDll   =   LoadLibraryA("dhcpcsvc"))   ==   NULL)
        return   FALSE;

    if((pDhcpNotifyProc   =   (DHCPNOTIFYPROC)GetProcAddress(hDhcpDll,   "DhcpNotifyConfigChange"))   !=   NULL)
        if((pDhcpNotifyProc)(NULL,   wcAdapterName,   TRUE,   nIndex,   inet_addr(pIPAddress),   inet_addr(pNetMask),   0)   ==   ERROR_SUCCESS)
            bResult   =   TRUE;

    FreeLibrary(hDhcpDll);

    return   TRUE;
}
//! 月薪 五险一金 专项扣除金额
void Calc(int monthly,int payment,int deduction,int mons)
{
    int deducted = 0;
    int deductedcounts = 0;
    for (int i = 1;i <= mons;i ++)
    {
        int monys = (monthly * i - 5000 * i - payment * i - deduction * i);
        if (monys <= 36000)
        {
            deducted = monys * 0.03 - deductedcounts;
            deductedcounts += deducted;
        }
        else if (monys <= 144000)
        {
            deducted = monys * 0.1 - deductedcounts - 2520;
            deductedcounts += deducted;
        }
        else if (monys <= 300000)
        {
            deducted = monys * 0.2 - deductedcounts - 16920;
            deductedcounts += deducted;
        }
        else if (monys <= 420000)
        {
            deducted = monys * 0.25 - deductedcounts - 31920;
            deductedcounts += deducted;
        }
        else if (monys <= 660000)
        {
            deducted = monys * 0.3 - deductedcounts - 52920;
            deductedcounts += deducted;
        }
        else if (monys <= 960000)
        {
            deducted = monys * 0.35 - deductedcounts - 85920;
            deductedcounts += deducted;
        }
        else if (monys > 960000)
        {
            deducted = monys * 0.45 - deductedcounts - 181920;
            deductedcounts += deducted;
        }
        
        std::cout << "第"  << i << "月" << "  个税:" << deducted << "  总共已经扣除" << deductedcounts << std::endl;
    }
}


void PareserConfigString(const std::wstring& strConfig,std::map<std::wstring,std::wstring>& mapProperty)
{
    int temp = 0,start = 0;
    bool isValue = false;
    std::wstring strKey,strValue;
    
    for (size_t i = 0;i < strConfig.length();i ++)
    {
        if (i == strConfig.length() - 1 && isValue)
        {
            mapProperty[strKey] = strValue;
            continue;
        }
        //! 是否是换行
        if (L'\n'== strConfig[i])
        {
            if (strKey.empty() && strValue.empty()) 
                continue;

            if (isValue) 
            {
                {
                    mapProperty[strKey] = strValue;
                    strKey = L"";
                    strValue = L"";
                    isValue=false;
                    start = 0;
                    continue;
                }
            }
        }
       
        //! 判断属性头
        if (0 == start)
        {
            if (L';'== strConfig[i] || L'#' == strConfig[i]) 
            {
                while (L'\n' != strConfig[i] && i < strConfig.length()) i++;
                i --;
                continue;
            }

            if (L' ' == strConfig[i])
            {
                continue;
            }
        }

        if (!isValue)
        {
            if (L'=' == strConfig[i])
            {
                isValue = true;
                start = 0;
                continue;
            }
            strKey += strConfig[i];
            start ++;
        }
        else 
        {
            strValue += strConfig[i];
        }
    }
}

int main()
{
    std::map<std::wstring,std::wstring> mapProperty;
    std::wstring strSoftAp = L"\ndriver=nl80211\n\n\
        interface=wlan0\n\
        hw_mode=ad\n\
        channel=1\n\
        ctrl_interface=/var/run/hostapd\n\
        ctrl_interface_group=0\n\
        ssid=Leopard_60G_000073\n\
        country_code=US\n\
        auth_algs=1\n\
        wpa=2\n\
        wpa_key_mgmt=WPA-PSK\n\
        wpa_pairwise=GCMP\n\
        wpa_passphrase=123456789";
    PareserConfigString(strSoftAp,mapProperty);
//     std::wcout.imbue(std::locale("chs"));
//     std::cout.imbue(std::locale("chs"));
//     Calc(20000,1560,1000,12);
//     system("pause");
     return 0;
//     SetIP("{7C9F1D3A-7960-44CE-9B17-CE024A257D49}",0,"192.168.213.12","255.255.255.0","192.168.213.252");
//     std::string strTemp = "192.168.1.26";
//     std::tr1::regex regExpress("((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");
// 
//     std::cout << regex_match("123", regex("\\d")) << endl;       //结果为0
//     std::cout << regex_match("123", regex("\\d+")) << endl;      //结果为1
// 
//     if(!regex_match(strTemp.c_str(),regExpress))
//     {
//         return -1;
//     }


    return 0;
//     bool bEnable = JGW::JGW_GetAdpateDHCPStatus(L"{7C9F1D3A-7960-44CE-9B17-CE024A257D49}");
//     std::wstring strIP = JGW::JGW_GetStaticIPAddrToAdpaterName(L"{7C9F1D3A-7960-44CE-9B17-CE024A257D49}");
// 
//      DWORD64 dw64 = 5566;
//     wchar_t szBu22f[260] = {0};
// 
//     _stprintf(szBu22f,L"%08I64d",dw64);
// 
//     dw64 = 0;
#if 0
    struct s_wigig_softap_conf 
    {
        uint32 resverd;
        uint8 space;
        uint8 ssid_len;
        char ssid[1];
    };
    
    std::ifstream fssid_conf;
    char szBuf[256] = {0};
    fssid_conf.open("C:\\Users\\Administrator\\wigig_softap.conf");
    fssid_conf.read(szBuf,256);

    s_wigig_softap_conf* psWigigSsid = (s_wigig_softap_conf*)szBuf;
    s_wigig_softap_conf* psWigigPassword = (s_wigig_softap_conf*)(psWigigSsid->ssid + psWigigSsid->ssid_len + 8);

    //strIperfLog = "[  5]   2.00-3.00   sec  70.1 MBytes   588 Mbits/sec                  "
    std::wstring strTempThr = L"[  5]   2.00-3.00   sec  70.1 MBytes   588 Mbits/sec                  ";
    //! sec Bytes bits/sec
    if (std::wstring::npos != strTempThr.find(L"sec") && std::wstring::npos != strTempThr.find(L"s/sec") && strTempThr[0] == '[')
    {
        std::wstring strID;
        int id = 0;
        size_t start = 0,end = 0;float speed = 0;
        id = strTempThr.find(L']',2) - 1;
        strID = strTempThr.substr(1,id);
        start = _ttoi(strTempThr.substr(id + 2).c_str());
        id = strTempThr.find(L'-',id);
        end = _ttoi(strTempThr.substr(id + 1).c_str());
        id = strTempThr.find(L"s/sec",id + 1);
        id = strTempThr.rfind(L's',id - 1);
        speed = _ttof (strTempThr.substr(id + 1).c_str());
    }



    std::wstring strCommand = L"adb shell $$_TSE_IPERF_IP_$$ ping -c 1 -i 1 $$_TSE_IPERF_IP_$$";
    {
        std::wstring strTemp = strCommand,strEnvironmentValue,strEnvironmentName,strReplaceSrc;
        int nStart = strTemp.find(L"$$_"),nEnd = 0;
        while (std::wstring::npos != nStart)
        {
            nEnd = strTemp.find(L"_$$",nStart + 3);
            if (nEnd == std::wstring::npos) return 0;
            strEnvironmentName = strTemp.substr(nStart + 3,nEnd - nStart - 3);
            strEnvironmentValue = L"192.168.50.243"/*GetGlobalEnvironment()->GetString(strEnvironmentName)*/;
            JGW::JGW_FormatWString(strReplaceSrc,L"$$_%s_$$",strEnvironmentName.c_str());
            JGW::JGW_ReplaceStringW(strCommand,strReplaceSrc,strEnvironmentValue);
            nStart = strTemp.find(L"$$_",nEnd + 3);
        } 
    }
    
    std::string str = "{\"code\":0,\"images\":[{\"url\":\"fmn057/20111221/1130/head_kJoO_05d9000251de125c.jpg\"},{\"url\":\"fmn057/20111221/1130/original_kJoO_05d9000251de125c.jpg\"}]}";  
    using namespace boost::property_tree;  

    std::stringstream ss(str);  
    ptree pt;  
//     try{      
//         read_json(ss, pt);  
//     }  
//     catch(ptree_error & e) {  
//         return 1;   
//     }  

    // 修改/增加一个key-value，key不存在则增加   
    pt.put("bStatus", "true");  
    pt.put("responseType",0);
    std::vector<std::string> strArray;
    strArray.push_back("mt7612e-BDFE");
    strArray.push_back("TP-LINK_66A9");
    strArray.push_back("UNISUN");
    strArray.push_back("1006-5G");
    ptree ptreeSSID;
    for (std::vector<std::string>::iterator it = strArray.begin();
        it != strArray.end();
        ++ it)
    {
        ptree array1;
        array1.put_value(it[0]);
        ptreeSSID.push_back(std::make_pair("", array1));
        //ptreeSSID.put_child("",array1);
    }
    pt.put_child("ssidArray",ptreeSSID);
    // 插入一个数组   
//     ptree exif_array;  
//     ptree array1, array2, array3;  
//     array1.put("Make", "NIKON");  
//     array2.put("DateTime", "2011:05:31 06:47:09");  
//     array3.put("Software", "Ver.1.01");  
//     exif_array.push_back(std::make_pair("", array1));  
//     exif_array.push_back(std::make_pair("", array2));  
//     exif_array.push_back(std::make_pair("", array3));  

    //   exif_array.push_back(std::make_pair("Make", "NIKON"));   
    //   exif_array.push_back(std::make_pair("DateTime", "2011:05:31 06:47:09"));   
    //   exif_array.push_back(std::make_pair("Software", "Ver.1.01"));   

    /*pt.put_child("exifs", exif_array);  */
    std::stringstream s2;  
    write_json(s2, pt);  
    std::string outstr = s2.str();  

    return 0;  


    std::string strMsg = "[{\"availableBlocks\":6633537,\"availableBytes\":27170967552,\"blockByteSize\":4096,\"description\":\"Internal shared storage\",\"freeBlocks\":6637633,\"freeBytes\":27187744768,\"totalBlocks\":6676031,\"totalBytes\":27345022976},{\"availableBlocks\":6633537,\"availableBytes\":27170967552,\"blockByteSize\":4096,\"description\":\"T-Flash\",\"freeBlocks\":6637633,\"freeBytes\":27187744768,\"totalBlocks\":6676031,\"totalBytes\":27345022976},{\"availableBlocks\":6633537,\"availableBytes\":27170967552,\"blockByteSize\":4096,\"description\":\"T-Flash1\",\"freeBlocks\":6637633,\"freeBytes\":27187744768,\"totalBlocks\":6676031,\"totalBytes\":27345022976}]";

    stringstream msg_ss(strMsg);
    ptree msg;
    try
    {
        read_json(msg_ss, msg);
    }
    catch (json_parser::json_parser_error& e)
    {
        std::cout << e.what() << std::endl;
    }
/*    boost::property_tree::ptree tkt_node = msg.get_child("");*/
    for(ptree::iterator msg_it = msg.begin(); msg_it != msg.end(); ++msg_it)
    {
        ptree msg_1 = msg_it->second;
        for(ptree::iterator msg_it1 = msg_1.begin(); msg_it1 != msg_1.end(); ++msg_it1)
        {
            std::cout << msg_it1->first << " : " << msg_it1->second.get_value<string>(msg_it1->first) << std::endl;
        }
    }
//     write_json_data_into_file();
//     system("pause");
//     read_json_data_from_file();
//     system("pause");
//     write_json_data_into_string();
//     system("pause");
//     read_json_data_from_string();
    //cout<<"---------------------------"<<endl;
    //testMsg();
    //cout<<"---------------------------"<<endl;
    //bianli_json();
    //cout<<"---------------------------"<<endl;
    //test();
    //cout<<"---------------------------"<<endl;
    //parseMsg();
    return 0;
#else
     //float fMin,fMax;
     //wchar_t szhhh[260];
     //_stscanf_s(L"[  3]  0.0-30.0 sec   884 MBytes   247 Mbits/sec",L"[%s]%f-%f",&szhhh,&fMin,&fMax);
//     std::wstring strPath(JGW::JGW_GetApplicationFolder());
//     strPath += L"tool\\iperf-2.0.9-win32\\iperf.exe";
//     std::wstring strFolder (JGW::JGW_GetApplicationFolder());
//     strFolder += L"tool\\iperf-2.0.9-win32";
// 
//     ShellExecute(NULL,L"open",strPath.c_str(),L" -s",JGW::JGW_GetApplicationFolder(),SW_NORMAL);

    //EnumIniFile(L"c:\\JW\\exec\\text.data");
    //TCHAR szBufini[260] = {0};
    //DWORD dwIniSize = GetPrivateProfileSection(L"MESClient",szBufini,20,L"c:\\JW\\exec\\jgw_mes_config.data");
    //DWORD dwKeyNameSize = GetPrivateProfileString(L"MESClient",NULL,NULL,szBufini,12,_T("test.INI"));  

    //TCHAR* pbuf = new TCHAR[dwIniSize+1];
    //GetPrivateProfileSection(L"MESClient",pbuf,dwIniSize,L"c:\\JW\\exec\\jgw_mes_config.data");
    ////GetPrivateProfileSection(L"MESClient",szBufini,260,L"c:\\JW\\exec\\jgw_mes_config.data");
    ////GetPrivateProfileSection(L"MESClient",szBufini,260,L"c:\\JW\\exec\\jgw_mes_config.data");
    //return 0;

    JGW::CCJGW_FTM_ClientSocket ftm;

    char *szBuf = (char*)calloc(1024,1);

    if (!ftm.AsyncAdbTcpIPConnnectSocket("192.168.133.13",3001,10)) {free(szBuf); return 0;}

    std::cout << "connected socket" << std::endl;

   // std::string strMsg = GetLoginJson("101","1","000");

   if (ftm.AsyncWriterSocket("{\"requestType\":0}",10)) std::cout << "writed socket" << std::endl;

    if (ftm.AsyncReadSocket(szBuf,1024,1000)) std::cout << "read socket" << std::endl;
    std::cout << szBuf<< std::endl;

    memset(szBuf,0x00,1024);
    if (ftm.AsyncWriterSocket("{\"requestType\":1,\"ssidname\":\"JW_MIFI_60G_41C2\",\"passwrod\":\"23461768\"}",10)) std::cout << "writed socket" << std::endl;

    if (ftm.AsyncReadSocket(szBuf,1024,1000)) std::cout << "read socket" << std::endl;
    std::cout << W(szBuf) << std::endl;
    //!std::cout << U(szBuf) << std::endl;
    //memset(szBuf,0x00,1024);
//     if (ftm.AsyncWriterSocket("{\"requestType\":2}",10)) std::cout << "writed socket" << std::endl;
// 
//     if (ftm.AsyncReadSocket(szBuf,1024,1000)) std::cout << "read socket" << std::endl;
//     std::cout << szBuf<< std::endl;

    return 0;
    if (ftm.AsyncWriterSocket(JGW::CCJGW_MESDatabaseJsonCmd::GetGetInfoSOMOJson("DHW181367","Y"),10)) std::cout << "writed socket" << std::endl;

    if (ftm.AsyncReadSocket(szBuf,1024,10)) std::cout << "read socket" << std::endl;
    std::cout << szBuf<< std::endl;


    //! LE241836000000 DHW180037

   //if (ftm.AsyncWriterSocket(JGW::CCJGW_MESDatabaseJsonCmd::GetGetInfoSNJson("L10P1841000009","4520065687-010"),10)) std::cout << "writed socket" << std::endl;
	
	//if (ftm.AsyncWriterSocket(JGW::CCJGW_MESDatabaseJsonCmd::GetGetTestStatusJson("L10P1841000007","008"),10)) std::cout << "writed socket" << std::endl;
	//memset(szBuf,0x00,1024);
	//if (ftm.AsyncWriterSocket(JGW::CCJGW_MESDatabaseJsonCmd::GetGetInfoIMEIJson("863408025041055","L10Q1842000181","DHW181367"),10)) std::cout << "writed socket" << std::endl;
	if (ftm.AsyncWriterSocket(JGW::CCJGW_MESDatabaseJsonCmd::GetGetInfoIMEIJson("863408025041055","L10Q1842000181","DHW181367-010"),10)) std::cout << "writed socket" << std::endl;

	if (ftm.AsyncReadSocket(szBuf,1024,10)) std::cout << "read socket" << std::endl;
    std::cout << szBuf<< std::endl;



    if (ftm.AsyncWriterSocket(JGW::CCJGW_MESDatabaseJsonCmd::GetLogoutJson(),10)) std::cout << "writed socket" << std::endl;

    if (ftm.AsyncReadSocket(szBuf,1024,10)) std::cout << "read socket" << std::endl;
    std::cout << szBuf<< std::endl;

    free(szBuf);


    return 0;
    if (ftm.AsyncReadSocket(szBuf,1024,10)) std::cout << "read socket" << std::endl;
    std::cout << szBuf<< std::endl;
    
    stringstream msg_ss(szBuf);
    ptree msg;
    read_json(msg_ss, msg);

    for(ptree::iterator msg_it = msg.begin(); msg_it != msg.end(); ++msg_it)
    {
        std::cout << msg_it->first << " : " << msg.get<string>(msg_it->first) << std::endl;
    }

    try
    {
        std::string strTemp = msg.get<string>("1234");
    }
    catch (ptree_bad_path& e)
    {
    	
    }

    
    //boost::asio::ip::tcp::socket sock(ios);
    //boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"),5432);

    //sock.connect(ep);

    //std::string strMsg = "{\"requestCode\":1}";

    //vector<char> str(1024,0);
    //sock.read_some(boost::asio::buffer(str));
    //std::cout << &str[0] << std::endl;

    //sock.write_some(boost::asio::buffer(strMsg));

    //sock.read_some(boost::asio::buffer(str));
    //std::cout << &str[0] << std::endl;

    //strMsg = "{\"requestCode\":2,\"type\":1}";

    ////vector<char> str(1024,0);
    //sock.write_some(boost::asio::buffer(strMsg));

    //sock.read_some(boost::asio::buffer(str));
    //std::cout << &str[0] << std::endl;

    //strMsg = "{\"requestCode\":2,\"type\":2}";

    ////vector<char> str(1024,0);
    //sock.write_some(boost::asio::buffer(strMsg));
    ////     sock.read_some(boost::asio::buffer(str));
    ////     std::cout << &str[0] << std::endl;
    //sock.close();

    return 0;
#endif
}


