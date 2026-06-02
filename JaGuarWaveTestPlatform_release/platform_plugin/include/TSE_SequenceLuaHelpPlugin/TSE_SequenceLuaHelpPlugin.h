#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#ifdef TSE_SEQUENCELUAHELPPLUGIN_EXPORTS
#define TSE_SEQUENCELUAHELPPLUGIN_API __declspec(dllexport)
#else
#define TSE_SEQUENCELUAHELPPLUGIN_API __declspec(dllimport)
#endif

extern "C" TSE_SEQUENCELUAHELPPLUGIN_API int luaopen_TSE_SequenceLuaHelpPlugin(lua_State *L);//定义导出函数

