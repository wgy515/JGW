#pragma once
#include "CJGW_TestRecordImpl.h"
// #ifdef JGW_TESTRECORD_API
// #undef JGW_TESTRECORD_API
// #endif

#ifdef JGW_TESTRECORD_EXPORTS
#define JGW_TESTRECORD_API __declspec(dllexport)
#else
#define JGW_TESTRECORD_IMPORTS
#define JGW_TESTRECORD_API __declspec(dllimport)
#endif

#ifdef JGW_TESTRECORD_IMPORTS
#   ifdef _DEBUG
#       pragma comment(lib,"JGW_TestRecord_d.lib")
#   else
#       pragma comment(lib,"JGW_TestRecord.lib")
#   endif
#endif

#ifdef __cplusplus
extern "C" 
{
#endif

    JGW_TESTRECORD_API JGW::CCJGW_TestRecordImpl* GetTestRecordInstance();

    JGW_TESTRECORD_API JGW::CCJGW_TestRecordImpl* GetTestRecordInstanceToIndex();

#ifdef __cplusplus
}
#endif