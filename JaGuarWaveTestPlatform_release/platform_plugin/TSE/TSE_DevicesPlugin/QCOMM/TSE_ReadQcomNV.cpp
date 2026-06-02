#include "StdAfx.h"
#include "TSE_ReadQcomNV.h"
#include "../TSE_DevicesGlobalResource.h"
#include <algorithm>
namespace JGW
{
    CTSE_ReadQcomNV::CTSE_ReadQcomNV(void):mitemID(0),mPacketLen(0),mpData(NULL),miSub_ID(0)
    {
    }

    const wchar_t* CTSE_ReadQcomNV::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ReadQcomNV\",\
                \"TestName\": \"读取NV(Qualcomm)\",\
                \"NV_ID\": \"NV 项索引\",\
                \"Sub_ID\": \"NV数组索引\",\
                \"NV_Value\": \"校验NV值,如BYTE:0\"\
                }";
    }

    bool CTSE_ReadQcomNV::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ReadQcomNV::TSE_Run()
    {
        LogD(L"ctf_read_nv_phone_cmd runing\n");
        unsigned char readvalue[128] = {0};
        std::wstring strReadTemp,strTemp;

        if( !mitemID || !mPacketLen || !mpData )
        {
            LogE(L"ctf_read_nv_phone_cmd: 无效参数");
            return false;
        }

        for ( int i = 0;i < 3;i ++)
        {
            if( CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->ReadNV(mitemID,readvalue,mPacketLen,miSub_ID) )
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
                        LogE_F(L"读取%05d-NV值为(16进制)：%s\n比对失败\n",mitemID,strReadTemp.c_str());
                        return false;
                    }
                }
                LogI_F(_T("读取%05d-NV值为(16进制)：%s\n"),mitemID,strReadTemp.c_str());
                return true;
            }
            Sleep(200);
        }
        LogE_F(_T("读取%05d-NV失败"),mitemID);
        return false;
    }
}

