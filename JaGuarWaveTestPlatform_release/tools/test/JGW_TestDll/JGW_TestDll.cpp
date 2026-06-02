// JGW_TestDll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

//! using CallbackInfo = void (void* param,const char* info);
typedef void (*CallbackInfo)(void* param,const char* info);
typedef void (*CallbackInfoValue)(void* param,const char* info,const char* value);
typedef void (*CallbackInfoValueLimit)(void* param,const char* info,double value,double upLimit,double lowLimit);
typedef void (*CallbackStop)(void* param);


struct CallbackParam{
    CallbackInfo* saveFunctionRecord;
    CallbackInfo* showNormalInformation;
    CallbackInfo* showFailInformation;
    CallbackInfo* showDebugInformation;
    CallbackInfoValue* uploadStringValue;
    CallbackInfoValueLimit* uploadDoubleValue;
    CallbackStop* stopWhenRequired;
    void* callbackFunctionReservel;
    void* callbackFunctionReserve2;
    void* callbackFunctionReserve3;
    void* param;
    char* logPath;
    char* sn;
    void* record;
    void* paramReservel;
    void* paramReserve2;
    void* paramReserve3;
};

int SerchPortInTimeout(const char* usbPath,DevInfo *devResult,const char* findDeviceType,int timeout,void *callback)
{
    //CallbackParam *pCallbackParam = (CallbackParam *)callback;
    //MessageBoxA(NULL,pCallbackParam->sn,pCallbackParam->logPath,MB_ICONERROR);
    MessageBoxA(NULL,"1233","",MB_ICONERROR);
    return -1;
}