#include "StdAfx.h"
#include <JGW_Device_WCNTester_CommonSCPIWLAN/WCNTester_CommonSCPIWLAN.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_StringFuncTemplate.hpp>
#include <JGW_FoundationFunc/vecfunc.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_gpib_agilent.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_tcp_ip.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_VISA.h>
#include <JGW_NIGPIBControllImpl/EquipCommtl_TcpClient.h>
#include <JGW_NIGPIBControllImpl/equipcommtlv1_0_HIVE.h>
#include <xutility>
//#define DEBUG_TEST
namespace JGW
{
	static char _scpiConcatenationToken = ';';
	//! Power Level + _vsaRefLevelHeadspaceDbr = POW:REF
	//! :QCOM:WLAN:SENS:POW:REF 2,25
	static double _vsaRefLevelHeadspaceDbr = 10.0;
	static double _vsaTriggerLevelMaxDbm = -20.0;
    std::string mstrscpiDutIndexList("(@1)");
    std::string mstrscpiPmas("PMAS1");
	std::vector<std::string> mvsTxStationCalPath;
	std::vector<std::string> mvsRxStationCalPath;
	int gnCurrentTxMask = -1;
	int gnCurrentRxMask = -1;
	/* CJGW_IWlanInstrumentInterface* GetWlanInstrumentInterface()
	{
	static WCNTester_CommonSCPIWLAN gWCNTester_CommonSCPIWLAN;
	return &gWCNTester_CommonSCPIWLAN;
	}*/

    WCNTester_CommonSCPIWLAN::WCNTester_CommonSCPIWLAN(void) : mpEqipCommtl(NULL),mbIsRSCWM(true)/*,mstrscpiPmas("PMAS1"),mstrscpiDutIndexList("(@1)")*/,mnnDutIndex(1)
	{
	}


	WCNTester_CommonSCPIWLAN::~WCNTester_CommonSCPIWLAN(void)
	{
		if (mpEqipCommtl)
		{
			delete mpEqipCommtl;
			mpEqipCommtl = NULL;
		}
	}
    #define GetRFTxStrFlags(x) (ERFAC == (x))?"AC":((ERFBC == (x))?"BC":"AO")
    bool WCNTester_CommonSCPIWLAN::CMW_GPS_InitBSE(int nTxNumb /* = ERFAC */,float fCellPower /* = -130 */,double dFreqDL /* = 1575.42 */)
    {
        std::string strCommand;
        if( fCellPower > 0) fCellPower = -fCellPower;
        // ":SOUR:GPRF:GEN1:ARB:REP CONT;"
        // ":TRIG:GPRF:GEN1:ARB:RETR ON;"
        // ":TRIG:GPRF:GEN1:ARB:AUT ON;"
        JGW_FormatString(strCommand,":SOURce:GPRF:GEN:STATe OFF;"
            ":ROUTe:GPRF:GENerator:SCENario:SALone RF%s, TX1;"
            ":SOURce:GPRF:GENerator:RFSettings:FREQuency %.2f MHz;"
            ":SYSTem:BASE:REFerence:FREQuency:SOURce INT;"
            ":SOURce:GPRF:GENerator:BBMode CW;"
            ":SOURce:GPRF:GEN:LIST OFF;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel %.2f;"
            "*OPC?;:SYST:ERR?;"
            ,GetRFTxStrFlags(nTxNumb),dFreqDL,fCellPower
            );
        mpEqipCommtl->write(strCommand);
        mpEqipCommtl->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        int i = 0;
        mpEqipCommtl->write("SOURce:GPRF:GEN:STATe ON");
        for( i = 0; i < 100; i++)
        {
            if( CheckGPRFGenStateIsOn() )break;

            Sleep(1000);
        }

        if (i >= 100)
        {
            return false;
        }

        return true;
    }

    bool WCNTester_CommonSCPIWLAN::CheckGPRFGenStateIsOn()
    {
        std::string strCommand;
        mpEqipCommtl->query("SOUR:GPRF:GEN:STAT?", strCommand);
        return ( std::string::npos != strCommand.find("ON") );
    }

