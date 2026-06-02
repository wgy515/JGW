// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_DevicePool.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "usb_host\AutoUsbDefine.h"
#include <vector>
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
std::vector <PHUBPORTPOINTINFO> g_vSortPortInfo;// 所有端口信息
int g_nUserPortSort = 0;