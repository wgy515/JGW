#include "stdafx.h"
#include "cmdhandler.h"
#include <tchar.h>
#include <string>
#ifdef WIN32
#include <Tlhelp32.h>
#endif
#include <algorithm>
#include <JGW_FoundationFunc/JGW_StringFunc.h>


#define EXCEPTIION_STATE_CHECK \
    if (!m_bInit) return E_NOTIMPL

CCmdHandler::CCmdHandler()
    : m_bInit(FALSE)
    , m_dwErrorCode(0)
    , m_hPipeRead(NULL)
    , m_hPipeWrite(NULL)
    , m_port(PORT)
{
    ZeroMemory(m_szReadBuffer, sizeof(m_szReadBuffer));
    ZeroMemory(m_szWriteBuffer, sizeof(m_szWriteBuffer));
    ZeroMemory(&m_CommandParam, sizeof(m_CommandParam));
}
CCmdHandler::~CCmdHandler()
{
}

HRESULT CCmdHandler::Cli_Initalize()
{
    // 初始化，创建匿名管道
    if (m_bInit) return S_OK;
    m_bInit = TRUE;
    ZeroMemory(m_szReadBuffer, sizeof(m_szReadBuffer));
    ZeroMemory(&m_saOutPipe, sizeof(m_saOutPipe));
    m_saOutPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
    m_saOutPipe.lpSecurityDescriptor = NULL;
    m_saOutPipe.bInheritHandle = TRUE;
    ZeroMemory(&m_startupInfo, sizeof(STARTUPINFO));
    ZeroMemory(&m_processInfo, sizeof(PROCESS_INFORMATION));
    if (!CreatePipe(&m_hPipeRead, &m_hPipeWrite, &m_saOutPipe, PIPE_BUFFER_SIZE))
    {
        m_dwErrorCode = GetLastError();
        return E_FAIL;
    }
    return S_OK;
}

HRESULT CCmdHandler::Ser_Initalize(DWORD port)
{
    m_port = port;
    // 初始化，创建匿名管道
    if (m_bInit) return S_OK;
    m_bInit = TRUE;
    ZeroMemory(m_szReadBuffer, sizeof(m_szReadBuffer));
    ZeroMemory(&m_saOutPipe, sizeof(m_saOutPipe));
    m_saOutPipe.nLength = sizeof(SECURITY_ATTRIBUTES);
    m_saOutPipe.lpSecurityDescriptor = NULL;
    m_saOutPipe.bInheritHandle = TRUE;
    ZeroMemory(&m_startupInfo, sizeof(STARTUPINFO));
    ZeroMemory(&m_processInfo, sizeof(PROCESS_INFORMATION));
    if (!CreatePipe(&m_hPipeRead, &m_hPipeWrite, &m_saOutPipe, PIPE_BUFFER_SIZE))
    {
        m_dwErrorCode = GetLastError();
        printf("CreatePipe failed!\n");
        return E_FAIL;
    }

    //检测系统中是否已开启iperf3服务
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        printf("error:invalid handle value!\n");
        return E_FAIL;
    }
    PROCESSENTRY32 pi;
    pi.dwSize = sizeof(PROCESSENTRY32); //第一次使用必须初始化成员
    BOOL bRet = Process32First(hSnapshot, &pi);
    std::string item("iperf3.exe");
    while (bRet)
    {
        std::string process(JGW::JGW_W2A_W(pi.szExeFile));
        transform(process.begin(), process.end(), process.begin(), ::tolower);

        if (process.find(item) <= process.length())//find
        {
            HANDLE hProcess;
            //printf("Find the process! id:%d\n", pi.th32ProcessID);
            hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pi.th32ProcessID);
            if (hProcess)
            {
                printf("Close %d precess successful!\n", pi.th32ProcessID);
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);//OpenProcess打开的也要关闭  
            }
            else
            {
                printf("Close iperf precess failed!\n");
            }
        }

        bRet = Process32Next(hSnapshot, &pi);
    }//close all iperf3 services


    CloseHandle(hSnapshot);
    return S_OK;
}

