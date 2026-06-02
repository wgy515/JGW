#include "StdAfx.h"
#include "TSE_WlanLoadDut.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_WlanLoadDut::CTSE_WlanLoadDut(void) : mstrDevDLLName("ipq4019"),mstrRefDesign("ipq4019"),mstrEepFName("boardData_1_0_IPQ4019_DK06_5G.bin"),
        meiNVMemOption(DataFile),mnSSID(0x40)
    {

    }


    CTSE_WlanLoadDut::~CTSE_WlanLoadDut(void)
    {
    }

    const wchar_t* CTSE_WlanLoadDut::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanLoadDut\",\
                \"TestName\": \"Wlan Load Dut\",\
                \"DevDLLName\": \"Device dll name (less than 30 charactors),def:ipq4019\",\
                \"refDesign\": \"refDesign,def:ipq4019\",\
                \"eepFName\": \"eep file name (less than 50 charactors),def:boardData_1_0_IPQ4019_DK06_5G.bin\",\
                \"iNVMemOption\": \"NV memory Option DataNone=0;DataFlash=1;DataEeprom=2;DataOtp=3;DataDontLoad=4;DataFile=5;DataDram=6\",\
                \"ssid\": \"Device ID 0x40,Device ID: 0x3c:QC98xx; 0x3d:AR6004; 0x3e:AR6174\"\
                }";
    }

    bool CTSE_WlanLoadDut::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"DevDLLName",strParamName))
        {
            mstrDevDLLName = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"refDesign",strParamName))
        {
            mstrRefDesign = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"eepFName",strParamName))
        {
            mstrEepFName = JGW_RealativePathToAbsPathA(JGW_W2A(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"iNVMemOption",strParamName))
        {
            meiNVMemOption = (NVMEM_OPTION)_ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(L"ssid",strParamName))
        {
            wchar_t* str = NULL;
            mnSSID = wcstol(strParamValue,&str,16);
        }
        else
        {
            return false;
        }
        return true;
    }

    void QmslMsgCB(unsigned int type,char *strMsg)
    {
        switch (type)
        {
        case 0:
            //QTALogMgr.Error(strMsg);
            LogE(strMsg);
            break;
        case 1:
            //logMessage(strMsg);
            LogI(strMsg);
            break;
        default:
            LogI(strMsg);
            //DebugMessage.Write("Debug", strMsg);
            break;
        }
    }

    bool CTSE_WlanLoadDut::TSE_Run()
    {
        PHONE->FTM_WLAN_Atheros_Qmsl_Msg_Callback(QmslMsgCB);
        return PHONE->FTM_WLAN_Atheros_LoadDUT(mstrDevDLLName,mstrEepFName,(int)meiNVMemOption,mnSSID);
    }
}

