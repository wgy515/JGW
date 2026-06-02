#include "stdafx.h"
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <tinyxml/tinyxml.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_FoundationLib/TSE_TestBase.h>

#include <Windows.h>

// ***** global macros ***** //
static const int kMaxInfoBuffer = 256;
#define  GBYTES  1073741824  
#define  MBYTES  1048576  
#define  KBYTES  1024  
#define  DKBYTES 1024.0 

// #include <poco/ClassLoader.h>
// #include <JGW_FoundationFunc\JGW_FilePath.h>
// #include <JGW_FoundationFunc\JGW_StringFunc.h>
// #include <JGW_FoundationFunc\CUtf8String.hpp>
// #include <JGW_FoundationFunc/CJGW_DllDirectoryStateSaver.h>
using namespace JGW;

typedef struct
{
    std::string mstrLibraryName;
    std::map<std::string,std::wstring> mmapClassNameParam;
}S_TSE_LIBRARY_INFO;

void ParseParam(TiXmlElement* pSuiteProject,std::map<std::string,std::wstring>::iterator itMap)
{
    boost::property_tree::ptree mJsonTree;
    std::stringstream msg_ss(JGW_W2A(itMap->second));
    try 
    {
        read_json(msg_ss, mJsonTree);
       // return;
    }
    catch (boost::property_tree::json_parser::json_parser_error& e)
    {
        std::cout << "parser Json() Fail()" << msg_ss.str().c_str() << "  " << e.what() << std::endl;
      //  LogE_F("parser Json(%s) Fail(%s)\n",strJson.c_str(),e.what());   
        return ;
    }

    std::string strParamName,strClassName;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &pos, mJsonTree)
    {
        strParamName = pos.first;
        TiXmlElement* pParamNode = new TiXmlElement(strParamName);

        if (NULL == strParamName.compare("RealName"))
        {
            strClassName = pos.second.get_value<std::string>();
            SetElementAttributeValue(pParamNode,"paramVal",JGW_A2W(pos.second.get_value<std::string>()));
            if (NULL != strClassName.compare(itMap->first))
            {
                std::cout << "Class Name " << itMap->first << " Param Class Name: " << strClassName << std::endl;
            }
            SetElementAttributeValue(pParamNode,"description",L"class name");
        }
        //         else if (NULL == strParamName.compare("TestName"))
        //         {
        //             SetElementAttributeValue(pSuiteProject,"paramVal",JGW_A2W(pos->second.get_value<std::string>()));
        //             SetElementAttributeValue(pSuiteProject,"description",L"TSE测试名称描述");
        //         }
        else
        {
            SetElementAttributeValue(pParamNode,"paramVal",L"");
            SetElementAttributeValue(pParamNode,"description",JGW_A2W(pos.second.get_value<std::string>()));
        }
        pSuiteProject->LinkEndChild(pParamNode);
    }



    //! 遍历JSON
//     std::string strParamName;
//     for(BOOST_AUTO(pos, mJsonTree.front()); pos != mJsonTree.end(); ++pos)
//     {
//         strParamName = pos->first;
//         TiXmlElement* pParamNode = new TiXmlElement(strParamName);
// 
//         if (NULL == strParamName.compare("RealName"))
//         {
//             SetElementAttributeValue(pSuiteProject,"paramVal",JGW_A2W(pos->second.get_value<std::string>()));
//             SetElementAttributeValue(pSuiteProject,"description",L"TSE测试实例名称");
//         }
// //         else if (NULL == strParamName.compare("TestName"))
// //         {
// //             SetElementAttributeValue(pSuiteProject,"paramVal",JGW_A2W(pos->second.get_value<std::string>()));
// //             SetElementAttributeValue(pSuiteProject,"description",L"TSE测试名称描述");
// //         }
//         else
//         {
//             SetElementAttributeValue(pSuiteProject,"paramVal",L"");
//             SetElementAttributeValue(pSuiteProject,"description",JGW_A2W(pos->second.get_value<std::string>()));
//         }
//         pSuiteProject->LinkEndChild(pParamNode);
//     }
}

