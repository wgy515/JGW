#include "StdAfx.h"
#include <JGW_Device_RohdeSchwarz_CMW500/CMW500_RFVFS.h>
#include <JGW_NIGPIBControllImpl/equipcommtlv1_0_HIVE.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_gpib_agilent.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_tcp_ip.h>
#include <JGW_NIGPIBControllImpl/equipcommtl_VISA.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_StringFuncTemplate.hpp>

namespace JGW
{

    CMW500_RFVFS::CMW500_RFVFS()
    {

    }

    CMW500_RFVFS::~CMW500_RFVFS()
    {
        if ( m_pEquipInter )
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
        }
    }

    void CMW500_RFVFS::SetGPIB(BYTE bGPIBType /* = NI_GPIB_TYPE */, DWORD dwBoardIndex /* = 0 */, DWORD dwPrimaryAddress /* = 20 */,DWORD secAddress /* = 96 */)
    {
//         if ( m_pEquipInter )
//         {
//             delete m_pEquipInter;
//             m_pEquipInter = NULL;
//         }
// 
//         if ( NI_GPIB_TYPE == bGPIBType )
//         {
//             m_pEquipInter = new CEquipCommTLV1_0_HIVE( dwPrimaryAddress,96,dwBoardIndex );
//         }

        if ( m_pEquipInter )
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
        }

        if (AGILENT_GPIB_TYPE == bGPIBType)
        {
            m_pEquipInter = new CEquipCommTL_GPIB_Agilent( dwPrimaryAddress,secAddress,dwBoardIndex );
        }
        else
        {       
            m_pEquipInter = new CEquipCommTLV1_0_HIVE( dwPrimaryAddress,secAddress,dwBoardIndex );
        }
    }

    void CMW500_RFVFS::SetTCPIP(const char* strIPAddress,int nSubInstrument /* = 0 */)
    {
        if ( m_pEquipInter )
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
        }
        m_pEquipInter = new CEquipCommTL_TCP_IP(strIPAddress,nSubInstrument);
    }

    void CMW500_RFVFS::SetVisatToDeviceName(const char* strDeviceName,int nSubInstrument /* = 0 */)
    {
        char visaAddress[80] = {0};
        sprintf_s(visaAddress,"TCPIP0::%s::inst%d::INSTR",strDeviceName,nSubInstrument);
        SetVisa(visaAddress);
    }

    void CMW500_RFVFS::SetVisa(const char* visaAddress /* = "TCPIP0:::CMW50050-134371:::inst0:::INSTR" */)
    {
        if ( m_pEquipInter )
        {
            delete m_pEquipInter;
            m_pEquipInter = NULL;
        }
        m_pEquipInter = new CEquipCommTL_VISA(visaAddress);
    }


    void CMW500_RFVFS::SetTxLoss( float fTxLoss /* = 1.00f */ )
    {
        std::string strCommand;
        JGW_FormatString(strCommand,"SOURce:GPRF:GEN:RFSettings:EATTenuation %0.2f",fTxLoss);

        m_pEquipInter->write(strCommand);
    }

    void CMW500_RFVFS::SetRxLoss( float fRxLoss /* = 1.00f */ )
    {
        std::string strCommand;
        JGW_FormatString(strCommand,"CONFigure:%s:MEAS:RFSettings:EATTenuation %0.2f",g_szSysName[m_eSysName],fRxLoss);

        m_pEquipInter->write(strCommand);
    }

    bool CMW500_RFVFS::InitMode()
    {
        return ResetMode();
    }

    bool CMW500_RFVFS::ResetMode()
    {
        std::vector<std::string> lossTableList;
        std::string strReturn;
        bool bResult = true;

        m_pEquipInter->query(std::string("CONFigure:BASE:FDCorrection:CTABle:CATalog?"),strReturn);

        if ( std::string::npos != strReturn.find(",") )
        {
            JGW_StringFunc<std::string>::StokString( strReturn, ",",lossTableList);

            for( size_t i = 0; i < lossTableList.size(); i++ )
            {
                JGW_FormatString(strReturn,"CONFigure:BASE:FDCorrection:CTABle:DELete %s ;*OPC?",lossTableList.at(i).c_str());
                bResult &= m_pEquipInter->write(strReturn);
            }
        }

        std::string strCommand;
        JGW_FormatString(strCommand,"*RST;"
            "*CLS;"
            "*OPC?;:SYST:ERR?;"
            );
        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return bResult;
    }

    bool CMW500_RFVFS::FinalizeBSE()
    {
        std::string strCommand;
        strCommand = "SOURce:GPRF:GEN:STATe OFF;"
            "*OPC?;:SYST:ERR?;";
        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::SetInOutPutPort(int nTxNumb,int nRxNumb /* = ERFAC  */)
    {
        std::string buffer;
        char opc[10] = {0};
        bool bResult = true;

        bResult &= m_pEquipInter->write("*CLS;*OPC?");
        bResult &= m_pEquipInter->read(opc,10);
        CHECKOPC(opc);
        //RFAC RFBC
        JGW_FormatString(buffer,"ROUT:%s:MEAS:SCEN:SALone RF%s, RX1",g_szSysName[m_eSysName],GetRFRxStrFlags(nRxNumb));
        m_pEquipInter->write(buffer);
        //RFAC RFAO RFBC
        //仪器TX对应设备RX  我们不用RFAO口，所以只用RFAC RFBC 理论上应该是都应该配置为都使用，但配置已固定死2口为RFBC 因此不对RFAO在进行适配
        JGW_FormatString(buffer,"ROUTe:GPRF:GENerator:SCENario:SALone RF%s, TX1",GetRFTxStrFlags(nTxNumb));
        m_pEquipInter->write(buffer);

        Sleep(200);

        return true;
    }

    const char* CMW500_RFVFS::LTE_GetWaveFile( E_LTE_MODE eLteMode /* = E_LTE_FDD */ )
    {
        switch( m_eLteChBWType )
        {
        case E_LTE_CH_BW_1_4_MHZ:
            return "1.4MHz Bandwidth is not supported";
        case E_LTE_CH_BW_03_MHZ:
            return "3MHz Bandwidth is not supported";
        case E_LTE_CH_BW_05_MHZ:
            return ( E_LTE_FDD == eLteMode )?"LTE_ProductionTest_8_v01.wv":"LTE_TDD_RMCA322_5MHz.wv";
        case E_LTE_CH_BW_10_MHZ:
            return ( E_LTE_FDD == eLteMode )?"LTE_ProductionTest_1_v01.wv":"LTE_ProductionTest_TDD_1_v01.wv";
        case E_LTE_CH_BW_15_MHZ:
            return ( E_LTE_FDD == eLteMode )?"LTE_RMC_15MHz_QPSK_AllUp_FDD_v01.wv":"LTE_TDD_RMCA322_15MHz.wv";
        case E_LTE_CH_BW_20_MHZ:
            return ( E_LTE_FDD == eLteMode )?"LTE_RMC_20MHz_QPSK_AllUp_FDD_v01.wv":"LTE_TDD_RMCA322_20MHz.wv";
        }

        return "";
    }

    bool CMW500_RFVFS::CheckGPRFGenStateIsOn()
    {
        static std::string strTemp;

        m_pEquipInter->query("SOUR:GPRF:GEN:STAT?", strTemp);

        if ( std::string::npos != strTemp.find("ON") )
            return true;

        return false;
    }

    bool CMW500_RFVFS::LTE_SetBandAndMode(int nBand)
    {
        E_LTE_MODE eMode = (nBand >= 1 && nBand <= 32)?E_LTE_FDD :E_LTE_TDD;
        std::string strTemp;
        std::string strCommand;
        int i = 0;

        JGW_FormatString(strTemp,"OB%d",nBand);

        JGW_FormatString(strCommand,"CONFigure:LTE:MEAS:DMODe %s", g_szLteModeName[eMode]); //set to FDD or TDD
        m_pEquipInter->write(strCommand);

        JGW_FormatString(strCommand,"CONFigure:LTE:MEAS:BAND %s", strTemp.c_str()); //set to band7/band38/band39/band40
        m_pEquipInter->write(strCommand);
        m_pEquipInter->write("SOURce:GPRF:GEN:BBMode ARB");
        m_pEquipInter->write("SOURce:GPRF:GEN:ARB:REP CONT");
        m_pEquipInter->query("SOURce:GPRF:GENerator:ARB:FILE?", strTemp);

        std::string sWfmFile;
        sWfmFile = LTE_GetWaveFile( eMode );

        if (std::string::npos == strTemp.find(sWfmFile.c_str()))
        {
            m_pEquipInter->write("SOURce:GPRF:GEN:STATe OFF");
            JGW_FormatString(strCommand,"SOURce:GPRF:GENerator:ARB:FILE \'D:\\Rohde-Schwarz\\CMW\\Data\\waveform\\library1\\%s\'",sWfmFile.c_str());
            m_pEquipInter->write(strCommand);
            m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");

            for( i = 0; i < 100; i++)
            {
                if( CheckGPRFGenStateIsOn() )break;

                Sleep(200);
            }

            if (i >= 100)
            {
                return false;
            }

            m_pEquipInter->query("SOURce:GPRF:GENerator:ARB:FILE?", strTemp);

            if ( std::string::npos == strTemp.find( sWfmFile.c_str() ) )
            {
                return  false;
            }
        }
        else
        {
            m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");
        }

        return true;
    }

    bool CMW500_RFVFS::SetGPRFDlUlFreq(double dFreqDL, double dFreqUL,float nCellPower /* = -57.00f */,float nEnPower /* = 23.00f */)
    {
        std::string strCommand;

        SetCellPoweAEnPower( nCellPower,nEnPower );

        JGW_FormatString(strCommand,"CONFigure:%s:MEASurement:RFSettings:FREQuency %fMHZ;*OPC",g_szSysName[m_eSysName], dFreqUL);
        m_pEquipInter->write(strCommand);
        //m_pEquipInter->read(strCommand);

        JGW_FormatString(strCommand, "SOURce:GPRF:GENerator:RFSettings:FREQuency %fMHZ;*OPC", dFreqDL);
        m_pEquipInter->write(strCommand);
        //m_pEquipInter->read(strCommand);


        return true;
    }

    bool CMW500_RFVFS::SetGPRFDlUlFreq(S_BAND_CHANNEL_INFO& sBandChannelInfo,float nCellPower /* = -57.00f */,float nEnPower /* = 23.00f */)
    {
        std::string strCommand;

        SetCellPoweAEnPower( nCellPower,nEnPower );

        JGW_FormatString(strCommand,"CONFigure:%s:MEASurement:RFSettings:FREQuency %fMHZ;*OPC",g_szSysName[m_eSysName], sBandChannelInfo.m_fUpLinkFreq);
        m_pEquipInter->write(strCommand);
        //m_pEquipInter->read(strCommand);

        JGW_FormatString(strCommand, "SOURce:GPRF:GENerator:RFSettings:FREQuency %fMHZ;*OPC", sBandChannelInfo.m_fDownLinkFreq);
        m_pEquipInter->write(strCommand);
        //m_pEquipInter->read(strCommand);


        return true;
    }

    bool CMW500_RFVFS::SetCellPoweAEnPower( float nCellPower /* = -57.00f */,float nEnPower /* = 23.00f */ )
    {
        std::string strCommand;
        if( nCellPower > 0) nCellPower = -57;

        JGW_FormatString(strCommand,"CONFigure:%s:MEASurement:RFSettings:ENPower %.2f;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel %.2f;"
            ":CONFigure:%s:MEAS:RFSettings:UMARgin 10;"
            "*OPC?;:SYST:ERR?;"
            ,g_szSysName[m_eSysName],nEnPower,nCellPower,g_szSysName[m_eSysName]
        );
        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::SetCellPower( float fCellPower )
    {
        std::string strCommand;
        if( fCellPower > 0) fCellPower = -57.00f;

        JGW_FormatString(strCommand,"SOURce:GPRF:GEN:RFSettings:LEVel %.2f;"
            "*OPC?;:SYST:ERR?;"
            ,fCellPower
            );
        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::SetGPRFBand(int nBand)
    {
        std::string strCommand;

        if ( E_LTE_NAME == m_eSysName ||  E_WCDMA_NAME == m_eSysName )
        {
            JGW_FormatString(strCommand,"CONFigure:%s:MEAS:BAND OB%d",g_szSysName[m_eSysName], nBand);
            return m_pEquipInter->write(strCommand);
        }
        else if ( E_CDMA_NAME == m_eSysName )
        {
            return m_pEquipInter->write("CONFigure:CDMA:MEAS:RFSettings:BCLass USC");
        }
        else if ( E_GSM_NAME == m_eSysName )
        {
            switch(nBand)
            {
            case 850:
            case 5:
                JGW_FormatString(strCommand,"CONFigure:GSM:MEAS:BAND G085");
                break;
            case 900:
            case 8:
                JGW_FormatString(strCommand,"CONFigure:GSM:MEAS:BAND G09");
                break;
            case 1800:
            case 3:
                JGW_FormatString(strCommand,"CONFigure:GSM:MEAS:BAND G18");
                break;
            case 1900:
            case 2:
                JGW_FormatString(strCommand,"CONFigure:GSM:MEAS:BAND G19");
                break;
            default: 
                break;
            }

            return m_pEquipInter->write(strCommand);
        }
        else
        {

        }

        return true;
    }

    bool CMW500_RFVFS::GPS_InitBSE(int nTxNumb /* = ERFAC */,float fCellPower /* = -130 */,double dFreqDL /* = 1575.42 */)
    {
        //sprintf_s(szBuffer, MAX_PATH, "*OPC?", );

        std::string strCommand;
        if( fCellPower > 0) fCellPower = -fCellPower;

        JGW_FormatString(strCommand,":SOURce:GPRF:GEN:STATe OFF;"
            ":ROUTe:GPRF:GENerator:SCENario:SALone RF%s, TX1;"
            ":SOURce:GPRF:GENerator:RFSettings:FREQuency %.2f MHz;"
            ":SYSTem:BASE:REFerence:FREQuency:SOURce INT;"
            ":SOURce:GPRF:GENerator:BBMode CW;"
            ":SOURce:GPRF:GEN:LIST OFF;"
            ":SOUR:GPRF:GEN1:ARB:REP CONT;"
            ":TRIG:GPRF:GEN1:ARB:RETR ON;"
            ":TRIG:GPRF:GEN1:ARB:AUT ON;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel %.2f;"
            "*OPC?;:SYST:ERR?;"
            ,GetRFTxStrFlags(nTxNumb),dFreqDL,fCellPower
            );
        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        int i = 0;
        m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");
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

    bool CMW500_RFVFS::LTE_InitMeas()
    {
        std::string strCommand;

        strCommand = ":CONFigure:LTE:MEAS:MEValuation:RESult:ALL OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:LTE:MEASurement:MEValuation:REPetition SINGleshot;"
            ":CONFigure:LTE:MEASurement:MEValuation:MSUBframes 0, 10, 2;"
            ":CONFigure:LTE:MEASurement:MEValuation:SCON NONE;"
            ":CONFigure:LTE:MEASurement:MEValuation:MOEX OFF;"
            ":CONFigure:LTE:MEASurement:MEValuation:CBAN B100;"
            ":CONF:LTE:MEAS:MEV:CTYP AUTO;"
            ":CONF:LTE:MEAS:MEV:RBAL:AUTO ON;"
            ":CONFigure:LTE:MEASurement:RFSettings:ENPower 28;"
            ":TRIG:LTE:MEAS:MEV:SOUR 'GPRF Gen1: Waveform Marker 2';"
            ":CONF:LTE:MEAS:MEV:MOD:MSCH QPSK;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        strCommand = ":CONFigure:LTE:MEASurement:MEValuation:SCOunt:SPECtrum:ACLR 10;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:ACLR ON;"
            ":CONFigure:LTE:MEASurement:MEValuation:CBAN B100;"
            ":CONFigure:LTE:MEASurement:MEValuation:SCOunt:SPEctrum:SEMask 10;"
            ":CONFigure:LTE:MEASurement:MEValuation:SCOunt:MODulation 10;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:MERRor ON;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:SEMask ON;"
            ":CONFigure:LTE:MEASurement:MEValuation:LIMit:SEMask:LIMit3:CBANdwidth100 ON, 5e+006, 1e+007, -11.5, M1;"
            ":CONFigure:LTE:MEASurement:MEValuation:LIMit:SEMask:LIMit4:CBANdwidth100 ON, 1e+007, 1.5e+007, -23.5, M1;"
            ":CONFigure:LTE:MEASurement:MEValuation:LIMit:SEMask:LIMit1:CBANdwidth100 ON, 0, 1e+006, -16.5, K030;"
            ":CONFigure:LTE:MEASurement:MEValuation:LIMit:SEMask:LIMit2:CBANdwidth100 ON, 1e+006, 5e+006, -8.5, M1;"
            ":CONFigure:LTE:MEASurement:MEValuation:LIMit:SEMask:LIMit2:CBANdwidth100 ON, 1e+006, 5e+006, -8.5, M1;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:EVMagnitude ON;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:PERRor ON;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:TXM ON;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:ALL ON,ON,ON,ON,OFF,OFF,ON,ON,ON,ON,OFF,OFF,OFF,OFF;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    void CMW500_RFVFS::OpenMEValuation()
    {
        std::string strCommand;

        JGW_FormatString(strCommand,"STOP:%s:MEAS:MEV",g_szSysName[m_eSysName]);
        m_pEquipInter->write(strCommand);

        Sleep(500);

        JGW_FormatString(strCommand,"INIT:%s:MEAS:MEV",g_szSysName[m_eSysName]);
        m_pEquipInter->write(strCommand);
    }


    void CMW500_RFVFS::CloseMEValuation()
    {
        std::string strCommand;
        JGW_FormatString(strCommand,"ABORt:%s:MEAS:MEV",g_szSysName[m_eSysName]);
        m_pEquipInter->write(strCommand);
    }

    bool CMW500_RFVFS::CheckMeasIsComplete()
    {	
        std::string strResult;
        std::string strCommand;
        int i = 50;

        JGW_FormatString(strCommand,"FETCh:%s:MEAS:MEValuation:STATe:ALL?",g_szSysName[m_eSysName]);

        while( i-- > 0 )
        {
            m_pEquipInter->query(strCommand.c_str(),strResult);

            if ( std::string::npos != strResult.find("RDY") )
            {
                return true;
            }

            Sleep(200);
        }

        return false;
    }

    bool CMW500_RFVFS::LTE_NS_MesurmentAll(LTE_NS_MaxPwrTestValue& testValue)
    {
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        if( !LTE_InitMeas() )
        {
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();
            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:LTE:MEASurement:MEValuation:MODulation:AVERage?", strResult);  //maxpwr && wq
                _vfData.clear();
                Reliability = atoi(strResult.c_str());

                //if (0 != Reliability)
                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                testValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;

                testValue.evm		= _vfData[1];
                testValue.freqError = _vfData[14];
                testValue.maxPwr	= _vfData[16];
                //_vfData
                //////////////////////////////////////////////////////////////////////////
                m_pEquipInter->query("FETCh:LTE:MEASurement:MEValuation:ACLR:AVERage?", strResult); //aclr
                _vfData.clear();
                Reliability = atoi(strResult.c_str());

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),"," ,_vfData);
                testValue.ACLR[0] = _vfData[2];
                testValue.ACLR[1] = _vfData[4];
                testValue.ACLR[2] = _vfData[1];
                testValue.ACLR[3] = _vfData[5];
                testValue.ACLR[4] = _vfData[0];
                testValue.ACLR[5] = _vfData[6];

                //////////////////////////////////////////////////////////////////////////
                m_pEquipInter->query("FETCH:LTE:MEASurement:MEValuation:SEMask:AVERage?", strResult);//obw   sem
                _vfData.clear();
                Reliability = atoi(strResult.c_str());
                //if (0 != Reliability)
                //if (0 != Reliability)
                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",", _vfData);
                testValue.sem = ( int(_vfData[0]) == 0 ? 1:0);
                testValue.OBW = _vfData[1];
                break;
            }
        } 

        m_pEquipInter->write("STOP:LTE:MEASurement:MEValuation");
        CloseMEValuation();

        return true;
    }

    bool CMW500_RFVFS::LTE_NS_GetMinPower(float &fMinPower)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = ":CONFigure:LTE:MEASurement:RFSettings:ENPower -40;"
            ":CONFigure:LTE:MEAS:MEValuation:RESult:ALL OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:LTE:MEASurement:MEValuation:REPetition SINGleshot;"
            ":CONFigure:LTE:MEASurement:MEValuation:MSUBframes 0, 10, 2;"
            ":CONFigure:LTE:MEASurement:MEValuation:SCON NONE;"
            ":CONFigure:LTE:MEASurement:MEValuation:MOEX OFF;"
            ":CONFigure:LTE:MEASurement:MEValuation:CBAN B100;"
            ":CONF:LTE:MEAS:MEV:CTYP AUTO;"
            ":CONF:LTE:MEAS:MEV:RBAL:AUTO ON;"
            ":TRIG:LTE:MEAS:MEV:SOUR 'GPRF Gen1: Waveform Marker 4';"
            ":CONFigure:LTE:MEASurement:MEValuation:SCOunt:MODulation 30;"
            ":CONFigure:LTE:MEASurement:MEValuation:RESult:TXM ON;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:LTE:MEASurement:MEValuation:MODulation:AVERage?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                //testValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;
                fMinPower	= _vfData[16];
                break;
            }
        }

        strCommand = ":CONFigure:LTE:MEASurement:MEValuation:RESult:TXM OFF;"
            ":STOP:LTE:MEASurement:MEValuation;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        CloseMEValuation();

        return true;
    }

    //STOP:%s:MEAS:MEV
    bool CMW500_RFVFS::TDSCDMA_InitBSE()
    {
        std::string strCommand;

        strCommand = "SOURce:GPRF:GEN:STATe OFF;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult OFF;"
            ":STOP:TDSCDMA:MEAS:MEV;"
            ":CONFigure:TDSCdma:MEAS1:MEValuation:REPetition SING;"
            ":SOURce:GPRF:GENerator:BBMode ARB;"
            ":SOUR:GPRF:GEN:ARB:FILE \'D:\\Rohde-Schwarz\\CMW\\Data\\waveform\\TDSCDMA_onlyDTCH_PN9_TFCI3_MultiSegmentB.wv\';"
            ":TRIG:GPRF:GEN:ARB:SEGM:MODE CSE;"
            ":CONFigure:TDSCdma:MEAS:UESignal:MTYPe QPSK;"
            ":CONFigure:TDSCDMA:MEAS:RFSettings:UMARgin 5;"
            ":TRIGger:TDSCdma:MEAS:MEValuation:SOURce 'Free Run';"
            ":SOUR:GPRF:GEN1:ARB:REP CONT;"
            ":TRIG:GPRF:GEN1:ARB:RETR ON;"
            ":TRIG:GPRF:GEN1:ARB:AUT ON;"
            ":SOURce:GPRF:GENerator:RFSettings:FREQuency 2.0174e+009;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            ":CONFigure:TDSCDMA:MEAS:RFSettings:ENPower 0;"
            ":CONFigure:TDSCDMA:MEAS:UES:CDTH -20;"
            ":CONFigure:TDSCdma:MEAS:UESignal:CPARameterid 0;"
            ":CONFigure:TDSCdma:MEASurement:UESignal:FAUSlot SL2;"
            ":CONFigure:TDSCdma:MEASurement:MEValuation:PSLot 2;"
            ":CONFigure:TDSCdma:MEASurement:MEValuation:SSLot 2;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        if( !m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON") )
        {
            m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");
        }


        int i = 0;
        for( i = 0; i < 100; i++)
        {
            if( CheckGPRFGenStateIsOn() )
            {
                break;
            }

            Sleep(200);
        }

        if (i >= 100)
        {
            return false;
        }

        strCommand = "CONFigure:TDSCdma:MEASurement:UESignal:CDTHreshold -20;"
            ":CONFigure:TDSCdma:MEASurement:UESignal:SPOint TS2;"
            ":CONFigure:TDSCdma:MEASurement:UESignal:NUSers US16;"
            ":CONFigure:TDSCdma:MEASurement:UESignal:CIMode AUTO;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }
    //STOP:%s:MEAS:MEV

    bool CMW500_RFVFS::WCDMA_InitBSE()
    {
        //
        std::string strCommand;
        //	":CONFigure:WCDMA:MEAS:MEValuation:RESult OFF;"
        strCommand = "SOURce:GPRF:GEN:STATe OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:ALL OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":STOP:WCDMA:MEAS:MEV;"
            ":SOURce:GPRF:GEN:RFSettings:FREQuency 2140.0 MHz;"
            ":SOURce:GPRF:GENerator:BBMode ARB;"
            ":SOUR:GPRF:GEN:ARB:FILE \'D:\\Rohde-Schwarz\\CMW\\Data\\waveform\\RMC12_2_MultiSegment_0_9_A02.wv\';"
            ":SOUR:GPRF:GEN1:ARB:REP CONT;"
            ":TRIG:GPRF:GEN1:ARB:RETR ON;"
            ":TRIG:GPRF:GEN1:ARB:AUT ON;"
            ":TRIG:GPRF:GEN:ARB:SEGM:MODE CSE;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            ":SOURce:GPRF:GEN:ARB:SEGMents:NEXT 1;"
            ":TRIGger:GPRF:GEN:ARB:SEGMents:MANual:EXECute;"
            ":SOURce:GPRF:GEN:LIST OFF;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        if( !m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON") )
        {
            m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");
        }

        int i = 0;
        for( i = 0; i < 100; i++)
        {
            if( CheckGPRFGenStateIsOn() )
            {
                break;
            }

            Sleep(200);
        }

        if (i >= 100)
        {
            return false;
        }

        strCommand = "ABORT:WCDMA:MEASurement:MEValuation;"
            ":CONFigure:WCDMa:MEAS:BAND OB1;"
            ":CONFigure:WCDMA:MEAS:RFSettings:FREQuency 1950.0 MHz;"
            ":CONFigure:WCDMa:MEAS:MEValuation:LIMit:RCDerror:EECDp:DPCCh ON, 8, 64;"	
            ":CONFigure:WCDMa:MEAS:MEValuation:LIMit:RCDerror:EECDp:DPDCh ON, 15, 64;"
            ":CONFigure:WCDMa:MEAS:RFSettings:UMARgin 10;"
            ":CONFigure:WCDMa:MEAS:RFSettings:ENPower 24;"
            ":ABORt:WCDMa:MEAS:TPC;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:ALL OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:REPetition SINGleshot;"
            ":CONFigure:WCDMa:MEAS:MEValuation:TOUT 60;"
            ":CONFigure:WCDMa:MEAS:MEValuation:MSCount 1;"
            ":CONFigure:WCDMa:MEAS:MEValuation:SCOunt:MODulation 10;"
            ":CONFigure:WCDMa:MEAS:MEValuation:SCOunt:SPECtrum 10;"
            ":CONFigure:WCDMa:MEAS:MEValuation:PSLot 0;"
            "*OPC?;:SYST:ERR?;";
        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        strCommand = "CONFigure:WCDMa:MEAS:MEValuation:MOEX OFF;"
            ":CONFigure:WCDMa:MEAS:UESignal:SFORmat 0;"
            ":CONFigure:WCDMa:MEAS:UESignal:ULConfig WCDMa;"
            ":CONFigure:WCDMa:MEAS:UESignal:DPDCh ON;"
            ":TRIGger:WCDMa:MEAS:MEValuation:SOURce 'Free Run (Fast Sync)';"
            ":TRIGger:WCDMa:MEAS:MEValuation:MGAP 0;"
            ":CONFigure:WCDMa:MEAS:MEValuation:LIMit:RCDerror:EECDp:HSDPcch OFF,60,256;"
            ":CONFigure:WCDMa:MEAS:MEValuation:LIMit:RCDerror:EECDp:EDPCch OFF,30,256;"
            ":CONFigure:WCDMa:MEAS:MEValuation:LIMit:RCDerror:EECDp:EDPDch2 OFF,168,4;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::CDMA_InitBSE()
    {
        std::string strCommand;
        //		":CONFigure:CDMA:MEAS1:MEValuation:RESult OFF;"
        strCommand = "SOURce:GPRF:GEN:STATe OFF;"
            ":STOP:CDMA:MEAS:MEV;"
            ":SYSTem:BASE:REFerence:FREQuency:SOURce INT;"
            ":SOURce:GPRF:GEN:RFSettings:FREQuency 881.61 MHz;"
            ":SOURce:GPRF:GENerator:BBMode ARB;"
            ":SOUR:GPRF:GEN:ARB:FILE \'D:\\Rohde-Schwarz\\CMW\\Data\\waveform\\library1\\C2K_ch387_RC1_hold_upsens_down_upmaxinp.wv\';"
            ":SOUR:GPRF:GEN1:ARB:REP CONT;"
            ":TRIG:GPRF:GEN1:ARB:RETR ON;"
            ":TRIG:GPRF:GEN1:ARB:AUT ON;"
            ":TRIG:GPRF:GEN:ARB:SEGM:MODE CSE;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            ":SOURce:GPRF:GEN:ARB:SEGMents:NEXT 1;"
            ":TRIGger:GPRF:GEN:ARB:SEGMents:MANual:EXECute;"
            ":SOURce:GPRF:GEN:LIST OFF;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        int i = 0;
        m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");
        if( !m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON") )
        {
            m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");
        }

        for( i = 0; i < 100; i++)
        {
            if( CheckGPRFGenStateIsOn() )break;

            Sleep(200);
        }

        if (i >= 100)
        {
            return false;
        }

        strCommand = "CONFigure:CDMA:MEAS:RFSettings:BCLass USC;"
            ":CONFigure:CDMA:MEAS:RCONfig R12Q;"
            ":CONFigure:CDMA:MEAS:RFSettings:FREQuency 836.61 MHz;*OPC?;"
            ":CONFigure:CDMA:MEAS:RFSettings:UMARgin 7;*OPC?;"
            ":CONFigure:CDMA:MEAS:RFSettings:ENPower 30;*OPC?;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:ALL OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:CDMA:MEAS:MEValuation:REPetition SINGleshot;"
            ":CONFigure:CDMA:MEAS:MEValuation:TOUT 10;"
            ":CONFigure:CDMA:MEAS:MEValuation:SCONdition NONE;"
            ":CONFigure:CDMA:MEAS:MEValuation:MOEX OFF;"
            ":TRIGger:CDMA:MEAS:MEValuation:SLOPe REDGe;"
            ":TRIGger:CDMA:MEAS:MEValuation:THReshold -30;"
            ":TRIGger:CDMA:MEAS:MEValuation:TOUT 5000;"
            ":TRIGger:CDMA:MEAS:MEValuation:DELay 0;"
            ":TRIGger:CDMA:MEAS:MEValuation:MGAP 0;"
            ":TRIGger:CDMA:MEAS:MEValuation:SOURce 'GPRF Gen1: Waveform Marker 4';"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::GSM_InitBSE()
    {
        //	":CONFigure:GSM:MEAS:MEValuation:RESult OFF;"
        std::string strCommand;
        strCommand = "SOURce:GPRF:GEN:STATe OFF;"
            ":STOP:GSM:MEAS:MEV;"
            ":SYSTem:BASE:REFerence:FREQuency:SOURce INT;"
            ":SOURce:GPRF:GEN:RFSettings:FREQuency 925.2 MHz;"
            ":SOURce:GPRF:GENerator:BBMode ARB;"
            ":SOUR:GPRF:GEN:ARB:FILE \'D:\\Rohde-Schwarz\\CMW\\Data\\waveform\\library1\\loopC_1040.wv';"
            ":SOUR:GPRF:GEN1:ARB:REP CONT;"
            ":TRIG:GPRF:GEN1:ARB:RETR ON;"
            ":TRIG:GPRF:GEN1:ARB:AUT ON;"
            ":TRIG:GPRF:GEN:ARB:SEGM:MODE CSE;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            ":SOURce:GPRF:GEN:ARB:SEGMents:NEXT 1;"
            ":TRIGger:GPRF:GEN:ARB:SEGMents:MANual:EXECute;"
            ":SOURce:GPRF:GEN:LIST OFF;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        int i = 0;
        m_pEquipInter->write("SOURce:GPRF:GEN:STATe ON");
        for( i = 0; i < 100; i++)
        {
            if( CheckGPRFGenStateIsOn() )break;

            Sleep(200);
        }

        if (i >= 100)
        {
            return false;
        }

        strCommand = "CONFigure:GSM:MEAS:BAND G09;"
            ":CONFigure:GSM:MEAS:RFSettings:FREQuency 880.2 MHz;"
            ":CONFigure:GSM:MEAS:MEValuation:TSEQuence TSC0;"
            ":CONFigure:GSM:MEAS:MEValuation:PCLMode PCL;"
            ":CONFigure:GSM:MEAS:MEValuation:PCL 5,5,5,5,5,5,5,5;"
            ":CONFigure:GSM:MEAS:RFSettings:UMARgin 10;"
            ":CONFigure:GSM:MEAS:RFSettings:ENPower 33;"
            ":ABORt:GSM:MEAS:MEValuation;"
            ":CONFigure:GSM:MEAS:MEValuation:RESult:ALL OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:TOUT 10;"
            ":CONFigure:GSM:MEAS:MEValuation:REPetition SINGleshot;"
            ":CONFigure:GSM:MEAS:MEValuation:MOEXception OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:FCRange WIDE;"
            ":CONFigure:GSM:MEAS:MEValuation:HDALevel -50;"
            ":CONFigure:GSM:MEAS:MEValuation:NBQSearch OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:ABSearch OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:VAMos:TSCSet 1;"
            ":CONFigure:GSM:MEAS:MEValuation:LIST OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:RPMode AVERage;"
            ":CONFigure:GSM:MEAS:MEValuation:FILTer:PVTime G05M;"
            ":CONFigure:GSM:MEAS:MEValuation:MODulation:DECode GTBits;"
            ":CONFigure:GSM:MEAS:MEValuation:MVIew GMSK,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:MSLots 3,1,3;"
            ":TRIGger:GSM:MEAS:MEValuation:SOURce 'GPRF Gen1: Waveform Marker 3';"
            ":TRIGger:GSM:MEAS:MEValuation:SLOPe REDGe;"
            ":TRIGger:GSM:MEAS:MEValuation:THReshold -20;"
            ":TRIGger:GSM:MEAS:MEValuation:TOUT 5;"
            ":TRIGger:GSM:MEAS:MEValuation:MGAP 0;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::TDSCDMA_NS_GetMinPower(double &fMinPower)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = "SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            ":CONFigure:TDSCDMA:MEAS:RFSettings:ENPower -38.183;"
            ":CONFigure:TDSCDMA:MEAS:UES:CDTH 0;"
            ":TRIGger:TDSCdma:MEAS:MEValuation:SOURce \'GPRF Gen1: Waveform Marker 4\';"
            ":SOURce:GPRF:GENerator:ARB:SEGM:NEXT 1;"
            ":TRIG:GPRF:GEN:ARB:SEGM:MAN:EXEC;"
            ":CONFigure:TDSCdma:MEAS1:MEValuation:SCOunt:MODulation 5;"
            ":CONFigure:TDSCdma:MEAS1:MEValuation:RESult:MSCalar ON;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:TDSCdma:MEAS:MEValuation:SPECtrum:AVERage?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                fMinPower	= _vfData[12];
                break;
            }
        }

        strCommand = "CONFigure:TDSCdma:MEAS1:MEValuation:RESult:MSCalar OFF;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }
        CloseMEValuation();
        return true;
    }

    bool CMW500_RFVFS::TDSCDMA_NS_MesurmentAll(TDS_NS_MaxPwrTestValue &TDtestValue)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = "SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:ALL OFF,OFF, OFF, OFF, OFF,OFF, OFF, OFF, OFF,OFF, OFF;"
            ":CONFigure:TDSCdma:MEAS:RFSettings:ENPower 28;"
            ":CONFigure:TDSCdma:MEAS:RFSettings:UMARgin 10;"
            ":CONFigure:TDSCDMA:MEAS:UES:CDTH -20;"
            ":TRIGger:TDSCdma:MEAS:MEValuation:SOURce \'GPRF Gen1: Waveform Marker 4\';"
            ":SOURce:GPRF:GENerator:ARB:SEGM:NEXT 1;"
            ":TRIG:GPRF:GEN:ARB:SEGM:MAN:EXEC;"
            ":CONFigure:TDSCdma:MEAS1:MEValuation:SCOunt:MODulation 5;"
            ":CONFigure:TDSCdma:MEAS1:MEValuation:RESult:MSCalar ON;"
            ":CONFigure:TDSCdma:MEAS1:MEValuation:RESult:MSCalar ON;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:ACLR ON;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:EMASk ON;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:SCOunt:SPECtrum 5;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:EVMagnitude ON;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:SCOunt:MODulation 5;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:LIMit:PVTime:YABSolute -70,-60,-70;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:LIMit:PVTime:XRELative -55,-33,-13,12,45;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:LIMit:PVTime:EAReas ON,ON,ON;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:PVT ON;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:HDALevel -60;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:HDALevel ON;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:SCOunt:PVT 5;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:TDSCdma:MEAS:MEValuation:SPECtrum:MAXimum?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();
                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                //TDtestValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;
                TDtestValue.ACLR[0] = _vfData[0]-_vfData[1];
                TDtestValue.ACLR[1] = _vfData[0]-_vfData[2];
                TDtestValue.ACLR[2] = _vfData[0]-_vfData[3];
                TDtestValue.ACLR[3] = _vfData[0]-_vfData[4];

                TDtestValue.OBW	  = _vfData[5];

                TDtestValue.sem[0]  = _vfData[6];
                TDtestValue.sem[1]  = _vfData[7];
                TDtestValue.sem[2]  = _vfData[8];
                TDtestValue.sem[3]  = _vfData[9];
                TDtestValue.sem[4]  = _vfData[10];
                TDtestValue.sem[5]  = _vfData[11];

                TDtestValue.maxPwr  = _vfData[12];

                m_pEquipInter->query("FETCh:TDSCdma:MEAS:MEValuation:MODulation:AVERage?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();
                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                TDtestValue.evm				= _vfData[0];
                TDtestValue.PhaseError		= _vfData[4];
                TDtestValue.IQoffset		= _vfData[6];
                TDtestValue.IQbalance		= _vfData[7];
                TDtestValue.freqError		= _vfData[9];
                TDtestValue.PCDE			= _vfData[12];
                TDtestValue.pvt				= 1;
                TDtestValue.indicator		= 1;
                break;
            }
        }

        strCommand = "CONFigure:TDSCdma:MEAS:MEValuation:RESult:ACLR OFF;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:EMASk OFF;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:EVMagnitude OFF;"
            ":CONFigure:TDSCdma:MEAS1:MEValuation:RESult:MSCalar OFF;"
            ":CONFigure:TDSCdma:MEAS:MEValuation:RESult:PVT OFF;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }
        CloseMEValuation();

        return true;
    }

    bool CMW500_RFVFS::WCDMA_NS_MinPwr(float& fMinPower)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = "SOURce:GPRF:GEN:RFSettings:LEVel -85;"
            ":CONFigure:WCDMa:MEAS:RFSettings:ENPower -56;"
            ":CONFigure:WCDMa:MEAS:RFSettings:UMARgin 10;"
            ":SOURce:GPRF:GEN:ARB:SEGMents:NEXT 2;"
            ":TRIG:GPRF:GEN:ARB:SEGM:MAN:EXEC;"
            ":TRIGger:WCDMa:MEAS:MEValuation:MGAP 0;"
            ":TRIGger:WCDMa:MEAS:MEValuation:SOURce 'Free Run (Fast Sync)';"
            ":SOURce:GPRF:GENerator:ARB:SEGM:NEXT 1;"
            ":CONFigure:WCDMa:MEAS:MEValuation:SCOunt:MODulation 5;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:UEPower ON;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:WCDMa:MEAS:MEValuation:TRACe:UEPower:AVERage?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                //testValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;
                fMinPower	= _vfData[0];
                break;
            }
        }

        strCommand = "CONFigure:WCDMa:MEAS:MEValuation:RESult:UEPower OFF;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }
        CloseMEValuation();
        return true;
    }

    bool CMW500_RFVFS::WCDMA_NS_MesurmentAll (W_NS_MaxPwrTestValue &testValue)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            ":TRIG:GPRF:GEN:ARB:SEGM:MAN:EXEC;"
            ":TRIGger:WCDMa:MEAS:MEValuation:MGAP 0;"
            ":TRIGger:WCDMa:MEAS:MEValuation:SOURce 'Free Run (Fast Sync)';"
            ":SOURce:GPRF:GENerator:ARB:SEGM:NEXT 1;"
            ":CONFigure:WCDMa:MEAS:MEValuation:SCOunt:MODulation 5;"
            ":CONFigure:WCDMa:MEAS:RFSettings:ENPower 24;"
            ":CONFigure:WCDMa:MEAS:RFSettings:UMARgin 10;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:ACLR ON;"
            ":CONFigure:WCDMa:MEAS:MEValuation:SCOunt:SPECtrum 5;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:EMASk ON;"
            ":TRIGger:WCDMa:MEAS:MEValuation:SOUR 'GPRF Gen1: Waveform Marker 3';"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:EVMagnitude ON;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:PERRor ON;"
            ":CONFigure:WCDMa:MEAS:MEValuation:SCOunt:MODulation 5;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:MERRor ON;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:CDPower ON;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:CDPMonitor ON;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:FERRor ON;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:UEPower ON;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:WCDMa:MEAS:MEValuation:SPECtrum:MAXimum?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        Sleep(1000);
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                //testValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;
                testValue.ACLR[0] = _vfData[0]-_vfData[1];
                testValue.ACLR[1] = _vfData[0]-_vfData[2];
                testValue.ACLR[2] = _vfData[0]-_vfData[3];
                testValue.ACLR[3] = _vfData[0]-_vfData[4];

                testValue.OBW	  = _vfData[5];

                testValue.sem[0]  = _vfData[6];
                testValue.sem[1]  = _vfData[7];
                testValue.sem[2]  = _vfData[8];
                testValue.sem[3]  = _vfData[9];
                testValue.sem[4]  = _vfData[10];
                testValue.sem[5]  = _vfData[11];
                testValue.sem[6]  = _vfData[12];
                testValue.sem[7]  = _vfData[13];

                testValue.maxPwr  = _vfData[14];


                m_pEquipInter->query("FETCh:WCDMa:MEAS:MEValuation:MODulation:MAXimum?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                testValue.evm			= _vfData[0];
                testValue.PhaseError	= _vfData[4];
                testValue.IQoffset		= _vfData[6];
                testValue.IQbalance		= _vfData[7];
                testValue.freqError		= _vfData[8];

                m_pEquipInter->query("FETCh:WCDMa:MEAS:MEValuation:PCDE:MAXimum?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                testValue.PCDE = _vfData[0];
                testValue.indicator = 1;
                break;
            }
        }

        strCommand = "CONFigure:WCDMa:MEAS:MEValuation:RESult:ACLR OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:EMASk OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:EVMagnitude OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:MERRor OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:PERRor OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:CDPower OFF;"
            ":TRIGger:WCDMa:MEAS:MEValuation:SOURce 'Free Run (Fast Sync)';"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:EVMagnitude OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:PERRor OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:CDPower OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:CDPMonitor OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:FERRor OFF;"
            ":CONFigure:WCDMa:MEAS:MEValuation:RESult:UEPower OFF;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }
        CloseMEValuation();

        return true;
    }

    bool CMW500_RFVFS::CDMA_NS_MinPwr(float& fMinPower)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = "SOURce:GPRF:GENerator:RFSettings:LEVel -30;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:POWer ON;"
            ":CONFigure:CDMA:MEAS:MEValuation:SCOunt:MODulation 4;"
            ":TRIGger:CDMA:MEAS:MEValuation:SOURce \'CDMA2000 Sig1: Super Frame (80.00 ms)\';"
            ":CONFigure:CDMA:MEAS:MEValuation:MOEXception OFF;"
            ":CONFigure:CDMA:MEAS:MEValuation:REPetition SINGleshot;"
            ":CONFigure:CDMA:MEAS:RCONfig R12Q;"
            ":SOURce:GPRF:GEN:ARB:SEGMents:NEXT 2;"
            ":TRIGger:GPRF:GEN:ARB:SEGMents:MANual:EXECute;"
            ":CONFigure:CDMA:MEAS:RFSettings:UMARgin 7;"
            ":CONFigure:CDMA:MEAS:RFSettings:ENPower -48;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:POWer ON;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:CDMA:MEAS:MEValuation:MODulation:AVERage?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                //testValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;
                fMinPower	= _vfData[10];
                break;
            }
        }

        strCommand ="CONFigure:CDMA:MEAS:MEValuation:RESult:POWer OFF;"
            ":ABORT:CDMA:MEAS:MEValuation;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;	
    }

    bool CMW500_RFVFS::CDMA_NS_ACPR( int &OutOfTolerance )
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = "SOURce:GPRF:GEN:RFSettings:LEVel -101;"
            ":CONFigure:CDMA:MEAS:RFSettings:ENPower 24;"
            ":CONFigure:CDMA:MEAS:RFSettings:UMARgin 10;"
            ":TRIGger:CDMA:MEAS:MEValuation:SOURce \'CDMA2000 Sig1: Super Frame (80.00 ms)\';"
            ":CONFigure:CDMA:MEAS:MEValuation:MOEXception OFF;"
            ":CONFigure:CDMA:MEAS:MEValuation:REPetition SINGleshot;"
            ":CONFigure:CDMA:MEAS:RCONfig R12Q;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:ACP ON;"
            ":CONFigure:CDMA:MEAS:MEValuation:SCOunt:SPECtrum 5;"
            ":CONFigure:CDMA:MEAS:MEValuation:ACP:FOFFsets 0.885,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,1.98;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:CDMA:MEAS:MEValuation:ACP?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                //testValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;
                OutOfTolerance = _vfData[0]==1?1:0;
                break;
            }
        }

        strCommand ="CONFigure:CDMA:MEAS:MEValuation:RESult:ACP OFF;"
            ":ABORT:CDMA:MEAS:MEValuation;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::CDMA_2000_NS_MesurmentAll( C_NS_MaxPwrTestValue &testValue )
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        strCommand = "SOURce:GPRF:GEN:RFSettings:LEVel -75;"
            ":CONFigure:CDMA:MEAS:RFSettings:ENPower 28;"
            ":CONFigure:CDMA:MEAS:RFSettings:UMARgin 10;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:EVMagnitude ON;"
            ":TRIGger:CDMA:MEAS:MEValuation:SOURce \'CDMA2000 Sig1: Super Frame (80.00 ms)\';"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:MERRor ON;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:PERRor ON;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:MODQuality ON;"
            ":CONFigure:CDMA:MEAS:MEValuation:SCOunt:MODulation 5;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:POWer ON;"
            ":CONFigure:CDMA:MEAS:MEValuation:MOEXception OFF;"
            ":CONFigure:CDMA:MEAS:MEValuation:REPetition SINGleshot;"
            ":CONFigure:CDMA:MEAS:RCONfig R12Q;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:CDMA:MEAS:MEValuation:MODulation:AVERage?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability && E_MEAS_Underdriven != Reliability)
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    if  (E_MEAS_OVERDRIVER == Reliability || E_MEAS_SYNC_ERROR == Reliability)
                    {
                        continue;
                    }
                    return false;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                //testValue.indicator = ( int(_vfData[0]) == 0 ? 1:0) ;
                testValue.evm = _vfData[0];
                testValue.freqError = _vfData[8];
                testValue.maxPwr = _vfData[10];
                testValue.wfQuality = _vfData[12];
                testValue.PhaseError = _vfData[4];
                testValue.IQbalance = _vfData[7];
                testValue.indicator = 1;
                break;
            }
        }

        strCommand = "CONFigure:CDMA:MEAS:MEValuation:RESult:EVMagnitude OFF;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:MERRor OFF;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:PERRor OFF;"
            ":CONFigure:CDMA:MEAS:MEValuation:RESult:MODQuality OFF;"
            ":ABORT:CDMA:MEAS:MEValuation;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }

    bool CMW500_RFVFS::GSM_MesurmentBer(float nCellPower,int nBerCount,float &fBer)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::string strResult;

        JGW_FormatString( strCommand ,"CONFigure:GSM:MEAS:MEValuation:RESult:BER ON;"
            ":CONFigure:GSM:MEAS:MEValuation:BER:LOOP C;"
            ":CONFigure:GSM:MEAS:MEValuation:BER:TST 30;"
            ":CONFigure:GSM:MEAS:MEValuation:BER:TRUN 30;"
            ":CONFigure:GSM:MEAS:MEValuation:SCOunt:BER %d;"
            ":SOURce:GPRF:GENerator:RFSettings:LEVel %.2f;"
            "*OPC?;:SYST:ERR?;",nBerCount,nCellPower );

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        for (int i = 0; i < INSTRUMENT_TEST_COUNTS; i++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:GSM:MEAS:MEValuation:BER?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();

                if (E_MEAS_OK != Reliability )
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)
                    continue;
                }

                strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);

                if ( 0 != Reliability )
                {
                    fBer = 100;
                    continue;
                }
                else
                {
                    fBer = _vfData[0];
                    break;
                }
            }
        }

        strCommand = "CONFigure:GSM:MEAS:MEValuation:RESult:BER OFF;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -65;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }


    bool CMW500_RFVFS::GSM_MesurmentAll(int nPowerLevel,float nCellBerPower,GSM_NS_TestValue &sGsm_TestValue)
    {
        std::string strCommand;
        int Reliability = 0;
        std::vector<float> _vfData;
        std::vector<int> _vIntData;
        std::string strResult;
        bool bResult = true;

        JGW_FormatString(strCommand ,"CONFigure:GSM:MEAS:MEValuation:MVIew GMSK,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:PCLMode PCL;"
            ":CONFigure:GSM:MEAS:MEValuation:PCL %d,%d,%d,%d,%d,%d,%d,%d;"
            ":CONFigure:GSM:MEAS:RFSettings:UMARgin 10;"
            "*OPC?;:SYST:ERR?;",nPowerLevel,nPowerLevel,nPowerLevel,nPowerLevel,nPowerLevel,nPowerLevel,nPowerLevel,nPowerLevel
            );

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        if (nPowerLevel <= 5)
        {
            bResult &= m_pEquipInter->write("CONFigure:GSM:MEAS:RFSettings:ENPower 33;*OPC?");
        }
        else if (nPowerLevel <= 11)
        {
            bResult &= m_pEquipInter->write("CONFigure:GSM:MEAS:RFSettings:ENPower 21;*OPC?");
        }
        else if (nPowerLevel <= 19)
        {
            bResult &= m_pEquipInter->write("CONFigure:GSM:MEAS:RFSettings:ENPower 5;*OPC?");
        }
        else
        {
            bResult &= m_pEquipInter->write("CONFigure:GSM:MEAS:RFSettings:ENPower 10;*OPC?");
        }

        strCommand = "CONFigure:GSM:MEAS:MEValuation:RESult:PERRor ON;"
            ":CONFigure:GSM:MEAS:MEValuation:RESult:EVM ON;"
            ":CONFigure:GSM:MEAS:MEValuation:RESult:MERRor ON;"
            ":CONFigure:GSM:MEAS:MEValuation:SCOunt:MODulation 30;"
            ":CONFigure:GSM:MEAS:MEValuation:RESult:PVTime ON;"
            ":CONFigure:GSM:MEAS:MEValuation:SCOunt:PVTime 30;"
            ":CONFigure:GSM:MEAS:MEValuation:RESult:MSCalar ON;"
            ":CONFigure:GSM:MEAS:MEValuation:SCOunt:MODulation 30;"
            ":CONFigure:GSM:MEAS:MEValuation:RESult:SSFRequency ON;"
            ":CONFigure:GSM:MEAS:MEValuation:SCOunt:SSWitching 30;"
            ":CONFigure:GSM:MEAS:MEValuation:SMODulation:OFRequence 0.4e+6,0.6e+6,1.2e+6,1.8e+6,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            ":CONFigure:GSM:MEAS:MEValuation:RESult:SMFRequency ON;"
            ":CONFigure:GSM:MEAS:MEValuation:SCOunt:SMODulation 30;"
            ":CONFigure:GSM:MEAS:MEValuation:SMODulation:OFRequence 0.1e+6,0.2e+6,0.25e+6,0.4e+6,0.6e+6,0.8e+6,1e+6,1.2e+6,1.4e+6,1.6e+6,1.8e+6,2e+6,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF;"
            "*OPC?;:SYST:ERR?;";

        //std::string strResult;
        for (int i = 0;i < 3;i ++)
        {
            Sleep(300);
            m_pEquipInter->write(strCommand);
            m_pEquipInter->read(strResult);

            if ( '1' != strResult[0] || '0' !=  strResult[2] )
            {
                Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strResult).c_str(), _T(__FUNCTION__), __LINE__ );
                //return false;
            }
            else
            {
                break;
            }
        }


        if ( '1' != strResult[0] || '0' !=  strResult[2] )
        {
            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strResult).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        bool bReadModOk = false;
        bool bReadPERCOk = false;
        bool bReadPVTOk = false;
        bool bReadSMODOk = false;
        bool bReadSSWOk = false;

        for (int nIndex = 0; nIndex < INSTRUMENT_TEST_COUNTS; nIndex++ )
        {
            OpenMEValuation();

            if ( CheckMeasIsComplete() )
            {
                m_pEquipInter->query("FETCh:GSM:MEAS:MEV:MOD:AVERage?", strResult);  //maxpwr && wq
                Reliability = atoi(strResult.c_str());
                _vfData.clear();
                //if (0 != Reliability)
                if (E_MEAS_OK != Reliability )
                {
                    //if  ( 4==Reliability || 3==Reliability ||  8==Reliability)

                }

                //strResult.erase(0,strResult.find(",") + 1);
                JGW_ParserFloatA(strResult.c_str(),",",_vfData);


                if ( (E_MEAS_OK != Reliability && nIndex < 2) || _vfData.size() < 13)
                {
                    continue;
                }
                else
                {
                    if ( !bReadModOk )
                    {
                        m_pEquipInter->write("CONFigure:GSM:MEAS:MEValuation:RESult:PERRor OFF");
                        m_pEquipInter->write("CONFigure:GSM:MEAS:MEValuation:RESult:EVM OFF");
                        m_pEquipInter->write("CONFigure:GSM:MEAS:MEValuation:RESult:MERRor OFF");

                        sGsm_TestValue.evm = _vfData[2];
                        sGsm_TestValue.fPhaseErrorRMS = _vfData[6];
                        sGsm_TestValue.fPhaseErrorPeak = _vfData[7];
                        sGsm_TestValue.freqError = _vfData[10];
                        sGsm_TestValue.fPowerlevel = _vfData[12];
                        bReadModOk = true;
                        nIndex = 0;
                    }
                }

                m_pEquipInter->query("FETCh:GSM:MEAS:MEValuation:PVTime:ALL?",strResult);
                Reliability = atoi(strResult.c_str());
                _vIntData.clear();
                JGW_ParserIntA(strResult.c_str(),",",_vIntData);

                if ( (E_MEAS_OK != Reliability && _vIntData[1] != 0.00 && nIndex < 2) || _vIntData.size() < 2)
                {
                    Sleep(200);
                    continue;
                }
                else
                {
                    if ( !bReadPVTOk )
                    {
                        m_pEquipInter->write("CONFigure:GSM:MEAS:MEValuation:RESult:PVTime OFF");
                        m_pEquipInter->write("CONFigure:GSM:MEAS:MEValuation:RESult:MSCalar OFF");
                        sGsm_TestValue.bPVtime = _vIntData[1] == 0.00?true:false;
                        bReadPVTOk = true;
                        nIndex = 0;
                    }
                }

                m_pEquipInter->query("FETCh:GSM:MEAS:MEValuation:SMODulation?",strResult);
                Reliability = atoi(strResult.c_str());
                _vIntData.clear();
                JGW_ParserIntA(strResult.c_str(),",",_vIntData);

                if ( (E_MEAS_OK != Reliability && nIndex < 2) || _vIntData.size() < 2 )
                {
                    continue;
                }
                else
                {
                    if ( !bReadSMODOk )
                    {	
                        m_pEquipInter->write("CONFigure:GSM:MEAS:MEValuation:RESult:SMFRequency OFF");
                        sGsm_TestValue.bSpectrumModulation = _vIntData[1] == 0.00?true:false;
                        nIndex = 0;
                        bReadSMODOk = true;
                    }
                }


                m_pEquipInter->query("FETCh:GSM:MEAS:MEValuation:SSWitching?",strResult);
                Reliability = atoi(strResult.c_str());
                _vIntData.clear();
                JGW_ParserIntA(strResult.c_str(),",",_vIntData);

                if ( (E_MEAS_OK != Reliability && nIndex < 2 && _vIntData[1] != 0.00) || _vIntData.size() < 2 )
                {
                    continue;
                }
                else
                {
                    if ( !bReadSSWOk )
                    {
                        m_pEquipInter->write("CONFigure:GSM:MEAS:MEValuation:RESult:SSFRequency OFF");

                        sGsm_TestValue.bSpectrumSwitching = _vIntData[1] == 0.00?true:false;
                        nIndex = 0;
                        bReadSSWOk = true;
                    }
                }

                break;
            }
        }

        strCommand = "CONFigure:GSM:MEAS:MEValuation:RESult:BER OFF;"
            ":SOURce:GPRF:GEN:RFSettings:LEVel -60;"
            "*OPC?;:SYST:ERR?;";

        m_pEquipInter->write(strCommand);
        m_pEquipInter->read(strCommand);

        if ( '1' != strCommand[0] || '0' !=  strCommand[2] )
        {

            Log4WE_F(L"Error [%s],%s, %d ", JGW_A2W(strCommand).c_str(), _T(__FUNCTION__), __LINE__ );
            return false;
        }

        return true;
    }
}