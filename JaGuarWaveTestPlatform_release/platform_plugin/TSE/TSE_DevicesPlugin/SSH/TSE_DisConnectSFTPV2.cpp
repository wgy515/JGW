#include "StdAfx.h"
#include "TSE_DisConnectSFTPV2.h"
#include <JGW_SSHPlugin/CJGW_SSHSFTPV2.h>
namespace JGW
{
    CTSE_DisConnectSFTPV2::CTSE_DisConnectSFTPV2(void) : mstrSFTPInstanceEnvironment(L"TSE_SFTP_INSTANCE")
    {
    }


    CTSE_DisConnectSFTPV2::~CTSE_DisConnectSFTPV2(void)
    {
    }

    const wchar_t* CTSE_DisConnectSFTPV2::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_DisConnectSFTPV2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SFTPInstanceEnvironment",strParamName))
        {
            mstrSFTPInstanceEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_DisConnectSFTPV2::TSE_Run()
    {
        if (!mstrSFTPInstanceEnvironment.empty())
        {
            CCJGW_SSHSFTPV2 *pSSHSFTPV2 = (CCJGW_SSHSFTPV2 *)GetGlobalEnvironment()->GetInt(mstrSFTPInstanceEnvironment);
            if (NULL != pSSHSFTPV2)
            {
                pSSHSFTPV2->DisConnectSSHSFTP();
            }
            GetGlobalEnvironment()->PutInt(mstrSFTPInstanceEnvironment,0);
        }
        return true;
    }
}
