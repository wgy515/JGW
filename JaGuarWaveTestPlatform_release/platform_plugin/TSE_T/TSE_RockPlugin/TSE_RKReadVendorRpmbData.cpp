#include "StdAfx.h"
#include "TSE_RKReadVendorRpmbData.h"


namespace JGW
{
    CTSE_RKReadVendorRpmbData::CTSE_RKReadVendorRpmbData(void) : mnDest(DEST_VENDOR),mpIDMap2(NULL),mnVendorID(VENDOR_SN_ID),mnBinOrText(TYPE_TXT),mbIsRead(true)
    {
    }

    CTSE_RKReadVendorRpmbData::~CTSE_RKReadVendorRpmbData(void)
    {
    }

    const wchar_t* CTSE_RKReadVendorRpmbData::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RKReadVendorRpmbData\",\
                \"TestName\": \"Read data from rpmb or vendor by id\",\
                \"Dest\": \"vendor(0) or rpmb(1)\",\
                \"ItemID\": \"0 : SN,1 : WIFI\",\
                \"SaveReadValueEnvironment\": \"Save Read Value Environment\",\
                }";
    }

    bool CTSE_RKReadVendorRpmbData::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Dest"))
        {
            mnDest = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ItemID"))
        {
            //mnVendorID = static_cast<unsigned short>(_get_vendorid(_ttoi(strParamValue),NULL));
            mpIDMap2 = get_id_map(_ttoi(strParamValue));
            if (NULL != mpIDMap2)
            {
                mnVendorID = static_cast<unsigned short>(_get_vendorid(_ttoi(strParamValue),mpIDMap2));
                mnBinOrText = CCJGW_RKDeviceHelp::GetStringIntHexToVendorID(mnVendorID,mpIDMap2);
                mbIsRead = (FALSE == _is_writeonly(_ttoi(strParamValue),mpIDMap2));
            }
            else
            {
                mnVendorID = static_cast<unsigned short>(_ttoi(strParamValue));
                mnBinOrText = TYPE_TXT;
                mbIsRead = true;
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaveReadValueEnvironment"))
        {
            mstrSaveReadValueEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    TCHAR byte_to_tchar(BYTE hex)
    {
        if (hex < 10) {
            return hex + _T('0');
        } else if (hex < 16 ) {
            return hex - 10 + _T('A');
        }
        return _T(' ');
    }

    TCHAR *HEX_TO_WSTRING_GET(const BYTE *buf, int len)
    {
        TCHAR *pOStr;
        USHORT usStrLen;
        int i;

        usStrLen = len*2 + 1;
        pOStr = new TCHAR[usStrLen];
        if (!pOStr)
            return NULL;
        for (i = 0; i < len; i ++ ) {
            pOStr[2*i] = byte_to_tchar((buf[i]>>4)&0xf);
            pOStr[2*i + 1] = byte_to_tchar((buf[i])&0xf);
        }
        pOStr[2*i] = 0;
        return pOStr;
    }

    bool CTSE_RKReadVendorRpmbData::TSE_Run()
    {
        BYTE readbuffer[VENDOR_MAX_LEN];
        unsigned short readbuffer_size = VENDOR_MAX_LEN;

        if (!mbIsRead)
        {
            LogE_F("Error : RK_ReadVendorRpmbData(dst:%d; id:%d) is write only.", mnDest, mnVendorID);
            return false;
        }
		LogI_F(L"Layer ID : %d - %s",CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer,CCJGW_RKDeviceHelp::m_currentDeviceDesc.szLinkName);
        if (TRUE != RK_ReadVendorRpmbData(mnVendorID, mnDest, readbuffer, readbuffer_size, CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer) || readbuffer_size <= 0) 
        {
            LogE_F("RK_ReadVendorRpmbData(dst:%d; id:%d) failed.", mnDest, mnVendorID);
            return false;
        }

        std::wstring strValue;
        //! 是否检查只读只写判断
        if (TYPE_TXT == mnBinOrText) 
        {
            char szBuff[128] = {0};
            memcpy_s(szBuff,128,readbuffer,readbuffer_size);
            strValue = JGW::JGW_A2W_A(szBuff);
            //pszTemp = BYTE_TO_WSTRING_GET(pBuf, len);
            //Str = pszTemp;
            //WSTRING_PUT(pszTemp);
        } 
        else
        {
            wchar_t* pszTemp = HEX_TO_WSTRING_GET(readbuffer, readbuffer_size);
            strValue = pszTemp;
            delete[] pszTemp;
        }
        LogI_F(L"%s : %d Value : %s",mpIDMap2 ? mpIDMap2->name : L"ID",mnVendorID,strValue.c_str());

        if (!mstrSaveReadValueEnvironment.empty())
        {
            GetGlobalEnvironment()->PutString(mstrSaveReadValueEnvironment,strValue);
        }

        return true;
    }
}
