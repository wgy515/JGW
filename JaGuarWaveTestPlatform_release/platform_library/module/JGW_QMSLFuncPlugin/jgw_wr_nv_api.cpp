#include "StdAfx.h"

#include <time.h>
#include <tchar.h>

#include <JGW_QMSLFuncPlugin/jgw_wr_nv_api.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
// #include "../JGWmsvclibheader/JGWlib.h"




#define IsConnect(x)\
	if( (x) )\
	return true;

namespace JGW
{
	jgw_wr_nv_api::jgw_wr_nv_api(void* hResourceContent,bool bUseQPST):jgw_device_api(hResourceContent,bUseQPST)
	{
	}


	jgw_wr_nv_api::~jgw_wr_nv_api(void)
	{

	}

	void jgw_wr_nv_api::DisConnectDevice()
	{
		//! StopLogging();
		DisconnectServer();
	}

	bool jgw_wr_nv_api::ConnectDevice(unsigned short comPort /* = QLIB_COM_AUTO_DETECT */,int nTimeOuts /* = 50 */)
	{
		static bool bInit = false;

		IsConnect( IsMobileConnected() );
		DisconnectServer();
		InitializeQMSL(jgw_device_api::mbUseQPST);
        if (!mbUseQPST)
        {
            QLIB_SetLibraryMode(mbUseQPST);
            QLIB_SetTargetType(QLIB_TARGET_TYPE_MSM_MDM);
        }
		TESTFUNCOK ( ConnectPhone(nTimeOuts,comPort) );

// 		if ( !bInit )
// 		{
// 			QLIB_ExtendedTextMessage_SetCategoryEnable( GetResourceContext(), QMSL_ExtTextMsgCat_NV_Manager_Debug, 1 );
// 			QLIB_Set_QPHONEMS_Userdefined_Sleep_Counter( GetResourceContext(), 1 );
// 			bInit = true;
// 		}
// 		StartLogging(sLogFile,uiLogFlags);
		return true;
	}

	bool jgw_wr_nv_api::SetDeviceMode( mode_enum_type phone_mode /* = MODE_FTM_F */ )
	{
		unsigned long iPhoneOperatingMode = static_cast<unsigned long>(SYS_OPRT_MODE_NONE);//init to invalid value
		unsigned long desiredPhoneMode = SYS_OPRT_MODE_ONLINE;//default is online

		switch (phone_mode)
		{
		case MODE_FTM_F:
			desiredPhoneMode = SYS_OPRT_MODE_FTM;
			break;
		case MODE_ONLINE_F:
			desiredPhoneMode = SYS_OPRT_MODE_ONLINE;
			break;
		case MODE_OFFLINE_A_F://fall through intentionally
		case MODE_OFFLINE_D_F:
			desiredPhoneMode = SYS_OPRT_MODE_OFFLINE;
			break;
		case MODE_RESET_F:
			desiredPhoneMode = SYS_OPRT_MODE_RESET;
			break;
		case MODE_LPM_F:
			desiredPhoneMode = SYS_OPRT_MODE_LPM;
			break;
		case MODE_POWER_OFF_F:
			desiredPhoneMode = SYS_OPRT_MODE_PWROFF;
			break;
		default:
			PELOG4WW(L"SetPhoneMode -- invalid phone mode.");
			return false;
		}
		//! bool bDone = false;//use a done flag, do not reset bOk as we use bOk as a flag to perform reset
		//! const unsigned long modeCheckTimeout_ms = 6000;//35 worst case on 8960, use 60 with some margin.
		jgw_timer tim;
		while ( IsMobileConnected() && tim.elapsed() < 6000 )
		{
			if ( QLIB_GetPhoneOperatingMode( GetResourceContext(), &iPhoneOperatingMode ) ) //success
			{
				if( iPhoneOperatingMode == desiredPhoneMode )
					return true;//mode already match. in case desired mode is reset or pwr off
				jgw_device_api::SetPhoneMode( phone_mode );
			}
			// sleep if this is a retry
			Sleep( 300 );
		}

		return false;
	}

	bool jgw_wr_nv_api::EnterFTMMode(void)
	{
		return SetDeviceMode( MODE_FTM_F );
		//		unsigned long mode  = -1;

		// 		if (!CheckFTMMode(mode))
		// 		{
		// 			return false;
		// 		}
		// 		else if ( mode != SYS_OPRT_MODE_FTM )
		// 		{
		// 			if ( !SetPhoneMode(MODE_FTM_F) )
		// 			{
		// 				return false;
		// 			}
		// 
		// 			if (!CheckFTMMode(mode) || mode != SYS_OPRT_MODE_FTM)
		// 			{
		// 				return false;
		// 			}
		// 		}
	}

	bool jgw_wr_nv_api::CheckFTMMode(unsigned long& _iPhoneOperatingMode)
	{
		TESTFUNCOK( IsMobileConnected() );
		short _iCommandCode = 0;
		TESTFUNCOK ( QLIB_FTM_GET_COMMAND_CODE(GetResourceContext(),&_iCommandCode) );

		if ( !QLIB_GetPhoneOperatingMode(m_hResourceContext,&_iPhoneOperatingMode) )
		{
			return false;
		}

		if ( FTM_COMMAND_75 != _iCommandCode ) return false;

		return SYS_OPRT_MODE_FTM == _iPhoneOperatingMode;
	}

