#include "stdafx.h"
#include <JGW_NIGPIBControllImpl/EquipCommtl_TcpClient.h>
#include <iostream>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <conio.h>
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_WindowsFuncPlugin/CJGW_AdapterCommonInfo.h>
#include <JGW_FoundationFunc/JGW_ModuleEnCrypt.h>
#include <JGW_CryptPlugin/CJGW_CryptDes.h>
struct jw_device_info 
{
    char model[32];         //项目型号：JW-PTP6030
    char mfr[32];           //生产厂商：JAGUARWAVE
    char sn[32];            //出厂写入SN号，与包装盒上一致：JGWT17101520
    char smt_sn[32];        //SMT写入SN号：JGWT17101520
    char mac[32];           //MAC地址，通常是有线网口的MAC, "B64C8436DC3D"
    char ssid_60g[32];      //Wireless_60G SSID, "PTP_60G_36DC3D"
    char password_60g[32];  //Wireless_60G 密码, "123456789"
    char ssid_5g[32];       //Wireless_5G SSID, "JW_5G_36DC3D"
    char password_5g[32];   //Wireless_5G 密码, "123456789"
    char ssid_2_4g[32];     //Wireless_2.4G SSID, "JW_2.4G_36DC3D"
    char password_2_4g[32]; //Wireless_2.4G 密码, "123456789"
    char user_mode;         //1 : 为用户模式。0：工厂模式（默认为0）
    char reserve[1695];     //预留字段，device_info.bin总文件大小2KB
};
//Base device_info.txt
 //   [device_info] 
///model=JW-IMS1030B
// mfr=JAGUARWAVE
//     sn=JGWT123456789
//     user_mode=0

static const unsigned int Te0[256] = {
    0xc66363a5U, 0xf87c7c84U, 0xee777799U, 0xf67b7b8dU,
    0xfff2f20dU, 0xd66b6bbdU, 0xde6f6fb1U, 0x91c5c554U,
    0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
    0xe7fefe19U, 0xb5d7d762U, 0x4dababe6U, 0xec76769aU,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
    0xeffafa15U, 0xb25959ebU, 0x8e4747c9U, 0xfbf0f00bU,
    0x41adadecU, 0xb3d4d467U, 0x5fa2a2fdU, 0x45afafeaU,
    0x239c9cbfU, 0x53a4a4f7U, 0xe4727296U, 0x9bc0c05bU,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
    0x6c36365aU, 0x7e3f3f41U, 0xf5f7f702U, 0x83cccc4fU,
    0x6834345cU, 0x51a5a5f4U, 0xd1e5e534U, 0xf9f1f108U,
    0xe2717193U, 0xabd8d873U, 0x62313153U, 0x2a15153fU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
    0x30181828U, 0x379696a1U, 0x0a05050fU, 0x2f9a9ab5U,
    0x0e070709U, 0x24121236U, 0x1b80809bU, 0xdfe2e23dU,
    0xcdebeb26U, 0x4e272769U, 0x7fb2b2cdU, 0xea75759fU,
    0x1209091bU, 0x1d83839eU, 0x582c2c74U, 0x341a1a2eU,
    0x361b1b2dU, 0xdc6e6eb2U, 0xb45a5aeeU, 0x5ba0a0fbU,
    0xa45252f6U, 0x763b3b4dU, 0xb7d6d661U, 0x7db3b3ceU,
    0x5229297bU, 0xdde3e33eU, 0x5e2f2f71U, 0x13848497U,
    0xa65353f5U, 0xb9d1d168U, 0x00000000U, 0xc1eded2cU,
    0x40202060U, 0xe3fcfc1fU, 0x79b1b1c8U, 0xb65b5bedU,
    0xd46a6abeU, 0x8dcbcb46U, 0x67bebed9U, 0x7239394bU,
    0x944a4adeU, 0x984c4cd4U, 0xb05858e8U, 0x85cfcf4aU,
    0xbbd0d06bU, 0xc5efef2aU, 0x4faaaae5U, 0xedfbfb16U,
    0x864343c5U, 0x9a4d4dd7U, 0x66333355U, 0x11858594U,
    0x8a4545cfU, 0xe9f9f910U, 0x04020206U, 0xfe7f7f81U,
    0xa05050f0U, 0x783c3c44U, 0x259f9fbaU, 0x4ba8a8e3U,
    0xa25151f3U, 0x5da3a3feU, 0x804040c0U, 0x058f8f8aU,
    0x3f9292adU, 0x219d9dbcU, 0x70383848U, 0xf1f5f504U,
    0x63bcbcdfU, 0x77b6b6c1U, 0xafdada75U, 0x42212163U,
    0x20101030U, 0xe5ffff1aU, 0xfdf3f30eU, 0xbfd2d26dU,
    0x81cdcd4cU, 0x180c0c14U, 0x26131335U, 0xc3ecec2fU,
    0xbe5f5fe1U, 0x359797a2U, 0x884444ccU, 0x2e171739U,
    0x93c4c457U, 0x55a7a7f2U, 0xfc7e7e82U, 0x7a3d3d47U,
    0xc86464acU, 0xba5d5de7U, 0x3219192bU, 0xe6737395U,
    0xc06060a0U, 0x19818198U, 0x9e4f4fd1U, 0xa3dcdc7fU,
    0x44222266U, 0x542a2a7eU, 0x3b9090abU, 0x0b888883U,
    0x8c4646caU, 0xc7eeee29U, 0x6bb8b8d3U, 0x2814143cU,
    0xa7dede79U, 0xbc5e5ee2U, 0x160b0b1dU, 0xaddbdb76U,
    0xdbe0e03bU, 0x64323256U, 0x743a3a4eU, 0x140a0a1eU,
    0x924949dbU, 0x0c06060aU, 0x4824246cU, 0xb85c5ce4U,
    0x9fc2c25dU, 0xbdd3d36eU, 0x43acacefU, 0xc46262a6U,
    0x399191a8U, 0x319595a4U, 0xd3e4e437U, 0xf279798bU,
    0xd5e7e732U, 0x8bc8c843U, 0x6e373759U, 0xda6d6db7U,
    0x018d8d8cU, 0xb1d5d564U, 0x9c4e4ed2U, 0x49a9a9e0U,
    0xd86c6cb4U, 0xac5656faU, 0xf3f4f407U, 0xcfeaea25U,
    0xca6565afU, 0xf47a7a8eU, 0x47aeaee9U, 0x10080818U,
    0x6fbabad5U, 0xf0787888U, 0x4a25256fU, 0x5c2e2e72U,
    0x381c1c24U, 0x57a6a6f1U, 0x73b4b4c7U, 0x97c6c651U,
    0xcbe8e823U, 0xa1dddd7cU, 0xe874749cU, 0x3e1f1f21U,
    0x964b4bddU, 0x61bdbddcU, 0x0d8b8b86U, 0x0f8a8a85U,
    0xe0707090U, 0x7c3e3e42U, 0x71b5b5c4U, 0xcc6666aaU,
    0x904848d8U, 0x06030305U, 0xf7f6f601U, 0x1c0e0e12U,
    0xc26161a3U, 0x6a35355fU, 0xae5757f9U, 0x69b9b9d0U,
    0x17868691U, 0x99c1c158U, 0x3a1d1d27U, 0x279e9eb9U,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
    0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x2d9b9bb6U, 0x3c1e1e22U, 0x15878792U, 0xc9e9e920U,
    0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x038c8c8fU, 0x59a1a1f8U, 0x09898980U, 0x1a0d0d17U,
    0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
    0x824141c3U, 0x299999b0U, 0x5a2d2d77U, 0x1e0f0f11U,
    0x7bb0b0cbU, 0xa85454fcU, 0x6dbbbbd6U, 0x2c16163aU,
};

