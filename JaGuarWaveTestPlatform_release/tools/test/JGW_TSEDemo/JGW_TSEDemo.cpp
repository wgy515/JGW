// JGW_TSEDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/CJGW_NumberRule.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_CryptPlugin/CJGW_CryptFileMD5.h>
#include <JGW_FoundationFunc\CUtf8String.hpp>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_QMSLFuncPlugin/jgw_wr_nv_api.h>
#include <JGW_WindowsFuncPlugin/CJGW_MediaTransferProtocol.h>
#include "common_defs.h"
#include <regex>
#include <poco/Crypto/DigestEngine.h>
#include <poco/Path.h>
#include <poco/File.h>
#include <poco/Zip/Compress.h>
#include <poco/Zip/ZipArchive.h>
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
#include <fstream>
// using namespace JGW;
// 
// class CTestLog : public CTSE_LogListener,public CTSE_TestStatusListener
// {
// public:
//     void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
//     {
//         std::wcout << strTestName <<"  "<< gTestStatus[eTestStatus] << std::endl;
//     }
// 
//     void OnInfoLog(const wchar_t* strInfoLog)
//     {
//         std::wcout << strInfoLog;
//     }
// 
//     void OnErrorLog(const wchar_t* strErrorLog)
//     {
//         std::wcout << "ERROR:" << strErrorLog;
//     }
// 
//     void OnDebugLog(const wchar_t* strDebugLog)
//     {
//         std::wcout << strDebugLog;
//     }
// };
// 
// int get_file_size_to_file(FILE* fp)
// {
//     int file_size;
// 
//     if (!fp) return 0;
//     fseek (fp, 0, SEEK_END);  // seek to end of file
//     file_size = ftell (fp); // get current file pointer
//     fseek (fp, 0, SEEK_SET);  // seek back to beginning of file
// 
//     return file_size;
// }
// 
// bool add_mac(char* mac)
// {
//     FILE* fp = NULL;
//     int file_size = 0;
//     char* file_buf = NULL;
//     const char* p = NULL;
// 
//     if ((fp = fopen("d:\\net","a+")) == NULL)
//     {
//         return false;
//     }
// 
//     file_size = get_file_size_to_file(fp);
//     //! 文件不为空的时候
//     if (0 != file_size)
//     {
//         file_buf = (char*)calloc(file_size + 1, 1);
//         //! 如果文件读取错误 则这直接返回false
//         if (1 != fread (file_buf, file_size, 1,fp))
//         {
//             free(file_buf);
//             return false;
//         }
//         //! 查找当前mac是否已经在当前文件中
//         p = strstr(file_buf,mac);
//         //! 如果存在则直接返回否则
//         if (NULL != p)
//         {
//             free(file_buf);
//             fclose(fp);
//             return true;
//         }
//         else
//         {
//             fseek (fp, 0, SEEK_END);  // seek to end of file
//         }
//     }
//     //! 写mac
//     if (1 != fwrite(mac,strlen(mac),1,fp))
//     {
//         return false;
//     }
//     //! 写入空格分隔符
//     if (1 != fwrite(" ",1,1,fp))
//     {
//         return false;
//     }
//     //! 
//     if(NULL != file_buf)
//     {
//         free(file_buf);
//     }
//     fclose(fp);
//     return true;
// }
// 
// bool dec_mac(char* mac)
// {
//     FILE* fp = NULL;
//     int file_size = 0;
//     char* file_buf = NULL;
//     char* file_write_buf = NULL;
//     const char* p = NULL;
//     int i = 0;
//     int index = 0;
//     
//     if ((fp = fopen("d:\\net","a+")) == NULL)
//     {
//         return false;
//     }
// 
//     file_size = get_file_size_to_file(fp);
//     //! 文件为空的时候
//     if (0 == file_size)
//     {
//         fclose(fp);
//         return true;
//     }
// 
//     file_buf = (char*)calloc(file_size + 1, 1);
//     //! 如果文件读取错误 则这直接返回false
//     if (1 != fread (file_buf, file_size, 1,fp))
//     {
//         free(file_buf);
//         return false;
//     }
//     //! 查找当前mac是否已经在当前文件中
//     p = strstr(file_buf,mac);
//     //! 如果不存在则直接返回
//     if (NULL == p)
//     {
//         free(file_buf);
//         fclose(fp);
//         return true;
//     }
// 
//     file_write_buf = (char*)calloc(file_size + 1, 1);
//     //! 复制前面部分
//     for (i = 0;i < p - file_buf;i ++,index ++)
//     {
//         file_write_buf[index] = file_buf[i];
//     }
//     //! 复制后面部分
//     for (i = p - file_buf + strlen(mac) + 1;i < file_size + 1;i ++)
//     {
//         file_write_buf[index ++] = file_buf[i];        
//     }
//     fclose(fp);
//     i = 0;
// 
//     fp = fopen("d:\\net","w+");
// 
//     //! 写mac
//     if (NULL != fp)
//         i = fwrite(file_write_buf,strlen(file_write_buf),1,fp);
//     //! 
//     if(NULL != file_buf)
//     {
//         free(file_buf);
//     }
// 
//     if(NULL != file_write_buf)
//     {
//         free(file_write_buf);
//     }
//     fclose(fp);
// 
//     return (1 == i);
// }
// 
//01ad ea08 74a4 304b cbad 3eca 2a24 bc92
struct S_FILE_MD5_INFO
{
    char mszFileMD5[33];
    std::string mstrFilePath;
    bool operator==(const S_FILE_MD5_INFO& sFileMD5Info) const
    {
        if (NULL == strcmp(sFileMD5Info.mszFileMD5,this->mszFileMD5) && NULL == sFileMD5Info.mstrFilePath.compare(this->mstrFilePath))
            return true;
        return false;
    }
};