    bool WCNTester_CommonSCPIWLAN::CMW_GPS_FinalizeBSE()
    {
        std::string strCommand;
        strCommand = "SOURce:GPRF:GEN:STATe OFF;"
            "*OPC?;:SYST:ERR?;";
        mpEqipCommtl->write(strCommand);
        mpEqipCommtl->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    /*bool WCNTester_CommonSCPIWLAN::CMW_GPS_ResetMode()*/

	bool WCNTester_CommonSCPIWLAN::ConnectWlanInstrument(PS_MONITOR_INFO psMonitorInfo)
	{
		if (mpEqipCommtl)
		{
			delete mpEqipCommtl;
			mpEqipCommtl = NULL;
		}

		switch (psMonitorInfo->m_cInterfaceType)
		{
		case E_GPIB_HIVE:
			if (AGILENT_GPIB_TYPE == psMonitorInfo->msNIGPIBInstrumentInfo.m_cNIGPIBType)
			{
				mpEqipCommtl = new CEquipCommTL_GPIB_Agilent(psMonitorInfo->msNIGPIBInstrumentInfo.m_nPrimaryAddress,psMonitorInfo->msNIGPIBInstrumentInfo.m_nSecAddress,psMonitorInfo->msNIGPIBInstrumentInfo.m_nGpibInterface);
			}
			else
			{       
				mpEqipCommtl = new CEquipCommTLV1_0_HIVE(psMonitorInfo->msNIGPIBInstrumentInfo.m_nPrimaryAddress,psMonitorInfo->msNIGPIBInstrumentInfo.m_nSecAddress,psMonitorInfo->msNIGPIBInstrumentInfo.m_nGpibInterface);
			}
			break;
		case E_VISA_HIVE:
			mpEqipCommtl = new CEquipCommTL_VISA(psMonitorInfo->msVISAInstrumentInfo.mstrVisaAddress);
			break;
		case E_TCP_IP_HIVE:
			mpEqipCommtl = new CEquipCommtl_TcpClient(psMonitorInfo->msTCPIPInstrumentInfo.mstrIPAddress,psMonitorInfo->msTCPIPInstrumentInfo.mnSocketPort,psMonitorInfo->msTCPIPInstrumentInfo.mstrSourceAddress);
			break;
		default:
			return false;
		}

		return NULL != mpEqipCommtl ? mpEqipCommtl->IsPresent():false;
	}

	void WCNTester_CommonSCPIWLAN::DisConnectWlanInstrument()
	{
		if (mpEqipCommtl)
		{
			mpEqipCommtl->CloseSession();
			delete mpEqipCommtl;
			mpEqipCommtl = NULL;
		}
	}

	bool WCNTester_CommonSCPIWLAN::InitWlanInstrument(const std::wstring& strTRxChainPort, std::vector<S_STATION_CAL_PATH>& vstxStationCalPath,std::vector<S_STATION_CAL_PATH>& vsRxStationCalPath)
	{
		std::vector<int> vTemp;
		JGW_ParserIntW(strTRxChainPort.c_str(),L",",vTemp);
		int ports[4] = {0};
		uint portMask = 1;
		for (size_t i = 0;i < 4 && i < vTemp.size(); i ++)
		{
			ports[i] = vTemp[i];
		}
		for (size_t i = 0;i < 4 - 1 && i < vTemp.size() - 1;i ++)
		{
			portMask = portMask <<  1;
			portMask |= 0x01;
		}

		if(!InitWCNVSATestBox(ports,portMask))
		{
			return false;
		}
		gnCurrentTxMask = -1;
		gnCurrentRxMask = -1;
		mvsTxStationCalPath.clear();
		mvsRxStationCalPath.clear();
		for (size_t i = 0;i < vstxStationCalPath.size();i ++)
		{
			mvsTxStationCalPath.push_back(vstxStationCalPath[i].mstrPathName);
			mvsRxStationCalPath.push_back(vsRxStationCalPath[i].mstrPathName);
		}

		return SetTRxLoss(true,vstxStationCalPath,1) && SetTRxLoss(false,vsRxStationCalPath,1);
	}

	void WCNTester_CommonSCPIWLAN::SwitchLossPath(bool tx,size_t chainMask)
	{
        if (!mbIsRSCWM) return;


		std::string command;
		if (tx)
		{
			if (gnCurrentTxMask != chainMask)
			{
				if (chainMask > 0 && chainMask <= mvsTxStationCalPath.size())
				{
					//! 仪器接收端
					//:QCOM:WLAN:SENS:CORR 'BH0_LP',1,1
					JGW_FormatString(command,":QCOM:WLAN:SENS:CORR '%s',1,%u",mvsTxStationCalPath[chainMask - 1].c_str(),chainMask);
					mpEqipCommtl->write(command);
				}
				gnCurrentTxMask = chainMask;
			}
		}
		else
		{
			if (gnCurrentRxMask != chainMask)
			{
				//! :QCOM:WLAN:SOUR:CORR 'BH0_LP',1,1
				if (chainMask > 0 && chainMask <= mvsTxStationCalPath.size())
				{
					//:QCOM:WLAN:SENS:CORR 'BH0_LP',1,1
					JGW_FormatString(command,":QCOM:WLAN:SOUR:CORR '%s',1,%u",mvsRxStationCalPath[chainMask - 1].c_str(),chainMask);
					mpEqipCommtl->write(command);
				}
				gnCurrentRxMask = chainMask;
			}
			
		}
	}

    bool WCNTester_CommonSCPIWLAN::MeasureCWFreqErrPower(double channelFreqMhz, double powerLevelDbm, uint dutPortMask,double& cwFreqErrorHz,double& cwPower)
    {
        getCWFreqErrPower(channelFreqMhz,powerLevelDbm,cwFreqErrorHz,cwPower,dutPortMask);
        return true;
    }

	bool WCNTester_CommonSCPIWLAN::MeasureCWFreqErr(double channelFreqMhz, double powerLevelDbm, uint dutPortMask,double& cwFreqErrorHz)
	{
		cwFreqErrorHz = getCWFreqErr(channelFreqMhz,powerLevelDbm,dutPortMask);
		return true;
	}

	bool WCNTester_CommonSCPIWLAN::ConnectWCNTestBox(const std::string& host,uint port,const std::string& source)
	{
		if (NULL == mpEqipCommtl)
		{
			mpEqipCommtl = new CEquipCommtl_TcpClient(host,port,source);
		}
		return mpEqipCommtl->IsPresent();
	}

	void WCNTester_CommonSCPIWLAN::DisConnectWCNTestBox()
	{
		if (mpEqipCommtl)
		{
			delete mpEqipCommtl;
			mpEqipCommtl = NULL;
		}
	}

	void WCNTester_CommonSCPIWLAN::SetVsgRfEnable(uint dutPortMask, bool rfEnable, int numOfFramesZeroForContinuous)
	{
		if (rfEnable)
		{
			if (0 == numOfFramesZeroForContinuous)
			{
				std::string strWrite = ":QCOM:WLAN:SOUR:TRIG:TYPE ";
				AppendBitMaskParameters(strWrite,dutPortMask,"CONT");
				mpEqipCommtl->write(strWrite);

				strWrite = ":QCOM:WLAN:SOUR:ARB:STAT ";
				AppendBitMaskStateMask(strWrite,dutPortMask,dutPortMask);
				mpEqipCommtl->write(strWrite);

				strWrite = ":QCOM:WLAN:SOUR:STAT ";
				AppendBitMaskStateMask(strWrite,dutPortMask,dutPortMask);
				mpEqipCommtl->write(strWrite);
			}
			else
			{
				std::string strWrite = ":QCOM:WLAN:SOUR:TRIG:TYPE ";
				AppendBitMaskParameters(strWrite,dutPortMask,"SING");
				mpEqipCommtl->write(strWrite);

				strWrite = ":QCOM:WLAN:SOUR:SWE:COUN ";
				AppendBitMaskParameters(strWrite,dutPortMask,JGW_GetFormatString("%d",numOfFramesZeroForContinuous));
				mpEqipCommtl->write(strWrite);

				strWrite = ":QCOM:WLAN:SOUR:ARB:STAT ";
				AppendBitMaskStateMask(strWrite,dutPortMask,dutPortMask);
				mpEqipCommtl->write(strWrite);

				strWrite = ":QCOM:WLAN:SOUR:STAT ";
				AppendBitMaskStateMask(strWrite,dutPortMask,0);
				mpEqipCommtl->write(strWrite);
			}
			std::string strWrite = ":QCOM:WLAN:SOUR:INIT";
			mpEqipCommtl->write(strWrite);
		}
		else
		{
			std::string strWrite = ":QCOM:WLAN:SOUR:TRIG:TYPE ";
			AppendBitMaskParameters(strWrite,dutPortMask,"CONT");
			mpEqipCommtl->write(strWrite);

			strWrite = ":QCOM:WLAN:SOUR:ARB:STAT ";
			AppendBitMaskStateMask(strWrite,dutPortMask,0);
			mpEqipCommtl->write(strWrite);

			strWrite = ":QCOM:WLAN:SOUR:STAT ";
			AppendBitMaskStateMask(strWrite,dutPortMask,0);
			mpEqipCommtl->write(strWrite);
		}

	}

	bool WCNTester_CommonSCPIWLAN::GetWaveformParameters(uint rate,int& streamCount,std::string& strWifiRate)
	{
		for (int i = 0;i < _countof(WaveformParameters);i ++)
		{
			if (WaveformParameters[i].meWlanRate == rate)
			{
				streamCount = WaveformParameters[i].msWaveformParameter.StreamCount;
				strWifiRate = WaveformParameters[i].msWaveformParameter.WifiRate;
				return true;
			}
		}
		return false;
	}

	//bool WCNTester_CommonSCPIWLAN::InitRxWlanInstrument(uint dutPortMask,uint rate, ,bool LDPC, bool STBC)
	//{
	//    //! SetVsgRfEnable(dutPortMask,rfEnable,numOfFramesZeroForContinuous);
	//    //SetVsgModulationMode(dutPortMask,LDPC,STBC);
	//    //SetVsgPortMask(dutPortMask);
	//    //bool result = CheckForListModeError();

	//    //std::string strWrite = ":QCOM:WLAN:SOUR:ARB:LOAD ",strWifiRate;
	//    //std::vector<WLAN_RATE> vRate;
	//    //vRate.push_back((WLAN_RATE)rate);
	//    //AppendBitMaskWaveformParameters(strWrite,dutPortMask,vRate);
	//    //mpEqipCommtl->write(strWrite);

	//}

	void WCNTester_CommonSCPIWLAN::SetVsgFreqLevel(uint dutPortMask, uint chFreqMHz, double signalLeveldBm)
	{
		std::string strWrite = ":QCOM:WLAN:SOUR:FREQ ";
		/*double chFreqHz = chFreqMHz * 1000 * 1000;
		std::string strFreqHz;
		JGW_FormatString(strFreqHz,"%f",chFreqHz);*/
		__int64 chananel = static_cast<__int64>(chFreqMHz * 1000000.0);
		std::string strFreqHz;
		JGW_FormatString(strFreqHz,"%I64d",chananel);
		AppendBitMaskParameters(strWrite, dutPortMask, strFreqHz);
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SOUR:POW ";
		AppendActiveMaskParameters(strWrite,dutPortMask,static_cast<int>(signalLeveldBm));
		mpEqipCommtl->write(strWrite);
	}

	void WCNTester_CommonSCPIWLAN::InitRxCalibration(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz, double signalLeveldBm,uint numPackets)
	{
		SetVsgPortMask(dutPortMask);
		CheckForListModeError();
		std::string strWrite = ":QCOM:WLAN:SOUR:ARB:LOAD ",strWifiRate;
		std::vector<WLAN_RATE> vRate;
		vRate.push_back(rate);
		AppendBitMaskWaveformParameters(strWrite,dutPortMask,vRate);
		mpEqipCommtl->write(strWrite);
		SetVsgRfEnable(dutPortMask,true,0);
		SetVsgFreqLevel(dutPortMask,chFreqMHz,signalLeveldBm);
		SetVsgRfEnable(dutPortMask,true,numPackets);
	}

	void WCNTester_CommonSCPIWLAN::InitVsgModulation(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz, double signalLeveldBm,bool LDPC, bool STBC)
	{
		SetVsgModulationMode(dutPortMask,LDPC,STBC);
		SetVsgPortMask(dutPortMask);
		CheckForListModeError();
		std::string strWrite = ":QCOM:WLAN:SOUR:ARB:LOAD ",strWifiRate;
		std::vector<WLAN_RATE> vRate;
		vRate.push_back(rate);
		AppendBitMaskWaveformParameters(strWrite,dutPortMask,vRate);
		mpEqipCommtl->write(strWrite);
		SetVsgRfEnable(dutPortMask,false,1);
		SetVsgFreqLevel(dutPortMask,chFreqMHz,signalLeveldBm);
	}

	bool WCNTester_CommonSCPIWLAN::CheckVSGSentDone(uint timeMaxSec /* = 10 */,uint intervalMsec /* = 250 */)
	{
		std::string write (":QCOM:WLAN:SYST:OPC?"),read;
		bool flag = true;
		jgw_timer jt;

		mpEqipCommtl->query(write,read);
		flag = (1 == atoi(read.c_str()));
		while (!flag && jt.elapsed() < timeMaxSec)
		{
			Sleep(intervalMsec);
			mpEqipCommtl->query(write,read);
			flag = (1 == atoi(read.c_str()));
		}
		return flag;
	}

	void WCNTester_CommonSCPIWLAN::SetVsgModulationMode(uint dutPortMask, bool LDPC, bool STBC)
	{
		std::string strWrite = ":QCOM:WLAN:SOUR:FEC ";
		AppendBitMaskParameters(strWrite, dutPortMask, LDPC ? "LDPC" : "BCC");
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SOUR:STBC ";
		AppendBitMaskParameters(strWrite, dutPortMask, STBC ? "1" : "0");
		mpEqipCommtl->write(strWrite);
	}

	bool WCNTester_CommonSCPIWLAN::InitWCNVSATestBox(int values[],uint portMask /* = 15 */)
	{
#if 0
		if (NULL == mpEqipCommtl) return false;
		bool result = true;
		//! CPE项目默认4X4 15 1111(二进制)
		//int dutPortMaskForVsa = 15, dutPortMaskForVsg = 15;
		ClearErrorQueue();
		//! SetPortMaskRoutingIndex(out dutPortMaskForVsa, out dutPortMaskForVsg, timeoutMsec);
		if (!BlockReturnUntilOperationCompleted(":QCOM:SYST:PRES"))
		{
			return false;
		}
		//! ClearErrorQueue();
		std::ostringstream strCommand;
		strCommand << ":QCOM:UNIT:FREQ HZ" << _scpiConcatenationToken;
		strCommand << ":QCOM:UNIT:POW:ABS DBM" << _scpiConcatenationToken;
		strCommand << ":QCOM:UNIT:POW:REL DBC" << _scpiConcatenationToken;
		strCommand << ":QCOM:UNIT:TIME MS" << _scpiConcatenationToken;
		//! mpEqipCommtl->write(strCommand.str());
		//mpEqipCommtl->write(":QCOM:UNIT:FREQ HZ");
		//mpEqipCommtl->write(":QCOM:UNIT:POW:ABS DBM");
		//mpEqipCommtl->write(":QCOM:UNIT:POW:REL DBC");
		//mpEqipCommtl->write(":QCOM:UNIT:TIME MS");
		//! ClearErrorQueue();

		//! 检查仪器版本，默认是支持

		//result &= CheckForListModeError();
		////! WlanSenseRouteDefinePort - QCOM:WLAN:SENSe:ROUTe:PMASk
		std::string strSenseRouteDefinePort (":QCOM:WLAN:SOUR:ROUT:PMAS ");
		uint dutPortMaskTemp = portMask;
		strSenseRouteDefinePort += JGW_GetFormatString("%d",dutPortMaskTemp);
		int num = 0;
		do
		{
			if ((dutPortMaskTemp & 1) != 0)
			{
				strSenseRouteDefinePort += ",";
				strSenseRouteDefinePort += JGW_GetFormatString("%d",values[num++]);
			}
		}
		while ((dutPortMaskTemp >>= 1) != 0);
		ClearErrorQueue();
		strCommand << strSenseRouteDefinePort << _scpiConcatenationToken;
		//mpEqipCommtl->write(strSenseRouteDefinePort);

		dutPortMaskTemp = portMask;
		num = 0;
		strSenseRouteDefinePort = (":QCOM:WLAN:SENS:ROUT:PMAS ");
		strSenseRouteDefinePort += JGW_GetFormatString("%d",dutPortMaskTemp);
		do
		{
			if ((dutPortMaskTemp & 1) != 0)
			{
				strSenseRouteDefinePort += ",";
				strSenseRouteDefinePort += JGW_GetFormatString("%d",values[num++]);
			}
		}
		while ((dutPortMaskTemp >>= 1) != 0);
		//ClearErrorQueue();
		strCommand << strSenseRouteDefinePort << _scpiConcatenationToken;
		mpEqipCommtl->write(strCommand.str());
		//!mpEqipCommtl->write(strSenseRouteDefinePort);
		result &= CheckForListModeError();
		//BlockReturnUntilOperationCompleted
		//mpEqipCommtl->write(":QCOM:SYST:PRES");
		//mpEqipCommtl->query(":QCOM:WLAN:SYST:OPC?",ret);
		std::string ret;
		mpEqipCommtl->query("*IDN?",ret);
		mpEqipCommtl->query(":QCOM:SYST:VERS?",ret);
		mpEqipCommtl->query(":QCOM:SYST:VERS? WLAN",ret);
		mpEqipCommtl->query(":QCOM:SYST:INST:VERS? WLAN",ret);

		SetVsaPortMask(dutPortMaskForVsa);
		SetVsgPortMask(dutPortMaskForVsg);
		//! ClearErrorQueue();

		return result;
#else
		if (NULL == mpEqipCommtl) return false;
		bool result = true;

		ClearErrorQueue();
		if (!BlockReturnUntilOperationCompleted(":QCOM:SYST:PRES"))
		{
			return false;
		}
		ClearErrorQueue();

		std::ostringstream strCommand;
		strCommand << ":QCOM:UNIT:FREQ HZ" << _scpiConcatenationToken;
		strCommand << ":QCOM:UNIT:POW:ABS DBM" << _scpiConcatenationToken;
		strCommand << ":QCOM:UNIT:POW:REL DBC" << _scpiConcatenationToken;
		strCommand << ":QCOM:UNIT:TIME MS" << _scpiConcatenationToken;
		mpEqipCommtl->write(strCommand.str());

		ClearErrorQueue();
		std::string strSenseRouteDefinePort (":QCOM:WLAN:SOUR:ROUT:PMAS ");
		uint dutPortMaskTemp = portMask;
		strSenseRouteDefinePort += JGW_GetFormatString("%d",dutPortMaskTemp);
		int num = 0;
		do
		{
			if ((dutPortMaskTemp & 1) != 0)
			{
				strSenseRouteDefinePort += ",";
				strSenseRouteDefinePort += JGW_GetFormatString("%d",values[num++]);
			}
		}
		while ((dutPortMaskTemp >>= 1) != 0);
		mpEqipCommtl->write(strSenseRouteDefinePort);
		ClearErrorQueue();

		dutPortMaskTemp = portMask;
		num = 0;
		strSenseRouteDefinePort = (":QCOM:WLAN:SENS:ROUT:PMAS ");
		strSenseRouteDefinePort += JGW_GetFormatString("%d",dutPortMaskTemp);
		do
		{
			if ((dutPortMaskTemp & 1) != 0)
			{
				strSenseRouteDefinePort += ",";
				strSenseRouteDefinePort += JGW_GetFormatString("%d",values[num++]);
			}
		}
		while ((dutPortMaskTemp >>= 1) != 0);
		mpEqipCommtl->write(strSenseRouteDefinePort);
		ClearErrorQueue();

		//int dutPortMaskForVsa = 15, dutPortMaskForVsg = 15;
		SetVsaPortMask(portMask);
		SetVsgPortMask(portMask);

		std::string ret;
		mpEqipCommtl->query("*IDN?",ret);
        if (NULL != JGW_StrCaseStr(ret.c_str(),"Rohde&Schwarz"))
        {
            mbIsRSCWM = true;
        }
        else
        {
            mbIsRSCWM = false;
        }
		mpEqipCommtl->query(":QCOM:SYST:VERS?",ret);
		mpEqipCommtl->query(":QCOM:SYST:VERS? WLAN",ret);
		mpEqipCommtl->query(":QCOM:SYST:INST:VERS? WLAN",ret);
#endif
		return true;
	}

    void WCNTester_CommonSCPIWLAN::getCWFreqErrPower(double chFreqMHz, double powerLevelDBm,double& cwFreqErrorHz,double& cwPower, uint dutPortMask /* = 1 */)
    {
        std::string command,ret;
        std::ostringstream strCommand;
        //! :QCOM:WLAN:SENS:CONF:MOD 1,1;:QCOM:WLAN:SENS:CONF:POW 1,1;:QCOM:WLAN:SENS:CONF:SMAS 1,0;:QCOM:WLAN:SENS:PAR 1,'WIFI_11AC_HT20_MCS5',1;:QCOM:WLAN:SENS:FREQ 1,5180000000;:QCOM:WLAN:SENS:POW:REF 1,10;:QCOM:WLAN:SENS:TRIG:LEV 1,-25;:QCOM:WLAN:SENS:AVER:COUN 1,1;:QCOM:WLAN:ERR?
        JGW_FormatString(command,":QCOM:WLAN:SENS:PMAS %d",dutPortMask);
        strCommand << command << _scpiConcatenationToken;
        //! 
        __int64 chananel = static_cast<__int64>(chFreqMHz * 1000000.0);
        JGW_FormatString(command,":QCOM:WLAN:SENS:CW:FREQ %d,%I64d",dutPortMask,chananel);
        strCommand << command << _scpiConcatenationToken;
        //! 
        JGW_FormatString(command,":QCOM:WLAN:SENS:CW:POW:REF %d,%d",dutPortMask,static_cast<int>(powerLevelDBm + _vsaRefLevelHeadspaceDbr));
        strCommand << command << _scpiConcatenationToken;

        command = ":QCOM:WLAN:SENS:CW:INIT";
        strCommand << command << _scpiConcatenationToken;

        command = ":QCOM:WLAN:CW:FETC";
        strCommand << command << _scpiConcatenationToken;

#ifndef DEBUG_TEST
        BlockReturnUntilOperationCompleted(strCommand.str());
#endif
        //! 
        std::vector<double> vresult;
        command = ":QCOM:WLAN:CALC:CW:EFR:HZ?";
        QueryResultWlanActiveMask(command,vresult);
        if (vresult.empty() || vresult[0] == INVAILD_WLAN_RESULT)
        {
            cwFreqErrorHz = INVAILD_WLAN_RESULT;
            return ;
        }
        cwFreqErrorHz = vresult[0] / chFreqMHz;
        //! :QCOM:WLAN:CALC:CW:POW:AVER?
        vresult.clear();
        command = ":QCOM:WLAN:CALC:CW:POW:AVER?";
        QueryResultWlanActiveMask(command,vresult);
        cwPower = vresult[0];
        if (vresult.empty() || vresult[0] == INVAILD_WLAN_RESULT)
        {
            cwPower = INVAILD_WLAN_RESULT;
            return ;
        }
    }

	double WCNTester_CommonSCPIWLAN::getCWFreqErr(double chFreqMHz, double powerLevelDBm,uint dutPortMask /* = 1*/)
	{
		std::string command,ret;
		std::ostringstream strCommand;
		//! :QCOM:WLAN:SENS:CONF:MOD 1,1;:QCOM:WLAN:SENS:CONF:POW 1,1;:QCOM:WLAN:SENS:CONF:SMAS 1,0;:QCOM:WLAN:SENS:PAR 1,'WIFI_11AC_HT20_MCS5',1;:QCOM:WLAN:SENS:FREQ 1,5180000000;:QCOM:WLAN:SENS:POW:REF 1,10;:QCOM:WLAN:SENS:TRIG:LEV 1,-25;:QCOM:WLAN:SENS:AVER:COUN 1,1;:QCOM:WLAN:ERR?
		JGW_FormatString(command,":QCOM:WLAN:SENS:PMAS %d",dutPortMask);
		strCommand << command << _scpiConcatenationToken;
		//! 
		__int64 chananel = static_cast<__int64>(chFreqMHz * 1000000.0);
		JGW_FormatString(command,":QCOM:WLAN:SENS:CW:FREQ %d,%I64d",dutPortMask,chananel);
		strCommand << command << _scpiConcatenationToken;
		//! 
		JGW_FormatString(command,":QCOM:WLAN:SENS:CW:POW:REF %d,%d",dutPortMask,static_cast<int>(powerLevelDBm + _vsaRefLevelHeadspaceDbr));
		strCommand << command << _scpiConcatenationToken;

		command = ":QCOM:WLAN:SENS:CW:INIT";
		strCommand << command << _scpiConcatenationToken;

		command = ":QCOM:WLAN:CW:FETC";
		strCommand << command << _scpiConcatenationToken;

#ifndef DEBUG_TEST
		BlockReturnUntilOperationCompleted(strCommand.str());
#endif
		//! 
		std::vector<double> vresult;
		command = ":QCOM:WLAN:CALC:CW:EFR:HZ?";
		QueryResultWlanActiveMask(command,vresult);
		if (vresult.empty() || vresult[0] == INVAILD_WLAN_RESULT)
		{
			return INVAILD_WLAN_RESULT;
		}
        //! :QCOM:WLAN:CALC:CW:POW:AVER?
        vresult.clear();
        QueryResultWlanActiveMask(command,vresult);
        if (vresult.empty() || vresult[0] == INVAILD_WLAN_RESULT)
        {
            return INVAILD_WLAN_RESULT;
        }

		return vresult[0] / chFreqMHz;
	}

	double WCNTester_CommonSCPIWLAN::GetCurrentConfigAvgPower()
	{
		std::string strWrite = ":QCOM:WLAN:SENS:INIT";
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:FETC";
		BlockReturnUntilOperationCompleted(strWrite);

		//strWrite = ":QCOM:WLAN:CALC:POW:AVER?";
		std::vector<double> vAvgTxPower;
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER?",vAvgTxPower);

		return vAvgTxPower.empty()?INVAILD_WLAN_RESULT:vAvgTxPower[0];
	}

	double WCNTester_CommonSCPIWLAN::GetAvgPower(uint dutPortMask,WLAN_RATE rate,uint chFreqMHz,double powerLevelDBm,int iTriggerLevel,int vsaAverages/* = 1*/)
	{
		double vsaTriggerLevelOffsetDbr = -25.0;
		std::ostringstream strCommand;
		std::string strWrite ;
		SetVsaPortMask(dutPortMask);
		//JGW_FormatString(strWrite,":QCOM:WLAN:SENS:PMAS %d",dutPortMask);
		//strCommand << strWrite << scpiConcatenationToken;
        //! :QCOM:WLAN:SENS:CONF:MOD 1,1;:QCOM:WLAN:SENS:CONF:POW 1,1;:QCOM:WLAN:SENS:CONF:SMAS 1,0;:QCOM:WLAN:SENS:PAR 1,'WIFI_11AC_HT20_MCS5',1;:QCOM:WLAN:SENS:FREQ 1,5180000000;:QCOM:WLAN:SENS:POW:REF 1,10;:QCOM:WLAN:SENS:TRIG:LEV 1,-25;:QCOM:WLAN:SENS:AVER:COUN 1,1;:QCOM:WLAN:ERR?
		//！ AppendWlanTestMethodologyConfiguration
		strWrite = ":QCOM:WLAN:SENS:CONF:MOD ";
		AppendBitMaskParameters(strWrite,dutPortMask,"0");
		strCommand << strWrite << scpiConcatenationToken;

		strWrite = ":QCOM:WLAN:SENS:CONF:POW ";
		AppendBitMaskParameters(strWrite,dutPortMask,"1");
		strCommand << strWrite << scpiConcatenationToken;

		strWrite = ":QCOM:WLAN:SENS:CONF:SMAS ";
		AppendBitMaskParameters(strWrite,dutPortMask,"0");
		strCommand << strWrite << scpiConcatenationToken;
		//！ ConfigureVsaWlanSingleUserMeasurement
		strWrite = ":QCOM:WLAN:SENS:PAR ";
		std::vector<WLAN_RATE> vRate;
		vRate.push_back(rate);
		AppendBitMaskWaveformParameters(strWrite,dutPortMask,vRate);
		strCommand << strWrite << scpiConcatenationToken;

		strWrite = ":QCOM:WLAN:SENS:FREQ ";
		std::string strFreqHz;
		JGW_FormatString(strFreqHz,"%I64d",static_cast<__int64>(chFreqMHz * 1000000.0));
		AppendBitMaskParameters(strWrite, dutPortMask, strFreqHz);
		strCommand << strWrite << scpiConcatenationToken;

		strWrite = ":QCOM:WLAN:SENS:POW:REF ";
		AppendBitMaskParameters(strWrite,dutPortMask,JGW_GetFormatString("%f",powerLevelDBm + _vsaRefLevelHeadspaceDbr));
		strCommand << strWrite << scpiConcatenationToken;

		//! :QCOM:WLAN:SENS:TRIG:LEV 1,-11
		strWrite = ":QCOM:WLAN:SENS:TRIG:LEV ";
		AppendBitMaskParameters(strWrite,dutPortMask,JGW_GetFormatString("%f", vsaTriggerLevelOffsetDbr + powerLevelDBm/*min(powerLevelDBm,iTriggerLevel)*/));
		strCommand << strWrite << scpiConcatenationToken;

		strWrite = ":QCOM:WLAN:SENS:AVER:COUN ";
		AppendBitMaskParameters(strWrite,dutPortMask,JGW_GetFormatString("%d",vsaAverages));
		strCommand << strWrite << scpiConcatenationToken;
        mpEqipCommtl->write(strCommand.str());

        CheckForListModeError();

		strWrite = ":QCOM:WLAN:SENS:INIT";
		//! strCommand << strWrite << scpiConcatenationToken;
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:FETC";
		BlockReturnUntilOperationCompleted(strWrite);

		std::vector<double> vAvgTxPower;
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER?",vAvgTxPower);

		return vAvgTxPower.empty()?INVAILD_WLAN_RESULT:vAvgTxPower[0];
	}

	void WCNTester_CommonSCPIWLAN::MeasureEVM(uint dutPortMask,WLAN_RATE wlan_rate,uint chFreqMHz,double powerLevelDBm,double& evmDb,double& evmRateMbps,double& avgTxPowerDbm,bool bTestEvm,bool bTestMask,int avgTestCount/* = 3*/)
	{
		std::string strWrite = ":QCOM:WLAN:SOUR:TRIG:TYPE ";
		AppendBitMaskParameters(strWrite,dutPortMask,"CONT");
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SOUR:ARB:STAT ";
		AppendBitMaskParameters(strWrite,dutPortMask,"0");
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SOUR:STAT ";
		AppendBitMaskParameters(strWrite,dutPortMask,"0");
		mpEqipCommtl->write(strWrite);

		SetVsaPortMask(dutPortMask);
		CheckForListModeError();

		strWrite = ":QCOM:WLAN:SENS:CONF:MOD ";
		AppendBitMaskParameters(strWrite,dutPortMask,bTestEvm?"1":"0");
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SENS:CONF:POW ";
		AppendBitMaskParameters(strWrite,dutPortMask,"1");
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SENS:CONF:SMAS ";
		AppendBitMaskParameters(strWrite,dutPortMask,bTestMask?"1":"0");
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SENS:PAR ";
		std::vector<WLAN_RATE> vRate;
		vRate.push_back(wlan_rate);
		AppendBitMaskWaveformParameters(strWrite,dutPortMask,vRate);
		mpEqipCommtl->write(strWrite);

		//:QCOM:WLAN:SENS:EVM:OCEM 2,PRE
		//! ChannelEstimationOption Enums
		//! ChannelEstimationOption.preambleOnly) ? "PRE" : "FULL";
		strWrite = ":QCOM:WLAN:SENS:EVM:OCEM ";
		AppendBitMaskParameters(strWrite,dutPortMask,"PRE");
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SENS:FREQ ";
		__int64 chananel = static_cast<__int64>(chFreqMHz * 1000000.0);
		std::string strFreqHz;
		JGW_FormatString(strFreqHz,"%I64d",chananel);
		AppendBitMaskParameters(strWrite, dutPortMask, strFreqHz);
		/*double chFreqHz = chFreqMHz * 1000 * 1000;
		std::string strFreqHz;
		JGW_FormatString(strFreqHz,"%f",chFreqHz);
		AppendBitMaskParameters(strWrite, dutPortMask, strFreqHz);*/
		mpEqipCommtl->write(strWrite);
		//double chFreqHz = chFreqMHz * 1000 * 1000;
		strWrite = ":QCOM:WLAN:SENS:POW:REF ";
		AppendBitMaskParameters(strWrite,dutPortMask,JGW_GetFormatString("%f",powerLevelDBm + _vsaRefLevelHeadspaceDbr));
		mpEqipCommtl->write(strWrite);

		//! :QCOM:WLAN:SENS:TRIG:LEV 1,-11
		strWrite = ":QCOM:WLAN:SENS:TRIG:LEV ";
		AppendBitMaskParameters(strWrite,dutPortMask,JGW_GetFormatString("%f",powerLevelDBm));
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:SENS:AVER:COUN ";
		AppendBitMaskParameters(strWrite,dutPortMask,JGW_GetFormatString("%d",avgTestCount)/* "10"*/);
		mpEqipCommtl->write(strWrite);


        CheckForListModeError();

		strWrite = ":QCOM:WLAN:SENS:INIT";
		mpEqipCommtl->write(strWrite);

		strWrite = ":QCOM:WLAN:FETC";
		BlockReturnUntilOperationCompleted(strWrite);

		std::vector<double> vTest;
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER?",vTest);
		avgTxPowerDbm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  
		if (bTestEvm)
		{
			vTest.clear();
			QueryResultWlanActiveMask(":QCOM:WLAN:CALC:EVM:CHA?",vTest);
			evmDb = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];    

			vTest.clear();
			QueryResultWlanActiveMask(":QCOM:WLAN:CALC:RATE?",vTest);
			evmRateMbps = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  
		}
	}

