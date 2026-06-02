#include "StdAfx.h"
#include "TSE_RKWriteVendorRpmbData.h"
#include "CJGW_RKDeviceHelp.h"
namespace JGW
{
    CTSE_RKWriteVendorRpmbData::CTSE_RKWriteVendorRpmbData(void) : mnDest(DEST_VENDOR),mnVendorID(VENDOR_SN_ID),mbIsWrite(false),mnBinOrText(TYPE_TXT),mpIDMap2(NULL)
    {
    }


    CTSE_RKWriteVendorRpmbData::~CTSE_RKWriteVendorRpmbData(void)
    {
    }

    const wchar_t* CTSE_RKWriteVendorRpmbData::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RKWriteVendorRpmbData\",\
                \"TestName\": \"Write data from rpmb or vendor by id\",\
                \"Dest\": \"vendor(0) or rpmb(1)\",\
                \"ItemID\": \"0 : SN,1 : WIFI\",\
                \"WriteValueCommand\": \"Write Value\",\
                }";
    }

    bool CTSE_RKWriteVendorRpmbData::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Dest"))
        {
            mnDest = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ItemID"))
        {
            mpIDMap2 = get_id_map(_ttoi(strParamValue));
            if (NULL != mpIDMap2)
            {
                mnVendorID = static_cast<unsigned short>(_get_vendorid(_ttoi(strParamValue),mpIDMap2));
                mnBinOrText = CCJGW_RKDeviceHelp::GetStringIntHexToVendorID(mnVendorID,mpIDMap2);
                mbIsWrite = (FALSE == _is_readonly(_ttoi(strParamValue),mpIDMap2));
            }
            else
            {
                mnVendorID = static_cast<unsigned short>(_ttoi(strParamValue));
                mnBinOrText = TYPE_TXT;
                mbIsWrite = true;
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WriteValueCommand"))
        {
            mstrWriteValueCommand = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_RKWriteVendorRpmbData::TSE_Run()
    {
        std::wstring strValue(mstrWriteValueCommand);
        GetEnvironmentCommandString(strValue);

        if (!mbIsWrite)
        {
            LogE_F("Error : RK_WriteVendorRpmbData(dst:%d; id:%d) is read only.", mnDest, mnVendorID);
            return false;
        }

        USHORT nBufferSize = 0;
        BYTE buffer[256] = {0};
        PBYTE pReadBuf = NULL;

        if (TYPE_TXT == mnBinOrText) 
        {
            nBufferSize = strValue.length() & 0xFFFF;
            strcpy_s((char*)buffer,256,JGW_W2A(strValue).c_str());
        } 
        else 
        {
            JGW_ReplaceStringW(strValue,L":",L"");
            JGW_ReplaceStringW(strValue,L".",L"");
            JGW_ReplaceStringW(strValue,L"-",L"");

            for (size_t index = 2;index <= strValue.size() + 1;index += 2)
            {
                buffer[nBufferSize++] = (std::stol(strValue.substr(index -2,2),NULL,16) & 0xFF);
            }
        }

        if (TRUE != RK_WriteVendorRpmbData(mnVendorID, mnDest, buffer, nBufferSize, CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer))
        {
            LogE_F(L"WriteVendorRpmbItem(dst:%d; id:%d) buf=0x%08x, len=%d", mnDest, mnVendorID, buffer, nBufferSize);
            return FALSE;
        }

        pReadBuf = new BYTE[nBufferSize];
        if (!pReadBuf) 
        {
            LogE_F(L"WriteVendorRpmbItem(dst:%d; id:%d) alloc read buffer failed.",mnDest, mnVendorID);
            return FALSE;
        }

        if (TRUE != RK_ReadVendorRpmbData(mnVendorID, mnDest, pReadBuf, nBufferSize, CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer)) 
        {
            LogE_F("RK_ReadVendorRpmbData(dst:%d; id:%d) failed.", mnDest, mnVendorID);
            return FALSE;
        }

        if (memcmp(pReadBuf, buffer, nBufferSize) != 0) 
        {
            if (pReadBuf) 
            {
                delete[] pReadBuf;
            }
            LogE_F("WriteVendorRpmbItem(dst:%d; id:%d) compare failed.", mnDest, mnVendorID);
            return FALSE;
        }

        if (pReadBuf) 
        {
            delete[] pReadBuf;
        }

        return true;
#if 0
        BOOL bRet;
        PBYTE pReadBuf = NULL;
        USHORT nBufferSize = len;
        int id_in_device = id;

        if (!buf || (0 >= len)) {
            printf("WriteVendorRpmbItem(dst:%d; id:%d) buf=0x%08x, len=%d", dest, id_in_device, buf, len);
            return FALSE;
        }
        bRet = RK_WriteVendorRpmbData(mnVendorID, mnDest, buf, len, CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer);
        if (!bRet) {
            printf("RK_WriteVendorRpmbData(dst:%d; id:%d) failed.", dest, id_in_device);
            return FALSE;
        }
        pReadBuf = new BYTE[nBufferSize];
        if (!pReadBuf) {
            printf("WriteVendorRpmbItem(dst:%d; id:%d) alloc read buffer failed.", dest, id_in_device);
            return FALSE;
        }
        bRet = RK_ReadVendorRpmbData(mnVendorID, mnDest, pReadBuf, nBufferSize, CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer);
        if (!bRet) {
            printf("RK_ReadVendorRpmbData(dst:%d; id:%d) failed.", dest, id_in_device);
            return FALSE;
        }

        if (memcmp(pReadBuf, buf, nBufferSize) != 0) {
            if (pReadBuf) {
                delete[] pReadBuf;
            }
            printf("WriteVendorRpmbItem(dst:%d; id:%d) compare failed.", dest, id_in_device);
            return FALSE;
        }
        printf("WriteVendorRpmbItem(dst:%d; id:%d), len=%d", dest, id_in_device, nBufferSize);
        if (pReadBuf) {
            delete[] pReadBuf;
        }
        return TRUE;
#endif
    }
}

