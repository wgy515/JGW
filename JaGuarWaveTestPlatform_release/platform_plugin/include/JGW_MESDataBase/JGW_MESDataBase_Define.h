#pragma once
#ifndef JGW_MES_DATABASE_DEFINE_H__
#define JGW_MES_DATABASE_DEFINE_H__
#include <string>
#include <vector>

#if (defined JGW_MESDATABASE_EXPORTS) || (defined JGW_MESDATABASE_IMPORTS)
#else
#define JGW_MESDATABASE_IMPORTS
#endif


#ifdef JGW_MESDATABASE_EXPORTS
#	define JGW_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_MESDATABASE_API __declspec(dllexport)
#		define JGW_MESDATABASE_CLASS __declspec(dllexport)
#	else
#		define JJGW_MESDATABASE_API 
#		define JGW_MESDATABASE_CLASS 
#	endif
#endif

#ifdef JGW_MESDATABASE_IMPORTS
#		define JGW_EXTERN_
#		if _MSC_VER >= 1500
#			define JGW_MESDATABASE_API __declspec(dllimport)
#			define JGW_MESDATABASE_CLASS __declspec(dllimport)
#		else
#			define JGW_MESDATABASE_API 
#			define JGW_MESDATABASE_CLASS 
#		endif
#			ifdef _DEBUG
#				pragma comment(lib,"JGW_MESDataBase_d.lib")
#			else
#				pragma comment(lib,"JGW_MESDataBase.lib")
#			endif
#endif

#endif