	void WCNTester_CommonSCPIWLAN::GetEVMTxQuality(double& carrierFreqErrorPpm,double& phaseNoiseDb,double& symbolClockErrorPpm, double& ampImbDb, double& loLeakageDbc,double& phaseImbDeg)
	{
		std::vector<double> vTest;
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:EAMP?",vTest);
		ampImbDb = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];    

		phaseNoiseDb = INVAILD_WLAN_RESULT;
		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:EPH?",vTest);
		phaseImbDeg = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  

		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:ECL?",vTest);
		symbolClockErrorPpm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  

		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:EFR?",vTest);
		carrierFreqErrorPpm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  

		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:LEAK?",vTest);
		loLeakageDbc = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];
	}

	void WCNTester_CommonSCPIWLAN::GetMeasureMask(bool& maskPass,double& marginsViolationPct/*,double& avgTxPowerDbm,std::vector<double>& marginsDbr,std::vector<double>& marginsFreqHz,bool wlanRemoveInBandSegments  = true , bool returnActivePorts  = false*/ )
	{
#if 1
		marginsViolationPct = INVAILD_WLAN_RESULT;
		maskPass = false;
		std::string strRead,strCommand = ":QCOM:WLAN:CALC:SMAS:MARG?";
		std::vector<std::string> vRead;

		mpEqipCommtl->query(strCommand,strRead);
		JGW_ParserStrA(strRead.c_str(),",",vRead);

		if (vRead.size() < 3)
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}

		uint index = 1;
		uint dutPortMask = atoi(vRead[index].c_str());
		uint totalPortCount = 0;
		int activeCount = MaskActiveCount(dutPortMask, totalPortCount);

		if (0 != atoi(vRead[0].c_str()))
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}

		int num5 = 0;
		for (size_t i = 0; i < totalPortCount; i++)
		{
			if (((dutPortMask >> i) & 1) != 0)
			{
				int num6 = atoi(vRead[++index].c_str());
				int num7 = atoi(vRead[++index].c_str());
				marginsViolationPct/*[num5]*/ = ((num6 != 0) ? (100.0 * (double)num7 / (double)num6) : INVAILD_WLAN_RESULT);
				if (0 == marginsViolationPct)
				{
					maskPass = true;
				}
				else
				{
					maskPass = false;
				}
			}
		}
#else

		std::vector<double> vTest;
		int num2 = 1;

		if (!QueryResultWlanActiveMask(":QCOM:WLAN:CALC:SMAS:MARG?",vTest,num2))
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}
		if (vTest.size() < 2)
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}
		//! maskPass = true;
		int num = 0;
		int num6 = static_cast<int>(vTest[num++]);
		int num7 = static_cast<int>(vTest[num++]);
		marginsViolationPct/*[num5]*/ = ((num6 != 0) ? (100.0 * (double)num7 / (double)num6) : INVAILD_WLAN_RESULT);
		if (0 == marginsViolationPct)
		{
			maskPass = true;
		}
		else
		{
			maskPass = false;
		}
#endif
#if 0
		size_t totalPortCount;
		int num3 = MaskActiveCount(num2, totalPortCount);
		int num4 = returnActivePorts ? num3 : totalPortCount;
		int num5 = 0;
		int num = 0;
		for (size_t i = 0; i < totalPortCount; i++)
		{
			if (((num2 >> i) & 1) != 0)
			{
				int num6 = static_cast<int>(vTest[num++]);
				int num7 = static_cast<int>(vTest[num++]);
				marginsViolationPct/*[num5]*/ = ((num6 != 0) ? (100.0 * (double)num7 / (double)num6) : INVAILD_WLAN_RESULT);
				int num8;
				int num9;
				int num10;
				if (wlanRemoveInBandSegments)
				{
					num8 = (int)((double)num6 / 2.0 - 0.5);
					num9 = ((num6 % 2 != 0) ? 1 : 2);
					num10 = num8 + num9 - 1;
				}
				else
				{
					num8 = num6;
					num9 = 0;
					num10 = num6;
				}
				/*marginsFreqHz[num5] = new double[num6 - num9];
				marginsDbr[num5] = new double[num6 - num9];*/
				for (int j = 0; j < num6; j++)
				{
					if (j < num8)
					{
						marginsFreqHz.push_back(vTest[num++]);
						//marginsFreqHz[num5][j] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
						marginsDbr.push_back(vTest[num++]);
						//marginsDbr[num5][j] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
					}
					else if (j > num10)
					{
						marginsFreqHz.push_back(vTest[num++]);
						marginsDbr.push_back(vTest[num++]);
						//marginsFreqHz[num5][j - num9] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
						//marginsDbr[num5][j - num9] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
					}
					else
					{
						num += 2;
					}
				}
				/*num5++;*/
			}
			else if (!returnActivePorts)
			{
				marginsViolationPct = 0.0;
				//marginsFreqHz[num5] = new double[0];
				//marginsDbr[num5++] = new double[0];
			}
		}

		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:SMAS:MARG:FREQ:ENDP?",vTest);



		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER?",vTest);
		avgTxPowerDbm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];
