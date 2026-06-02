#pragma once
#ifndef JGW_SSHPLUGIN_H__
#define JGW_SSHPLUGIN_H__
#include <string>
#include <vector>

#if (defined JGW_SSHPLUGIN_EXPORTS) || (defined JGW_SSHPLUGIN_IMPORTS)
#else
#define JGW_SSHPLUGIN_IMPORTS
#endif


#ifdef JGW_SSHPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_SSHPLUGIN_API __declspec(dllexport)
#		define JGW_SSHPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGW_SSHPLUGIN_API 
#		define JGW_SSHPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_SSHPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_SSHPLUGIN_API __declspec(dllimport)
#			define JGW_SSHPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGW_SSHPLUGIN_API 
#			define JGW_SSHPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_SSHPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_SSHPlugin.lib")
#			endif
#endif
//    __declspec(selelctany) 使在头文件中定义静态成员变量可行。
// #define JGW_SSHPLUGIN_COMDAT __declspec(selectany) 
// /*#define THROW_JGW*/
// #pragma warning(disable:4297)
// #pragma warning(disable:4190)
// #pragma warning(disable:4819)
// 
//#ifdef JGW_EXTERN_
template  class __declspec( dllexport ) std::allocator<wchar_t>;
template  class __declspec( dllexport ) std::allocator<char>;
template  class __declspec( dllexport ) std::allocator<int>;
template  class __declspec( dllexport ) std::allocator<float>;
template  class __declspec( dllexport ) std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >; 
template  class __declspec( dllexport ) std::basic_string<char,  std::char_traits<char>,  std::allocator<char> >; 
template  class __declspec( dllexport ) std::vector<int, std::allocator<int> >;
template  class __declspec( dllexport ) std::vector<float, std::allocator<float> >;
//#endif

#define SSH_BUFSIZE 102400
#define RECV_SSH_BUFSIZE 1024 * 24
#endif
