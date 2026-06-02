#pragma once

#ifdef __cplusplus
extern "C" 
{
#endif
    __declspec(dllexport) HWND LoadWin32Window(HWND hMainWnd);

//     //! ²âÊÔlog»Øµ÷º¯Êý
//     typedef void (*TSETestLogFunc)(int type,int id,const wchar_t* log);
//     __declspec(dllexport) void SetTSETestLogCallBack(TSETestLogFunc testLogCallBack);

    __declspec(dllexport) void UnLoadWin32Window();

#ifdef __cplusplus
};
#endif