    bool jgw_wr_nv_api::Write24GWiFiPassword(LPCSTR str24gWiFiPassword,int nCounts /* = 10 */)
    {
        return WriteLynxWiFiInfo(str24gWiFiPassword,offsetof(s_wifi_nv_info,password_2_4G),nCounts);
    }

    bool jgw_wr_nv_api::Write24GWiFiPassword(LPCTSTR str24gWiFiPassword,int nCounts /* = 10 */)
    {
        return Write24GWiFiPassword(JGW_W2A_W(str24gWiFiPassword).c_str(),nCounts);
    }

    bool jgw_wr_nv_api::Write24GWiFiSSID(LPCSTR str24gWiFiSSID,int nCounts /* = 10 */)
    {
        return WriteLynxWiFiInfo(str24gWiFiSSID,offsetof(s_wifi_nv_info,ssid_2_4G),nCounts);
    }

    bool jgw_wr_nv_api::Write24GWiFiSSID(LPCTSTR str24gWiFiSSID,int nCounts /* = 10 */)
    {
        return Write24GWiFiSSID(JGW_W2A_W(str24gWiFiSSID).c_str(),nCounts);
    }

    bool jgw_wr_nv_api::Write5GWiFiPassword(LPCSTR str5gWiFiPassword,int nCounts /* = 10 */)
    {
        return WriteLynxWiFiInfo(str5gWiFiPassword,offsetof(s_wifi_nv_info,password_5G),nCounts);
    }

    bool jgw_wr_nv_api::Write5GWiFiPassword(LPCTSTR str5gWiFiPassword,int nCounts /* = 10 */)
    {
        return Write5GWiFiPassword(JGW_W2A_W(str5gWiFiPassword).c_str(),nCounts);
    }

    bool jgw_wr_nv_api::Write5GWiFiSSID(LPCSTR str5gWiFiSSID,int nCounts /* = 10 */)
    {
        return WriteLynxWiFiInfo(str5gWiFiSSID,offsetof(s_wifi_nv_info,ssid_5G),nCounts);
    }

    bool jgw_wr_nv_api::Write5GWiFiSSID(LPCTSTR str5gWiFiSSID,int nCounts /* = 10 */)
    {
        return Write5GWiFiSSID(JGW_W2A_W(str5gWiFiSSID).c_str(),nCounts);
    }

    bool jgw_wr_nv_api::WriteADWiFiPassword(LPCSTR strADWiFiPassword,int nCounts /* = 10 */)
    {
        return WriteLynxWiFiInfo(strADWiFiPassword,offsetof(s_wifi_nv_info,password_ad),nCounts);
    }

    bool jgw_wr_nv_api::WriteADWiFiPassword(LPCTSTR strADWiFiPassword,int nCounts /* = 10 */)
    {
        return WriteADWiFiPassword(JGW_W2A_W(strADWiFiPassword).c_str(),nCounts);
    }

    bool jgw_wr_nv_api::WriteADWiFiSSID(LPCSTR strADWiFiSSID,int nCounts /* = 10 */)
    {
        return WriteLynxWiFiInfo(strADWiFiSSID,offsetof(s_wifi_nv_info,ssid_ad),nCounts);
    }

    bool jgw_wr_nv_api::WriteADWiFiSSID(LPCTSTR strADWiFiSSID,int nCounts /* = 10 */)
    {
        return WriteADWiFiSSID(JGW_W2A_W(strADWiFiSSID).c_str(),nCounts);
    }

    //!
    bool jgw_wr_nv_api::Read24GWiFiSSID(std::wstring& str24gWiFiSSID,int nCounts/* = 10*/)
    {
        return ReadLynxWiFiInfo(str24gWiFiSSID,offsetof(s_wifi_nv_info,ssid_2_4G),nCounts);
    }
    //! 
    bool jgw_wr_nv_api::Read24GWiFiPassword(std::wstring& str24gWiFiPassword,int nCounts/* = 10*/)
    {
        return ReadLynxWiFiInfo(str24gWiFiPassword,offsetof(s_wifi_nv_info,password_2_4G),nCounts);
    }
    //!
    bool jgw_wr_nv_api::Read5GWiFiSSID(std::wstring& str5gWiFiSSID,int nCounts/* = 10*/)
    {
        return ReadLynxWiFiInfo(str5gWiFiSSID,offsetof(s_wifi_nv_info,ssid_5G),nCounts);
    }
    //! 
    bool jgw_wr_nv_api::Read5GWiFiPassword(std::wstring& str5gWiFiPassword,int nCounts/* = 10*/)
    {
        return ReadLynxWiFiInfo(str5gWiFiPassword,offsetof(s_wifi_nv_info,password_5G),nCounts);
    }
    //!
    bool jgw_wr_nv_api::ReadADGWiFiSSID(std::wstring& strADWiFiSSID,int nCounts/* = 10*/)
    {
        return ReadLynxWiFiInfo(strADWiFiSSID,offsetof(s_wifi_nv_info,ssid_ad),nCounts);
    }
    //! 
    bool jgw_wr_nv_api::ReadADGWiFiPassword(std::wstring& strADWiFiPassword,int nCounts/* = 10*/)
    {
        return ReadLynxWiFiInfo(strADWiFiPassword,offsetof(s_wifi_nv_info,password_ad),nCounts);
    }

