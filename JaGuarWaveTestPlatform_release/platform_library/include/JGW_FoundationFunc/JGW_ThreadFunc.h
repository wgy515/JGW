// 导出文件操作函数

#ifndef JGW_FOUNDATIONFUNCLIB_THREADFUNC_H__
#define JGW_FOUNDATIONFUNCLIB_THREADFUNC_H__
#include "JGW_FoundationFuncLib_Define.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	extern "C" 
	{
#endif
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_ThreadIsRunning(HANDLE hThread);
		//! 获取模块当前工作目录下的子目录 如c:/123/subfolder/ 
		JGW_FOUNDATIONFUNC_LIB_API bool JGW_PostThreadMsg(DWORD dwThreadID,UINT Msg,WPARAM wParam,LPARAM lParam,int nCounts = 30);

#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	}
#endif
}

#endif

