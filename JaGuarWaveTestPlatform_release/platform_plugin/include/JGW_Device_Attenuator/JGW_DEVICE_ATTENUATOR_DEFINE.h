#pragma once

#if (defined JGW_DEVICE_ATTENUATOR_EXPORTS) || (defined JGW_DEVICE_ATTENUATOR_IMPORTS)
#else
#define JGW_DEVICE_ATTENUATOR_IMPORTS
#endif


#ifdef JGW_DEVICE_ATTENUATOR_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_DEVICE_ATTENUATOR_API __declspec(dllexport)
#		define JGW_DEVICE_ATTENUATOR_CLASS __declspec(dllexport)
#	else
#		define JGW_DEVICE_ATTENUATOR_API 
#		define JGW_DEVICE_ATTENUATOR_CLASS 
#	endif
#endif

#ifdef JGW_DEVICE_ATTENUATOR_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_DEVICE_ATTENUATOR_API __declspec(dllimport)
#			define JGW_DEVICE_ATTENUATOR_CLASS __declspec(dllimport)
#		else
#			define JGW_DEVICE_ATTENUATOR_API 
#			define JGW_DEVICE_ATTENUATOR_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_Device_Attenuator_d.lib")
#			else
#				pragma comment(lib,"JGW_Device_Attenuator.lib")
#			endif
#endif

// #ifdef JGW_EXTERN_
// template  class __declspec( dllexport ) std::allocator<wchar_t>;
// template  class __declspec( dllexport ) std::allocator<int>;
// template  class __declspec( dllexport ) std::allocator<float>;
// template  class __declspec( dllexport ) std::basic_string<wchar_t,  std::char_traits<wchar_t>,  std::allocator<wchar_t> >; 
// template  class __declspec( dllexport ) std::vector<int, std::allocator<int> >;
// template  class __declspec( dllexport ) std::vector<float, std::allocator<float> >;
// #endif