    bool jgw_wr_nv_api::ReadLynxWiFiInfo(std::wstring& strNumber,size_t offset,int nCounts /* = 10 */)
    {
        unsigned char wifiNVInfo[LYNX_WIFI_NV_INFO_LEN + 1] = {0};
        s_wifi_nv_info sWiFiNvInfo = {0};
        char szNumber[21] = {0};
        //! 读取失败,则说明当前未写入数据
        if (!ReadNV(LYNX_WIFI_NV_INFO,wifiNVInfo,LYNX_WIFI_NV_INFO_LEN,nCounts)) return false;
        memcpy_s(&sWiFiNvInfo,sizeof(s_wifi_nv_info),wifiNVInfo,sizeof(s_wifi_nv_info));  
        strcpy_s(szNumber,21,((char*)&sWiFiNvInfo) + offset);
        strNumber = JGW_A2W_A(szNumber);
        return true;
    }

    bool jgw_wr_nv_api::WriteLynxWiFiInfo(LPCSTR strNumber,size_t offset,int nCounts /* = 10 */)
    {
        unsigned char wifiNVInfo[LYNX_WIFI_NV_INFO_LEN + 1] = {0};
        s_wifi_nv_info sWiFiNvInfo = {0};
        //! 读取失败,则说明当前未写入数据
        if (ReadNV(LYNX_WIFI_NV_INFO,wifiNVInfo,LYNX_WIFI_NV_INFO_LEN,nCounts))
        {       
            memcpy_s(&sWiFiNvInfo,sizeof(s_wifi_nv_info),wifiNVInfo,sizeof(s_wifi_nv_info)); 
        }
        strcpy_s(((char*)&sWiFiNvInfo) + offset,21,strNumber); 

        for (size_t i = strlen(strNumber);i < 21;i ++)
        {
            (((char*)&sWiFiNvInfo) + offset)[i] = '\0';
        }

        return WriterNV(LYNX_WIFI_NV_INFO,(unsigned char*)&sWiFiNvInfo,sizeof(s_wifi_nv_info),nCounts);
    }

	bool jgw_wr_nv_api::WriteIMEI1( LPCTSTR strImei,int nCounts /* = 10 */,unsigned short iContextID /* = MAIN_SIM */ )
	{
		//! IMEI 为14位，最后一位为校验位
		int length = _tcslen(strImei);
		if ( !(14 == length || 15 == length) )
		{
			PELOG4WW(_T("The input IMEI segment must be 14 or 15 digits"));
			return false;
		}
		//! 
		char szImeiWithChecksum[16] = {0}; 
		GetCheckIMEI( strImei,szImeiWithChecksum );
		if ( 15 != strlen(szImeiWithChecksum) )
		{
			PELOG4WW(_T("Error inputing IMEI section to calculate check code"));
			return false;
		}
		//!
		if ( length == 15 && szImeiWithChecksum[14] !=  strImei[14] )
		{
			PELOG4WW(_T("Error inputing IMEI section check code"));
			return false;
		}

		unsigned char destImei[10] = {0};
		GenerateImeiRules((unsigned char*)szImeiWithChecksum,destImei);

		return WriterNV( IMEINVINDEX,destImei,9,iContextID,nCounts );
	}

	bool jgw_wr_nv_api::WriteIMEI2( LPCTSTR strIMEI,int nCounts )
	{
		return WriteIMEI1( strIMEI,nCounts,SECOND_SIM );
	}

	bool jgw_wr_nv_api::ReadIMEI1( std::wstring& strImei,int nCounts /* = 10 */,unsigned short iContextID /* = MAIN_SIM */ )
	{
		char pseIMEI[16] = {0};
		if ( ReadIMEI1(pseIMEI,16,nCounts,iContextID) )
		{
			strImei = a2w_a( pseIMEI );
			return true;
		}
		return false;
	}

	bool jgw_wr_nv_api::ReadIMEI1( char* pszImei,size_t size /* = 16 */,int nCounts /* = 10 */,unsigned short iContextID /* = MAIN_SIM */ )
	{
		CHECKFUNRES( pszImei && size > 15 );
		unsigned char destImei[10] = {0};
		CHECKFUNRES(ReadNV(IMEINVINDEX,destImei,_countof(destImei),iContextID,nCounts));
		return ReadImeiRules( destImei,_countof(destImei), pszImei,size );
	}

	bool jgw_wr_nv_api::ReadIMEI2( char *pszImei,size_t size /* = 16 */,int nCounts /* = 10 */ )
	{
		return ReadIMEI1(pszImei,size,nCounts,SECOND_SIM);
	}

	bool jgw_wr_nv_api::ReadIMEI2( std::wstring& strImei,int nCounts /* = 10 */ )
	{
		return ReadIMEI1(strImei,nCounts,SECOND_SIM);
	}

	bool jgw_wr_nv_api::WriteSN(LPCSTR pszSN,size_t iLength /* = 15 */,int nCounts /* = 10 */)
	{
		CHECKFUNRES(pszSN);
		unsigned char pszTemp[MAX_SN_LENGTH + 1] = {0};
		if( iLength > MAX_SN_LENGTH ) iLength = MAX_SN_LENGTH;

		for (size_t x = 0;x < (size_t)iLength;x ++)
		{
			pszTemp[x] = (char)pszSN[x] - 48;
		}
		pszTemp[iLength] = '\0';

		return WriterNV(SNNVINDEX,pszTemp,(int)iLength,nCounts);
	}

