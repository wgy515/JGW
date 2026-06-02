#include "StdAfx.h"
#include "CMultipleTestManageThread.h"
#include "CMultipleTestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CCMultipleTestManageThread::CCMultipleTestManageThread(std::vector<S_TSE_MULTIPLE_TEST_THREAD_PARAM>& vsMultipleTestThreadParam) : mvsMultipleTestThreadParam(vsMultipleTestThreadParam),mbManageThread(false),mpsMultipleConfig(NULL),mbCheckTFPT32ProcessRuning(false)
    {
    }

    CCMultipleTestManageThread::~CCMultipleTestManageThread(void)
    {
        for (size_t index = 0;index < mvsMultipleTestThreadParam.size();index++)
        {
            if (NULL != mvsMultipleTestThreadParam[index].mpMultipleTestThread)
            {
                ((CCMultipleTestThread*)mvsMultipleTestThreadParam[index].mpMultipleTestThread)->ExitThread();
                delete (CCMultipleTestThread*)mvsMultipleTestThreadParam[index].mpMultipleTestThread;
                mvsMultipleTestThreadParam[index].mpMultipleTestThread = NULL;
            }
        }
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCMultipleTestManageThread,CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()


    void CCMultipleTestManageThread::StartTSEMultipleTestManageThread(PS_MULTIPLE_CONFIG psMultipleConfig)
    {
        mbManageThread = true;
        mpsMultipleConfig = psMultipleConfig;
    }

    void CCMultipleTestManageThread::StopTSEMultipleTestManageThread()
    {
        mbManageThread = false;
    }

    void CCMultipleTestManageThread::RunThread()
    {
        if (!mbManageThread || NULL == mpsMultipleConfig) return;
        CCMultipleTestThread* pMultipleTestThread  = NULL;
        
        for (size_t index = 0;index < mvsMultipleTestThreadParam.size();index++)
        {
            if (mvsMultipleTestThreadParam[index].mbIsNewDevicePlugIn)
            {
                if (!mbCheckTFPT32ProcessRuning) StartTFTP32Process();
                //! 1、设备插入检查当前测试项目是否正在测试
                //! 2、检查当前测试西线程是否为空
                if (mvsMultipleTestThreadParam[index].meTSEMPTestStatus == E_TSEMP_BUSY) {mvsMultipleTestThreadParam[index].mbIsNewDevicePlugIn = false;continue;}

                if (NULL != mvsMultipleTestThreadParam[index].mpMultipleTestThread)
                {
                    pMultipleTestThread = (CCMultipleTestThread*)mvsMultipleTestThreadParam[index].mpMultipleTestThread;
                }
                else
                {
                    pMultipleTestThread = new CCMultipleTestThread;
                    mvsMultipleTestThreadParam[index].mpMultipleTestThread = pMultipleTestThread;
                    pMultipleTestThread->InitTestParam(&mvsMultipleTestThreadParam[index]);
                    pMultipleTestThread->CreateMessageThread(0,0,true);
                }

                mvsMultipleTestThreadParam[index].meTSEMPTestStatus = E_TSEMP_BUSY;
                pMultipleTestThread->PostThreadMessage(DOWN_LOAD_THREAD_RUN_MSG);;
                mvsMultipleTestThreadParam[index].mbIsNewDevicePlugIn = false;
            }
        }
    }

    void CCMultipleTestManageThread::StartTFTP32Process()
    {
        DWORD processID = JGW_GetProcessID2Name(L"tftpd32.exe");
        if (processID != -1)
        {
            mbCheckTFPT32ProcessRuning = true;
            return ;
        }

        std::wstring strTFTP32ExecFilePath = JGW_RealativePathToAbsPath(L"Tftpd32\\tftpd32.exe");
        if (JGW_PathFileExists(strTFTP32ExecFilePath.c_str()))
        {
            JGW_CreateProcess(strTFTP32ExecFilePath.c_str(),JGW_GetPathOfFile(strTFTP32ExecFilePath.c_str()).c_str());
        }
        mbCheckTFPT32ProcessRuning = true;
    }
}
