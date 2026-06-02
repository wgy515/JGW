#pragma once
#include <JGW_QMSLFuncPlugin\jgw_device_api.h>
#include <JGW_QMSLFuncPlugin\JGW_QMSLFuncPlugin_Define.h>
//"JGW_device_api.h"
/*
As we know, for ESN, we have NV#0 (NV_ESN_I) and NV#5597 (NV_ESN_ME_I). for MEID, we
have NV#1943 (NV_MEID_I) and NV#5598 (NV_MEID_ME_I). what is the difference between ESN
and ESN_ME, MEID and MEID_ME?

IMEI是国际移动通讯设备识别号(International Mobile Equipment Identity)的缩写，用于GSM系统。
由15位数字组成，前6位(TAC)是型号核准号码，代表手机类型。接着2位(FAC)是最后装配号，代表产地。后6位(SNR)是串号，代表生产顺序号。最后1位(SP)是检验码。

MEID是移动通讯设备识别号(Mobile Equipment IDentifier)的缩写，用于CDMA系统。
由15位16进制数字组成(一般使用前14位)，前8位是生产商编号，后6位是串号，最后1位是检验码。

IMEI校验码算法：
(1).将偶数位数字分别乘以2，分别计算个位数和十位数之和
(2).将奇数位数字相加，再加上上一步算得的值
(3).如果得出的数个位是0则校验位为0，否则为10减去个位数
如：35 89 01 80 69 72 41 偶数位乘以2得到5*2=10 9*2=18 1*2=02 0*2=00 9*2=18 2*2=04 1*2=02,计算奇数位数字之和和偶数位个位十位之和，得到 3+(1+0)+8+(1+8)+0+(0+2)+8+(0+0)+6+(1+8)+7+(0+4)+4+(0+2)=63 => 校验位 10-3 = 7
MEID校验码算法：
(1).将偶数位数字分别乘以2，分别计算个位数和十位数之和，注意是16进制数
(2).将奇数位数字相加，再加上上一步算得的值
(3).如果得出的数个位是0则校验位为0，否则为10(这里的10是16进制)减去个位数
如：AF 01 23 45 0A BC DE 偶数位乘以2得到F*2=1E 1*2=02 3*2=06 5*2=0A A*2=14 C*2=18 E*2=1C,计算奇数位数字之和和偶数位个位十位之和，得到 A+(1+E)+0+2+2+6+4+A+0+(1+4)+B+(1+8)+D+(1+C)=64 => 校验位 10-4 = C
*/
namespace JGW
{
	class JGWQMSLFUNCPLUGIN_API jgw_wr_nv_api:public jgw_device_api
	{
	public:
		jgw_wr_nv_api(void* hResourceContent = NULL,bool bUseQPST = false);
		~jgw_wr_nv_api(void);

		bool	ConnectDevice(unsigned short comPort = QLIB_COM_AUTO_DETECT,int nTimeOuts = 50);

		void	DisConnectDevice();

		//! 设置设备模式
		bool	SetDeviceMode( mode_enum_type phone_mode = MODE_FTM_F );
		//! 确认当前设备是否处于FTM模式
		bool	EnterFTMMode(void);
		//! 检查当前设备所处模式
		bool	CheckFTMMode(unsigned long& _iPhoneOperatingMode);
        //!
        bool    Write24GWiFiSSID(LPCTSTR str24gWiFiSSID,int nCounts = 10); 
        //! 
        bool    Write24GWiFiPassword(LPCTSTR str24gWiFiPassword,int nCounts = 10); 
        //!
        bool    Write24GWiFiSSID(LPCSTR str24gWiFiSSID,int nCounts = 10); 
        //! 
        bool    Write24GWiFiPassword(LPCSTR str24gWiFiPassword,int nCounts = 10); 
        //!
        bool    Write5GWiFiSSID(LPCTSTR str5gWiFiSSID,int nCounts = 10); 
        //! 
        bool    Write5GWiFiPassword(LPCTSTR str5gWiFiPassword,int nCounts = 10); 
        //!
        bool    Write5GWiFiSSID(LPCSTR str5gWiFiSSID,int nCounts = 10); 
        //! 
        bool    Write5GWiFiPassword(LPCSTR str5gWiFiPassword,int nCounts = 10); 

