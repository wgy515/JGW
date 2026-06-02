#include "StdAfx.h"
#include "TSE_MessageBox.h"
#include <TSE_FoundationLib/TSE_FoundationLib_Define.h>

namespace JGW
{
    CTSE_MessageBox::CTSE_MessageBox(void) : mdwFlags(MB_OK)
    {
    }


    CTSE_MessageBox::~CTSE_MessageBox(void)
    {
    }

    const wchar_t* CTSE_MessageBox::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MessageBox\",\
                \"TestName\": \"对话框消息\",\
                \"caption\": \"对话框标题\",\
                \"msg\": \"对话框提示消息\",\
                \"flag\": \"对话框类型 0 : MB_OK,1 : MB_OKCANCEL\"\
                }";
    }

    bool CTSE_MessageBox::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"msg"))
        {
            mstrMsg = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"caption"))
        {
            mstrCaption = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"flag"))
        {
            mdwFlags = _ttoi(strParamValue);
        }
        return true;
    }

    bool CTSE_MessageBox::TSE_Run()
    {
        if (MB_OKCANCEL == mdwFlags)
        {
            return (IDOK == MessageBox(GetGlobalMainHwnd(),mstrMsg.c_str(),mstrCaption.c_str(),MB_OKCANCEL));
        }
        MessageBox(GetGlobalMainHwnd(),mstrMsg.c_str(),mstrCaption.c_str(),mdwFlags);
        return true;
    }

}