#endif
	}

	bool WCNTester_CommonSCPIWLAN::GetPower2(double chFreqMHz, uint chainMask, WLAN_DataRate rate, double* powerLevelDBm, uint numAvg, double* oPower)
	{
		return false;
	}

	bool WCNTester_CommonSCPIWLAN::MeasurePower(WLAN_RATE rate, double channelFreqMhz, std::vector<double>& vPowerLevelDbm, ChainMask txChainMask, TriggerType triggerType, double* pathLossDb, std::vector<double>& vsaTriggerLevelOffsetDbr, bool shortGuardInterval, double* powerAvgEachBurst, double* powerPkEachBurst, int vsaAverages /* = 1 */)
	{
		std::string command;
		std::vector<WLAN_RATE> vWlanRate;
		std::vector<__int64> vChannelFreqHz;
		//! std::vector<double> vPowerLevelDbm;
		std::vector<uint> vPortBonding;
		std::vector<uint> vVsaAverages;
		std::vector<std::string> vWlanType;
		std::vector<double> avgTxPowerMinDbm,avgTxPowerMaxDbm,pkTxPowerMinDbm,pkTxPowerMaxDbm;
		std::vector<WLAN_P_TRANSMIT_POWER_CLASS> vPowerClass11p;

		vPortBonding.push_back(0);
		vVsaAverages.push_back(vsaAverages > 1 ? vsaAverages : 1);
		vWlanRate.push_back(rate);
		vChannelFreqHz.push_back(static_cast<__int64>(channelFreqMhz * 1000000.0));
		vWlanType.push_back("");
		vWlanType.push_back("");
		vPowerClass11p.push_back(CLASS_A);

		SetVsaPortMask(txChainMask);
		ClearErrorQueue();
		AppendWlanTestMethodologyConfiguration(txChainMask, true, true, false, NULL);
		ConfigureVsaWlanSingleUserMeasurement(txChainMask, vWlanRate, vPortBonding, vChannelFreqHz, vPowerLevelDbm, vsaTriggerLevelOffsetDbr, vVsaAverages, vPowerClass11p, vWlanType);
		InitiateFetchVsaData();

		GetWlanPowerAvg(powerAvgEachBurst, avgTxPowerMinDbm,  avgTxPowerMaxDbm, vWlanType);
		GetWlanPowerPeak(powerPkEachBurst, pkTxPowerMinDbm, pkTxPowerMaxDbm, vWlanType);

		return true;
	}

	uint WCNTester_CommonSCPIWLAN::sigGenTransmit(int inputSignalStrength_dBm, uint freq, WLAN_DataRate rate, uint numPackets, uint chain)
	{
		bool short11b_nGuard = true;
		WLAN_RATE rate2;
		uint dutPortMask = 1 << (int)chain;
		WLAN_Gen6_PHYDBG_PreambleRate preabmbleRate;
		std::vector<uint> vPortBonding(1,0);

		mapWlanRate(rate,short11b_nGuard,rate2,preabmbleRate);
		SetVsgModulationPortMask(dutPortMask,rate2,vPortBonding);
		VsgEnableRfPortMask(dutPortMask,false,1);
		VsgSetPortMask(dutPortMask,freq,inputSignalStrength_dBm);
		VsgEnableRfPortMask(dutPortMask,true,0);
		VsgEnableRfPortMask(dutPortMask,true,numPackets);

		int num2 = 100;
		while (!BlockReturnUntilOperationCompleted(":QCOM:WLAN:SYST:OPC?"))
		{
			Sleep(10);
			num2--;
			if (num2 <= 0)
			{
				return 0;;
			}
		}

		return 0;
	}

	bool WCNTester_CommonSCPIWLAN::ClearErrorQueue()
	{
		std::string query = ":QCOM:WLAN:ERR?",ret,temp;
		for (int i = 0; i < 256; i++)
		{
			mpEqipCommtl->query(query,ret);
			if (std::string::npos != ret.find(","))
			{
				temp = ret.substr(0,ret.find(","));
				JGW_ReplaceStringA(temp,"+","");
				if (!temp.empty() && temp[0] == '0')
				{
					return true;
				}
			}
		}
		return false;
	}

	bool WCNTester_CommonSCPIWLAN::CheckForListModeError()
	{
		std::string ret,temp;
		mpEqipCommtl->query(":QCOM:WLAN:ERR?",ret);
		//! 
		if (std::string::npos == ret.find(","))
		{
			return false;
		}

		//! if (text.Substring(0, text.IndexOf(',')).Trim() == "#" || !(text.Substring(0, text.IndexOf(',')).Trim() != "0"))
		temp = ret.substr(0,ret.find(","));
		if (std::string::npos != temp.find('#') || std::string::npos != temp.find('0'))
		{
			return true;
		}

		return false;
	}

	void WCNTester_CommonSCPIWLAN::WlanSystemPreset(long timeMaxSec /* = 120L */)
	{
		ClearErrorQueue();

	}

	bool WCNTester_CommonSCPIWLAN::BlockReturnUntilOperationCompleted(const std::string& command,uint timeMaxSec /* = 60 */,uint intervalMsec /* = 250 */)
	{
		if (command.empty()) return false;

		std::string write (":QCOM:WLAN:SYST:OPC?"),read;
		bool flag = true;
		jgw_timer jt;

		mpEqipCommtl->write(command);
		mpEqipCommtl->query(write,read);

		flag = (1 == atoi(read.c_str()));
		while (!flag && jt.elapsed() < timeMaxSec)
		{
			Sleep(intervalMsec);
			mpEqipCommtl->query(write,read);
			flag = (1 == atoi(read.c_str()));
		}
		return flag;
	}

	void WCNTester_CommonSCPIWLAN::SetVsaPortMask(uint dutPortMask)
	{
		std::string command;
		JGW_FormatString(command,":QCOM:WLAN:SENS:PMAS %d",dutPortMask);
		mpEqipCommtl->write(command);
		ClearErrorQueue();
	}

	void WCNTester_CommonSCPIWLAN::SetVsgPortMask(uint dutPortMask)
	{
		std::string command;
		JGW_FormatString(command,":QCOM:WLAN:SOUR:PMAS %d",dutPortMask);
		mpEqipCommtl->write(command);
		ClearErrorQueue();
	}
	//! 
	bool WCNTester_CommonSCPIWLAN::SetTRxLoss(bool dutTxTable,std::vector<S_STATION_CAL_PATH>& vsStationCalPath,int dutIndex /* = 1 */)
	{
		//! 2412 ~ 2482 2484 4900 - 7200 间隔5MHZ
		int index = 0,maxIndex = (2482-2412)/5 + 1 + 1 + (7200 - 4900) / 5 + 1;
		uint dFreqArray[(2482-2412)/5 + 1 + 1 + (7200 - 4900) / 5 + 1] = {0};
		double dLossArray[(2482-2412)/5 + 1 + 1 + (7200 - 4900) / 5 + 1] = {0};
		size_t numTRxStationCalPathName = 0;
		std::vector<std::string> vTRxStationCalPathName;
		std::vector<int> vRfPortIndex;
		std::vector<int> vdutIndex;

		for (index = 0;index < (2482-2412)/5 + 1;index ++)
		{
			dFreqArray[index] = 2412 + 5 * index;
		}
		dFreqArray[index++] = 2484;
		for (int i = 0;i < (7200 - 4900)/5 + 1;index ++,i ++)
		{
			dFreqArray[index] = 4900 + 5 * i;
		}

		index = 0;
		std::vector<S_STATION_CAL_PATH>::iterator it = vsStationCalPath.begin();
		while (it != vsStationCalPath.end())
		{
			if ((0 == numTRxStationCalPathName || -1 == find_if(vTRxStationCalPathName,std::bind2nd(std::equal_to<std::string>(), it->mstrPathName))) && !it->mstrPathName.empty())
			{
				vTRxStationCalPathName.push_back(it->mstrPathName);
				numTRxStationCalPathName ++;
			}
			vRfPortIndex.push_back(index + 1);
			index ++;
			++it;
		}

		if (index == 0)
		{
			return false;
		}
		//! 
		for (size_t i = 0;i < vRfPortIndex.size();i ++)
		{
			vdutIndex.push_back(dutIndex);
		}

		for (size_t i = numTRxStationCalPathName;i < vRfPortIndex.size();i ++)
		{
			vTRxStationCalPathName.push_back(vTRxStationCalPathName[0]);
		}

		//! 
		for (size_t i = 0;i < numTRxStationCalPathName;i++)
		{
			//! 设置频率对应线损
			for (int x = 0; x < maxIndex;x ++)
			{
				//! 
				dLossArray[x] = GetLossToLossData(dFreqArray[x],vsStationCalPath[i].mvsLossData);
			}
			CreatePathLossCorrectionTable(vTRxStationCalPathName.at(i),maxIndex,dFreqArray,dLossArray);
		}

		if (dutTxTable)
		{
			SetDutTxPathLossCorrectionTable(vTRxStationCalPathName,vdutIndex,vRfPortIndex);
		}
		else
		{
			SetDutRxPathLossCorrectionTable(vTRxStationCalPathName,vdutIndex,vRfPortIndex);
		}

		return true;
	}

	void WCNTester_CommonSCPIWLAN::CreatePathLossCorrectionTable(const std::string& correctionTableName,uint frequencyCount, uint* frequencyMHz, double* pathLossDb)
	{
		std::string command,strTemp;
		//command += correctionTableName;
		JGW_FormatString(command,":QCOM:WLAN:MMEM:STOR:CORR '%s'",correctionTableName.c_str());
#ifndef DEBUG_TEST
		mpEqipCommtl->write(command);
#endif

		JGW_FormatString(command,":QCOM:WLAN:MMEM:ADD:CORR '%s'",correctionTableName.c_str());
		for (size_t i = 0; i < frequencyCount; i++)
		{
			__int64 freq = frequencyMHz[i]; 
			//* 1000 * 1000;
			JGW_FormatString(strTemp,",%I64d,%.1f",freq * 1000 * 1000,pathLossDb[i]);
			command += strTemp;
		}
#ifndef DEBUG_TEST
		mpEqipCommtl->write(command);
#endif
	}

	void WCNTester_CommonSCPIWLAN::SetDutTxPathLossCorrectionTable(std::vector<std::string>& correctionTableName, std::vector<int>& dutIndex,std::vector<int>& rfPortIndex)
	{
		//! :QCOM:WLAN:SENS:CORR 'BH0_LP',1,1,'BH0_LP',1,2,'BH0_LP',1,3,'BH0_LP',1,4
		std::string command(":QCOM:WLAN:SENS:CORR "),strTemp;
		for (size_t i = 0;i < rfPortIndex.size();i ++)
		{
			if (i == rfPortIndex.size() - 1)
			{
				JGW_FormatString(strTemp,"'%s',%d,%d",correctionTableName[i].c_str(),dutIndex[i],rfPortIndex[i]);
			}
			else
			{
				JGW_FormatString(strTemp,"'%s',%d,%d,",correctionTableName[i].c_str(),dutIndex[i],rfPortIndex[i]);
			}
			command += strTemp;
		}
#ifndef DEBUG_TEST
		mpEqipCommtl->write(command);
#endif
	}

	void WCNTester_CommonSCPIWLAN::SetDutRxPathLossCorrectionTable(std::vector<std::string>& correctionTableName, std::vector<int>& dutIndex,std::vector<int>& rfPortIndex)
	{
		//! :QCOM:WLAN:SOUR:CORR 'BH0_LP',1,1,'BH0_LP',1,2,'BH0_LP',1,3,'BH0_LP',1,4
		std::string command(":QCOM:WLAN:SOUR:CORR "),strTemp;
		for (size_t i = 0;i < rfPortIndex.size();i ++)
		{
			if (rfPortIndex.size() - 1 == i)
			{
				JGW_FormatString(strTemp,"'%s',%d,%d",correctionTableName[i].c_str(),dutIndex[i],rfPortIndex[i]);
			}
			else
			{
				JGW_FormatString(strTemp,"'%s',%d,%d,",correctionTableName[i].c_str(),dutIndex[i],rfPortIndex[i]);
			}
			command += strTemp;
		}
#ifndef DEBUG_TEST
		mpEqipCommtl->write(command);
#endif
	}

	double WCNTester_CommonSCPIWLAN::GetLossToLossData(uint freq,std::vector<S_LOSS_Data>& sLossData)
	{
		//! loss data 数据线损从小到大排序
		double loss = 0.00,detal = 0;
		uint detalChannel = 0,detalCurrentChannel;
		for (size_t i = 0;i < sLossData.size() - 1; i ++)
		{
			if (freq == sLossData[i].mnChannel)
			{
				return sLossData[i].mfLoss;
			}
			if (freq > sLossData[i].mnChannel && freq < sLossData[i + 1].mnChannel)
			{
				//! 区间信道数除以区间差值线损得到每个单位信道对应线损大小在 乘以当前信道减去起始信道差值
				//! 
				if (sLossData[i + 1].mfLoss == sLossData[i].mfLoss)
				{
					return sLossData[i].mfLoss;
				}
				else
				{
					detal = sLossData[i + 1].mfLoss - sLossData[i].mfLoss;
					detalChannel = sLossData[i + 1].mnChannel - sLossData[i].mnChannel;
					detalCurrentChannel = freq - sLossData[i].mnChannel;

					loss = sLossData[i].mfLoss + (detal / detalChannel) * detalCurrentChannel;
				}
				return loss;
			}
		}
		return sLossData[sLossData.size() - 1].mfLoss;
	}

	bool WCNTester_CommonSCPIWLAN::QueryResultWlanActiveMask(const std::string& query,std::vector<double>& vResult,int& dutPortMask)
	{
		std::string read;
		std::vector<std::string> vstrTemp;
#ifndef DEBUG_TEST
		mpEqipCommtl->query(query,read);
#else
		read = "0,1,-11846.64941";
#endif
		JGW_ParserStrA(read.c_str(),",",vstrTemp);
		if (vstrTemp.size() < 3)
		{
			vResult.push_back(INVAILD_WLAN_RESULT);
			return false;
		}
		dutPortMask = atoi(vstrTemp[1].c_str());
		size_t num = MaskActiveCount(atoi(vstrTemp[1].c_str()));
		//! 第一个返回值0 表示OK 检查返回的值和返回字串大小长度是否一致
		if (0 != atoi(vstrTemp[0].c_str())/*
										  || num < vstrTemp.size() - 2*/)
		{
			for (size_t i = 0;i < num;i ++)
			{
				vResult.push_back(INVAILD_WLAN_RESULT);   
			}
			return false;
		}
		else
		{
			for (size_t i = 0;i < num;i ++)
			{
				vResult.push_back(atof(vstrTemp[i + 2].c_str()));
			}
		}
		return true;
	}

	bool WCNTester_CommonSCPIWLAN::QueryResultWlanActiveMask(const std::string& query,std::vector<double>& vResult)
	{
		int dutPortMask = 1;
		return QueryResultWlanActiveMask(query,vResult,dutPortMask);
		//        std::string read;
		//        std::vector<std::string> vstrTemp;
		//#ifndef DEBUG_TEST
		//        mpEqipCommtl->query(query,read);
		//#else
		//        read = "0,1,-11846.64941";
		//#endif
		//        JGW_ParserStrA(read.c_str(),",",vstrTemp);
		//        if (2 != vstrTemp.size())
		//        {
		//            vResult.push_back(INVAILD_WLAN_RESULT);
		//            return false;
		//        }
		//        int num = MaskActiveCount(atoi(vstrTemp[1].c_str()));
		//        //! 第一个返回值0 表示OK 检查返回的值和返回字串大小长度是否一致
		//        if (0 != atoi(vstrTemp[0].c_str())
		//            || num < vstrTemp.size() - 2)
		//        {
		//            for (int i = 0;i < num;i ++)
		//            {
		//                vResult.push_back(INVAILD_WLAN_RESULT);   
		//            }
		//            return false;
		//        }
		//        else
		//        {
		//            for (int i = 0;i < num;i ++)
		//            {
		//                vResult.push_back(atoi(vstrTemp[i + 2].c_str()));
		//            }
		//        }
		//        return true;
	}

	void WCNTester_CommonSCPIWLAN::AppendActiveMaskParameters(std::string& commands, uint portMask, int value)
	{
		commands += JGW_GetFormatString("%u",portMask);
		do
		{
			if ((portMask & 1) != 0)
			{
				commands += ",";
				commands += JGW_GetFormatString("%d",value);
			}
		}
		while ((portMask >>= 1) != 0);
	}

	void WCNTester_CommonSCPIWLAN::AppendWlanTestMethodologyConfiguration(uint portMask, bool modulationEnabled, bool spectralPowerEnabled, bool spectralMaskEnabled, char* errorFrequencyEnabled)
	{
		std::string commnad;

		commnad = ":QCOM:WLAN:SENS:CONF:MOD ";
		AppendActiveMaskParameters(commnad,portMask,modulationEnabled?1:0);
		mpEqipCommtl->write(commnad);

		commnad = ":QCOM:WLAN:SENS:CONF:POW ";
		AppendActiveMaskParameters(commnad,portMask,spectralPowerEnabled?1:0);
		mpEqipCommtl->write(commnad);

		commnad = ":QCOM:WLAN:SENS:CONF:SMAS ";
		AppendActiveMaskParameters(commnad,portMask,spectralMaskEnabled?1:0);
		mpEqipCommtl->write(commnad);

		if (NULL != errorFrequencyEnabled)
		{
			commnad = ":QCOM:WLAN:SENS:CONF:EFR ";
			AppendActiveMaskParameters(commnad,portMask,atoi(errorFrequencyEnabled) > 0?1:0);
			mpEqipCommtl->write(commnad);
		}
	}

	void WCNTester_CommonSCPIWLAN::ConfigureVsaWlanSingleUserMeasurement(uint dutPortMask, std::vector<WLAN_RATE>& vWlanRate, std::vector<uint>& vPortBonding, std::vector<__int64>& vVhannelFreqHz, std::vector<double>& vPowerLevelDbm, std::vector<double>& vVsaTriggerLevelOffsetDbr, std::vector<uint>& vVsaAverages, std::vector<WLAN_P_TRANSMIT_POWER_CLASS>& vPowerClass11p, std::vector<std::string>& vWlanType)
	{
		/*if (dutPortMask == 0)
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: No specified ports, port mask = zero (0)");
		}
		if (rate == null || rate.Length == 0)
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: rate array size = zero (0)");
		}
		if (channelFreqHz == null || channelFreqHz.Length == 0)
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: channelFreqHz array size = zero (0)");
		}
		if (powerLevelDbm == null || powerLevelDbm.Length == 0)
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: powerLevelDbm array size = zero (0)");
		}
		if (vsaTriggerLevelOffsetDbr == null || vsaTriggerLevelOffsetDbr.Length == 0)
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: vsaTriggerLevelOffsetDbr array size = zero (0)");
		}
		if (vsaAverages == null || vsaAverages.Length == 0)
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: vsaAverages array size = zero (0)");
		}
		if (rate[0].ToString().Contains("80P80") && (portBonding == null || portBonding.Length == 0))
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: channelPortBonding array size = zero (0)");
		}
		if (rate[0].ToString().Contains("11P_") && (powerClass11p == null || powerClass11p.Length == 0))
		{
		throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: powerClass11p array size = zero (0)");
		}*/
		size_t totalPortCount = 0;

		MaskActiveCount(dutPortMask,totalPortCount);
		//! vWlanRate.push_back()
		while (totalPortCount > vWlanRate.size())
		{
			vWlanRate.push_back(vWlanRate[0]);
		}
		while (totalPortCount > vPortBonding.size())
		{
			vPortBonding.push_back(vPortBonding[0]);
		}
		while (totalPortCount > vPowerLevelDbm.size())
		{
			vPowerLevelDbm.push_back(vPowerLevelDbm[0]);
		}
		while (totalPortCount > vVsaTriggerLevelOffsetDbr.size())
		{
			vVsaTriggerLevelOffsetDbr.push_back(vVsaTriggerLevelOffsetDbr[0]);
		}
		while (totalPortCount > vVsaAverages.size())
		{
			vVsaAverages.push_back(vVsaAverages[0]);
		}
		while (totalPortCount > vPowerClass11p.size())
		{
			vPowerClass11p.push_back(vPowerClass11p[0]);
		}
		while (totalPortCount > vWlanType.size())
		{
			vWlanType.push_back(vWlanType[0]);
		}

		double _vsaRefLevelHeadspaceDbr = 10.0;
		double _vsaTriggerLevelMaxDbm = -20.0;

		std::string strWifiRate;
		//double[] array3 = new double[totalPortCount];
		std::vector<double> array3 (totalPortCount,0);
		//double[] array4 = new double[totalPortCount];
		std::vector<double> array4 (totalPortCount,0);
		//!
		std::vector<double> vsaRefLevelHeadspaceDbr(totalPortCount,_vsaRefLevelHeadspaceDbr);
		//uint[] array5 = new uint[totalPortCount];
		std::vector<uint> array5 (totalPortCount,0);
		//WLAN_P_TRANSMIT_POWER_CLASS[] array6 = new WLAN_P_TRANSMIT_POWER_CLASS[totalPortCount];
		std::vector<uint> array6 (totalPortCount,vPowerClass11p[0]);
		uint num = 0u;
		uint num2 = 0u;
		uint num3 = 1u;
		int num4 = 0;
		int num5 = 0;

		for (size_t i = 0; i < totalPortCount; i++)
		{
			array3[i] = vPowerLevelDbm[i] + vsaRefLevelHeadspaceDbr[i];
			array4[i] = min(_vsaTriggerLevelMaxDbm,(vPowerLevelDbm[i] + vVsaTriggerLevelOffsetDbr[i]));
			//Math.Min(_vsaTriggerLevelMaxDbm, powerLevelDbm[i] + vsaTriggerLevelOffsetDbr[i]);
			//! check 
			CheckMaskVsStreamCountError(dutPortMask,vWlanRate[i],strWifiRate);

			if (std::string::npos != strWifiRate.find("80P80"))
			{
				if (vPortBonding[i] == 0)
				{
					PELOG4WW_F(L"ConfigureVsaWlanSingleUserMeasurement() failed: the 80+80 rate must specify the port-bonding for port-mask %d",(num3 << i));
					return ;
				}

				if (MaskActiveCount((num3 << i) | vPortBonding[i]) != 2)
				{
					//! throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: the 80+80 rate must use TWO bonded ports; port-mask " + (num3 << i) + " port-bonding (" + ((num3 << i) | portBonding[i]) + ") is NOT two bonded ports");
					PELOG4WW_F(L"ConfigureVsaWlanSingleUserMeasurement() failed: the 80+80 rate must use TWO bonded ports; port-mask %d  port-bonding (%d) is NOT two bonded ports",(num3 << i),((num3 << i) | vPortBonding[i]));
					return ;
				}
				if ((dutPortMask & vPortBonding[i]) != vPortBonding[i])
				{
					//! throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: the 80+80 rate portBonding (" + portBonding[i] + ") is NOT a subset of port mask (" + dutPortMask + ")");
					PELOG4WW_F(L"ConfigureVsaWlanSingleUserMeasurement() failed: the 80+80 rate portBonding (%d) is NOT a subset of port mask (%d)",vPortBonding[i],dutPortMask);
					return ;
				}
				num |= num3 << i;
				array5[num4++] |= ((num3 << i) | vPortBonding[i]);
			}

			if (std::string::npos != strWifiRate.find("11P_"))
			{
				if (vPowerClass11p.size() <= i)
				{
					//throw new WCNCustomException("ConfigureVsaWlanSingleUserMeasurement() failed: the 11P rate must specify the power-mask-class for port-mask " + (num3 << i));
					PELOG4WW_F(L"ConfigureVsaWlanSingleUserMeasurement() failed: the 11P rate must specify the power-mask-class for port-mask %d",(num3 << i));
					return;
				}
				num2 |= num3 << i;
				array6[num5++] = vPowerClass11p[i];
			}
		}

		std::string command;

		command = ":QCOM:WLAN:SENS:PAR ";
		AppendBitMaskWaveformParameters(command,dutPortMask,vWlanRate);
		mpEqipCommtl->write(command);

		if (num != 0)
		{
			command = ":QCOM:WLAN:SENS:PBON ";
			AppendActiveMaskParameters(command, num, array5);
			mpEqipCommtl->write(command);
		}

		/*if (num2 != 0)
		{
		command = ":QCOM:WLAN:SENS:SMAS:CLAS ";
		AppendActiveMaskParameters(command, num2, GetSmaskPowerClassStrings(array6));  
		}*/

		command = ":QCOM:WLAN:SENS:FREQ ";
		__int64 chananel = static_cast<__int64>(vVhannelFreqHz[0] * 1000000.0);
		std::string strFreqHz;
		JGW_FormatString(strFreqHz,"%I64d",chananel);
		//AppendActiveMaskParameters(command, num, strFreqHz);
		AppendBitMaskParameters(command, dutPortMask, strFreqHz);
		//AppendActiveMaskParameters(command, dutPortMask,vVhannelFreqHz);
		mpEqipCommtl->write(command);

		command = ":QCOM:WLAN:SENS:POW:REF ";
		AppendActiveMaskParameters(command, dutPortMask, array3);
		mpEqipCommtl->write(command);

		command = ":QCOM:WLAN:SENS:TRIG:LEV ";
		AppendActiveMaskParameters(command, dutPortMask, array4);
		mpEqipCommtl->write(command);

		command = ":QCOM:WLAN:SENS:AVER:COUN ";
		AppendActiveMaskParameters(command, dutPortMask, vVsaAverages);
		mpEqipCommtl->write(command);

		ClearErrorQueue();
	}

	int WCNTester_CommonSCPIWLAN::MaskActiveCount(uint portMask, size_t& totalPortCount)
	{
		uint num = 0;
		totalPortCount = 0;
		while (portMask >> totalPortCount != 0)
		{
			num += ((portMask >> totalPortCount) & 1);
			totalPortCount++;
		}
		return (int)num;
	}

	void WCNTester_CommonSCPIWLAN::AppendBitMaskWaveformParameters(std::string& commands, uint portMask, const std::vector<WLAN_RATE>& vWlanRate)
	{
		commands += JGW_GetFormatString("%u",portMask);
		if (vWlanRate.size() == 1)
		{
			int streamCount;
			std::string waveformParameters = GetWaveformParameters(vWlanRate[0], streamCount);
			do
			{
				if ((portMask & 1) != 0)
				{
					commands += JGW_GetFormatString(",'%s',%d",waveformParameters.c_str(),streamCount);
					/*commands.Append(",'");
					commands.Append(waveformParameters);
					commands.Append("',");
					commands.Append(streamCount);*/
				}
			}
			while ((portMask >>= 1) != 0);
		}
		else
		{

			int num = 0;
			int streamCount2;
			do
			{
				if ((portMask & 1) != 0)
				{
					std::string waveformParameters = GetWaveformParameters(vWlanRate[num], streamCount2);
					commands += JGW_GetFormatString(",'%s',%d",waveformParameters.c_str(),streamCount2);
					/*commands.Append(",'");
					commands.Append(GetWaveformParameters(rate[num], out int streamCount2));
					commands.Append("',");
					commands.Append(streamCount2);*/
				}
				num++;
			}
			while ((portMask >>= 1) != 0);
		}
	}

	std::string WCNTester_CommonSCPIWLAN::GetWaveformParameters(WLAN_RATE rate, int& streamCount)
	{
		for (size_t i = 0;i < _countof(WaveformParameters);i ++)
		{
			if (WaveformParameters[i].meWlanRate == rate)
			{
				streamCount = WaveformParameters[i].msWaveformParameter.StreamCount;
				return WaveformParameters[i].msWaveformParameter.WifiRate;
			}
		}
		return "";
	}

	bool WCNTester_CommonSCPIWLAN::CheckMaskVsStreamCountError(uint dutPortMask, WLAN_RATE rate,std::string& strWifiRate)
	{
		size_t totalPortCount = 0;
		MaskActiveCount(dutPortMask,totalPortCount);
		for (size_t i = 0;i < _countof(WaveformParameters);i ++)
		{
			if (WaveformParameters[i].meWlanRate == rate)
			{
				strWifiRate = WaveformParameters[i].msWaveformParameter.WifiRate;
				if (totalPortCount < (uint)WaveformParameters[i].msWaveformParameter.StreamCount)
				{
					//throw new WCNCustomException("CheckMaskVsStreamCountError() failed: The Port Mask (" + dutPortMask + ") count of " + MaskActiveCount(dutPortMask) + " is less than the Stream count of " + GetRateStreamCount(rate) + ", for the waveform '" + rate.ToString() + "'");
					return false;
				}
				else
				{
					return true;
				}
			}
		}
		return false;
	}

	int WCNTester_CommonSCPIWLAN::MaskActiveCount(uint portMask)
	{
		uint num = portMask & 1;
		while ((portMask >>= 1) != 0)
		{
			num += (portMask & 1);
		}
		return (int)num;
	}

	void WCNTester_CommonSCPIWLAN::AppendActiveMaskParameters(std::string& commands, uint portMask, const std::vector<uint>& values)
	{
		commands += JGW_GetFormatString("%u",portMask);
		if (values.size() == 1)
		{
			do
			{
				if ((portMask & 1) != 0)
				{
					commands += JGW_GetFormatString(",%u",values[0]);
				}
			}
			while ((portMask >>= 1) != 0);
		}
		else
		{
			int num = 0;
			do
			{
				if ((portMask & 1) != 0)
				{
					commands += JGW_GetFormatString(",%u",values[num++]);
				}
			}
			while ((portMask >>= 1) != 0);
		}
	}

	void WCNTester_CommonSCPIWLAN::AppendActiveMaskParameters(std::string& commands, uint portMask, const std::vector<double>& values)
	{
		commands += JGW_GetFormatString("%u",portMask);
		if (values.size() == 1)
		{
			do
			{
				if ((portMask & 1) != 0)
				{
					commands += JGW_GetFormatString(",%f",values[0]);
				}
			}
			while ((portMask >>= 1) != 0);
		}
		else
		{
			int num = 0;
			do
			{
				if ((portMask & 1) != 0)
				{
					commands += JGW_GetFormatString(",%f",values[num++]);
				}
			}
			while ((portMask >>= 1) != 0);
		}
	}

	bool WCNTester_CommonSCPIWLAN::InitiateFetchVsaData()
	{
		std::string command;

		command = ":QCOM:WLAN:SENS:INIT";
		mpEqipCommtl->write(command);

		command = ":QCOM:WLAN:FETC";
		//! mpEqipCommtl->write(command);
		return BlockReturnUntilOperationCompleted(command);
	}

	void WCNTester_CommonSCPIWLAN::GetWlanPowerAvg(double* avgTxPowerDbm, std::vector<double>& avgTxPowerMinDbm, std::vector<double>& avgTxPowerMaxDbm, std::vector<std::string>& wlanType)
	{
		std::vector<double> vAvgTxPower;
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER?",vAvgTxPower);
		for (size_t i = 0;i < vAvgTxPower.size();i ++)
		{
			avgTxPowerDbm[i] = vAvgTxPower[i];
		}

		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER:MIN?",avgTxPowerMinDbm);
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER:MAX?",avgTxPowerMaxDbm);
	}

	void WCNTester_CommonSCPIWLAN::GetWlanPowerPeak(double* pkTxPowerDbm, std::vector<double>& pkTxPowerMinDbm, std::vector<double>& pkTxPowerMaxDbm, std::vector<std::string>& wlanType)
	{
		std::vector<double> vPkTxPower;
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:PEAK?",vPkTxPower);
		for (size_t i = 0;i < vPkTxPower.size();i ++)
		{
			pkTxPowerDbm[i] = vPkTxPower[i];
		}
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:PEAK:MIN?",pkTxPowerMinDbm);
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:PEAK:MAX?",pkTxPowerMaxDbm);
	}

	void WCNTester_CommonSCPIWLAN::SetVsgModulationPortMask(uint dutPortMask, WLAN_RATE rate, std::vector<uint>& portBonding)
	{
		int streamCount = 0;
		std::string strRateName,command;

		if (!CheckMaskVsStreamCountError(dutPortMask,rate,strRateName))
		{
			return ;
		}

		if (std::string::npos != strRateName.find("80P80") && (portBonding.size() == 0))
		{
			//throw new WCNCustomException("SetVsgModulationPortMask() failed: portBonding array size = zero (0)");
			PELOG4WW(L"SetVsgModulationPortMask() failed: portBonding array size = zero (0)");
			return ;
		}

		size_t num = MaskActiveCount(dutPortMask);
		while (num > portBonding.size())
		{
			portBonding.push_back(portBonding[0]);
		}

		std::vector<uint> array(num,0);
		uint num2 = 0u;
		uint num3 = 1u;
		size_t i = 0;
		int num4 = 0;
		for (; i < num; i++)
		{
			if (((dutPortMask >> i) & 1) != 0 && std::string::npos != strRateName.find("80P80"))
			{
				if (portBonding[i] == 0)
				{
					//throw new WCNCustomException("SetVsgModulationPortMask() failed: the 80+80 rate must specify the port-bonding for port-mask " + (num3 << i));
					PELOG4WW_F(L"SetVsgModulationPortMask() failed: the 80+80 rate must specify the port-bonding for port-mask %d",(num3 << i));
					return ;
				}
				if (MaskActiveCount((num3 << i) | portBonding[i]) != 2)
				{
					//throw new WCNCustomException("SetVsgModulationPortMask() failed: the 80+80 rate must use TWO bonded ports; port-mask " + (num3 << i) + " port-bonding (" + ((num3 << i) | portBonding[i]) + ") is NOT two bonded ports");
					PELOG4WW_F(L"SetVsgModulationPortMask() failed: the 80+80 rate must use TWO bonded ports; port-mask %d port-bonding (%d) is NOT two bonded ports",(num3 << i),((num3 << i) | portBonding[i]));
					return ;
				}
				if ((dutPortMask & portBonding[i]) != portBonding[i])
				{
					//throw new WCNCustomException("SetVsgModulationPortMask() failed: the 80+80 rate portBonding (" + portBonding[i] + ") is NOT a subset of port mask (" + dutPortMask + ")");
					PELOG4WW_F(L"SetVsgModulationPortMask() failed: the 80+80 rate portBonding (%u) is NOT a subset of port mask (%u)",portBonding[i],dutPortMask);
					return ;
				}
				num2 |= num3 << i;
				array[num4++] |= ((num3 << i) | portBonding[i]);
			}
		}

		SetVsgPortMask(dutPortMask);

		command = ":QCOM:WLAN:SOUR:ARB:LOAD ";
		JGW_FormatString(command,":QCOM:WLAN:SOUR:ARB:LOAD %d,'%s',%d",dutPortMask,strRateName.c_str(),streamCount);
		mpEqipCommtl->write(command);

		if (num2 != 0)
		{
			command = ":QCOM:WLAN:SOUR:PBON ";
			AppendActiveMaskParameters(command, num2, array);
			mpEqipCommtl->write(command);
		}
	}

	void WCNTester_CommonSCPIWLAN::AppendBitMaskParameters(std::string& commands, uint portMask, const std::string& value)
	{
		commands += JGW_GetFormatString("%u",portMask);
		do
		{
			if ((portMask & 1) != 0)
			{
				commands += JGW_GetFormatString(",%s",value.c_str());
			}
		}
		while ((portMask >>= 1) != 0);
	}

	void WCNTester_CommonSCPIWLAN::AppendBitMaskStateMask(std::string& commands, uint bitMask, uint statePortMask)
	{
		int num = 0;
		commands += JGW_GetFormatString("%u",bitMask);

		do
		{
			if (((bitMask >> num) & 1) != 0)
			{
				commands += ((((statePortMask >> num) & 1) == 0) ? ",0" : ",1");
			}
		}
		while (bitMask >> ++num != 0);
		return;
	}

	void WCNTester_CommonSCPIWLAN::VsgEnableRfPortMask(uint dutPortMask, bool rfEnable, int numOfFramesZeroForContinuous)
	{
		std::string command;
		if (rfEnable)
		{
			if (numOfFramesZeroForContinuous == 0)
			{
				command = ":QCOM:WLAN:SOUR:TRIG:TYPE ";
				AppendBitMaskParameters(command, dutPortMask, "CONT");
				mpEqipCommtl->write(command);

				command = ":QCOM:WLAN:SOUR:ARB:STAT ";
				AppendBitMaskStateMask(command, dutPortMask, dutPortMask);
				mpEqipCommtl->write(command);

				command = ":QCOM:WLAN:SOUR:STAT ";
				AppendBitMaskStateMask(command, dutPortMask, dutPortMask);
				mpEqipCommtl->write(command);
			}
			else
			{
				command = ":QCOM:WLAN:SOUR:TRIG:TYPE ";
				AppendBitMaskParameters(command, dutPortMask, "SING");
				mpEqipCommtl->write(command);

				command = ":QCOM:WLAN:SOUR:SWE:COUN ";
				AppendActiveMaskParameters(command, dutPortMask, numOfFramesZeroForContinuous);
				mpEqipCommtl->write(command);


				command = ":QCOM:WLAN:SOUR:ARB:STAT ";
				AppendBitMaskStateMask(command, dutPortMask, dutPortMask);
				mpEqipCommtl->write(command);

				command = ":QCOM:WLAN:SOUR:STAT ";
				AppendBitMaskParameters(command, dutPortMask, 0);
				mpEqipCommtl->write(command);
			}
			command = ":QCOM:WLAN:SOUR:INIT";
			mpEqipCommtl->write(command);
		}
		else
		{
			command = ":QCOM:WLAN:SOUR:TRIG:TYPE ";
			AppendBitMaskParameters(command, dutPortMask, "CONT");
			mpEqipCommtl->write(command);

			command = ":QCOM:WLAN:SOUR:ARB:STAT ";
			AppendBitMaskParameters(command, dutPortMask, 0);
			mpEqipCommtl->write(command);

			command = ":QCOM:WLAN:SOUR:STAT ";
			AppendBitMaskParameters(command, dutPortMask, 0);
			mpEqipCommtl->write(command);
		}
	}

	void WCNTester_CommonSCPIWLAN::VsgSetPortMask(uint dutPortMask, double frequencyMHz, double signalLeveldBm)
	{
		std::string command;
		std::vector<double> vChannelFreqHz(1,frequencyMHz * 1000000.0);
		std::vector<double> signalLeveldBm2(1,signalLeveldBm);


		command = ":QCOM:WLAN:SOUR:FREQ ";
		AppendActiveMaskParameters(command, dutPortMask, vChannelFreqHz);
		mpEqipCommtl->write(command);

		command = ":QCOM:WLAN:SOUR:POW ";
		AppendActiveMaskParameters(command, dutPortMask, signalLeveldBm2);
		mpEqipCommtl->write(command);
	}



	void WCNTester_CommonSCPIWLAN::mapWlanRate(WLAN_DataRate wlanRate, bool short11b_nGuard,WLAN_RATE& rate,WLAN_Gen6_PHYDBG_PreambleRate& preambleType, uint channel2 /*= 0u*/)
	{
		bool flag = (channel2 != 0) ? true : false;
		switch (wlanRate)
		{
		case RATE_1Mbps:
		case RATE_2Mbps:
		case RATE_5_5Mbps:
		case RATE_11Mbps:
			if (short11b_nGuard)
			{
				preambleType = PREAMBLE_SHORTB_11B;
			}
			else
			{
				preambleType = PREAMBLE_LONGB_11B;
			}
			break;
		case RATE_6Mbps:
		case RATE_9Mbps:
		case RATE_12Mbps:
		case RATE_18Mbps:
		case RATE_24Mbps:
		case RATE_36Mbps:
		case RATE_48Mbps:
		case RATE_54Mbps:
			preambleType = PREAMBLE_OFDM_11G;
			break;
		default:
			preambleType = PREAMBLE_MIXED_11N;
			break;
		}
		switch (wlanRate)
		{
		case RATE_1Mbps:
			rate = RATE_11B_LONG_1_MBPS;
			break;
		case RATE_2Mbps:
			if (short11b_nGuard)
			{
				rate = RATE_11B_SHORT_2_MBPS;
			}
			else
			{
				rate = RATE_11B_LONG_2_MBPS;
			}
			break;
		case RATE_5_5Mbps:
			if (short11b_nGuard)
			{
				rate = RATE_11B_SHORT_5_5_MBPS;
			}
			else
			{
				rate = RATE_11B_LONG_5_5_MBPS;
			}
			break;
		case RATE_11Mbps:
			if (short11b_nGuard)
			{
				rate = RATE_11B_SHORT_11_MBPS;
			}
			else
			{
				rate = RATE_11B_LONG_11_MBPS;
			}
			break;
		case RATE_6Mbps:
			rate = RATE_11A_6_MBPS;
			break;
		case RATE_9Mbps:
			rate = RATE_11A_9_MBPS;
			break;
		case RATE_12Mbps:
			rate = RATE_11A_12_MBPS;
			break;
		case RATE_18Mbps:
			rate = RATE_11A_18_MBPS;
			break;
		case RATE_24Mbps:
			rate = RATE_11A_24_MBPS;
			break;
		case RATE_36Mbps:
			rate = RATE_11A_36_MBPS;
			break;
		case RATE_48Mbps:
			rate = RATE_11A_48_MBPS;
			break;
		case RATE_54Mbps:
			rate = RATE_11A_54_MBPS;
			break;
		case RATE_MCS_0_20:
			rate = RATE_11N_HT20_MCS0;
			break;
		case RATE_MCS_1_20:
			rate = RATE_11N_HT20_MCS1;
			break;
		case RATE_MCS_2_20:
			rate = RATE_11N_HT20_MCS2;
			break;
		case RATE_MCS_3_20:
			rate = RATE_11N_HT20_MCS3;
			break;
		case RATE_MCS_4_20:
			rate = RATE_11N_HT20_MCS4;
			break;
		case RATE_MCS_5_20:
			rate = RATE_11N_HT20_MCS5;
			break;
		case RATE_MCS_6_20:
			rate = RATE_11N_HT20_MCS6;
			break;
		case RATE_MCS_7_20:
			rate = RATE_11N_HT20_MCS7;
			break;
		case RATE_MCS_8_20:
			rate = RATE_11N_HT20_MCS8;
			break;
		case RATE_MCS_9_20:
			rate = RATE_11N_HT20_MCS9;
			break;
		case RATE_MCS_10_20:
			rate = RATE_11N_HT20_MCS10;
			break;
		case RATE_MCS_11_20:
			rate = RATE_11N_HT20_MCS11;
			break;
		case RATE_MCS_12_20:
			rate = RATE_11N_HT20_MCS12;
			break;
		case RATE_MCS_13_20:
			rate = RATE_11N_HT20_MCS13;
			break;
		case RATE_MCS_14_20:
			rate = RATE_11N_HT20_MCS14;
			break;
		case RATE_MCS_15_20:
			rate = RATE_11N_HT20_MCS15;
			break;
		case RATE_MCS_16_20:
			rate = RATE_11N_HT20_MCS16;
			break;
		case RATE_MCS_17_20:
			rate = RATE_11N_HT20_MCS17;
			break;
		case RATE_MCS_18_20:
			rate = RATE_11N_HT20_MCS18;
			break;
		case RATE_MCS_19_20:
			rate = RATE_11N_HT20_MCS19;
			break;
		case RATE_MCS_20_20:
			rate = RATE_11N_HT20_MCS20;
			break;
		case RATE_MCS_21_20:
			rate = RATE_11N_HT20_MCS21;
			break;
		case RATE_MCS_22_20:
			rate = RATE_11N_HT20_MCS22;
			break;
		case RATE_MCS_23_20:
			rate = RATE_11N_HT20_MCS23;
			break;
		case RATE_MCS_0_40:
			rate = RATE_11N_HT40_MCS0;
			break;
		case RATE_MCS_1_40:
			rate = RATE_11N_HT40_MCS1;
			break;
		case RATE_MCS_2_40:
			rate = RATE_11N_HT40_MCS2;
			break;
		case RATE_MCS_3_40:
			rate = RATE_11N_HT40_MCS3;
			break;
		case RATE_MCS_4_40:
			rate = RATE_11N_HT40_MCS4;
			break;
		case RATE_MCS_5_40:
			rate = RATE_11N_HT40_MCS5;
			break;
		case RATE_MCS_6_40:
			rate = RATE_11N_HT40_MCS6;
			break;
		case RATE_MCS_7_40:
			rate = RATE_11N_HT40_MCS7;
			break;
		case RATE_MCS_8_40:
			rate = RATE_11N_HT40_MCS8;
			break;
		case RATE_MCS_9_40:
			rate = RATE_11N_HT40_MCS9;
			break;
		case RATE_MCS_10_40:
			rate = RATE_11N_HT40_MCS10;
			break;
		case RATE_MCS_11_40:
			rate = RATE_11N_HT40_MCS11;
			break;
		case RATE_MCS_12_40:
			rate = RATE_11N_HT40_MCS12;
			break;
		case RATE_MCS_13_40:
			rate = RATE_11N_HT40_MCS13;
			break;
		case RATE_MCS_14_40:
			rate = RATE_11N_HT40_MCS14;
			break;
		case RATE_MCS_15_40:
			rate = RATE_11N_HT40_MCS15;
			break;
		case RATE_MCS_16_40:
			rate = RATE_11N_HT40_MCS16;
			break;
		case RATE_MCS_17_40:
			rate = RATE_11N_HT40_MCS17;
			break;
		case RATE_MCS_18_40:
			rate = RATE_11N_HT40_MCS18;
			break;
		case RATE_MCS_19_40:
			rate = RATE_11N_HT40_MCS19;
			break;
		case RATE_MCS_20_40:
			rate = RATE_11N_HT40_MCS20;
			break;
		case RATE_MCS_21_40:
			rate = RATE_11N_HT40_MCS21;
			break;
		case RATE_MCS_22_40:
			rate = RATE_11N_HT40_MCS22;
			break;
		case RATE_MCS_23_40:
			rate = RATE_11N_HT40_MCS23;
			break;
		case RATE_AC_MCS_0_20:
			rate = RATE_11AC_HT20_MCS0;
			break;
		case RATE_AC_MCS_1_20:
			rate = RATE_11AC_HT20_MCS1;
			break;
		case RATE_AC_MCS_2_20:
			rate = RATE_11AC_HT20_MCS2;
			break;
		case RATE_AC_MCS_3_20:
			rate = RATE_11AC_HT20_MCS3;
			break;
		case RATE_AC_MCS_4_20:
			rate = RATE_11AC_HT20_MCS4;
			break;
		case RATE_AC_MCS_5_20:
			rate = RATE_11AC_HT20_MCS5;
			break;
		case RATE_AC_MCS_6_20:
			rate = RATE_11AC_HT20_MCS6;
			break;
		case RATE_AC_MCS_7_20:
			rate = RATE_11AC_HT20_MCS7;
			break;
		case RATE_AC_MCS_8_20:
			rate = RATE_11AC_HT20_MCS8;
			break;
		case RATE_AC_MCS_9_20:
			rate = RATE_11AC_HT20_MCS9;
			break;
		case RATE_AC_MCS_10_20:
			rate = RATE_11AC_HT20_MCS10;
			break;
		case RATE_AC_MCS_11_20:
			rate = RATE_11AC_HT20_MCS11;
			break;
		case RATE_AC_MCS_12_20:
			rate = RATE_11AC_HT20_MCS12;
			break;
		case RATE_AC_MCS_13_20:
			rate = RATE_11AC_HT20_MCS13;
			break;
		case RATE_AC_MCS_14_20:
			rate = RATE_11AC_HT20_MCS14;
			break;
		case RATE_AC_MCS_15_20:
			rate = RATE_11AC_HT20_MCS15;
			break;
		case RATE_AC_MCS_16_20:
			rate = RATE_11AC_HT20_MCS16;
			break;
		case RATE_AC_MCS_17_20:
			rate = RATE_11AC_HT20_MCS17;
			break;
		case RATE_AC_MCS_18_20:
			rate = RATE_11AC_HT20_MCS18;
			break;
		case RATE_AC_MCS_19_20:
			rate = RATE_11AC_HT20_MCS19;
			break;
		case RATE_AC_MCS_20_20:
			rate = RATE_11AC_HT20_MCS20;
			break;
		case RATE_AC_MCS_21_20:
			rate = RATE_11AC_HT20_MCS21;
			break;
		case RATE_AC_MCS_22_20:
			rate = RATE_11AC_HT20_MCS22;
			break;
		case RATE_AC_MCS_23_20:
			rate = RATE_11AC_HT20_MCS23;
			break;
		case RATE_AC_MCS_24_20:
			rate = RATE_11AC_HT20_MCS24;
			break;
		case RATE_AC_MCS_25_20:
			rate = RATE_11AC_HT20_MCS25;
			break;
		case RATE_AC_MCS_26_20:
			rate = RATE_11AC_HT20_MCS26;
			break;
		case RATE_AC_MCS_27_20:
			rate = RATE_11AC_HT20_MCS27;
			break;
		case RATE_AC_MCS_28_20:
			rate = RATE_11AC_HT20_MCS28;
			break;
		case RATE_AC_MCS_29_20:
			rate = RATE_11AC_HT20_MCS29;
			break;
		case RATE_AC_MCS_30_20:
			rate = RATE_11AC_HT20_MCS30;
			break;
		case RATE_AC_MCS_31_20:
			rate = RATE_11AC_HT20_MCS31;
			break;
		case RATE_AC_MCS_32_20:
			rate = RATE_11AC_HT20_MCS32;
			break;
		case RATE_AC_MCS_33_20:
			rate = RATE_11AC_HT20_MCS33;
			break;
		case RATE_AC_MCS_34_20:
			rate = RATE_11AC_HT20_MCS34;
			break;
		case RATE_AC_MCS_35_20:
			rate = RATE_11AC_HT20_MCS35;
			break;
		case RATE_AC_MCS_36_20:
			rate = RATE_11AC_HT20_MCS36;
			break;
		case RATE_AC_MCS_37_20:
			rate = RATE_11AC_HT20_MCS37;
			break;
		case RATE_AC_MCS_38_20:
			rate = RATE_11AC_HT20_MCS38;
			break;
		case RATE_AC_MCS_39_20:
			rate = RATE_11AC_HT20_MCS39;
			break;
		case RATE_AC_MCS_0_40:
			rate = RATE_11AC_HT40_MCS0;
			break;
		case RATE_AC_MCS_1_40:
			rate = RATE_11AC_HT40_MCS1;
			break;
		case RATE_AC_MCS_2_40:
			rate = RATE_11AC_HT40_MCS2;
			break;
		case RATE_AC_MCS_3_40:
			rate = RATE_11AC_HT40_MCS3;
			break;
		case RATE_AC_MCS_4_40:
			rate = RATE_11AC_HT40_MCS4;
			break;
		case RATE_AC_MCS_5_40:
			rate = RATE_11AC_HT40_MCS5;
			break;
		case RATE_AC_MCS_6_40:
			rate = RATE_11AC_HT40_MCS6;
			break;
		case RATE_AC_MCS_7_40:
			rate = RATE_11AC_HT40_MCS7;
			break;
		case RATE_AC_MCS_8_40:
			rate = RATE_11AC_HT40_MCS8;
			break;
		case RATE_AC_MCS_9_40:
			rate = RATE_11AC_HT40_MCS9;
			break;
		case RATE_AC_MCS_10_40:
			rate = RATE_11AC_HT40_MCS10;
			break;
		case RATE_AC_MCS_11_40:
			rate = RATE_11AC_HT40_MCS11;
			break;
		case RATE_AC_MCS_12_40:
			rate = RATE_11AC_HT40_MCS12;
			break;
		case RATE_AC_MCS_13_40:
			rate = RATE_11AC_HT40_MCS13;
			break;
		case RATE_AC_MCS_14_40:
			rate = RATE_11AC_HT40_MCS14;
			break;
		case RATE_AC_MCS_15_40:
			rate = RATE_11AC_HT40_MCS15;
			break;
		case RATE_AC_MCS_16_40:
			rate = RATE_11AC_HT40_MCS16;
			break;
		case RATE_AC_MCS_17_40:
			rate = RATE_11AC_HT40_MCS17;
			break;
		case RATE_AC_MCS_18_40:
			rate = RATE_11AC_HT40_MCS18;
			break;
		case RATE_AC_MCS_19_40:
			rate = RATE_11AC_HT40_MCS19;
			break;
		case RATE_AC_MCS_20_40:
			rate = RATE_11AC_HT40_MCS20;
			break;
		case RATE_AC_MCS_21_40:
			rate = RATE_11AC_HT40_MCS21;
			break;
		case RATE_AC_MCS_22_40:
			rate = RATE_11AC_HT40_MCS22;
			break;
		case RATE_AC_MCS_23_40:
			rate = RATE_11AC_HT40_MCS23;
			break;
		case RATE_AC_MCS_24_40:
			rate = RATE_11AC_HT40_MCS24;
			break;
		case RATE_AC_MCS_25_40:
			rate = RATE_11AC_HT40_MCS25;
			break;
		case RATE_AC_MCS_26_40:
			rate = RATE_11AC_HT40_MCS26;
			break;
		case RATE_AC_MCS_27_40:
			rate = RATE_11AC_HT40_MCS27;
			break;
		case RATE_AC_MCS_28_40:
			rate = RATE_11AC_HT40_MCS28;
			break;
		case RATE_AC_MCS_29_40:
			rate = RATE_11AC_HT40_MCS29;
			break;
		case RATE_AC_MCS_30_40:
			rate = RATE_11AC_HT40_MCS30;
			break;
		case RATE_AC_MCS_31_40:
			rate = RATE_11AC_HT40_MCS31;
			break;
		case RATE_AC_MCS_32_40:
			rate = RATE_11AC_HT40_MCS32;
			break;
		case RATE_AC_MCS_33_40:
			rate = RATE_11AC_HT40_MCS33;
			break;
		case RATE_AC_MCS_34_40:
			rate = RATE_11AC_HT40_MCS34;
			break;
		case RATE_AC_MCS_35_40:
			rate = RATE_11AC_HT40_MCS35;
			break;
		case RATE_AC_MCS_36_40:
			rate = RATE_11AC_HT40_MCS36;
			break;
		case RATE_AC_MCS_37_40:
			rate = RATE_11AC_HT40_MCS37;
			break;
		case RATE_AC_MCS_38_40:
			rate = RATE_11AC_HT40_MCS38;
			break;
		case RATE_AC_MCS_39_40:
			rate = RATE_11AC_HT40_MCS39;
			break;
		case RATE_AC_MCS_0_80:
			rate = RATE_11AC_HT80_MCS0;
			break;
		case RATE_AC_MCS_1_80:
			rate = RATE_11AC_HT80_MCS1;
			break;
		case RATE_AC_MCS_2_80:
			rate = RATE_11AC_HT80_MCS2;
			break;
		case RATE_AC_MCS_3_80:
			rate = RATE_11AC_HT80_MCS3;
			break;
		case RATE_AC_MCS_4_80:
			rate = RATE_11AC_HT80_MCS4;
			break;
		case RATE_AC_MCS_5_80:
			rate = RATE_11AC_HT80_MCS5;
			break;
		case RATE_AC_MCS_6_80:
			rate = RATE_11AC_HT80_MCS6;
			break;
		case RATE_AC_MCS_7_80:
			rate = RATE_11AC_HT80_MCS7;
			break;
		case RATE_AC_MCS_8_80:
			rate = RATE_11AC_HT80_MCS8;
			break;
		case RATE_AC_MCS_9_80:
			rate = RATE_11AC_HT80_MCS9;
			break;
		case RATE_AC_MCS_10_80:
			rate = RATE_11AC_HT80_MCS10;
			break;
		case RATE_AC_MCS_11_80:
			rate = RATE_11AC_HT80_MCS11;
			break;
		case RATE_AC_MCS_12_80:
			rate = RATE_11AC_HT80_MCS12;
			break;
		case RATE_AC_MCS_13_80:
			rate = RATE_11AC_HT80_MCS13;
			break;
		case RATE_AC_MCS_14_80:
			rate = RATE_11AC_HT80_MCS14;
			break;
		case RATE_AC_MCS_15_80:
			rate = RATE_11AC_HT80_MCS15;
			break;
		case RATE_AC_MCS_16_80:
			rate = RATE_11AC_HT80_MCS16;
			break;
		case RATE_AC_MCS_17_80:
			rate = RATE_11AC_HT80_MCS17;
			break;
		case RATE_AC_MCS_18_80:
			rate = RATE_11AC_HT80_MCS18;
			break;
		case RATE_AC_MCS_19_80:
			rate = RATE_11AC_HT80_MCS19;
			break;
		case RATE_AC_MCS_20_80:
			rate = RATE_11AC_HT80_MCS20;
			break;
		case RATE_AC_MCS_21_80:
			rate = RATE_11AC_HT80_MCS21;
			break;
		case RATE_AC_MCS_22_80:
			rate = RATE_11AC_HT80_MCS22;
			break;
		case RATE_AC_MCS_23_80:
			rate = RATE_11AC_HT80_MCS23;
			break;
		case RATE_AC_MCS_24_80:
			rate = RATE_11AC_HT80_MCS24;
			break;
		case RATE_AC_MCS_25_80:
			rate = RATE_11AC_HT80_MCS25;
			break;
		case RATE_AC_MCS_26_80:
			rate = RATE_11AC_HT80_MCS26;
			break;
		case RATE_AC_MCS_27_80:
			rate = RATE_11AC_HT80_MCS27;
			break;
		case RATE_AC_MCS_28_80:
			rate = RATE_11AC_HT80_MCS28;
			break;
		case RATE_AC_MCS_29_80:
			rate = RATE_11AC_HT80_MCS29;
			break;
		case RATE_AC_MCS_30_80:
			rate = RATE_11AC_HT80_MCS30;
			break;
		case RATE_AC_MCS_31_80:
			rate = RATE_11AC_HT80_MCS31;
			break;
		case RATE_AC_MCS_32_80:
			rate = RATE_11AC_HT80_MCS32;
			break;
		case RATE_AC_MCS_33_80:
			rate = RATE_11AC_HT80_MCS33;
			break;
		case RATE_AC_MCS_34_80:
			rate = RATE_11AC_HT80_MCS34;
			break;
		case RATE_AC_MCS_35_80:
			rate = RATE_11AC_HT80_MCS35;
			break;
		case RATE_AC_MCS_36_80:
			rate = RATE_11AC_HT80_MCS36;
			break;
		case RATE_AC_MCS_37_80:
			rate = RATE_11AC_HT80_MCS37;
			break;
		case RATE_AC_MCS_38_80:
			rate = RATE_11AC_HT80_MCS38;
			break;
		case RATE_AC_MCS_39_80:
			rate = RATE_11AC_HT80_MCS39;
			break;
		case RATE_AC_MCS_0_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS0 : RATE_11AC_HT160_MCS0);
			break;
		case RATE_AC_MCS_1_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS1 : RATE_11AC_HT160_MCS1);
			break;
		case RATE_AC_MCS_2_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS2 : RATE_11AC_HT160_MCS2);
			break;
		case RATE_AC_MCS_3_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS3 : RATE_11AC_HT160_MCS3);
			break;
		case RATE_AC_MCS_4_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS4 : RATE_11AC_HT160_MCS4);
			break;
		case RATE_AC_MCS_5_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS5 : RATE_11AC_HT160_MCS5);
			break;
		case RATE_AC_MCS_6_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS6 : RATE_11AC_HT160_MCS6);
			break;
		case RATE_AC_MCS_7_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS7 : RATE_11AC_HT160_MCS7);
			break;
		case RATE_AC_MCS_8_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS8 : RATE_11AC_HT160_MCS8);
			break;
		case RATE_AC_MCS_9_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS9 : RATE_11AC_HT160_MCS9);
			break;
		case RATE_AC_MCS_10_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS10 : RATE_11AC_HT160_MCS10);
			break;
		case RATE_AC_MCS_11_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS11 : RATE_11AC_HT160_MCS11);
			break;
		case RATE_AC_MCS_12_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS12 : RATE_11AC_HT160_MCS12);
			break;
		case RATE_AC_MCS_13_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS13 : RATE_11AC_HT160_MCS13);
			break;
		case RATE_AC_MCS_14_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS14 : RATE_11AC_HT160_MCS14);
			break;
		case RATE_AC_MCS_15_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS15 : RATE_11AC_HT160_MCS15);
			break;
		case RATE_AC_MCS_16_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS16 : RATE_11AC_HT160_MCS16);
			break;
		case RATE_AC_MCS_17_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS17 : RATE_11AC_HT160_MCS17);
			break;
		case RATE_AC_MCS_18_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS18 : RATE_11AC_HT160_MCS18);
			break;
		case RATE_AC_MCS_19_160:
			rate = (flag ? RATE_11AC_HT80P80_MCS19 : RATE_11AC_HT160_MCS19);
			break;
		case RATE_P_1p5Mbps_5:
			rate = RATE_11P_BW5_1p5_MBPS;
			break;
		case RATE_P_2Mbps_5:
			rate = RATE_11P_BW5_2p25_MBPS;
			break;
		case RATE_P_3Mbps_5:
			rate = RATE_11P_BW5_3_MBPS;
			break;
		case RATE_P_4p5Mbps_5:
			rate = RATE_11P_BW5_4p5_MBPS;
			break;
		case RATE_P_6Mbps_5:
			rate = RATE_11P_BW5_6_MBPS;
			break;
		case RATE_P_9Mbps_5:
			rate = RATE_11P_BW5_9_MBPS;
			break;
		case RATE_P_12Mbps_5:
			rate = RATE_11P_BW5_12_MBPS;
			break;
		case RATE_P_13p5Mbps_5:
			rate = RATE_11P_BW5_13p5_MBPS;
			break;
		case RATE_P_3Mbps_10:
			rate = RATE_11P_BW10_3_MBPS;
			break;
		case RATE_P_4p5Mbps_10:
			rate = RATE_11P_BW10_4p5_MBPS;
			break;
		case RATE_P_6Mbps_10:
			rate = RATE_11P_BW10_6_MBPS;
			break;
		case RATE_P_9Mbps_10:
			rate = RATE_11P_BW10_9_MBPS;
			break;
		case RATE_P_12Mbps_10:
			rate = RATE_11P_BW10_12_MBPS;
			break;
		case RATE_P_18Mbps_10:
			rate = RATE_11P_BW10_18_MBPS;
			break;
		case RATE_P_24Mbps_10:
			rate = RATE_11P_BW10_24_MBPS;
			break;
		case RATE_P_27Mbps_10:
			rate = RATE_11P_BW10_27_MBPS;
			break;
		case RATE_P_6Mbps_20:
			rate = RATE_11P_BW20_6_MBPS;
			break;
		case RATE_P_9Mbps_20:
			rate = RATE_11P_BW20_9_MBPS;
			break;
		case RATE_P_12Mbps_20:
			rate = RATE_11P_BW20_12_MBPS;
			break;
		case RATE_P_18Mbps_20:
			rate = RATE_11P_BW20_18_MBPS;
			break;
		case RATE_P_24Mbps_20:
			rate = RATE_11P_BW20_24_MBPS;
			break;
		case RATE_P_36Mbps_20:
			rate = RATE_11P_BW20_36_MBPS;
			break;
		case RATE_P_48Mbps_20:
			rate = RATE_11P_BW20_48_MBPS;
			break;
		case RATE_P_54Mbps_20:
			rate = RATE_11P_BW20_54_MBPS;
			break;
		case RATE_MCS_24_40:
			rate = RATE_11N_HT40_MCS24;
			break;
		case RATE_MCS_25_40:
			rate = RATE_11N_HT40_MCS25;
			break;
		case RATE_MCS_26_40:
			rate = RATE_11N_HT40_MCS26;
			break;
		case RATE_MCS_27_40:
			rate = RATE_11N_HT40_MCS27;
			break;
		case RATE_MCS_28_40:
			rate = RATE_11N_HT40_MCS28;
			break;
		case RATE_MCS_29_40:
			rate = RATE_11N_HT40_MCS29;
			break;
		case RATE_MCS_30_40:
			rate = RATE_11N_HT40_MCS30;
			break;
		case RATE_MCS_31_40:
			rate = RATE_11N_HT40_MCS31;
			break;
		case RATE_MCS_24_20:
			rate = RATE_11N_HT20_MCS24;
			break;
		case RATE_MCS_25_20:
			rate = RATE_11N_HT20_MCS25;
			break;
		case RATE_MCS_26_20:
			rate = RATE_11N_HT20_MCS26;
			break;
		case RATE_MCS_27_20:
			rate = RATE_11N_HT20_MCS27;
			break;
		case RATE_MCS_28_20:
			rate = RATE_11N_HT20_MCS28;
			break;
		case RATE_MCS_29_20:
			rate = RATE_11N_HT20_MCS29;
			break;
		case RATE_MCS_30_20:
			rate = RATE_11N_HT20_MCS30;
			break;
		case RATE_MCS_31_20:
			rate = RATE_11N_HT20_MCS31;
			break;
		}
	}

    void WCNTester_CommonSCPIWLAN::SetDutIndex(int nDutIndex)
    {
        mnnDutIndex = nDutIndex;
        JGW_FormatString(mstrscpiDutIndexList,("(@%d)"),nDutIndex);
        JGW_FormatString(mstrscpiPmas,"PMAS%d",nDutIndex);
    }

    void WCNTester_CommonSCPIWLAN::SetVsaPortMask2(uint dutPortMask)
    {
        std::string command;
        JGW_FormatString(command,":QCOM:WLAN:SENS:%s %d",mstrscpiPmas.c_str(),dutPortMask);
        mpEqipCommtl->write(command);
        ClearErrorQueue();
    }

    void WCNTester_CommonSCPIWLAN::TxWlanMeasurementConfiguration(int dutPortMask,int channel,double& evmDb,double& evmRateMbps,double& avgTxPowerDbm,int nBandWidth /* = 80 */,int mcs /* = 11 */,int nss /* = 1 */,float gint /* = 0.8 */,int ltft /* = 4 */,const std::string& link /* = "DOWN" */,const std::string& strPPDU /* = "SU" */,const std::string& strFEC /* = "LDPC" */,int dcm /* = 0 */,int nRefPower /* = 20 */,int trigLev /* = -20 */,int averCount /* = 3 */,const std::string& strWlanStand /* = "WIFI_11AX" */,bool modulationEnabled /* = true */, bool spectralPowerEnabled /* = true */, bool spectralMaskEnabled /* = true */,bool errorFrequencyEnabled /* = false */)
    {
        //! :QCOM:WLAN:SENS:CONF:MOD:PMAS1 1,1;
        SetVsaPortMask2(dutPortMask);
        std::string strWrite = ":QCOM:WLAN:SENS:CONF:MOD:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,modulationEnabled?"1":"0");
        mpEqipCommtl->write(strWrite);
        //! :QCOM:WLAN:SENS:CONF:POW:PMAS1 1,1;
        strWrite = ":QCOM:WLAN:SENS:CONF:POW:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,spectralPowerEnabled?"1":"0");
        mpEqipCommtl->write(strWrite);
        //! :QCOM:WLAN:SENS:CONF:SMAS:PMAS1 1,1;
        strWrite = ":QCOM:WLAN:SENS:CONF:SMAS:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,spectralMaskEnabled?"1":"0");
        mpEqipCommtl->write(strWrite);
        //! :QCOM:WLAN:SENS:CONF:EFR:PMAS1 1,0;
        strWrite = ":QCOM:WLAN:SENS:CONF:EFR:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,errorFrequencyEnabled?"1":"0");
        mpEqipCommtl->write(strWrite);
        //! :QCOM:WLAN:SENS:PAR:STAN:PMAS1 1,'WIFI_11AX';
        strWrite = ":QCOM:WLAN:SENS:PAR:STAN:" + mstrscpiPmas + " ";
        //AppendBitMaskParameters(strWrite,dutPortMask,strWlanStand);
        JGW_FormatString(strWrite,":QCOM:WLAN:SENS:PAR:STAN:%s %d,'%s'",mstrscpiPmas.c_str(),dutPortMask,strWlanStand.c_str());
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SENS:FREQ:BWID:PMAS1 1,80000000;
        strWrite = ":QCOM:WLAN:SENS:FREQ:BWID:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,nBandWidth * 1000 * 1000);
        mpEqipCommtl->write(strWrite);
        //! :QCOM:WLAN:SENS:MCS:PMAS1 1,11;
        strWrite = ":QCOM:WLAN:SENS:MCS:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,mcs);
        mpEqipCommtl->write(strWrite);

        //:QCOM:WLAN:SENS:NSS:PMAS1 1,1;
        strWrite = ":QCOM:WLAN:SENS:NSS:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,nss);
        mpEqipCommtl->write(strWrite);

        //:QCOM:WLAN:SENS:GINT:PMAS1 1,0.8;
        //strWrite = ":QCOM:WLAN:SENS:GINT:" + mstrscpiPmas + " ";
        JGW_FormatString(strWrite,":QCOM:WLAN:SENS:GINT:%s %d,%f",mstrscpiPmas.c_str(),dutPortMask,gint);
        //AppendBitMaskStateMask(strWrite,dutPortMask,gint);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SENS:LTFT:PMAS1 1,4;
        strWrite = ":QCOM:WLAN:SENS:LTFT:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,ltft);
        mpEqipCommtl->write(strWrite);
       
        //:QCOM:WLAN:SENS:LINK:PMAS1 1,DOWN;
        strWrite = ":QCOM:WLAN:SENS:LINK:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,link);
        mpEqipCommtl->write(strWrite);

        //:QCOM:WLAN:SENS:PPDU:PMAS1 1,SU;
        strWrite = ":QCOM:WLAN:SENS:PPDU:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,strPPDU);
        mpEqipCommtl->write(strWrite);

        //:QCOM:WLAN:SENS:FEC:PMAS1 1,LDPC;
        strWrite = ":QCOM:WLAN:SENS:FEC:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,strFEC);
        mpEqipCommtl->write(strWrite);

        //:QCOM:WLAN:SENS:DCM:PMAS1 1,0;
        strWrite = ":QCOM:WLAN:SENS:DCM:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,dcm);
        mpEqipCommtl->write(strWrite);

        //!:QCOM:WLAN:SENS:EVM:OCEM:PMAS1 1,PRE;
        strWrite = ":QCOM:WLAN:SENS:EVM:OCEM:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,"PRE");
        mpEqipCommtl->write(strWrite);

        //!:QCOM:WLAN:SENS:CFO:OEM:PMAS1 1,PRE;
        strWrite = ":QCOM:WLAN:SENS:CFO:OEM:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,"PRE");
        mpEqipCommtl->write(strWrite);

        //!:QCOM:WLAN:SENS:FREQ:PMAS1 1,5210000000;
        //strWrite = ":QCOM:WLAN:SENS:FREQ:" + mstrscpiPmas + " ";
        //AppendActiveMaskParameters(strWrite,dutPortMask,channel * 1000 * 1000);
        __int64 chFreqHz = static_cast<__int64>(channel * 1000000.0);
        JGW_FormatString(strWrite,":QCOM:WLAN:SENS:FREQ:%s %d,%I64d",mstrscpiPmas.c_str(),dutPortMask,chFreqHz);
        mpEqipCommtl->write(strWrite);

        //:QCOM:WLAN:SENS:POW:REF:PMAS1 1,20;
        strWrite = ":QCOM:WLAN:SENS:POW:REF:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,nRefPower );
        mpEqipCommtl->write(strWrite);
        //!  :QCOM:WLAN:SENS:TRIG:LEV:PMAS1 1,-20;
        strWrite = ":QCOM:WLAN:SENS:TRIG:LEV:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,trigLev );
        mpEqipCommtl->write(strWrite);
        //! :QCOM:WLAN:SENS:AVER:COUN:PMAS1 1,3;
        strWrite = ":QCOM:WLAN:SENS:AVER:COUN:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,averCount );
        mpEqipCommtl->write(strWrite);

        CheckForListModeError();
        //! :QCOM:WLAN:SENS:INIT (@1);:QCOM:WLAN:ERR?, Visa32.WriteString(string data), 07/05/2021 11:47:29.689086 上午
        //strWrite = ":QCOM:WLAN:SENS:INIT";
        std::string ret;
        JGW_FormatString(strWrite,":QCOM:WLAN:SENS:INIT %s;:QCOM:WLAN:ERR?",mstrscpiDutIndexList.c_str());
        mpEqipCommtl->query(strWrite,ret);
        //! :QCOM:WLAN:FETC (@1);:QCOM:WLAN:SYST:OPC?, Visa32.WriteString(string data), 07/05/2021 11:47:29.928162 上午
        //strWrite = ":QCOM:WLAN:FETC";
        JGW_FormatString(strWrite,":QCOM:WLAN:FETC %s;:QCOM:WLAN:SYST:OPC?",mstrscpiDutIndexList.c_str());
        mpEqipCommtl->query(strWrite,ret);
        BlockReturnUntilOperationCompleted(strWrite);
        //! :QCOM:WLAN:SENS:PMAS1 1;:QCOM:WLAN:ERR?, Visa32.WriteString(string data), 07/05/2021 11:47:29.967078 上午
        //JGW_FormatString(strWrite,":QCOM:WLAN:SENS:%s %d;:QCOM:WLAN:ERR?",mstrscpiPmas.c_str(),dutPortMask);
        //mpEqipCommtl->query(strWrite,ret);

        std::vector<double> vTest;
        //! :QCOM:WLAN:CALC:FBW:POW1:AVER?, Visa32.WriteString(string data), 07/05/2021 11:47:30.771583 上午
        JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:POW%d:AVER?",mnnDutIndex);
        QueryResultWlanActiveMask(strWrite,vTest);
        avgTxPowerDbm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  
        if (modulationEnabled)
        {
            vTest.clear();
            //! :QCOM:WLAN:CALC:FBW:EVM1:CHA?, Visa32.WriteString(string data), 07/05/2021 11:47:29.977764 上午
            JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:EVM%d:CHA?",mnnDutIndex);
            QueryResultWlanActiveMask(strWrite,vTest);
            evmDb = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];    

            vTest.clear();
            //! :QCOM:WLAN:CALC:FBW:RATE1?, Visa32.WriteString(string data), 07/05/2021 11:47:30.78012 上午
            JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:RATE%d?",mnnDutIndex);
            QueryResultWlanActiveMask(strWrite,vTest);
            evmRateMbps = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  
        }
    }

    void WCNTester_CommonSCPIWLAN::GetEVMTxQuality2(double& carrierFreqErrorPpm,double& phaseNoiseDb,double& symbolClockErrorPpm, double& ampImbDb, double& loLeakageDbc,double& phaseImbDeg)
    {
        std::vector<double> vTest;

        std::string strWrite;
        JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:EAMP%d?",mnnDutIndex);
        QueryResultWlanActiveMask(strWrite,vTest);
        ampImbDb = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];    

        phaseNoiseDb = INVAILD_WLAN_RESULT;
        vTest.clear();
        JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:EPH%d?",mnnDutIndex);
        QueryResultWlanActiveMask(strWrite,vTest);
        phaseImbDeg = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  

        vTest.clear();
        //! 138. QCOM:WLAN:CALC:FBW:ECL1?
        JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:ECL%d?",mnnDutIndex);
        QueryResultWlanActiveMask(strWrite,vTest);
        symbolClockErrorPpm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  

        vTest.clear();
        //! :QCOM:WLAN:CALC:FBW:EFR1?
        JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:EFR%d?",mnnDutIndex);
        QueryResultWlanActiveMask(strWrite,vTest);
        carrierFreqErrorPpm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];  

        vTest.clear();
        //!:QCOM:WLAN:CALC:FBW:LEAK1?
        JGW_FormatString(strWrite,":QCOM:WLAN:CALC:FBW:LEAK%d?",mnnDutIndex);
        QueryResultWlanActiveMask(strWrite,vTest);
        loLeakageDbc = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];
    }

    void WCNTester_CommonSCPIWLAN::GetMeasureMask2(bool& maskPass,double& marginsViolationPct/*,double& avgTxPowerDbm,std::vector<double>& marginsDbr,std::vector<double>& marginsFreqHz,bool wlanRemoveInBandSegments  = true , bool returnActivePorts  = false*/ )
	{
#if 1
		marginsViolationPct = INVAILD_WLAN_RESULT;
		maskPass = false;
        //! :QCOM:WLAN:CALC:FBW:SMAS1:MARG?

		std::string strRead,strCommand;
		std::vector<std::string> vRead;

        JGW_FormatString(strCommand,":QCOM:WLAN:CALC:FBW:SMAS%d:MARG?",mnnDutIndex);
		mpEqipCommtl->query(strCommand,strRead);
		JGW_ParserStrA(strRead.c_str(),",",vRead);

		if (vRead.size() < 3)
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}

		uint index = 1;
		uint dutPortMask = atoi(vRead[index].c_str());
		uint totalPortCount = 0;
		int activeCount = MaskActiveCount(dutPortMask, totalPortCount);

		if (0 != atoi(vRead[0].c_str()))
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}

		int num5 = 0;
		for (size_t i = 0; i < totalPortCount; i++)
		{
			if (((dutPortMask >> i) & 1) != 0)
			{
				int num6 = atoi(vRead[++index].c_str());
				int num7 = atoi(vRead[++index].c_str());
				marginsViolationPct/*[num5]*/ = ((num6 != 0) ? (100.0 * (double)num7 / (double)num6) : INVAILD_WLAN_RESULT);
				if (0 == marginsViolationPct)
				{
					maskPass = true;
				}
				else
				{
					maskPass = false;
				}
			}
		}