typedef struct
{
    //! std::vector<std::string> mvUploadIgnoreFilePath;
    std::map<size_t,std::vector<S_FILE_MD5_INFO>> mmapUploadIgnoreFilePath;
    std::vector<std::string> mvUploadFilePath;
}S_UPLOAD_FILEPATH_PARAM,*LPS_UPLOAD_FILEPATH_PARAM;

void LoadUploadRecord(const std::string& strUploadRecordFilePath,std::map<size_t,std::vector<S_FILE_MD5_INFO>>& mapUploadIgnoreFilePath)
{
    std::ifstream inputFile;
    inputFile.imbue( std::locale("chs") );
    inputFile.open(strUploadRecordFilePath);
    if (!inputFile.is_open()) return;

    char str[512];  
    std::string strKey,strValue;
    S_FILE_MD5_INFO sFileMD5Info;
    size_t len = 0;
    while (inputFile.getline(str,512))
    {
        len = static_cast<size_t>(strlen(str));
        if (len <= 33) continue;
        memcpy_s(sFileMD5Info.mszFileMD5,32,str,32);
        sFileMD5Info.mszFileMD5[32] = '\0';
        sFileMD5Info.mstrFilePath = &str[34];
        mapUploadIgnoreFilePath[len - 34].push_back(sFileMD5Info);
    }
    inputFile.close();
}

