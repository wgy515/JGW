// CurlTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*#include "CJGW_CurlFtp.h"*/
#include <Windows.h>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include "CJGW_CurlHttp.h"
/*#include "CJGW_HelpSqlite3.h"*/
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <regex>
using namespace JGW;

void RefreshWindow(HWND windowHandle)
{
    RECT rect;
    GetClientRect(windowHandle, &rect);
    for (LONG x = 0; x < rect.right; x += 5)
        for (LONG y = 0; y < rect.bottom; y += 5)
            SendMessage(windowHandle, WM_MOUSEMOVE, 0, (y << 16) + x);
}
//! curl -H "Content-Type:application/json" -X POST --data {\"requestCode\":100} "http://192.168.1.1/cgi-bin/leopard.fcgi"


double AveragePowResult(std::vector<double> vdTestValue)
{
    if (vdTestValue.empty())
    {
        //PHONE->mbResult = false;
        return -999.9;
    }
    double averageResult = 0.0;
    double num = 20.0;
    int num2 = 0;
    for (size_t i = 0; i < vdTestValue.size(); i++)
    {

        if (vdTestValue[i] > -999.0 && (i != 0 || vdTestValue.size() <= 1 || std::abs(vdTestValue[0] - vdTestValue[1]) <= 3.0))
        {
            num2++;
            averageResult += std::pow(10.0, vdTestValue[i] / num);
        }
    }

    if (num2 <= 0)
    {
        averageResult = -999.0;
        return averageResult;
    }
    averageResult /= (double)num2;
    if (0.0 != averageResult)
    {
        averageResult = num * std::log10(averageResult);
        return averageResult;
    }
    averageResult = 0.0;
    return averageResult;
}

class A {
public:
    A() {
        std::cout << "This is A1." << std::endl;
    }
    ~A() {
        print();
    }
    void print() {
        std::cout << "This is A." << std::endl;
    }
};
class B : public A{
public:
    B() {
        std::cout << "This is B1." << std::endl;
    }
    ~B() {
        print();
    }
    void print() {
        std::cout<<"This is B."<<std::endl;
    }
};

void Test(void){
    char *str = (char *) malloc(100);
    strcpy(str, "hello");
    free(str);
    if(str != NULL){
        strcpy(str, "world");
        printf(str);
    }
}
typedef unsigned int uint32 ;

typedef struct
{
    unsigned char start_flag; //!= 0x7F
    union U_ID{
        uint32 process_id;
        uint32 thread_id;
        uint32 index_id;
    }uid;
    unsigned short msg_size;
    char msg[1]; //! msg = json
    unsigned char end_flag; //!= 0x7D
}S_TSE_MESSAGE;

enum E_NETCARD_PHYSICAL_STATE
{
    //! 网卡物理断开连接
    E_NETCARD_PHYSICAL_DISCONNECT = 0,
    //! 网卡物理插入
    E_NETCARD_PHYSICAL_CONNECT 
};

enum E_USB_PHYSICAL_STATE
{
    //! USB物理断开连接
    E_USB_PHYSICAL_DISCONNECT = 0,
    //! USB物理插入
    E_USB_PHYSICAL_CONNECT 
};

enum E_USB_DEVICE_MODE
{
    E_DEVICE_UNKNOWMODE = 0,
    E_ADB_MODE = 1,
    E_DIAG_MODE = 2,
    E_MODEM_MODE = 4,
    E_FASTBOOT_MODE = 8,
    E_DOWNLOADER_MODE = 16,
    E_LAN_MODE = 32,
    E_ALL_MODE = 255,
};

typedef struct
{
    //! 
    E_USB_PHYSICAL_STATE e_usb_physical_state;
    //!
    E_USB_DEVICE_MODE e_usb_device_mode;
    //! 平台排序索引 -1 未排序
    int	mnSortIndex;
    //! 设备COM口
    int mnComPort;
    //! dbcc name
    std::wstring mstrDbcc_name;//\\?\USB#Vid_05c6&Pid_9091&MI_00#7&30607750&1&0000#{86e0d1e0-8089-11d0-9ce4-08003e301f73}
    //! reg usb pid vid
    std::wstring mstrRegUsbPidVid;//! USB\Vid_05c6&Pid_9008\6&7c2d8f9&0&5
    //! adb serial number
    std::wstring mstrSeralNumber;//! adb 或者 fastboot SerialNumber
}S_USB_INFO;

typedef struct
{
    //! 当前网卡物理状态
    E_NETCARD_PHYSICAL_STATE meNetCardPhysicalState;
    //! 平台排序索引 -1 未排序
    int	mnSortIndex;
    //! 引用计数
    //! CCJGW_ReferenceCount mReferenceCount;
    //! IP 地址
    std::wstring mstrIPAddr;
    //! 描述信息
    std::wstring mstrDescription;
    //! 适配器名称
    std::wstring mstrAdapterName;
    //! 适配器GUID
    std::wstring mstrNetGUID;
}S_NETCARD_INFO;