	bool jgw_wr_nv_api::WriteSN(LPCTSTR strSN,size_t iLength /* = 15 */,int nCounts /* = 10 */)
	{
		CHECKFUNRES(strSN);
		char pszTemp[MAX_SN_LENGTH + 1] = {0};
		for( size_t i = 0;i < iLength; i ++ ) pszTemp[i] = (char)strSN[i];
		return WriteSN(pszTemp,iLength,nCounts);
	}

    bool jgw_wr_nv_api::ReadSN(std::wstring& strSn,size_t iLength /* = 15 */,int nCounts /* = 10 */)
    {
        char pszTemp[MAX_SN_LENGTH + 1] = {0};
        if(!ReadSN(pszTemp,iLength,nCounts)) return false;
        strSn = a2w_a(pszTemp);
        return true;
    }

    bool jgw_wr_nv_api::WriteTinnoSN(const std::wstring& strTinnoSN,int nCounts /* = 10 */)
    {
        return WriteTinnoSN(JGW_W2A(strTinnoSN),nCounts);
    }

    bool jgw_wr_nv_api::WriteTinnoSN(const std::string& strTinnoSN,int nCounts /* = 10 */)
    {
        std::string strBarcode;
        ReadBarcode(strBarcode,3);
        return WriteTinnoSNBarcode(strBarcode,strTinnoSN,nCounts);
    }

    bool jgw_wr_nv_api::WriteBarcode(const std::wstring& strBarcode,int nCounts /* = 10 */)
    {
        return WriteBarcode(JGW_W2A(strBarcode),nCounts);
    }
    //! 目前我们这边高通工具在校准写BARCODE的时候应该是没加入逗号的，包装写SN处理机制是写S前会回读2497后加逗号再写SN
    //! SN为空的时候，写如barcode并将barcode长度后一位改为'\0'
    bool jgw_wr_nv_api::WriteTinnoSNBarcode(const std::string& strBarcode,const std::string& strTinnoSN,int nCounts /* = 10 */)
    {
        unsigned char pszTemp[129] = {0};
        size_t nBarcodeLen = strBarcode.size();
        for (size_t x = 0;x < 128;x ++)
        {
            if (x < nBarcodeLen)
            {
                pszTemp[x] = (char)strBarcode[x];
            }
#if 0
            else if (x == nBarcodeLen)
            {
                pszTemp[x] = '\0';
            }
            else if (x == nBarcodeLen + 1 && !strTinnoSN.empty())
            {
                pszTemp[x] = ',';
            }
            else if (x > nBarcodeLen + 1 && !strTinnoSN.empty() && x < strTinnoSN.length() + nBarcodeLen + 2)
            {
                pszTemp[x] = (char)strTinnoSN[x - nBarcodeLen - 2];
            }
#else
            /*else if (x == nBarcodeLen)
            {
                pszTemp[x] = '\0';
            }*/
            else if (x == nBarcodeLen/* + 1*/ && !strTinnoSN.empty())
            {
                pszTemp[x] = ',';
            }
            else if (x > nBarcodeLen /*+ 1*/ && !strTinnoSN.empty() && x < strTinnoSN.length() + nBarcodeLen + 1)
            {
                pszTemp[x] = (char)strTinnoSN[x - nBarcodeLen - 1];
            }
#endif
        }
        return WriterNV(2497,pszTemp,128,nCounts);
    }

    bool jgw_wr_nv_api::WriteBarcode(const std::string& strBarcode,int nCounts /* = 10 */)
    {
        std::string strTinnoSN = "";
        ReadTinnoSN(strTinnoSN,3);
        return WriteTinnoSNBarcode(strBarcode,strTinnoSN,nCounts);
    }

    bool jgw_wr_nv_api::ReadTinnoSN(std::wstring& strTinnoSN,int nCounts /* = 10 */)
    {
        std::string strAnsiTinnoSN;
        strTinnoSN = L"";
        if (ReadTinnoSN(strAnsiTinnoSN,nCounts))
        {
            strTinnoSN = JGW_A2W(strAnsiTinnoSN);
            return true;
        }
        return false;
    }

    bool jgw_wr_nv_api::ReadBarcode(std::wstring& strBarcode,int nCounts /* = 10 */)
    {
        std::string strAnsiBarcode;
        strBarcode = L"";
        if (ReadBarcode(strAnsiBarcode,nCounts))
        {
            strBarcode = JGW_A2W(strAnsiBarcode);
            return true;
        }
        return false;
    }

    bool jgw_wr_nv_api::ReadTinnoSNBarcode(std::string& strBarcode,std::string& strTinnoSN,int nCounts /* = 10 */)
    {
        strBarcode = "";
        strTinnoSN = "";
        unsigned char pszTemp[129] = {0};
        if (!ReadNV(2497,pszTemp,128,nCounts))
        {
            return false;
        }
        bool bReadBarcode = true,bReadTinnoSN = false;
        for (size_t i = 0;i < 128;i ++)
        {
            if (bReadBarcode && ',' != pszTemp[i])
            {
                if ('\0' == pszTemp[i])
                {
                    bReadBarcode = false;
                    continue;
                }
                strBarcode += (char)pszTemp[i];
                continue;
            }
            if (',' == pszTemp[i] && !bReadTinnoSN)
            {
                bReadBarcode = false;
                bReadTinnoSN = true;
                continue;
            }
            if (bReadTinnoSN)
            {
                if ('\0' == pszTemp[i])
                {
                    bReadTinnoSN = false;
                    continue;
                }
                strTinnoSN += (char)pszTemp[i];
            } 
        }
        return true;
    }