#else

		std::vector<double> vTest;
		int num2 = 1;

		if (!QueryResultWlanActiveMask(":QCOM:WLAN:CALC:SMAS:MARG?",vTest,num2))
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}
		if (vTest.size() < 2)
		{
			marginsViolationPct = INVAILD_WLAN_RESULT;
			maskPass = false;
			return ;
		}
		//! maskPass = true;
		int num = 0;
		int num6 = static_cast<int>(vTest[num++]);
		int num7 = static_cast<int>(vTest[num++]);
		marginsViolationPct/*[num5]*/ = ((num6 != 0) ? (100.0 * (double)num7 / (double)num6) : INVAILD_WLAN_RESULT);
		if (0 == marginsViolationPct)
		{
			maskPass = true;
		}
		else
		{
			maskPass = false;
		}
#endif
#if 0
		size_t totalPortCount;
		int num3 = MaskActiveCount(num2, totalPortCount);
		int num4 = returnActivePorts ? num3 : totalPortCount;
		int num5 = 0;
		int num = 0;
		for (size_t i = 0; i < totalPortCount; i++)
		{
			if (((num2 >> i) & 1) != 0)
			{
				int num6 = static_cast<int>(vTest[num++]);
				int num7 = static_cast<int>(vTest[num++]);
				marginsViolationPct/*[num5]*/ = ((num6 != 0) ? (100.0 * (double)num7 / (double)num6) : INVAILD_WLAN_RESULT);
				int num8;
				int num9;
				int num10;
				if (wlanRemoveInBandSegments)
				{
					num8 = (int)((double)num6 / 2.0 - 0.5);
					num9 = ((num6 % 2 != 0) ? 1 : 2);
					num10 = num8 + num9 - 1;
				}
				else
				{
					num8 = num6;
					num9 = 0;
					num10 = num6;
				}
				/*marginsFreqHz[num5] = new double[num6 - num9];
				marginsDbr[num5] = new double[num6 - num9];*/
				for (int j = 0; j < num6; j++)
				{
					if (j < num8)
					{
						marginsFreqHz.push_back(vTest[num++]);
						//marginsFreqHz[num5][j] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
						marginsDbr.push_back(vTest[num++]);
						//marginsDbr[num5][j] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
					}
					else if (j > num10)
					{
						marginsFreqHz.push_back(vTest[num++]);
						marginsDbr.push_back(vTest[num++]);
						//marginsFreqHz[num5][j - num9] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
						//marginsDbr[num5][j - num9] = double.Parse(array[++num], WCNTester_CommonSCPICommon._numberStyleFloat, WCNTester_CommonSCPICommon._formatProvider);
					}
					else
					{
						num += 2;
					}
				}
				/*num5++;*/
			}
			else if (!returnActivePorts)
			{
				marginsViolationPct = 0.0;
				//marginsFreqHz[num5] = new double[0];
				//marginsDbr[num5++] = new double[0];
			}
		}

		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:SMAS:MARG:FREQ:ENDP?",vTest);



		vTest.clear();
		QueryResultWlanActiveMask(":QCOM:WLAN:CALC:POW:AVER?",vTest);
		avgTxPowerDbm = vTest.empty()?INVAILD_WLAN_RESULT:vTest[0];
