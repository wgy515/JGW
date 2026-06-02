#include "StdAfx.h"
#include "TSE_RKReadOtpData.h"
#include "CJGW_RKDeviceHelp.h"
namespace JGW
{
    CTSE_RKReadOtpData::CTSE_RKReadOtpData(void) : mnOtpType(0)
    {
    }


    CTSE_RKReadOtpData::~CTSE_RKReadOtpData(void)
    {
    }

    const wchar_t* CTSE_RKReadOtpData::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_RKReadOtpData\",\
                \"TestName\": \"Read data from otp\"\
                }";
    }

    bool CTSE_RKReadOtpData::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        return true;
    }

    #define nof(x) (sizeof(x) / sizeof(x[0]))

    bool printhex(const wchar_t *tag, BYTE *data, int size)
    {
        wchar_t buf[128];
        int linelength = 16;
        int i, length = 0;

        if (tag) LogI(tag);
        length = _sntprintf(buf, nof(buf), _T("00: %02X"), data[0]);
        for (i = 1; i < size; i ++) {
            if (!(i%linelength)) {
                length += _sntprintf(&buf[length], nof(buf) - length, _T("\r\n"));
                LogI(buf);
                length = _sntprintf(buf, nof(buf), _T("%02d: "), i/linelength);
            }
            length += _sntprintf(&buf[length], nof(buf) - length, _T(" %02X"), data[i]);
        }
        if (length) {
            length += _sntprintf(&buf[length], nof(buf) - length, _T("\r\n"));
            LogI(buf);
        }
        return true;
    }

    bool CTSE_RKReadOtpData::TSE_Run()
    {
        int readlen = VENDOR_MAX_LEN;
        BYTE bItemBufer[VENDOR_MAX_LEN] = {0};

        if (TRUE == RK_ReadOtpData(bItemBufer, readlen, 0, CCJGW_RKDeviceHelp::m_currentDeviceDesc.dwLayer)) 
        {
            printhex(_T("CPUID"), &bItemBufer[1], bItemBufer[0]);
            return true;
        }
		Sleep(mnIntervalTimeMS);
        return false;
    }
}