    bool jgw_wr_nv_api::ReadBarcode(std::string& strBarcode,int nCounts /* = 10 */)
    {
        std::string strTinnoSN;
        return ReadTinnoSNBarcode(strBarcode,strTinnoSN,nCounts);
    }

    bool jgw_wr_nv_api::ReadTinnoSN(std::string& strTinnoSN,int nCounts /* = 10 */)
    {
        std::string strBarcode;
        return ReadTinnoSNBarcode(strBarcode,strTinnoSN,nCounts);
    }

	bool jgw_wr_nv_api::ReadSN(char *pszSn,size_t iLength /* = 15 */,int nCounts /* = 10 */)
	{
		unsigned char pszTemp[MAX_SN_LENGTH + 1] = {0};
		if( iLength > MAX_SN_LENGTH ) iLength = MAX_SN_LENGTH;

		bool bResult = ReadNV(SNNVINDEX,pszTemp,iLength,nCounts);
		pszTemp[iLength] = '\0';
		for (size_t x = 0;x < iLength;x ++)
		{
			pszSn[x] = (char)pszTemp[x] + 48;
		}
		pszSn[iLength] = '\0';

		return bResult;
	}

	bool jgw_wr_nv_api::WriteClientSN(LPCTSTR strClientSN,int nCounts /* = 10 */)
	{
        CHECKFUNRES (wcslen(strClientSN) < MAX_CLIENT_SN_LENGTH );
		char szBuffer[MAX_CLIENT_SN_LENGTH] = {0};
		for( size_t i = 0;i < wcslen(strClientSN); i ++ ) szBuffer[i] = (char)strClientSN[i];
		return WriteClientSN(szBuffer,nCounts);
	}

	bool jgw_wr_nv_api::WriteClientSN(LPSTR pszClientSN,int nCounts /* = 10 */)
	{
        CHECKFUNRES (strlen(pszClientSN) < MAX_CLIENT_SN_LENGTH );
		s_sn_nv_info sSNNvInfo = {0};
		//! 读取失败,则说明当前未写入数据
		ReadNV(SNNVINDEX,(unsigned char*)&sSNNvInfo,sizeof(s_sn_nv_info),nCounts);
		strcpy_s(sSNNvInfo.clientsn,MAX_CLIENT_SN_LENGTH,pszClientSN); 
		for (size_t i = strlen(pszClientSN);i < MAX_CLIENT_SN_LENGTH;i ++) sSNNvInfo.clientsn[i] = '\0';
		return WriterNV(SNNVINDEX,(unsigned char*)&sSNNvInfo,sizeof(s_sn_nv_info),nCounts);
	}

	bool jgw_wr_nv_api::ReadClientSN(std::wstring& strClientSN,int nCounts /* = 10 */)
	{
		char szClientSN[ MAX_CLIENT_SN_LENGTH + 1 ] = {0};
		if (!ReadClientSN( szClientSN,nCounts )) return false;
		strClientSN = a2w_a(szClientSN);
		return true;
	}

	bool jgw_wr_nv_api::ReadClientSN(char* pszClientSN,int nCounts /* = 10 */)
	{
		s_sn_nv_info sSNNvInfo = {0};
		//! 读取失败,则说明当前未写入数据
		if (!ReadNV(SNNVINDEX,(unsigned char*)&sSNNvInfo,sizeof(s_sn_nv_info),nCounts)) return false;
		strcpy_s(pszClientSN,MAX_CLIENT_SN_LENGTH,sSNNvInfo.clientsn);
		return true;
	}

	bool jgw_wr_nv_api::WriteGNetBtAddress( LPCTSTR strBtAddress,int nCounts )
	{
		CHECKFUNRES( strBtAddress );
		if( WriteBtAddress(strBtAddress,2) )
		{
			return true;
		}
		std::wstring strReadBt;
		if( ReadBtAddress(strReadBt,2) && NULL == wstrcomparenocasewstr(strBtAddress,strReadBt.c_str()) )
		{
			return true;
		}
		if( WriteBtAddress(strBtAddress,nCounts) )
		{
			return true;
		}
		return false;
	}

	bool jgw_wr_nv_api::WriteGNetWifiMac( LPCTSTR strWifiMac,int nCounts )
	{
		CHECKFUNRES( strWifiMac );
		if( WriteWifiMac(strWifiMac,2) )
		{
			return true;
		}
		std::wstring strReadWifiMac;
		if( ReadWifiMac(strReadWifiMac,2) && NULL == wstrcomparenocasewstr(strWifiMac,strReadWifiMac.c_str()) )
		{
			return true;
		}
		if( WriteWifiMac(strWifiMac,nCounts) )
		{
			return true;
		}
		return false;
	}

	bool jgw_wr_nv_api::WriteBtAddress( LPCSTR pszBtAddress,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( pszBtAddress );
		std::string strBt = pszBtAddress;
		std::wstring strBtAddress( strBt.begin(),strBt.end() );
		return WriteBtAddress( strBtAddress.c_str(),nCounts );
	}