        //!
        bool    WriteADWiFiSSID(LPCTSTR strADWiFiSSID,int nCounts = 10); 
        //! 
        bool    WriteADWiFiPassword(LPCTSTR strADWiFiPassword,int nCounts = 10); 
        //!
        bool    WriteADWiFiSSID(LPCSTR strADWiFiSSID,int nCounts = 10); 
        //! 
        bool    WriteADWiFiPassword(LPCSTR strADWiFiPassword,int nCounts = 10); 

        //!
        bool    Read24GWiFiSSID(std::wstring& str24gWiFiSSID,int nCounts = 10); 
        //! 
        bool    Read24GWiFiPassword(std::wstring& str24gWiFiPassword,int nCounts = 10); 
        //!
        bool    Read5GWiFiSSID(std::wstring& str5gWiFiSSID,int nCounts = 10); 
        //! 
        bool    Read5GWiFiPassword(std::wstring& str5gWiFiPassword,int nCounts = 10); 
        //!
        bool    ReadADGWiFiSSID(std::wstring& strADWiFiSSID,int nCounts = 10); 
        //! 
        bool    ReadADGWiFiPassword(std::wstring& strADWiFiPassword,int nCounts = 10); 
    private:
        //! 
        bool    WriteLynxWiFiInfo(LPCSTR strNumber,size_t offset,int nCounts = 10);
        //! 
        bool    ReadLynxWiFiInfo(std::wstring& strNumber,size_t offset,int nCounts = 10);
    public:
		//! 读写入IMEI
		bool	WriteIMEI1( LPCTSTR strIMEI,int nCounts = 10,unsigned short iContextID = MAIN_SIM );

		bool	ReadIMEI1( std::wstring& strImei,int nCounts = 10,unsigned short iContextID = MAIN_SIM  );

		bool	ReadIMEI1( char* pszImei,size_t size = 16,int nCounts = 10,unsigned short iContextID = MAIN_SIM  );
		//! 读写入IMEI2
		bool	WriteIMEI2( LPCTSTR strIMEI,int nCounts = 10 );

		bool	ReadIMEI2( std::wstring& strImei,int nCounts = 10 );

		bool	ReadIMEI2( char* pszImei,size_t size = 16,int nCounts = 10 );
        //! 
        bool    ReadTinnoSN(std::wstring& strTinnoSN,int nCounts = 10);
        //! 
        bool    ReadBarcode(std::wstring& strBarcode,int nCounts = 10);
        //! 
        bool    ReadTinnoSN(std::string& strTinnoSN,int nCounts = 10);
        //! 
        bool    ReadBarcode(std::string& strBarcode,int nCounts = 10);
        //!
        bool    ReadTinnoSNBarcode(std::string& strBarcode,std::string& strTinnoSN,int nCounts = 10);
        //! 
        bool    WriteTinnoSN(const std::wstring& strTinnoSN,int nCounts = 10);
        //! 
        bool    WriteBarcode(const std::wstring& strBarcode,int nCounts = 10);
        //! 
        bool    WriteTinnoSN(const std::string& strTinnoSN,int nCounts = 10);
        //! 
        bool    WriteBarcode(const std::string& strBarcode,int nCounts = 10);
        //! 目前我们这边高通工具在校准写BARCODE的时候应该是没加入逗号的，包装写SN处理机制是写S前会回读2497后加逗号再写SN
        bool    WriteTinnoSNBarcode(const std::string& strBarcode,const std::string& strTinnoSN,int nCounts = 10);
		//! 读写SN 号段长度不能大于25 大于25 则截断
		bool	WriteSN( LPCTSTR strSN,size_t iLength = 15,int nCounts = 10 );

