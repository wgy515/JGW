#pragma once
#ifndef JGW_WindowsFuncPlugin_H__
#define JGW_WindowsFuncPlugin_H__
#include <string>
#include <vector>
#include <map>
#include <functional>
#if (defined JGW_WINDOWSFUNCPLUGIN_EXPORTS) || (defined JGW_WINDOWSFUNCPLUGIN_IMPORTS)
#else
#define JGW_WINDOWSFUNCPLUGIN_IMPORTS
#endif


#ifdef JGW_WINDOWSFUNCPLUGIN_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGWWINDOWSFUNCPLUGIN_API __declspec(dllexport)
#		define JGWWINDOWSFUNCPLUGIN_CLASS __declspec(dllexport)
#	else
#		define JGWWINDOWSFUNCPLUGIN_API 
#		define JGWWINDOWSFUNCPLUGIN_CLASS 
#	endif
#endif

#ifdef JGW_WINDOWSFUNCPLUGIN_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGWWINDOWSFUNCPLUGIN_API __declspec(dllimport)
#			define JGWWINDOWSFUNCPLUGIN_CLASS __declspec(dllimport)
#		else
#			define JGWWINDOWSFUNCPLUGIN_API 
#			define JGWWINDOWSFUNCPLUGIN_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_WindowsFuncPlugin_d.lib")
#			else
#				pragma comment(lib,"JGW_WindowsFuncPlugin.lib")
#			endif
#endif
//    __declspec(selelctany) 使在头文件中定义静态成员变量可行。
#define JGWWINDOWSFUNCPLUGIN_COMDAT __declspec(selectany) 
/*#define THROW_JGW*/
#pragma warning(disable:4297)
#pragma warning(disable:4190)
#pragma warning(disable:4819)

//#pragma warning(disable:4251)
#ifdef JGW_EXTERN_
template  class __declspec( dllexport ) std::allocator<wchar_t>;
template  class __declspec( dllexport ) std::allocator<int>;
template  class __declspec( dllexport ) std::allocator<float>;
template  class __declspec( dllexport ) std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >; 
template  class __declspec( dllexport ) std::vector<int, std::allocator<int> >;
template  class __declspec( dllexport ) std::vector<float, std::allocator<float> >;


//template  class __declspec( dllexport ) std::map<std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >,class _Ty,std::less<_Kty>,class _Alloc = allocator<pair<const _Kty, _Ty> > >;

//template class __declspec(dllexport) std::allocator<std::pair<std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t>>, int> >;
//template class __declspec(dllexport) std::map<std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >, int, std::less<std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >>, std::allocator<std::pair<std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >, int> > >;

//template class __declspec(dllexport) std::allocator<std::pair<const int, int> >;
//template class __declspec(dllexport) std::map<int, int, std::less<int>, std::allocator<std::pair<const int, int> > >;
#endif




#endif