void BuildSuiteAllTestXml(const std::string& strSuiteConfigPath,std::vector<S_TSE_LIBRARY_INFO>& sTSELibraryInfo)
{
    TiXmlDocument* myDocument = new TiXmlDocument();                        //创建一个XML文件
    TiXmlDeclaration* pDeclaration= new TiXmlDeclaration("1.0","UTF-8","");  //创建xml文件头（<?xml version="1.0" encoding="UTF-8" ?>）
    myDocument->LinkEndChild(pDeclaration);                                 //加入将xml文件头加入文档中

    TiXmlElement* pRoot = new TiXmlElement("Suite");                    //创建一个元素节点
    myDocument->LinkEndChild(pRoot);                                     //加入BUSINESS元素节点到文档中

    std::string strLibraryName;

    for (std::vector<S_TSE_LIBRARY_INFO>::iterator it = sTSELibraryInfo.begin();
        it != sTSELibraryInfo.end();
        ++ it)
    {
        strLibraryName = it->mstrLibraryName;
        for (std::map<std::string,std::wstring>::iterator itMap = it->mmapClassNameParam.begin();
            itMap != it->mmapClassNameParam.end();
            ++ itMap)
        {
            TiXmlElement* pSuiteProject = new TiXmlElement("SuiteProject");
            //! <ModuleName paramVal="TSE_DatabasePlugin.dll" description="模块名称或路径，默认路径：EXE执行路径" />
            SetElementAttributeValueA(pSuiteProject,"name",itMap->first);
            //! module name
            TiXmlElement* pModuleName = new TiXmlElement("ModuleName");
            SetElementAttributeValue(pModuleName,"paramVal",JGW_A2W(it->mstrLibraryName));
            SetElementAttributeValue(pModuleName,"description",L"module name");
            pSuiteProject->LinkEndChild(pModuleName);
            ParseParam(pSuiteProject,itMap);
            pRoot->LinkEndChild(pSuiteProject);
        }
    }
   // SetElementAttributeValue(pRoot,"test",L"\n\n\n");
    myDocument->SaveFile(strSuiteConfigPath);

    //std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
    //TiXmlElement* pRoot,*pCommonNode,*pConfigNode = NULL;
    //if (!ptrXmlDoc->LoadFile(strSuiteConfigPath.c_str())) return ;

    //pRoot = ptrXmlDoc->RootElement();
    //if (!pRoot) return ;

    // pCommonNode = pRoot->FirstChildElement("common");
    // while (pCommonNode)
    // {
    //      SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrIperfServerIPAddressEnvironment).c_str(),sThroughputConfig.mstrIperfServerIPAddress);
    //      SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrTestRangeEnvironment).c_str(),sThroughputConfig.mstrTestRange);
    //      SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrTestTimeEnvironment).c_str(),sThroughputConfig.mstrTestTime);
    //      SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrWiFiScanIPAddressEnvironment).c_str(),sThroughputConfig.mstrWiFiScanIPAddress);
    //      SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrIperfCommandEnvironment).c_str(),sThroughputConfig.mstrIperfCommand);
    //      SetElementTextValue(pCommonNode,JGW_W2A(sThroughputConfig.mstrIperfCommandTimeOutEnvironment).c_str(),sThroughputConfig.mstrIperfCommandTimeOut);
    //     pCommonNode = pCommonNode->NextSiblingElement("common");
    // }
    //ptrXmlDoc->SaveFile(strSuiteConfigPath);
}


__int64 CompareFileTime(FILETIME time1, FILETIME time2)
{
    __int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;
    __int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime ;
    return (b - a);
}

