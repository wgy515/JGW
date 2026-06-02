#include "StdAfx.h"
#include "CJGW_ReadQcomNV.h"
#include "../CJGW_DevicesGlobalResource.h"
#include <algorithm>
namespace JGW
{
    CCJGW_ReadQcomNV::CCJGW_ReadQcomNV(void):mitemID(0),mPacketLen(0),mpData(NULL),miSub_ID(0)
    {
    }

    bool CCJGW_ReadQcomNV::CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CCJGW_TestSuiteImpl::CreateParam(strParamName,strParamValue)) return true;
        if( 0 == JGW_WStrComparenoCaseWStr(_T("NV_ID"),strParamName) )
        {
            mitemID = static_cast<unsigned short>(_ttoi(strParamValue));
            DEBUG_MSG_FORMAT(L"NV_ID:	%s\n",strParamValue);
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("NV_Value"),strParamName) )
        {
            std::string replacedCmdStr = JGW_W2A_W(strParamValue);
            DEBUG_MSG_FORMAT(L"NV_Value:	%s\n",strParamValue);
            std::replace(replacedCmdStr.begin(), replacedCmdStr.end(), '#', ','); // replace # with ,
            mpData = (unsigned char*)mPacketFormatter.FormatDiagPkt( replacedCmdStr.c_str(), mPacketLen );
        }
        else if( 0 == JGW_WStrComparenoCaseWStr(_T("Sub_ID"),strParamName) )
        {
            miSub_ID = static_cast<unsigned short>(_ttoi(strParamValue));
            DEBUG_MSG_FORMAT(L"Sub_ID:	%s\n",strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CCJGW_ReadQcomNV::Run()
    {
        DEBUG_MSG(L"ctf_read_nv_phone_cmd runing\n");
        unsigned char readvalue[128] = {0};
        std::wstring strReadTemp,strTemp;

        if( !mitemID || !mPacketLen || !mpData )
        {
            ERROR_MSG(L"ctf_read_nv_phone_cmd: 无效参数");
            return false;
        }

        for ( int i = 0;i < 3;i ++)
        {
            if( CCJGW_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadNV(mitemID,readvalue,mPacketLen,miSub_ID) )
            {
                for ( size_t i = 0; i < mPacketLen;i ++)
                {
                    FormatWString(strTemp,L"%02X ",readvalue[i]);
                    strReadTemp += strTemp;
                    if ( i != 0 && i % 7 == 0 ) strReadTemp += L"\n";
                }
                for (size_t i = 0;i < mPacketLen;i ++)
                {
                    if( readvalue[i] != mpData[i] )
                    {
                        ERROR_MSG_FORMAT(L"读取%05d-NV值为(16进制)：\n%s\n比对失败\n",mitemID,strReadTemp.c_str());
                        return false;
                    }
                }
                INFO_MSG_FORMAT(_T("读取%05d-NV值为(16进制)：\n%s\n"),mitemID,strReadTemp.c_str());
                return true;
            }
            Sleep(200);
        }
        ERROR_MSG_FORMAT(_T("读取%05d-NV失败"),mitemID);
        return false;
    }
}

