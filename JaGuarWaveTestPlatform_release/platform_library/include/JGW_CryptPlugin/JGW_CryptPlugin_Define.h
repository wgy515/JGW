#pragma once
#ifndef JGW_CRYPTPLUGIN_H__
#define JGW_CRYPTPLUGIN_H__
#include <string>
#include <vector>

#if (defined JGW_CRYPTPLUGIN_EXPORTS) || (defined JGW_CRYPTPLUGIN_IMPORTS)
#else
#define JGW_CRYPTPLUGIN_IMPORTS
#endif


#ifdef JGW_CRYPTPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_CRYPTPLUGIN_API __declspec(dllexport)
#		define JGW_CRYPTPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGW_CRYPTPLUGIN_API 
#		define JGW_CRYPTPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_CRYPTPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_CRYPTPLUGIN_API __declspec(dllimport)
#			define JGW_CRYPTPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGW_CRYPTPLUGIN_API 
#			define JGW_CRYPTPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_CryptPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_CryptPlugin.lib")
#			endif
#endif
//    __declspec(selelctany) 使在头文件中定义静态成员变量可行。
// #define JGW_CRYPTPLUGIN_COMDAT __declspec(selectany) 
// /*#define THROW_JGW*/
// #pragma warning(disable:4297)
// #pragma warning(disable:4190)
// #pragma warning(disable:4819)
// 
// #ifdef JGW_EXTERN_
// template  class __declspec( dllexport ) std::allocator<wchar_t>;
// template  class __declspec( dllexport ) std::allocator<int>;
// template  class __declspec( dllexport ) std::allocator<float>;
// template  class __declspec( dllexport ) std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >; 
// template  class __declspec( dllexport ) std::vector<int, std::allocator<int> >;
// template  class __declspec( dllexport ) std::vector<float, std::allocator<float> >;
// #endif

#endif
