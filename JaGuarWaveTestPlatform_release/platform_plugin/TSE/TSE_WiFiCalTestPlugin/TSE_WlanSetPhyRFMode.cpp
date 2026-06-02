#include "StdAfx.h"
#include "TSE_WlanSetPhyRFMode.h"

namespace JGW
{
    CTSE_WlanSetPhyRFMode::CTSE_WlanSetPhyRFMode(void) : meMode(PhyA_only),mePhyID(PhyID_A),mnWlanInstance(0)
    {
    }


    CTSE_WlanSetPhyRFMode::~CTSE_WlanSetPhyRFMode(void)
    {
    }

    const wchar_t* CTSE_WlanSetPhyRFMode::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_WlanSetPhyRFMode\",\
                \"TestName\": \"set phone resouce context\",\
                \"mode\": \"Mode ID: 0= PhyA only,1 = DBS, 2= SBS,3= PhyB only\",\
                \"PhyID\": \"PHY ID, for HK and derivative chips only\"\
                }";
    }

    bool CTSE_WlanSetPhyRFMode::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("mode"),strParamName))
        {
            meMode = (RFMode)_ttoi(strParamValue);
        } 
        else if(TSE_PARAM_NAME_EQUAL(_T("PhyID"),strParamName))
        {
            mePhyID = (PhyID)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("WlanInstance"),strParamName))
        {
            mnWlanInstance = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_WlanSetPhyRFMode::TSE_Run()
    {
        return PHONE->WlanSetPhyRFMode(mePhyID,meMode,mnWlanInstance);
    }
}