typedef struct
{
    unsigned char device_type;//! 0: USB 1:NETCARD
    union {
        S_USB_INFO* ps_usb_info;
        S_NETCARD_INFO* ps_netcard_info;
    }udevice_info;
}S_DNP_INFO;

typedef unsigned int uint;

int MaskActiveCount(unsigned int portMask)
{
    uint num = portMask & 1;
    while ((portMask >>= 1) != 0)
    {
        num += (portMask & 1);
    }
    return (int)num;
}

#define  INVAILD_WLAN_RESULT  -999.99

bool QueryResultWlanActiveMask(const std::string& query,std::vector<double>& vResult,int& dutPortMask)
	{
		std::string read;
		std::vector<std::string> vstrTemp;

        read = "0,1,3.025992632\r\n";
//#ifndef DEBUG_TEST
//		mpEqipCommtl->query(query,read);
//#else
//		read = "0,1,-11846.64941";
//#endif
		JGW_ParserStrA(read.c_str(),",",vstrTemp);
		if (vstrTemp.size() < 3)
		{
			vResult.push_back(INVAILD_WLAN_RESULT);
			return false;
		}
		dutPortMask = atoi(vstrTemp[1].c_str());
		size_t num = MaskActiveCount(atoi(vstrTemp[1].c_str()));
		//! 第一个返回值0 表示OK 检查返回的值和返回字串大小长度是否一致
		if (0 != atoi(vstrTemp[0].c_str())/*
										  || num < vstrTemp.size() - 2*/)
		{
			for (size_t i = 0;i < num;i ++)
			{
				vResult.push_back(INVAILD_WLAN_RESULT);   
			}
			return false;
		}
		else
		{
			for (size_t i = 0;i < num;i ++)
			{
				vResult.push_back(atof(vstrTemp[i + 2].c_str()));
			}
		}
		return true;
	}


bool BlockReturnUntilOperationCompleted(const std::string& command,uint timeMaxSec /* = 60 */,uint intervalMsec /* = 250 */)
{
    if (command.empty()) return false;

    std::string write (":QCOM:WLAN:SYST:OPC?"),read;
    bool flag = true;
    jgw_timer jt;

    //mpEqipCommtl->write(command);
    mpEqipCommtl->query(write,read);

    flag = (1 == atoi(read.c_str()));
    while (!flag && jt.elapsed() < timeMaxSec)
    {
        Sleep(intervalMsec);
        mpEqipCommtl->query(write,read);
        flag = (1 == atoi(read.c_str()));
    }
    return flag;
}

#include <poco/FileStream.h>
#include <poco/MemoryStream.h>

