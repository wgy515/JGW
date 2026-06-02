#pragma once
//! 从注册表中读取设备端口
namespace JGW
{
	class CRegDevicePorts
	{
	public:
		CRegDevicePorts(void);
		~CRegDevicePorts(void);

		bool GetDiagFactoryComport(const wchar_t* strRegUsbPidVid,int& nComPort);
		bool GetDiagDownLoadComport(const wchar_t* strRegUsbPidVid,int& nComPort);//QDLoader
		bool GetModemComport(const wchar_t* strRegUsbPidVid,int& nComPort);
	private:
		bool GetComport(const wchar_t* strRegUsbPidVid,int& nComport,const wchar_t* strComportName);
	};
}


