#pragma once
#include <string>

namespace JGW
{
    //typedef enum
    //{
    //    //! MES操作类型
    //    E_MES_MODULE = 0,
    //    //! TSE 测试操作类型
    //    E_TSE_MODULE = 1
    //}E_MODULE_TYPE;

    typedef struct  
    {
        std::string mstrParamName;
        std::string mstrParamValue;
        std::string mstrParamDes;
    }S_SUITE_PROJECT_PARAM;

    typedef enum
    {
        E_CSM_UNKNOWN = -1,
        E_CSM_MES_CHECK_SN_STATUS = 0,
        E_CSM_MES_GET_IMEI_INFO = 1,
        E_CSM_MES_GET_SN_INFO = 2,
        E_CSM_MES_NEXT_IMEI_STATION = 3,
        E_CSM_MES_NEXT_SN_STATION = 4,
        E_CSM_MES_SAVE_TEST_RECORDS = 5,
        E_CSM_TSE_CREATE_SUITE_PROJECT = 6,
        E_CSM_TSE_GET_ENVIRONMENT_VALUE = 7,
        E_CSM_TSE_SET_ENVIRONMENT_VALUE = 8,
        E_CSM_TSE_RUN_SUITE_PROJECT = 9,
        E_CSM_UPDATE_TEST_LOG = 10
    }E_CSM_REQ_ID;

    typedef enum
    {
        E_CSM_DEBUG_LOG = 0,
        E_CSM_INFO_LOG = 1,
        E_CSM_ERROR_LOG = 2
    }E_CSM_LOG_TYPE;


    //#pragma pack (1)
    //
    //
    //
    //
    //
    //
    //#pragma pack ()
}