#include "StdAfx.h"
#include "CJGW_WriteQcomNV.h"
#include "../CJGW_DevicesGlobalResource.h"
#include <algorithm>
namespace JGW
{
    CCJGW_WriteQcomNV::CCJGW_WriteQcomNV(void):mitemID(0),mPacketLen(0),mpData(NULL),miSub_ID(0),mbOpenDMSSSecurityPlan(false),mstrSPC("000000")
    {
    }

    bool CCJGW_WriteQcomNV::CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CCJGW_TestSuiteImpl::CreateParam(strParamName,strParamValue)) return true;
        if( 0 == JGW_WStrComparenoCaseWStr(_T("NV_ID"),strParamName) )
        {
            mitemID = static_cast<unsigned short>(_ttoi(strParamValue));
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("NV_Value"),strParamName) )
        {
            std::string replacedCmdStr = JGW_W2A_W(strParamValue);
            std::replace(replacedCmdStr.begin(), replacedCmdStr.end(), '#', ','); // replace # with ,
            mpData = (unsigned char*)mPacketFormatter.FormatDiagPkt( replacedCmdStr.c_str(), mPacketLen );
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("Sub_ID"),strParamName) )
        {
            miSub_ID = static_cast<unsigned short>(_ttoi(strParamValue));
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("DMSSSecurityPlan"),strParamName) )
        {
            mbOpenDMSSSecurityPlan = _ttoi(strParamValue)?true:false;
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("spc"),strParamName) )
        {
            mstrSPC = JGW_W2A_W(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CCJGW_WriteQcomNV::Run()
    {
        DEBUG_MSG(L"ctf_write_nv_phone_cmd runing\n");
        if( !mitemID || !mPacketLen || !mpData )
        {
            ERROR_MSG(L"ctf_write_nv_phone_cmd: 无效参数");
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
            if( !mbOpenDMSSSecurityPlan || CCJGW_DevicesGlobalResource::GetQcommDiagActionIsntance()->OpenDMSSSecurityPlan(mstrSPC) )
            {
                if( CCJGW_DevicesGlobalResource::GetQcommDiagActionIsntance()->WriterNV(mitemID,mpData,mPacketLen,miSub_ID) )
                {
                    INFO_MSG_FORMAT(L"写%05d-NV值(16进制)：\n%s\n写入成功\n",mitemID,strReadTemp.c_str());
                    return true;
                }
            }
            Sleep(200);
        }
        INFO_MSG_FORMAT(L"写%05d-NV值(16进制)：\n%s\n写入失败\n",mitemID,strReadTemp.c_str());
        return false;
    }
}

