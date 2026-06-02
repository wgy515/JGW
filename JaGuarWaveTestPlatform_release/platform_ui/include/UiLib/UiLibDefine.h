#pragma once
#ifndef _UILIB_DEFINE_H_
#define _UILIB_DEFINE_H_
#include <objbase.h>
#include <zmouse.h>
#include <exdisp.h>
#include <comdef.h>
#include <vector>
#include <sstream>
#include <windows.h>
#include <tchar.h>
#include "UIlib.h"

#ifndef NO_USING_DUILIB_NAMESPACE
using namespace DuiLib;
using namespace std;
#endif

#include <olectl.h>

#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

// #define CLOSEBTN	 _T("closebtn")
// #define MAXBTN		 _T("maxbtn")
// #define MINBTN		 _T("minbtn")
// #define RESTORBTN	 _T("restorebtn")


using namespace UiLib;
#ifdef _DEBUG
#pragma comment(lib, "UiLib_d.lib")
#else
#pragma comment(lib, "UiLib.lib")
#endif



#endif