void encrypt(char* buf,size_t buf_len)
{
    int keyLen = _countof(Te0);
    for (int i = 0;i < buf_len;i ++)
    {
        buf[i] ^= Te0[i >= keyLen ? i % keyLen : i];
    }
}

void decrypt(char* buf,size_t buf_len)
{
    int keyLen = _countof(Te0);
    for (int i = 0;i < buf_len;i ++)
    {
        buf[i] ^= Te0[i >= keyLen ? i % keyLen : i];
    }
}
// if != 0,then there is data to be read on stdin
//int kbhit()
//{
//    // timeout structure passed into select
//    struct timeval tv;
//    // fd_set passed into select
//    fd_set fds;
//    // Set up the timeout.  here we can wait for 1 second
//    tv.tv_sec = 1;
//    tv.tv_usec = 0;
//
//    // Zero out the fd_set - make sure it's pristine
//    FD_ZERO(&fds);
//    // Set the FD that we want to read
//   
//    FD_SET( _fileno(stdin),&fds); //STDIN_FILENO is 0
//    // select takes the last file descriptor value + 1 in the fdset to check,// the fdset for reads,writes,and errors.  We are only passing in reads.
//    // the last parameter is the timeout.  select will return if an FD is ready or 
//    // the timeout has occurred
//    select( _fileno(stdin)+1,&fds,NULL,NULL,&tv);
//    // return 0 if STDIN is not ready to be read.
//    return FD_ISSET( _fileno(stdin),&fds);
//}