int _tmain(int argc, _TCHAR* argv[])
{
    std::vector<double> vdTemp;
    int dutPortMask = 1;
    bool bresult = QueryResultWlanActiveMask("",vdTemp,dutPortMask);
    MemoryInputStream

    //Poco::FileInputStream 	fos(file, std::ios::binary);
    //fos.getline()
    //return -1;

    ////int a[2][3]={{1,2,3},{4,5,6}};

    ////int m,*p=&a[0][0];

    ////m=(*p)*(*(p+2))*(*(p+4));

    ////char strss[] = "http://www.ibegroup.com/";
    ////int i = sizeof (strss);
    //////reinterpret_cast
    ////std::vector<double> vdTestValue;
    ////vdTestValue.push_back(9.46);
    //////vdTestValue.push_back(9.58);
    ////double averageResult = AveragePowResult(vdTestValue);
    ////double num = 20.0;
    ////double vdTestValue = 9.46;
    ////averageResult = std::pow(10.0, vdTestValue / num);


    //wchar_t str[512] = {0};
    //size_t len = 0;
    //wchar_t szFileMD5[33] = {0};
    //wcscpy_s(str,512,L"61e3966f901114cda61df27fcb1306ae C:\\JW\\exec\\TestLog\\UC600AA\\WIF_BT_FT\\wifi_bt_ft_suite.xml\\2020-12-22\\F__324764593.log");
    //wcscpy_s(szFileMD5,33,L"2111111111111111111");
    //wcsncpy_s(szFileMD5,33,str,32);









    std::wstring strRsp = L"18:d6:1c:8a:97:7d";
    std::wregex partten(L"^([0-9A-Fa-f]{2}[:]){5}([0-9A-Fa-f]{2})$");
    if (regex_match(strRsp,partten))
    {
        return 0;
    }
    std::wregex partten1(L"nand-ipq40xx-single.img",std::wregex::icase);
    if (regex_match(L"nand-ipq40xx-single.img",partten1))
    {
        return 0;
    }


    return -1;





    //CCJGW_CurlHttp curl;
    //std::wstring strUrl = L"http://192.168.1.1/cgi-bin/leopard.fcgi",strPostData = L"{\"requestCode\":100}",strResponce,strErrorMsg,strContentType = L"Content-Type:application/json;charset=UTF-8";
    //std::wstring strFilePath = L"F:\\download\\new",strRemoteFileName = L"UC600AAV01.10.10_DEV.zip";
    //bool result = curl.SendCurlHttpPostData(strUrl,strPostData,strResponce,strErrorMsg,strContentType);

    //strUrl = L"http://192.168.1.1/cgi-bin/upload.cgi?upload=2";
    //result = curl.UploadCurlHttpPostFile(strUrl,strFilePath,strResponce,strErrorMsg);

    return 0;
    //CCJGW_CurlFtp curlFtp("192.168.3.2","sa","123456",21);
    //CCJGW_CurlFtp curlFtp("192.168.8.10","jiebuser","Jie@2019!",26);
    //std::string error_msg;
    ////bool result = curlFtp.UploadCurlFtpFile("/Update/123.xml","d:\\upload.txt",error_msg);
    ////result = curlFtp.DownloadCurlFtpFile("/Update/123.xml","d:\\123.txt",error_msg);
    //bool result = curlFtp.CheckPathCurlFtpExist("/JaGuarWaveTestPlatform_Version/Update/update.ini",error_msg);
    //result = curlFtp.CheckPathCurlFtpExist("/JaGuarWaveTestPlatform_Version/Update/DHW206996/update.xml",error_msg);
    //return 0;

//     CCJGW_HelpSqlite3 helpSqliteDB;
//     S_TASK_SQL sTaskSQL;
//     helpSqliteDB.LoadHelpSqlite3("E:\\sqlite-tools-win32-x86-3330000\\help.db",L"DHW206996",sTaskSQL);














   // std::string mystring("\xe4\xb8\xad\xe6\x96\x87");//UTF-8编码的“中文”字符串
   // std::wstring_convert<std::codecvt_utf8<wchar_t>>cvt_utf8;//UTF-8<->Unicode转换器
   //std::wstring_convert<std::codecvt<wchar_t,char,std::mbstate_t>>cvt_ansi(new std::codecvt<wchar_t,char,std::mbstate_t>(""));//GBK<->Unicode转换器
   // std::wstring ws=cvt_utf8.from_bytes(mystring);//UTF-8转换为Unicode
   // //std::locale loc("zh-CN");
   // //mbstate_t state = {};
   // //int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
   // //    (loc).in(state,
   // //    strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
   // //    buff.data(), buff.data() + buff.size(), pwszNext);
   // //if (std::codecvt_base::ok == res)

   // std::string myansistr=cvt_ansi.to_bytes(L"即系统区域设置，即国家或地区设置");//Unicode转换为GBK
   // std::cout<<myansistr<<std::endl;

    /* std::wifstream propertyStream;
    propertyStream.imbue(std::locale("en_US.UTF-8"));
    propertyStream.open(L"E:\\工具包\\JGW_Log4cplusPropFileImplPlugin.dll");
    if (!propertyStream.good ()) 
    return false;


    std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
    std::wstring wTemp = cutf8.from_bytes("\xe4\xb8\xad\xe6\x96\x87");

    std::wstring_convert<std::codecvt<wchar_t,char,std::mbstate_t>> cvt_ansi(new std::codecvt<wchar_t,char,std::mbstate_t>("CHS"));
    std::string myansistr = cvt_ansi.to_bytes(wTemp);
    wTemp = cutf8.from_bytes(myansistr);*/
    //std::wstring_convert<std::codecvt_utf8<char>> autf8;
    //std::wstring Temp = autf8.from_bytes("\xe4\xb8\xad\xe6\x96\x87");
#if 0
    CCJGW_CurlFtp curlFtp("192.168.133.19","sa","123456",21);
    std::string error_msg;
    //bool result = curlFtp.UploadCurlFtpFile("/Update/123.xml","d:\\upload.txt",error_msg);
    //result = curlFtp.DownloadCurlFtpFile("/Update/123.xml","d:\\123.txt",error_msg);
    bool result = curlFtp.CheckPathCurlFtpExist("/Update2/123.xml",error_msg);
    result = curlFtp.CheckPathCurlFtpExist("/Update/126663.xml",error_msg);
    return 0;
#else
    HWND TrayWndHandle = FindWindowEx(NULL, NULL,L"Shell_TrayWnd",NULL);
    HWND TrayNotifyWndHandle = FindWindowEx(TrayWndHandle, NULL, L"TrayNotifyWnd", NULL);
    HWND SysPagerHandle = FindWindowEx(TrayNotifyWndHandle, NULL, L"SysPager", NULL);
    HWND NotifyAreaHandle = FindWindowEx(SysPagerHandle, NULL, L"ToolbarWindow32", NULL);
    RefreshWindow(NotifyAreaHandle);
    HWND OverHandle = FindWindowEx(NULL, NULL, L"NotifyIconOverflowWindow", NULL);
    HWND NotifyOverHandle = FindWindowEx(OverHandle,NULL, L"ToolbarWindow32", NULL);
    RefreshWindow(NotifyOverHandle);
    return 0;
#endif
}

