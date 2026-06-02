#include "StdAfx.h"
#include "TSE_WriteQcomNV.h"
#include "../TSE_DevicesGlobalResource.h"
#include <algorithm>
namespace JGW
{
    CTSE_WriteQcomNV::CTSE_WriteQcomNV(void):mitemID(0),mPacketLen(0),mpData(NULL),miSub_ID(0),mbOpenDMSSSecurityPlan(false),mstrSPC("000000")
    {
    }

    const wchar_t* CTSE_WriteQcomNV::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WriteQcomNV\",\
                \"TestName\": \"写NV(Qualcomm)\",\
                \"NV_ID\": \"NV 项索引\",\
                \"Sub_ID\": \"NV数组索引\",\
                \"NV_Value\": \"写NV值,如BYTE:1\",\
                \"spc\": \"SPC值 默认000000\",\
                \"DMSSSecurityPlan\": \"是否要进行安全认证\"\
                }";
    }

    bool CTSE_WriteQcomNV::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("NV_ID"),strParamName) )
        {
            mitemID = static_cast<unsigned short>(_ttoi(strParamValue));
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("NV_Value"),strParamName) )
        {
            std::string replacedCmdStr = JGW_W2A_W(strParamValue);
            std::replace(replacedCmdStr.begin(), replacedCmdStr.end(), '#', ','); // replace # with ,
            mpData = (unsigned char*)mPacketFormatter.FormatDiagPkt( replacedCmdStr.c_str(), mPacketLen );
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("Sub_ID"),strParamName) )
        {
            miSub_ID = static_cast<unsigned short>(_ttoi(strParamValue));
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("DMSSSecurityPlan"),strParamName) )
        {
            mbOpenDMSSSecurityPlan = _ttoi(strParamValue)?true:false;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("spc"),strParamName) )
        {
            mstrSPC = JGW_W2A_W(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WriteQcomNV::TSE_Run()
    {
        LogI(L"ctf_write_nv_phone_cmd runing\n");
        if( !mitemID || !mPacketLen || !mpData )
        {
            LogE(L"ctf_write_nv_phone_cmd: 无效参数");
            return false;
        }
        std::wstring strReadTemp,strTemp;
        for ( size_t i = 0; i < mPacketLen;i ++)
        {
            FormatWString(strTemp,L"%02X ",mpData[i]);
            strReadTemp += strTemp;
            if ( i != 0 && i % 7 == 0 ) strReadTemp += L"\n";
        }

        for ( int i = 0;i < 3;i ++)
        {
            if( !mbOpenDMSSSecurityPlan || CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->OpenDMSSSecurityPlan(mstrSPC) )
            {
                if( CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriterNV(mitemID,mpData,mPacketLen,miSub_ID) )
                {
                    LogI_F(L"写%05d-NV值(16进制)：%s\n写入成功\n",mitemID,strReadTemp.c_str());
                    return true;
                }
            }
            Sleep(200);
        }
        LogI_F(L"写%05d-NV值(16进制)：%s\n写入失败\n",mitemID,strReadTemp.c_str());
        return false;
    }
}