	bool jgw_wr_nv_api::WriteBtAddress( LPCTSTR strBtAddress,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( strBtAddress );
		std::wstring  strBtAddressRule = strBtAddress;
		reverse(strBtAddressRule.begin(),strBtAddressRule.end());

		size_t iLength = strBtAddressRule.length();
		unsigned char szTemp[7] = {0};

		for (size_t x = 1;x < iLength;x += 2)
		{
			szTemp[x/2] = hexCharToInt((char)strBtAddressRule[x]) * 16 + hexCharToInt((char)strBtAddressRule[x-1]);
		}

		return WriterNV(BTADDRESSNVINDEX,(unsigned char *)szTemp,6,nCounts); 
	}

	bool jgw_wr_nv_api::ReadBtAddress( char* pszBtAddress,size_t buf_size /* = 13 */,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( buf_size > 12 );
		unsigned char destBtAddr[7] = {0};
		CHECKFUNRES( ReadNV( BTADDRESSNVINDEX,destBtAddr,_countof(destBtAddr) - 1,nCounts) );
		std::string strBtAddress;
		char szBtAddr[13] = {0};

		for (int x = 0;x < 11;x+=2)
		{
			szBtAddr[x + 1] =  hexIntToChar( destBtAddr[ x/2 ]/16 );
			szBtAddr[x]  = hexIntToChar( destBtAddr[ x/2 ]%16 );
		}
		szBtAddr[12] = '\0';
		strBtAddress = szBtAddr;
		reverse(strBtAddress.begin(),strBtAddress.end());
		strcpy_s(pszBtAddress,buf_size,strBtAddress.c_str());

		return true;
	}

	bool jgw_wr_nv_api::ReadBtAddress( std::wstring& strBtAddress,int nCounts /* = 10 */ )
	{
		char szBtAddr[13] = {0};
		if( ReadBtAddress(szBtAddr,13,nCounts) )
		{
			strBtAddress = a2w_a(szBtAddr);
			return true;
		}
		return false;
	}

	bool jgw_wr_nv_api::WriteWifiMac( LPCSTR strWifiMac,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( strWifiMac );
		std::string strTemp = strWifiMac;
		std::wstring wstrWifiMac( strTemp.begin(),strTemp.end() );
		return WriteWifiMac( wstrWifiMac.c_str(),nCounts );
	}

	bool jgw_wr_nv_api::WriteWifiMac( LPCTSTR strWifiMac,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( strWifiMac );
		unsigned char szTemp[7] = {0};
		size_t iLength = _tcslen(strWifiMac);

		for (size_t x = 1;x < iLength;x += 2)
		{
			szTemp[x/2] = hexCharToInt((char)strWifiMac[x-1]) * 16 + hexCharToInt((char)strWifiMac[x]);
		}

		return WriterNV(WIFIMACNVINDEX,szTemp,6,nCounts);
	}

	bool jgw_wr_nv_api::ReadWifiMac( std::wstring& strWifiMac,int nCounts /* = 10 */ )
	{
		char szMifiMac[13] = {0};
		if( ReadWifiMac(szMifiMac,13,nCounts) )
		{
			strWifiMac = a2w_a(szMifiMac);
			return true;
		}
		return false;
	}

	bool jgw_wr_nv_api::ReadWifiMac( char* pszWifiMac,size_t buf_size /* = 13 */,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( buf_size > 12 );
		unsigned char destMifiMac[7] = {0};
		CHECKFUNRES(ReadNV(WIFIMACNVINDEX,destMifiMac,_countof(destMifiMac) - 1,nCounts));

		for (int x = 0;x < 11;x+=2)
		{
			pszWifiMac[x + 1] =  hexIntToChar( destMifiMac[ x/2 ]%16 );
			pszWifiMac[x]  = hexIntToChar( destMifiMac[ x/2 ]/16 );
		}
		pszWifiMac[12] = '\0';
		return true;
	}

	bool jgw_wr_nv_api::WriteGNetMEID(LPCTSTR strMeid,int nCounts /* = 10 */)
	{
		CHECKFUNRES( strMeid );
		if( WriteMEID(strMeid,2) )
		{
			return true;
		}
		std::wstring strReadMEID;
		if( ReadMEID(strReadMEID,2) && NULL == wstrcomparenocasewstr(strMeid,strReadMEID.c_str()) )
		{
			return true;
		}
		if( WriteMEID(strMeid,nCounts) )
		{
			return true;
		}
		return false;
	}

	bool jgw_wr_nv_api::WriteMEID( LPCTSTR strMeid,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( strMeid && 14 == _tcslen(strMeid) );
		unsigned char szTemp[8] = {0};
		std::wstring  strBtMEIDRule = strMeid;
		reverse(strBtMEIDRule.begin(),strBtMEIDRule.end());
		//size_t iLength = _tcslen(strMeid);

		for (size_t x = 1;x <= strBtMEIDRule.length()&& x < 15;x += 2)
		{
			szTemp[x/2] = hexCharToInt((char)strBtMEIDRule[x]) * 16 + hexCharToInt((char)strBtMEIDRule[x-1]);
		}

		return WriterNV(MEIDNVINDEX,szTemp,7,nCounts);
	}