void ScanPuamThroughputTestResultFile(const char* filepath,const char* filename,void* lpData)
{
    LPS_UPLOAD_FILEPATH_PARAM psUploadFilePathParam = (LPS_UPLOAD_FILEPATH_PARAM)lpData;
    std::string strFilePathTemp(filepath),strMD5;
    strFilePathTemp += "\\";
    strFilePathTemp += filename;
    S_FILE_MD5_INFO sFileMD5Info;
    sFileMD5Info.mstrFilePath = strFilePathTemp;
    strMD5 = JGW::CCJGW_CryptFileMD5::GetFileMD5A(strFilePathTemp);
    strcpy_s(sFileMD5Info.mszFileMD5,33,strMD5.c_str());
    //sFileMD5Info.mszFileMD5 =

    std::map<size_t,std::vector<S_FILE_MD5_INFO>>::iterator it = psUploadFilePathParam->mmapUploadIgnoreFilePath.find(strFilePathTemp.length());
    if (it == psUploadFilePathParam->mmapUploadIgnoreFilePath.end())
    {
        psUploadFilePathParam->mvUploadFilePath.push_back(strFilePathTemp); 
        return ;
    }

    if (-1 == JGW::find_if(it->second,std::bind2nd(std::equal_to<S_FILE_MD5_INFO>(), sFileMD5Info)))
        psUploadFilePathParam->mvUploadFilePath.push_back(sFileMD5Info.mstrFilePath); 
}
std::string GetHostNameIPAddr()
{
    JGW::CCJGW_SyncSocket syncSocket;
    std::string host,strHostNameIPAddr;
    std::vector<std::string> vListAddr;
    syncSocket.GetHostIP(host,vListAddr);

    strHostNameIPAddr = host;
    for (std::vector<std::string>::iterator it = vListAddr.begin();
        it != vListAddr.end(); ++ it)
    {
        strHostNameIPAddr += "_";
        strHostNameIPAddr += it[0];
    }
    JGW::JGW_ReplaceStringA(strHostNameIPAddr,":","");
    return strHostNameIPAddr;
}

int CalcRSSIAverage(std::vector<int>& vRSSI,const int max_rssi,const int min_rssi)
{
    int rssi = 0;
    int count = 0;
    for (std::vector<int>::iterator it = vRSSI.begin();
        it != vRSSI.end();
        ++ it)
    {
        count ++;
        rssi += it[0];
    }
    //         int rssi = std::accumulate(vRSSI.begin() , vRSSI.end() , 0) - max_rssi - min_rssi;
    return (rssi - max_rssi - min_rssi) / (count - 2);
}

