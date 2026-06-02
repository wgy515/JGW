#pragma once
#include <tchar.h>
#include <vector>
#include <string>
#include <JGW_DevicePool/DevicePortDefine.h>

#define MAX_DOWNLOAD_THREAD_COUNT               (8)
// #define MAX_DOWNLOAD_THREAD_COUNT               (8)
// //! 切换fastboot信息
// #define SWITCH_FASTOOT_MODE_THREAD_MSG			(WM_USER + 60)
// //! 切换下载模式9008
// #define SWITCH_DOWNLOAD_MODE_THREAD_MSG			(WM_USER + 61)
// //! EDL切换信息
// #define SWITCH_EDL_MODE_THREAD_MSG				(WM_USER + 62)
// 
// #define DOWN_PROCESS_NAME	L"\\DownLoad.exe"
class Cache_Config_Info
{
public:
	Cache_Config_Info():m_bAutoSwitchComport(false)
		,m_bResetAfterDownload(false)
		,m_strDownPath(L"")
		,m_strMode(L"")
	{
		ClearCacheConfigInfo();
	}

	void ClearCacheConfigInfo()
	{
		m_bAutoSwitchComport = (false);
		memset(m_strProgEmmcFirehoseLitembnFilePath,0x00,512);
		memset(m_strRawProgramUnSparseXmlFilePath,0x00,512);
		memset(m_strPatchXmlFilePath,0x00,512);
		memset(m_strRawProgram2XmlFilePath,0x00,512);
		memset(m_strPatch2XmlFilePath,0x00,512);
		m_strRawprograms = L"";
		m_strPatchs = L"";
		m_bResetAfterDownload = (false);
		m_strDownPath = L"";
		m_strMode = L"";
		//! m_strConfigXmlFilePath = L"";
		//! m_dwDownLoadFileSize = 0;
	}
	std::wstring	m_strDownPath;//!下载路径
	std::wstring	m_strMode; //!机型名称
    std::string     m_strMemoryname; //! flash type
	std::wstring	m_strProcessPath;//! 下载进程路径
	std::wstring	m_strCurrentWorkDir;//! 当前工作目录
    std::wstring    mstrDownloadPortSortConfigFilePath;

	std::wstring	m_strConfigXmlFilePath;
	std::wstring	m_wstrProgEmmcFirehoseLitembnFilePath;
	std::wstring	m_strRawprograms;
	std::wstring	m_strPatchs;
 	char			m_strRawProgramUnSparseXmlFilePath[512];
 	char			m_strPatchXmlFilePath[512];
 
 	char			m_strRawProgram2XmlFilePath[512];
 	char			m_strPatch2XmlFilePath[512];

	char			m_strProgEmmcFirehoseLitembnFilePath[512];

	bool			m_bAutoSwitchComport;

	bool			m_bResetAfterDownload; //! 是否下载重启
	//! DWORD64			m_dwDownLoadFileSize;
};
// #define SECTOR_SIZE_IN_BYTES	"SECTOR_SIZE_IN_BYTES"
// #define file_sector_offset		"file_sector_offset"
// #define filename				"filename"
// #define label					"label"
// #define num_partition_sectors	"num_partition_sectors"
// #define physical_partition_number "physical_partition_number"
// #define size_in_KB				"size_in_KB"
// #define start_byte_hex			"start_byte_hex"
// #define start_sector			"start_sector"
// #define sparse					"sparse"

class STRUCT_RAWPROGRAM
{
public:
	STRUCT_RAWPROGRAM():m_strLable("")
		,m_strFileName("")
		,m_strSector_Size_In_Bytes("")
		,m_strfile_sector_offset("")
		,m_strnum_partition_sectors("")
		,m_strphysical_partition_number("")
		,m_strsize_in_KB("")
		,m_strstart_sector("")
		,m_strsparse("")
		,m_strstart_byte_hex("")
		,m_nFileSize(0)
	{

	}
	std::string				m_strSector_Size_In_Bytes;
	std::string				m_strfile_sector_offset;
	std::string				m_strFileName;
	std::string				m_strLable;
	std::string				m_strnum_partition_sectors;
	std::string				m_strphysical_partition_number;
	std::string				m_strsize_in_KB;
	std::string				m_strsparse;
	std::string				m_strstart_byte_hex;
	std::string				m_strstart_sector;
	int						m_nFileSize;
};

typedef std::vector<STRUCT_RAWPROGRAM> _vsRawProgram;

enum ETHREAD_STATUS
{
	THREAD_NOT_STATRA = 0,
	THREAD_IDEL,
	THREAD_RUN,
};

class S_SORT_SHOW_BIND
{
public:
	S_SORT_SHOW_BIND():m_nShowndex(-1)
		,m_nSortIndex(-1)
	{

	}
	int		m_nShowndex;
	int		m_nSortIndex;
};

namespace JGW
{
	class CDownloadThread;
	//! 传递给线程的消息
	class STRUCT_THREAD_ADB_INFO
	{
	public:
		ETHREAD_STATUS			m_nThreadStatus;		//线程状态  空闲 还是工作状态
		bool					m_bIsLock;				//此线程是否已经绑定设备
		CDownloadThread				*m_pThread;				//线程
		int						m_nBindPortIndex;		//绑定端口索引
		int						m_nComPort;
		HWND					m_hMainHwnd;			//主窗口句柄
	};

	typedef class STRUCT_THREAD_ADB_INFO  *LPTHREAD_ADB_INFO;
}



typedef class STRUCT_DEVICE_MSG_INFO *LPSTRUCT_DEVICE_MSG_INFO;
#define CRITICALSpinCount		40000

extern CRITICAL_SECTION						g_csAddInstance;
extern CRITICAL_SECTION						g_csInitCmd;
extern std::vector<JGW::SDeviceMsg_Info*>		g_vDeviceInfo;
namespace JGW
{
    extern Cache_Config_Info					g_sCacheConfigInfo;
}

extern _vsRawProgram						g_sUpdateRawProgram;
extern CRITICAL_SECTION						g_csSwithMode;

extern void SwdlCallBack_one(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);
extern void SwdlCallBack_two(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);
extern void SwdlCallBack_three(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);
extern void SwdlCallBack_four(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);
extern void SwdlCallBack_five(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);
extern void SwdlCallBack_six(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);
extern void SwdlCallBack_seven(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);
extern void SwdlCallBack_eight(HANDLE hContextID, char * sMsg, unsigned short iMsgSize);

const wchar_t g_szSerialNumberDevice[] = _T("adb -s %s get-state\n");
const wchar_t g_szGetSerialNumber[] = _T("adb devices\n");
const wchar_t g_szGetState[] = _T("adb -s %s get-state\n");
const wchar_t g_szRebootbooloader[] = _T("adb -s %s reboot-bootloader\n");
const wchar_t g_szFastbootReboot[] = _T("fastboot -s %s reboot\n");
const wchar_t g_szFastbootDevices[] = _T("fastboot -s %s devices\n");
const wchar_t g_szSelectOemInfo[] = _T("fastboot -s %s oem device-info\n");
const wchar_t g_szOemUnLock[] = _T("fastboot -s %s oem unlock-fdl\n");
const wchar_t g_szFastBootDownCmd[] = _T("fastboot flash %s \"%s\" -s %s\n");
const wchar_t g_szFasterasePartion[] = _T("fastboot erase %s -s %s\n");
const wchar_t g_szADBSwitchEDL[] = _T("adb -s %s reboot edl\n");
const wchar_t g_szADBDevices[] = L"adb -s %s devices\n";