#include "StdAfx.h"
#include "CSTSE_GetTSEInfoJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCSTSE_GetTSEInfoJson::CCSTSE_GetTSEInfoJson(void) : CCSTSE_BaseJson(ECSTSE_ACTION_GET_TSE_INFO)
    {
    }


    CCSTSE_GetTSEInfoJson::~CCSTSE_GetTSEInfoJson(void)
    {
    }

    std::string CCSTSE_GetTSEInfoJson::GetTSEInfoJson()
    {
        return JGW_GetFormatString("{\"CSTSEAction\":%d}",(int)CSTSEAction); 
    }
}