int _tmain(int argc, _TCHAR* argv[])
{
#if 1
    CTSE_SequencePocoClassLoader<CTSE_TestBase> classLoaderModule;
    classLoaderModule.LoadTestSuiteModule("TSE_ConfigUIPlugin.dll");
    classLoaderModule.LoadTestSuiteModule("TSE_DatabasePlugin.dll");
    classLoaderModule.LoadTestSuiteModule("TSE_DevicesPlugin.dll");
    std::cout.imbue(std::locale("chs"));
    std::wcout.imbue(std::locale("chs"));

    std::vector<S_TSE_LIBRARY_INFO> mvTSELibraryInfo;
    S_TSE_LIBRARY_INFO sTSELibraryInfo;
    CTSE_TestBase* pTestBase =  NULL;

    for (Poco::ClassLoader<CTSE_TestBase>::Iterator it = classLoaderModule.GetClassLoaderModule().begin();
        it != classLoaderModule.GetClassLoaderModule().end();
        ++ it)
    {
        std::cout << "Library: " << JGW_W2A(JGW_GetApplicationFolder()) << W(it->first.c_str()) << std::endl;
        sTSELibraryInfo.mstrLibraryName = it->first.c_str();
        sTSELibraryInfo.mmapClassNameParam.clear();

        for (Poco::Manifest<CTSE_TestBase>::Iterator itClassName = it->second->begin();
            itClassName != it->second->end();
            ++ itClassName)
        {
            std::cout << "ClassName: " << itClassName->name() << std::endl;
            pTestBase = itClassName->create();
            //! std::wcout << L"Param: " << pTestBase->TSE_GetParamDescription() << std::endl;
            sTSELibraryInfo.mmapClassNameParam[itClassName->name()] = pTestBase->TSE_GetParamDescription();
            itClassName->destroy(pTestBase);
        }
        mvTSELibraryInfo.push_back(sTSELibraryInfo);
    }

    classLoaderModule.UnloadTestSuiteModule();

    std::string strSuiteConfigPath;
    JGW_GetModuleWorkPathA(strSuiteConfigPath);
    strSuiteConfigPath += "suite_all.xml";
    BuildSuiteAllTestXml(strSuiteConfigPath,mvTSELibraryInfo);

    system("pause");
#else
    std::string memory_info;
    MEMORYSTATUSEX statusex;
    statusex.dwLength = sizeof(statusex);
    if (GlobalMemoryStatusEx(&statusex))
    {
        unsigned long long total = 0, remain_total = 0, avl = 0, remain_avl = 0;
        double decimal_total = 0, decimal_avl = 0;
        remain_total = statusex.ullTotalPhys % GBYTES;
        total = statusex.ullTotalPhys / GBYTES;
        avl = statusex.ullAvailPhys / GBYTES;
        remain_avl = statusex.ullAvailPhys % GBYTES;
        if (remain_total > 0)
            decimal_total = (remain_total / MBYTES) / DKBYTES;
        if (remain_avl > 0)
            decimal_avl = (remain_avl / MBYTES) / DKBYTES;

        decimal_total += (double)total;
        decimal_avl += (double)avl;
        char  buffer[kMaxInfoBuffer];
        sprintf_s(buffer, kMaxInfoBuffer, "total %.2f GB (%.2f GB available)", decimal_total, decimal_avl);
        memory_info.append(buffer);
    }
    std::cout << memory_info << std::endl;

    FILETIME idleTime;//空闲时间 
    FILETIME kernelTime;//核心态时间 
    FILETIME userTime;//用户态时间 
    bool res;
    res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
    std::cout << "GetSystemTimes(&, &, &) = " << res << std::endl << std::endl;

    HANDLE hEvent;
    FILETIME pre_idleTime;
    FILETIME pre_kernelTime;
    FILETIME pre_userTime;

    pre_idleTime = idleTime;
    pre_kernelTime = kernelTime;
    pre_userTime = userTime;

    hEvent = CreateEventW(NULL, FALSE, FALSE, NULL); 
    //初始值为nonsignaled，并且每次触发后自动设置为nonsignaled

    while (1){
        WaitForSingleObject( hEvent,1000 );//等待500毫秒

        res = GetSystemTimes(&idleTime, &kernelTime, &userTime );

        __int64 idle = CompareFileTime(pre_idleTime, idleTime);
        __int64 kernel = CompareFileTime(pre_kernelTime, kernelTime);
        __int64 user = CompareFileTime(pre_userTime, userTime);

        int cpu_occupancy_rate = (kernel + user - idle) * 100 / (kernel + user);
        //（总的时间 - 空闲时间）/ 总的时间 = 占用CPU时间的比率，即占用率

        int cpu_idle_rate = idle * 100 / (kernel + user);
        //空闲时间 / 总的时间 = 闲置CPU时间的比率，即闲置率 

        int cpu_kernel_rate = kernel * 100 / (kernel + user);
        //核心态时间 / 总的时间 = 核心态占用的比率 

        int cpu_user_rate = user * 100 / (kernel + user);
        //用户态时间 / 总的时间 = 用户态占用的比率 

        std::cout << std::left << std::setw(15) << "CPU占用率：" << cpu_occupancy_rate << "%" << std::endl
            << std::setw(15) << "CPU闲置率：" << cpu_idle_rate << "%" << std::endl
            << std::setw(15) << "核心态占比率：" << cpu_kernel_rate << "%" << std::endl
            << std::setw(15) << "用户态占比率：" << cpu_user_rate << "%" << std::endl << std::endl;

        pre_idleTime = idleTime;
        pre_kernelTime = kernelTime;
        pre_userTime = userTime;
    }

    return 0;






    return 0;
#endif
}