void ProcessStdin(void)
{
    INPUT_RECORD record;
    DWORD numRead;
    BOOL result = ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &numRead);
    result = !result;
    char szBuf[1024] = {0};
    //getline(std::cin, szBuf, "}\n");
    //if(!result) {
    //    // hmm handle this error somehow...
    //    return;
    //}
    if(record.EventType != KEY_EVENT) {
        // don't care about other console events
        return;
    }

    if(!record.Event.KeyEvent.bKeyDown) {
        // really only care about keydown
        return;
    }

    // if you're setup for ASCII, process this:
    //record.Event.KeyEvent.uChar.AsciiChar

} // end ProcessStdin
#pragma comment(lib, "Ws2_32.lib")
using namespace std;

bool RecvTestInfo(JGW::CCJGW_ProcessPipe& m_pDownloadPipe)
{
    int readSize = 0;
    std::wstring strTemp;
    //! 如果十分钟还未成功则表示超时
    while(1)
    {
        std::wcin >> strTemp;
        m_pDownloadPipe.send(strTemp);
        std::wcout << strTemp << std::endl;
        ///m_pDownloadPipe.recv(strTemp);
        Sleep(50);
    }
    return false;
}
typedef const unsigned int* (*GetDecryptDataFunc)();
int main(int argc, char** argv)
{
    std::string strDecrypt = JGW::CCJGW_CryptDes::EncryptDESCode("JaGuarWave202009041703","5DA8B82F4D48","71DC1F03");
    //L"F:\\project\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug"
    /*HMODULE module = LoadLibrary(L"F:\\project\\git_code_debug\\FTM-tools\\JaGuarWaveTestPlatform\\bin\\debug\\exec_debug\\JaGuarWaveTestPlatform_d.exe");
    GetDecryptDataFunc func = (GetDecryptDataFunc)GetProcAddress(module,"x2x31313axaxazzadasdazz");
    func();*/
    //JGW::EncryptModuleIsLoaded();
    //GetCurrentProcessId();
    /*JGW::CCJGW_AdapterCommonInfo adpater;
    std::vector<JGW::NetWorkConection>& vNetWorkConection = adpater.GetNetAdaptersInfo();
    DWORD dwSpeed = 0;
    for (size_t i = 0;i < vNetWorkConection.size(); i ++)
    {
    if (0 == vNetWorkConection[i].ip_address.compare(L"192.168.1.5"))
    {
    dwSpeed = adpater.GetNetAdaptersSpeedToDescription(vNetWorkConection[i].description);
    }
    }*/

    //JGW::CCJGW_ProcessPipe m_pDownloadPipe;
    //m_pDownloadPipe.InitProcessPipe(L"JGWPerasoComPort.exe",L"",JGW::JGW_GetApplicationFolder());
    //if (m_pDownloadPipe.CheckProcessIsRun())
    //{
    //    RecvTestInfo(m_pDownloadPipe);
    //}
   
   return 0;


    static HANDLE stdinHandle;
    // Get the IO handles
    // getc(stdin);
    stdinHandle = GetStdHandle(STD_INPUT_HANDLE);

    while( 1 )
    {
        switch( WaitForSingleObject( stdinHandle, 1000 ) )
        {
        case( WAIT_TIMEOUT ):
            cerr << "timeout" << endl;
            break; // return from this function to allow thread to terminate
        case( WAIT_OBJECT_0 ):
            if( _kbhit() ) // _kbhit() always returns immediately
            {
                int i = _getch();
                cerr << "key: " << i << endl;
            }
            else // some sort of other events , we need to clear it from the queue
            {
                // clear events
                INPUT_RECORD r[512];
                DWORD read;
                ReadConsoleInput( stdinHandle, r, 512, &read );
                cerr << "mouse event" << endl;
            }
            break;
        case( WAIT_FAILED ):
            cerr << "WAIT_FAILED" << endl;
            break;
        case( WAIT_ABANDONED ): 
            cerr << "WAIT_ABANDONED" << endl;
            break;
        default:
            cerr << "Someting's unexpected was returned.";
        }
    }

    return 0;



    std::string inStr;
    DWORD fdwMode, fdwOldMode;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hStdIn, &fdwOldMode);
    // disable mouse and window input
    fdwMode = fdwOldMode ^ ENABLE_MOUSE_INPUT ^ ENABLE_WINDOW_INPUT;
    SetConsoleMode(hStdIn, fdwMode);
    // flush to remove existing events
    FlushConsoleInputBuffer(hStdIn);
    while (1)
    {
        if (WaitForSingleObject(hStdIn, 1000000000) == WAIT_OBJECT_0)
        {
            std::getline(std::cin, inStr);
        }
    }
    // restore console mode when exit
    SetConsoleMode(hStdIn, fdwOldMode);
    //HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    //int timer = GetTickCount();
    //std::string msg;
    //while(timer+10000 > GetTickCount())
    //{
    //    unsigned int length = 0;
    //    DWORD bytesAvailable = 0; 
    //    PeekNamedPipe(hStdin,NULL,0,NULL,&bytesAvailable,NULL);
    //    if(bytesAvailable > 0)
    //    {
    //        for (int i = 0; i < 4; i++)
    //        {
    //            unsigned int read_char = getchar();
    //            length = length | (read_char << i*8);
    //        }


    //        for (int i = 0; i < length; i++)
    //        {
    //            msg += getchar();
    //        }
    //        timer = GetTickCount();
    //    }
    //    else
    //    {
    //        // nothing to read, stdin empty
    //        getchar();
    //    }
    //}


    /*char Buf[1024] = {0};
    INPUT_RECORD record = {0};
    DWORD numRead = 0;

    while (1)
    {
       ///BOOL result = ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &numRead);
        int i = _kbhit();
       if (i)
       {
         
              char* pbuf = gets(Buf);
       }
       Sleep(1 * 1000);
    }*/
    
    //HANDLE eventHandles[] = {
    //    GetStdHandle(STD_INPUT_HANDLE)
    //    // ... add more handles and/or sockets here
    //};

    //DWORD result = WSAWaitForMultipleEvents(sizeof(eventHandles)/sizeof(eventHandles[0]), 
    //    &eventHandles[0], 
    //    FALSE, 
    //    1000, 
    //    TRUE
    //    );

    //switch(result) {
    //case WSA_WAIT_TIMEOUT: // no I/O going on right now
    //    break;

    //case WSA_WAIT_EVENT_0 + 0: // stdin at array index 0
    //    ProcessStdin();
    //    break;

    //case WSA_WAIT_EVENT_0 + 1: // handle/socket at array index 1
    //    break;

    //case WSA_WAIT_EVENT_0 + 2: // ... and so on
    //    break;

    //default: // handle the other possible conditions
    //    break;
    //} // end switch result


    return -1;

    char szBuf[2048] = {0};
    jw_device_info* dev = (jw_device_info*)szBuf;
    memset(dev->model,0x00,32);
    memcpy_s(dev->model,32,"JW-IMS1030B00000000000000000",32);
    
    memcpy_s(dev->mfr,32,"JAGUARWAVE",strlen("JAGUARWAVE"));
    memcpy_s(dev->sn,32,"JGWT123456789",strlen("JGWT123456789"));
    dev->user_mode = '0';
    //JGW::JGW_WriteFileToBuf("base.bin",szBuf,2048);

    
    JGW::JGW_WriteFileToBuf("base.bin",szBuf,2048);
    /*
    2.Relay device_info.txt
    [device_info] 
    model=JW-IMS1020R
    mfr=JAGUARWAVE
    sn=JGWT123456788
    user_mode=0
    */

    /*memset(szBuf,0x00,2048);
    memcpy_s(dev->model,32,"JW-IMS1020R",strlen("JW-IMS1020R"));
    memcpy_s(dev->mfr,32,"JAGUARWAVE",strlen("JAGUARWAVE"));
    memcpy_s(dev->sn,32,"JGWT123456788",strlen("JGWT123456788"));
    dev->user_mode = '0';*/
    JGW::JGW_WriteFileToBuf("relay.bin",szBuf,2048);