HRESULT CCmdHandler::Finish()
{
    EXCEPTIION_STATE_CHECK;
    if (m_hPipeRead)
    {
        CloseHandle(m_hPipeRead);
        m_hPipeRead = NULL;
    }
    if (m_hPipeWrite)
    {
        CloseHandle(m_hPipeWrite);
        m_hPipeWrite = NULL;
    }
    return S_OK;
}
HRESULT CCmdHandler::HandleCommand(CHCmdParam* pCommmandParam)
{
    EXCEPTIION_STATE_CHECK;
    if (!pCommmandParam || pCommmandParam->iSize != sizeof(CHCmdParam))
        return E_INVALIDARG;
    if (strlen(pCommmandParam->szCommand) <= 0)
        return E_UNEXPECTED;
    memset(&m_CommandParam, 0, sizeof(m_CommandParam));
    m_CommandParam = *pCommmandParam;
    return ExecuteCmdWait();
}
HRESULT CCmdHandler::ExecuteCmdWait()
{
    EXCEPTIION_STATE_CHECK;
    HRESULT hResult = E_FAIL;
    DWORD dwReadLen = 0;
    DWORD dwStdLen = 0;
    m_startupInfo.cb = sizeof(STARTUPINFO);
    m_startupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    m_startupInfo.hStdOutput = m_hPipeWrite;
    m_startupInfo.hStdError = m_hPipeWrite;
    m_startupInfo.wShowWindow = SW_HIDE;
    DWORD dTimeOut = (DWORD)m_CommandParam.iTimeOut;// >= 3000 ? m_CommandParam.iTimeOut : 5000;

    std::wstring strCommand = JGW::JGW_A2W_A(m_CommandParam.szCommand);

    if (!CreateProcess(NULL, const_cast<wchar_t*>(strCommand.data()),
        NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL,
        &m_startupInfo, &m_processInfo))
    {
        m_dwErrorCode = GetLastError();
        hResult = E_FAIL;
        goto over;
    }
    //等待时间到达或进程退出，进程不退出将一直读取管道数据（每隔dTimeOut读取一次）
    while (WAIT_TIMEOUT == WaitForSingleObject(m_processInfo.hProcess, dTimeOut))
    {
        // 预览管道中数据的内容
        if (!PeekNamedPipe(m_hPipeRead, NULL, 0, NULL, &dwReadLen, NULL)
            || dwReadLen <= 0)
        {
            m_dwErrorCode = GetLastError();
            hResult = E_FAIL;
            continue;
        }
        else
        {
            ZeroMemory(m_szPipeOut, sizeof(m_szPipeOut));
            // 读取管道中的数据
            if (ReadFile(m_hPipeRead, m_szPipeOut, dwReadLen, &dwStdLen, NULL))
            {
                hResult = S_OK;
                if (m_CommandParam.OnCmdEvent)
                    m_CommandParam.OnCmdEvent(&m_CommandParam, S_OK, m_szPipeOut);
                //break;
            }
            else
            {
                m_dwErrorCode = GetLastError();
                //break;
            }
        }
    }
    // 预览管道中数据的内容
    if (!PeekNamedPipe(m_hPipeRead, NULL, 0, NULL, &dwReadLen, NULL)
        || dwReadLen <= 0)
    {
        m_dwErrorCode = GetLastError();
        hResult = E_FAIL;
        //continue;
    }
    else
    {
        ZeroMemory(m_szPipeOut, sizeof(m_szPipeOut));
        // 读取管道中的数据
        if (ReadFile(m_hPipeRead, m_szPipeOut, dwReadLen, &dwStdLen, NULL))
        {
            hResult = S_OK;
            if (m_CommandParam.OnCmdEvent)
                m_CommandParam.OnCmdEvent(&m_CommandParam, S_OK, m_szPipeOut);
            //break;
        }
        else
        {
            m_dwErrorCode = GetLastError();
            //break;
        }
    }


over:

    if (m_processInfo.hThread)
    {
        CloseHandle(m_processInfo.hThread);
        m_processInfo.hThread = NULL;
    }
    if (m_processInfo.hProcess)
    {
        CloseHandle(m_processInfo.hProcess);
        m_processInfo.hProcess = NULL;
    }
    return hResult;
}