	bool jgw_wr_nv_api::WriteMEID( LPCSTR strMeid,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( strMeid );
		std::string strTemp = strMeid;
		std::wstring wstrMEID( strTemp.begin(),strTemp.end() );
		return WriteMEID( wstrMEID.c_str(),nCounts );
	}

	bool jgw_wr_nv_api::ReadMEID( char* pszMEID,size_t buf_size,int nCounts /* = 10 */ )
	{
		CHECKFUNRES( buf_size > 15 );
		std::wstring strMEID;
		if ( ReadMEID(strMEID,nCounts) )
		{
			for( size_t i = 0;i < strMEID.length();i ++ ) pszMEID[i] = (char)strMEID[i];
			return true;
		}
		return false;
	}

	bool jgw_wr_nv_api::ReadMEID( std::wstring & strMeid,int nCounts /* = 10 */ )
	{
		unsigned char destMEID[8] = {0};
		CHECKFUNRES(ReadNV(MEIDNVINDEX,destMEID,_countof(destMEID) - 1,nCounts));

		char szMeid[16] = {0};
		for (int x = 0;x < 13;x+=2)
		{
			szMeid[x + 1] =  hexIntToChar( destMEID[ x/2 ]/16 );
			szMeid[x]  = hexIntToChar( destMEID[ x/2 ]%16 );
		}
		szMeid[14] = '\0';
		strMeid = a2w(szMeid);

		reverse(strMeid.begin(),strMeid.end());
		return true;
	}

	bool jgw_wr_nv_api::WritePSN(LPCTSTR strPsn,int iLength /* = 15 */,int nCounts /* = 10 */)
	{
		char *pszTemp = new char[iLength + 1];
		CHECKFUNRES( pszTemp && strPsn );
		memset(pszTemp,0x00,iLength);

		for (size_t x = 0;x < (size_t)iLength;x ++)
		{
			pszTemp[x] = (char)strPsn[x] - 48;
		}
		pszTemp[iLength] = '\0';
		bool bResult = WriterNV(PSNNVINDEX,(unsigned char *)pszTemp,iLength,nCounts);
		delete []pszTemp;
		pszTemp = NULL;

		return bResult;
	}

	bool jgw_wr_nv_api::ReadPSN(std::wstring & strPsn,int iLength /* = 15 */,int nCounts /* = 10 */)
	{
		unsigned char *pszTemp = new  unsigned char[iLength + 1];
		CHECKFUNRES(pszTemp);

		bool bResult = ReadNV(PSNNVINDEX,pszTemp,iLength,nCounts);
		for ( int i = 0;i < iLength;nCounts )
		{
			pszTemp[i] = pszTemp[i] + 48;
		}
		pszTemp[iLength] = '\0';
		strPsn = a2w((const char*)pszTemp);
		delete []pszTemp;
		pszTemp = NULL;

		return bResult;
	}

	bool IsHex(wchar_t c)
	{
		if (c >= '0' && c <= '9') return true;  
		if (c >= 'A' && c <= 'F') return true;  
		if (c >= 'a' && c <= 'f') return true;
		return false;
	}

	int HexToInt( char c )
	{
		if( c >='0'&& c <= '9')
		{
			return c - 48;
		}

		else if( c >= 'a'&& c <= 'f' )
		{
			return c - 'a' + 10;
		}

		else if( c >= 'A'&& c <= 'F' ) 
			return c - 'A' + 10;
		return 0;
	}

	void jgw_wr_nv_api::GetCheckMEID( std::string strMEID,char* szMEIDWithCheckSum,size_t length /* = 16 */ )
	{
		char szMEIDTemp[16] = {0};
		char szTemp[3] = {0};
		int nTemp = 0,nSum = 0;

		strcpy_s( szMEIDTemp,16,strMEID.c_str() );
		strcpy_s( szMEIDWithCheckSum,16,strMEID.c_str() );
		for ( int i = 1;i < 14 ;i += 2)
		{
			nTemp = HexToInt( szMEIDTemp[i] );
			nTemp *= 2;
			itoa(nTemp, szTemp, 16);//转换为是小写16进制
			nSum += HexToInt( szTemp[0] );
			nSum += HexToInt( szTemp[1] );
		}

		for ( int i = 0;i < 14 ;i += 2)
		{
			nTemp = HexToInt( szMEIDTemp[i] );
			nSum += nTemp;
		}

		if ( 0 == nSum % 16 )
		{
			szMEIDWithCheckSum[14] = '0';
		}
		else
		{
			int result = 16 - nSum % 16; 
			if( result <= 9)
				szMEIDWithCheckSum[14] = result + '0';
			else
				szMEIDWithCheckSum[14] = result + 'A' - 10;
			//MakestringToUpper(strMEID);
		}
	}