#if 0
    std::string host = "127.0.0.1";
    unsigned int port = 5026;
    std::string source = "";
    std::string strRead,strOut;

    if (argc > 1)
    {
        host = argv[1];
    }
    if (argc > 2)
    {
        port = atoi(argv[2]);
    }
    if (argc > 3)
    {
        source = argv[3];
    }

    std::cout << "host:" << host << std::endl;
    std::cout << "port:" << port << std::endl;
    std::cout << "source:" << source << std::endl;

    JGW::CEquipCommtl_TcpClient equip(host,port,source);
    if (!equip.IsPresent())
    {
        return false;
    }

    while (1)
    {
        getline(std::cin,strOut);
        if (std::string::npos != strOut.find("QUIT") || std::string::npos != strOut.find("quit") || std::string::npos != strOut.find("exit") || std::string::npos != strOut.find("EXIT")) break;
        //! sp.ReadRespCommandToWriteCommand(strOut.c_str(),strRead);
        if (std::string::npos != strOut.find('?'))
        {
            equip.query(strOut,strRead);
            std::cout << strRead;
        }
        else
        {
            equip.write(strOut);
        }
        std::cin.clear();
    }
#else



    int index = 0,maxIndex = (2482-2412)/5 + 1 + 1 + (7200 - 4900) / 5 + 1;
    double dFreqArray[(2482-2412)/5 + 1 + 1 + (7200 - 4900) / 5 + 1] = {0};
    for (index = 0;index < (2482-2412)/5 + 1;index ++)
    {
        dFreqArray[index] = 2412 + 5 * index;
    }
    dFreqArray[index++] = 2484;
    for (int i = 0;i < (7200 - 4900)/5 + 1;index ++,i ++)
    {
        dFreqArray[index] = 4900 + 5 * i;
    }
#endif
    return -1;
}