//! 1024
int _tmain(int argc, _TCHAR* argv[])
{
    int rssi = 0,min_rssi = 0,max_rssi = -128;
    std::vector<int> vRSSI;
    for (int i = 0;i < 10;i ++)
    {
        rssi = -56;
        if (i == 2) rssi = -50;
        if (i == 8) rssi = -80;
        vRSSI.push_back(rssi);
        if (min_rssi > rssi) min_rssi = rssi;
        if (max_rssi < rssi) max_rssi = rssi;
    }
    rssi = CalcRSSIAverage(vRSSI,max_rssi,min_rssi);





// 
// 
//     JGW::CCJGW_WiFiSettingImpl* pImpl = JGW::GetWiFiSetting();
// 
//     std::string strRead;
//     pImpl->SetWiFiServerInfo("127.0.0.1",3001,1000);
//     if (pImpl->ConnectPerasoDevice())
//     {
//         pImpl->WriteReadPerasoCommad("version\n",strRead);
//     }
//     pImpl->DisConnectPerasoDevice();
//     boost::property_tree::ptree ptWriteReadPerasoJson;
//     ptWriteReadPerasoJson.put("requestType",3);
//     std::string strTemp = "C:\\JGW\\exec\\中国\\";
//     ptWriteReadPerasoJson.put("command",strTemp);
//     std::stringstream sstream;
//     boost::property_tree::json_parser::write_json(sstream, ptWriteReadPerasoJson);
//     std::string strRequestParams = sstream.str();
// 
//    strRequestParams =  "{\"requestType\":5,\"command\":\"C:\\\\JGW\\\\exec\\\\中国\\\\\"}";
// 
//     boost::property_tree::ptree mJsonTree;
//     std::stringstream msg_ss(strRequestParams);
//     read_json(msg_ss, mJsonTree);
//     std::string strValue = mJsonTree.get<std::string>("command");
/*    std::string test = U(strValue.c_str());*/



    return 0;

// 
//     std::string strUploadFilePathFTP,strFolder("log"),strUploadFolderFTP,strUploadFolder(strFolder);
//     //! 如果不是相对路径，则将此名称改为custom 表示是客户自定义上传的目录
//     if (!JGW::JGW_PathIsRelativeA(strFolder.c_str())) strUploadFolder = "custom";
//     std::string mstrFTPLogRootPath("/JaGuarWaveTestPlatform_Log/"),mstrTaskNumber("DHW");
// 
//     JGW::JGW_FormatString(strUploadFilePathFTP,"%s/%s/%s/%s_%s_%d.zip",mstrFTPLogRootPath.c_str(),JGW::JGW_W2A(JGW::JGW_GetTimeFolder()).c_str(),GetHostNameIPAddr().c_str(),strUploadFolder.c_str(),mstrTaskNumber.c_str(),JGW::JGW_GetTimeOfDay());
//     JGW::JGW_FormatString(strUploadFolderFTP,"%s/%s/%s/",mstrFTPLogRootPath.c_str(),JGW::JGW_W2A(JGW::JGW_GetTimeFolder()).c_str(),GetHostNameIPAddr().c_str());
// 
// 
// 
// 
// 



//     JGW::CCJGW_SyncSocket syncSocket;
// 
//     std::string host,addr;
//     std::vector<std::string> vListAddr;
//     addr = syncSocket.GetHostIP(host);
//     syncSocket.GetHostIP(host,vListAddr);

    S_UPLOAD_FILEPATH_PARAM sUploadFilePathParam;
    std::string strPath("C:\\JW\\exec\\TestLog\\");
    int numFiles = 0;

    LoadUploadRecord("C:\\JW\\exec\\TestLog.upload.ignore",sUploadFilePathParam.mmapUploadIgnoreFilePath);
    JGW::JGW_TraverseFolder_C(strPath,numFiles,"\\*.*"/*"/ *QSEQ*.html"*/,ScanPuamThroughputTestResultFile,&sUploadFilePathParam);

    std::ofstream out("D:\\360安全浏览器下载\\appinf中国人民.zip", std::ios::binary);
    
    try
    {
        Poco::Zip::Compress c(out, true);
        for (size_t i = 0;i < sUploadFilePathParam.mvUploadFilePath.size();i ++)
        {
            std::string strTemp = sUploadFilePathParam.mvUploadFilePath.at(i);
            Poco::Path theFile(U(strTemp.c_str()));
            JGW::JGW_ReplaceStringA(strTemp,"C:\\JW\\exec\\log\\","");
            const Poco::Path fileName(U(strTemp.c_str()));
            c.addFile(theFile,fileName);
        }
        Poco::Zip::ZipArchive a(c.close());
    }
    catch (Poco::Exception&)
    {
    	return 0;
    }

    return 0;




//    dec_mac("155558");
////     add_mac("155554");
////     add_mac("155556");
//
//    std::wstring strReadNumber(L"S123456"),mstrNumberPrefix(L"S1");
//   if (NULL != strReadNumber.compare(0,mstrNumberPrefix.length(),mstrNumberPrefix))
//   {
//       return -1;
//   }
//
//    return 0;
//    HANDLE hFile =  CreateFileW(L"D://modemst1.bin", GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
// 
//     unsigned char szFile[1024] = {0};
// 
//     for (int i = 0;i < 1024;i ++)
//     {
//         szFile[i] = '\0';
//     }
//     DWORD dwWrite = 1024;
// 
//     for (int i = 0;i < 1024;i++)
//     {
//         WriteFile(hFile,&szFile,1024,&dwWrite,NULL);
//     }
// 
//     CloseHandle(hFile);
//     return 0;
    //std::wstring strSource (L"123\n4656\r\n\r\n58855");
    //JGW_EraseFristToRemoveCharW(strSource,L'\n');
    //JGW_EraseAfterToRemoveCharW(strSource,L'\n');
    //JGW_EraseLastAndFristChars(strSource,'\r');
    //JGW_EraseLastAndFristChars(strSource,'\n');
    //JGW_EraseLastAndFristChars(strSource,'\r');
    //JGW_EraseLastAndFristChars(strSource,'\n');
    //return 0;
//     std::wstring strRegex = L"adb shell am broadcast -a com.jaguar.action.FACTORY_TEST_SERVER_START\r\n Broadcasting: Intent { act=com.jaguar.action.FACTORY_TEST_SERVER_START } \r\nBroadcast completed: result=0\r\n  E:\\project\\JaGuarWaveTestPlatform_Debug\\bin\\debug\\exec_debug>";
//     std::tr1::wregex regExpress(L"adb shell",std::tr1::regex_constants::icase);
//     if(regex_match(strRegex.c_str(),std::tr1::wregex(L"FACTORY_TEST_SERVER_START", std::tr1::wregex::icase))) 
//     {
//         return 0;
//     }
// 
//     std::cout << regex_match("aaaAAA", std::tr1::regex("a*", std::tr1::regex::icase)) << std::endl;   //结果为1
// 
//     return 0;
//     CTSE_SequenceManageInterface* pSequenceManageInterface = NULL;
//     {
//         std::wcout.imbue(std::locale("chs"));
//         std::cout.imbue(std::locale("chs"));
// 
//         CTSE_SequencePocoClassLoader<CTSE_SequenceManageInterface> mTestSuitePocoClassLoader;
//         std::string strDllPath = JGW_W2A_W(JGW_GetApplicationFolder());
//         strDllPath += "TSE_SequenceManagerPlugin.dll";
//         mTestSuitePocoClassLoader.LoadTestSuiteModule(strDllPath);
// 
//         pSequenceManageInterface = mTestSuitePocoClassLoader.CreateTestSuitePtrClass(std::string("CTSE_SequenceManageExport"));
// 
//         CTestLog testLog;
//         CTSE_SequenceManageImpl* pSequenceManageImpl= pSequenceManageInterface->GetSequenceManageImpl();
//         pSequenceManageImpl->GetGlobalEnvironmentImpl()->PutInt(MULTIUNIT_TSE_TEST,0);
//         pSequenceManageImpl->SetOnLogListener(&testLog);
//         pSequenceManageImpl->SetOnTestStatusListener(&testLog);
//         pSequenceManageImpl->GetSequenceTestImpl()->LoadSequenceTest("E:\\project\\JaGuarWaveTestPlatform_Debug\\bin\\debug\\exec_debug\\cache\\TSE_DB\\suite.xml");
// 
//         pSequenceManageImpl->GetSequenceTestImpl()->InitSequenceTest();
// 
//         //pSequenceManageImpl->GetSequenceTestImpl()->UnLoadSequenceTest();
//         pSequenceManageImpl->GetSequenceTestImpl()->ExecuteSequenceTest(true);
//        //! pSequenceManageInterface->ReleaseSequenceManageImpl(pSequenceManageImpl);
//     }
//     Poco::File file("C:\\JW\\exec\\TestLog");





/*    JGW::JGW_TraverseFolder_C(strPath,numFiles,"\\*.*"/ *"/ *QSEQ*.html"* /,ScanPuamThroughputTestResultFile,&strFile);*/
//     Poco::Crypto::DigestEngine engine("MD5");
//     std::ifstream fileStream;
//     char szBuffer[1024];
//     int read_size = 0;
//     fileStream.imbue( std::locale("chs") );
//     fileStream.open("C:\\JW\\exec\\log\\global\\2019-02-26\\global.log",std::ios_base::in | std::ios_base::binary );
//     if (!fileStream.is_open()) return 0;
// 
//     while(!fileStream.eof())
//     {
//         fileStream.read(szBuffer,1024);
//         read_size = fileStream.gcount();
//         engine.update(szBuffer,read_size);
//     }
//     std::string md5 = Poco::Crypto::DigestEngine::digestToHex(engine.digest());

    return 0;
}