#endif
	}

    void WCNTester_CommonSCPIWLAN::RxWlanMeasurementConfiguration(int dutPortMask,int channel,int nBandWidth /* = 80 */,int mcs /* = 11 */,const std::string& strWlanStand /* = "WIFI_11AX" */,int nss /* = 1 */,float signalLeveldBm /* = -55.00 */,int numPktCount /* = 100 */,int ltft /* = 4*/,float gint /* = 0.8 */,const std::string& link /* = "UP" */,const std::string& strPPDU /* = "SU" */,const std::string& strFEC /* = "LDPC" */,int dcm /* = 0 */,int stbc /* = 0 */)
    {
#if 1
        std::string strWrite,strRet;
        //! :QCOM:WLAN:SOUR:PMAS1 1;
        //! :QCOM:WLAN:ERR?
        //! :QCOM:WLAN:SOUR:PMAS1 1;:QCOM:WLAN:ERR?
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:%s %d;:QCOM:WLAN:ERR?",mstrscpiPmas.c_str(),dutPortMask);
        mpEqipCommtl->query(strWrite,strRet);

        //! :QCOM:WLAN:SOUR:FREQ:PMAS1 1,5500000000;:QCOM:WLAN:SOUR:POW:PMAS1 1,-50
        __int64 chFreqHz = static_cast<__int64>(channel * 1000000.0);
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:FREQ:%s %d,%I64d;:QCOM:WLAN:SOUR:POW:%s %d,%.2f",mstrscpiPmas.c_str(),dutPortMask,chFreqHz,mstrscpiPmas.c_str(),dutPortMask,signalLeveldBm);
        mpEqipCommtl->write(strWrite);
        //! :QCOM:WLAN:SOUR:PMAS1 1;:QCOM:WLAN:ERR?
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:%s %d;:QCOM:WLAN:ERR?",mstrscpiPmas.c_str(),dutPortMask);
        mpEqipCommtl->query(strWrite,strRet);
        //! :QCOM:WLAN:SOUR:PAR:STAN:PMAS1 1,'WIFI_11AX';:QCOM:WLAN:SOUR:FREQ:BWID:PMAS1 1,80000000;:QCOM:WLAN:SOUR:MCS:PMAS1 1,11;:QCOM:WLAN:SOUR:NSS:PMAS1 1,1;:QCOM:WLAN:SOUR:TRIG:TYPE:PMAS1 1,SING;:QCOM:WLAN:SOUR:SWE:COUN:PMAS1 1,100;:QCOM:WLAN:SOUR:ARB:STAT:PMAS1 1,1;:QCOM:WLAN:SOUR:STAT:PMAS1 1,0
        //! :QCOM:WLAN:SOUR:PAR:STAN:PMAS1 1,'WIFI_11AC';:QCOM:WLAN:SOUR:FREQ:BWID:PMAS1 1,80000000;:QCOM:WLAN:SOUR:MCS:PMAS1 1,9;:QCOM:WLAN:SOUR:NSS:PMAS1 1,1;:QCOM:WLAN:SOUR:TRIG:TYPE:PMAS1 1,SING;:QCOM:WLAN:SOUR:SWE:COUN:PMAS1 1,100;:QCOM:WLAN:SOUR:ARB:STAT:PMAS1 1,1;:QCOM:WLAN:SOUR:STAT:PMAS1 1,0
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:PAR:STAN:%s %d,'%s';:QCOM:WLAN:SOUR:FREQ:BWID:%s %d,%d;:QCOM:WLAN:SOUR:MCS:%s %d,%d;:QCOM:WLAN:SOUR:NSS:%s %d,%d;:QCOM:WLAN:SOUR:TRIG:TYPE:%s %d,SING;:QCOM:WLAN:SOUR:SWE:COUN:%s %d,%d;:QCOM:WLAN:SOUR:ARB:STAT:%s %d,1;:QCOM:WLAN:SOUR:STAT:%s %d,0"
            ,mstrscpiPmas.c_str(),dutPortMask,strWlanStand.c_str()
            ,mstrscpiPmas.c_str(),dutPortMask,nBandWidth * 1000 * 1000
            ,mstrscpiPmas.c_str(),dutPortMask,mcs
            ,mstrscpiPmas.c_str(),dutPortMask,nss
            ,mstrscpiPmas.c_str(),dutPortMask
            ,mstrscpiPmas.c_str(),dutPortMask,numPktCount
            ,mstrscpiPmas.c_str(),dutPortMask
            ,mstrscpiPmas.c_str(),dutPortMask);
        mpEqipCommtl->write(strWrite);

        if (0 == JGW_StrComparenoCaseStr(strWlanStand.c_str(),"WIFI_11AX"))
        {
            //!:QCOM:WLAN:SOUR:PAR:STAN:PMAS1 1,'WIFI_11AX';:QCOM:WLAN:SOUR:FREQ:BWID:PMAS1 1,80000000;:QCOM:WLAN:SOUR:MCS:PMAS1 1,11;:QCOM:WLAN:SOUR:NSS:PMAS1 1,1;:QCOM:WLAN:SOUR:GINT:PMAS1 1,0.8;:QCOM:WLAN:SOUR:LTFT:PMAS1 1,0;:QCOM:WLAN:SOUR:LINK:PMAS1 1,UP;:QCOM:WLAN:SOUR:PPDU:PMAS1 1,SU;:QCOM:WLAN:SOUR:FEC:PMAS1 1,LDPC;:QCOM:WLAN:SOUR:DCM:PMAS1 1,0;:QCOM:WLAN:SOUR:STBC:PMAS1 1,0;:QCOM:WLAN:SOUR:FREQ:PMAS1 1,5500000000;:QCOM:WLAN:SOUR:POW:PMAS1 1,-50
            //ltft = 0;
            //link = "UP";
            //strPPDU = "SU";
            //strFEC = "LDPC";
            JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:PAR:STAN:%s %d,'%s';:QCOM:WLAN:SOUR:FREQ:BWID:%s %d,%d;:QCOM:WLAN:SOUR:MCS:%s %d,%d;:QCOM:WLAN:SOUR:NSS:%s %d,%d;:QCOM:WLAN:SOUR:GINT:%s %d,%.1f;:QCOM:WLAN:SOUR:LTFT:%s %d,%d;:QCOM:WLAN:SOUR:LINK:%s %d,%s;:QCOM:WLAN:SOUR:PPDU:%s %d,%s;:QCOM:WLAN:SOUR:FEC:%s %d,%s;:QCOM:WLAN:SOUR:DCM:%s %d,%d;:QCOM:WLAN:SOUR:STBC:%s %d,%d;:QCOM:WLAN:SOUR:FREQ:%s %d,%I64d;:QCOM:WLAN:SOUR:POW:%s %d,%.2f"
                ,mstrscpiPmas.c_str(),dutPortMask,strWlanStand.c_str()
                ,mstrscpiPmas.c_str(),dutPortMask,nBandWidth * 1000 * 1000
                ,mstrscpiPmas.c_str(),dutPortMask,mcs
                ,mstrscpiPmas.c_str(),dutPortMask,nss
                ,mstrscpiPmas.c_str(),dutPortMask,gint
                ,mstrscpiPmas.c_str(),dutPortMask,0/*ltft*/
                ,mstrscpiPmas.c_str(),dutPortMask,"UP"/*link.c_str()*/
                ,mstrscpiPmas.c_str(),dutPortMask,"SU"/*strPPDU.c_str()*/
                ,mstrscpiPmas.c_str(),dutPortMask,"LDPC"/*strFEC.c_str()*/
                ,mstrscpiPmas.c_str(),dutPortMask,dcm
                ,mstrscpiPmas.c_str(),dutPortMask,stbc
                ,mstrscpiPmas.c_str(),dutPortMask,chFreqHz
                ,mstrscpiPmas.c_str(),dutPortMask,signalLeveldBm
                );
            mpEqipCommtl->write(strWrite);
        }
        else
        {
            //! :QCOM:WLAN:SOUR:PAR:STAN:PMAS1 1,'WIFI_11AC';:QCOM:WLAN:SOUR:FREQ:BWID:PMAS1 1,80000000;:QCOM:WLAN:SOUR:MCS:PMAS1 1,9;:QCOM:WLAN:SOUR:NSS:PMAS1 1,1;:QCOM:WLAN:SOUR:GINT:PMAS1 1,0.8;:QCOM:WLAN:SOUR:LINK:PMAS1 1,UP;:QCOM:WLAN:SOUR:PPDU:PMAS1 1,SU;:QCOM:WLAN:SOUR:FEC:PMAS1 1,LDPC;:QCOM:WLAN:SOUR:STBC:PMAS1 1,0;:QCOM:WLAN:SOUR:FREQ:PMAS1 1,5500000000;:QCOM:WLAN:SOUR:POW:PMAS1 1,-60
            JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:PAR:STAN:%s %d,'%s';:QCOM:WLAN:SOUR:FREQ:BWID:%s %d,%d;:QCOM:WLAN:SOUR:MCS:%s %d,%d;:QCOM:WLAN:SOUR:NSS:%s %d,%d;:QCOM:WLAN:SOUR:GINT:%s %d,%.1f;:QCOM:WLAN:SOUR:LINK:%s %d,%s;:QCOM:WLAN:SOUR:PPDU:%s %d,%s;:QCOM:WLAN:SOUR:FEC:%s %d,%s;:QCOM:WLAN:SOUR:STBC:%s %d,%d;:QCOM:WLAN:SOUR:FREQ:%s %d,%I64d;:QCOM:WLAN:SOUR:POW:%s %d,%.2f"
                ,mstrscpiPmas.c_str(),dutPortMask,strWlanStand.c_str()
                ,mstrscpiPmas.c_str(),dutPortMask,nBandWidth * 1000 * 1000
                ,mstrscpiPmas.c_str(),dutPortMask,mcs
                ,mstrscpiPmas.c_str(),dutPortMask,nss
                ,mstrscpiPmas.c_str(),dutPortMask,gint
                ,mstrscpiPmas.c_str(),dutPortMask,link.c_str()
                ,mstrscpiPmas.c_str(),dutPortMask,strPPDU.c_str()
                ,mstrscpiPmas.c_str(),dutPortMask,strFEC.c_str()
                ,mstrscpiPmas.c_str(),dutPortMask,stbc
                ,mstrscpiPmas.c_str(),dutPortMask,chFreqHz
                ,mstrscpiPmas.c_str(),dutPortMask,signalLeveldBm
                );
            mpEqipCommtl->write(strWrite);
        }
        

        
        

        

        

        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:TRIG:TYPE:%s %d,SING;:QCOM:WLAN:SOUR:SWE:COUN:%s %d,%d;:QCOM:WLAN:SOUR:ARB:STAT:%s %d,1;:QCOM:WLAN:SOUR:STAT:%s %d,0;:QCOM:WLAN:SOUR:CONF:INIT %s;:QCOM:WLAN:SYST:OPC?"
            ,mstrscpiPmas.c_str(),dutPortMask
            ,mstrscpiPmas.c_str(),dutPortMask,numPktCount
            ,mstrscpiPmas.c_str(),dutPortMask
            ,mstrscpiPmas.c_str(),dutPortMask
            ,mstrscpiDutIndexList.c_str());

        mpEqipCommtl->query(strWrite,strRet);

        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:INIT %s",mstrscpiDutIndexList.c_str());
        mpEqipCommtl->write(strWrite);

        //! START ARB 
        //:QCOM:WLAN:SOUR:TRIG:TYPE:PMAS1 1,CONT;:QCOM:WLAN:SOUR:ARB:STAT:PMAS1 1,0;:QCOM:WLAN:SOUR:STAT:PMAS1 1,0
