#include "StdAfx.h"
#include "TSE_QualcommQCN.h"
#include "../TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "../TSE_DevicesGlobalResource.h"
/*#include <JGW_FoundationFunc/JGW_FilePath.h>*/

namespace JGW
{
    CTSE_QualcommQCN::CTSE_QualcommQCN(void) : mbIsReportLog(false),mbIsBackupQCN(false),mstrSPC("000000"),mstrSNEnvironment(L"TSE_READER_SN_NUMBER"),mstrQCNPath("c://tse.qcn")/*,mstrQCNEnvironment(L"TSE_QCN_FILE_PATH")*/
    {
    }


    CTSE_QualcommQCN::~CTSE_QualcommQCN(void)
    {
    }

    const wchar_t* CTSE_QualcommQCN::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_QualcommQCN\",\
                \"TestName\": \"备份或上传QCN(Qualcomm)\",\
                \"ReportLog\": \"是否上报QCN Log信息\",\
                \"BackupQCN\": \"是否备份QCN\",\
                \"SPC\": \"SPC值 默认000000\",\
                \"SNEnvironment\": \"SN环境变量\",\
                \"Environment\": \"备份或者还原QCN路径,注释掉此项,则保存在默认路径\"\
                }";
    }

    bool CTSE_QualcommQCN::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"ReportLog",strParamName))
        {
            mbIsReportLog = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"BackupQCN",strParamName))
        {
            mbIsBackupQCN = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SPC",strParamName))
        {
            mstrSPC = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SNEnvironment",strParamName))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Environment",strParamName))
        {
            mstrQCNEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_QualcommQCN::BuildQCNPath()
    {
        if (!mstrQCNEnvironment.empty())
        {
            mstrQCNPath = JGW_W2A(GetGlobalEnvironment()->GetString(mstrQCNEnvironment));
        }
        else
        {
            //! 还原QCN必须先选择还原的QCN
            if (!mbIsBackupQCN) return false;
            std::wstring strPath (JGW_GetApplicationFolder()),strSN = GetGlobalEnvironment()->GetString(mstrSNEnvironment);
            strPath += L"TestLog\\";
            strPath += GetGlobalEnvironment()->GetString(TSE_TEST_PROJECT_NAME);
            strPath += L"\\";
            strPath += JGW_GetTimeFolder();
            if (strSN.empty())
            {
                strPath += L"\\";
                strPath += L"backup.qcn";
            }
            else
            {
                strPath += L"\\"; 
                strPath += strSN;
                strPath += L"\\";
                strPath += strSN;
                strPath += L".qcn";
            }          
            JGW_CreateDirectory(strPath.c_str(),false);
            mstrQCNPath = JGW_W2A(strPath);
        }
        return true;
    }

    void BackupNVFuncLog 
        (
        HANDLE hQMSLContext,
        unsigned char  iSubscriptionId,
        const char     *sRFNVid_EFSPath,//Change to char so that the efs filename is also shown
        unsigned short iSourceFunc,
        unsigned short iEvent,
        unsigned short iProgress
        )
    {
        if ( !sRFNVid_EFSPath )
            LogI_F( L"Subscription ID:%d NV Id:%d, Function:BackupNV_LoadNVsFromMobile, Event:%d  %u%%\n",iSubscriptionId,iSourceFunc,iEvent,iProgress);
        else
            LogI_F( L"Subscription ID:%d EFS File:%s, Function:BackupNV_WriteNVsToQCN, Event:%d  %u%%\n" ,iSubscriptionId,a2w_a( sRFNVid_EFSPath ).c_str(),iEvent,iProgress);
    }

    void UploadNVFuncLog
        (
        HANDLE hQMSLContext,
        unsigned char  iSubscriptionId,
        const char     *sRFNVid_EFSPath,//Change to char so that the efs filename is also shown
        unsigned short iSourceFunc,
        unsigned short iEvent,
        unsigned short iProgress
        )
    {
        if ( !sRFNVid_EFSPath )
            LogI_F( L"Subscription ID:%d NV Id:%d, Function:NV_Tool_WriteNVsToMobile, Event:%d  %u%%\n",iSubscriptionId,iSourceFunc,iEvent,iProgress);
        else
            LogI_F( L"Subscription ID:%d EFS File:%s, Function:NV_Tool_WriteNVsToMobile, Event:%d  %u%%\n" ,iSubscriptionId,a2w_a(sRFNVid_EFSPath).c_str(),iEvent,iProgress);
    }

    bool CTSE_QualcommQCN::TSE_Run()
    {
        if ( mbIsReportLog ) CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->NVConfigureCallBack( mbIsBackupQCN?BackupNVFuncLog:UploadNVFuncLog );

        if (!BuildQCNPath())
        {
            LogE(L"生成QCN路径失败！！！\n");
            return false;
        }

        return mbIsBackupQCN?BackupQCN():RestoreQCN();
    }

    bool CTSE_QualcommQCN::BackupQCN()
    {
        jgw_timer time;
        LogI_F(L"Backup QCN FILE: %s\n",a2w(mstrQCNPath).c_str());
        if ( !CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->BackupQCNToMobile(mstrQCNPath) )
        {
            LogE(_T("Backup QCN(ERROR:)\n"));
            return false;
        }
        LogI_F(L"Backup QCN FILE: %s (PASS)(time:%.2lfs)\n",a2w(mstrQCNPath).c_str(),time.elapsed());
        return true;
    }

    bool CTSE_QualcommQCN::RestoreQCN()
    {
        LogI_F(L"restore qcn\n");
        jgw_timer time;

        if (!CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->OpenDMSSSecurityPlan(mstrSPC))
        {
            LogE(_T("OpenDMSSSecurityPlan(ERROR)\n"));
            return false;
        }
        LogI_F(L"Restore QCN FILE: %s\n",a2w(mstrQCNPath).c_str());
        if (!CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->RestoreQCN(mstrQCNPath))
        {
            LogE(_T("Restore QCN(ERROR:)\n"));
            return false;
        }
        LogI_F(L"Restore QCN FILE: %s (PASS)(time:%.2lfs)\n",a2w(mstrQCNPath).c_str(),time.elapsed());
        return true;
    }

}