		bool	WriteSN( LPCSTR pszSN,size_t iLength = 15,int nCounts = 10 );

		bool	ReadSN( char* pszSn,size_t iLength = 15,int nCounts = 10 );

		bool	ReadSN( std::wstring& strSn,size_t iLength = 15,int nCounts = 10 );
		//! 读写客户SN均默认SN号段长度为15
		bool	WriteClientSN( LPCTSTR strClientSN,int nCounts = 10 );

		bool	WriteClientSN( LPSTR pszClientSN,int nCounts = 10 );

		bool	ReadClientSN(std::wstring& strClientSN,int nCounts = 10);
		//! pszClientSN buffer 必须大于iLength + 1
		bool	ReadClientSN(char* pszClientSN,int nCounts = 10);
		//! write gnet bt wifi mac
		bool	WriteGNetBtAddress( LPCTSTR strBtAddress,int nCounts  = 10  );

		bool	WriteGNetWifiMac( LPCTSTR strWifiMac,int nCounts  = 10  );
		//! BT Address
		bool	WriteBtAddress( LPCTSTR strBtAddress,int nCounts = 10 );

		bool	WriteBtAddress( LPCSTR pszBtAddress,int nCounts = 10 );

		bool	ReadBtAddress( std::wstring& strBtAddress,int nCounts = 10 );

		bool	ReadBtAddress( char* pszBtAddress,size_t buf_size = 13,int nCounts = 10 );
		//! WiFi MAC
		bool	WriteWifiMac( LPCTSTR strWifiMac,int nCounts = 10 );

		bool	WriteWifiMac( LPCSTR strWifiMac,int nCounts = 10 );

		bool	ReadWifiMac( char* pszWifiMac,size_t buf_size = 13,int nCounts = 10 );

		bool	ReadWifiMac( std::wstring& strWifiMac,int nCounts = 10 );
		//! MEID
		bool	WriteGNetMEID( LPCTSTR strMeid,int nCounts = 10);

		bool	WriteMEID( LPCTSTR strMeid,int nCounts = 10 );

		bool	WriteMEID( LPCSTR strMeid,int nCounts = 10 );

		bool	ReadMEID( std::wstring & strMeid,int nCounts = 10);

		bool	ReadMEID( char* pszMEID,size_t buf_size,int nCounts = 10 );
		//! PSN
		bool	WritePSN( LPCTSTR strPsn,int iLength = 15,int nCounts = 10);

		bool	ReadPSN( std::wstring & strPsn,int iLength = 15,int nCounts = 10);

	public:
		static void GetCheckMEID( std::string strMEID,char* szMEIDWithCheckSum,size_t length = 16 );

		static void GetCheckIMEI( std::wstring  strIMEI, TCHAR *szImeiWithChecksum,size_t length = 16 );

		static void GetCheckIMEI(std::wstring  strIMEI,char szImeiWithChecksum[16]);
	private:
		bool	GenerateImeiRules(unsigned char srcImei[16],unsigned char *destImei);
		bool	ReadImeiRules( unsigned char* pItemData,size_t pItemLength,std::wstring& strDestImei );
		bool	ReadImeiRules( unsigned char* pItemData,size_t pItemLength,char* strDestImei,size_t destImeiLength = 16 );
	public:
		bool	WriterNV( unsigned short iItemID,unsigned char *pItemData,int iLength,int nCounst = 10 );
		//ID of SIM index NV Item pertains to ( 0 or 1)
		bool	WriterNV( unsigned short iItemID,unsigned char *pItemData,int iLength,unsigned short iContextID,int nCounst = 10 );
		bool	ReadNV( unsigned short iItemID,unsigned char *pItemData,int iLength,int nCounst = 10 );
		bool	ReadNV( unsigned short iItemID,unsigned char *pItemData,int iLength,unsigned short iContextID,int nCounst = 10 );
	};
}


