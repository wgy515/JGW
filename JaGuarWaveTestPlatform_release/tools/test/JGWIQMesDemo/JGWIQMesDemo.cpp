// JGWIQMesDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Include/IQmeasure.h"

#define MAX_BUFFER_SIZE 4096

char errMsg[MAX_BUFFER_SIZE] = {0};
void CheckReturnCode( int returnCode, char *functionName)
{
    if( ERR_OK!=returnCode )
    {
        //TODO
        if( NULL!=functionName )
        {
            sprintf_s(errMsg, MAX_BUFFER_SIZE, "%s returned error: %d", functionName, returnCode);
        }
        else
        {
            sprintf_s(errMsg, MAX_BUFFER_SIZE, "Error code: %d", returnCode);
        }
        throw errMsg;
    }
    else
        printf("%s, return OK.\n", functionName);

    return;
}
typedef void             *LPVOID;

int _tmain(int argc, _TCHAR* argv[])
{
    int nResult = 0;
    nResult = LP_Init(IQTYPE_XEL);
    LPVOID pFunc = (LPVOID)LP_InitTesterN;
    CheckReturnCode(LP_InitTesterN("192.168.133.111",1),"CheckReturnCode");
    CheckReturnCode(LP_SetTesterMode(UP_TO_2GHz_SIGNAL),"CheckReturnCode");
	return 0;
}