#else
        std::string strWrite;
        //! :QCOM:WLAN:SOUR:PMAS1 1;
        //! :QCOM:WLAN:ERR?
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:%s %d",mstrscpiPmas.c_str(),dutPortMask);
        mpEqipCommtl->write(strWrite);
        ClearErrorQueue();
       
        //! :QCOM:WLAN:SOUR:PAR:STAN:PMAS1 1,'WIFI_11AX';
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:PAR:STAN:%s %d,'%s'",mstrscpiPmas.c_str(),dutPortMask,strWlanStand.c_str());
        mpEqipCommtl->write(strWrite);
        
        //! :QCOM:WLAN:SOUR:FREQ:BWID:PMAS1 1,80000000;
        strWrite = ":QCOM:WLAN:SOUR:FREQ:BWID:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,nBandWidth * 1000 * 1000);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:MCS:PMAS1 1,11;
        strWrite = ":QCOM:WLAN:SOUR:MCS:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,mcs);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:NSS:PMAS1 1,1;
        strWrite = ":QCOM:WLAN:SOUR:NSS:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,nss);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:GINT:PMAS1 1,0.8;
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:GINT:%s %d,%.1f",mstrscpiPmas.c_str(),dutPortMask,gint);
        mpEqipCommtl->write(strWrite);


        //! :QCOM:WLAN:SOUR:LTFT:PMAS1 1,4;
        strWrite = ":QCOM:WLAN:SOUR:LTFT:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,ltft);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:LINK:PMAS1 1,UP;
        strWrite = ":QCOM:WLAN:SOUR:LINK:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,link);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:PPDU:PMAS1 1,SU;
        strWrite = ":QCOM:WLAN:SOUR:PPDU:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,strPPDU);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:FEC:PMAS1 1,LDPC;
        strWrite = ":QCOM:WLAN:SOUR:FEC:" + mstrscpiPmas + " ";
        AppendBitMaskParameters(strWrite,dutPortMask,strFEC);
        mpEqipCommtl->write(strWrite);
        
        //! :QCOM:WLAN SOUR:DCM:PMAS1 1,0
        //strWrite = ":QCOM:WLAN SOUR:DCM:" + mstrscpiPmas + " ";
        //AppendActiveMaskParameters(strWrite,dutPortMask,dcm);
        //mpEqipCommtl->write(strWrite);
        
        //! :QCOM:WLAN:SOUR:STBC:PMAS1 1,0;
        strWrite = ":QCOM:WLAN:SOUR:STBC:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,stbc);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:FREQ:PMAS1 1,5220000000;
        __int64 chFreqHz = static_cast<__int64>(channel * 1000000.0);
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:FREQ:%s %d,%I64d",mstrscpiPmas.c_str(),dutPortMask,chFreqHz);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:POW:PMAS1 1,-56
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:POW:%s %d,%.2f",mstrscpiPmas.c_str(),dutPortMask,signalLeveldBm);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:TRIG:TYPE:PMAS1 1,SING;
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:TRIG:TYPE:%s %d,%s",mstrscpiPmas.c_str(),dutPortMask,"SING");
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:SWE:COUN:PMAS1 1,100;
        strWrite = ":QCOM:WLAN:SOUR:SWE:COUN:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,numPktCount);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:ARB:STAT:PMAS1 1,1;
        strWrite = ":QCOM:WLAN:SOUR:ARB:STAT:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,1);
        mpEqipCommtl->write(strWrite);

        //! :QCOM:WLAN:SOUR:STAT:PMAS1 1,0;
        strWrite = ":QCOM:WLAN:SOUR:STAT:" + mstrscpiPmas + " ";
        AppendActiveMaskParameters(strWrite,dutPortMask,0);
        mpEqipCommtl->write(strWrite);

        std::string ret;
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:CONF:INIT %s;:QCOM:WLAN:ERR?",mstrscpiDutIndexList.c_str());
        mpEqipCommtl->query(strWrite,ret);
        //! :QCOM:WLAN:SOUR:CONF:INIT (@1);
        //:QCOM:WLAN:SYST:OPC?
#endif
    }

    void WCNTester_CommonSCPIWLAN::StopRx(int dutPortMask)
    {
        //! :QCOM:WLAN:SOUR:TRIG:TYPE:PMAS1 1,CONT;:QCOM:WLAN:SOUR:ARB:STAT:PMAS1 1,0;:QCOM:WLAN:SOUR:STAT:PMAS1 1,0
        std::string strWrite;
        JGW_FormatString(strWrite,":QCOM:WLAN:SOUR:TRIG:TYPE:%s %d,CONT;:QCOM:WLAN:SOUR:ARB:STAT:%s %d,0;:QCOM:WLAN:SOUR:STAT:%s %d,0"
            ,mstrscpiPmas.c_str(),dutPortMask
            ,mstrscpiPmas.c_str(),dutPortMask
            ,mstrscpiPmas.c_str(),dutPortMask);
        mpEqipCommtl->write(strWrite);
    }
}