    void jgw_wr_nv_api::GetCheckIMEI( std::wstring strIMEI, TCHAR *szImeiWithChecksum,size_t length /* = 16 */ )
    {
        std::wstring strImeiWithChecksum;
        if (length < 16 || !CCJGW_NumberRule::GetIMEIChecksum(strIMEI,strImeiWithChecksum)) return;
        _tcscpy_s(szImeiWithChecksum,16, strImeiWithChecksum.c_str());
        return ;
    }

#if 0
	void jgw_wr_nv_api::GetCheckIMEI( std::wstring strIMEI, TCHAR *szImeiWithChecksum,size_t length /* = 16 */ )
	{
		TCHAR szMinIMEI[16] = {0};
		TCHAR szTemp[2] = {0};
		int nCheckSum = 0;

		if( strIMEI.length() > 15 && length < 16 )
			return ;
		_tcscpy_s(szMinIMEI,16, strIMEI.c_str());
		/**********IMEI号算法***********/
		//IMEI号奇数位乘二
		int szMinBuffer[14] = {0};
		int szMaxBuffer[14] = {0};

		for( int i = 1; i < 14; i += 2 )
		{
			szTemp[0] = szMinIMEI[i];
			szMinBuffer[i/2] = _ttoi(szTemp) * 2;
		}

		//与偶数位相加
		for( int i = 0; i < 14; i += 2 )
		{
			szTemp[0] = szMinIMEI[i];
			if(szMinBuffer[i/2] >= 10)
			{
				nCheckSum += szMinBuffer[i/2]/10 + szMinBuffer[i/2]%10 + _ttoi(szTemp) ;
			}
			else
			{
				nCheckSum += szMinBuffer[i/2] +  _ttoi(szTemp);
			}
		}
		_tcscpy_s(szImeiWithChecksum, 16,szMinIMEI);
		//累加值,如果以0结尾,那么Check Digit=0;
		//如果不为零则将十位进一,个位置0,然后减去第二步得到值.
		if(!(nCheckSum % 10))
		{
			szImeiWithChecksum[14] = 0 + '0';
		}
		else
		{
			int nMinuend = ((nCheckSum + 10) / 10)  * 10;
			szImeiWithChecksum[14] = (nMinuend - nCheckSum) + '0';
		}
		szImeiWithChecksum[15] = L'\0';
	}
#endif

	void jgw_wr_nv_api::GetCheckIMEI(std::wstring  strIMEI,char szImeiWithChecksum[16])
	{
		TCHAR szIMEI[16] = {0};
		GetCheckIMEI(strIMEI,szIMEI);
		for(int i = 0;i < 16 && szIMEI[i] != '\0';i++) szImeiWithChecksum[i] = (char)szIMEI[i];
	}

	bool jgw_wr_nv_api::GenerateImeiRules(unsigned char srcImei[16],unsigned char *destImei)
	{
		destImei[0] = 8;
		destImei[1] = (srcImei[0] - 48)*16 + 0xA;

		for (int x = 1,ndest = 2;x < 15;x+=2,ndest++)
		{
			int i = (srcImei[x + 1] - 48);
			destImei[ndest] =i *16 + srcImei[x] - 48;
		}
		return true;
	}

	bool jgw_wr_nv_api::ReadImeiRules( unsigned char* pItemData,size_t pItemLength,std::wstring& strDestImei )
	{
		if (pItemLength < 9)
		{
			return false;
		}

		char srcImei[16] = {0};
		ReadImeiRules(pItemData,pItemLength,srcImei,16);
		strDestImei = a2w(srcImei);

		return true ;
	}

	bool jgw_wr_nv_api::ReadImeiRules( unsigned char* pItemData,size_t pItemLength,char* strDestImei,size_t destImeiLength /* = 16 */ )
	{	
		if (pItemLength < 9 || destImeiLength < 16)
		{
			return false;
		}

		strDestImei[0] = pItemData[1]/16 + '0';

		for (int x = 2;x < 15;x+=2)
		{
			strDestImei[x - 1] =  pItemData[x/2 + 1]%16 + '0';
			strDestImei[x ]  = pItemData[x/2 + 1 ]/16 + '0';
		}

		strDestImei[15] = '\0';

		return true ;
	}

	bool jgw_wr_nv_api::WriterNV( unsigned short iItemID,unsigned char *pItemData,int iLength,unsigned short iContextID,int nCounst /* = 10 */ )
	{
		TESTFUNCOK( IsMobileConnected() );
		for ( int nIndex = 0; nIndex < nCounst ;nIndex ++)
		{
			if( jgw_device_api::WriterNV(iItemID,pItemData,iLength,iContextID) ) return true;
			Sleep(300);
		}

		return false;
	}

	bool jgw_wr_nv_api::WriterNV( unsigned short iItemID,unsigned char *pItemData,int iLength,int nCounst /* = 10 */ )
	{
		TESTFUNCOK( IsMobileConnected() );
		for ( int nIndex = 0; nIndex < nCounst ;nIndex ++)
		{
			if( jgw_device_api::WriterNV(iItemID,pItemData,iLength) ) return true;
			Sleep(300);
		}

		return false;
	}

	bool jgw_wr_nv_api::ReadNV( unsigned short iItemID,unsigned char *pItemData,int iLength,unsigned short iContextID,int nCounst /* = 10 */ )
	{
		TESTFUNCOK( IsMobileConnected() );
		for ( int nIndex = 0; nIndex < nCounst ;nIndex ++)
		{
			if( jgw_device_api::ReadNV(iItemID,pItemData,iLength,iContextID) ) return true;
			Sleep(300);
		}

		return false;
	}

	bool jgw_wr_nv_api::ReadNV( unsigned short iItemID,unsigned char *pItemData,int iLength,int nCounst /* = 10 */ )
	{
		TESTFUNCOK( IsMobileConnected() );
		for ( int nIndex = 0; nIndex < nCounst ;nIndex ++)
		{
			if( jgw_device_api::ReadNV(iItemID,pItemData,iLength) ) return true;
			Sleep(300);
		}

		return false;
	}

}

