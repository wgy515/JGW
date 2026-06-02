#pragma once
#if 0
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#ifdef TSE_SEQUENCEMANAGERLUAPLUGIN_EXPORTS
#define TSE_SEQUENCEMANAGERLUAPLUGIN_API __declspec(dllexport)
#else
#define TSE_SEQUENCEMANAGERLUAPLUGIN_API __declspec(dllimport)
#endif

extern "C" TSE_SEQUENCEMANAGERLUAPLUGIN_API int luaopen_TSE_SequenceManagerLuaPlugin(lua_State *L);//定义导出函数
#endif

